#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX 10

typedef struct node {
  char elem;
  struct node *link;
}*charList; 


void init(charList*);
void insertFirst(charList*, char);
void insertAt(charList*, int, char);
void insertLast(charList*, char);

bool findElem(charList, char);
void display(charList);

void deleteElem(charList*, char);
void deleteAllOccur(charList*, char);

int main(){
  charList L;
  printf("ASD");
  init(&L);
  insertFirst(&L, 'a');
  insertLast(&L, 'b');
  insertAt(&L, 5, 'c');
  insertLast(&L, 'b');
  insertAt(&L, 3, 'x');
  deleteElem(&L, 'x');
  deleteAllOccur(&L, 'b');

  display(L);

  return 0;
}


void init(charList* L){
  *L= NULL;
}


void insertFirst(charList* L, char x){
  charList newnode = (charList)malloc(sizeof(struct node));
  if(!newnode){
    printf("Insertion failed!\n"); return;
  }
  newnode->elem = x;
  newnode->link = *L;
  *L = newnode;
}


void insertAt(charList *L, int idx, char x){
  charList newnode = (charList)malloc(sizeof(struct node));
  if(!newnode){
    printf("Insertion failed!\n"); return;
  }
  newnode->elem = x;
  charList *trav = L;
  for(int count = 1; *trav != NULL && count < idx; trav = &(*trav)->link, count++){}
  newnode->link = *trav;
  *trav = newnode;
}


void insertLast(charList *L, char x){
  charList newnode = (charList)malloc(sizeof(struct node));
  if(!newnode){
    printf("Insertion failed!\n"); return;
  }
  newnode->elem = x;
  newnode->link = NULL;
  charList *trav;
  for(trav = L; *trav != NULL; trav = &(*trav)->link){}
  *trav = newnode;
}

bool findElem(charList L, char x){
  charList trav;
  for(trav = L; trav != NULL && trav->elem != x; trav = trav->link){}
  return (trav!= NULL) ? true: false;
}

void display(charList L){
  charList trav;
  printf("\n\nLinked List Data:\n");
  for(trav = L; trav != NULL; trav = trav->link){
    printf("%c\n", trav->elem);
  }
}

void deleteElem(charList *L, char x){
  charList *trav;
  for(trav = L; *trav != NULL && (*trav)->elem != x; trav = &(*trav)->link){}
  if(trav != NULL){
    charList toDelete = *trav;
    *trav = toDelete->link;
    free(toDelete);
  }
}


void deleteAllOccur(charList *L, char x){
  charList *trav;
  for(trav = L; *trav != NULL;){
    if((*trav)->elem == x){
      charList toDelete = *trav;
      *trav = toDelete->link;
      free(toDelete);
    }else trav = &(*trav)->link;
  }
}