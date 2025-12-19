// HANDLES MATH AND PHYSICS

#include "game.h" // Include our shared definitions

void UpdatePlayer(Character* player, Rectangle* walls, int wallCount) {
    float delta = GetFrameTime();

    // --- 1. INPUT ---
    Vector2 direction = {0.0f, 0.0f};
    if (IsKeyDown(KEY_A) != IsKeyDown(KEY_D)) direction.x = (IsKeyDown(KEY_A)) ? -1 : 1;
    if (IsKeyDown(KEY_W) != IsKeyDown(KEY_S)) direction.y = (IsKeyDown(KEY_W)) ? -1 : 1;

    if (Vector2Length(direction) > 0) direction = Vector2Normalize(direction);

    // --- 2. PHYSICS CALCULATIONS (Acceleration/Deceleration) ---
    Vector2 desired_velocity = Vector2Scale(direction, player->speed);
    Vector2 steering = Vector2Subtract(desired_velocity, player->velocity);
    Vector2 force = Vector2Scale(steering, player->steering_factor);

    // Updates player's velocity
    player->velocity = Vector2Add(player->velocity, Vector2Scale(force, delta));

    // --- 3. MOVEMENT & COLLISION ---
    float hitboxSize = player->size * 1.5f;
    Rectangle playerHitbox = {
        player->position.x - hitboxSize / 2,
        player->position.y - hitboxSize / 2,
        hitboxSize,
        hitboxSize
    };

    // Move X
    player->position.x += player->velocity.x * delta;
    playerHitbox.x = player->position.x - hitboxSize / 2;
    
    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(playerHitbox, walls[i])) {
            player->position.x -= player->velocity.x * delta;
            player->velocity.x = 0;
            playerHitbox.x = player->position.x - hitboxSize / 2;
        }
    }

    // Move Y
    player->position.y += player->velocity.y * delta;
    playerHitbox.y = player->position.y - hitboxSize / 2;

    for (int i = 0; i < wallCount; i++) {
        if (CheckCollisionRecs(playerHitbox, walls[i])) {
            player->position.y -= player->velocity.y * delta;
            player->velocity.y = 0;
        }
    }
}