#include "game.h"

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount) {
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    for (int i = 0; i < count; i++) {
        if (entities[i].isActive) {
            
            if (entities[i].state == STATE_RAW) {
                DrawCircleV(entities[i].position, entities[i].size, entities[i].color);
                DrawCircleLines(entities[i].position.x, entities[i].position.y, entities[i].size, BLACK);
            } 
            else if (entities[i].state == STATE_PROJECTILE) {
                DrawCircleV(entities[i].position, entities[i].size, entities[i].color);
                DrawCircleLines(entities[i].position.x, entities[i].position.y, entities[i].size + 2, WHITE);
                
                for(int j=0; j < entities[i].spellData.auxCount; j++) {
                    float angle = (GetTime()*5) + (j * (PI*2/entities[i].spellData.auxCount));
                    Vector2 offset = { cosf(angle)*20, sinf(angle)*20 };
                    Vector2 orbPos = Vector2Add(entities[i].position, offset);
                    DrawLineEx(entities[i].position, orbPos, 1.0f, Fade(BLACK, 0.5f));
                    DrawCircleV(orbPos, 5, GetElementColor(entities[i].spellData.aux[j]));
                }
            }
            else if (entities[i].state == STATE_STATIC_WALL) {
                DrawRectangle(entities[i].position.x-20, entities[i].position.y-20, 40, 40, BROWN);
            }
        }
    }
}