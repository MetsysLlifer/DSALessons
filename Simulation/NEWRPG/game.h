/*
 THIS HEADER ACTS AS "CONTRACT".
 It holds the definitions that everyone needs to know about.
*/

#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include <string.h> 

// --- CONSTANTS ---
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WALL_COUNT 3
#define INVENTORY_CAPACITY 5
#define MAX_PARTICLES 1000 

// --- MAGIC ENUMS & STRUCTS ---
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

// --- ENTITY STRUCT ---
typedef struct {
    // Physics Properties
    Vector2 position;
    Vector2 velocity;
    float mass;           // Determines inertia
    float friction;       // How fast it stops
    float size;           // Collision size
    
    // Movement Properties
    float maxSpeed;       
    float moveForce;      
    
    // Visuals
    Color color;

    // --- MAGIC PROPERTIES ---
    bool isSpell;     
    bool isActive;    
    Spell spellData;  
} Entity;

// --- INVENTORY ---
typedef struct {
    Entity items[INVENTORY_CAPACITY];
    int count;
    int selectedSlot; 
} Inventory;

// --- PARTICLE SYSTEM ---
typedef struct {
    Vector2 position;
    Vector2 velocity;
    Color color;
    float life; 
    float size;
    bool active;
} Particle;

typedef struct {
    Particle particles[MAX_PARTICLES];
} ParticleSystem;

// --- PROTOTYPES ---

// Particles
void InitParticles(ParticleSystem* ps);
void UpdateParticles(ParticleSystem* ps);
void DrawParticles(ParticleSystem* ps);
void SpawnExplosion(ParticleSystem* ps, Vector2 position, Color color);

// Inventory
void InitInventory(Inventory* inv);
bool AddItem(Inventory* inv, Entity item);
Entity DropItem(Inventory* inv, int index);
void DrawInventory(Inventory* inv, int x, int y);

// Magic
Spell FuseSpellData(Spell A, Spell B);
Color GetSpellColor(Spell s);
Entity CreateSpellEntity(Spell s, Vector2 pos, Vector2 velocity);

// Physics
void ResolveEntityCollisions(Entity* entities, int count, ParticleSystem* ps);
void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount);
void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount);

// Drawing & UI
void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount);
void DisplayEntityStatus(Entity *e, bool isVisible);
void DrawEntityTooltip(Entity* e, int x, int y);
void DrawSpeedSlider(float* value, int x, int y);

#endif


