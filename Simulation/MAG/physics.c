#include "game.h"

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount) {
    if (e->isHeld || e->state == STATE_STATIC_WALL) return; 

    float dt = GetFrameTime();
    e->lifeTime += dt * 5.0f;

    // --- 1. APPLY FORCES ---
    if (Vector2Length(inputDirection) > 0) {
        inputDirection = Vector2Normalize(inputDirection);
        Vector2 desiredVelocity = Vector2Scale(inputDirection, e->speed);
        Vector2 steering = Vector2Subtract(desiredVelocity, e->velocity);
        
        if (Vector2Length(steering) > e->steering_factor) {
            steering = Vector2Normalize(steering);
            steering = Vector2Scale(steering, e->steering_factor);
        }
        Vector2 acceleration = Vector2Scale(steering, 1.0f / e->mass);
        e->velocity = Vector2Add(e->velocity, Vector2Scale(acceleration, dt));
    } 
    else {
        // Friction
        if (Vector2Length(e->velocity) > 0) {
            Vector2 frictionDir = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
            Vector2 frictionForce = Vector2Scale(frictionDir, e->friction * e->mass);
            Vector2 frictionAccel = Vector2Scale(frictionForce, 1.0f / e->mass);
            e->velocity = Vector2Add(e->velocity, Vector2Scale(frictionAccel, dt));
        }
    }

    // --- 2. MOVE & CHECK WALLS (Prevent Tunneling) ---
    float hitboxSize = e->size * 1.5f; // Slightly larger for wall checks
    Rectangle hitBox = { e->position.x - hitboxSize/2, e->position.y - hitboxSize/2, hitboxSize, hitboxSize };

    // Move X
    e->position.x += e->velocity.x * dt;
    hitBox.x = e->position.x - hitboxSize / 2;
    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(hitBox, walls[i])) {
            // Revert Move
            e->position.x -= e->velocity.x * dt;
            e->velocity.x = 0; // Stop
            hitBox.x = e->position.x - hitboxSize / 2;
        }
    }

    // Move Y
    e->position.y += e->velocity.y * dt;
    hitBox.y = e->position.y - hitboxSize / 2;
    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(hitBox, walls[i])) {
            // Revert Move
            e->position.y -= e->velocity.y * dt;
            e->velocity.y = 0; // Stop
        }
    }
    
    // Bounds Check (Keep inside screen)
    if (e->position.x < 0) { e->position.x = 0; e->velocity.x *= -1; }
    if (e->position.x > SCREEN_WIDTH) { e->position.x = SCREEN_WIDTH; e->velocity.x *= -1; }
    if (e->position.y < 0) { e->position.y = 0; e->velocity.y *= -1; }
    if (e->position.y > SCREEN_HEIGHT) { e->position.y = SCREEN_HEIGHT; e->velocity.y *= -1; }
}

void ResolveEntityCollisions(Entity* entities, int count, Entity* player, ParticleSystem* ps) {
    for (int i = 0; i < count; i++) {
        if (!entities[i].isActive) continue;

        // A. WALL SPELL (Solidify)
        if (entities[i].state == STATE_PROJECTILE && entities[i].spellData.behavior == SPELL_WALL) {
             if (Vector2Length(entities[i].velocity) < 20.0f) {
                 entities[i].state = STATE_STATIC_WALL;
                 entities[i].mass = 5000.0f; 
                 entities[i].color = BROWN;
             }
        }

        // B. ENTITY vs ENTITY
        for (int j = i + 1; j < count; j++) {
            if (!entities[j].isActive) continue;
            
            if (CheckCollisionCircles(entities[i].position, entities[i].size, entities[j].position, entities[j].size)) {
                
                // 1. MID-AIR FUSION (Spell + Spell)
                if (entities[i].state == STATE_PROJECTILE && entities[j].state == STATE_PROJECTILE) {
                    Spell fused = FuseSpellData(entities[i].spellData, entities[j].spellData);
                    SpawnExplosion(ps, entities[i].position, GOLD);
                    entities[i].spellData = fused;
                    entities[i].size += 5.0f;
                    entities[j].isActive = false;
                    continue; 
                }

                // 2. PHYSICAL PUSH (Bounce)
                Vector2 diff = Vector2Subtract(entities[i].position, entities[j].position);
                if (Vector2Length(diff) == 0) diff = (Vector2){1,0};
                Vector2 push = Vector2Scale(Vector2Normalize(diff), 1.0f);
                
                entities[i].position = Vector2Add(entities[i].position, push);
                entities[j].position = Vector2Subtract(entities[j].position, push);
            }
        }
    }
}

void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount) {}
void ApplyTelekinesis(Entity* player, Entity* entities, int count) {}