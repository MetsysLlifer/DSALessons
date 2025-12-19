// inventory.c
#include "game.h"

void InitInventory(Inventory* inv) {
    inv->count = 0;
    // Clear memory to avoid garbage data
    for(int i=0; i<INVENTORY_CAPACITY; i++) {
        inv->items[i].size = 0; // Mark as empty
    }
}

bool AddItem(Inventory* inv, Entity item) {
    if (inv->count >= INVENTORY_CAPACITY) return false; // Full

    inv->items[inv->count] = item;
    inv->count++;
    return true;
}

Entity DropItem(Inventory* inv, int index) {
    Entity itemToDrop = {0};
    
    if (index >= 0 && index < inv->count) {
        itemToDrop = inv->items[index];

        // Shift remaining items left to fill the gap
        // Example: [A, B, C] -> Drop A -> [B, C]
        for (int i = index; i < inv->count - 1; i++) {
            inv->items[i] = inv->items[i + 1];
        }
        
        inv->count--;
    }
    return itemToDrop;
}

void DrawInventory(Inventory* inv, int x, int y) {
    int slotSize = 40;
    int padding = 5;

    DrawText("INVENTORY", x, y - 20, 10, DARKGRAY);

    for (int i = 0; i < INVENTORY_CAPACITY; i++) {
        Rectangle slot = { x + (i * (slotSize + padding)), y, slotSize, slotSize };
        
        // Draw Slot Background
        DrawRectangleRec(slot, Fade(LIGHTGRAY, 0.5f));
        DrawRectangleLinesEx(slot, 2, DARKGRAY);

        // Draw Item inside if it exists
        if (i < inv->count) {
            // Draw a mini version of the entity
            DrawCircle(slot.x + slotSize/2, slot.y + slotSize/2, 10, inv->items[i].color);
            
            // Draw Mass label
            DrawText(TextFormat("%.0f", inv->items[i].mass), slot.x + 2, slot.y + 2, 10, BLACK);
        }
    }
    
    DrawText("Press 'E' to Pickup, 'Q' to Drop", x, y + slotSize + 5, 10, DARKGRAY);
}