// inventory.c
#include "game.h"

void InitInventory(Inventory* inv) {
    inv->count = 0;
    inv->selectedSlot = -1; // Start with nothing selected
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
