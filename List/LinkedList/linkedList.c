#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct node{
    int elem;
    struct node *next;
} *linkedList;

void init(linkedList*);
void read(linkedList);
void insertFirst(linkedList*, int);
void insertAt(linkedList*, int, int);
void insertLast(linkedList*, int);
void delete(linkedList*, int);
bool member(linkedList, int);
int locate(linkedList, int);


int main(){
    linkedList List;

    init(&List);

    insertLast(&List, 1);
    insertLast(&List, 2);
    insertLast(&List, 3);
    insertFirst(&List, -1);
    insertFirst(&List, -2);
    insertLast(&List, -3);

    delete(&List, 2);
    insertAt(&List, 6, 99);

    printf("Member: %d\n", member(List, -3));
    printf("Locate: %d\n", locate(List, -3));

    read(List);
}


void init(linkedList *List){
    *List = NULL;
}

void read(linkedList List){
    for(linkedList trav = List; trav != NULL; trav = trav->next){
        printf("%d%s", trav->elem, (trav->next != NULL)? ", ": ".\n");
    }
}


void insertFirst(linkedList *List, int data){
    linkedList newnode = (linkedList)malloc(sizeof(struct node));
    newnode->elem = data;
    newnode->next = *List;
    *List = newnode;
}


void insertAt(linkedList *List, int size, int data){
    linkedList newnode = (linkedList)malloc(sizeof(struct node));
    if(newnode == NULL){
        printf("Insertion failed!\n");
        return;
    }
    linkedList *trav = List;
    for(int count = 1; *trav != NULL && count != size; trav = &(*trav)->next, count++){}
    newnode->elem = data;
    newnode->next = *trav;
    *trav = newnode;
}


void insertLast(linkedList *List, int data){
    linkedList *trav;
    for(trav = List; *trav != NULL; trav = &(*trav)->next){}
    linkedList newnode = (linkedList)malloc(sizeof(struct node));
    if(newnode == NULL){
        printf("\n\nInsertion Failed!\n\n");
        return;
    }
    newnode->elem = data;
    newnode->next = NULL;
    *trav = newnode;
}


void delete(linkedList *List, int data){
    linkedList *trav;
    for(trav = List; *trav != NULL && (*trav)->elem != data; trav = &(*trav)->next){}
    if(trav != NULL){
        linkedList toDelete = (linkedList)malloc(sizeof(struct node));
        toDelete = *trav;
        *trav = (*trav)->next;
        free(toDelete);
    }
}


bool member(linkedList List, int data){
    linkedList trav;
    for(trav = List; trav != NULL && trav->elem != data; trav = trav->next){}
    return(trav == NULL)? false: true;
}


int locate(linkedList List, int data){
    linkedList trav;
    int counter = 0;
    for(trav = List; trav != NULL && trav->elem != data; trav = trav->next, counter++){}
    return(trav == NULL)? -1: counter;
}