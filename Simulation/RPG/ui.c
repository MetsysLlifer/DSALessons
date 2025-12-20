// ui.c
#include "game.h"

// --- Helper Constants ---
#define SLIDER_WIDTH 150
#define SLIDER_HEIGHT 20
#define KNOB_WIDTH 10
#define MIN_SPEED 0.0f
#define MAX_SPEED 2000.0f // Max pixels per second

// --- Helper Arrays for Enums ---
const char* ElementNames[] = { "Solid", "Liquid", "Gas", "Plasma", "Unknown" };
const char* FormNames[]    = { "Solid", "Liquid", "Gas", "Plasma", "Unknown" };

// NEW: Tooltip Function
void DrawEntityTooltip(Entity* e, int x, int y) {
    // 1. Define Box Size & Position
    int width = 200;
    int height = 0; // Calculated dynamically
    int startX = x + 20; // Offset slightly to right of mouse/entity
    int startY = y - 20;
    int padding = 10;
    int lineSize = 20;

    // Calculate height based on what we are showing
    if (e->isSpell) height = 180; // Needs more space for magic stats
    else height = 80;             // Just physics stats

    // 2. Draw Background Panel
    DrawRectangle(startX, startY, width, height, Fade(BLACK, 0.9f));
    DrawRectangleLines(startX, startY, width, height, WHITE);

    // 3. Draw Stats
    int textX = startX + padding;
    int textY = startY + padding;

    // -- HEADER --
    if (e->isSpell) {
        DrawText(e->spellData.name, textX, textY, 20, YELLOW);
    } else {
        DrawText("Physics Object", textX, textY, 20, LIGHTGRAY);
    }
    textY += 25;

    // -- BASIC STATS --
    DrawText(TextFormat("Mass: %.1f kg", e->mass), textX, textY, 10, WHITE);
    textY += 15;
    DrawText(TextFormat("Vel: %.0f", Vector2Length(e->velocity)), textX, textY, 10, WHITE);
    textY += 20; // Extra Gap

    // -- MAGIC STATS (Only if it's a spell) --
    if (e->isSpell) {
        DrawText("----------------", textX, textY, 10, GRAY);
        textY += 15;
        
        // Element & Form
        DrawText(TextFormat("Type: %s", ElementNames[e->spellData.type]), textX, textY, 10, SKYBLUE);
        textY += 15;
        DrawText(TextFormat("Form: %s", FormNames[e->spellData.form]), textX, textY, 10, SKYBLUE);
        textY += 15;

        // Properties
        DrawText(TextFormat("Temp: %.1f C", e->spellData.temperature), textX, textY, 10, (e->spellData.temperature > 50)? RED : BLUE);
        textY += 15;
        DrawText(TextFormat("Intensity: %.2f", e->spellData.intensity), textX, textY, 10, WHITE);
        textY += 15;
        DrawText(TextFormat("Dryness: %.2f", e->spellData.dryness), textX, textY, 10, ORANGE);
    }
}

// Can modify the player's speed
void DrawSpeedSlider(float* speed, int x, int y) {
    // 1. Logic: Handle Mouse Input
    Vector2 mouse = GetMousePosition();
    
    // Define the area of the slider bar
    Rectangle sliderBar = { x, y, SLIDER_WIDTH, SLIDER_HEIGHT };

    // Check if Mouse is "Inside" the slider AND clicking
    if (CheckCollisionPointRec(mouse, sliderBar)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            // Calculate where we clicked relative to the bar (0.0 to 1.0)
            float mouseRelX = mouse.x - x;
            float percent = mouseRelX / (float)SLIDER_WIDTH;
            
            // Clamp value so we don't go below 0 or above 100%
            if (percent < 0.0f) percent = 0.0f;
            if (percent > 1.0f) percent = 1.0f;

            // Map Percentage to Speed Range
            *speed = MIN_SPEED + (percent * (MAX_SPEED - MIN_SPEED));
        }
    }

    // 2. Visuals: Draw the Slider
    
    // Draw the Background Bar (Dark Gray)
    DrawRectangleRec(sliderBar, DARKGRAY);
    DrawRectangleLinesEx(sliderBar, 2, WHITE); // Border

    // Draw the "Fill" (How full is the bar?)
    // Calculate current percentage based on actual speed
    float currentPercent = (*speed - MIN_SPEED) / (MAX_SPEED - MIN_SPEED);
    DrawRectangle(x, y, (int)(SLIDER_WIDTH * currentPercent), SLIDER_HEIGHT, MAROON);

    // Draw Text Label
    DrawText("MAX SPEED", x, y - 20, 10, WHITE);
    DrawText(TextFormat("%.0f", *speed), x + SLIDER_WIDTH + 10, y + 5, 10, WHITE);
}

void DrawInventory(Inventory* inv, int x, int y) {
    int slotSize = 40;
    int padding = 5;
    Vector2 mouse = GetMousePosition();

    DrawText("INVENTORY (Click to Select)", x, y - 20, 10, DARKGRAY);

    for (int i = 0; i < INVENTORY_CAPACITY; i++) {
        Rectangle slot = { x + (i * (slotSize + padding)), y, slotSize, slotSize };
        
        // 1. Handle Selection Logic (Clicking a slot)
        bool isMouseOver = CheckCollisionPointRec(mouse, slot);
        
        if (isMouseOver && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Toggle logic: If clicking the same slot, unselect it. Otherwise select new one.
            if (inv->selectedSlot == i) {
                inv->selectedSlot = -1;
            } else {
                // Only select if there is actually an item there
                if (i < inv->count) {
                    inv->selectedSlot = i;
                }
            }
        }

        // 2. Draw Slot Visuals
        // If this is the selected slot, draw it Green, otherwise Gray
        Color borderColor = (inv->selectedSlot == i) ? GREEN : DARKGRAY;
        int borderThick = (inv->selectedSlot == i) ? 3 : 2;

        DrawRectangleRec(slot, Fade(LIGHTGRAY, 0.5f));
        DrawRectangleLinesEx(slot, borderThick, borderColor);

        // 3. Draw Item inside the slot (Static View)
        if (i < inv->count) {
            DrawCircle(slot.x + slotSize/2, slot.y + slotSize/2, 10, inv->items[i].color);
            DrawText(TextFormat("%.0f", inv->items[i].mass), slot.x + 2, slot.y + 2, 10, BLACK);
        }
    }
    
    // 4. Draw "Held" Item (Follows Mouse)
    // If we have a selected slot AND it's a valid item
    if (inv->selectedSlot != -1 && inv->selectedSlot < inv->count) {
        // Draw the item slightly offset from cursor to look like we are holding it
        DrawCircle(mouse.x + 10, mouse.y + 10, 12, inv->items[inv->selectedSlot].color);
        DrawCircleLines(mouse.x + 10, mouse.y + 10, 13, BLACK); // Outline
    }
    
    DrawText("Press 'E' to Pickup, Select + 'Q' to Drop", x, y + slotSize + 5, 10, DARKGRAY);
}

// Future stats placeholder: (Additional stats to be added)
void DisplayEntityStatus(Entity *entity, bool isVisible) {
    if (!isVisible) return; // If toggle is off, stop immediately

    // 1. Setup the Panel
    int panelX = 10, panelY = 50, width = 200, height = 180;

    // Draw a semi-transparent black box
    DrawRectangle(panelX, panelY, width, height, Fade(BLACK, 0.8f));
    DrawRectangleLines(panelX, panelY, width, height, WHITE); // Border

    // 2. Draw the Stats
    int textX = panelX + 10;
    int startY = panelY + 10;

    DrawText("PLAYER STATUS", textX, startY, 20, WHITE);
    
    // Display Position (Formatted to 1 decimal place)
    DrawText("ENTITY STATUS", textX, startY, 20, YELLOW);
    DrawText(TextFormat("Pos: %.0f, %.0f", entity->position.x, entity->position.y), textX, startY + 30, 10, WHITE);
    
    // Display Speed/Velocity
    DrawText(TextFormat("Vel: %.1f, %.1f", entity->velocity.x, entity->velocity.y), textX, startY + 50, 10, GREEN);

    // Modify player's speed
    DrawText(TextFormat("Mass: %.1f kg", entity->mass), textX, startY + 70, 10, GRAY);

    // Slider modifies maxSpeed
    DrawSpeedSlider(&entity->maxSpeed, textX, startY + 120);
}

