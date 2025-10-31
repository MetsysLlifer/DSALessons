#include <stdio.h>
#include <stdlib.h>
#define MAX 10

typedef int Data;

typedef struct node{
    int data;
    struct node *next;
} *Set;

typedef Set Dictionary[MAX];

int hash(Data);
void initDict(Dictionary);
void insert(Dictionary, Data);
void delete(Dictionary, Data);
void displayDict(Dictionary);


int main(){
    Dictionary D;

    // int a = hash((int)'m');
    // printf("%d", a);

    initDict(D);
    insert(D, 124);
    insert(D, 44);
    insert(D, 444);
    insert(D, 4);
    insert(D, 77);
    insert(D, 7);
    insert(D, 7);
    insert(D, -123);

    delete(D, 124);
    delete(D, 44);

    displayDict(D);

    return 0; 
}


int hash(Data data){
    // int sum = 0;
    // while(data != 0){
    //     sum += (data % 10);
    //     data /= 10;
    // }
    return data % MAX;
}

void initDict(Dictionary D){
    for(int i = 0; i < MAX; i++){
        D[i] = NULL;
    }
}

void insert(Dictionary D, Data data){
    Data idx = hash(data);
    Set *trav;
    for(trav = &D[abs(idx)]; *trav != NULL && (*trav)->data < data; trav = &(*trav)->next){}

    if(*trav == NULL || (*trav)->data != data){
        Set newNode = (Set)malloc(sizeof(struct node));
        if(newNode != NULL){
            newNode->data = data;
            newNode->next = *trav;
            *trav = newNode;
        }
    }
}

void delete(Dictionary D, Data data){
    Data idx = hash(data);
    Set *trav;
    for(trav = &D[idx]; *trav != NULL && (*trav)->data != data; trav = &(*trav)->next){}
    if(*trav != NULL){
        Set toDelete = *trav;
        *trav = (*trav)->next;
        free(toDelete);
    }
}

void displayDict(Dictionary D){
    for(int trav = 0; trav < MAX; trav++){
        if(D[trav] != NULL){
            Set temp = D[trav];
            printf("Index [%d]\n", trav);
            while(temp != NULL){
                printf("  - %d\n", temp->data);
                temp = temp->next;
            }
        }
    }
}