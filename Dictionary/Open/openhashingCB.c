#include <stdio.h>
#define VHMAX 10
#define MAX 4

typedef int Data, Index;

typedef struct{
    Data data;
    Index link;
} ElementType;

typedef struct{
    ElementType elements[VHMAX];
    Index avail;
} VirtualHeap;

typedef int Dictionary[MAX];

int hash(Data);
void initDict(Dictionary);
void insert(Dictionary, VirtualHeap*, Data);
void delete(VirtualHeap*);
void displayDict(Dictionary, VirtualHeap);

void initVH(VirtualHeap*);
Index mallocVH(VirtualHeap*);
void freeVH(VirtualHeap*);


int main(){
    VirtualHeap VH;
    Dictionary D;

    initVH(&VH);
    initDict(D);
    insert(D, &VH, 1);
    insert(D, &VH, 2);
    insert(D, &VH, 7);
    insert(D, &VH, 8);
    insert(D, &VH, 123);
    insert(D, &VH, 1541);
    insert(D, &VH, 246);
    insert(D, &VH, 34);
    insert(D, &VH, 9);

    displayDict(D, VH);

    return 0;
}

int hash(Data data){
    return data % MAX;
}

void initDict(Dictionary D){
    for(int trav = 0; trav < MAX; trav++){
        D[trav] = -1;
    }
}

void insert(Dictionary D, VirtualHeap *VH, Data data){
    Index idx = hash(data);
    int *trav = &D[idx];

    for(; *trav != -1 && VH->elements[*trav].data < data; trav = &VH->elements[*trav].link){}

    if(*trav == -1 || VH->elements[*trav].data != data){
        Index avail = mallocVH(VH);
        if(avail != -1){
            VH->elements[avail].data = data;
            VH->elements[avail].link = *trav;
            *trav = avail;
        }
    }
}

void displayDict(Dictionary D, VirtualHeap VH){
    for(int trav = 0; trav < MAX; trav++){
        if(D[trav] != -1){
            printf("Dictionary [%d]\n", trav);
            Index temp = D[trav];
            while(temp != -1){
                printf("\t- %d\n", VH.elements[temp].data);
                temp = VH.elements[temp].link;
            }
            printf("\n");
        }
    }
}



void initVH(VirtualHeap *VH){
    VH->avail = 0;
    for(int trav = 0; trav < VHMAX; trav++){
        VH->elements[trav].data = 0;
        VH->elements[trav].link = trav+1;
    }
    VH->elements[VHMAX-1].link = -1;
}

Index mallocVH(VirtualHeap *VH){
    Index idx = VH->avail;
    if(idx != -1){
        VH->avail = VH->elements[idx].link;
    }
    return idx;
}