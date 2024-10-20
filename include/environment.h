#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object.h"
#include "token.h"
#include "table.h"

typedef struct EnvironmentEntry {
    char* name;
    Object* value;
} EnvironmentEntry;

typedef struct Environment {
    struct Environment* enclosing;
    EnvironmentEntry* values;
    int capacity;
    int count;
} Environment;

Environment* environment_create(Environment* enclosing);
void environment_destroy(Environment* environment);
void environment_define(Environment* environment, const char* name, Object* value);
Object* environment_get(Environment* environment, Token name);
void environment_assign(Environment* environment, Token name, Object* value);

#endif