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
void readTopToBottom(StackArray);
void readBottomToTop(StackArray);
ElementType top(StackArray);
void push(StackArray*, int data);
void pop(StackArray*);
void popAll(StackArray*);
bool isEmpty(StackArray);
bool isFull(StackArray);



int main(){
    StackArray S;

    init(&S);

    push(&S, 1);
    push(&S, 2);
    push(&S, 3);
    push(&S, 4);
    push(&S, 5);

    // readTopToBottom(S);

    // // popAll(&S);
    readTopToBottom(S);
    readBottomToTop(S);

    popAll(&S);
    popAll(&S);
    readTopToBottom(S);

    return 0;
}


void init(StackArray *S){
    S->top = -1;
}


void readTopToBottom(StackArray S){
    if(isEmpty(S)){
        printf("\nThe stack is empty!\n\n");
        return;
    }

    /*
    Display with for loop
    This can be used if it would not ask to use pop() and top() to display the data
    */
    // for(int trav = S.top; trav >= 0; trav--){
    //     printf("%d", S.Elements[trav].data);
    //     printf("%s", (trav > 0 ? ", ": ".\n"));
    // }


    /*
    Display with using top and pop functions (abstract)
    */
    while(!isEmpty(S)){
        ElementType data = top(S);
        printf("%d", S.Elements[S.top].data);
        pop(&S);
        printf("%s", (!isEmpty(S) ? ", ": ".\n"));
    }
}

void readBottomToTop(StackArray S){
    if(isEmpty(S)){
        printf("\nThe stack is empty!\n\n");
    }else{
        for(int trav = 0; trav <= S.top; trav++){
        printf("%d", S.Elements[trav].data);
        printf("%s", (trav < S.top ? ", " : ".\n"));
    }
    }
}

ElementType top(StackArray S){
    ElementType d = {-1};
    return(!isEmpty(S))? S.Elements[S.top]: d;
}

void push(StackArray *S, int data){
    if(!isFull(*S)){
        S->Elements[++S->top].data = data;
    }else{
        printf("\nThe Stack is full!\n\n");
    }
}

void pop(StackArray *S){
    if(!isEmpty(*S)){
        --S->top;
    }
}

void popAll(StackArray *S){
    if(isEmpty(*S)){
        printf("\nThe stack is already empty\n\n");
    }else {
        while(!isEmpty(*S)){
            pop(S);
        }
    }
}

bool isFull(StackArray S){
    return(S.top == MAX - 1)? true: false;
}

bool isEmpty(StackArray S){
    return(S.top == -1)? true: false;
}

