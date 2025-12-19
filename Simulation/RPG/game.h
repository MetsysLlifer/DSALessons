/*
 THIS HEADER ACTS AS "CONTRACT".
 It holds the definitions that everyone needs to know about (The structs and the constants).
*/

#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include <string.h> // For memcpy

// --- CONSTANTS ---
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WALL_COUNT 3
#define INVENTORY_CAPACITY 5
#define MAX_PARTICLES 1000 // Maximum particles on screen at once

// --- MAGIC ENUMS & STRUCTS (From your old base.h) ---
#define MAX_NAME 16
typedef char Name[MAX_NAME];

typedef enum {
    ELEMENT_SOLID,
    ELEMENT_LIQUID,
    ELEMENT_GAS,
    ELEMENT_PLASMA,
    ELEMENT_UNKNOWN
} ElementType;

typedef enum {
    FORM_SOLID,
    FORM_LIQUID,
    FORM_GAS,
    FORM_PLASMA,
    FORM_UNKNOWN
} PhysicalForm;

typedef struct {
    Name name;
    ElementType type;
    PhysicalForm form;
    double temperature;
    double intensity;
    double dryness;
    bool complete;
} Spell;

// --- NEW: PARTICLE STRUCTS ---
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life;      // 1.0 (Born) -> 0.0 (Dead)
    float size;
    bool active;
} Particle;

typedef struct {
    Particle particles[MAX_PARTICLES];
} ParticleSystem;

// --- ENTITY STRUCT ---
typedef struct {
    // Physics Properties
    Vector2 position;
    Vector2 velocity;
    float mass;           // Determines inertia (1.0 = light, 50.0 = heavy)
    float friction;       // How fast it stops (e.g. 10.0)
    float size;           // Collision size
    
    // Movement Properties
    float maxSpeed;       // Replaces "speed"
    float moveForce;      // Replaces "steering_factor" (The engine power)
    
    // Visuals
    Color color;

    // --- NEW: MAGIC PROPERTIES ---
    bool isSpell;     // Is this a magic projectile?
    bool isActive;    // Should this be drawn/updated? (Used for deleting objects)
    Spell spellData;  // The actual magic data
} Entity;

typedef struct {
    Entity items[INVENTORY_CAPACITY];
    int count;
    int selectedSlot; // Tracks which slot is highlighted (-1 = None)
} Inventory;

// --- PROTOTYPES ---

// Physics (UPDATED: Now takes ParticleSystem* so collisions can spawn effects)
void ResolveEntityCollisions(Entity* entities, int count, ParticleSystem* ps);

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount);
void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount);
void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount);
void DisplayEntityStatus(Entity *e, bool isVisible);
void DrawSpeedSlider(float* value, int x, int y);

// Inventory
void InitInventory(Inventory* inv);
bool AddItem(Inventory* inv, Entity item);
Entity DropItem(Inventory* inv, int index);
void DrawInventory(Inventory* inv, int x, int y);

// Magic
Spell FuseSpellData(Spell A, Spell B);
Color GetSpellColor(Spell s);
Entity CreateSpellEntity(Spell s, Vector2 pos, Vector2 velocity);

// NEW: Particle Functions
void InitParticles(ParticleSystem* ps);
void UpdateParticles(ParticleSystem* ps);
void DrawParticles(ParticleSystem* ps);
void SpawnExplosion(ParticleSystem* ps, Vector2 position, Color color);
#endif