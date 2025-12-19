// HANDLES DRAWING
// It takes the data as "Read Only" (passed by value, not pointer) so it can't accidentally break the physics.

#include "game.h"

void DrawGame(Character p, Rectangle* walls, int wallCount) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw Walls
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    // Draw Player
    DrawText("Use WASD to move.", 10, 10, 20, DARKGRAY);
    DrawCircleV(p.position, p.size, p.color);

    // Debug
    DrawText(TextFormat("Pos: %.0f, %.0f", p.position.x, p.position.y), 10, 60, 20, DARKGREEN);
    DrawFPS(10, 40);

    EndDrawing();
}