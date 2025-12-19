#include "base.h"
#include "ui.h"

void createSpell(Spell *A){
    double input;
    Name name;
    A->complete = false;
    printf("\n\n---\tCreate Spell\t---\n\n");
    // Name
    printf("Name: ");
    scanf("%s", name);
    memcpy(A->name, name, sizeof(name));
    // TYPE
    printf("Type: ");
    scanf("%lf", &input);
    if(!verifyInput(input)) inputFailReturnMenu(3, pow(10, 5)); return;
    // FORM
    printf("Form: ");
    scanf("%lf", &input);
    if(!verifyInput(input)) inputFailReturnMenu(3, pow(10, 5));
    // Temperature
    printf("Type: ");
    scanf("%lf", &input);
    if(!verifyInput(input)) inputFailReturnMenu(3, pow(10, 5));
    // Intensity
    printf("Intensity: ");
    scanf("%lf", &input);
    if(!verifyInput(input)) inputFailReturnMenu(3, pow(10, 5));
    // Dryness
    printf("Dryness: ");
    scanf("%lf", &input);
    if(!verifyInput(input)) inputFailReturnMenu(3, pow(10, 5));
    A->complete = true;
}

Spell fuseSpell(Spell A, Spell B){
    Spell C;
    if((A.complete && B.complete) == false){
        C.complete = false;
        printf("Can't be fused: Incomplete Spell(s)\n");
        returnMenu(3, pow(10, 5));
        return C;
    }

    Name n = {};
    memcpy(C.name, n, sizeof(n));
    C.type = ELEMENT_UNKNOWN; // The type is set to unknown as for now
    C.form = (A.intensity > B.intensity)? A.form: B.form;
    C.temperature = ((A.temperature * A.intensity) + (B.temperature * B.intensity)) / (A.intensity + B.intensity);
    C.intensity = (A.intensity + B.intensity) / 2;
    C.dryness = ((A.dryness * A.intensity) + (B.dryness * B.intensity)) / (A.intensity + B.intensity);
    C.complete = true;
    return C;
}
