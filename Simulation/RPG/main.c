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

    Rectangle walls[WALL_COUNT] = {
        {200, 150, 400, 50},
        {150, 150, 50, 300},
        {600, 300, 50, 200}
    };

    // --- GAME LOOP ---
    while (!WindowShouldClose()) {
        // Pass the ADDRESS (&) of player to physics so it can change the values
        UpdatePlayer(&player, walls, WALL_COUNT);
        
        // Pass the VALUE of player to graphics just to look at it
        DrawGame(player, walls, WALL_COUNT);
        updateUI();
    }

    CloseWindow();
    return 0;
}