#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 8


typedef struct SetType{
    unsigned int data;
    struct SetType *next;
} *Set;

void initSet(Set*);
void populateSet(Set*);
void displaySet(Set S);
void Union(Set *C, Set A, Set B);
void Intersection(Set *C, Set A, Set B);
void Difference(Set *C, Set A, Set B);

int main(){
    Set A = NULL;
    Set B = NULL;
    Set C = NULL;

    initSet(&A);
    initSet(&B);
    initSet(&C);

    printf("SET A: ");
    populateSet(&A);
    displaySet(A);

    printf("SET B: ");
    populateSet(&B);
    displaySet(B);

    Union(&C, A, B);
    displaySet(C);

    Intersection(&C, A, B);
    displaySet(C);

    // Differene (A - B) & (B - A)
    printf("Difference (A - B): ");
    Difference(&C, A, B);
    displaySet(C);

    printf("Difference (B - A): ");
    Difference(&C, B, A);
    displaySet(C);

    return 0;
}

void initSet(Set *S){
    srand(time(NULL));
    if(*S != NULL){
        Set temp = *S;
        while(temp != NULL){
            Set toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }

    for(int i = 0; i < MAX; i++){
        Set temp = (Set)malloc(sizeof(struct SetType));
        temp->data = 0;
        temp->next = NULL;
        *S = temp;
        S = &(*S)->next;
    }
}


void populateSet(Set *S){
    while(*S != NULL){
        (*S)->data = rand() % 2; // +1 because it's 0-based index (0-1)
        S = &(*S)->next;
    }
}

void displaySet(Set S){
    while(S != NULL){
        printf("%d", S->data);
        S = S->next;
    }
    printf("\n\n");
}

void Union(Set *C, Set A, Set B){
    initSet(C);
    while(*C != NULL){
        (*C)->data = A->data | B->data;
        C = &(*C)->next;
        A = A->next;
        B = B->next;
    }
}

void Intersection(Set *C, Set A, Set B){
    while(*C != NULL){
        (*C)->data = A->data & B->data;
        C = &(*C)->next;
        A = A->next;
        B = B->next;
    }
}

void Difference(Set *C, Set A, Set B){
    while(*C != NULL){
        (*C)->data = A->data & (~B->data);
        C = &(*C)->next;
        A = A->next;
        B = B->next;
    }
}
// void Intersection(Set *C, Set A, Set B){}
// void Difference(Set *C, Set A, Set B){}