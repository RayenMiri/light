#include <stdlib.h>
#include <string.h>
#include "include/environment.h"
#include <stdint.h>
#define TABLE_MAX_LOAD 0.75

Environment* environment_create(Environment* enclosing) {
    Environment* environment = malloc(sizeof(Environment));
    environment->enclosing = enclosing;
    environment->values = NULL; // Change from entries to values
    environment->count = 0;
    environment->capacity = 0;
    return environment;
}

static void adjust_capacity(Environment* environment) {
    int new_capacity = environment->capacity == 0 ? 8 : environment->capacity * 2;
    EnvironmentEntry* new_entries = calloc(new_capacity, sizeof(EnvironmentEntry));

    for (int i = 0; i < environment->capacity; i++) {
        EnvironmentEntry* entry = &environment->values[i]; // Change from entries to values
        if (entry->name != NULL) { // Change from key to name
            int index = hash_string(entry->name) % new_capacity; // Change from key to name
            while (new_entries[index].name != NULL) { // Change from key to name
                index = (index + 1) % new_capacity;
            }
            new_entries[index] = *entry;
        }
    }

    free(environment->values); // Change from entries to values
    environment->values = new_entries; // Change from entries to values
    environment->capacity = new_capacity;
}

void environment_define(Environment* environment, const char* name, Object* value) {
    if (environment->count + 1 > environment->capacity * TABLE_MAX_LOAD) {
        adjust_capacity(environment);
    }

    int index = hash_string(name) % environment->capacity;
    while (environment->values[index].name != NULL) { // Change from entries to values
        if (strcmp(environment->values[index].name, name) == 0) { // Change from key to name
            environment->values[index].value = value;
            return;
        }
        index = (index + 1) % environment->capacity;
    }

    environment->values[index].name = strdup(name); // Use strdup to allocate memory for name
    environment->values[index].value = value;
    environment->count++;
}

Object* environment_get(Environment* environment, Token name) {
    if (environment->count == 0) {
        if (environment->enclosing != NULL) {
            return environment_get(environment->enclosing, name);
        }
        // Runtime error: Undefined variable.
        return NULL;
    }

    int index = hash_string(name.start) % environment->capacity;
    while (environment->values[index].name != NULL) { // Change from entries to values
        if (strcmp(environment->values[index].name, name.start) == 0) { // Change from key to name
            return environment->values[index].value;
        }
        index = (index + 1) % environment->capacity;
    }

    if (environment->enclosing != NULL) {
        return environment_get(environment->enclosing, name);
    }

    // Runtime error: Undefined variable.
    return NULL;
}

void environment_assign(Environment* environment, Token name, Object* value) {
    if (environment->count == 0) {
        if (environment->enclosing != NULL) {
            environment_assign(environment->enclosing, name, value);
            return;
        }
        // Runtime error: Undefined variable.
        return;
    }

    int index = hash_string(name.start) % environment->capacity;
    while (environment->values[index].name != NULL) { // Change from entries to values
        if (strcmp(environment->values[index].name, name.start) == 0) { // Change from key to name
            environment->values[index].value = value;
            return;
        }
        index = (index + 1) % environment->capacity;
    }

    if (environment->enclosing != NULL) {
        environment_assign(environment->enclosing, name, value);
        return;
    }

    // Runtime error: Undefined variable.
}

void environment_destroy(Environment* environment) {
    if (environment == NULL) return;

    // Free each name in the values
    for (int i = 0; i < environment->capacity; i++) {
        if (environment->values[i].name != NULL) {
            free(environment->values[i].name); // Free the duplicated name
        }
    }

    free(environment->values); // Free the array of values
    free(environment); // Free the environment itself
}