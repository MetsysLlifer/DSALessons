#include "util.h"

int 
main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Cursor Based (Array)");

    // Variables
    VirtualHeap VH;
    List L;
    init(&L);
    initVH(&VH);


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
        if(IsKeyPressed(KEY_A)) insertLast(&L, &VH, GetRandomValue(1, MAX));
        if(IsKeyPressed(KEY_D)) Delete(&L, &VH, GetRandomValue(1, MAX));
        if(IsKeyPressed(KEY_L)) insertFirst(&L, &VH, GetRandomValue(1, MAX));

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            drawVirtualHeap(VH);
            drawList(L);
            linkConnect(VH, L);

            DrawText("Cursor Based (Array)", 10, 10, 20, LIGHTGRAY);
            DrawText("LET'S DO THIS!", 10, 30, 10, GRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
