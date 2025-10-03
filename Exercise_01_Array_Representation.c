#include <stdio.h>
#include <stdbool.h>
#define MAX 10


typedef struct {
    char elem[MAX];
    int count;
} charList;

bool findElem(charList, char);
void deleteElem(charList*, char);
void deleteAllOccur(charList*, char);
void display(charList);

int main(){
    charList L = {"USSSKUTSVS", 10};

    display(L);

    printf("%d", findElem(L, 'U'));
    printf("%d", findElem(L, 'Z'));
    printf("%d", findElem(L, 'C'));

    deleteAllOccur(&L, 'S');
    display(L);
    // it should display 101 based on the print

    return 0;
}


bool findElem(charList L, char x){
    int trav;
    for(trav = 0; trav < L.count && L.elem[trav] != x; trav++){}
    return(trav != L.count)? true: false;
}


void deleteElem(charList* L, char x){
    int trav;
    for(trav = 0; trav < L->count && L->elem[trav] != x; trav++){}
    if(trav != L->count){
        for(; trav < L->count ; trav++){
            L->elem[trav] = L->elem[trav+1];
        }
        L->count--;
    }else{
        printf("\nNothing to remove: The element '%c' does not exist in the array\n\n", x);
    }
}


void deleteAllOccur(charList* L, char x){
    int ptr;
    for(ptr = 0; ptr < L->count;){
        if(L->elem[ptr] == x){
            for(int trav = ptr; trav < L->count; trav++){
                L->elem[trav] = L->elem[trav+1];
            }
            L->count--;
        }else ptr++;
    }
}


void display(charList L){
    printf("\n\nDISPLAY:\n");
    for(int trav = 0; trav < L.count; trav++){
        printf("%c", L.elem[trav]);
    }
    printf("\n\n");
}