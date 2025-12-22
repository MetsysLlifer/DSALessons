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

        // Use Shared Geometry Logic for Perfect Sync
        int chunks = GetSpellChunkCount(e);
        
        for(int k=0; k<chunks; k++) {
            Vector2 pos = GetSpellChunkPos(e, k, time);
            float size = GetSpellChunkSize(e, k);
            
            // Draw Based on Behavior
            if (e->spellData.behavior == SPELL_VOID) {
                DrawCircleV(pos, size + sinf(time*10)*2, BLACK);
                DrawCircleLines(pos.x, pos.y, size + 5, PURPLE);
            }
            else if (e->spellData.behavior == SPELL_CHAIN_LIGHTNING) {
                 DrawLineEx(pos, Vector2Add(pos, (Vector2){(float)GetRandomValue(-20,20), (float)GetRandomValue(-20,20)}), 2.0f, YELLOW);
                 DrawCircleV(pos, size, YELLOW);
            }
            else if (e->spellData.behavior == SPELL_PHANTOM) {
                 DrawRectangle(pos.x - size, pos.y - size, size*2, size*2, Fade(WHITE, 0.3f));
            }
            else if (e->spellData.core == ELEM_EARTH || e->state == STATE_STATIC_WALL) {
                DrawRectangle(pos.x - size, pos.y - size, size*2, size*2, (e->state==STATE_STATIC_WALL)?e->color:DARKBROWN);
                DrawRectangleLines(pos.x - size, pos.y - size, size*2, size*2, BLACK);
            }
            else if (e->spellData.core == ELEM_FIRE) {
                DrawRectangle(pos.x - size, pos.y - size, size*2, size*2, RED);
            }
            else if (e->spellData.core == ELEM_WATER) {
                 DrawCircleV(pos, size, BLUE);
            }
            else {
                 DrawRectangle(pos.x - size, pos.y - size, size*2, size*2, e->color);
            }
        }
        
        // Aux
        if (e->state == STATE_PROJECTILE) {
            for(int j=0; j < e->spellData.auxCount; j++) {
                float angle = (time * 3.0f) + (j * (PI * 2 / e->spellData.auxCount));
                Vector2 offset = { cosf(angle)*30.0f, sinf(angle)*30.0f };
                Vector2 orbPos = Vector2Add(e->position, offset);
                DrawLineEx(e->position, orbPos, 1.0f, Fade(BLACK, 0.5f));
                DrawRectangle(orbPos.x-4, orbPos.y-4, 8, 8, GetElementColor(e->spellData.aux[j]));
            }
        }
    }
}