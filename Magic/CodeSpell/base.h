#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_AUX 4
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
    ElementType type;       // Base element type (solid, liquid, gas, plasma)
    PhysicalForm form;      // Dominant physical state/form of the fusion
    double temperature;     // Temperature affecting the element and form state
    double intensity;       // Percentage intensity (0.0 to 1.0) representing concentration
    double dryness;         // Additional property to express dryness (0.0 = soaked, 1.0 = completely dry)
} Spell;

#endif