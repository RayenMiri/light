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

  
    ast-> if_condition = NULL;
    ast-> if_body = NULL;
    ast-> elsif_conditions = NULL; // Array of `elsif` conditions
    ast->elsif_bodies = NULL ;  // Array of `elsif` bodies
    ast-> elsif_count = 0;           // Number of `elsif` branches
    ast-> else_branch_body = NULL; // `else` body

    
    ast->while_condition = NULL;
    ast->while_body = NULL;
    
    return ast;

}
