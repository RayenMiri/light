#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include <stdint.h>
#include "object.h"

#define TABLE_MAX_LOAD 0.75

typedef struct {
    char* key;
    Object* value;
} TableEntry;

typedef struct {
    int count;
    int capacity;
    TableEntry* entries;
} Table;

Table* table_create(void);
void table_destroy(Table* table);
bool table_set(Table* table, const char* key, Object* value);
Object* table_get(Table* table, const char* key);
bool table_delete(Table* table, const char* key);

uint32_t hash_string(const char* key);

#endif