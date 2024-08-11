#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"


#define TABLE_SIZE 128

typedef struct hashmap_entry {
    const char* key;
    ast_t* value;
    struct hashmap_entry* next;
} hashmap_entry_t;

typedef struct {
    hashmap_entry_t* table[TABLE_SIZE];
} hashmap_t;

unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % TABLE_SIZE;
}

hashmap_t* create_hashmap() {
    hashmap_t* hashmap = (hashmap_t*)calloc(1, sizeof(hashmap_t));
    return hashmap;
}

void hashmap_set(hashmap_t* hashmap, const char* key, ast_t* value) {
    unsigned int index = hash(key);
    hashmap_entry_t* new_entry =(hashmap_entry_t*) malloc(sizeof(hashmap_entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = hashmap->table[index];
    hashmap->table[index] = new_entry;
}

ast_t* hashmap_get(hashmap_t* hashmap, const char* key) {
    unsigned int index = hash(key);
    hashmap_entry_t* entry = hashmap->table[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}
