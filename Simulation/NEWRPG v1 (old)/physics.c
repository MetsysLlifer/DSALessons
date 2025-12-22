#include "game.h"

void UpdateEntityAI(Entity* e, Entity* entities, int count, Vector2 targetPos) {
    if (!e->isActive || !e->isSpell) return;
    
    // Auto-Targeting for Offensive AI
    int bestTarget = -1;
    float closest = 10000.0f;
    
    // If it's Homing/Predict/Swarm, find a "bad guy" or "wall" to hit
    if (e->spellData.aiType == AI_HOMING || e->spellData.aiType == AI_PREDICT || e->spellData.aiType == AI_SWARM) {
        for(int i=0; i<count; i++) {
            if(&entities[i] == e || !entities[i].isActive) continue;
            // Target things that are NOT projectiles (Player or Static Walls/Items)
            if(entities[i].state == STATE_STATIC_WALL || entities[i].state == STATE_RAW) {
                float d = Vector2Distance(e->position, entities[i].position);
                if(d < closest) { closest = d; bestTarget = i; }
            }
        }
        if(bestTarget != -1) targetPos = entities[bestTarget].position;
    }

    if (e->spellData.aiType == AI_HOMING) {
        Vector2 toTarget = Vector2Subtract(targetPos, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toTarget), 25.0f));
    }
    else if (e->spellData.aiType == AI_PREDICT && bestTarget != -1) {
        Vector2 predicted = Vector2Add(targetPos, Vector2Scale(entities[bestTarget].velocity, 0.5f));
        Vector2 toTarget = Vector2Subtract(predicted, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toTarget), 40.0f));
    }
    else if (e->spellData.aiType == AI_ORBIT) {
        float time = GetTime();
        Vector2 orbitPoint = { targetPos.x + cosf(time*3.0f)*100.0f, targetPos.y + sinf(time*3.0f)*100.0f };
        Vector2 toOrbit = Vector2Subtract(orbitPoint, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toOrbit), 50.0f));
    }
    else if (e->spellData.aiType == AI_SWARM) {
        // Cohesion
        Vector2 swarmCenter = targetPos;
        Vector2 toCenter = Vector2Subtract(swarmCenter, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toCenter), 15.0f));
    }
    else if (e->spellData.aiType == AI_ERRATIC) {
        Vector2 jitter = { (float)GetRandomValue(-10,10), (float)GetRandomValue(-10,10) };
        e->velocity = Vector2Add(e->velocity, jitter);
    }
}

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount) {
    if (e->isHeld || e->state == STATE_STATIC_WALL) return; 
    float dt = GetFrameTime(); e->lifeTime += dt * 5.0f;

    // Forces
    if (Vector2Length(inputDirection) > 0) {
        inputDirection = Vector2Normalize(inputDirection);
        Vector2 desiredVelocity = Vector2Scale(inputDirection, e->maxSpeed);
        Vector2 steering = Vector2Subtract(desiredVelocity, e->velocity);
        if (Vector2Length(steering) > e->moveForce) {
            steering = Vector2Normalize(steering); steering = Vector2Scale(steering, e->moveForce);
        }
        Vector2 acceleration = Vector2Scale(steering, 1.0f / e->mass);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(acceleration, dt));
    } else {
        Vector2 frictionDir = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
        Vector2 frictionForce = Vector2Scale(frictionDir, e->friction * e->mass);
        if (Vector2Length(e->velocity) < 10.0f) e->velocity = (Vector2){0,0};
        else e->velocity = Vector2Add(e->velocity, Vector2Scale(frictionForce, dt));
    }

    // --- GRANULAR WALL COLLISION ---
    // Check every chunk against walls
    int chunkCount = GetSpellChunkCount(e);
    
    // Move X
    e->position.x += e->velocity.x * dt;
    for(int k=0; k<chunkCount; k++) {
        Vector2 cPos = GetSpellChunkPos(e, k, GetTime());
        float cSize = GetSpellChunkSize(e, k);
        Rectangle hitBox = { cPos.x - cSize, cPos.y - cSize, cSize*2, cSize*2 };
        
        if (e->spellData.behavior != SPELL_PHANTOM) {
            for (int w = 0; w < wallCount; w++) {
                if (CheckCollisionRecs(hitBox, walls[w])) { 
                    e->position.x -= e->velocity.x * dt; // Revert Entity
                    e->velocity.x = 0; 
                    goto checkedX; // Break out of loops for this axis
                }
            }
        }
    }
    checkedX:;

    // Move Y
    e->position.y += e->velocity.y * dt;
    for(int k=0; k<chunkCount; k++) {
        Vector2 cPos = GetSpellChunkPos(e, k, GetTime());
        float cSize = GetSpellChunkSize(e, k);
        Rectangle hitBox = { cPos.x - cSize, cPos.y - cSize, cSize*2, cSize*2 };
        
        if (e->spellData.behavior != SPELL_PHANTOM) {
            for (int w = 0; w < wallCount; w++) {
                if (CheckCollisionRecs(hitBox, walls[w])) { 
                    e->position.y -= e->velocity.y * dt; 
                    e->velocity.y = 0; 
                    goto checkedY;
                }
            }
        }
    }
    checkedY:;
    
    // Bounds
    if (e->position.x < 0) { e->position.x = 0; e->velocity.x *= -1; }
    if (e->position.x > SCREEN_WIDTH) { e->position.x = SCREEN_WIDTH; e->velocity.x *= -1; }
    if (e->position.y < 0) { e->position.y = 0; e->velocity.y *= -1; }
    if (e->position.y > SCREEN_HEIGHT) { e->position.y = SCREEN_HEIGHT; e->velocity.y *= -1; }
}

void ApplySpellFieldEffects(Entity* entities, int count, ParticleSystem* ps) {
    for(int i=0; i<count; i++) {
        if(!entities[i].isActive || entities[i].state != STATE_PROJECTILE) continue;
        
        // VOID WELL / MAGNET / GRAVITY
        if(entities[i].spellData.behavior == SPELL_VOID) {
            for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive || entities[j].state == STATE_STATIC_WALL) continue;
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 300.0f) {
                    Vector2 pull = Vector2Subtract(entities[i].position, entities[j].position);
                    entities[j].velocity = Vector2Add(entities[j].velocity, Vector2Scale(Vector2Normalize(pull), 20.0f));
                }
            }
        }
        if(entities[i].spellData.behavior == SPELL_MAGNET) {
            for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive) continue;
                if(entities[j].spellData.core == ELEM_EARTH || entities[j].state == STATE_STATIC_WALL) {
                    float dist = Vector2Distance(entities[i].position, entities[j].position);
                    if(dist < 200.0f) {
                        Vector2 pull = Vector2Subtract(entities[i].position, entities[j].position);
                        entities[j].velocity = Vector2Add(entities[j].velocity, Vector2Scale(Vector2Normalize(pull), 30.0f));
                    }
                }
            }
        }
        if(entities[i].spellData.behavior == SPELL_TELEKINESIS) {
             for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive) continue;
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 200.0f) {
                    // STOP MOVING (Gravity Crush)
                    entities[j].velocity = Vector2Scale(entities[j].velocity, 0.8f);
                }
            }
        }
    }
}

// --- GRANULAR ENTITY COLLISION ---
void ResolveEntityCollisions(Entity* entities, int count, Entity* player, ParticleSystem* ps) {
    for (int i = 0; i < count; i++) {
        if (!entities[i].isActive) continue;

        if (entities[i].state == STATE_PROJECTILE && entities[i].spellData.behavior == SPELL_WALL) {
             if (Vector2Length(entities[i].velocity) < 20.0f) { entities[i].state = STATE_STATIC_WALL; entities[i].mass = 5000.0f; entities[i].color = BROWN; }
        }
        if (entities[i].state == STATE_PROJECTILE && entities[i].spellData.behavior == SPELL_LANDMINE) {
             if (Vector2Length(entities[i].velocity) < 10.0f) entities[i].color.a = 50; 
        }

        for (int j = i + 1; j < count; j++) {
            if (!entities[j].isActive) continue;
            
            // MULTI-POINT CHECK
            bool collision = false;
            int chunksI = GetSpellChunkCount(&entities[i]);
            int chunksJ = GetSpellChunkCount(&entities[j]);
            
            for(int ki=0; ki<chunksI; ki++) {
                for(int kj=0; kj<chunksJ; kj++) {
                    Vector2 pI = GetSpellChunkPos(&entities[i], ki, GetTime());
                    float sI = GetSpellChunkSize(&entities[i], ki);
                    Vector2 pJ = GetSpellChunkPos(&entities[j], kj, GetTime());
                    float sJ = GetSpellChunkSize(&entities[j], kj);
                    
                    if (CheckCollisionCircles(pI, sI, pJ, sJ)) {
                        collision = true;
                        goto hit;
                    }
                }
            }
            hit:;

            if (collision) {
                Entity* proj = (entities[i].state == STATE_PROJECTILE) ? &entities[i] : (entities[j].state == STATE_PROJECTILE ? &entities[j] : NULL);
                Entity* target = (proj == &entities[i]) ? &entities[j] : &entities[i];
                
                if (proj && target->state != STATE_PROJECTILE) {
                    if (proj->spellData.behavior == SPELL_MIDAS) {
                        target->state = STATE_STATIC_WALL; target->color = GOLD; target->mass = 1000.0f; target->velocity = (Vector2){0,0};
                        SpawnExplosion(ps, target->position, GOLD); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_PETRIFY) {
                        target->state = STATE_STATIC_WALL; target->color = GRAY; target->mass = 1000.0f; target->velocity = (Vector2){0,0};
                        SpawnExplosion(ps, target->position, GRAY); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_FREEZE) {
                        target->velocity = (Vector2){0,0}; target->color = SKYBLUE; 
                        SpawnExplosion(ps, target->position, SKYBLUE); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_SHRINK) {
                        target->size *= 0.5f; SpawnExplosion(ps, target->position, PURPLE); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_GROWTH) {
                        target->size *= 1.5f; SpawnExplosion(ps, target->position, GREEN); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_CLUSTER) {
                        SpawnExplosion(ps, proj->position, ORANGE); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_CHAIN_LIGHTNING) {
                         SpawnExplosion(ps, target->position, YELLOW); proj->isActive = false; continue;
                    }
                    if (proj->spellData.behavior == SPELL_SLOW) {
                        target->velocity = Vector2Scale(target->velocity, 0.1f); target->color = Fade(target->color, 0.5f);
                        SpawnExplosion(ps, target->position, BROWN); proj->isActive = false; continue;
                    }
                }

                if (entities[i].state == STATE_PROJECTILE && entities[j].state == STATE_PROJECTILE) {
                    Spell fused = FuseSpellData(entities[i].spellData, entities[j].spellData);
                    SpawnExplosion(ps, entities[i].position, GOLD);
                    entities[i].spellData = fused; entities[i].size += 5.0f; entities[j].isActive = false;
                    continue; 
                }

                Vector2 diff = Vector2Subtract(entities[i].position, entities[j].position);
                if (Vector2Length(diff) == 0) diff = (Vector2){1,0};
                Vector2 push = Vector2Scale(Vector2Normalize(diff), 1.0f);
                entities[i].position = Vector2Add(entities[i].position, push);
                entities[j].position = Vector2Subtract(entities[j].position, push);
                Vector2 tempVel = entities[i].velocity; entities[i].velocity = entities[j].velocity; entities[j].velocity = tempVel;
            }
        }
    }
}

void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount) {}