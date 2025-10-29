#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX 5

typedef struct SetType{
    unsigned int Elements[MAX];
} *Set;

void initSet(Set*);
void populateSet(Set*);
void displaySet(Set S);
void Union(Set *C, Set A, Set B);
void Intersection(Set *C, Set A, Set B);
void Difference(Set *C, Set A, Set B);

int main(){
    srand(time(NULL)); // sets the random seed based on the clock

    /* INITIALIZE SETS */
    Set A;
    Set B;
    Set C;
    
    initSet(&C);
    initSet(&A);
    initSet(&B);

    printf("SET C:\t");
    displaySet(C);

    populateSet(&A);
    printf("\n\tSET A:\t");
    displaySet(A);

    populateSet(&B);
    printf("\tSET B:\t");
    displaySet(B);

    /* END INITIALIZATION */


    /* MODIFICATION */
    initSet(&C);

    printf("\nUnion:\t\t");
    Union(&C, A, B);
    displaySet(C);

    printf("Intersection:\t");
    Intersection(&C, A, B);
    displaySet(C);

    printf("Diff (A-B):\t");
    Difference(&C, A, B);
    displaySet(C);

    return 0;
}

void initSet(Set *S){
    *S = (Set)malloc(sizeof(struct SetType));
    unsigned int temp[MAX] = {};
    memcpy((*S)->Elements, temp, sizeof(temp));
}

void populateSet(Set *S){
    int data;
    for(int i = 0; i < MAX; i++){
        data = rand() % 5 + 0;
        // printf("Element %d: %d\t", i, data);

        if (data >= 0 && data < MAX) (*S)->Elements[data] = 1;
    }
    // printf("\n\n");
}

void displaySet(Set S){
    for(int i = 0; i < MAX; i++){
        printf("%d ", S->Elements[i]);
    }
    printf("\n");
}

void Union(Set *C, Set A, Set B){
    for(int i = 0; i < MAX; i++){
        (*C)->Elements[i] = A->Elements[i] | B->Elements[i];
    }
}

void Intersection(Set *C, Set A, Set B){
    for(int i = 0; i < MAX; i++){
        (*C)->Elements[i] = A->Elements[i] & B->Elements[i];
    }
}

void Difference(Set *C, Set A, Set B){
    for(int i = 0; i < MAX; i++){
        (*C)->Elements[i] = A->Elements[i] & (~B->Elements[i]);
    }
}