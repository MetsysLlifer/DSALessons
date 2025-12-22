#include "game.h"
#include <math.h>

// Helper to create deterministic "random" offsets based on ID and Index
// This ensures the scattered pieces stay together and don't jitter uncontrollably
float PsuedoRandom(int entityID, int index, float range) {
    return sinf(entityID * 99.0f + index * 13.0f) * range;
}

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount) {
    float time = GetTime();

    // 1. Draw Walls
    for (int i = 0; i < wallCount; i++) {
        DrawRectangleRec(walls[i], GRAY);
        DrawRectangleLinesEx(walls[i], 2, DARKGRAY);
    }

    // 2. Draw Entities
    for (int i = 0; i < count; i++) {
        Entity* e = &entities[i];
        if (!e->isActive) continue;

        // Use the memory address or index as a seed for stable "randomness"
        int seed = i; 

        // --- A. RAW ELEMENT (Item on Floor) ---
        if (e->state == STATE_RAW) {
            // Draw as a small pile of 3 pieces
            for(int k=0; k<3; k++) {
                float offX = PsuedoRandom(seed, k, 8.0f);
                float offY = PsuedoRandom(seed, k+10, 8.0f);
                float r = e->size * 0.6f;
                DrawRectangle(e->position.x + offX - r/2, e->position.y + offY - r/2, r, r, e->color);
                DrawRectangleLines(e->position.x + offX - r/2, e->position.y + offY - r/2, r, r, BLACK);
            }
        } 
        
        // --- B. ACTIVE SPELL (Scattered Cluster) ---
        else if (e->state == STATE_PROJECTILE) {
            
            // 1. EARTH: DEBRIS FIELD
            if (e->spellData.core == ELEM_EARTH || e->spellData.behavior == SPELL_MIDAS) {
                int chunks = 8;
                for(int k=0; k<chunks; k++) {
                    // Orbiting heavy chunks
                    float angle = (time * 1.0f) + (k * (PI*2/chunks));
                    float dist = e->size * 0.8f + PsuedoRandom(seed, k, 5.0f);
                    
                    float offX = cosf(angle) * dist;
                    float offY = sinf(angle) * dist;
                    float pieceSize = e->size * 0.5f; // Smaller pieces
                    
                    Color c = (e->spellData.behavior == SPELL_MIDAS) ? GOLD : DARKBROWN;
                    
                    DrawRectangle(e->position.x + offX - pieceSize/2, e->position.y + offY - pieceSize/2, pieceSize, pieceSize, c);
                    DrawRectangleLines(e->position.x + offX - pieceSize/2, e->position.y + offY - pieceSize/2, pieceSize, pieceSize, BLACK);
                }
            }
            
            // 2. FIRE: CHAOTIC SWARM
            else if (e->spellData.core == ELEM_FIRE) {
                int sparks = 15;
                for(int k=0; k<sparks; k++) {
                    // Jittery movement
                    float jitterX = PsuedoRandom(seed + (int)(time*10), k, 15.0f);
                    float jitterY = PsuedoRandom(seed + (int)(time*10), k+50, 15.0f);
                    
                    Vector2 pos = Vector2Add(e->position, (Vector2){jitterX, jitterY});
                    
                    // Triangle Shape
                    Vector2 top = { pos.x, pos.y - 6 };
                    Vector2 left = { pos.x - 4, pos.y + 4 };
                    Vector2 right = { pos.x + 4, pos.y + 4 };
                    
                    Color c = (k%2==0) ? RED : ORANGE;
                    DrawTriangle(top, left, right, c);
                }
            }
            
            // 3. WATER: FLUID SPRAY
            else if (e->spellData.core == ELEM_WATER) {
                int drops = 10;
                for(int k=0; k<drops; k++) {
                    // Trail behind velocity
                    Vector2 trail = Vector2Scale(Vector2Normalize(e->velocity), -1.0f);
                    float dist = k * 4.0f;
                    
                    // Wavy trail
                    float wave = sinf(time * 15.0f + k) * 6.0f;
                    Vector2 perp = { -trail.y, trail.x }; // Perpendicular vector
                    
                    Vector2 pos = Vector2Add(e->position, Vector2Scale(trail, dist));
                    pos = Vector2Add(pos, Vector2Scale(perp, wave));
                    
                    float size = (e->size * 0.6f) * (1.0f - (float)k/drops);
                    DrawCircleV(pos, size, Fade(BLUE, 0.7f));
                }
            }
            
            // 4. AIR / VOID: SWIRLING VORTEX
            else if (e->spellData.core == ELEM_AIR || e->spellData.behavior == SPELL_VOID) {
                int particles = 12;
                Color baseCol = (e->spellData.behavior == SPELL_VOID) ? PURPLE : SKYBLUE;
                
                for(int k=0; k<particles; k++) {
                    // Fast spin
                    float angle = (time * 8.0f) + (k * (PI*2/particles));
                    // Spiraling in and out
                    float radius = e->size + sinf(time * 5.0f + k) * 5.0f;
                    
                    Vector2 off = { cosf(angle) * radius, sinf(angle) * radius };
                    Vector2 pos = Vector2Add(e->position, off);
                    
                    DrawCircleV(pos, 3.0f, baseCol);
                    if(e->spellData.behavior == SPELL_VOID) DrawCircleLines(pos.x, pos.y, 4.0f, BLACK);
                }
            }
            
            // 5. SPECIAL: PHANTOM / CHAIN LIGHTNING
            if (e->spellData.behavior == SPELL_CHAIN_LIGHTNING) {
                 // Arcs of electricity
                 for(int k=0; k<3; k++) {
                     Vector2 start = e->position;
                     Vector2 end = Vector2Add(e->position, (Vector2){PsuedoRandom(seed, k, 30), PsuedoRandom(seed, k+5, 30)});
                     DrawLineEx(start, end, 2.0f, YELLOW);
                 }
            }
            
            // --- DRAW AUXILIARY CONNECTIONS ---
            // These connect the scattered pieces to the "Core" logic
            for(int j=0; j < e->spellData.auxCount; j++) {
                float angle = (time * 2.0f) + (j * (PI * 2 / e->spellData.auxCount));
                Vector2 offset = { cosf(angle)*35.0f, sinf(angle)*35.0f };
                Vector2 orbPos = Vector2Add(e->position, offset);
                
                // Draw a faint line connecting the center to the aux
                DrawLineEx(e->position, orbPos, 1.0f, Fade(BLACK, 0.3f));
                
                // Draw the Aux element as a small spinning square
                DrawRectanglePro((Rectangle){orbPos.x, orbPos.y, 10, 10}, (Vector2){5,5}, time*100.0f, GetElementColor(e->spellData.aux[j]));
            }
        }
        
        // --- C. STATIC WALL ---
        else if (e->state == STATE_STATIC_WALL) {
            // Draw as a solid block of bricks
            DrawRectangle(e->position.x-20, e->position.y-20, 40, 40, e->color);
            DrawRectangleLines(e->position.x-20, e->position.y-20, 40, 40, BLACK);
            // Detail lines
            DrawLine(e->position.x-20, e->position.y, e->position.x+20, e->position.y, BLACK);
            DrawLine(e->position.x, e->position.y-20, e->position.x, e->position.y+20, BLACK);
        }
    }
}