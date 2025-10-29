#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 24
#define SETSIZE 8

typedef int Data, Index, Set;

typedef struct{
    Data data;
    Index next;
} ElementType;

typedef struct{
    ElementType elements[MAX];
    Index avail;
} VirtualHeap;

void initSet(Set*);
void populateSet(Set*, VirtualHeap*);
void populateModSet(Set*, VirtualHeap*);
void displaySet(Set S, VirtualHeap);
void Union(Set *C, Set A, Set B, VirtualHeap*);
void Intersection(Set *C, Set A, Set B, VirtualHeap*);
void Difference(Set *C, Set A, Set B, VirtualHeap*);


void insertFirst(Set*, VirtualHeap*, Data);
void insertLast(Set*, VirtualHeap*, Data);

// Virtual Heap
void initVH(VirtualHeap*);
int mallocVH(VirtualHeap*);
void freeVH(VirtualHeap*, Set);

int main(){
    srand(time(NULL));
    VirtualHeap VH;
    Set A;
    Set B;
    Set C;

    initVH(&VH);
    initSet(&A);
    initSet(&B);
    initSet(&C);

    printf("SET A: \t\t\t");
    populateSet(&A, &VH);
    displaySet(A, VH);

    printf("SET B: \t\t\t");
    populateSet(&B, &VH);
    displaySet(B, VH);

    printf("Union - SET C: \t\t");
    populateModSet(&C, &VH);
    Union(&C, A, B, &VH);
    displaySet(C, VH);

    printf("Intersection - SET C: \t");
    populateModSet(&C, &VH);
    Intersection(&C, A, B, &VH);
    displaySet(C, VH);

    printf("Difference - (A-B): \t");
    populateModSet(&C, &VH);
    Difference(&C, A, B, &VH);
    displaySet(C, VH);

    printf("Difference - (B-A): \t");
    populateModSet(&C, &VH);
    Difference(&C, B, A, &VH);
    displaySet(C, VH);

    return 0;
}


void initSet(Set *S){
    *S = -1;
}

void initVH(VirtualHeap *VH){
    VH->avail = 0;
    for(int i = 0; i < MAX; i++){
        VH->elements[i].data = 0;
        VH->elements[i].next = i+1;
    }
    VH->elements[MAX-1].next = -1;
}

void populateSet(Set *S, VirtualHeap *VH){
    for(int i = 0; i < SETSIZE; i++){
        Index avail = mallocVH(VH);
        if(avail != -1){
            Index *trav;
            for(trav = S; *trav != -1; trav = &(VH->elements[*trav].next)){}
            VH->elements[avail].data = rand() % 2;
            VH->elements[avail].next = -1;

            *trav = avail;
        }
    }
}

void populateModSet(Set *S, VirtualHeap *VH){
    for(int i = 0; i < SETSIZE; i++){
        Index avail = mallocVH(VH);
        if(avail != -1){
            Index *trav;
            for(trav = S; *trav != -1; trav = &(VH->elements[*trav].next)){}
            VH->elements[avail].data = 0;
            VH->elements[avail].next = -1;

            *trav = avail;
        }
    }
}

void insertFirst(Set *S, VirtualHeap *VH, Data data){
    Index avail = mallocVH(VH);
    if(avail != -1){
        VH->elements[avail].data = data;
        VH->elements[avail].next = *S;

        *S = avail;
    }
}

void insertLast(Set *S, VirtualHeap *VH, Data data){
    Index avail = mallocVH(VH);
    if(avail != -1){
        Index *trav;
        for(trav = S; *trav != -1; trav = &(VH->elements[*trav].next)){}
        VH->elements[avail].data = data;
        VH->elements[avail].next = -1;

        *trav = avail;
    }
}

void displaySet(Set S, VirtualHeap VH){
    for(int i = S; i != -1; i = VH.elements[i].next){
        printf("%d", VH.elements[i].data);
    }
    printf("\n");
}

void Union(Set *C, Set A, Set B, VirtualHeap *VH){
    Index aPtr = A;
    Index bPtr = B;
    Index *cPtr;

    for(cPtr = C; *cPtr != -1; cPtr = &VH->elements[*cPtr].next){
        VH->elements[*cPtr].data = VH->elements[aPtr].data | VH->elements[bPtr].data;
        aPtr = VH->elements[aPtr].next;
        bPtr = VH->elements[bPtr].next;
    }
}

void Intersection(Set *C, Set A, Set B, VirtualHeap *VH){
    Index aPtr = A;
    Index bPtr = B;
    Index *cPtr;

    for(cPtr = C; *cPtr != -1; cPtr = &VH->elements[*cPtr].next){
        VH->elements[*cPtr].data = VH->elements[aPtr].data & VH->elements[bPtr].data;
        aPtr = VH->elements[aPtr].next;
        bPtr = VH->elements[bPtr].next;
    }
}

void Difference(Set *C, Set A, Set B, VirtualHeap *VH){
    Index aPtr = A;
    Index bPtr = B;
    Index *cPtr;

    for(cPtr = C; *cPtr != -1; cPtr = &VH->elements[*cPtr].next){
        VH->elements[*cPtr].data = VH->elements[aPtr].data & (~VH->elements[bPtr].data);
        aPtr = VH->elements[aPtr].next;
        bPtr = VH->elements[bPtr].next;
    }
}

int mallocVH(VirtualHeap *VH){
    int temp = VH->avail;
    if(temp != -1){
        VH->avail = VH->elements[temp].next;
    }
    return temp;
}

void freeVH(VirtualHeap *VH, Index idx){
    if(idx >= 0 && idx < MAX){
        VH->elements[idx].next = idx;
        VH->avail = idx;
    }
}

void Delete(Set *S, VirtualHeap *VH, Data data){
    int *trav;
    for(trav = S; *trav != -1 && VH->elements[*trav].data != data; trav = &(VH->elements[*trav].next)){}
    if(*trav != -1){
        int temp = *trav;
        *trav = VH->elements[*trav].next;
        freeVH(VH, temp);
    }
}