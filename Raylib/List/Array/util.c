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
        List->Elements[++List->count].data = data;
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
