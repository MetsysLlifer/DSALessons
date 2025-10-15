#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 5

typedef struct{
    int data;
} ElementType;

typedef struct{
    ElementType Elements[MAX];
    int top;
} StackArray;

void init(StackArray*);
void read(StackArray);
void readTopToBottom(StackArray);
void readBottomToTop(StackArray);
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
    readBottomToTop(S);

    popAll(&S);
    read(S);


    return 0;
}


void init(StackArray *S){
    S->top = MAX;
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


void readBottomToTop(StackArray S){
    if(isEmpty(S)){
        printf("\nStack is Empty!\n");
    }else{
        for(int trav = MAX-1; trav >= 0; trav--){
            printf("%d", S.Elements[trav].data);
            printf("%s", (trav-1 >= 0? ", ": ".\n"));
        }
    }
}


void popAll(StackArray *S){
    while(!isEmpty(*S)){
        pop(S);
    }
}


bool isEmpty(StackArray S){
    return(S.top == MAX)? true: false;
}


bool isFull(StackArray S){
    return(S.top == 0)? true: false;
}

ElementType top(StackArray S){
    ElementType d = {-1};
    return(!isEmpty(S)) ? S.Elements[S.top] : d;
}

void pop(StackArray *S){
    if(!isEmpty(*S)){
        ++S->top;
    }
}

void push(StackArray *S, int data){
    if(!isFull(*S)){
        S->Elements[--S->top].data = data;
    }
}