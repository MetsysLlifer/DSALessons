#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 10


typedef struct{
    int data;
} ElementType;

typedef struct{
    ElementType *Elements;
    int count;
} VersionThree;

void init(VersionThree*);
void read(VersionThree);
void insert(VersionThree*, int);
void delete(VersionThree*, int);
bool member(VersionThree, int);
int locate(VersionThree, int);


int main(){
    VersionThree List;

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
    insert(&List, 12);
    insert(&List, 13);

    delete(&List, 14);

    printf("Member [14]: %d\n", member(List, 14));
    printf("Locate [5]: %d\n", locate(List, 5));

    read(List);
}


void init(VersionThree *List){
    List->count = 0;
    List->Elements = (ElementType*)malloc(sizeof(ElementType) * MAX);
    List->Elements[0].data = 1;
}


void read(VersionThree List){
    printf("List:\n");
    for(int trav = 0; trav < List.count; trav++){
        printf("%d%s", List.Elements[trav].data, (trav != List.count-1) ? ", ": ".\n");
    }
}


void insert(VersionThree *List, int data){
    if(List->count != MAX){
        List->Elements[List->count++].data = data;
    }else{
        printf("\n\nInsertion failed: Out of space\n\n");
    }
}


void delete(VersionThree *List, int data){
    int trav = 0;
    for(; trav < List->count && List->Elements[trav].data != data; trav++){}
    if(trav != List->count){
        for(; trav < List->count; trav++){
            List->Elements[trav].data = List->Elements[trav+1].data;
        }
        List->count--;
    }else{
        printf("\n\nDeletion failed: The data '%d' does not exist in the List\n\n", data);
    }
}


bool member(VersionThree List, int data){
    int trav = 0;
    for(; trav < List.count && List.Elements[trav].data != data; trav++){}
    return(trav != List.count)? true: false;
}


int locate(VersionThree List, int data){
    int trav = 0;
    for(; trav < List.count && List.Elements[trav].data != data; trav++){}
    return(trav < List.count)? trav: -1;
}