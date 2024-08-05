#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

//one symbol structure
typedef struct symbol 
{
    char* name;
    ast_t* value; // Can be a variable value or function definition
} symbol_t;

//whole symbols table structure
typedef struct symbol_table
{
    symbol_t** symbols;
    size_t size;
    size_t capacity;

}symbol_table_t;

symbol_table_t* init_symbol_table();

void free_symbol_table(symbol_table_t* table);

void symbol_table_insert(symbol_table_t* table, char* name, ast_t* value);

ast_t* symbol_table_lookup(symbol_table_t* table, char* name);

#endif