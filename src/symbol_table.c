#include "include/symbol_table.h"
#include <string.h>
#include <stdio.h>

symbol_table_t* init_symbol_table(){
    symbol_table_t* symbol_table = calloc(1,sizeof(symbol_table_t));
    symbol_table->symbols = calloc(8, sizeof(symbol_t*)); // Initial capacity
    symbol_table->capacity = 8;

    return symbol_table;
}

void free_symbol_table(symbol_table_t* table) {
    for (size_t i = 0; i < table->size; i++) {
        free(table->symbols[i]->name);
        free(table->symbols[i]);
    }
    free(table->symbols);
    free(table);
}

//inseret a symbol in the symbol_table 
void symbol_table_insert(symbol_table_t* table, char* name, ast_t* value){
    // tracking symbol state and allocating memory for it and increasing the size of the table if it reaches capacity
    if(table->size >= table->capacity){
        table->capacity *= 2;
        table->symbols = realloc(
            table->symbols,
            table->capacity*sizeof(symbol_t*)
            );
        
    }
    symbol_t* symbol = calloc(1,sizeof(symbol_t));
    symbol->name = name;
    symbol->value = value;
    table->symbols[table->size++] = symbol;

}

//look up for a symbol by its name to return its value when it's being called
ast_t* symbol_table_lookup(symbol_table_t* table, char* name) {
    for (size_t i = 0; i < table->size; i++) {
        if (strcmp(table->symbols[i]->name, name) == 0) {
            return table->symbols[i]->value;
        }
    }
    
    return NULL;
}
