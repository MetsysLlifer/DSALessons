// HANDLES DRAWING
// It takes the data as "Read Only" (passed by value, not pointer) so it can't accidentally break the physics.

#include "game.h"

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount) {
    // Draw Walls
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    // Draw All Entities
    for (int i = 0; i < count; i++) {
        DrawCircleV(entities[i].position, entities[i].size, entities[i].color);
    }
}