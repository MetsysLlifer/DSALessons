#include "util.h"
// Caution: Define RAYGUI only here
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const Vector2 slot = {SCREENWIDTH * 0.20, SCREENHEIGHT * 0.85}; // start slot pos for array
// increment the slot position by its width to display each slot in order (horizontal alignment)

void displayElements(VersionOne List){
    for(int trav = 0; trav <= List.count; trav++){
        ElementType element = List.Elements[trav];
        DrawCircle(element.position.x, element.position.y, element.size, element.color);
    }
}

void connectLine(VersionOne List){
    int size = List.count;
    for(int trav = 0; trav < size ; trav++){
        ElementType A = List.Elements[trav];
        ElementType B = List.Elements[trav+1];
        DrawLine(A.position.x, A.position.y, B.position.x, B.position.y, (Color){100, 50, 200, 100});   
    }
}

void updateHolding(VersionOne* List, ElementType** holdElement, bool* holding){
    if(!(*holding)){
        for(int trav = List->count; trav >= 0; trav--){
            ElementType* element = &List->Elements[trav];
            bool collision = CheckCollisionCircles(GetMousePosition(), 1, element->position, element->size);
            if(collision){
                // highlight the element
                DrawRing(element->position, element->size, element->size + 3, 0, 360, 1, GOLD); // element.size + 3 (thickness - outer)
                if(IsKeyPressed(KEY_H)){
                    *holdElement = element;
                    *holding = true;
                }

                break; // Stop the iteration when the element is found
            }
        }
    }else{
        if (*holdElement != NULL) {
            // show position and value of an element
            DrawText(TextFormat("position {%.2f, %.2f}", (*holdElement)->position.x , (*holdElement)->position.y, (*holdElement)->data), (*holdElement)->position.x + 20, (*holdElement)->position.y, 5, RED);
            (*holdElement)->position = GetMousePosition();
        }
        
        if(!IsKeyDown(KEY_H)) {
            *holding = false; // Update the value
            *holdElement = NULL; // clear the pointer when not holding
        }
    }
}

void drawButtons(VersionOne* List, float* feedbackTimer, char* textAlert){
    // DELETE LAST (Delete the last node in the List)
    if(GuiButton((Rectangle){ preX + 50, 50, 100, 25 }, "#119#Delete Last")){
        if(!deleteLast(List, feedbackTimer) && *feedbackTimer != 0.0) strcpy(textAlert, "Deletion Failed!");
    };
    // LOCATE (unused)
    GuiButton((Rectangle){ preX + 65, 85, 75, 25 }, "#119#Locate");
    // READ (unused)
    if(GuiButton((Rectangle){ preX + 65, 120, 75, 25 }, "#119#Read")){

    };
}

void drawslots(VersionOne List){
    // update the slots display
    for(int trav = 0; trav < MAX; trav++){
        GuiDrawRectangle((Rectangle){slot.x + (trav * 50), slot.y, 50, 50}, 2, GRAY, LIGHTGRAY);
        // Display the node if the node exists (trav <= List.count)
        if(trav <= List.count) DrawCircle(slot.x + (trav * 50) + 25, slot.y + 25, List.Elements[trav].size, List.Elements[trav].color);
    }
    // display number of nodes 'n': n/MAX (n+1 to set EMPTY as 0)
    DrawText(TextFormat("%d/%d", List.count + 1, MAX), slot.x, slot.y - 25, 18, DARKGRAY);
}