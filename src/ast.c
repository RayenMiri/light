#include "include/ast.h"
#include <string.h>

ast_t* init_ast(int type) {
    ast_t* ast = calloc(1, sizeof(ast_t));
    ast->type = type;

   
        
    ast->scope = NULL;

    ast->variable_def_name = NULL;
    ast->variable_def_var_value = NULL;
    ast->variable_name = NULL;
    

    ast->func_call_name = NULL;
    ast->func_call_args = NULL;
    ast->func_call_args_size = 0;
    

    ast->func_def_body = NULL;
    ast->func_def_name = NULL;
    ast->func_def_args = NULL;
    ast->func_def_args_size = 0;
    

    ast->func_arg_name = NULL;
    ast->func_arg_value = NULL;
    

    ast->string_value = NULL;
    
    ast->number_value = 0;

    ast->bool_value = true;

    ast->compound_value = NULL;
    ast->compound_size = 0;
    
    ast->binary_op_left = NULL;
    ast->binary_op_right = NULL;
    ast->binary_op_type = 0;

    ast->condition = NULL;
    ast->body= NULL;
    
    return ast;

}
