#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct{
    int data;
} ElementType;

typedef struct node{
    ElementType element;
    struct node *next;
} *StackLinkedList;

void init(StackLinkedList*);
// void readTopToBottom(StackLinkedList);
// void readBottomToTop(StackLinkedList);
void read(StackLinkedList);
ElementType top(StackLinkedList);
void push(StackLinkedList*, int data);
void pop(StackLinkedList*);
void popAll(StackLinkedList*);
bool isEmpty(StackLinkedList);
// bool isFull(StackLinkedList);   /*isFull funtion is not applicable in linked-list*/


int main(){
    StackLinkedList S;

    init(&S);
    push(&S, 1);
    push(&S, 2);
    push(&S, 3);

    read(S);
    popAll(&S);
    popAll(&S);



    return 0;
}


void init(StackLinkedList *S){
    *S = NULL;
}

ElementType top(StackLinkedList S){
    ElementType d = {-1};
    return(S != NULL)? S->element: d;
}

void read(StackLinkedList S){
    if(isEmpty(S)){
        printf("\nStack is empty!\n\n");
    }else{
        while(!isEmpty(S)){
            ElementType d = S->element;
            pop(&S);
            printf("%d", d.data);
            printf("%s", (S != NULL)? ", ": ".\n");
        }
    }
}

void push(StackLinkedList *S, int data){
    StackLinkedList temp = (StackLinkedList)malloc(sizeof(struct node));
    if(temp != NULL){
        temp->element.data = data;
        temp->next = *S;
        *S = temp;
    }
}

void pop(StackLinkedList *S){
    if(!isEmpty(*S)){
        StackLinkedList toDelete = *S;
        *S = (*S)->next;
        free(toDelete);
    }else{
        printf("\nThe stack is empty\n\n");
    }
}


void popAll(StackLinkedList *S){
    if(!isEmpty(*S)){
        while(!isEmpty(*S)){
            pop(S);
        }
    }else{
        printf("\nThe stack is empty\n\n");
    }
}


bool isEmpty(StackLinkedList S){
    return(S == NULL)? true: false;
}

