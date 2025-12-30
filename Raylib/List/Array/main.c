#include "util.h"

int 
main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Linked List (Array)");
    
    // Variables

    float feedbackTimer = 0.0;  // To track timer for "insertion/deletion failed"
    char textAlert[50] = "";    // 

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
        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)) break; // terminate the system
        
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        Vector2 mouse = GetMousePosition(); 

        // Update List (insert new element)
        if (IsKeyPressed(KEY_A)){
            if(!insert(&List, 0, mouse, &feedbackTimer) && feedbackTimer != 0.0) strcpy(textAlert, "Insertion Failed!");
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            connectLine(List);
            displayElements(List);

            // // Hold Element (Physics)
            updateHolding(&List, &holdElement, &holding);
            
            drawButtons(&List, &feedbackTimer, textAlert); // mainly delete, locate, and read
            drawslots(List);
            drawPanel(&List);

            // Display notfication (insertion/deletion failed)
            if(feedbackTimer > 0.0){
                feedbackTimer -= GetFrameTime();
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
