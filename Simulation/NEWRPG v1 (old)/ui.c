#include "game.h"

#define WHEEL_RADIUS 60.0f
#define ITEM_SIZE 20.0f 

const char* ElementNames[] = { "None", "Earth", "Water", "Fire", "Air" };

typedef struct {
    char name[32];
    char desc[64];
    Color col;
} SpellInfo;

SpellInfo GetSpellInfo(SpellBehavior b) {
    switch(b) {
        case SPELL_WALL: return (SpellInfo){"Wall Creator", "Creates solid barriers.", ORANGE};
        case SPELL_MIDAS: return (SpellInfo){"Midas Touch", "Turns targets to gold.", GOLD};
        case SPELL_VOID: return (SpellInfo){"Void Well", "Sucks objects in.", MAGENTA};
        case SPELL_TELEKINESIS: return (SpellInfo){"Gravity Well", "Levitates objects.", LIGHTGRAY};
        case SPELL_HEAL: return (SpellInfo){"Healing Orb", "Restores health.", GREEN};
        case SPELL_SLOW: return (SpellInfo){"Mud Trap", "Slows enemies.", BROWN};
        case SPELL_CHAIN_LIGHTNING: return (SpellInfo){"Chain Lightning", "Arcs damage.", YELLOW};
        case SPELL_FREEZE: return (SpellInfo){"Permafrost", "Freezes targets.", SKYBLUE};
        case SPELL_VAMPIRISM: return (SpellInfo){"Blood Siphon", "Steals life.", RED};
        case SPELL_CLUSTER: return (SpellInfo){"Cluster Bomb", "Explosive shards.", RED};
        case SPELL_REFLECT: return (SpellInfo){"Mirror Shield", "Reflects attacks.", WHITE};
        case SPELL_PHANTOM: return (SpellInfo){"Phase Shift", "Passes through walls.", PURPLE};
        case SPELL_CONFUSE: return (SpellInfo){"Confuse Ray", "Reverses controls.", PINK};
        case SPELL_BERSERK: return (SpellInfo){"Berserker", "Moves fast & wild.", MAROON};
        case SPELL_NECROMANCY: return (SpellInfo){"Necromancy", "Raises objects.", DARKGRAY};
        case SPELL_TSUNAMI: return (SpellInfo){"Tsunami", "Massive wave.", BLUE};
        case SPELL_WHIRLWIND: return (SpellInfo){"Whirlwind", "Pushes away.", LIGHTGRAY};
        case SPELL_MAGNET: return (SpellInfo){"Magnetism", "Attracts Earth.", GRAY};
        case SPELL_PETRIFY: return (SpellInfo){"Petrification", " turns to stone.", DARKGRAY};
        case SPELL_MIRROR: return (SpellInfo){"Doppelganger", "Clone.", SKYBLUE};
        case SPELL_REWIND: return (SpellInfo){"Time Rewind", "Resets position.", GOLD};
        case SPELL_POISON: return (SpellInfo){"Toxic Cloud", "Poison area.", LIME};
        case SPELL_SWARM: return (SpellInfo){"Insect Swarm", "Homing swarm.", BROWN};
        case SPELL_SNIPER: return (SpellInfo){"Railgun", "Fast shot.", RED};
        case SPELL_BOUNCE: return (SpellInfo){"Bouncer", "Bounces walls.", ORANGE};
        case SPELL_LANDMINE: return (SpellInfo){"Landmine", "Trap.", GRAY};
        case SPELL_GROWTH: return (SpellInfo){"Growth Ray", "Enlarges.", GREEN};
        case SPELL_SHRINK: return (SpellInfo){"Shrink Ray", "Shrinks.", PURPLE};
        default: return (SpellInfo){"Unknown", "???", GRAY};
    }
}

void DrawHUD(Entity* player, Player* stats) {
    DrawRectangle(10, 10, 200, 20, DARKGRAY);
    if (player->maxHealth > 0) DrawRectangle(10, 10, (int)(200 * (player->health/player->maxHealth)), 20, RED);
    DrawRectangleLines(10, 10, 200, 20, BLACK);
    DrawText("HEALTH", 15, 12, 10, WHITE);
    
    DrawRectangle(10, 40, 200, 20, DARKGRAY);
    DrawText(TextFormat("MANA: %.0f", stats->mana), 10, 40, 20, BLUE);
    
    DrawText("Press [C] for Compendium", 10, 70, 10, BLACK);
    
    if (stats->book.notificationTimer > 0) {
        stats->book.notificationTimer -= GetFrameTime();
        int popupY = SCREEN_HEIGHT / 4;
        int textW = MeasureText(stats->book.notificationText, 30);
        DrawRectangle(SCREEN_WIDTH/2 - textW/2 - 20, popupY - 10, textW + 40, 50, Fade(BLACK, 0.8f));
        DrawRectangleLines(SCREEN_WIDTH/2 - textW/2 - 20, popupY - 10, textW + 40, 50, GOLD);
        DrawText(stats->book.notificationText, SCREEN_WIDTH/2 - textW/2, popupY, 30, GOLD);
    }
}

void DrawCompendium(Player* player) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.85f));
    int cols = 4;
    int cardW = 160;
    int cardH = 60;
    int startX = 60;
    int startY = 80;
    
    DrawText("- SPELL COMPENDIUM -", SCREEN_WIDTH/2 - 100, 30, 20, GOLD);
    
    int index = 0;
    for (int i = 1; i < SPELL_COUNT; i++) {
        int col = index % cols;
        int row = index / cols;
        int x = startX + col * (cardW + 10);
        int y = startY + row * (cardH + 10);
        
        Rectangle card = {x, y, cardW, cardH};
        DrawRectangleRec(card, Fade(DARKGRAY, 0.5f));
        DrawRectangleLinesEx(card, 1, GRAY);
        
        if (player->book.discovered[i]) {
            SpellInfo info = GetSpellInfo(i);
            DrawRectangle(x+5, y+5, 15, 15, info.col);
            DrawText(info.name, x+25, y+5, 10, WHITE);
            DrawText(info.desc, x+5, y+25, 10, LIGHTGRAY);
        } else {
            DrawText("???", x+cardW/2-10, y+cardH/2-10, 20, GRAY);
            DrawText("Locked", x+5, y+5, 10, DARKGRAY);
        }
        index++;
    }
}

void DrawEntityTooltip(Entity* e, int x, int y) {
    int width = 230;
    int height = 200; 
    
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
        
        // --- THIS BLOCK WAS FIXED ---
        if(e->spellData.behavior != SPELL_PROJECTILE) {
             SpellInfo info = GetSpellInfo(e->spellData.behavior); 
             DrawText(TextFormat("[!] %s", info.name), tx, ty+55, 10, info.col);
        }
        // ----------------------------
        
        if(e->spellData.aiType != AI_LINEAR) {
            DrawText("[AI] Smart Movement", tx, ty+70, 10, WHITE);
        }
        
        ty += 40;
        DrawText(TextFormat("Temp: %.1f C", e->spellData.temperature), tx, ty+40, 10, (e->spellData.temperature>50)?RED:BLUE);
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
        if (hover) { player->selectedElement = types[i]; DrawRectangleRec(btn, WHITE); } 
        else if (player->selectedElement == types[i]) { DrawRectangleLinesEx(btn, 3, GOLD); }
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
        if (i < inv->count) { DrawRectangle(slot.x + 10, slot.y + 10, 20, 20, inv->items[i].color); }
        DrawText(TextFormat("%d", i+1), slot.x + 2, slot.y + 2, 10, GRAY);
    }
}

void DisplayEntityStatus(Entity *e, bool isVisible) {}