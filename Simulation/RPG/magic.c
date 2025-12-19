#include "game.h"

// The pure data fusion logic (Adapted from your old code)
Spell FuseSpellData(Spell A, Spell B) {
    Spell C;
    
    // Basic fusion logic
    const char* fusedName = "Fused Magic";
    memcpy(C.name, fusedName, strlen(fusedName)+1);
    
    C.type = ELEMENT_UNKNOWN; 
    C.form = (A.intensity > B.intensity) ? A.form : B.form;
    
    // Weighted Averages
    double totalIntensity = A.intensity + B.intensity;
    if (totalIntensity == 0) totalIntensity = 1.0; // Prevent divide by zero

    C.temperature = ((A.temperature * A.intensity) + (B.temperature * B.intensity)) / totalIntensity;
    C.dryness = ((A.dryness * A.intensity) + (B.dryness * B.intensity)) / totalIntensity;
    
    // New intensity is average
    C.intensity = totalIntensity / 2.0;
    C.complete = true;

    return C;
}

// Convert Spell Properties to Raylib Color
Color GetSpellColor(Spell s) {
    // Logic: Temp affects Red/Blue. Form affects Alpha/Darkness.
    
    unsigned char r = 0, g = 0, b = 0;
    unsigned char a = 255; // Alpha

    // Temperature coloring
    if (s.temperature > 50) {
        r = 255; // Hot -> Red
        g = (unsigned char)(255 - (s.temperature * 2)); 
    } else {
        b = 255; // Cold -> Blue
        g = (unsigned char)(s.temperature * 2);
    }

    // Form coloring adjustments
    if (s.form == FORM_SOLID) {
        r /= 2; g /= 2; b /= 2; // Darker
    } else if (s.form == FORM_PLASMA) {
        a = 200; // Transparent glow
        if (r < 100) r = 100; // Make sure it's bright
    }

    return (Color){r, g, b, a};
}

// Factory function to create an Entity from Spell Data
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
        e.mass = 0.5f; // Very light
        e.size = 20.0f;
        e.friction = 1.0f;
    } else {
        e.mass = 5.0f;
        e.size = 12.0f;
        e.friction = 2.0f;
    }

    e.maxSpeed = 1000.0f;
    e.moveForce = 0.0f; // Projectiles don't have engines
    e.color = GetSpellColor(s);

    return e;
}