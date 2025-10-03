#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define defaultMAX 4
#define bitSize 8


/*

Computer word (ARRAY)

*/

typedef unsigned char SET;
typedef unsigned int position;

typedef struct{
    SET *sets;
    int setSize;
    int count;
} ArrSets;

// Operations
void initSet(SET*);
void insertBit(SET*, position);
void populateSet(SET*);
void removeBit(SET*, position);
void displayBits(SET);
// Mid-complex
void invertSet(SET*);
void swapBit(SET*, position, position);
void displayChar(SET);
// Array of SETS
void initSetToArray(ArrSets*);
void addSetToArray(ArrSets*);
void removeSetFromArray(ArrSets*);
void displaySetsFromArray(ArrSets);
int resizeArrSize(ArrSets*);

// Tool
void clearInputBuffer();
int verifyInput(int);
void cooldown(int count, int microsecond);
void returnMenu(int count, int microseconds);

// UI
void menu(ArrSets*);
void manageSets(ArrSets*);
void selectedSet(SET*);



int main(){
    ArrSets ArrS = {0};
    initSetToArray(&ArrS);
    // SET S;
    // initSet(&S);
    // selectedSet(&S);

    menu(&ArrS);

    printf("\n\nComputerWord Terminated!\n\n\n");
    return 0;
}




// CREATE
void initSet(SET *S){
    *S = 0;
}

void insertBit(SET *S, position p){
    if(p < bitSize){
        SET mask = 1;
        *S |= (mask << p);
    }
}

void populateSet(SET *S){
    int input;
    printf("Input 8 digits [0-1]: ");
    scanf("%d", &input);
    initSet(S);
    for(int trav = 0; input; input/=10, trav++){
        if(input % 10 != 0){
            *S |= (1 << trav);
        }
    }
    cooldown(2, 50000);
}

// READ
void displayBits(SET S){
    printf("\n");
    for(int trav = 7; trav >= 0; trav--){
        printf("%d", (S >> trav) & 1);
    }
    printf("\n");
}

void displayChar(SET S){
    printf("%c\n", S);
}

// UPDATE
void invertSet(SET *S){
    *S = ~(*S);
}

void swapBit(SET *S, position p1, position p2){
    if(p1 < bitSize && p2 < bitSize){
        SET bit1 = (*S >> p1) & 1;
        SET bit2 = (*S >> p2) & 1;
        SET mask = 1;
        if(bit1 != bit2) *S = *S ^ (mask << p1) | (mask << p2);
    }
}

// DELETE
void removeBit(SET *S, position p){
    if(p < bitSize){
        SET mask = 0;
        *S &= ~(1 << p);
    }
}



void initSetToArray(ArrSets *AS){
    if(AS->sets != NULL) free(AS->sets);
    AS->sets = (SET*)malloc(sizeof(SET) * defaultMAX);
    AS->count = 0;
    AS->setSize = defaultMAX;
}

void addSetToArray(ArrSets *AS){
    int resized = 0;
    if(AS->count == AS->setSize) resized = resizeArrSize(AS);
    if(resized || AS->count < AS->setSize){
        initSet(&AS->sets[AS->count]);
        AS->count++;
    }
}

void removeSetFromArray(ArrSets *AS){
    if(AS->count > 0) AS->count--;
}

void displaySetsFromArray(ArrSets AS){
    for(int trav = 0; trav < AS.count; trav++){
        displayBits(AS.sets[trav]);
    }
    cooldown(5 + AS.count, 500000);
}

int resizeArrSize(ArrSets* AS){
    SET *newSets = realloc(AS->sets, AS->setSize * 2 * sizeof(SET));
    if(newSets != NULL){
        AS->sets = newSets;
        AS->setSize *= 2;
        return 1;
    }else{
        printf("\n\nResizing Array failed!\n\n");
        return 0;
    }
}


// TOOLS (KINGKONG)

int verifyInput(int result){
    if(result != 1){
        printf("INVALID INPUT\n");
        cooldown(30, 15000);
        clearInputBuffer();
        return 0;
    }
    return 1;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void cooldown(int count, int microseconds){
    for(int i = 0; i < count; i++){
        printf(".");
        fflush(stdout);
        usleep(microseconds);
    }
}

void returnMenu(int count, int microseconds){
    printf("Returning back to the Menu.");
    for(int i = 3; i >= 0; i--){
        printf("%d", i);
        cooldown(count, microseconds);
    }
}



// UI

void menu(ArrSets *ArrS){
    int input, result;
    
    do{
        system("clear");
        printf("Main Menu:\n1: Add Set\n2: Remove Set\n3: Display Sets\n4: Manage Sets\n0: Exit\nInsert Input [1-4]: ");
        result = scanf("%d", &input);
        if(verifyInput(result)){
            switch(input){
                case 1:
                    system("clear");
                    addSetToArray(ArrS);
                    break;
                case 2:
                    system("clear");
                    removeSetFromArray(ArrS);
                    break;
                case 3:
                    system("clear");
                    displaySetsFromArray(*ArrS);
                    break;
                case 4:
                    system("clear");
                    manageSets(ArrS);
                    break;
            }
        }
    }while(input != 0);
}

void manageSets(ArrSets *ArrS){
    int input, result;
    printf("Select Set{%d <-> %d}: ", (ArrS->count != 0)? 0: -1, (ArrS->count != 0)? ArrS->count: -1);
    result = scanf("%d", &input);
    if(verifyInput(result) && input >= 0 && input < ArrS->count){
        selectedSet(&ArrS->sets[input]);
    }else{
        verifyInput(0);
    }
}

void selectedSet(SET *S){
    position p1, p2;
    int input, result;
    do{
        system("clear");
        printf("Selected set: ");
        displayBits(*S);
        printf("\n\n1: Insert\n2: Remove\n3: Invert Set\n4: Swap Bit\n5: Display:\n6: Remove All\n7: Display (char)\n8: Populate Set\n0: Exit\n[Insert Input 1-4]: ");
        result = scanf("%d", &input);
        if(verifyInput(result)){
            switch (input){
                case 1:
                    system("clear");
                    printf("Insert at position [0-7]: ");
                    result = scanf("%d", &p1);
                    if(verifyInput(result)) insertBit(S, p1);
                    break;
                case 2:
                    system("clear");
                    printf("Remove at position [0-7]: ");
                    result = scanf("%d", &p1);
                    if(verifyInput(result)) removeBit(S, p1);
                    break;
                case 3:
                    system("clear");
                    printf("Do you want to invert the Set? [1/0]: ");
                    result = scanf("%d", &input);
                    if(verifyInput(result) && input) invertSet(S);
                    break;
                case 4:
                    system("clear");
                    printf("Swap positions a <-> b:\na: ");
                    result = scanf("%d", &p1);
                    if(verifyInput(result)){
                        printf("b: ");
                        result = scanf("%d", &p2);
                        if(verifyInput(result)) swapBit(S, p1, p2);
                    }
                    break;
                case 5:
                    system("clear");
                    displayBits(*S);
                    returnMenu(3, 500000);
                    break;
                case 6:
                    system("clear");
                    printf("Do you want to clear the Set? [1/0]: ");
                    result = scanf("%d", &input);
                    if(verifyInput(result) && input) initSet(S);
                case 7:
                    system("clear");
                    displayChar(*S);
                    returnMenu(3, 500000);
                    break;
                case 8:
                    system("clear");
                    populateSet(S);
                default:
                    break;
            }
        }
    }while(input != 0);
}