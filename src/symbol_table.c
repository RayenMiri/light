#include "include/symbol_table.h"
#include <string.h>
#include <stdio.h>

// Hash function for symbol names
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

symbol_table_t* init_symbol_table() {
    symbol_table_t* symbol_table = calloc(1, sizeof(symbol_table_t));
    if (!symbol_table) {
        perror("Failed to allocate memory for symbol table");
        exit(EXIT_FAILURE);
    }
    symbol_table->symbols = calloc(8, sizeof(symbol_t*)); // Initial capacity
    if (!symbol_table->symbols) {
        perror("Failed to allocate memory for symbols");
        free(symbol_table);
        exit(EXIT_FAILURE);
    }
    symbol_table->capacity = 8;
    symbol_table->size = 0;

    return symbol_table;
}

void free_symbol_table(symbol_table_t* table) {
    for (size_t i = 0; i < table->capacity; i++) {
        symbol_t* symbol = table->symbols[i];
        while (symbol) {
            symbol_t* next = symbol->next;
            free(symbol->name);
            free(symbol);
            symbol = next;
        }
    }
    free(table->symbols);
    free(table);
}

void symbol_table_insert(symbol_table_t* table, const char* name, ast_t* value) {
    // Rehash if necessary
    if (table->size >= table->capacity) {
        size_t old_capacity = table->capacity;
        table->capacity *= 2;
        symbol_t** new_symbols = calloc(table->capacity, sizeof(symbol_t*));
        if (!new_symbols) {
            perror("Failed to allocate memory for rehashing");
            exit(EXIT_FAILURE);
        }

        // Rehash existing symbols
        for (size_t i = 0; i < old_capacity; i++) {
            symbol_t* symbol = table->symbols[i];
            while (symbol) {
                symbol_t* next = symbol->next;
                unsigned long hash = hash_function(symbol->name) % table->capacity;
                symbol->next = new_symbols[hash];
                new_symbols[hash] = symbol;
                symbol = next;
            }
        }
        free(table->symbols);
        table->symbols = new_symbols;
    }

    unsigned long hash = hash_function(name) % table->capacity;
    symbol_t* symbol = calloc(1, sizeof(symbol_t));
    if (!symbol) {
        perror("Failed to allocate memory for symbol");
        exit(EXIT_FAILURE);
    }
    symbol->name = strdup(name);
    if (!symbol->name) {
        perror("Failed to allocate memory for symbol name");
        free(symbol);
        exit(EXIT_FAILURE);
    }
    symbol->value = value;

    // Insert at the head of the list
    symbol->next = table->symbols[hash];
    table->symbols[hash] = symbol;
    table->size++;
}

ast_t* symbol_table_lookup(symbol_table_t* table, const char* name) {
    unsigned long hash = hash_function(name) % table->capacity;
    symbol_t* symbol = table->symbols[hash];
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol->value;
        }
        symbol = symbol->next;
    }
    return NULL;
}

void symbol_table_remove(symbol_table_t* table, const char* name) {
    unsigned long hash = hash_function(name) % table->capacity;
    symbol_t* symbol = table->symbols[hash];
    symbol_t* prev = NULL;
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            if (prev) {
                prev->next = symbol->next;
            } else {
                table->symbols[hash] = symbol->next;
            }
            free(symbol->name);
            free(symbol);
            table->size--;
            return;
        }
        prev = symbol;
        symbol = symbol->next;
    }
}
