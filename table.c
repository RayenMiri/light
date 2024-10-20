#include <stdlib.h>
#include <string.h>
#include "table.h"

#define TABLE_INIT_CAPACITY 8

static TableEntry* find_entry(TableEntry* entries, int capacity, const char* key);
static void adjust_capacity(Table* table, int new_capacity);

Table* table_create(void) {
    Table* table = malloc(sizeof(Table));
    table->count = 0;
    table->capacity = TABLE_INIT_CAPACITY;
    table->entries = calloc(table->capacity, sizeof(TableEntry));
    return table;
}

void table_destroy(Table* table) {
    for (int i = 0; i < table->capacity; i++) {
        free(table->entries[i].key);
    }
    free(table->entries);
    free(table);
}

static TableEntry* find_entry(TableEntry* entries, int capacity, const char* key) {
    uint32_t index = hash_string(key) % capacity;
    TableEntry* tombstone = NULL;

    for (;;) {
        TableEntry* entry = &entries[index];

        if (entry->key == NULL) {
            if (entry->value == NULL) {
                return tombstone != NULL ? tombstone : entry;
            } else {
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (strcmp(entry->key, key) == 0) {
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

static void adjust_capacity(Table* table, int new_capacity) {
    TableEntry* new_entries = calloc(new_capacity, sizeof(TableEntry));

    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        TableEntry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        TableEntry* dest = find_entry(new_entries, new_capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
}

bool table_set(Table* table, const char* key, Object* value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int new_capacity = table->capacity * 2;
        adjust_capacity(table, new_capacity);
    }

    TableEntry* entry = find_entry(table->entries, table->capacity, key);
    bool is_new_key = entry->key == NULL;
    if (is_new_key && entry->value == NULL) table->count++;

    if (is_new_key) {
        entry->key = strdup(key);
    }
    entry->value = value;
    return is_new_key;
}

Object* table_get(Table* table, const char* key) {
    if (table->count == 0) return NULL;

    TableEntry* entry = find_entry(table->entries, table->capacity, key);
    if (entry->key == NULL) return NULL;

    return entry->value;
}

bool table_delete(Table* table, const char* key) {
    if (table->count == 0) return false;

    TableEntry* entry = find_entry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;

    free(entry->key);
    entry->key = NULL;
    entry->value = NULL;
    return true;
}

uint32_t hash_string(const char* key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; key[i]; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}