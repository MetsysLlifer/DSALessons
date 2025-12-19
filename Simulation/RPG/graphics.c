// HANDLES DRAWING
// It takes the data as "Read Only" (passed by value, not pointer) so it can't accidentally break the physics.

#include "game.h"

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount) {
    // Walls
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    // Entities
    for (int i = 0; i < count; i++) {
        // NEW: Check if active
        if (entities[i].isActive) {
            DrawCircleV(entities[i].position, entities[i].size, entities[i].color);
            
            // Debug: Draw "S" on spells
            if(entities[i].isSpell) {
                DrawText("S", entities[i].position.x - 5, entities[i].position.y - 5, 10, WHITE);
            }
        }
    }
}