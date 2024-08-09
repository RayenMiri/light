#ifndef SCOPE_H
#define SCOPE_H
#include "AST.h"

typedef struct scope
{
    struct scope** parent_scope; // Pointer to parent scope
    ast_t** function_definitions;
    size_t function_definitions_size;

    ast_t** variable_definitions;
    size_t variable_definitions_size;
} scope_t;

scope_t* init_scope();

scope_t* init_scope_with_parent(scope_t** parent_scope);

ast_t* scope_set_function_definition(scope_t* scope, ast_t* fdef);

ast_t* scope_get_function_definition(scope_t* scope, const char* fname);

ast_t* scope_set_variable_definition(scope_t* scope, ast_t* vdef);

ast_t* scope_get_variable_definition(scope_t* scope, const char* name);
#endif