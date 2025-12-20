#include "game.h"

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount) {
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    for (int i = 0; i < count; i++) {
        if (entities[i].isActive) {
            DrawCircleV(entities[i].position, entities[i].size, entities[i].color);
            if(entities[i].isSpell) {
                DrawText("S", entities[i].position.x - 5, entities[i].position.y - 5, 10, WHITE);
            }
        }
    }
}