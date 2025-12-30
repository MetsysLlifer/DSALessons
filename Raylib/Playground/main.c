#include <stdio.h>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h" // Ensure you have raygui.h in your include path

int main(void)
{
    // 1. Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raygui Scroll Container Example");
    SetTargetFPS(60);

    // -- SCROLL PANEL STATE --
    // The physical area on screen where the box appears
    Rectangle panelBounds = { 100, 50, 300, 400 }; 
    
    // The virtual area containing all items (Width usually matches panel, Height is total height of items)
    // We start with 0,0, but we will update the height dynamically below.
    Rectangle contentBounds = { 0, 0, 280, 0 }; 
    
    // Stores how far we have scrolled (X and Y)
    Vector2 scrollOffset = { 0, 0 };

    // -- DATA TO STORE -- 
    // Let's generalize this: simpler is better. Just 20 dummy items.
    int itemCount = 20;
    float itemHeight = 40.0f;
    float itemSpacing = 5.0f;

    while (!WindowShouldClose())
    {
        // 2. Update Logic
        // Calculate total content height based on items
        // Formula: (Items * Height) + (Items * Spacing)
        float totalContentHeight = (itemCount * (itemHeight + itemSpacing)) + 10; // +10 for padding
        contentBounds.height = totalContentHeight;
        contentBounds.width = panelBounds.width - 20; // -20 to make room for the scrollbar

        // 3. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText("Generalized Storage Container", 100, 20, 20, DARKGRAY);

            // --- START SCROLL PANEL ---
            
            // GuiScrollPanel draws the border/scrollbar and calculates the visible area.
            // It modifies 'scrollOffset' automatically when you drag the bar and writes the visible area into 'view'.
            Rectangle view = { 0 };
            GuiScrollPanel(panelBounds, "My Storage", contentBounds, &scrollOffset, &view);

            // CRITICAL: BeginScissorMode clips anything drawn outside the 'view' rectangle.
            BeginScissorMode((int)view.x, (int)view.y, (int)view.width, (int)view.height);

                // Draw the items inside.
                // NOTE: We add 'panelBounds.x/y' to position them relative to the panel.
                // NOTE: We add 'scrollOffset.y' to move them up/down based on scrolling.
                for (int i = 0; i < itemCount; i++)
                {
                    float yPos = panelBounds.y + scrollOffset.y + 10 + (i * (itemHeight + itemSpacing)); // 10 is top padding
                    float xPos = panelBounds.x + 10; // 10 is left padding
                    
                    Rectangle itemRect = { xPos, yPos, contentBounds.width - 20, itemHeight };

                    // Only draw items that are actually visible (Optimization)
                    // If the item is way above or way below the view, skip it.
                    if ((yPos + itemHeight) > panelBounds.y && yPos < (panelBounds.y + panelBounds.height))
                    {
                        // Example: Using a GuiButton as a list item
                        if (GuiButton(itemRect, TextFormat("Stored Item %d", i+1))) 
                        {
                            // Action when item is clicked
                            printf("Clicked Item %d\n", i+1);
                        }
                    }
                }

            EndScissorMode();
            // --- END SCROLL PANEL ---

        EndDrawing();
    }

    CloseWindow();
    return 0;
}