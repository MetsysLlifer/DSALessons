// HANDLES MATH AND PHYSICS
#include "game.h" 

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount) {
    float dt = GetFrameTime();

    // --- 1. APPLY FORCES ---
    if (Vector2Length(inputDirection) > 0) {
        // Normalize input
        inputDirection = Vector2Normalize(inputDirection);

        // Calculate desired velocity
        Vector2 desiredVelocity = Vector2Scale(inputDirection, e->maxSpeed);
        
        // Steering force = Desired - Current
        Vector2 steering = Vector2Subtract(desiredVelocity, e->velocity);
        
        // Limit the force by the entity's "Strength"
        if (Vector2Length(steering) > e->moveForce) {
            steering = Vector2Normalize(steering);
            steering = Vector2Scale(steering, e->moveForce);
        }

        // NEWTON'S LAW: Acceleration = Force / Mass
        // Heavier objects accelerate slower
        Vector2 acceleration = Vector2Scale(steering, 1.0f / e->mass);
        
        e->velocity = Vector2Add(e->velocity, Vector2Scale(acceleration, dt));
    } 
    else {
        // --- FRICTION (Stop when no input) ---
        // Friction opposes current velocity
        Vector2 frictionDir = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
        Vector2 frictionForce = Vector2Scale(frictionDir, e->friction * e->mass);
        
        // Stop completely if moving very slowly to prevent jitter
        if (Vector2Length(e->velocity) < 10.0f) {
             e->velocity = (Vector2){0,0};
        } else {
             e->velocity = Vector2Add(e->velocity, Vector2Scale(frictionForce, dt));
        }
    }

    // --- 2. MOVEMENT & COLLISION ---
    float hitboxSize = e->size * 1.5f;
    Rectangle hitBox = { 
        e->position.x - hitboxSize / 2, 
        e->position.y - hitboxSize / 2, 
        hitboxSize, 
        hitboxSize 
    };

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

void ResolveEntityCollisions(Entity* entities, int count) {
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            Entity* a = &entities[i];
            Entity* b = &entities[j];

            // 1. Check Intersection
            if (CheckCollisionCircles(a->position, a->size, b->position, b->size)) {
                
                // --- STEP A: POSITIONAL CORRECTION (Un-stick them) ---
                Vector2 diff = Vector2Subtract(a->position, b->position);
                float distance = Vector2Length(diff);
                if (distance == 0) { distance = 0.1f; diff = (Vector2){0.1f, 0.0f}; }

                float overlap = (a->size + b->size) - distance;
                Vector2 normal = Vector2Normalize(diff);
                Vector2 pushVector = Vector2Scale(normal, overlap);

                // Mass ratios for pushing
                float totalMass = a->mass + b->mass;
                float ratioA = b->mass / totalMass; 
                float ratioB = a->mass / totalMass;

                // Move them apart
                a->position = Vector2Add(a->position, Vector2Scale(pushVector, ratioA));
                b->position = Vector2Subtract(b->position, Vector2Scale(pushVector, ratioB));


                // --- STEP B: MOMENTUM RESOLUTION (The Bounce) ---
                // This makes them bounce off each other based on Mass
                
                // 1. Calculate Relative Velocity
                Vector2 relVelocity = Vector2Subtract(a->velocity, b->velocity);
                
                // 2. Calculate velocity along the normal (Dot Product)
                float velocityAlongNormal = Vector2DotProduct(relVelocity, normal);

                // If objects are already moving apart, do nothing
                if (velocityAlongNormal > 0) continue;

                // 3. Calculate "Restitution" (Bounciness)
                // 0.0 = Mud (No bounce), 1.0 = Superball (Full bounce)
                float e = 0.5f; 

                // 4. Calculate Impulse Scalar (The magic physics formula)
                float j = -(1 + e) * velocityAlongNormal;
                j /= (1 / a->mass + 1 / b->mass);

                // 5. Apply Impulse to velocities
                Vector2 impulse = Vector2Scale(normal, j);
                
                a->velocity = Vector2Add(a->velocity, Vector2Scale(impulse, 1 / a->mass));
                b->velocity = Vector2Subtract(b->velocity, Vector2Scale(impulse, 1 / b->mass));
            }
        }
    }
}

void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount) {
    for (int i = 0; i < count; i++) {
        Entity* e = &entities[i];
        
        // Recalculate Hitbox (Since the entity might have been shoved)
        float hitboxSize = e->size * 1.5f;
        Rectangle hitBox = { 
            e->position.x - hitboxSize / 2, 
            e->position.y - hitboxSize / 2, 
            hitboxSize, 
            hitboxSize 
        };

        for (int w = 0; w < wallCount; w++) {
            if (CheckCollisionRecs(hitBox, walls[w])) {
                // Get the intersection rectangle (The overlap area)
                Rectangle overlap = GetCollisionRec(hitBox, walls[w]);

                // LOGIC: Push out the shortest way possible
                // If overlap is wider than it is tall, we probably hit the top/bottom
                if (overlap.width > overlap.height) {
                    // Vertical Collision
                    if (e->position.y < walls[w].y) {
                        e->position.y -= overlap.height; // Push Up
                    } else {
                        e->position.y += overlap.height; // Push Down
                    }
                    e->velocity.y = 0; // Kill momentum
                } 
                else {
                    // Horizontal Collision
                    if (e->position.x < walls[w].x) {
                        e->position.x -= overlap.width; // Push Left
                    } else {
                        e->position.x += overlap.width; // Push Right
                    }
                    e->velocity.x = 0; // Kill momentum
                }
            }
        }
    }
}