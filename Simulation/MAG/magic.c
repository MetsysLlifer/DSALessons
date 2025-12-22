#include "game.h"

Color GetElementColor(ElementType type) {
    switch (type) {
        case ELEM_EARTH: return ORANGE;      
        case ELEM_WATER: return SKYBLUE;     
        case ELEM_FIRE:  return RED;         
        case ELEM_AIR:   return PURPLE;      
        default: return LIGHTGRAY;
    }
}

Entity CreateRawElement(ElementType type, Vector2 pos) {
    Entity e = {0};
    e.position = pos;
    e.state = STATE_RAW;
    e.isActive = true;
    e.mass = 20.0f; 
    e.friction = 5.0f; 
    e.size = 15.0f;
    e.color = GetElementColor(type);
    e.speed = 0.0f; 
    e.steering_factor = 0.0f;
    
    // Core Data
    e.spellData.core = type;
    e.spellData.auxCount = 0;
    
    // Thermodynamics (Based on Type)
    if(type == ELEM_FIRE) { e.spellData.temperature = 100.0; e.spellData.dryness = 1.0; }
    else if(type == ELEM_WATER) { e.spellData.temperature = 20.0; e.spellData.dryness = 0.0; }
    else if(type == ELEM_EARTH) { e.spellData.temperature = 50.0; e.spellData.dryness = 1.0; }
    else if(type == ELEM_AIR) { e.spellData.temperature = 40.0; e.spellData.dryness = 0.5; }
    e.spellData.intensity = 0.5;
    
    return e;
}

// Logic to determine spell effects
void RecalculateStats(Spell* s) {
    s->power = 10.0f;
    s->hasHoming = false;
    s->isTeleport = false;
    s->behavior = SPELL_PROJECTILE;
    
    int airCount = 0, earthCount = 0;
    for(int i=0; i<s->auxCount; i++) {
        if(s->aux[i] == ELEM_FIRE) s->power += 20.0f;
        if(s->aux[i] == ELEM_AIR) { s->hasHoming = true; airCount++; }
        if(s->aux[i] == ELEM_EARTH) earthCount++;
    }
    
    if (s->core == ELEM_EARTH && airCount >= 2) s->isTeleport = true;
    else if (s->core == ELEM_EARTH && earthCount >= 1) s->behavior = SPELL_WALL;
    else if (s->core == ELEM_AIR && airCount >= 1) s->behavior = SPELL_TELEKINESIS;
    else if (s->core == ELEM_WATER && s->auxCount > 0) s->behavior = SPELL_HEAL;
}

// SPATIAL FUSION (Floor)
void PerformSpatialFusion(Entity* entities, int count, int coreIndex, ParticleSystem* ps) {
    Entity* core = &entities[coreIndex];
    Spell* s = &core->spellData;
    
    double totalTemp = s->temperature * s->intensity;
    double totalDry = s->dryness * s->intensity;
    double totalInt = s->intensity;

    for(int i = 1; i < count; i++) {
        if (i == coreIndex) continue; 
        if (!entities[i].isActive || entities[i].state != STATE_RAW) continue;

        float dist = Vector2Distance(core->position, entities[i].position);
        if (dist < FUSION_RADIUS) {
            // Add to Structure
            if (s->auxCount < MAX_AUX) s->aux[s->auxCount++] = entities[i].spellData.core;
            
            // Add to Math
            totalTemp += entities[i].spellData.temperature * entities[i].spellData.intensity;
            totalDry += entities[i].spellData.dryness * entities[i].spellData.intensity;
            totalInt += entities[i].spellData.intensity;
            
            SpawnExplosion(ps, entities[i].position, entities[i].color);
            entities[i].isActive = false; 
        }
    }
    
    if(totalInt > 0) {
        s->temperature = totalTemp / totalInt;
        s->dryness = totalDry / totalInt;
        s->intensity = totalInt;
    }
    
    RecalculateStats(s);
    core->state = STATE_PROJECTILE;
    core->size = 25.0f; 
    core->speed = 800.0f;           
    core->steering_factor = 2000.0f; 
    core->isSpell = true; 
    sprintf(s->name, "Fused Spell");
}

// COLLISION FUSION (Mid-Air)
Spell FuseSpellData(Spell A, Spell B) {
    Spell C = {0};
    C.core = (A.intensity > B.intensity) ? A.core : B.core;
    
    for(int i=0; i<A.auxCount && C.auxCount < MAX_AUX; i++) C.aux[C.auxCount++] = A.aux[i];
    for(int i=0; i<B.auxCount && C.auxCount < MAX_AUX; i++) C.aux[C.auxCount++] = B.aux[i];
    
    double totalIntensity = A.intensity + B.intensity;
    if (totalIntensity == 0) totalIntensity = 1.0; 

    C.temperature = ((A.temperature * A.intensity) + (B.temperature * B.intensity)) / totalIntensity;
    C.dryness = ((A.dryness * A.intensity) + (B.dryness * B.intensity)) / totalIntensity;
    C.intensity = totalIntensity;

    RecalculateStats(&C);
    sprintf(C.name, "Impact Fusion");
    return C;
}