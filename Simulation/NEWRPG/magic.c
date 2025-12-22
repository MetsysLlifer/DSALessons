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
    
    e.maxSpeed = 0.0f; 
    e.moveForce = 0.0f;
    
    e.spellData.core = type;
    e.spellData.auxCount = 0;
    
    if(type == ELEM_FIRE) { e.spellData.temperature = 100.0; e.spellData.dryness = 1.0; }
    else if(type == ELEM_WATER) { e.spellData.temperature = 20.0; e.spellData.dryness = 0.0; }
    else if(type == ELEM_EARTH) { e.spellData.temperature = 50.0; e.spellData.dryness = 1.0; }
    else if(type == ELEM_AIR) { e.spellData.temperature = 40.0; e.spellData.dryness = 0.5; }
    e.spellData.intensity = 0.5;
    
    return e;
}

// Determines Physics based on Element
void ApplyElementPhysics(Entity* e) {
    if (e->spellData.core == ELEM_FIRE) {
        e->mass = 1.0f; e->maxSpeed = 1000.0f; e->friction = 1.0f;
    } else if (e->spellData.core == ELEM_EARTH) {
        e->mass = 50.0f; e->maxSpeed = 500.0f; e->friction = 10.0f;
    } else if (e->spellData.core == ELEM_WATER) {
        e->mass = 10.0f; e->maxSpeed = 700.0f; e->friction = 2.0f;
    } else { // AIR
        e->mass = 5.0f; e->maxSpeed = 900.0f; e->friction = 0.5f;
    }
    e->moveForce = 2000.0f;
}

void RecalculateStats(Spell* s) {
    s->power = 10.0f;
    s->hasHoming = false;
    s->isTeleport = false;
    s->behavior = SPELL_PROJECTILE;
    
    int airCount = 0, earthCount = 0, waterCount = 0;
    for(int i=0; i<s->auxCount; i++) {
        if(s->aux[i] == ELEM_FIRE) s->power += 20.0f;
        if(s->aux[i] == ELEM_AIR) { s->hasHoming = true; airCount++; }
        if(s->aux[i] == ELEM_EARTH) earthCount++;
        if(s->aux[i] == ELEM_WATER) waterCount++;
    }
    
    // --- RECIPES ---
    if (s->core == ELEM_EARTH && airCount >= 2) s->isTeleport = true;
    else if (s->core == ELEM_EARTH && earthCount >= 1) s->behavior = SPELL_WALL;
    else if (s->core == ELEM_WATER && s->auxCount > 0) s->behavior = SPELL_HEAL;
    
    // NEW HIDDEN POWERS
    else if (s->core == ELEM_FIRE && earthCount >= 1) s->behavior = SPELL_MIDAS; // Fire + Earth = Gold
    else if (s->core == ELEM_FIRE && airCount >= 2) s->behavior = SPELL_VOID;    // Fire + 2 Air = Black Hole
    else if (s->core == ELEM_EARTH && waterCount >= 1) s->behavior = SPELL_SLOW; // Earth + Water = Mud
    else if (s->core == ELEM_AIR && airCount >= 1) s->behavior = SPELL_TELEKINESIS; // Air + Air = Gravity Well
}

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
            if (s->auxCount < MAX_AUX) s->aux[s->auxCount++] = entities[i].spellData.core;
            
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
    core->isSpell = true; 
    ApplyElementPhysics(core);
    sprintf(s->name, "Fused Spell");
}

Spell FuseSpellData(Spell A, Spell B) {
    Spell C = {0};
    C.core = (A.intensity > B.intensity) ? A.core : B.core;
    for(int i=0; i<A.auxCount && C.auxCount < MAX_AUX; i++) C.aux[C.auxCount++] = A.aux[i];
    for(int i=0; i<B.auxCount && C.auxCount < MAX_AUX; i++) C.aux[C.auxCount++] = B.aux[i];
    
    double totalInt = A.intensity + B.intensity; if(totalInt==0) totalInt=1.0;
    C.temperature = ((A.temperature * A.intensity) + (B.temperature * B.intensity)) / totalInt;
    C.dryness = ((A.dryness * A.intensity) + (B.dryness * B.intensity)) / totalInt;
    C.intensity = totalInt;

    RecalculateStats(&C);
    sprintf(C.name, "Impact Fusion");
    return C;
}