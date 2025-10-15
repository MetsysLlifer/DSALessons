#include <stdio.h>
#include <stdbool.h>
#define MAX 5

typedef struct{
    int data;
    int link;
} ElementType;

typedef struct{
    ElementType elements[MAX];
    int avail;
} stackCursorBased, stackVirtualHeap;


void init(stackCursorBased*);
void read(stackCursorBased);
ElementType top(stackCursorBased);
void push(stackCursorBased*, int data);
void pop(stackCursorBased*);
bool isEmpty(stackCursorBased);
bool isFull(stackCursorBased);


int main(){

    return 0;
}
