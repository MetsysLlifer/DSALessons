#include "util.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int 
main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "List (Array)");
    
    // Variables


    float feetBackTimer = 0.0;  // To track timer for "insertion/deletion failed"
    char textAlert[50] = "";    // 

    const Vector2 slot = {SCREENWIDTH * 0.20, SCREENHEIGHT * 0.85}; // start slot pos for array
    // increment the slot position by its width to display each slot in order (horizontal alignment)

    VersionOne List;
    init(&List);
    

    // Flags
    bool holding = false; // If it's holding something
    ElementType* holdElement = NULL; // Store the item that is currently holding

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    SetExitKey(KEY_NULL);
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button (control + Q)
    {
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) break;
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        Vector2 mouse = GetMousePosition(); 

        // Update List (insert new element)
        if (IsKeyPressed(KEY_A)){
            if(!insert(&List, 0, mouse, &feetBackTimer) && feetBackTimer != 0.0) strcpy(textAlert, "Insertion Failed!");
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            connectLine(List);
            displayElements(List);

            // // Hold Element (Physics)
            updateHolding(&List, &holdElement, &holding);

            // DELETE LAST (Delete the last node in the List)
            if(GuiButton((Rectangle){ preX + 50, 50, 100, 25 }, "#119#Delete Last")){
                if(!deleteLast(&List, &feetBackTimer) && feetBackTimer != 0.0) strcpy(textAlert, "Deletion Failed!");
            };
            // LOCATE (unused)
            GuiButton((Rectangle){ preX + 65, 85, 75, 25 }, "#119#Locate");
            // READ (unused)
            if(GuiButton((Rectangle){ preX + 65, 120, 75, 25 }, "#119#Read")){

            };

            // update the slots display
            for(int trav = 0; trav < MAX; trav++){
                GuiDrawRectangle((Rectangle){slot.x + (trav * 50), slot.y, 50, 50}, 2, GRAY, LIGHTGRAY);
                // Display the node if the node exists (trav <= List.count)
                if(trav <= List.count) DrawCircle(slot.x + (trav * 50) + 25, slot.y + 25, List.Elements[trav].size, List.Elements[trav].color);
            }
            // display number of nodes 'n': n/MAX (n+1 to set EMPTY as 0)
            DrawText(TextFormat("%d/%d", List.count + 1, MAX), slot.x, slot.y - 25, 18, DARKGRAY);

            // Display notfication (insertion/deletion failed)
            if(feetBackTimer > 0.0){
                feetBackTimer -= GetFrameTime();
                DrawText(textAlert, 10, 50, 30, RED);
            }

            DrawText("List (Array)", 10, 10, 20, LIGHTGRAY);
            DrawText(TextFormat("MAX NODES: %d", MAX), 10, 30, 10, GRAY);
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
