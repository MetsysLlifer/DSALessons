#include "util.h"
// CAUTION: Define RAYGUI only here
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// POSITION (VH CELL) 
const Vector2 cellPos = {
    SCREENWIDTH * 0.4,
    60
};

// Rect Size (VH CELL)
const int recWidth = 150;
const int recHeight = 30;
const int cPad = recHeight + 5; // padding (Cell)
const int fPad = 5; // padding for Cell's fields
Vector2 linkSize = {(recWidth/2)-10, recHeight-10};

void drawVirtualHeap(VirtualHeap VH){
    int totalBGHeight = (cPad * (MAX - 1)) + recHeight + 20;
    DrawRectangle(cellPos.x-10, cellPos.y-10, recWidth+20, totalBGHeight, GRAY);

    for(int trav = 0; trav < MAX; trav++){
        // Cell
        DrawRectangle(cellPos.x, cellPos.y+(trav*cPad), recWidth, recHeight, LIGHTGRAY);


        // Fields
        DrawRectangle(cellPos.x+fPad, cellPos.y+(trav*cPad)+fPad, (recWidth/2)-10, recHeight-10, DARKGRAY);
        DrawText(TextFormat("Value: %d", VH.Nodes[trav].elem.data), cellPos.x+(fPad*2), cellPos.y+(trav*cPad)+(fPad*2), 8, WHITE);

        DrawRectangle(cellPos.x+(recWidth/2)+fPad, cellPos.y+(trav*cPad)+fPad, linkSize.x, linkSize.y, DARKGRAY);
        DrawText(TextFormat("Link: %d", VH.Nodes[trav].link), cellPos.x+(recWidth/2)+(fPad*2), cellPos.y+(trav*cPad)+(fPad*2), 8, WHITE);

        
        
    }
}

void linkConnect(VirtualHeap VH, List L){

    DrawCircle((cellPos.x+(recWidth/2)+linkSize.x), cellPos.y+(L*cPad)+(recHeight/2), 5, (Color){ 102, 191, 255, 150 });

    for(int trav = L; trav != -1; trav = VH.Nodes[trav].link){
        // Check for valid node and draw a spline
        if(trav != -1 && VH.Nodes[trav].link != -1) {
            int targetIdx = VH.Nodes[trav].link;

            // 1st point (current node)
            Vector2 linkPos = {cellPos.x+(recWidth/2), cellPos.y+(trav*cPad)};
            Vector2 start = {linkPos.x+linkSize.x+fPad, linkPos.y+(recHeight / 2)};

            // Last point (target node)
            Vector2 end = { cellPos.x+recWidth, cellPos.y+(targetIdx * cPad)+(recHeight / 2) };

            // Control point (curvature)
            float distance = abs(targetIdx - trav);
            Vector2 control = { 
                cellPos.x + recWidth + (distance * cPad), // Pulls the curve to the right
                (start.y + end.y) / 2              // Midway vertically
            };

            Vector2 points[3] = { start, control, end };

            DrawSplineBezierQuadratic(points, 3, 2.0f, (Color){ 102, 191, 255, 150 });
        }
    }
}

void drawList(List L){
    DrawRectangle(SCREENWIDTH * 0.2, SCREENHEIGHT * 0.47, 50, 30, LIGHTGRAY);
}