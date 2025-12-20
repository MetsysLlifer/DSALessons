#include "game.h"

// --- Helper Constants ---
#define SLIDER_WIDTH 150
#define SLIDER_HEIGHT 20
#define MIN_SPEED 0.0f
#define MAX_SPEED 2000.0f 

// --- Helper Arrays for Enums ---
const char* ElementNames[] = { "Solid", "Liquid", "Gas", "Plasma", "Unknown" };
const char* FormNames[]    = { "Solid", "Liquid", "Gas", "Plasma", "Unknown" };

void DrawEntityTooltip(Entity* e, int x, int y) {
    int width = 200;
    int height = e->isSpell ? 180 : 80;
    int startX = x + 20; 
    int startY = y - 20;

    DrawRectangle(startX, startY, width, height, Fade(BLACK, 0.9f));
    DrawRectangleLines(startX, startY, width, height, WHITE);

    int textX = startX + 10;
    int textY = startY + 10;

    if (e->isSpell) DrawText(e->spellData.name, textX, textY, 20, YELLOW);
    else DrawText("Physics Object", textX, textY, 20, LIGHTGRAY);
    textY += 25;

    DrawText(TextFormat("Mass: %.1f kg", e->mass), textX, textY, 10, WHITE);
    textY += 15;
    DrawText(TextFormat("Vel: %.0f", Vector2Length(e->velocity)), textX, textY, 10, WHITE);
    textY += 20;

    if (e->isSpell) {
        DrawText("----------------", textX, textY, 10, GRAY);
        textY += 15;
        DrawText(TextFormat("Type: %s", ElementNames[e->spellData.type]), textX, textY, 10, SKYBLUE);
        textY += 15;
        DrawText(TextFormat("Form: %s", FormNames[e->spellData.form]), textX, textY, 10, SKYBLUE);
        textY += 15;
        DrawText(TextFormat("Temp: %.1f C", e->spellData.temperature), textX, textY, 10, (e->spellData.temperature > 50)? RED : BLUE);
        textY += 15;
        DrawText(TextFormat("Intensity: %.2f", e->spellData.intensity), textX, textY, 10, WHITE);
    }
}

void DrawSpeedSlider(float* speed, int x, int y) {
    Vector2 mouse = GetMousePosition();
    Rectangle sliderBar = { x, y, SLIDER_WIDTH, SLIDER_HEIGHT };

    if (CheckCollisionPointRec(mouse, sliderBar)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            float mouseRelX = mouse.x - x;
            float percent = mouseRelX / (float)SLIDER_WIDTH;
            if (percent < 0.0f) percent = 0.0f;
            if (percent > 1.0f) percent = 1.0f;
            *speed = MIN_SPEED + (percent * (MAX_SPEED - MIN_SPEED));
        }
    }
    DrawRectangleRec(sliderBar, DARKGRAY);
    DrawRectangleLinesEx(sliderBar, 2, WHITE); 
    float currentPercent = (*speed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
    DrawRectangle(x, y, (int)(SLIDER_WIDTH * currentPercent), SLIDER_HEIGHT, MAROON);
    DrawText(TextFormat("%.0f", *speed), x + SLIDER_WIDTH + 10, y + 5, 10, WHITE);
}

void DrawInventory(Inventory* inv, int x, int y) {
    int slotSize = 40;
    Vector2 mouse = GetMousePosition();

    DrawText("INVENTORY (1-5)", x, y - 20, 10, DARKGRAY);

    for (int i = 0; i < INVENTORY_CAPACITY; i++) {
        Rectangle slot = { x + (i * 45), y, slotSize, slotSize };
        
        if (CheckCollisionPointRec(mouse, slot) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (inv->selectedSlot == i) inv->selectedSlot = -1;
            else if (i < inv->count) inv->selectedSlot = i;
        }

        bool isSelected = (inv->selectedSlot == i);
        DrawRectangleRec(slot, Fade(LIGHTGRAY, 0.5f));
        DrawRectangleLinesEx(slot, isSelected ? 3 : 2, isSelected ? GREEN : DARKGRAY);

        if (i < inv->count) {
            DrawCircle(slot.x + slotSize/2, slot.y + slotSize/2, 10, inv->items[i].color);
            DrawText(TextFormat("%.0f", inv->items[i].mass), slot.x + 2, slot.y + 2, 10, BLACK);
        }
    }
}

void DisplayEntityStatus(Entity *entity, bool isVisible) {
    if (!isVisible) return; 
    int panelX = 10, panelY = 50, width = 200, height = 180;
    DrawRectangle(panelX, panelY, width, height, Fade(BLACK, 0.8f));
    DrawRectangleLines(panelX, panelY, width, height, WHITE); 
    
    DrawText("PLAYER STATUS", panelX + 10, panelY + 10, 20, WHITE);
    DrawText(TextFormat("Pos: %.0f, %.0f", entity->position.x, entity->position.y), panelX + 10, panelY + 40, 10, WHITE);
    DrawSpeedSlider(&entity->maxSpeed, panelX + 10, panelY + 120);
}