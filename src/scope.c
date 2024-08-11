#include "include/scope.h"
#include "include/ast.h"
#include "include/hashmap.h"
#include <string.h>
#include <stdio.h>


scope_t* init_scope() {
    scope_t *scope = calloc(1, sizeof(struct scope));
    scope->parent_scope = NULL;
    scope->function_definitions = create_hashmap();
    scope->variable_definitions = create_hashmap();
    scope->scope_level = 0;
    return scope;
}

scope_t* init_scope_with_parent(scope_t* parent_scope) {
    scope_t* scope = init_scope();
    scope->parent_scope = parent_scope;
    scope->scope_level = parent_scope->scope_level + 1;
    return scope;
}

ast_t* scope_set_function_definition(scope_t* scope, ast_t* fdef) {
    hashmap_set(scope->function_definitions, fdef->func_def_name, fdef);
    return fdef;
}

ast_t* scope_get_function_definition(scope_t* scope, const char* fname) {
    ast_t* fdef = hashmap_get(scope->function_definitions, fname);
    if (fdef) {
        return fdef;
    }
    if (scope->parent_scope != NULL) {
        return scope_get_function_definition(scope->parent_scope, fname);
    }
    return NULL;
}

ast_t* scope_set_variable_definition(scope_t* scope, ast_t* vdef) {
    hashmap_set(scope->variable_definitions, vdef->variable_def_name, vdef);
    return vdef;
}

ast_t* scope_get_variable_definition(scope_t* scope, const char* name) {
    ast_t* vdef = hashmap_get(scope->variable_definitions, name);
    if (vdef) {
        return vdef;
    }
    if (scope->scope_level > 0) {
        return scope_get_variable_definition(scope->parent_scope, name);
    }
    return NULL;
}
