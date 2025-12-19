#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 4
#define EMPTYLAST -1
// #define EMPTY (int)(pow(2, 32) - 1)

typedef int element, index;

typedef struct{
    element Elements[MAX];
    index lastIdx;
} SET;

void init(SET*);
void insert(SET*, element);
int deletemin(SET*);
void sort(SET*);
void display(SET);


int main(){
    SET A;
    // printf("%d", EMPTY);
    init(&A);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    // insert(&A, rand() % 100);
    insert(&A, 5);
    insert(&A, 15);
    insert(&A, 7);
    insert(&A, 3);
    insert(&A, 20);
    insert(&A, 10);
    insert(&A, 11);
    sort(&A);
    display(A);

    return 0;
}


void init(SET* A){
    A->lastIdx = EMPTYLAST;
}

void insert(SET *A, element elem){
    if(A->lastIdx + 1 < MAX){
        A->Elements[++A->lastIdx] = elem;

        index node = A->lastIdx;
        if(A->Elements[node] < A->Elements[(node-1)/2]){
            while(A->Elements[node] < A->Elements[(node-1)/2]){
                element temp = A->Elements[(node-1)/2];
                A->Elements[(node-1)/2] = A->Elements[node];
                A->Elements[node] = temp;
                node = (node-1)/2;
            }
        }

    }
}

int deletemin(SET* A){
    element temp = A->Elements[0];
    A->Elements[0] = A->Elements[A->lastIdx];
    A->Elements[A->lastIdx--] = temp;

    index smallest = 0;
    bool satisfied = false;
    while(!satisfied){
        index root = smallest;
        index lc = root * 2 + 1;
        index rc = root * 2 + 2;
        
        if(lc <= A->lastIdx && A->Elements[lc] < A->Elements[smallest]) smallest = lc;
        if(rc <= A->lastIdx && A->Elements[rc] < A->Elements[smallest]) smallest = rc;

        if(root != smallest){
            element temp = A->Elements[root];
            A->Elements[root] = A->Elements[smallest];
            A->Elements[smallest] = temp;
        } else{
            satisfied = true;
        }
    }
    return temp;
}

void sort(SET* A){
    element temp_element[MAX];
    index tempIdx = A->lastIdx;
    for(int trav = 0; trav <= tempIdx; trav++){
        temp_element[trav] = deletemin(A);
        printf("Element [%d]: %d\n", trav, temp_element[trav]);
    }
    
    A->lastIdx = tempIdx;
    for(int i = 0; i <= A->lastIdx; i++){
        A->Elements[i] = temp_element[i];
    }
}

void display(SET A){
    for(int trav = 0; trav <= A.lastIdx; trav++){
        printf("%d ", A.Elements[trav]);
    }
}