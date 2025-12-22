#include "game.h"
#include <math.h>

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount) {
    float time = GetTime();

    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    for (int i = 0; i < count; i++) {
        Entity* e = &entities[i];
        if (!e->isActive) continue;

        if (e->state == STATE_RAW) {
            float r = e->size;
            DrawRectangle(e->position.x - r, e->position.y - r, r*2, r*2, e->color);
            DrawRectangleLines(e->position.x - r, e->position.y - r, r*2, r*2, BLACK);
        } 
        else if (e->state == STATE_PROJECTILE) {
            
            // VOID / BLACK HOLE Visual
            if (e->spellData.behavior == SPELL_VOID) {
                DrawCircleV(e->position, e->size + sinf(time*10)*2, BLACK);
                DrawCircleLines(e->position.x, e->position.y, e->size + 5, PURPLE);
            }
            // MIDAS / GOLD Visual
            else if (e->spellData.behavior == SPELL_MIDAS) {
                DrawRectangle(e->position.x - e->size, e->position.y - e->size, e->size*2, e->size*2, GOLD);
                DrawRectangleLines(e->position.x - e->size, e->position.y - e->size, e->size*2, e->size*2, YELLOW);
            }
            // STANDARD (Earth/Fire/Water/Air) Visuals from previous turn
            else if (e->spellData.core == ELEM_EARTH) {
                int rockCount = 10;
                for(int k=0; k<rockCount; k++) {
                    float offsetX = sinf(k * 132.0f) * (e->size * 0.8f);
                    float offsetY = cosf(k * 91.0f) * (e->size * 0.8f);
                    float rockSize = e->size * 0.4f;
                    DrawRectangle(e->position.x + offsetX - rockSize/2, e->position.y + offsetY - rockSize/2, rockSize, rockSize, DARKBROWN);
                    DrawRectangleLines(e->position.x + offsetX - rockSize/2, e->position.y + offsetY - rockSize/2, rockSize, rockSize, BLACK);
                }
            }
            else if (e->spellData.core == ELEM_FIRE) {
                int flameCount = 12;
                for(int k=0; k<flameCount; k++) {
                    float jitterX = sinf(time * 10.0f + k) * 5.0f;
                    float rise = fmodf(time * 50.0f + (k * 20.0f), 20.0f);
                    Vector2 top = { e->position.x + jitterX, e->position.y - 10.0f - rise };
                    Vector2 left = { e->position.x - 5.0f + jitterX, e->position.y + 5.0f - rise };
                    Vector2 right = { e->position.x + 5.0f + jitterX, e->position.y + 5.0f - rise };
                    Color flameCol = (k%2==0) ? RED : ORANGE;
                    flameCol.a = (unsigned char)(255 - (rise * 10.0f));
                    DrawTriangle(top, left, right, flameCol);
                }
            }
            else if (e->spellData.core == ELEM_WATER) {
                int droplets = 8;
                for(int k=0; k<droplets; k++) {
                    Vector2 trailPos = Vector2Subtract(e->position, Vector2Scale(Vector2Normalize(e->velocity), k * 5.0f));
                    trailPos.x += sinf(time * 10.0f + k) * 3.0f;
                    float dropSize = e->size * (1.0f - (float)k/droplets);
                    DrawCircleV(trailPos, dropSize, Fade(BLUE, 0.6f));
                }
            }
            else if (e->spellData.core == ELEM_AIR) {
                int particles = 6;
                for(int k=0; k<particles; k++) {
                    float angle = time * 8.0f + (k * (PI*2/particles));
                    Vector2 orb = { cosf(angle) * e->size, sinf(angle) * e->size };
                    DrawCircleV(Vector2Add(e->position, orb), 4.0f, SKYBLUE);
                }
            }
            
            // Draw Auxiliaries
            for(int j=0; j < e->spellData.auxCount; j++) {
                float angle = (time * 3.0f) + (j * (PI * 2 / e->spellData.auxCount));
                Vector2 offset = { cosf(angle)*30.0f, sinf(angle)*30.0f };
                Vector2 orbPos = Vector2Add(e->position, offset);
                DrawLineEx(e->position, orbPos, 1.0f, Fade(BLACK, 0.5f));
                DrawRectangle(orbPos.x-4, orbPos.y-4, 8, 8, GetElementColor(e->spellData.aux[j]));
            }
        }
        else if (e->state == STATE_STATIC_WALL) {
            DrawRectangle(e->position.x-20, e->position.y-20, 40, 40, e->color);
            DrawRectangleLines(e->position.x-20, e->position.y-20, 40, 40, BLACK);
        }
    }
}