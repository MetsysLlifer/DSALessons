#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct{
    int data;
} ElementType;

typedef struct node{
    ElementType element;
    struct node *next;
} *Nodetype;

typedef struct{
    Nodetype front;
    Nodetype rear;
} QueueLinkedList;

void init(QueueLinkedList*);
void front(QueueLinkedList);
void rear(QueueLinkedList);
void enqueue(QueueLinkedList*, int data);
void dequeue(QueueLinkedList*);
bool isEmpty(QueueLinkedList);
// bool isFull(QueueLinkedList); 


int main(){
    QueueLinkedList Q;

    init(&Q);
    enqueue(&Q, 1);
    enqueue(&Q, 2);
    enqueue(&Q, 3);
    enqueue(&Q, 7);
    front(Q);
    rear(Q);

    return 0;
}


void init(QueueLinkedList *Q){
    Q->front = NULL;
    Q->rear = NULL;
}

void enqueue(QueueLinkedList *Q, int data){
    Nodetype temp = (Nodetype)malloc(sizeof(struct node));
    if(temp != NULL){
        temp->element.data = data;
        temp->next = NULL;

        if(isEmpty(*Q)){
            Q->front = temp;
        }else{
            Q->rear->next = temp;
        }
        Q->rear = temp;
    }
}

void dequeue(QueueLinkedList *Q){
    if(!isEmpty(*Q)){
        Nodetype toDelete = Q->front;
        Q->front = Q->front->next;
        if(isEmpty(*Q)){
            Q->rear = NULL;
        }
        free(toDelete);
    }
}

void front(QueueLinkedList Q){
    printf("%d\n", Q.front->element.data);
}

void rear(QueueLinkedList Q){
    printf("%d\n", Q.rear->element.data);
}

bool isEmpty(QueueLinkedList Q){
    return (Q.front == NULL)? true: false;
}