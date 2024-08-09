#include "include/scope.h"
#include "include/ast.h"
#include <string.h>
#include <stdio.h>

scope_t* init_scope(){
    scope_t * scope = calloc(1,sizeof(struct scope));
    scope->parent_scope = NULL;
    scope->function_definitions = NULL;
    scope->function_definitions_size = 0;
    scope->variable_definitions = NULL;
    scope->variable_definitions_size = 0;
    return scope;

}

scope_t* init_scope_with_parent(scope_t** parent_scope){
    scope_t* scope = init_scope();
    scope->parent_scope = parent_scope;
    return scope;
}


ast_t* scope_set_function_definition(scope_t* scope, ast_t* fdef){
    printf("are we stting smth here %s\n",fdef->func_def_name);
    scope->function_definitions_size++;
    printf("++\n");
    if (scope->function_definitions) {
        printf("first if\n");
        scope->function_definitions = calloc(1,sizeof(struct ast *));
        
    }else{
        printf("second if\n");
        scope->function_definitions = realloc(
            scope->function_definitions,scope->function_definitions_size * sizeof(struct ast **)
            );
    }
    printf("func def size %d",scope->function_definitions_size);
    scope->function_definitions[scope->function_definitions_size - 1] = fdef;
    return fdef;
}

ast_t* scope_get_function_definition(scope_t* scope, const char* fname){
    printf("scope->function_definitions_size = %d\n",scope->function_definitions_size);
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
    
    printf("scope var def size %d \n",scope->variable_definitions_size);
    
    for (int i = 0; i < scope->variable_definitions_size; i++)
    {
        printf("we made it into the for statement\n");
        ast_t* v_def = scope->variable_definitions[i];
        printf("var[%d] = %s \n",i,scope->variable_definitions[i]->variable_def_name);
        if(strcmp(v_def->variable_def_name,name) == 0){
            return v_def;
        }
    }
    return NULL;
}