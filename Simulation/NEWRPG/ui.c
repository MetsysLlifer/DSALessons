#include "game.h"

#define WHEEL_RADIUS 60.0f
#define ITEM_SIZE 20.0f 

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
    int width = 230;
    int height = 200; // Increased height
    
    DrawRectangle(x + 20, y - 20, width, height, Fade(BLACK, 0.9f));
    DrawRectangleLines(x + 20, y - 20, width, height, WHITE);
    
    int tx = x+30; int ty = y-10;

    if (e->state == STATE_RAW) {
        DrawText("Raw Element", tx, ty, 10, LIGHTGRAY);
        DrawText(TextFormat("Core: %s", ElementNames[e->spellData.core]), tx, ty+20, 10, GetElementColor(e->spellData.core));
        DrawText("Right Click to Fuse", tx, ty+40, 10, YELLOW);
    } 
    else if (e->state == STATE_PROJECTILE) {
        DrawText(e->spellData.name, tx, ty, 10, YELLOW);
        DrawText(TextFormat("Core: %s", ElementNames[e->spellData.core]), tx, ty+20, 10, GetElementColor(e->spellData.core));
        DrawText(TextFormat("Aux: %d", e->spellData.auxCount), tx, ty+35, 10, GRAY);
        
        // HIDDEN POTENTIALS
        ty += 55;
        if(e->spellData.isTeleport) DrawText("[!] Teleport Enabled", tx, ty, 10, PURPLE);
        if(e->spellData.hasHoming) DrawText("[!] Homing AI", tx, ty+=15, 10, SKYBLUE);
        if(e->spellData.behavior == SPELL_WALL) DrawText("[!] Wall Creator", tx, ty+=15, 10, ORANGE);
        if(e->spellData.behavior == SPELL_HEAL) DrawText("[!] Healing Property", tx, ty+=15, 10, GREEN);
        if(e->spellData.behavior == SPELL_MIDAS) DrawText("[!] Midas Touch", tx, ty+=15, 10, GOLD);
        if(e->spellData.behavior == SPELL_VOID) DrawText("[!] Void Well", tx, ty+=15, 10, MAGENTA);
        if(e->spellData.behavior == SPELL_SLOW) DrawText("[!] Time Slow", tx, ty+=15, 10, LIGHTGRAY);
        if(e->spellData.behavior == SPELL_TELEKINESIS) DrawText("[!] Gravity Well", tx, ty+=15, 10, WHITE);
        
        ty += 25;
        DrawText(TextFormat("Temp: %.1f C", e->spellData.temperature), tx, ty, 10, (e->spellData.temperature>50)?RED:BLUE);
        DrawText(TextFormat("Dry: %.2f", e->spellData.dryness), tx, ty+10, 10, ORANGE);
    }
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
        Rectangle btn = { positions[i].x - ITEM_SIZE, positions[i].y - ITEM_SIZE, ITEM_SIZE*2, ITEM_SIZE*2 };
        bool hover = CheckCollisionPointRec(mousePos, btn);
        
        if (hover) {
            player->selectedElement = types[i];
            DrawRectangleRec(btn, WHITE); 
        } else if (player->selectedElement == types[i]) {
            DrawRectangleLinesEx(btn, 3, GOLD); 
        }
        DrawRectangle(positions[i].x - ITEM_SIZE + 2, positions[i].y - ITEM_SIZE + 2, ITEM_SIZE*2 - 4, ITEM_SIZE*2 - 4, GetElementColor(types[i]));
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
            DrawRectangle(slot.x + 10, slot.y + 10, 20, 20, inv->items[i].color);
        }
        DrawText(TextFormat("%d", i+1), slot.x + 2, slot.y + 2, 10, GRAY);
    }
}