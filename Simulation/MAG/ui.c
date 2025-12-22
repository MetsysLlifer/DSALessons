#include "game.h"

#define WHEEL_RADIUS 60.0f
#define ITEM_RADIUS 20.0f

const char* ElementNames[] = { "None", "Earth", "Water", "Fire", "Air" };

void DrawHUD(Entity* player, Player stats) {
    DrawRectangle(10, 10, 200, 20, DARKGRAY);
    if (player->maxHealth > 0) DrawRectangle(10, 10, (int)(200 * (player->health/player->maxHealth)), 20, RED);
    DrawRectangleLines(10, 10, 200, 20, BLACK);
    DrawText("HEALTH", 15, 12, 10, WHITE);
    
    DrawRectangle(10, 40, 200, 20, DARKGRAY);
    DrawText(TextFormat("MANA: %.0f", stats.mana), 10, 40, 20, BLUE);
}

void DrawEntityTooltip(Entity* e, int x, int y) {
    int width = 220;
    int height = 140; 
    
    DrawRectangle(x + 20, y - 20, width, height, Fade(BLACK, 0.9f));
    DrawRectangleLines(x + 20, y - 20, width, height, WHITE);
    
    int tx = x+30; int ty = y-10;

    if (e->state == STATE_RAW) {
        DrawText("Raw Element", tx, ty, 10, LIGHTGRAY);
        DrawText("Core:", tx, ty+20, 10, WHITE);
        DrawText(ElementNames[e->spellData.core], tx+40, ty+20, 10, GetElementColor(e->spellData.core));
        DrawText("Right Click to Fuse", tx, ty+40, 10, YELLOW);
    } 
    else if (e->state == STATE_PROJECTILE) {
        DrawText(e->spellData.name, tx, ty, 10, YELLOW);
        
        DrawText(TextFormat("Core: %s", ElementNames[e->spellData.core]), tx, ty+20, 10, GetElementColor(e->spellData.core));
        DrawText(TextFormat("Aux: %d", e->spellData.auxCount), tx, ty+35, 10, GRAY);
        
        DrawText(TextFormat("Temp: %.1f C", e->spellData.temperature), tx, ty+55, 10, (e->spellData.temperature>50)?RED:BLUE);
        DrawText(TextFormat("Dry: %.2f", e->spellData.dryness), tx, ty+70, 10, ORANGE);
        DrawText(TextFormat("Inten: %.2f", e->spellData.intensity), tx, ty+85, 10, WHITE);
    }
    
    DrawText(TextFormat("Mass: %.0f", e->mass), tx, ty+110, 10, LIGHTGRAY);
}

void DrawElementWheel(Player* player, Vector2 mousePos) {
    Vector2 center = { SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100 };
    DrawCircleV(center, WHEEL_RADIUS, Fade(BLACK, 0.5f));
    
    Vector2 positions[4];
    ElementType types[4] = { ELEM_EARTH, ELEM_WATER, ELEM_FIRE, ELEM_AIR };
    
    positions[0] = (Vector2){ center.x - 40, center.y }; 
    positions[1] = (Vector2){ center.x + 40, center.y }; 
    positions[2] = (Vector2){ center.x, center.y - 40 }; 
    positions[3] = (Vector2){ center.x, center.y + 40 }; 
    
    for(int i=0; i<4; i++) {
        bool hover = CheckCollisionPointCircle(mousePos, positions[i], ITEM_RADIUS);
        if (hover) {
            player->selectedElement = types[i];
            DrawCircleV(positions[i], ITEM_RADIUS + 5, WHITE); 
        } else if (player->selectedElement == types[i]) {
            DrawCircleV(positions[i], ITEM_RADIUS + 3, GOLD); 
        }
        DrawCircleV(positions[i], ITEM_RADIUS, GetElementColor(types[i]));
    }
}

void DrawInventory(Inventory* inv, int x, int y) {
    int slotSize = 40;
    DrawText("INVENTORY (1-5)", x, y - 20, 10, DARKGRAY);
    for (int i = 0; i < INVENTORY_CAPACITY; i++) {
        Rectangle slot = { x + (i * 45), y, slotSize, slotSize };
        
        bool isSelected = (inv->selectedSlot == i);
        DrawRectangleRec(slot, Fade(LIGHTGRAY, 0.5f));
        DrawRectangleLinesEx(slot, isSelected ? 3 : 2, isSelected ? GREEN : DARKGRAY);
        
        if (i < inv->count) {
            DrawCircle(slot.x + 20, slot.y + 20, 10, inv->items[i].color);
        }
        DrawText(TextFormat("%d", i+1), slot.x + 2, slot.y + 2, 10, GRAY);
    }
}

void DisplayEntityStatus(Entity *e, bool isVisible) {}