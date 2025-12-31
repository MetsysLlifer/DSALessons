#include "util.h"
// CAUTION: Define RAYGUI only here
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Initialization
// -----------------------------------------------------------------------------------------------------------------------
// UI Panel Configuration
const int panelsize[2] = {150, 150}; // Panel dimensions
Rectangle panelBounds = {SCREENWIDTH * 0.78, SCREENHEIGHT * 0.27, panelsize[0], panelsize[1]}; // Panel position and size
Rectangle contentBounds = {0}; // Content area inside panel
Vector2 scrollOffset = {0, 0}; // Scroll position tracking
float buttonHeight = 20; // Fixed height makes UI cleaner
float itemSpacing = 5.0f; // Space between list items
float buttonPad[2] = {10, 30}; // Padding inside buttons [horizontal, vertical]
Rectangle view = { 0 }; // Visible area for clipping

// Store Selected Element
ElementType* displayElem = {0};
int idx = 0; // Get the index of selected element
bool toDisplayElem = false; // Flag (to display selected element)

// -----------------------------------------------------------------------------------------------------------------------



// Array slot display - bottom of screen
const Vector2 slot = {SCREENWIDTH * 0.20, SCREENHEIGHT * 0.85}; // Starting position for array slots
// Slots arranged horizontally; increment X by slot width to display each in order


// Draw all elements from the list as circles at their positions
void displayElements(LinkedList List){
    LinkedList trav = List;
    for(; trav != NULL; trav = trav->next){
        ElementType element = trav->Element;
        DrawRectangle(element.position.x - element.size, element.position.y - element.size, element.size * 2, element.size * 2, element.color);
        // Draw Nex Pointer
        DrawRectangleLines(element.position.x + element.size, element.position.y - element.size, element.size * 2, element.size * 2, DARKGRAY);
        DrawCircle(element.position.x + (element.size * 2), element.position.y, 3, DARKGRAY);
    }
}

// Draw lines connecting consecutive elements to visualize order
void connectLine(LinkedList List){
    LinkedList trav = List;
    for(; trav != NULL ; trav = trav->next){
        ElementType A = trav->Element;
        if(trav->next != NULL){
            ElementType B = trav->next->Element;
            DrawLine(A.position.x + (A.size * 2), A.position.y, B.position.x + (B.size * 2), B.position.y, (Color){100, 50, 200, 100});
        }
    }
}

// Handle picking up and dragging elements with H key
void updateHolding(LinkedList* List, ElementType** holdElement, bool* holding){
    if(!(*holding)){
        // Not currently holding - check for mouse collision with elements
        LinkedList* trav = List;
        for(; *trav != NULL; trav = &(*trav)->next){
            ElementType* element = &(*trav)->Element;
            bool collision = CheckCollisionCircles(GetMousePosition(), 1, element->position, element->size);
            if(collision){
                // Highlight hovered element with gold ring
                DrawRing(element->position, element->size, element->size + 3, 0, 360, 1, GOLD);
                // Grab element on H key press
                if(IsKeyPressed(KEY_H)){
                    *holdElement = element;
                    *holding = true;
                }
                break;
            }
        }
    }else{
        // Currently holding - move element with mouse
        if (*holdElement != NULL) {
            // Display element info while dragging
            DrawText(TextFormat("position {%.2f, %.2f}", (*holdElement)->position.x , (*holdElement)->position.y, (*holdElement)->data), (*holdElement)->position.x + 20, (*holdElement)->position.y, 5, RED);
            (*holdElement)->position = GetMousePosition();
        }
        
        // Release on H key release
        if(!IsKeyDown(KEY_H)) {
            *holding = false;
            *holdElement = NULL;
        }
    }
}

// Draw control buttons for list operations
void drawButtons(LinkedList* List, float* feedbackTimer, char* textAlert){
    // Delete last element from list
    if(GuiButton((Rectangle){ preX + 50, 20, 100, 25 }, "#119#Delete Last")){
        if(!deleteLast(List, feedbackTimer) && *feedbackTimer != 0.0) strcpy(textAlert, "Deletion Failed!");
    };
    // Placeholder buttons (TODO: implement functionality)
    GuiButton((Rectangle){ preX + 65, 55, 75, 25 }, "#119#Locate");
    GuiButton((Rectangle){ preX + 65, 90, 75, 25 }, "#119#Read");
}

// Draw scrollable panel listing all elements
void drawPanel(LinkedList* List) {
    // Calculate total content height for scrolling
    float totalHeight = 0;
    int totalcount = 0; // Nodes in Linked List
    for(LinkedList trav = *List; trav != NULL; trav = trav->next, totalcount++){}

    if (*List != NULL) {
        // Height = (num items) * (item height + spacing) + padding
        totalHeight = (totalcount + 1) * (buttonHeight + itemSpacing) + 20; 
    }
    
    contentBounds.height = totalHeight;
    contentBounds.width = panelBounds.width - 20; // Account for scrollbar

    // Draw the scrollable panel container
    GuiScrollPanel(panelBounds, "Elements", contentBounds, &scrollOffset, &view);

    // Clip rendering to panel bounds
    BeginScissorMode((int)view.x, (int)view.y, (int)view.width, (int)view.height);


    if (*List != NULL) {
        LinkedList* trav = List;
        for (int count = 0; *trav != NULL; trav = &(*trav)->next, count++) {
            // Calculate item position accounting for scroll offset
            float yPos = panelBounds.y + scrollOffset.y + buttonPad[1] + (count * (buttonHeight + itemSpacing));
            float xPos = panelBounds.x + buttonPad[0];
            
            Rectangle itemRect = { xPos, yPos, contentBounds.width - 10, buttonHeight };

            // Only render if item is visible in panel
            if ((yPos + buttonHeight) > panelBounds.y && yPos < (panelBounds.y + panelBounds.height)) {
                // Draw clickable item button
                if (GuiButton(itemRect, "")) {
                    printf("Clicked element index: %d\n", count);
                    idx = count;
                    displayElem = &(*trav)->Element;
                    toDisplayElem = true;
                }
                // Draw element color indicator
                DrawCircle(xPos + buttonPad[0], yPos + buttonPad[0], buttonHeight / 3, (*trav)->Element.color);
                // Draw item label
                DrawText(TextFormat("Element [%d]", count), xPos + (buttonPad[0] * 2), yPos + (buttonPad[0]/ 2), 11 , DARKGRAY);
            }
        }
    }

    EndScissorMode();

    GuiDrawRectangle((Rectangle){panelBounds.x, panelBounds.y + panelBounds.height, panelsize[0], panelsize[1] * 0.7}, 1, GRAY, LIGHTGRAY);
    if(toDisplayElem) displayElementDetails(*displayElem, (Vector2){panelBounds.x, panelBounds.y + panelBounds.height}, idx);
}

// Display detailed info about a single element
void displayElementDetails(ElementType Elem, Vector2 position, int idx){
    int xPos = position.x + 5;
    DrawText(TextFormat("Index: %d", idx), xPos, position.y + 10, 10, WHITE);
    DrawText(TextFormat("Data: %d", Elem.data), xPos, position.y + 20, 10, WHITE);
    DrawText(TextFormat("Global Position: (%.0f, %.0f)", Elem.position.x, Elem.position.y), xPos, position.y + 30, 10, WHITE);
    DrawText(TextFormat("Size: %d", Elem.size), xPos, position.y + 40, 10, WHITE);
    DrawText(TextFormat("Color: (%d, %d, %d, %d)", Elem.color.r, Elem.color.g, Elem.color.b, Elem.color.a), xPos, position.y + 50, 10, Elem.color);
}
