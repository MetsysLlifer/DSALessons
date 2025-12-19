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

// --- STRUCTS ---
typedef struct {
    Vector2 position;
    Vector2 velocity;
    float size;
    float speed;
    float steering_factor;
    Color color;
} Character;

// --- FUNCTION PROTOTYPES ---
// We promise these functions exist in other .c files
void UpdatePlayer(Character* player, Rectangle* walls, int wallCount);
void DrawGame(Character player, Rectangle* walls, int wallCount);
void updateUI();

#endif