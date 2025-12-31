#include "util.h"

void init(LinkedList *List)
{
    *List = NULL;
}

bool insert(LinkedList* List, int data, Vector2 position, float* timer)
{
    LinkedList newnode = (LinkedList)malloc(sizeof(struct node));
    LinkedList* trav = List;
    if(newnode != NULL){
        for(; *trav != NULL; trav = &(*trav)->next){}
        ElementType elem = {data, 10, (Color){10 + GetRandomValue(10, 255), 10 + GetRandomValue(10, 255), 10 + GetRandomValue(10, 255), 255}, position};
        newnode->Element = elem;
        newnode->next = NULL;
        *trav = newnode;
    }else{
        *timer = 2.0;
        return false;
    }
    return true;
}

int deleteLast(LinkedList* List, float* timer)
{
    if(*List != NULL){
        LinkedList* trav = List;
        for(; (*trav)->next != NULL; trav = &(*trav)->next){}
        LinkedList toDelete = *trav;
        *trav = NULL;
        free(toDelete);
    }else{
        *timer = 2.0;
        return 0;
    }
    return 1;
}