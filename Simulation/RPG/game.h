/*
 THIS HEADER ACTS AS "CONTRACT".
 It holds the definitions that everyone needs to know about (The structs and the constants).
*/

#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "raymath.h"

// --- CONSTANTS ---
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WALL_COUNT 3
#define INVENTORY_CAPACITY 5


// --- STRUCTS ---
typedef struct {
    // Physics Properties
    Vector2 position;
    Vector2 velocity;
    float mass;           // New: Determines inertia (1.0 = light, 50.0 = heavy)
    float friction;       // New: How fast it stops (e.g. 10.0)
    float size;           // Collision size
    
    // Movement Properties
    float maxSpeed;       // Replaces "speed"
    float moveForce;      // Replaces "steering_factor" (The engine power)
    
    // Visuals
    Color color;
} Entity;

typedef struct {
    Entity items[INVENTORY_CAPACITY];
    int count;
} Inventory;

// --- FUNCTION PROTOTYPES ---
// Physics now takes 'inputDirection' so it works for Players (Keyboard) AND NPCs (AI)
void UpdateEntityPhysics(Entity* e, Vector2 inputDirection, Rectangle* walls, int wallCount);
void ResolveEntityCollisions(Entity* entities, int count);
// This forces entities out of walls if they get pushed in
void EnforceWallConstraints(Entity* entities, int count, Rectangle* walls, int wallCount);
void DrawGame(Entity* entities, int count, Rectangle* walls, int wallCount);
void DisplayEntityStatus(Entity *e, bool isVisible);
void DrawSpeedSlider(float* value, int x, int y);
// NEW INVENTORY FUNCTIONS
void InitInventory(Inventory* inv);
bool AddItem(Inventory* inv, Entity item);
Entity DropItem(Inventory* inv, int index); // Returns the item so we can put it back in the world
void DrawInventory(Inventory* inv, int x, int y);
#endif