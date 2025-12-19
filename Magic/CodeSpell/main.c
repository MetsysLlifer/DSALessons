#include "base.h"
#include "ui.h"
#include "magic.h"


int main(){
    Spell Fire = {{"fire"}, ELEMENT_PLASMA, FORM_PLASMA, 100, 0.5, 0.7, 1};
    Spell Rock = {{"rock"}, ELEMENT_SOLID, FORM_SOLID, 100, 0.7, 0.4, 1};

    Spell C = fuseSpell(Fire, Rock);
    display(C);
    printf("\n\n");
    returnMenu(5, pow(10, 5));
    system("clear");
    createSpell(&C);
    return 0;
}