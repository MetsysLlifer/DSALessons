/*
ACTS AS THE "MANAGER"
That creates the data and tells the other files what to do.
*/
#include "game.h"
#define ENTITY_COUNT 2

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Top Down Collision");
    SetTargetFPS(60); // 128 is fine as well

    // --- DATA INITIALIZATION ---
    // We create an array of entities as the "Source of Truth"
    Entity entities[ENTITY_COUNT];

    // Initialize Player (Index 0)
    entities[0] = (Entity){
        .position = {100, 100}, .velocity = {0,0}, 
        .mass = 1.0f, .friction = 500.0f, .size = 32.0f, 
        .maxSpeed = 600.0f, .moveForce = 3000.0f, 
        .color = MAROON
    };

    // Initialize Rock (Index 1)
    entities[1] = (Entity){
        .position = {400, 400}, .velocity = {0,0}, 
        .mass = 2.0f, .friction = 100.0f, .size = 40.0f,
        .maxSpeed = 0.0f, .moveForce = 0.0f, 
        .color = DARKGRAY
    };

    // Create handy pointers so you don't have to type entities[0] everywhere
    Entity* player = &entities[0];
    Entity* rock = &entities[1];

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

    // UPDATE PHYSICS
    UpdateEntityPhysics(player, playerInput, walls, WALL_COUNT);
    // TESTING
    // Rock has no input (pass 0,0), but physics still runs (friction/collision)
    UpdateEntityPhysics(rock, (Vector2){0,0}, walls, WALL_COUNT);


    // Solve Entity vs Entity Collisions
    // We pass the whole array so it can check everyone against everyone
    ResolveEntityCollisions(entities, ENTITY_COUNT);
    // Resolve Wall overlap (If Rock was pushed into Wall, push it back out)
    EnforceWallConstraints(entities, ENTITY_COUNT, walls, WALL_COUNT);
    
    camera.target = player->position;
    // Check for Toggle Input (Tab Key)
    if (IsKeyPressed(KEY_TAB)) showStatus = !showStatus; // Flip true to false, false to true

    // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // A. DRAW WORLD (Everything inside here moves with camera)
            BeginMode2D(camera);
                DrawGame(entities, ENTITY_COUNT, walls, WALL_COUNT);
            EndMode2D(); // Stop applying camera transform

            // B. DRAW UI (Everything here stays fixed on screen)

            DisplayEntityStatus(player, showStatus);

            // Debug
            DrawFPS(SCREEN_WIDTH - 80, 10);

        EndDrawing();
}

    CloseWindow();
    return 0;
}