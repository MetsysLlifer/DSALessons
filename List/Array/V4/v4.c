#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX 10

typedef struct{
    int data;
} ElementType;

typedef struct node{
    ElementType *Elements;
    int count;
} *VersionFour;


void init(VersionFour*);
void read(VersionFour);
void insert(VersionFour, int);
void delete(VersionFour, int);
bool member(VersionFour, int);
int locate(VersionFour, int);


int main(){
    VersionFour List;

    init(&List);

    insert(List, 1);
    insert(List, 2);
    insert(List, 3);

    delete(List, 2);

    printf("Member: %d\n", member(List, 2));
    printf("Locate: %d\n", locate(List, 4));

    read(List);

    return 0;
}


void init(VersionFour *List){
    *List = (VersionFour)malloc(sizeof(struct node));
    (*List)->count = 0;
    (*List)->Elements = (ElementType*)malloc(sizeof(ElementType) * MAX);
}


void read(VersionFour List){
    printf("List:\n");
    for(int trav = 0; trav < List->count; trav++){
        printf("%d%s", List->Elements[trav].data, (trav != List->count-1)? ", ": ".\n");
    }
}


void insert(VersionFour List, int data){
    if(List->count != MAX){
        List->Elements[List->count++].data = data;
    }else{
        printf("\n\nInsertion failed\n\n");
    }
}


void delete(VersionFour List, int data){
    int trav = 0;
    for(; trav < List->count && List->Elements[trav].data != data; trav++){}
    if(trav <= List->count){
        for(; trav < List->count; trav++){
            List->Elements[trav].data = List->Elements[trav+1].data;
        }
        List->count--;
    }else{
        printf("\n\nDeletion Failed!");
    }
}


bool member(VersionFour List, int data){
    int trav = 0;
    for(; trav < List->count && List->Elements[trav].data != data; trav++){}
    return(trav == List->count)? false: true;
}


int locate(VersionFour List, int data){
    int trav = 0;
    for(; trav < List->count && List->Elements[trav].data != data; trav++){}
    return(trav == List->count)? -1: trav;
}