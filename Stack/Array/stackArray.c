#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 10

typedef struct{
    int data;
} ElementType;

typedef struct{
    ElementType Elements[MAX];
    int top;
} StackArray;

void init(StackArray*);
void read(StackArray);
bool isEmpty(StackArray);
bool isFull(StackArray);
ElementType top(StackArray);
void pop(StackArray*);
void popAll(StackArray*);
void push(StackArray*, int data);



int main(){
    StackArray S;

    init(&S);

    push(&S, 1);
    push(&S, 2);
    push(&S, 3);
    push(&S, 4);
    push(&S, 5);

    read(S);

    popAll(&S);
    read(S);


    return 0;
}


void init(StackArray *S){
    S->top = -1;
}


void read(StackArray S){
    printf("Stack:\n");
    if(isEmpty(S)){
        printf("EMPTY\n");
    }

    while(!isEmpty(S)){
        ElementType T = top(S);
        printf("%d", T.data);
        pop(&S);
        printf("%s", (!isEmpty(S)? ", ": ".\n"));
    }
}


void popAll(StackArray *S){
    while(!isEmpty(*S)){
        pop(S);
    }
}


bool isEmpty(StackArray S){
    return(S.top == -1)? true: false;
}


bool isFull(StackArray S){
    return(S.top == MAX - 1)? true: false;
}

ElementType top(StackArray S){
    ElementType d = {-1};
    return(!isEmpty(S)) ? S.Elements[S.top] : d;
}

void pop(StackArray *S){
    if(!isEmpty(*S)){
        S->top--;
    }
}

void push(StackArray *S, int data){
    if(!isFull(*S)){
        S->Elements[++S->top].data = data;
    }
}