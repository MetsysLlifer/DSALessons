#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define MAX 8
#define bitSize 8
#define maxSpeed 1000000
#define maxLoop 1000
// #define maxLoop 320/(4*10)



/*
unsigned int - 8
unint16_t - 16
*/ 

// typedef uint16_t orb, position;
typedef unsigned int orb, position;

typedef struct{
    orb blue[bitSize];
    orb red[bitSize];
    orb modifier[bitSize];
    int count;
    int loop;
} circle;

void displayRect(circle*);
void UnionOrbs(orb, orb);
void DifferenceOrbs(orb, orb);

void updateOrbs(circle*);
void loopDisplay(circle*, int, int);


int checkLoop(circle*);


// TOOL
void clearInputBuffer();
int verifyInput(int);
void cooldown(int count, int microsecond);
void cooldownNoText(int count, int microseconds);
void returnMenu(int count, int microseconds);


int main(){
    system("clear");
    // circle C = {{1, 2, 3, 4, 5, 6, 7, 8}, {9, 10, 11, 12, 13, 14, 15, 16}, {9, 10, 11, 12, 13, 14, 15, 16}, 0, 1};
    circle C = {{1, 2, 3, 4, 5, 6, 7, 8}, {9, 10, 11, 12, 13, 14, 15, 16}, {9, 10, 11, 12, 13, 14, 15, 16}, 0, 1};
    // circle C = {{255, 2, 5, 57, 88, 77, 67, 109}, {13, 120, 70, 77, 128, 64, 100, 200}, {11110001, 01111000, 01001010, 01001001, 10000000, 01011100, 01100000, 11010100}, 0 ,1};


    // displayRect(&C);
    loopDisplay(&C, maxLoop, maxSpeed);

}


void displayRect(circle *C){
    // prepares the output to display
    // for(int travOrb = 0; travOrb < bitSize; travOrb++){
    //     // C->modifier[travOrb] = C->blue[travOrb] | C->blue[(travOrb + 1) % 4];
    //     C->modifier[travOrb] = 0;
    //     // C->modifier[travOrb] = C->blue[travOrb] | C->red[travOrb];
    //     C->modifier[travOrb] = (C->modifier[travOrb]) | (C->red[travOrb]);
    // }

    // display output
    for(int travOrb = 0; travOrb < bitSize; travOrb++){
        for(int travBit = bitSize-1; travBit >= 0; travBit--){
            printf("%c ", ((C->red[travOrb] >> travBit) & 1)? 'o': ' ');
        }
        printf("\n");
    }
}


void updateOrbs(circle* C){
    // orb maxValue = C->blue[0] | C->red[0];

    // Update the red orb's value
    for(int travOrb = 0; travOrb < bitSize; travOrb++){
        // C->red[travOrb] = (++maxValue % (1 << 8));
        // C->red[travOrb] += 1 % (1 << bitSize);
        C->red[travOrb] ^= C->blue[travOrb];
    }

        // Updates the blue orb's value
    for(int travOrb = 0; travOrb < bitSize; travOrb++){
        // C->blue[travOrb] = C->blue[travOrb] | C->red[travOrb];
        // C->blue[travOrb] = C->red[travOrb];
        C->blue[travOrb] = (C->blue[travOrb] + 1) % (1 << bitSize);
        // if(C->blue[travOrb] > maxValue) maxValue = C->blue[travOrb];
    }
}


int checkLoop(circle *C){
    C->loop = 1;
    for(int travOrb = 0; travOrb < bitSize; travOrb++){
        C->loop = ((C->red[travOrb] == C->modifier[travOrb])) ? C->loop & 1: 0;
    }
    C->count++;
    return(C->loop == 1)? 1: 0;
}


void loopDisplay(circle* C, int count, int speed){
    system("clear");
    displayRect(C);
    updateOrbs(C);
    C->count++;
    usleep(speed * 1);

    for(int i = 0; i < count-1; i++){
        system("clear");
        displayRect(C);
        if(checkLoop(C)){
            printf("LOOP!!! ");
            break;
        }
        updateOrbs(C);
        usleep(speed * 1);
    }
    printf("Count: %d", C->count);
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