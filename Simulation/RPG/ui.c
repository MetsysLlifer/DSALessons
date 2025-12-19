// ui.c
#include "game.h"

// --- Helper Constants ---
#define SLIDER_WIDTH 150
#define SLIDER_HEIGHT 20
#define KNOB_WIDTH 10
#define MIN_SPEED 0.0f
#define MAX_SPEED 2000.0f // Max pixels per second

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