#include "base.h"
// TOOLS (KINGKONG)


void display(Spell A){
    printf("Spell: Unkown\n");
    printf("Type: %d\nForm: %d\nTemperature: %.2lf\nIntensity: %.2lf\nDryness: %.2lf", A.type, A.form, A.temperature, A.intensity, A.dryness);
}

void cooldown(int count, int microseconds){
    for(int i = 0; i < count; i++){
        printf(".");
        fflush(stdout);
        usleep(microseconds);
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int verifyInput(int result){
    if(result != 1){
        printf("INVALID INPUT\n");
        cooldown(30, 15000);
        clearInputBuffer();
        return 0;
    }
    return 1;
}

void returnMenu(int count, int microseconds){
    printf("Returning back to the Menu.");
    for(int i = 3; i >= 0; i--){
        printf("%d", i);
        cooldown(count, microseconds);
    }
}