#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "raylib.h"


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

typedef struct node{
    ElementType Element;
    struct node* next;
} *LinkedList;

// Utilities
void init(LinkedList*);
bool insert(LinkedList* List, int data, Vector2 position, float* timer);
int deleteLast(LinkedList*, float* timer);


// UI
void displayElements(LinkedList List);
void connectLine(LinkedList List); // Connect Elements from 0 to MAX - 1
void updateHolding(LinkedList* List, ElementType** holdElement, bool* holding);
void drawButtons(LinkedList* List, float* feedbackTimer, char* textAlert);
void drawPanel(LinkedList* List);
void displayElementDetails(ElementType Elem, Vector2 position, int idx);

#endif // UTIL_H
