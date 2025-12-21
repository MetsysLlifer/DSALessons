#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include <string.h> 
#include <stdio.h>

// --- CONSTANTS ---
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WALL_COUNT 3
#define INVENTORY_CAPACITY 5
#define MAX_PARTICLES 1000 
#define MAX_AUX 8 
#define FUSION_RADIUS 150.0f 

#ifndef PI
#define PI 3.14159265358979323846f
#endif

// --- MAGIC TYPES ---
#define MAX_NAME 32
typedef char Name[MAX_NAME];

typedef enum { 
    ELEM_NONE = 0,
    ELEM_EARTH, 
    ELEM_WATER, 
    ELEM_FIRE,  
    ELEM_AIR    
} ElementType;

typedef enum {
    STATE_RAW,          // Ingredient on floor
    STATE_PROJECTILE,   // Active Spell
    STATE_STATIC_WALL   // Solidified Earth
} EntityState;

typedef enum {
    SPELL_PROJECTILE,
    SPELL_WALL,
    SPELL_TELEKINESIS,
    SPELL_HEAL
} SpellBehavior;

// --- HYBRID SPELL STRUCT ---
// Combines Old Stats (Temp/Dryness) with New Stats (Core/Aux)
typedef struct {
    Name name;
    
    // NEW SYSTEM (Structure)
    ElementType core;       
    ElementType aux[MAX_AUX]; 
    int auxCount;
    
    // OLD SYSTEM (Thermodynamics)
    double temperature; // 0 to 100+
    double intensity;   // 0.0 to 1.0
    double dryness;     // 0.0 (Wet) to 1.0 (Dry)
    
    // BEHAVIOR
    float power;        
    bool hasHoming;     
    bool hasGravity;
    bool isTeleport;
    SpellBehavior behavior;
    float manaCost;
} Spell;

// --- ENTITY ---
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float mass;           
    float friction;       
    float size;           
    
    // Physics
    float speed;           
    float steering_factor; 
    
    Color color;
    float health;
    float maxHealth;

    // Logic
    EntityState state;
    Spell spellData;  
    
    bool isActive;    
    bool isSpell;     
    
    // Telekinesis
    bool isHeld;      
    float holdAngle;  

    float lifeTime; 
} Entity;

// --- PLAYER ---
typedef struct {
    ElementType selectedElement; 
    float mana;
    float maxMana;
} Player;

typedef struct { Entity items[INVENTORY_CAPACITY]; int count; int selectedSlot; } Inventory;
typedef struct { Vector2 position; Vector2 velocity; Color color; float life; float size; bool active; } Particle;
typedef struct { Particle particles[MAX_PARTICLES]; } ParticleSystem;

// --- PROTOTYPES ---
void InitParticles(ParticleSystem* ps);
void UpdateParticles(ParticleSystem* ps);
void DrawParticles(ParticleSystem* ps); 
void SpawnExplosion(ParticleSystem* ps, Vector2 position, Color color);

void InitInventory(Inventory* inv);
bool AddItem(Inventory* inv, Entity item);
Entity DropItem(Inventory* inv, int index);
void DrawInventory(Inventory* inv, int x, int y);

Color GetElementColor(ElementType type);
Entity CreateRawElement(ElementType type, Vector2 pos);
void PerformSpatialFusion(Entity* entities, int count, int coreIndex, ParticleSystem* ps);
Spell FuseSpellData(Spell A, Spell B); 

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount);
void ResolveEntityCollisions(Entity* entities, int count, Entity* player, ParticleSystem* ps); 
void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount);
void ApplyTelekinesis(Entity* player, Entity* entities, int count);

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount);
void DrawElementWheel(Player* player, Vector2 mousePos);
void DrawEntityTooltip(Entity* e, int x, int y);
void DrawHUD(Entity* player, Player stats); 

#endif