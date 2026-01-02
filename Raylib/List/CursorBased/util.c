#include "util.h"


void init(List *L){
    *L = -1;
}

void read(List L, VirtualHeap VH){
    printf("List:\n");
    for(int trav = L; trav != -1; trav = VH.Nodes[trav].link){
        printf("%d%s", VH.Nodes[trav].elem.data, (VH.Nodes[trav].link != -1)? ", ": ".\n");
    }
}

void insertFirst(List *L, VirtualHeap *VH, Data data){
    int avail = mallocVH(VH);
    if(avail != -1){
        VH->Nodes[avail].elem.data = data;
        VH->Nodes[avail].link = *L;
        *L = avail;
    }
}

void insertLast(List *L, VirtualHeap *VH, Data data){
    int avail = mallocVH(VH);
    if(avail != -1){
        int *trav;
        for(trav = L; *trav != -1; trav = &(VH->Nodes[*trav].link)){}
        VH->Nodes[avail].elem.data = data;
        VH->Nodes[avail].link = -1;
        *trav = avail;
    }
}

void Delete(List *L, VirtualHeap *VH, Data data){
    int *trav;
    for(trav = L; *trav != -1 && VH->Nodes[*trav].elem.data != data; trav = &(VH->Nodes[*trav].link)){}
    if(*trav != -1){
        int temp = *trav;
        *trav = VH->Nodes[*trav].link;
        freeVH(VH, temp);
    }
}




void initVH(VirtualHeap *VH){
    VH->avail = 0;

    for(int trav = 1; trav <= MAX; trav++){
        VH->Nodes[trav-1].link = trav;
        VH->Nodes[trav-1].elem.data = 0;
    }

    VH->Nodes[MAX-1].link = -1;
}

int mallocVH(VirtualHeap *VH){
    int temp = VH->avail;
    if(temp != -1){
        VH->avail = VH->Nodes[temp].link;
    }
    return temp;
}

void freeVH(VirtualHeap *VH, int index){
    if(index >= 0 && index < MAX){
        VH->Nodes[index].link = VH->avail;
        VH->avail = index;
    }
}