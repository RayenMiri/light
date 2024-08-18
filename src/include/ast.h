#ifndef AST_H
#define AST_H
#include <stdlib.h>
#include <stdbool.h>

// Define token types for binary operations
typedef enum {
    OPERATOR_PLUS = 15,
    OPERATOR_MINUS,
    OPERATOR_MULTIPLY,
    OPERATOR_DIVIDE,
    OPERATOR_MODULO,
    OPERATOR_LT = 24,
    OPERATOR_GT, 
    OPERATOR_EQ , 
    OPERATOR_LE,   
    OPERATOR_GE,  
    OPERATOR_NE,   
    // Add other operators as needed
} binary_op_type_t;

typedef struct ast {
    enum {
        ast_variable_def,
        ast_variable,
        ast_variable_assign,
        ast_exp,
        ast_string,
        ast_number,
        ast_unary_op,
        ast_bool,
        ast_compound,
        ast_statement,
        ast_func_call,
        ast_func_def,
        ast_func_arg, 
        ast_binary_op,
        ast_if,
        ast_while,
        ast_for,
        ast_return,
        ast_noop,
    } type;
    struct scope* scope;

    // ast_variable_def
    char* variable_def_name; 
    struct ast* variable_def_var_value;
    
    // ast_variable
    char* variable_name;
    
    //ast_variable_assign
    char* variable_assign_name;
    struct ast* variable_assign_value;
    
    // ast_func_call
    char* func_call_name;
    struct ast** func_call_args;
    size_t func_call_args_size;

    // ast_func_def
    struct ast* func_def_body;
    char* func_def_name;
    struct ast** func_def_args;
    size_t func_def_args_size;
    char* func_def_return_type;

    //ast_return
    struct ast* return_value;

    // ast_func_arg
    char* func_arg_name;   
    struct ast* func_arg_value; 

    // ast_string
    char* string_value;

    // ast_number
    double number_value; 
    
    // ast_bool
    bool bool_value; 

    // ast_compound
    struct ast** compound_value;
    size_t compound_size;

    // ast_binary_op
    struct ast* binary_op_left;
    struct ast* binary_op_right;
    binary_op_type_t binary_op_type;

    // ast_if
    struct ast* if_condition;
    struct ast* if_body;
    struct ast** elsif_conditions; // Array of `elsif` conditions
    struct ast** elsif_bodies;     // Array of `elsif` bodies
    size_t elsif_count;           // Number of `elsif` branches
    struct ast* else_branch_body; // `else` body
    
    //ast_while
    struct ast* while_condition;
    struct ast* while_body;

    //ast_for
    struct ast* for_condition;
    size_t for_index;
    size_t for_step;
    struct ast* for_body;

    //ast_unary_op
    struct ast*  unary_op_operand ; 
    binary_op_type_t  unary_op_type ;

} ast_t;

// Initialize the AST
ast_t* init_ast(int type);

#endif