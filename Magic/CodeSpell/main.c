#include "base.h"
#include "ui.h"


Spell fuseSpell(Spell, Spell);


int main(){
    Spell Fire = {{"fire"}, ELEMENT_PLASMA, FORM_PLASMA, 100, 0.5, 0.7};
    Spell Rock = {{"rock"}, ELEMENT_SOLID, FORM_SOLID, 100, 0.7, 0.4};

    Spell C = fuseSpell(Fire, Rock);
    display(C);
    return 0;
}


Spell fuseSpell(Spell A, Spell B){
    Spell C;
    Name n = {};
    memcpy(C.name, n, sizeof(n));
    C.type = ELEMENT_UNKNOWN; // The type is set to unknown as for now
    C.form = (A.intensity > B.intensity)? A.form: B.form;
    C.temperature = ((A.temperature * A.intensity) + (B.temperature * B.intensity)) / (A.intensity + B.intensity);
    C.intensity = (A.intensity + B.intensity) / 2;
    C.dryness = ((A.dryness * A.intensity) + (B.dryness * B.intensity)) / (A.intensity + B.intensity);
    return C;
}

