#include "game.h"

// The pure data fusion logic
Spell FuseSpellData(Spell A, Spell B) {
    Spell C;
    
    // Basic fusion name
    const char* fusedName = "Fused Magic";
    memcpy(C.name, fusedName, strlen(fusedName)+1);
    
    C.type = ELEMENT_UNKNOWN; 
    // Dominant form wins
    C.form = (A.intensity > B.intensity) ? A.form : B.form;
    
    // Weighted Averages for Temperature/Dryness
    double totalIntensity = A.intensity + B.intensity;
    if (totalIntensity == 0) totalIntensity = 1.0; 

    C.temperature = ((A.temperature * A.intensity) + (B.temperature * B.intensity)) / totalIntensity;
    C.dryness = ((A.dryness * A.intensity) + (B.dryness * B.intensity)) / totalIntensity;
    
    C.intensity = totalIntensity / 2.0;
    C.complete = true;

    return C;
}

Color GetSpellColor(Spell s) {
    unsigned char r = 0, g = 0, b = 0;
    unsigned char a = 255; 

    // Temperature coloring
    if (s.temperature > 50) {
        r = 255; // Hot -> Red
        g = (unsigned char)(255 - (s.temperature * 2)); 
    } else {
        b = 255; // Cold -> Blue
        g = (unsigned char)(s.temperature * 2);
    }

    // Form coloring
    if (s.form == FORM_SOLID) {
        r /= 2; g /= 2; b /= 2; // Darker
    } else if (s.form == FORM_PLASMA) {
        a = 200; // Transparent glow
        if (r < 100) r = 100; 
    }

    return (Color){r, g, b, a};
}

Entity CreateSpellEntity(Spell s, Vector2 pos, Vector2 velocity) {
    Entity e = {0};
    e.position = pos;
    e.velocity = velocity;
    e.spellData = s;
    e.isSpell = true;
    e.isActive = true;
    
    // Determine Physics based on Form
    if (s.form == FORM_SOLID) {
        e.mass = 20.0f;
        e.size = 15.0f;
        e.friction = 5.0f;
    } else if (s.form == FORM_PLASMA) {
        e.mass = 0.5f; 
        e.size = 20.0f;
        e.friction = 1.0f;
    } else {
        e.mass = 5.0f;
        e.size = 12.0f;
        e.friction = 2.0f;
    }

    e.maxSpeed = 1000.0f;
    e.moveForce = 0.0f; 
    e.color = GetSpellColor(s);

    return e;
}