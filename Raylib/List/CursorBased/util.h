#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "raylib.h"


#define MAX 10
#define SCREENWIDTH 800
#define SCREENHEIGHT 450


typedef struct{
    int data;
} ElementType;

typedef struct{
    ElementType elem;
    int link;
} NodeType;

typedef struct{
    NodeType Nodes[MAX];
    int avail;
} VirtualHeap;

typedef int List, Data;

void init(List*);
void read(List, VirtualHeap);
void insertFirst(List*, VirtualHeap*, Data);
void insertLast(List*, VirtualHeap*, Data);
void Delete(List*, VirtualHeap*, Data);
bool member(List, VirtualHeap, Data);
int locate(List, VirtualHeap, Data);

// CursorBased Functions
void initVH(VirtualHeap*);
int mallocVH(VirtualHeap*);
void freeVH(VirtualHeap*, int index);


// UI
void drawVirtualHeap(VirtualHeap VH);
void drawList(List L);
void linkConnect(VirtualHeap VH, List L);

#endif // UTIL_H
