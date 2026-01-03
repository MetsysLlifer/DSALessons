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


Color ColorLerp3(Color c1, Color c2, Color c3, float t) {
    // Safety clamp (ensure t is between 0.0 and 1.0)
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    if (t < 0.5f) {
        // FIRST HALF: Transition from c1 to c2
        // We multiply t by 2 to map the range [0.0 - 0.5] to [0.0 - 1.0]
        return ColorLerp(c1, c2, t * 2.0f);
    } else {
        // SECOND HALF: Transition from c2 to c3
        // We subtract 0.5, then multiply by 2 to map [0.5 - 1.0] to [0.0 - 1.0]
        return ColorLerp(c2, c3, (t - 0.5f) * 2.0f);
    }
}

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

// Draw spline from node
void linkConnect(VirtualHeap VH, List L){

    // Count the total number of nodes in the List
    // To add dynamic color for splines
    int listCount = 0;
    for(int t = L; t != -1; t = VH.Nodes[t].link) {
        listCount++;
    }
    int currentStep = 0; // Counter to track progress

    for(int trav = L; trav != -1; trav = VH.Nodes[trav].link){
        // Check for valid node and draw a spline
        if(trav != -1 && VH.Nodes[trav].link != -1) {
            int targetIdx = VH.Nodes[trav].link;

            // --- COLOR CALCULATION ---
            // Calculate factor 't' between 0.0 (start) and 1.0 (end)
            float t = (listCount > 2) ? (float)currentStep / (float)(listCount - 2) : 0.0f;
            
            // Clamp t to max 1.0 (just in case)
            if (t > 1.0f) t = 1.0f;

            Color startColor = SKYBLUE;
            Color midColor = GOLD;
            Color endColor = RED;
            
            // Raylib function to mix two colors based on 't'
            Color dynamicColor = ColorLerp3(startColor, midColor, endColor, t);
            // -------------------------

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

            // Draw the spline on the screen
            DrawSplineBezierQuadratic(points, 3, 2.0f, dynamicColor);

            // Add an indicator (triangle) to know which node/index the link is pointing to
            DrawTriangle(
                // Draw ORder: Counter-Clockwise
                // (Triangle pointing to left)
                (Vector2){end.x + 5, end.y + 5}, // Bootom base
                (Vector2){end.x + 5, end.y -5}, // Top Base
                (Vector2){end.x, end.y}, // Tip
                dynamicColor
            );

            // Increment the step counter for the next loop
            currentStep++;
        }
    }
}

void drawList(List L){
    DrawRectangle(SCREENWIDTH * 0.2, SCREENHEIGHT * 0.47, 50, 30, LIGHTGRAY);
}  