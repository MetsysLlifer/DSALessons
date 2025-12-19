/*
ACTS AS THE "MANAGER"
That creates the data and tells the other files what to do.
*/
#include "game.h"

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Top Down Collision");
    SetTargetFPS(60); // 128 is fine as well

    // --- DATA INITIALIZATION ---
    Character player = {{SCREEN_WIDTH/2, SCREEN_HEIGHT/2}, {0,0}, 32.0f, 600.0f, 5.0f, MAROON};

    // --- BASIC WALLS ---
    Rectangle walls[WALL_COUNT] = {
        {200, 150, 400, 50},
        {150, 150, 50, 300},
        {600, 300, 50, 200}
    };

    // --- 1. CAMERA SETUP ---
    Camera2D camera = { 0 };
    camera.target = player.position; // Camera looks at player
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f }; // Player stays in middle of screen
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // --- TOGGLE STATE VARIABLE ---
    bool showStatus = false;

    // --- GAME LOOP ---
while (!WindowShouldClose()) {
    // UPDATE PHYSICS
    UpdatePlayer(&player, walls, WALL_COUNT);
    camera.target = player.position;
    
    // Check for Toggle Input (Tab Key)
    if (IsKeyPressed(KEY_TAB)) {
        showStatus = !showStatus; // Flip true to false, false to true
    }

    // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // A. DRAW WORLD (Everything inside here moves with camera)
            BeginMode2D(camera);
                
                DrawGame(player, walls, WALL_COUNT);
            
            EndMode2D(); // Stop applying camera transform


            // B. DRAW UI (Everything here stays fixed on screen)
            
            DrawSpeedSlider(&player.speed, 10, 200);
            DisplayPlayerStatus(&player, showStatus);

            // Debug
            DrawFPS(SCREEN_WIDTH - 80, 10);

        EndDrawing();
}

    CloseWindow();
    return 0;
}