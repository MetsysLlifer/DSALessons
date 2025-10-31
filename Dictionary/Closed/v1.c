#include <stdio.h>
#include <string.h>
#define MAX 10


typedef enum{
    EMPTY,
    FULL,
    DELETED
} Status;

const char* statusName[3] = {"Empty", "Full", "Deleted"};

typedef struct{
    int data;
    Status status;
} NodeType;


typedef int Data, Index;
typedef NodeType Dictionary[MAX];

int hash(Data);
void initDict(Dictionary);
void insertDict(Dictionary, Data);
void displayDict(Dictionary);

void checkStatsAll(Dictionary);
void completeData(Dictionary, Data);

int main(){
    Dictionary D;

    initDict(D);
    
    insertDict(D, 1);
    insertDict(D, 29);
    insertDict(D, 10);

    checkStatsAll(D);
    displayDict(D);

    completeData(D, 1);
    completeData(D, 29);
    completeData(D, 10);

    return 0;
}

int hash(Data data){
    Data sum = 0;
    while(data != 0){
        sum += (data % 10);
        data /= 10;
    }
    return sum % MAX;
}

void initDict(Dictionary D){
    Dictionary temp = {};
    memcpy(D, temp, sizeof(temp));
}

void insertDict(Dictionary D, Data data){
    Index idx = hash(data);
    Index start = idx;
    do{
        if(D[idx].status == EMPTY || (D[idx].status == DELETED)){
            D[idx].data = data;
            D[idx].status = FULL;
            return;
        } else if (D[idx].data == data && D[idx].status == FULL){
            return;
        }
        idx = (idx + 1) % MAX;
    } while(idx != start);
}

void displayDict(Dictionary D){
    for(int trav = 0; trav < MAX; trav++){
        printf("%d", D[trav].data? 1: 0);
    }
    printf("\n");
}

void checkStatsAll(Dictionary D){
    for(int trav = 0; trav < MAX; trav++){
        printf("D[%d]: %s\n", trav, statusName[D[trav].status]);
    }
}

void completeData(Dictionary D, Data data){
    printf("\n\nD: %d", data);
    Index idx = hash(data);
    Index trav = idx;
    do{
        if(D[trav].data == data && D[trav].status == FULL){
            printf("\tIndex: %d\n\tSearch: %d\n", trav, trav - idx + 1);
        }else if(D[trav].data == data && D[trav].status == DELETED){
            return;
        }
        trav = (trav + 1) % MAX;
    }while(trav != idx);
}