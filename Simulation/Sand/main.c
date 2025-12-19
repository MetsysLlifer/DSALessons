#include "raylib.h"
#include <string.h> // For memset

// Define the size of our simulation grid
// We use a small resolution and scale it up so the "pixels" are visible
#define GRID_W 200
#define GRID_H 150
#define CELL_SIZE 4 

// Particle Types
#define EMPTY 0
#define SAND  1
#define WALL  2

int grid[GRID_H][GRID_W];
int nextGrid[GRID_H][GRID_W]; // Double buffering to prevent state conflicts

void UpdatePhysics() {
    // Reset nextGrid to current grid state implies we need to be careful.
    // Actually, for simple falling sand, modifying the grid directly 
    // is often faster/easier if you iterate carefully (Bottom-Up).
    
    // We will iterate BOTTOM-UP so sand doesn't teleport
    for (int y = GRID_H - 2; y >= 0; y--) {
        for (int x = 0; x < GRID_W; x++) {
            
            if (grid[y][x] == SAND) {
                // RULE 1: Try to move down
                if (grid[y + 1][x] == EMPTY) {
                    grid[y + 1][x] = SAND;
                    grid[y][x] = EMPTY;
                } 
                // RULE 2: Try to move down-left
                else if (x > 0 && grid[y + 1][x - 1] == EMPTY) {
                    grid[y + 1][x - 1] = SAND;
                    grid[y][x] = EMPTY;
                }
                // RULE 3: Try to move down-right
                else if (x < GRID_W - 1 && grid[y + 1][x + 1] == EMPTY) {
                    grid[y + 1][x + 1] = SAND;
                    grid[y][x] = EMPTY;
                }
            }
        }
    }
}

int main(void) {
    // Initialization
    const int screenWidth = GRID_W * CELL_SIZE;
    const int screenHeight = GRID_H * CELL_SIZE;

    InitWindow(screenWidth, screenHeight, "C Physics - Noita Style Sand");
    SetTargetFPS(1000);

    // clear grid
    memset(grid, 0, sizeof(grid));
    
    // Fixed timestep for frame-rate independent physics
    double accumulator = 0.0;
    const double PHYSICS_TIMESTEP = 1.0 / 60.0; // 60 physics updates per second

    while (!WindowShouldClose()) {
        // --- 1. INPUT ---
        // Left Click to spawn Sand
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            int gx = mousePos.x / CELL_SIZE;
            int gy = mousePos.y / CELL_SIZE;

            // Simple brush size (add a few pixels around the mouse)
            for(int i=-2; i<=2; i++){
                for(int j=-2; j<=2; j++){
                   if(gx+i >= 0 && gx+i < GRID_W && gy+j >= 0 && gy+j < GRID_H) {
                       // Only add if randomly true (for scattering effect)
                       if(GetRandomValue(0, 10) > 2) 
                           grid[gy+j][gx+i] = SAND;
                   }
                }
            }
        }
        
        // Right click to spawn a Wall
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mousePos = GetMousePosition();
            int gx = mousePos.x / CELL_SIZE;
            int gy = mousePos.y / CELL_SIZE;
            
            if(gx >= 0 && gx < GRID_W && gy >= 0 && gy < GRID_H)
                grid[gy][gx] = WALL;
        }

        // --- 2. UPDATE ---
        // Use fixed timestep for consistent physics speed regardless of FPS
        accumulator += GetFrameTime();
        while (accumulator >= PHYSICS_TIMESTEP) {
            UpdatePhysics();
            accumulator -= PHYSICS_TIMESTEP;
        }

        // --- 3. DRAW ---
        BeginDrawing();
            ClearBackground(BLACK);

            for (int y = 0; y < GRID_H; y++) {
                for (int x = 0; x < GRID_W; x++) {
                    if (grid[y][x] == SAND) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, GOLD);
                    } else if (grid[y][x] == WALL) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
                    }
                }
            }
            
            DrawText("L-Click: Sand | R-Click: Wall", 10, 10, 20, RAYWHITE);
            DrawFPS(50, 400);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}