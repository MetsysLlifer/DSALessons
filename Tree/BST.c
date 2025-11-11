#include <stdio.h>
#include <string.h>
#define MAX 10
#define EMPTY -1

typedef int Index, Set[MAX];

typedef struct{
    Set elements;
    Index last;
} Tree;

void initTree(Tree*);
void insert(Tree*, Index);
void display(Tree);
void displayDetailed(Tree);


int main(){
    // Tree arr = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 9};
    // displayDetailed(arr);
    Tree arr;
    
    initTree(&arr);
    insert(&arr, 1);
    insert(&arr, 2);
    insert(&arr, 3);
    insert(&arr, 1);
    insert(&arr, 5);
    display(arr);

    return 0;
}


void initTree(Tree *arr){
    for(int trav = 0; trav < MAX; trav++){
        arr->elements[trav] = -1;
    }
    arr->last = EMPTY;
}

void insert(Tree *arr, Index key){
    if(arr->last == EMPTY){
        arr->elements[++arr->last] = key;
        return;
    }

    int current = 0; // start from the root tree
    int parent; // keep track of the parent node during traversal

    while(current <= arr->last && arr->elements[current] != EMPTY){
        parent = current;

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
    arr->elements[current] = key;
    if (current > arr->last) {
        arr->last = current;
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
    for(int i = 0; i < MAX / 2; i++){
        printf("Root [%d]: %d\n", i, arr.elements[i]);
        printf("\tchild/ren:\n\t\t");
        printf("%d ", arr.elements[2*i+1]);
        if(2*i+2 < MAX) printf("%d ", arr.elements[2*i+2]);
        printf("\n");
    }
}

