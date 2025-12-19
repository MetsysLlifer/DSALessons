/*
ACTS AS THE "MANAGER"
That creates the data and tells the other files what to do.
*/
#include "game.h"
#define MAX_ENTITIES 100

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
    // -- INPUT HANDLING (done in Main) ---
    Vector2 playerInput = {0.0f, 0.0f};
    if (IsKeyDown(KEY_A) != IsKeyDown(KEY_D)) playerInput.x = (IsKeyDown(KEY_A)) ? -1 : 1;
    if (IsKeyDown(KEY_W) != IsKeyDown(KEY_S)) playerInput.y = (IsKeyDown(KEY_W)) ? -1 : 1;

    // --- PICKUP LOGIC (Press E) ---
    if (IsKeyPressed(KEY_E)) {
        // Check all entities (Skip index 0 because that is player)
        for (int i = 1; i < entityCount; i++) {
            float dist = Vector2Distance(player->position, entities[i].position);

            // CALCULATE REQUIRED DISTANCE DYNAMICALLY
            // Player Radius + Item Radius + Extra Reach (20 pixels)
            float pickupRange = player->size + entities[i].size + 20.0f;

            // If close enough (pickupRange)
            if (dist < pickupRange) {
                // Try to add to inventory
                if (AddItem(&inventory, entities[i])) {
                    
                    // REMOVE FROM WORLD (Swap with last item)
                    entities[i] = entities[entityCount - 1];
                    entityCount--; 
                    
                    // Decrement i so we don't skip the item we just swapped in
                    i--; 
                }
                break; // Only pick up one item at a time
            }
        }
    }

    // --- DROP LOGIC (Press Q) ---
    if (IsKeyPressed(KEY_Q) && inventory.count > 0) {
        // Take item out of inventory (Drop the last one picked up)
        Entity droppedItem = DropItem(&inventory, inventory.count - 1);
        
        // Set position to Player's position (plus a little offset so we don't get stuck)
        droppedItem.position = Vector2Add(player->position, (Vector2){40, 0});
        droppedItem.velocity = (Vector2){0,0}; // Reset speed

        // Add back to world
        if (entityCount < MAX_ENTITIES) {
            entities[entityCount] = droppedItem;
            entityCount++;
        }
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