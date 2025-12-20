#include "game.h"

void InitInventory(Inventory* inv) {
    inv->count = 0;
    inv->selectedSlot = -1; 
    for(int i=0; i<INVENTORY_CAPACITY; i++) {
        inv->items[i].size = 0; 
        inv->items[i].mass = 0;
        inv->items[i].color = BLANK;
    }
}

bool AddItem(Inventory* inv, Entity item) {
    if (inv->count >= INVENTORY_CAPACITY) return false; 
    inv->items[inv->count] = item;
    inv->count++;
    return true;
}

Entity DropItem(Inventory* inv, int index) {
    Entity itemToDrop = {0};
    if (index >= 0 && index < inv->count) {
        itemToDrop = inv->items[index];
        for (int i = index; i < inv->count - 1; i++) {
            inv->items[i] = inv->items[i + 1];
        }
        inv->count--;
        inv->selectedSlot = -1;
    }
    return itemToDrop;
}