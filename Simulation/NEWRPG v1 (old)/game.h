#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

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

#define MAX_NAME 32
typedef char Name[MAX_NAME];

typedef enum { ELEM_NONE = 0, ELEM_EARTH, ELEM_WATER, ELEM_FIRE, ELEM_AIR } ElementType;
typedef enum { STATE_RAW, STATE_PROJECTILE, STATE_STATIC_WALL } EntityState;

typedef enum {
    SPELL_PROJECTILE = 0, SPELL_WALL, SPELL_TELEKINESIS, SPELL_HEAL, SPELL_MIDAS, SPELL_VOID, SPELL_SLOW,
    SPELL_CHAIN_LIGHTNING, SPELL_FREEZE, SPELL_VAMPIRISM, SPELL_CLUSTER, SPELL_REFLECT, SPELL_PHANTOM,
    SPELL_CONFUSE, SPELL_BERSERK, SPELL_NECROMANCY, SPELL_TSUNAMI, SPELL_WHIRLWIND, SPELL_MAGNET,
    SPELL_PETRIFY, SPELL_MIRROR, SPELL_REWIND, SPELL_POISON, SPELL_SWARM, SPELL_SNIPER, SPELL_BOUNCE,
    SPELL_LANDMINE, SPELL_GROWTH, SPELL_SHRINK, SPELL_COUNT 
} SpellBehavior;

typedef enum { AI_NONE = 0, AI_LINEAR, AI_HOMING, AI_PREDICT, AI_ORBIT, AI_FLEE, AI_SWARM, AI_ERRATIC } AiType;

typedef struct {
    Name name; ElementType core; ElementType aux[MAX_AUX]; int auxCount;
    double temperature; double intensity; double dryness; float power;
    AiType aiType; SpellBehavior behavior; float manaCost;
    bool isTeleport; bool hasHoming; bool hasGravity;
} Spell;

typedef struct {
    Vector2 position; Vector2 velocity; float mass; float friction; float size;
    float maxSpeed; float moveForce; Color color; float health; float maxHealth;
    EntityState state; Spell spellData; bool isActive; bool isSpell; bool isHeld;
    float lifeTime; Vector2 targetPos; int targetID;         
} Entity;

typedef struct {
    bool discovered[SPELL_COUNT]; float notificationTimer; char notificationText[64];    
} Compendium;

typedef struct {
    ElementType selectedElement; float mana; float maxMana; Compendium book; 
} Player;

typedef struct { Entity items[INVENTORY_CAPACITY]; int count; int selectedSlot; } Inventory;
typedef struct { Vector2 position; Vector2 velocity; Color color; float life; float size; bool active; } Particle;
typedef struct { Particle particles[MAX_PARTICLES]; } ParticleSystem;

// PROTOTYPES
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
void PerformSpatialFusion(Entity* entities, int count, int coreIndex, ParticleSystem* ps, Player* player);
Spell FuseSpellData(Spell A, Spell B); 

// Shared Geometry for Granular Physics
int GetSpellChunkCount(Entity* e);
Vector2 GetSpellChunkPos(Entity* e, int index, float time);
float GetSpellChunkSize(Entity* e, int index);

void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount);
void UpdateEntityAI(Entity* e, Entity* entities, int count, Vector2 targetPos); 
void ResolveEntityCollisions(Entity* entities, int count, Entity* player, ParticleSystem* ps); 
void ApplySpellFieldEffects(Entity* entities, int count, ParticleSystem* ps); 

void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount);
void DrawElementWheel(Player* player, Vector2 mousePos);
void DrawEntityTooltip(Entity* e, int x, int y);
void DrawHUD(Entity* player, Player* stats); 
void DrawCompendium(Player* player); 

#endif