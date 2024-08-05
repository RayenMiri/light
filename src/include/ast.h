#ifndef AST_H
#define AST_H
#include <stdlib.h>

typedef struct ast {
    enum {
        ast_variable_def,
        ast_variable,
        ast_exp,
        ast_string,
        ast_number,
        ast_compound,
        ast_statement,
        ast_func_call,
        ast_func_def,
        ast_noop,
    } type;

    // ast_variable_def
    char* variable_def_name; // Corrected the typo here
    struct ast* variable_def_var_value;
    
    // ast_variable
    char* variable_name;
   
    // ast_func_call
    char* func_call_name;
    struct ast** func_call_args;
    size_t func_call_args_size;

    // ast_func_def
    struct ast* func_def_body;
    char* func_def_name;


    // ast_string
    char* string_value;

    // ast_compound
    struct ast** compound_value;
    size_t compound_size;

} ast_t;

// Initialize the AST
ast_t* init_ast(int type);

#endif
