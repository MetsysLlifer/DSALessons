/*
ACTS AS THE "MANAGER"
That creates the data and tells the other files what to do.
*/
#include "game.h"
#define MAX_ENTITIES 100
#define PICKUP_RANGE 100.0f // How far you can reach

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Top Down Collision");
    SetTargetFPS(60); // 128 is fine as well

    // --- DATA INITIALIZATION ---
    // We create an array of entities as the "Source of Truth"
    Entity entities[MAX_ENTITIES];
    int entityCount = 0;

    // Initialize Player (Index 0)
    entities[entityCount++] = (Entity){
        .position = {100, 100}, .velocity = {0,0}, 
        .mass = 1.0f, .friction = 500.0f, .size = 32.0f, 
        .maxSpeed = 600.0f, .moveForce = 3000.0f, 
        .color = MAROON
    };

    // Initialize Rock (Index 1)
    // entities[1] = (Entity){
    //     .position = {400, 400}, .velocity = {0,0}, 
    //     .mass = 2.0f, .friction = 100.0f, .size = 40.0f,
    //     .maxSpeed = 0.0f, .moveForce = 0.0f, 
    //     .color = DARKGRAY
    // };
    // 2. Add some Rocks
    entities[entityCount++] = (Entity){ {300, 300}, {0,0}, 20.0f, 5.0f, 25.0f, 0, 0, DARKGRAY };
    entities[entityCount++] = (Entity){ {500, 200}, {0,0}, 10.0f, 5.0f, 15.0f, 0, 0, BLUE };
    entities[entityCount++] = (Entity){ {400, 500}, {0,0}, 50.0f, 5.0f, 35.0f, 0, 0, GOLD };

    // Create handy pointers so you don't have to type entities[0] everywhere
    Entity* player = &entities[0];

    // --- INVENTORY SETUP ---
    Inventory inventory;
    InitInventory(&inventory);

    // --- BASIC WALLS ---
    Rectangle walls[WALL_COUNT] = {
        {200, 150, 400, 50},
        {150, 150, 50, 300},
        {600, 300, 50, 200}
    };

    // --- 1. CAMERA SETUP ---
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f }; // Player stays in middle of screen
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // --- TOGGLE STATE VARIABLE ---
    bool showStatus = false;

    // --- GAME LOOP ---
    while (!WindowShouldClose()) {
        // --- MOUSE CALCULATION ---
        // Vital: Convert Screen Mouse (pixels) to World Mouse (game coordinates)
        Vector2 mouseScreen = GetMousePosition();
        Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);

        // -- INPUT HANDLING (done in Main) ---

        // A. Movement
        Vector2 playerInput = {0.0f, 0.0f};
        if (IsKeyDown(KEY_A) != IsKeyDown(KEY_D)) playerInput.x = (IsKeyDown(KEY_A)) ? -1 : 1;
        if (IsKeyDown(KEY_W) != IsKeyDown(KEY_S)) playerInput.y = (IsKeyDown(KEY_W)) ? -1 : 1;

        // B. Inventory Hotkeys (1-5)
        // We loop through the capacity. KEY_ONE + 0 = '1', KEY_ONE + 1 = '2', etc.
        for (int i = 0; i < INVENTORY_CAPACITY; i++) {
            if (IsKeyPressed(KEY_ONE + i)) {
                // Only select if the slot actually has an item
                if (i < inventory.count) {
                    // Toggle Logic: If already selected, deselect. Else select.
                    if (inventory.selectedSlot == i) {
                        inventory.selectedSlot = -1;
                    } else {
                        inventory.selectedSlot = i;
                    }
                }
            }
        }

        // --- HOVER LOGIC ---
        int hoveredEntityIndex = -1; // -1 means nothing hovered

        // Check only World Objects (Skip Player at index 0)
        for (int i = 1; i < entityCount; i++) {
            // A. Is Mouse touching this entity?
            bool mouseOnEntity = CheckCollisionPointCircle(mouseWorld, entities[i].position, entities[i].size);
            
            // B. Is Player close enough to reach it?
            float distToPlayer = Vector2Distance(player->position, entities[i].position);
            bool inRange = distToPlayer < PICKUP_RANGE;

            if (mouseOnEntity && inRange) {
                hoveredEntityIndex = i; // Found one!
                break; // Stop looking, we found the one we are hovering
            }
        }

        // --- PICKUP LOGIC (Press E) ---
        // Interaction: Press E to pickup THIS specific one
        if (IsKeyPressed(KEY_E) && hoveredEntityIndex != -1) {
            // Try to add to inventory
            if (AddItem(&inventory, entities[hoveredEntityIndex])) {
                
                // REMOVE FROM WORLD (Swap with last item)
                entities[hoveredEntityIndex] = entities[entityCount - 1];
                entityCount--; 
                
                // Reset hover since item is gone
                hoveredEntityIndex = -1;
            }
        }

        // --- DROP LOGIC (Mouse Aim) ---
        if (IsKeyPressed(KEY_Q) && inventory.selectedSlot != -1) {
            // Calculate Drop Position
            Vector2 dropPos = mouseWorld;
            
            // Clamp Drop Position to Range
            // If mouse is too far, drop it at the max reach limit
            float distToMouse = Vector2Distance(player->position, mouseWorld);
            
            if (distToMouse > PICKUP_RANGE) {
                // Math: Normalize vector to mouse, then multiply by range
                Vector2 dir = Vector2Subtract(mouseWorld, player->position);
                dir = Vector2Normalize(dir);
                dropPos = Vector2Add(player->position, Vector2Scale(dir, PICKUP_RANGE));
            }

            // DROP THE SELECTED ITEM
            Entity droppedItem = DropItem(&inventory, inventory.selectedSlot);
            droppedItem.position = dropPos;
            droppedItem.velocity = (Vector2){0,0}; // Reset speed

            // Add back to world
            if (entityCount < MAX_ENTITIES) {
                entities[entityCount] = droppedItem;
                entityCount++;
            }

            // Unselect after dropping so we don't accidentally drop the next item
            inventory.selectedSlot = -1;
        }

        // UPDATE PHYSICS
        UpdateEntityPhysics(player, playerInput, walls, WALL_COUNT);

        // Update all other entities
        for(int i=1; i<entityCount; i++) {
            UpdateEntityPhysics(&entities[i], (Vector2){0,0}, walls, WALL_COUNT);
        }

        // Solve Entity vs Entity Collisions
        // We pass the whole array so it can check everyone against everyone
        ResolveEntityCollisions(entities, entityCount);
        // Resolve Wall overlap (If Rock was pushed into Wall, push it back out)
        EnforceWallConstraints(entities, entityCount, walls, WALL_COUNT);
        
        camera.target = player->position;
        // Check for Toggle Input (Tab Key)
        if (IsKeyPressed(KEY_TAB)) showStatus = !showStatus; // Flip true to false, false to true

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // A. DRAW WORLD (Everything inside here moves with camera)
            BeginMode2D(camera);
                DrawGame(entities, entityCount, walls, WALL_COUNT);

                // Highlight the object when it is hovered
                if (hoveredEntityIndex != -1) {
                    Entity e = entities[hoveredEntityIndex];
                    DrawCircleLines(e.position.x, e.position.y, e.size + 5, YELLOW);
                    DrawText("E", e.position.x - 5, e.position.y - 5, 20, YELLOW);
                }

            EndMode2D(); // Stop applying camera transform

            // B. DRAW UI (Everything here stays fixed on screen)
            DrawInventory(&inventory, SCREEN_WIDTH/2 -100, SCREEN_HEIGHT - 80);
            
            DisplayEntityStatus(player, showStatus);

            // Debug
            DrawFPS(SCREEN_WIDTH - 80, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}