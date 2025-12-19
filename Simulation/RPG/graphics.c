// HANDLES DRAWING
// It takes the data as "Read Only" (passed by value, not pointer) so it can't accidentally break the physics.

#include "game.h"

void DrawGame(Character player, Rectangle* walls, int wallCount) {
    // Draw Walls
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    // Draw Player
    DrawCircleV(player.position, player.size, player.color);
    
    // Debug
    // DrawText("Use WASD to move.", 10, 10, 20, DARKGRAY);
    // DrawText(TextFormat("Pos: %.0f, %.0f", player.position.x, player.position.y), 10, 60, 20, DARKGREEN);
    // DrawFPS(10, 40);
}