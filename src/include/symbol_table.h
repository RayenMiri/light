#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

// One symbol structure with a pointer to the next symbol for chaining in hash table
typedef struct symbol 
{
    char* name;
    ast_t* value; // Can be a variable value or function definition
    struct symbol* next; // Pointer to the next symbol for separate chaining
} symbol_t;

// Whole symbol table structure with a dynamic array of symbol pointers
typedef struct symbol_table
{
    symbol_t** symbols; // Array of pointers to symbol_t (for hash table buckets)
    size_t size;        // Number of symbols in the table
    size_t capacity;    // Capacity of the symbol table (number of buckets)
} symbol_table_t;

// Initialize a new symbol table
symbol_table_t* init_symbol_table();

// Free the memory allocated for the symbol table
void free_symbol_table(symbol_table_t* table);

// Insert a symbol into the symbol table
void symbol_table_insert(symbol_table_t* table, const char* name, ast_t* value) ;
// Lookup a symbol in the symbol table by its name
ast_t* symbol_table_lookup(symbol_table_t* table, const char* name) ;

// Remove a symbol from the symbol table by its name
void symbol_table_remove(symbol_table_t* table, const char* name);

#endif
