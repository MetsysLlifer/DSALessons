#include "util.h"

void init(VersionOne *List)
{
    List->count = -1;
    printf("count: %d\n", List->count);
}

void read(VersionOne List)
{
    printf("List\n");
    for(int trav = 0; trav < List.count; trav++){
        printf("%d%s", List.Elements[trav].data, (trav !=  List.count - 1) ? ", " : ".\n");
    }
}


int insert(VersionOne* List, int data, Vector2 position, float* timer)
{
    if(List->count + 1 != MAX){
        List->Elements[List->count++].data = data;
        List->Elements[List->count].position = position;
        List->Elements[List->count].size = 10;
        List->Elements[List->count].color = (Color){10 + GetRandomValue(10, 255), 10 + GetRandomValue(10, 255), 10 + GetRandomValue(10, 255), 255};
        printf("COUNT: %d\n", List->count);
    }else{
        *timer = 2.0;
        printf("Insertion failed: Out of space\n\n");
        return 0;
    }
    return 1;
}

void delete(VersionOne* List, int data)
{
    int trav = 0;
    for(; trav < List->count && List->Elements[trav].data != data; trav++){}
    if(trav != List->count){
        for(; trav != List->count; trav++){
            List->Elements[trav].data = List->Elements[trav + 1].data;
        }
        List->count--;
    }else printf("\n\nDeletion failed: '%d' cannot be found in the List!\n\n", data);
}

int deleteLast(VersionOne* List, float* timer)
{
    if(List->count != -1){
        List->count--;
    }else {
        *timer = 2.0;
        printf("Deletion failed: array is empty!\n");
        return 0;
    }
    return 1;
}

bool member(VersionOne List, int data){
    int trav = 0;
    for(;trav != List.count && List.Elements[trav].data != data; trav++){}
    return(trav != List.count)? true: false;
}

int locate(VersionOne List, int data){
    int trav = 0;
    for(; trav < List.count && List.Elements[trav].data != data; trav++){}
    return(trav != List.count)? 1: -1;
}


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