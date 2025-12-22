#include "game.h"

// Standard Movement with Wall Checks
void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount) {
    if (e->isHeld || e->state == STATE_STATIC_WALL) return; 

    float dt = GetFrameTime();
    e->lifeTime += dt * 5.0f;

    // Forces
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
        if (Vector2Length(e->velocity) > 0) {
            Vector2 frictionDir = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
            Vector2 frictionForce = Vector2Scale(frictionDir, e->friction * e->mass);
            Vector2 frictionAccel = Vector2Scale(frictionForce, 1.0f / e->mass);
            e->velocity = Vector2Add(e->velocity, Vector2Scale(frictionAccel, dt));
        }
    }

    // Move X
    float hitboxSize = e->size * 1.5f; 
    Rectangle hitBox = { e->position.x - hitboxSize/2, e->position.y - hitboxSize/2, hitboxSize, hitboxSize };
    e->position.x += e->velocity.x * dt;
    hitBox.x = e->position.x - hitboxSize / 2;
    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(hitBox, walls[i])) {
            e->position.x -= e->velocity.x * dt;
            e->velocity.x = 0; 
            hitBox.x = e->position.x - hitboxSize / 2;
        }
    }
    // Move Y
    e->position.y += e->velocity.y * dt;
    hitBox.y = e->position.y - hitboxSize / 2;
    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(hitBox, walls[i])) {
            e->position.y -= e->velocity.y * dt;
            e->velocity.y = 0; 
        }
    }
    
    // Bounds
    if (e->position.x < 0) { e->position.x = 0; e->velocity.x *= -1; }
    if (e->position.x > SCREEN_WIDTH) { e->position.x = SCREEN_WIDTH; e->velocity.x *= -1; }
    if (e->position.y < 0) { e->position.y = 0; e->velocity.y *= -1; }
    if (e->position.y > SCREEN_HEIGHT) { e->position.y = SCREEN_HEIGHT; e->velocity.y *= -1; }
}

// ACTIVE EFFECTS (Void / Telekinesis)
void ApplySpellFieldEffects(Entity* entities, int count, ParticleSystem* ps) {
    for(int i=0; i<count; i++) {
        if(!entities[i].isActive || entities[i].state != STATE_PROJECTILE) continue;
        
        // VOID WELL (Black Hole)
        if(entities[i].spellData.behavior == SPELL_VOID) {
            for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive || entities[j].state == STATE_STATIC_WALL) continue;
                
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 300.0f && dist > 10.0f) {
                    Vector2 pull = Vector2Subtract(entities[i].position, entities[j].position);
                    pull = Vector2Normalize(pull);
                    // Strong Pull
                    entities[j].velocity = Vector2Add(entities[j].velocity, Vector2Scale(pull, 50.0f));
                }
            }
        }
        
        // TELEKINESIS (Gravity Well)
        if(entities[i].spellData.behavior == SPELL_TELEKINESIS) {
             for(int j=0; j<count; j++) {
                if(i==j || !entities[j].isActive) continue;
                float dist = Vector2Distance(entities[i].position, entities[j].position);
                if(dist < 200.0f) {
                    // Gentle orbit/float
                    entities[j].velocity.y -= 10.0f; // Levitate
                }
            }
        }
    }
}

// COLLISIONS & ON-HIT EFFECTS
void ResolveEntityCollisions(Entity* entities, int count, Entity* player, ParticleSystem* ps) {
    for (int i = 0; i < count; i++) {
        if (!entities[i].isActive) continue;

        // WALL SPELL (Solidify)
        if (entities[i].state == STATE_PROJECTILE && entities[i].spellData.behavior == SPELL_WALL) {
             if (Vector2Length(entities[i].velocity) < 20.0f) {
                 entities[i].state = STATE_STATIC_WALL;
                 entities[i].mass = 5000.0f; 
                 entities[i].color = BROWN;
             }
        }

        for (int j = i + 1; j < count; j++) {
            if (!entities[j].isActive) continue;
            
            if (CheckCollisionCircles(entities[i].position, entities[i].size, entities[j].position, entities[j].size)) {
                
                // 1. PROJECTILE HITTING SOMETHING
                Entity* proj = (entities[i].state == STATE_PROJECTILE) ? &entities[i] : (entities[j].state == STATE_PROJECTILE ? &entities[j] : NULL);
                Entity* target = (proj == &entities[i]) ? &entities[j] : &entities[i];
                
                if (proj && target->state != STATE_PROJECTILE) {
                    // MIDAS TOUCH (Turn to Gold)
                    if (proj->spellData.behavior == SPELL_MIDAS) {
                        target->state = STATE_STATIC_WALL;
                        target->color = GOLD;
                        target->mass = 1000.0f;
                        target->velocity = (Vector2){0,0};
                        SpawnExplosion(ps, target->position, GOLD);
                        proj->isActive = false; // Spell consumed
                        continue;
                    }
                    
                    // SLOW / MUD (Slow down)
                    if (proj->spellData.behavior == SPELL_SLOW) {
                        target->velocity = Vector2Scale(target->velocity, 0.1f);
                        target->color = Fade(target->color, 0.5f); // Look muddy
                        SpawnExplosion(ps, target->position, BROWN);
                        proj->isActive = false;
                        continue;
                    }
                }

                // 2. MID-AIR FUSION
                if (entities[i].state == STATE_PROJECTILE && entities[j].state == STATE_PROJECTILE) {
                    Spell fused = FuseSpellData(entities[i].spellData, entities[j].spellData);
                    SpawnExplosion(ps, entities[i].position, GOLD);
                    entities[i].spellData = fused;
                    entities[i].size += 5.0f;
                    entities[j].isActive = false;
                    continue; 
                }

                // 3. PHYSICAL BOUNCE
                Vector2 diff = Vector2Subtract(entities[i].position, entities[j].position);
                if (Vector2Length(diff) == 0) diff = (Vector2){1,0};
                Vector2 push = Vector2Scale(Vector2Normalize(diff), 1.0f);
                
                entities[i].position = Vector2Add(entities[i].position, push);
                entities[j].position = Vector2Subtract(entities[j].position, push);
                
                Vector2 tempVel = entities[i].velocity;
                entities[i].velocity = entities[j].velocity;
                entities[j].velocity = tempVel;
            }
        }
    }
}

void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount) {}