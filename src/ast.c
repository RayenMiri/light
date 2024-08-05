#include "include/ast.h"
#include <stdlib.h>

ast_t* init_ast(int type) {
    ast_t* ast = calloc(1, sizeof(struct ast));
    ast->type = type;

    // ast_variable_def
    ast->variable_def_name = NULL;
    ast->variable_def_var_value = NULL;
    
    // ast_variable
    ast->variable_name = NULL;
   
    // ast_func_call
    ast->func_call_name = NULL;
    ast->func_call_args = NULL;
    ast->func_call_args_size = 0;

    // ast_func_def
    ast->func_def_body = NULL;
    ast->func_def_name = NULL;
    
    // ast_string
    ast->string_value = NULL;

    // ast_compound
    ast->compound_value = NULL;
    ast->compound_size = 0;

    return ast;
}
