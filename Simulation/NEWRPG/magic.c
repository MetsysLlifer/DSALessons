#include "game.h"

// ... (Color and Raw Element creation - Same as before) ...
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
    e.mass = 20.0f; e.friction = 5.0f; e.size = 15.0f;
    e.color = GetElementColor(type);
    e.maxSpeed = 0.0f; e.moveForce = 0.0f;
    e.spellData.core = type;
    e.spellData.auxCount = 0;
    
    if(type == ELEM_FIRE) { e.spellData.temperature = 100.0; e.spellData.dryness = 1.0; }
    else if(type == ELEM_WATER) { e.spellData.temperature = 20.0; e.spellData.dryness = 0.0; }
    else if(type == ELEM_EARTH) { e.spellData.temperature = 50.0; e.spellData.dryness = 1.0; }
    else if(type == ELEM_AIR) { e.spellData.temperature = 40.0; e.spellData.dryness = 0.5; }
    e.spellData.intensity = 0.5;
    
    return e;
}

void ApplyElementPhysics(Entity* e) {
    switch (e->spellData.core) {
        case ELEM_FIRE:
            e->mass = 1.0f;
            e->maxSpeed = 1000.0f;
            e->friction = 1.0f;
            break;
        case ELEM_EARTH:
            e->mass = 50.0f;
            e->maxSpeed = 500.0f;
            e->friction = 10.0f;
            break;
        case ELEM_WATER:
            e->mass = 10.0f;
            e->maxSpeed = 700.0f;
            e->friction = 2.0f;
            break;
        case ELEM_AIR:
        default:
            e->mass = 5.0f;
            e->maxSpeed = 900.0f;
            e->friction = 0.5f;
            break;
    }
    e->moveForce = 2000.0f;
}

void RecalculateStats(Spell* s) {
    s->power = 10.0f; s->aiType = AI_LINEAR; s->behavior = SPELL_PROJECTILE;
    s->isTeleport = false; s->hasHoming = false; s->hasGravity = false;
    
    int air = 0, earth = 0, water = 0, fire = 0;
    for(int i=0; i<s->auxCount; i++) {
        if(s->aux[i] == ELEM_FIRE) fire++;
        if(s->aux[i] == ELEM_AIR) air++;
        if(s->aux[i] == ELEM_EARTH) earth++;
        if(s->aux[i] == ELEM_WATER) water++;
    }
    
    // --- AI ---
    if (air >= 2) { s->aiType = AI_HOMING; s->hasHoming = true; }
    if (air >= 1 && fire >= 1) s->aiType = AI_PREDICT; 
    if (water >= 2) s->aiType = AI_ORBIT;       
    if (earth >= 2 && fire >= 1) s->aiType = AI_ERRATIC; 
    if (water >= 1 && air >= 1 && earth >= 1) s->aiType = AI_SWARM;
    
    // --- POWERS ---
    if (s->core == ELEM_EARTH) {
        if (air >= 2) s->isTeleport = true; 
        else if (earth >= 2) s->behavior = SPELL_PETRIFY;
        else if (earth >= 1) s->behavior = SPELL_WALL;
        else if (water >= 2) s->behavior = SPELL_GROWTH;
        else if (fire >= 1 && air == 0) s->behavior = SPELL_LANDMINE;
        else if (water >= 1 && air >= 1) s->behavior = SPELL_REWIND;
        else if (air >= 1 && fire >= 1) s->behavior = SPELL_MAGNET;
    }
    else if (s->core == ELEM_FIRE) {
        if (earth >= 1 && fire == 0) s->behavior = SPELL_MIDAS;
        else if (fire >= 2) s->behavior = SPELL_BERSERK;
        else if (fire >= 1 && earth >= 1) s->behavior = SPELL_CLUSTER;
        else if (air >= 2) s->behavior = SPELL_VOID;
        else if (air >= 1 && earth == 0) s->behavior = SPELL_CHAIN_LIGHTNING;
        else if (water >= 1 && air >= 1) s->behavior = SPELL_CONFUSE;
        else if (air >= 1) s->behavior = SPELL_SHRINK;
    }
    else if (s->core == ELEM_WATER) {
        if (water >= 2) s->behavior = SPELL_TSUNAMI;
        else if (air >= 1 && earth == 0 && fire == 0) s->behavior = SPELL_FREEZE;
        else if (fire >= 1 && earth >= 1) s->behavior = SPELL_VAMPIRISM;
        else if (earth >= 1 && fire == 0) s->behavior = SPELL_BOUNCE;
        else if (air >= 1 && earth >= 1) s->behavior = SPELL_SWARM;
        else if (earth >= 1 && fire >= 1) s->behavior = SPELL_POISON;
        else if (air >= 1 && fire >= 1) s->behavior = SPELL_MIRROR;
        else if (s->auxCount > 0) s->behavior = SPELL_HEAL;
    }
    else if (s->core == ELEM_AIR) {
        if (air >= 2) s->behavior = SPELL_WHIRLWIND;
        else if (air >= 1 && fire == 0) { s->behavior = SPELL_TELEKINESIS; s->hasGravity = true; }
        else if (water >= 1 && earth == 0) s->behavior = SPELL_PHANTOM; 
        else if (fire >= 1 && earth >= 1) s->behavior = SPELL_SNIPER;
    }
}

void PerformSpatialFusion(Entity* entities, int count, int coreIndex, ParticleSystem* ps, Player* player) {
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
    if(totalInt > 0) { s->temperature = totalTemp/totalInt; s->dryness = totalDry/totalInt; s->intensity = totalInt; }
    RecalculateStats(s);
    core->state = STATE_PROJECTILE; core->size = 25.0f; core->isSpell = true; 
    ApplyElementPhysics(core);
    sprintf(s->name, "Fused Spell");
    if (s->behavior != SPELL_PROJECTILE) {
        if (!player->book.discovered[s->behavior]) {
            player->book.discovered[s->behavior] = true;
            player->book.notificationTimer = 3.0f; 
            sprintf(player->book.notificationText, "NEW DISCOVERY!");
        }
    }
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