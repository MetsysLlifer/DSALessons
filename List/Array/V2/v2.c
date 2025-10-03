#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX 10

typedef struct{
    int data;
} ElementType;

typedef struct node{
    ElementType Elements[MAX];
    int count;
}*VersionTwo;

void init(VersionTwo*);
void read(VersionTwo);
void insert(VersionTwo, int);
void delete(VersionTwo, int);


int main(){
    VersionTwo List;

    init(&List);
    insert(List, 1);
    insert(List, 2);
    insert(List, 3);
    insert(List, 4);
    insert(List, 5);
    insert(List, 6);
    insert(List, 7);
    insert(List, 8);
    insert(List, 9);
    insert(List, 10);
    insert(List, 11);
    insert(List, 12);
    insert(List, 13);
    
    delete(List, 2);

    read(List);
}


void init(VersionTwo *List){
    *List = (VersionTwo)malloc(sizeof(struct node));
    (*List)->count = 0;
}

void read(VersionTwo List){
    int trav = 0;
    printf("List:\n");
    for(; trav < List->count; trav++){
        printf("%d%s", List->Elements[trav].data, (trav != List->count-1)? ", ": ".\n");
    }
}

void insert(VersionTwo List, int data){
    if(List->count != MAX){
        List->Elements[List->count++].data = data;
    }else printf("\n\nInsertion failed: Out of space!\n\n");
}

void delete(VersionTwo List, int data){
    int trav = 0;
    for(; trav < List->count && List->Elements[trav].data != data; trav++){}
    if(trav != List->count){
        for(; trav < List->count; trav++){
            List->Elements[trav].data = List->Elements[trav + 1].data;
        }
        List->count--;
    }
}

