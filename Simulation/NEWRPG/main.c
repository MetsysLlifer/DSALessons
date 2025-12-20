/*
ACTS AS THE "MANAGER"
Restored to the version before "Alchemy/Drafting".
*/
#include "game.h"
#define MAX_ENTITIES 100
#define PICKUP_RANGE 100.0f 

// Garbage Collection
void CleanupEntities(Entity* entities, int* count) {
    for (int i = 1; i < *count; i++) {
        if (!entities[i].isActive) {
            entities[i] = entities[*count - 1];
            (*count)--;
            i--; 
        }
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Top Down Collision");
    SetTargetFPS(60); 

    // --- DATA INITIALIZATION ---
    Entity entities[MAX_ENTITIES];
    int entityCount = 0;

    // Initialize Player (Index 0)
    entities[entityCount++] = (Entity){
        .position = {100, 100}, .velocity = {0,0}, 
        .mass = 1.0f, .friction = 10.0f, .size = 32.0f, 
        .maxSpeed = 600.0f, .moveForce = 3000.0f, 
        .color = MAROON,
        .isSpell = false, .isActive = true
    };
    Entity* player = &entities[0];

    // --- PARTICLE SYSTEM ---
    ParticleSystem particleSystem;
    InitParticles(&particleSystem);

    // --- SPELL TEMPLATES ---
    Spell fireSpell = { {"Fire"}, ELEMENT_PLASMA, FORM_PLASMA, 100.0, 0.5, 0.9, true };
    Spell rockSpell = { {"Rock"}, ELEMENT_SOLID, FORM_SOLID, 20.0, 0.8, 1.0, true };

    // Add some initial objects
    entities[entityCount++] = CreateSpellEntity(rockSpell, (Vector2){300, 300}, (Vector2){0,0});
    entities[entityCount++] = CreateSpellEntity(fireSpell, (Vector2){500, 200}, (Vector2){0,0});

    // --- INVENTORY ---
    Inventory inventory = { 0 };
    InitInventory(&inventory);

    Rectangle walls[WALL_COUNT] = {
        {200, 150, 400, 50},
        {150, 150, 50, 300},
        {600, 300, 50, 200}
    };

    Camera2D camera = { 0 };
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    camera.zoom = 1.0f;
    bool showStatus = false;

    // --- GAME LOOP ---
    while (!WindowShouldClose()) {
        Vector2 mouseScreen = GetMousePosition();
        Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
        camera.target = player->position;

        // -- 1. PLAYER INPUT (WASD) ---
        Vector2 playerInput = {0.0f, 0.0f};
        if (IsKeyDown(KEY_A)) playerInput.x -= 1;
        if (IsKeyDown(KEY_D)) playerInput.x += 1;
        if (IsKeyDown(KEY_W)) playerInput.y -= 1;
        if (IsKeyDown(KEY_S)) playerInput.y += 1;

        // --- 2. MAGIC INPUT (F and R) ---
        if (IsKeyPressed(KEY_F) && entityCount < MAX_ENTITIES) {
            Vector2 dir = Vector2Normalize(Vector2Subtract(mouseWorld, player->position));
            Vector2 vel = Vector2Scale(dir, 600.0f); 
            Vector2 spawn = Vector2Add(player->position, Vector2Scale(dir, 40));
            entities[entityCount++] = CreateSpellEntity(fireSpell, spawn, vel);
        }
        if (IsKeyPressed(KEY_R) && entityCount < MAX_ENTITIES) {
            Vector2 dir = Vector2Normalize(Vector2Subtract(mouseWorld, player->position));
            Vector2 vel = Vector2Scale(dir, 400.0f); 
            Vector2 spawn = Vector2Add(player->position, Vector2Scale(dir, 40));
            entities[entityCount++] = CreateSpellEntity(rockSpell, spawn, vel);
        }

        // --- 3. INVENTORY INPUT (1-5) ---
        for (int i = 0; i < INVENTORY_CAPACITY; i++) {
            if (IsKeyPressed(KEY_ONE + i) || IsKeyPressed(KEY_KP_1 + i)) {
                if (i < inventory.count) {
                    if (inventory.selectedSlot == i) inventory.selectedSlot = -1;
                    else inventory.selectedSlot = i;
                }
            }
        }

        // --- 4. PICKUP/DROP LOGIC ---
        int hoveredEntityIndex = -1; 
        for (int i = 1; i < entityCount; i++) {
            if (!entities[i].isActive) continue;
            bool mouseOnEntity = CheckCollisionPointCircle(mouseWorld, entities[i].position, entities[i].size);
            bool inRange = Vector2Distance(player->position, entities[i].position) < PICKUP_RANGE;

            if (mouseOnEntity && inRange) {
                hoveredEntityIndex = i; 
                break; 
            }
        }

        if (IsKeyPressed(KEY_E) && hoveredEntityIndex != -1) {
            if (AddItem(&inventory, entities[hoveredEntityIndex])) {
                entities[hoveredEntityIndex] = entities[entityCount - 1];
                entityCount--; 
                hoveredEntityIndex = -1;
            }
        }

        if (IsKeyPressed(KEY_Q) && inventory.selectedSlot != -1) {
            Vector2 dropPos = mouseWorld;
            if (Vector2Distance(player->position, mouseWorld) > PICKUP_RANGE) {
                Vector2 dir = Vector2Normalize(Vector2Subtract(mouseWorld, player->position));
                dropPos = Vector2Add(player->position, Vector2Scale(dir, PICKUP_RANGE));
            }
            Entity droppedItem = DropItem(&inventory, inventory.selectedSlot);
            droppedItem.position = dropPos;
            droppedItem.velocity = (Vector2){0,0}; 
            droppedItem.isActive = true; 
            if (entityCount < MAX_ENTITIES) entities[entityCount++] = droppedItem;
        }

        // --- 5. PHYSICS UPDATES ---
        UpdateEntityPhysics(player, playerInput, walls, WALL_COUNT);

        for(int i=1; i<entityCount; i++) {
            if(entities[i].isActive) {
                UpdateEntityPhysics(&entities[i], (Vector2){0,0}, walls, WALL_COUNT);
            }
        }

        ResolveEntityCollisions(entities, entityCount, &particleSystem);
        EnforceWallConstraints(entities, entityCount, walls, WALL_COUNT);
        
        // Garbage Collection
        CleanupEntities(entities, &entityCount);
        UpdateParticles(&particleSystem);
        
        if (IsKeyPressed(KEY_TAB)) showStatus = !showStatus; 

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                DrawGame(entities, entityCount, walls, WALL_COUNT);
                DrawParticles(&particleSystem);

                // Highlight hovered
                if (hoveredEntityIndex != -1) {
                    Entity e = entities[hoveredEntityIndex];
                    DrawCircleLines(e.position.x, e.position.y, e.size + 5, YELLOW);
                    DrawText("E", e.position.x - 5, e.position.y - 5, 20, YELLOW);
                }
            EndMode2D(); 

            DrawInventory(&inventory, SCREEN_WIDTH/2 -100, SCREEN_HEIGHT - 80);
            
            // HUD
            DrawText("WASD: Move | F: Fire | R: Rock | E: Pickup | Q: Drop", 10, 10, 20, DARKGRAY);
            DrawText("Shoot Fire (F) at Rock (R) to FUSE them!", 10, 35, 20, MAROON);
            
            DisplayEntityStatus(player, showStatus);

            // Tooltip
            if (hoveredEntityIndex != -1) {
                Vector2 screenPos = GetWorldToScreen2D(entities[hoveredEntityIndex].position, camera);
                DrawEntityTooltip(&entities[hoveredEntityIndex], (int)screenPos.x, (int)screenPos.y);
            }

            DrawFPS(SCREEN_WIDTH - 80, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}