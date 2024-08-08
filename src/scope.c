#include "include/scope.h"
#include "include/ast.h"
#include <string.h>
#include <stdio.h>

scope_t* init_scope(){
    scope_t * scope = calloc(1,sizeof(struct scope));
    scope->function_definitions = NULL;
    scope->function_definitions_size = 0;
    scope->variable_definitions = NULL;
    scope->variable_definitions_size = 0;
    return scope;

}

ast_t* scope_set_function_definition(scope_t* scope, ast_t* fdef){
    
    scope->function_definitions_size++;
    if (scope->function_definitions) {
        scope->function_definitions = calloc(1,sizeof(struct ast *));
        
    }else{
        scope->function_definitions = realloc(
            scope->function_definitions,scope->function_definitions_size * sizeof(struct ast **)
            );
    }
    scope->function_definitions[scope->function_definitions_size - 1] = fdef;
    return fdef;
}

ast_t* scope_get_function_definition(scope_t* scope, const char* fname){
    for (size_t i = 0; i < scope->function_definitions_size; i++)
    {
        ast_t* f_def = scope->function_definitions[i];
        if(strcmp(f_def->func_def_name,fname) == 0){
            return f_def;
        }
    }
    return NULL;
    
}

ast_t* scope_set_variable_definition(scope_t* scope, ast_t* vdef){
    //ast_t* existing_vdef = scope_get_variable_definition(scope,vdef->variable_def_name);
    //printf("existing var def  in scope get var is : %s\n",existing_vdef->variable_def_name);
    if (scope->variable_definitions == NULL){

        scope->variable_definitions = calloc(1, sizeof(struct ast*));
        scope->variable_definitions[0] = vdef;
        scope->variable_definitions_size += 1;
    }else
    {
        scope->variable_definitions_size += 1;
        scope->variable_definitions = realloc(
            scope->variable_definitions,
            scope->variable_definitions_size * sizeof(struct ast*)  
        );
        scope->variable_definitions[scope->variable_definitions_size-1] = vdef;
    }

    return vdef;
}

ast_t* scope_get_variable_definition(scope_t* scope, const char* name){
    for (int i = 0; i < scope->variable_definitions_size; i++)
    {
        ast_t* v_def = scope->variable_definitions[i];
        printf("var[%d] = %s \n",i,scope->variable_definitions[i]->variable_def_name);
        if(strcmp(v_def->variable_def_name,name) == 0){
            return v_def;
        }
    }
    return NULL;
}