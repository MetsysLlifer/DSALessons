#include "util.h"
// CAUTION: Define RAYGUI only here
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// POSITION (VH CELL) 
int xPos = SCREENWIDTH * 0.4;
int yPos = 60;

// Rect Size (VH CELL)
const int recWidth = 150;
const int recHeight = 30;
const int cPad = recHeight + 5; // padding (Cell)
const int fPad = 5; // padding for Cell's fields

void drawVirtualHeap(VirtualHeap VH){
    int totalBGHeight = (cPad * (MAX - 1)) + recHeight + 20;
    DrawRectangle(xPos-10, yPos-10, recWidth+20, totalBGHeight, GRAY);

    for(int trav = 0; trav < MAX; trav++){
        // Cell
        DrawRectangle(xPos, yPos + (trav * cPad), recWidth, recHeight, LIGHTGRAY);
        // Fields
        DrawRectangle(xPos + fPad, yPos + (trav * cPad) + fPad, (recWidth/2) - 10, recHeight - 10, DARKGRAY);
        DrawRectangle(xPos + (recWidth/2) + fPad, yPos + (trav * cPad) + fPad, (recWidth/2) - 10, recHeight - 10, DARKGRAY);
    }
}

void drawList(List L){
    DrawRectangle(SCREENWIDTH * 0.2, SCREENHEIGHT * 0.47, 50, 30, LIGHTGRAY);
}