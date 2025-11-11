#include <stdio.h>
#define MAX 10

typedef int Index, Set[MAX];

typedef struct{
    Set elements;
    Index last;
} Tree;

void heapify(Tree);
void display(Tree);


int main(){
    // Set arr = {97, 84, 71, 69, 51, 44, 36, 22, 12, 1};
    // Set arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Tree arr = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 9};

    display(arr);

    return 0;
}



// void heapify(Set S){}

void display(Tree arr){
    for(int i = 0; i < MAX / 2; i++){
        printf("Root [%d]: %d\n", i, arr.elements[i]);
        printf("\tchild/ren:\n\t\t");
        printf("%d ", arr.elements[2*i+1]);
        if(2*i+2 < MAX) printf("%d ", arr.elements[2*i+2]);
        printf("\n");
    }
}

