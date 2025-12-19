#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX 15
#define EMPTY -1

typedef int Index, Set[MAX];

typedef struct{
    Set elements;
    Index last;
} Tree;

void initTree(Tree*);
void insert(Tree*, int);
// void delete(Tree*, int);
void display(Tree);
void displayDetailed(Tree);


int main(){
    // Tree arr = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 9};
    // displayDetailed(arr);
    Tree arr;
    
    initTree(&arr);
    insert(&arr, 14);
    insert(&arr, 15);
    insert(&arr, 5);
    insert(&arr, 10);
    insert(&arr, 0);
    insert(&arr, 18);
    insert(&arr, 7);
    insert(&arr, 12);
    insert(&arr, 99);
    insert(&arr, 1);
    insert(&arr, 17);
    display(arr);
    displayDetailed(arr);

    return 0;
}


void initTree(Tree *arr){
    for(int trav = 0; trav < MAX; trav++){
        arr->elements[trav] = EMPTY;
    }
    arr->last = EMPTY;
}

void insert(Tree *arr, int key){
    if(arr->last == EMPTY){
        arr->elements[++arr->last] = key;
        return;
    }

    int current = 0; // start from the root tree

    while(current <= arr->last && current < MAX && arr->elements[current] != EMPTY){
        if(key < arr->elements[current]){
            current = current * 2 + 1; // left child
        }else if(key > arr->elements[current]){
            current = current * 2 + 2; // right child
        }else{
            return; // Key duplication -> cease searching for an insertion point
        }
    }

    // Insert key at current position
    // Update last if needed
    if(current < MAX){
        arr->elements[current] = key;
        if (current > arr->last) {
            arr->last = current;
        }
    }
}



void display(Tree arr){
    for(int trav = 0; trav < MAX; trav++){
        if(arr.elements[trav] == EMPTY)
            printf("_ ");  // underscore for empty
        else
            printf("%d ", arr.elements[trav]);
    }
    printf("\n");
}


void displayDetailed(Tree arr){
    char parent[12];
    char child[12];
    for(int posParent = 0; posParent < MAX / 2; posParent++){
        sprintf(parent, "%d", arr.elements[posParent]); // converts int to string
        printf("\nParent [%d]: %s\n", posParent, (arr.elements[posParent] != EMPTY) ? parent : "_");
        printf("\tchild/ren:\n\t\t");
        for(int posChild = 1; posChild < 3; posChild++){
            sprintf(child, "%d", arr.elements[2*posParent+posChild]);
            if(2*posParent+posChild < MAX) printf("%s ", (arr.elements[2*posParent+posChild] != EMPTY) ? child : "_");
        }
        printf("\n");
    }
}

