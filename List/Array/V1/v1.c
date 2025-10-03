#include <stdio.h>
#include <stdbool.h>
#define MAX 10

typedef struct{
    int data;
} ElementType;

typedef struct{
    ElementType Elements[MAX];
    int count;
} VersionOne;

// Create, Read, Update, Delete
void init(VersionOne*);
void read(VersionOne);
void insert(VersionOne*, int);
void delete(VersionOne*, int);
bool member(VersionOne, int);
int locate(VersionOne, int);


int main(){
    VersionOne List;

    init(&List);
    insert(&List, 1);
    insert(&List, 2);
    insert(&List, 3);
    insert(&List, 4);
    insert(&List, 5);
    insert(&List, 6);
    insert(&List, 7);
    insert(&List, 8);
    insert(&List, 9);
    insert(&List, 10);
    insert(&List, 11);
    
    delete(&List, 4);
    
    printf("Member: %d\n", member(List, 3));
    printf("Locate: %d\n", locate(List, 5));
    read(List);

    return 0;
}


void init(VersionOne *List)
{
    List->count = 0;
}


void read(VersionOne List)
{
    printf("List\n");
    for(int trav = 0; trav < List.count; trav++){
        printf("%d%s", List.Elements[trav].data, (trav !=  List.count - 1) ? ", " : ".\n");
    }
}


void insert(VersionOne *List, int data)
{
    if(List->count < MAX){
        List->Elements[List->count++].data = data;
    }else{
        printf("\n\nInsertion failed: Out of space\n\n");
    }
}


void delete(VersionOne *List, int data)
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