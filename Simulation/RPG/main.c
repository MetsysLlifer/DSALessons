/*
ACTS AS THE "MANAGER"
That creates the data and tells the other files what to do.
*/
#include "game.h"
#define MAX_ENTITIES 1000
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
        .mass = 1.0f, .friction = 10.0f, .size = 32.0f, 
        .maxSpeed = 600.0f, .moveForce = 3000.0f, 
        .color = MAROON,
        .isSpell = false, .isActive = true
    };

    // --- PARTICLE SYSTEM SETUP ---
    ParticleSystem particleSystem;
    InitParticles(&particleSystem);

    // --- SPELL TEMPLATES ---
    Spell fireSpell = { {"Fire"}, ELEMENT_PLASMA, FORM_PLASMA, 100.0, 0.5, 0.9, true };
    Spell rockSpell = { {"Rock"}, ELEMENT_SOLID, FORM_SOLID, 20.0, 0.8, 1.0, true };

    // Initialize Rock (Index 1) using CreateSpellEntity to make it compatible
    // entities[1] = ... (Old manual init)
    
    // 2. Add some Rocks/Spells
    entities[entityCount++] = CreateSpellEntity(rockSpell, (Vector2){300, 300}, (Vector2){0,0});
    entities[entityCount++] = CreateSpellEntity(fireSpell, (Vector2){500, 200}, (Vector2){0,0});

    // Create handy pointers so you don't have to type entities[0] everywhere
    Entity* player = &entities[0];

    // --- INVENTORY SETUP ---
    Inventory inventory = { 0 };
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
        Vector2 playerInput = {0.0f, 0.0f};
        if (IsKeyDown(KEY_A) != IsKeyDown(KEY_D)) playerInput.x = (IsKeyDown(KEY_A)) ? -1 : 1;
        if (IsKeyDown(KEY_W) != IsKeyDown(KEY_S)) playerInput.y = (IsKeyDown(KEY_W)) ? -1 : 1;

        // --- MAGIC INPUT (Shoot Spells) ---
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

        // --- INVENTORY HOTKEYS (1-5) ---
        for (int i = 0; i < INVENTORY_CAPACITY; i++) {
            // Check Number Row (1-5) AND Numpad (1-5)
            if (IsKeyPressed(KEY_ONE + i) || IsKeyPressed(KEY_KP_1 + i)) {
                
                // Only allow selecting if the slot has an item
                if (i < inventory.count) {
                    // Toggle: If selecting the same slot, unselect it.
                    if (inventory.selectedSlot == i) {
                        inventory.selectedSlot = -1;
                    } else {
                        inventory.selectedSlot = i;
                    }
                }
            }
        }

        // --- PICKUP LOGIC (Mouse Hover) ---
        int hoveredEntityIndex = -1; // -1 means nothing hovered

        // Check only World Objects (Skip Player at index 0)
        for (int i = 1; i < entityCount; i++) {
            // Skip inactive (dead/fused) entities
            if (!entities[i].isActive) continue;

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
            droppedItem.isActive = true; // Make sure it's alive when dropped

            // Add back to world
            if (entityCount < MAX_ENTITIES) {
                entities[entityCount] = droppedItem;
                entityCount++;
            }

            // Unselect after dropping so we don't accidentally drop the next item
            // inventory.selectedSlot = -1;
            // Note: DropItem inside inventory.c now handles resetting selectedSlot to -1 automatically
        }

        // UPDATE PHYSICS
        UpdateEntityPhysics(player, playerInput, walls, WALL_COUNT);

        // Update all other entities
        for(int i=1; i<entityCount; i++) {
            // Check if active so we don't move dead spells
            if(entities[i].isActive) {
                UpdateEntityPhysics(&entities[i], (Vector2){0,0}, walls, WALL_COUNT);
            }
        }

        // UPDATED: Pass the particle system to the collision resolver
        ResolveEntityCollisions(entities, entityCount, &particleSystem);
        // Resolve Wall overlap (If Rock was pushed into Wall, push it back out)
        EnforceWallConstraints(entities, entityCount, walls, WALL_COUNT);

        // NEW: Update Particles
        UpdateParticles(&particleSystem);
        
        camera.target = player->position;
        // Check for Toggle Input (Tab Key)
        if (IsKeyPressed(KEY_TAB)) showStatus = !showStatus; // Flip true to false, false to true

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // A. DRAW WORLD (Everything inside here moves with camera)
            BeginMode2D(camera);
                DrawGame(entities, entityCount, walls, WALL_COUNT);

                // NEW: Draw Particles (Draw them after game entities so they appear on top)
                DrawParticles(&particleSystem);

                // Highlight the object when it is hovered
                if (hoveredEntityIndex != -1) {
                    Entity e = entities[hoveredEntityIndex];
                    DrawCircleLines(e.position.x, e.position.y, e.size + 5, YELLOW);
                    DrawText("E", e.position.x - 5, e.position.y - 5, 20, YELLOW);
                }

            EndMode2D(); // Stop applying camera transform

            // B. DRAW UI (Everything here stays fixed on screen)
            DrawInventory(&inventory, SCREEN_WIDTH/2 -100, SCREEN_HEIGHT - 80);

            DrawText(TextFormat("Inv Count: %d", inventory.count), 10, 100, 20, BLACK);
            DrawText(TextFormat("Selected Slot: %d", inventory.selectedSlot), 10, 130, 20, (inventory.selectedSlot == -1) ? RED : GREEN);
            
            // Instructions for Magic
            DrawText("WASD: Move | F: Fire | R: Rock | E: Pickup | Q: Drop", 10, 10, 20, DARKGRAY);
            DrawText("Shoot Fire (F) at Rock (R) to FUSE them!", 10, 35, 20, MAROON);

            DisplayEntityStatus(player, showStatus);

            // Debug
            DrawFPS(SCREEN_WIDTH - 80, 10);

            // ---------------------------------------------------------
            // NEW: DRAW TOOLTIP (Must be done in UI space)
            // ---------------------------------------------------------
            if (hoveredEntityIndex != -1) {
                // 1. Get the hovered entity
                Entity* e = &entities[hoveredEntityIndex];
                
                // 2. Convert its World Position (Game Map) to Screen Position (Pixel on Monitor)
                Vector2 screenPos = GetWorldToScreen2D(e->position, camera);

                // 3. Draw the tooltip at that screen position
                DrawEntityTooltip(e, (int)screenPos.x, (int)screenPos.y);
            }
            // ---------------------------------------------------------

        EndDrawing();
    }

    CloseWindow();
    return 0;
}