#include "raylib.h"
#include "raygui.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX 10
#define SCREENWIDTH 800
#define SCREENHEIGHT 450
#define preX SCREENWIDTH * 0.75 // Where to start to draw the rect



// (char* text, Vector2 size, Color color, Vector2 position)
typedef struct{
    char text[25];
    int value;
    int textSize;
    Vector2 recSize;
    Color color;
    Vector2 position;
} Button;

// (int data, int size, Color color, Vector2 position)
typedef struct{
    int data;
    int size;
    Color color;
    Vector2 position;
} ElementType;

typedef struct{
    ElementType Elements[MAX];
    int count;
} VersionOne;

// Create, Read, Update, Delete
void init(VersionOne*); // Initialize the List and set the count to -1 (Empty)
void read(VersionOne);
int insert(VersionOne* List, int data, Vector2 position, float* timer);
void delete(VersionOne*, int data);
int deleteLast(VersionOne*, float* timer);
bool member(VersionOne, int);
int locate(VersionOne, int);

void displayElements(VersionOne List);
void connectLine(VersionOne List); // Connect Elements from 0 to MAX - 1
void updateHolding(VersionOne* List, ElementType** holdElement, bool* holding);

// UI
void table(Button*);