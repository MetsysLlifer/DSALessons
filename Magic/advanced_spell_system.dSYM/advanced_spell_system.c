#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AUX 4
#define MAX_NAME 32

typedef enum {
    ELEMENT_SOLID,
    ELEMENT_LIQUID,
    ELEMENT_PLASMA,
    ELEMENT_GAS,
    ELEMENT_UNKNOWN
} ElementType;

typedef enum {
    FORM_SOLID,
    FORM_LIQUID,
    FORM_PLASMA,
    FORM_GAS,
    FORM_UNKNOWN
} PhysicalForm;

typedef struct {
    float temperature;
    PhysicalForm form;
    int healingPower;
    int canTeleport;
    int psychic;
    int agility;
    // Extendable with other properties
} ElementProperties;

typedef struct Element {
    ElementType type;
    char name[MAX_NAME];
    ElementProperties props;
    struct Element *aux[MAX_AUX];
    int aux_count;
} Element;


// ----------- Utility: String to Enum Mapping -----------
ElementType parse_element_type(const char *s) {
    if (strcmp(s, "solid") == 0) return ELEMENT_SOLID;
    if (strcmp(s, "liquid") == 0) return ELEMENT_LIQUID;
    if (strcmp(s, "plasma") == 0) return ELEMENT_PLASMA;
    if (strcmp(s, "gas") == 0) return ELEMENT_GAS;
    return ELEMENT_UNKNOWN;
}

PhysicalForm parse_form(const char *s) {
    if (strcmp(s, "solid") == 0) return FORM_SOLID;
    if (strcmp(s, "liquid") == 0) return FORM_LIQUID;
    if (strcmp(s, "plasma") == 0) return FORM_PLASMA;
    if (strcmp(s, "gas") == 0) return FORM_GAS;
    return FORM_UNKNOWN;
}

// ----------- Element Creation & Input -----------
Element *create_element_interactive() {
    char buf[64];

    printf("Enter element name: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;

    char type_str[20];
    printf("Type [solid|liquid|plasma|gas]: ");
    fgets(type_str, sizeof(type_str), stdin);
    type_str[strcspn(type_str, "\n")] = 0;

    char form_str[20];
    printf("Physical Form [solid|liquid|plasma|gas]: ");
    fgets(form_str, sizeof(form_str), stdin);
    form_str[strcspn(form_str, "\n")] = 0;

    ElementProperties props;
    printf("Temperature: ");
    fgets(buf, sizeof(buf), stdin);
    props.temperature = atof(buf);

    printf("Healing Power (integer): ");
    fgets(buf, sizeof(buf), stdin);
    props.healingPower = atoi(buf);

    printf("Can Teleport? [0|1]: ");
    fgets(buf, sizeof(buf), stdin);
    props.canTeleport = atoi(buf);

    printf("Psychic? [0|1]: ");
    fgets(buf, sizeof(buf), stdin);
    props.psychic = atoi(buf);

    printf("Agility? [0|1]: ");
    fgets(buf, sizeof(buf), stdin);
    props.agility = atoi(buf);

    props.form = parse_form(form_str);

    Element *el = malloc(sizeof(Element));
    el->type = parse_element_type(type_str);
    strncpy(el->name, buf, MAX_NAME-1);
    el->name[MAX_NAME-1] = 0;
    el->props = props;
    el->aux_count = 0;
    for (int i = 0; i < MAX_AUX; ++i) el->aux[i] = NULL;

    return el;
}

void add_aux(Element *core, Element *aux) {
    if (core->aux_count < MAX_AUX) {
        core->aux[core->aux_count++] = aux;
    } else {
        printf("Max aux reached!\n");
    }
}

// ----------- Combination Logic: Recursive Property Fusion -----------
ElementProperties fuse_properties(ElementProperties a, ElementProperties b) {
    ElementProperties result = a;
    // Example fusion logic
    result.temperature = (a.temperature + b.temperature) / 2;
    result.healingPower = (a.healingPower > b.healingPower) ? a.healingPower : b.healingPower;
    result.canTeleport = a.canTeleport || b.canTeleport;
    result.psychic = a.psychic || b.psychic;
    result.agility = a.agility || b.agility;

    // Form is set as core, but could be blended/decided other ways
    return result;
}

void fuse_all_aux(Element *core) {
    for (int i = 0; i < core->aux_count; ++i) {
        // Recursively combine sub-aux first
        fuse_all_aux(core->aux[i]);
        core->props = fuse_properties(core->props, core->aux[i]->props);
    }
}

// ----------- Spell Display: Recursive Output -----------
void print_element_details(Element *el, int depth) {
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("- %s [%d aux]\n", el->name, el->aux_count);
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("  Type: %d, Form: %d\n", el->type, el->props.form);
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("  Temp: %.2f, Healing: %d, Teleport: %d, Psychic: %d, Agility: %d\n",
           el->props.temperature, el->props.healingPower, el->props.canTeleport,
           el->props.psychic, el->props.agility);
    for (int i = 0; i < el->aux_count; ++i)
        print_element_details(el->aux[i], depth + 1);
}

// ----------- Memory Cleanup -----------
void free_element(Element *el) {
    for (int i = 0; i < el->aux_count; ++i)
        free_element(el->aux[i]);
    free(el);
}

// ----------- Main: Interactive Spell Creation and Fusion -----------
int main() {
    printf("==== Spell Creation ====\n");
    Element *core = create_element_interactive();

    char buf[8];
    printf("Add aux elements? [y/n]: ");
    fgets(buf, sizeof(buf), stdin);

    while (buf[0] == 'y' && core->aux_count < MAX_AUX) {
        printf("\nCreating aux #%d...\n", core->aux_count+1);
        Element *aux = create_element_interactive();

        core->aux[core->aux_count++] = aux;

        printf("Add another aux element? [y/n]: ");
        fgets(buf, sizeof(buf), stdin);

        // Optional: allow aux to have sub-aux
        if (aux->aux_count < MAX_AUX && buf[0] == 'y') {
            printf("Add sub-aux to this aux? [y/n]: ");
            fgets(buf, sizeof(buf), stdin);
            while (aux->aux_count < MAX_AUX && buf[0] == 'y') {
                printf("\nCreating sub-aux #%d...\n", aux->aux_count+1);
                Element *sub_aux = create_element_interactive();
                aux->aux[aux->aux_count++] = sub_aux;
                printf("Add another sub-aux to this aux? [y/n]: ");
                fgets(buf, sizeof(buf), stdin);
            }
        }
        printf("Add another aux element to core? [y/n]: ");
        fgets(buf, sizeof(buf), stdin);
    }

    // Combine all properties (metamorphosis/fusion)
    fuse_all_aux(core);

    printf("\n==== Spell Details ====\n");
    print_element_details(core, 0);

    free_element(core);
    return 0;
}
