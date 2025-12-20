// HANDLES MATH AND PHYSICS
#include "game.h" 

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount) {
    float dt = GetFrameTime();

    // --- 1. APPLY FORCES (Standard Movement) ---
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
    } 
    else {
        // Friction
        Vector2 frictionDir = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
        Vector2 frictionForce = Vector2Scale(frictionDir, e->friction * e->mass);
        
        if (Vector2Length(e->velocity) < 10.0f) e->velocity = (Vector2){0,0};
        else e->velocity = Vector2Add(e->velocity, Vector2Scale(frictionForce, dt));
    }

    // --- 2. INTEGRATION & COLLISION ---
    float hitboxSize = e->size * 1.5f;
    Rectangle hitBox = { e->position.x - hitboxSize/2, e->position.y - hitboxSize/2, hitboxSize, hitboxSize };

    // Move X
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
}

void ResolveEntityCollisions(Entity* entities, int count, ParticleSystem* ps) {
    for (int i = 0; i < count; i++) {
        if (!entities[i].isActive) continue; 

        for (int j = i + 1; j < count; j++) {
            if (!entities[j].isActive) continue;

            Entity* a = &entities[i];
            Entity* b = &entities[j];

            if (CheckCollisionCircles(a->position, a->size, b->position, b->size)) {
                
                // --- MAGIC FUSION LOGIC ---
                if (a->isSpell && b->isSpell) {
                    Color explosionColor = Fade(a->color, 0.8f);
                    SpawnExplosion(ps, a->position, explosionColor);

                    Spell fusedSpell = FuseSpellData(a->spellData, b->spellData);
                    
                    Vector2 midVel = Vector2Scale(Vector2Add(a->velocity, b->velocity), 0.5f);
                    *a = CreateSpellEntity(fusedSpell, a->position, midVel);
                    b->isActive = false; 
                    continue; 
                }
                
                // --- PHYSICS BOUNCE ---
                Vector2 diff = Vector2Subtract(a->position, b->position);
                float distance = Vector2Length(diff);
                if (distance == 0) { distance = 0.1f; diff = (Vector2){0.1f, 0.0f}; }

                float overlap = (a->size + b->size) - distance;
                Vector2 normal = Vector2Normalize(diff);
                Vector2 pushVector = Vector2Scale(normal, overlap);

                float totalMass = a->mass + b->mass;
                float ratioA = b->mass / totalMass; 
                float ratioB = a->mass / totalMass;

                a->position = Vector2Add(a->position, Vector2Scale(pushVector, ratioA));
                b->position = Vector2Subtract(b->position, Vector2Scale(pushVector, ratioB));
                
                Vector2 relVelocity = Vector2Subtract(a->velocity, b->velocity);
                float velocityAlongNormal = Vector2DotProduct(relVelocity, normal);
                if (velocityAlongNormal > 0) continue;
                float e = 0.5f; 
                float imp = -(1 + e) * velocityAlongNormal;
                imp /= (1 / a->mass + 1 / b->mass);
                Vector2 impulse = Vector2Scale(normal, imp);
                a->velocity = Vector2Add(a->velocity, Vector2Scale(impulse, 1 / a->mass));
                b->velocity = Vector2Subtract(b->velocity, Vector2Scale(impulse, 1 / b->mass));
            }
        }
    }
}

void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount) {
    for (int i = 0; i < count; i++) {
        Entity* e = &entities[i];
        float hitboxSize = e->size * 1.5f;
        Rectangle hitBox = { e->position.x - hitboxSize/2, e->position.y - hitboxSize/2, hitboxSize, hitboxSize };

        for (int w = 0; w < wallCount; w++) {
            if (CheckCollisionRecs(hitBox, walls[w])) {
                Rectangle overlap = GetCollisionRec(hitBox, walls[w]);
                if (overlap.width > overlap.height) {
                    if (e->position.y < walls[w].y) e->position.y -= overlap.height; 
                    else e->position.y += overlap.height; 
                    e->velocity.y = 0; 
                } else {
                    if (e->position.x < walls[w].x) e->position.x -= overlap.width; 
                    else e->position.x += overlap.width; 
                    e->velocity.x = 0; 
                }
            }
        }
    }
}