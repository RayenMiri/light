#ifndef AST_H
#define AST_H
#include <stdlib.h>

typedef struct ast
{
    enum{
        ast_variabe_def,
        ast_variabe,
        ast_exp,
        ast_string,
        ast_number,
        ast_compound,
        ast_statement,
        ast_func_call,
        ast_func_def
    }type;

    // ast_variabe_def,
    char* variable_def_vame;
    struct ast* variable_def_var_value;
    
    // ast_variabe
    char * variable_name;
   
    // ast_func_call
    char* func_call_name;
    struct ast** func_call_args;
    size_t func_call_args_size;

    // ast_string
    char* string_value;

    // ast_compound
    struct ast** compound_value;
    size_t compound_size;

}ast_t;
//initiliz the AST
ast_t * init_ast(int type);

#endif