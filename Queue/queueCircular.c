#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 3

typedef struct{
    int data;
} ElementType;

typedef struct{
    ElementType elements[MAX];
    int front;
    int rear;
} QueueCircularArray;


void init(QueueCircularArray*);
void front(QueueCircularArray);
void rear(QueueCircularArray);
void displayAll(QueueCircularArray);
void enqueue(QueueCircularArray*, int data);
void dequeue(QueueCircularArray*);
void dequeueAll(QueueCircularArray*);
bool isFull(QueueCircularArray);
bool isEmpty(QueueCircularArray);


int main(){
    QueueCircularArray Q;

    init(&Q);
    enqueue(&Q, 5);
    enqueue(&Q, 6);
    enqueue(&Q, 7);

    displayAll(Q);
    dequeueAll(&Q);
    displayAll(Q);

    return 0;
}


void init(QueueCircularArray *Q){
    Q->rear = -1;
    Q->front = 0;
    // Q->front = (Q->rear - 1) % MAX;
}

void enqueue(QueueCircularArray *Q, int data){
    if(!isFull(*Q)){
        Q->rear = (Q->rear + 1) % MAX;
        Q->elements[Q->rear].data = data;
    }else{
        printf("\nQueue is full!\n\n");
    }
}


void dequeue(QueueCircularArray *Q){
    if(!isEmpty(*Q)){
        ++Q->front;
    }
}

void dequeueAll(QueueCircularArray *Q){
    if(!isEmpty(*Q)){
        Q->front = (Q->rear + 1) % MAX;
    }
}

void front(QueueCircularArray Q){
    printf("%d\n", Q.elements[Q.front].data);
}

void displayAll(QueueCircularArray Q){
    if(!isEmpty(Q)){
        while(!isEmpty(Q)){
            front(Q);
            dequeue(&Q);
        }
    }else{
        printf("\nQueue is empty!\n\n");
    }
}

void rear(QueueCircularArray Q){
    printf("%d\n", Q.elements[Q.rear].data);
}

bool isEmpty(QueueCircularArray Q){
    return((Q.rear + 1) % MAX == Q.front)? true: false;
}

bool isFull(QueueCircularArray Q){
    return((Q.rear + 2) % MAX == Q.front)? true: false;
}