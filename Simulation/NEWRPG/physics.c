#include "game.h"

// --- SMART AI SYSTEM ---
void UpdateEntityAI(Entity* e, Entity* entities, int count, Vector2 playerPos) {
    if (!e->isActive || !e->isSpell) return;
    
    // DEFAULT TARGET: The Player (for enemies) or Mouse (for player spells - overridden in main)
    // However, since these are "Spells" usually cast BY the player, 
    // let's make them target the nearest "Enemy" or static object if it's an offensive spell.
    
    Vector2 targetPos = playerPos; 
    float closestDist = 9999.0f;
    int targetIndex = -1;

    // Find nearest valid target (Skip self, skip player if friendly fire is off)
    for(int i=0; i<count; i++) {
        if (!entities[i].isActive || &entities[i] == e) continue;
        // Don't target held items or other projectiles usually
        if (entities[i].state == STATE_STATIC_WALL || entities[i].state == STATE_RAW) {
            float d = Vector2Distance(e->position, entities[i].position);
            if(d < closestDist) { closestDist = d; targetPos = entities[i].position; targetIndex = i; }
        }
    }

    // BEHAVIOR LOGIC
    if (e->spellData.aiType == AI_HOMING) {
        // Steer towards target
        Vector2 toTarget = Vector2Subtract(targetPos, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toTarget), 25.0f));
    }
    else if (e->spellData.aiType == AI_PREDICT && targetIndex != -1) {
        // Lead the target based on its velocity
        Vector2 predicted = Vector2Add(targetPos, Vector2Scale(entities[targetIndex].velocity, 0.5f));
        Vector2 toTarget = Vector2Subtract(predicted, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toTarget), 40.0f));
    }
    else if (e->spellData.aiType == AI_ORBIT) {
        // Defensive Circle around the Player (or origin)
        float time = GetTime();
        Vector2 orbitPoint = { 
            playerPos.x + cosf(time * 3.0f + e->size) * 120.0f, 
            playerPos.y + sinf(time * 3.0f + e->size) * 120.0f 
        };
        Vector2 toOrbit = Vector2Subtract(orbitPoint, e->position);
        // Spring force to orbit position
        e->velocity = Vector2Add(e->velocity, Vector2Scale(toOrbit, 0.1f)); 
    }
    else if (e->spellData.aiType == AI_SWARM) {
        // Flocking behavior (Separation + Cohesion)
        Vector2 separation = {0,0};
        int neighbors = 0;
        for(int i=0; i<count; i++) {
            if(&entities[i] == e || !entities[i].isActive) continue;
            float d = Vector2Distance(e->position, entities[i].position);
            if(d < 50.0f) {
                Vector2 push = Vector2Subtract(e->position, entities[i].position);
                separation = Vector2Add(separation, Vector2Scale(Vector2Normalize(push), 10.0f));
                neighbors++;
            }
        }
        e->velocity = Vector2Add(e->velocity, separation);
        
        // Also seek target
        Vector2 toTarget = Vector2Subtract(targetPos, e->position);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(Vector2Normalize(toTarget), 5.0f));
    }
    else if (e->spellData.aiType == AI_ERRATIC) {
        // Jitter randomly
        Vector2 jitter = { (float)GetRandomValue(-20,20), (float)GetRandomValue(-20,20) };
        e->velocity = Vector2Add(e->velocity, jitter);
    }
}

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount) {
    if (e->isHeld || e->state == STATE_STATIC_WALL) return; 
    float dt = GetFrameTime(); 
    e->lifeTime += dt * 5.0f;

    // Movement Physics
    if (Vector2Length(inputDirection) > 0) {
        inputDirection = Vector2Normalize(inputDirection);
        Vector2 desiredVelocity = Vector2Scale(inputDirection, e->maxSpeed);
        Vector2 steering = Vector2Subtract(desiredVelocity, e->velocity);
        
        if (Vector2Length(steering) > e->moveForce) {
            steering = Vector2Normalize(steering);
            steering = Vector2Scale(steering, e->moveForce);
        }
        Vector2 acceleration = Vector2Scale(steering, 1.0f / e->mass);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(acceleration, dt));
    } else {
        // Friction
        Vector2 frictionDir = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
        Vector2 frictionForce = Vector2Scale(frictionDir, e->friction * e->mass);
        
        if (Vector2Length(e->velocity) < 10.0f) e->velocity = (Vector2){0,0};
        else e->velocity = Vector2Add(e->velocity, Vector2Scale(frictionForce, dt));
    }

    // Wall Collisions (Slide)
    float hitboxSize = e->size * 1.5f; 
    Rectangle hitBox = { e->position.x - hitboxSize/2, e->position.y - hitboxSize/2, hitboxSize, hitboxSize };
    
    // PHANTOM spells ignore walls
    if (e->spellData.behavior != SPELL_PHANTOM) {
        // X Axis
        e->position.x += e->velocity.x * dt;
        hitBox.x = e->position.x - hitboxSize / 2;
        for (int i = 0; i < wallCount; i++) {
            if (CheckCollisionRecs(hitBox, walls[i])) { 
                e->position.x -= e->velocity.x * dt; 
                e->velocity.x = 0; 
                hitBox.x = e->position.x - hitboxSize / 2; 
            }
        }
        // Y Axis
        e->position.y += e->velocity.y * dt;
        hitBox.y = e->position.y - hitboxSize / 2;
        for (int i = 0; i < wallCount; i++) {
            if (CheckCollisionRecs(hitBox, walls[i])) { 
                e->position.y -= e->velocity.y * dt; 
                e->velocity.y = 0; 
            }
        }
    } else {
        e->position = Vector2Add(e->position, Vector2Scale(e->velocity, dt));
    }
    
    // Bounds Check
    if (e->position.x < 0) { e->position.x = 0; e->velocity.x *= -1; }
    if (e->position.x > SCREEN_WIDTH) { e->position.x = SCREEN_WIDTH; e->velocity.x *= -1; }
    if (e->position.y < 0) { e->position.y = 0; e->velocity.y *= -1; }
    if (e->position.y > SCREEN_HEIGHT) { e->position.y = SCREEN_HEIGHT; e->velocity.y *= -1; }
}

void ApplySpellFieldEffects(Entity* entities, int count, ParticleSystem* ps) {
    for(int i=0; i<count; i++) {
        if(!entities[i].isActive || entities[i].state != STATE_PROJECTILE) continue;
        
        // VOID WELL: Pulls strongly towards center
        if(entities[i].spellData.behavior == SPELL_VOID) {
            for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive || entities[j].state == STATE_STATIC_WALL) continue;
                
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 300.0f) {
                    Vector2 pull = Vector2Subtract(entities[i].position, entities[j].position);
                    // The closer they are, the stronger the pull
                    float strength = (300.0f - dist) / 5.0f; 
                    entities[j].velocity = Vector2Add(entities[j].velocity, Vector2Scale(Vector2Normalize(pull), strength));
                }
            }
        }
        
        // MAGNET: Pulls Earth/Metal objects only
        if(entities[i].spellData.behavior == SPELL_MAGNET) {
            for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive) continue;
                if(entities[j].spellData.core == ELEM_EARTH || entities[j].state == STATE_STATIC_WALL) {
                    float dist = Vector2Distance(entities[i].position, entities[j].position);
                    if(dist < 250.0f) {
                        Vector2 pull = Vector2Subtract(entities[i].position, entities[j].position);
                        entities[j].velocity = Vector2Add(entities[j].velocity, Vector2Scale(Vector2Normalize(pull), 40.0f));
                    }
                }
            }
        }
        
        // GRAVITY WELL (TELEKINESIS): Stops movement (Crushing Gravity)
        if(entities[i].spellData.behavior == SPELL_TELEKINESIS) {
             for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive || entities[j].isSpell) continue;
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 150.0f) {
                    // Apply MASSIVE friction to simulate being held down
                    entities[j].velocity = Vector2Scale(entities[j].velocity, 0.5f);
                }
            }
        }

        // WHIRLWIND / TSUNAMI: Push away
        if(entities[i].spellData.behavior == SPELL_WHIRLWIND || entities[i].spellData.behavior == SPELL_TSUNAMI) {
             for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive) continue;
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 100.0f) {
                    Vector2 push = Vector2Subtract(entities[j].position, entities[i].position);
                    entities[j].velocity = Vector2Add(entities[j].velocity, Vector2Scale(Vector2Normalize(push), 50.0f));
                }
            }
        }
    }
}

void ResolveEntityCollisions(Entity* entities, int count, Entity* player, ParticleSystem* ps) {
    for (int i = 0; i < count; i++) {
        if (!entities[i].isActive) continue;

        // SPELL: WALL (Solidify on stop)
        if (entities[i].state == STATE_PROJECTILE && entities[i].spellData.behavior == SPELL_WALL) {
             if (Vector2Length(entities[i].velocity) < 20.0f) {
                 entities[i].state = STATE_STATIC_WALL;
                 entities[i].mass = 50000.0f; // Very heavy
                 entities[i].color = BROWN;
             }
        }
        
        // SPELL: LANDMINE (Invisible when slow)
        if (entities[i].state == STATE_PROJECTILE && entities[i].spellData.behavior == SPELL_LANDMINE) {
             if (Vector2Length(entities[i].velocity) < 10.0f) {
                 entities[i].color.a = 20; // Almost invisible
             }
        }

        // Collisions
        for (int j = i + 1; j < count; j++) {
            if (!entities[j].isActive) continue;
            
            if (CheckCollisionCircles(entities[i].position, entities[i].size, entities[j].position, entities[j].size)) {
                
                Entity* proj = (entities[i].state == STATE_PROJECTILE) ? &entities[i] : (entities[j].state == STATE_PROJECTILE ? &entities[j] : NULL);
                Entity* target = (proj == &entities[i]) ? &entities[j] : &entities[i];
                
                // If a Projectile hits a Non-Projectile (Target)
                if (proj && target->state != STATE_PROJECTILE) {
                    
                    // MIDAS: Turn to Gold
                    if (proj->spellData.behavior == SPELL_MIDAS) {
                        target->state = STATE_STATIC_WALL; 
                        target->color = GOLD; 
                        target->mass = 5000.0f; 
                        target->velocity = (Vector2){0,0};
                        SpawnExplosion(ps, target->position, GOLD); 
                        proj->isActive = false; continue;
                    }
                    
                    // PETRIFY: Turn to Stone
                    if (proj->spellData.behavior == SPELL_PETRIFY) {
                        target->state = STATE_STATIC_WALL; 
                        target->color = GRAY; 
                        target->mass = 5000.0f; 
                        target->velocity = (Vector2){0,0};
                        SpawnExplosion(ps, target->position, GRAY); 
                        proj->isActive = false; continue;
                    }
                    
                    // FREEZE: Stop Movement
                    if (proj->spellData.behavior == SPELL_FREEZE) {
                        target->velocity = (Vector2){0,0}; 
                        target->color = SKYBLUE; 
                        // Give target high mass temporarily to simulate frozen? (Simplification: just color/vel)
                        SpawnExplosion(ps, target->position, SKYBLUE); 
                        proj->isActive = false; continue;
                    }
                    
                    // SHRINK
                    if (proj->spellData.behavior == SPELL_SHRINK) {
                        target->size *= 0.5f; 
                        if(target->size < 5.0f) target->isActive = false; // Poof
                        SpawnExplosion(ps, target->position, PURPLE); 
                        proj->isActive = false; continue;
                    }
                    
                    // GROWTH
                    if (proj->spellData.behavior == SPELL_GROWTH) {
                        target->size *= 1.5f; 
                        SpawnExplosion(ps, target->position, GREEN); 
                        proj->isActive = false; continue;
                    }
                    
                    // CLUSTER: Spawn mini-explosions
                    if (proj->spellData.behavior == SPELL_CLUSTER) {
                        SpawnExplosion(ps, proj->position, ORANGE); 
                        SpawnExplosion(ps, Vector2Add(proj->position, (Vector2){20,0}), RED);
                        SpawnExplosion(ps, Vector2Add(proj->position, (Vector2){-20,0}), RED);
                        proj->isActive = false; continue;
                    }
                    
                    // BOUNCE: Don't die, just bounce off
                    if (proj->spellData.behavior == SPELL_BOUNCE) {
                        // Handled by physical bounce below
                    } else {
                        // Standard projectiles die on impact
                        proj->isActive = false; 
                    }
                }

                // Mid-Air Magic Fusion
                if (entities[i].state == STATE_PROJECTILE && entities[j].state == STATE_PROJECTILE) {
                    Spell fused = FuseSpellData(entities[i].spellData, entities[j].spellData);
                    SpawnExplosion(ps, entities[i].position, GOLD);
                    entities[i].spellData = fused; 
                    entities[i].size += 5.0f; 
                    entities[j].isActive = false;
                    continue; 
                }

                // Physics Body Bounce
                Vector2 diff = Vector2Subtract(entities[i].position, entities[j].position);
                if (Vector2Length(diff) == 0) diff = (Vector2){1,0};
                Vector2 push = Vector2Scale(Vector2Normalize(diff), 1.0f);
                
                entities[i].position = Vector2Add(entities[i].position, push);
                entities[j].position = Vector2Subtract(entities[j].position, push);
                
                // Exchange momentum
                Vector2 tempVel = entities[i].velocity; 
                entities[i].velocity = entities[j].velocity; 
                entities[j].velocity = tempVel;
            }
        }
    }
}

void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount) {}