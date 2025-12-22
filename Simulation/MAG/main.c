#include "game.h"
#define MAX_ENTITIES 200
#define PICKUP_RANGE 100.0f 

void CleanupEntities(Entity* entities, int* count) {
    for (int i = 1; i < *count; i++) {
        if (!entities[i].isActive) {
            entities[i] = entities[*count - 1]; (*count)--; i--; 
        }
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Merged System");
    SetTargetFPS(60); 

    Entity entities[MAX_ENTITIES];
    int entityCount = 0;

    // Player
    entities[entityCount++] = (Entity){
        .position = {100, 100}, .velocity = {0,0}, 
        .mass = 1.0f, .friction = 10.0f, .size = 30.0f, 
        .speed = 600.0f, .steering_factor = 3000.0f,
        .color = MAROON, .isActive = true, .state = STATE_RAW, .health = 100, .maxHealth = 100
    };
    Entity* player = &entities[0];
    Player playerData = { .selectedElement = ELEM_EARTH, .mana = 100, .maxMana = 100 };

    Inventory inventory = { 0 }; InitInventory(&inventory);
    ParticleSystem particleSystem; InitParticles(&particleSystem);
    Rectangle walls[WALL_COUNT] = { {200, 150, 400, 50}, {150, 150, 50, 300}, {600, 300, 50, 200} };
    Camera2D camera = { 0 }; camera.zoom = 1.0f; camera.offset = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

    while (!WindowShouldClose()) {
        Vector2 mouseScreen = GetMousePosition();
        Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
        camera.target = player->position;

        bool isSelecting = IsKeyDown(KEY_TAB);
        
        // 1. PLACE ELEMENT (Left Click)
        if (!isSelecting && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && entityCount < MAX_ENTITIES) {
            if (playerData.selectedElement != ELEM_NONE) {
                entities[entityCount++] = CreateRawElement(playerData.selectedElement, mouseWorld);
            }
        }

        // 2. FUSE (Right Click)
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            for (int i = 1; i < entityCount; i++) {
                if (entities[i].state == STATE_RAW && CheckCollisionPointCircle(mouseWorld, entities[i].position, entities[i].size)) {
                    PerformSpatialFusion(entities, entityCount, i, &particleSystem);
                    Vector2 dir = Vector2Normalize(Vector2Subtract(mouseWorld, player->position)); 
                    entities[i].velocity = Vector2Scale(dir, 800.0f);
                    break; 
                }
            }
        }

        // 3. INVENTORY & INTERACTION
        int hovered = -1;
        for (int i = 1; i < entityCount; i++) {
            if (!entities[i].isActive || entities[i].isHeld) continue;
            if (CheckCollisionPointCircle(mouseWorld, entities[i].position, entities[i].size)) { hovered = i; break; }
        }

        // Select Slot (1-5)
        for (int i = 0; i < INVENTORY_CAPACITY; i++) {
            if (IsKeyPressed(KEY_ONE + i) || IsKeyPressed(KEY_KP_1 + i)) {
                if (i < inventory.count) inventory.selectedSlot = (inventory.selectedSlot == i) ? -1 : i;
            }
        }

        // Pickup (E)
        if (IsKeyPressed(KEY_E) && hovered != -1) {
            if (AddItem(&inventory, entities[hovered])) {
                entities[hovered] = entities[entityCount - 1]; entityCount--; hovered = -1;
            }
        }

        // Drop (Q)
        if (IsKeyPressed(KEY_Q) && inventory.selectedSlot != -1) {
            Entity dropped = DropItem(&inventory, inventory.selectedSlot);
            dropped.position = mouseWorld;
            dropped.isActive = true;
            dropped.velocity = (Vector2){0,0};
            if(entityCount < MAX_ENTITIES) entities[entityCount++] = dropped;
        }

        // --- UPDATE ---
        Vector2 input = {0,0};
        if (IsKeyDown(KEY_W)) input.y -= 1;
        if (IsKeyDown(KEY_S)) input.y += 1;
        if (IsKeyDown(KEY_A)) input.x -= 1;
        if (IsKeyDown(KEY_D)) input.x += 1;
        
        UpdateEntityPhysics(player, input, walls, WALL_COUNT);

        for(int i=1; i<entityCount; i++) {
            if(entities[i].isActive) {
                Vector2 aiInput = {0,0};
                if(entities[i].state == STATE_PROJECTILE && entities[i].spellData.hasHoming) {
                    aiInput = Vector2Normalize(Vector2Subtract(mouseWorld, entities[i].position));
                }
                UpdateEntityPhysics(&entities[i], aiInput, walls, WALL_COUNT);
            }
        }

        ResolveEntityCollisions(entities, entityCount, player, &particleSystem);
        UpdateParticles(&particleSystem);
        CleanupEntities(entities, &entityCount);

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                DrawGame(entities, entityCount, walls, WALL_COUNT);
                DrawParticles(&particleSystem);
                
                // Highlight Interaction
                if (hovered != -1) {
                    DrawCircleLines(entities[hovered].position.x, entities[hovered].position.y, entities[hovered].size+5, YELLOW);
                    DrawText("E", entities[hovered].position.x, entities[hovered].position.y-10, 20, YELLOW);
                }
                
                // Show Fusion Radius
                for(int i=1; i<entityCount; i++) {
                    if(entities[i].state == STATE_RAW && CheckCollisionPointCircle(mouseWorld, entities[i].position, entities[i].size)) {
                        DrawCircleLines(entities[i].position.x, entities[i].position.y, FUSION_RADIUS, Fade(GREEN, 0.5f));
                    }
                }
            EndMode2D();

            DrawInventory(&inventory, SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 80);
            DrawHUD(player, playerData);

            if (isSelecting) DrawElementWheel(&playerData, mouseScreen);
            else {
                DrawCircle(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 40, 20, GetElementColor(playerData.selectedElement));
                DrawText("TAB", SCREEN_WIDTH - 55, SCREEN_HEIGHT - 45, 10, BLACK);
            }
            
            if (hovered != -1) DrawEntityTooltip(&entities[hovered], mouseScreen.x, mouseScreen.y);

            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}