#include "include/visitor.h"
#include "include/scope.h"
#include "include/token.h"
//#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//predefined functions
static ast_t* pre_defined_func_print(visitor_t* visitor, ast_t** args, int args_size) {
    for (int i = 0; i < args_size; i++) {
        ast_t* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type) {
            case ast_string:
                printf("%s ", process_escaped_sequences(visited_ast->string_value));
                break;
            case ast_number:
                printf("%f ",visited_ast->number_value);
                break;
            case ast_variable_def:
                {
                    // Declare var_value inside a block
                   
                    ast_t* var_value = visitor_visit(visitor, visited_ast->variable_def_var_value);
                    printf("%s", var_value->string_value);
                }
                break;
            case ast_bool:
                {
                    visited_ast->bool_value == 1 ?
                    printf("true"):
                    printf("false");
                }
                break;
            default:
                printf("Unhandled type: %d\n", visited_ast->type);
        }
    }
    return init_ast(ast_noop);
}

visitor_t* init_visitor() {
    visitor_t* visitor = malloc(sizeof(visitor_t));
    visitor->global_scope = init_scope(); // Initialize global scope
    visitor->global_scope->scope_level = 0; // Global scope is level 0
    visitor->current_scope = visitor->global_scope; // Start with global scope
    return visitor;
}


ast_t* visitor_visit(visitor_t* visitor, ast_t* node) {
    switch (node->type) {
        case ast_variable_def:
            return visitor_visit_ast_variable_def(visitor, node);
        case ast_variable:
            return visitor_visit_ast_variable(visitor, node);
        case ast_variable_assign:
            return visitor_visit_ast_variable_def(visitor, node);
        case ast_binary_op:
            return visitor_visit_ast_binary_op(visitor,node);
        case ast_if:
            return visitor_visit_if_statement(visitor, node);
        case ast_string:
            return visitor_visit_ast_string(visitor, node);
        case ast_number:
            return visitor_visit_ast_number(visitor, node);
        case ast_compound:
            return visitor_visit_ast_compound(visitor, node);
        case ast_statement:
            return visitor_visit_ast_statement(visitor, node);
        case ast_func_call:
            return visitor_visit_ast_func_call(visitor, node);
        case ast_func_def:
            return visitor_visit_ast_func_def(visitor, node);
        
        default:
            printf("Uncaught statement of type %d\n", node->type);
            exit(1);
    }
}

ast_t* visitor_visit_ast_variable_def(visitor_t* visitor, ast_t* node) {
    // Check if the variable already exists in the scope
    ast_t* existing_var_def = scope_get_variable_definition(visitor->current_scope, node->variable_def_name);

    if (existing_var_def != NULL) {
        // Variable exists, so update its value
        existing_var_def->variable_def_var_value = node->variable_def_var_value;
    } else {
        // Variable does not exist, so add a new definition
        scope_set_variable_definition(visitor->current_scope, node);
    }

    return node;
}


ast_t* visitor_visit_ast_variable(visitor_t* visitor, ast_t* node) {
    // Handle variable
    
    ast_t* v_def = scope_get_variable_definition(visitor->current_scope,node->variable_name);
    if(v_def != NULL){
        return visitor_visit(visitor,v_def->variable_def_var_value);
    }
    printf("Undefined variable %s\n", node->variable_name);
    exit(1);
    return node; // It's unreachable but I'll keep it
}

// Handle expression nodes
ast_t* visitor_visit_ast_binary_op(visitor_t* visitor, ast_t* node) {
    // Visit left and right operands
    ast_t* left = visitor_visit(visitor, node->binary_op_left);
    ast_t* right = visitor_visit(visitor, node->binary_op_right);

    // Determine the type of the operation
    binary_op_type_t op_type = node->binary_op_type;  

    // Handle equality and other comparison operations
    if (op_type >= OPERATOR_LT) {
        bool condition_res = false;
        
        switch (op_type) {
            case OPERATOR_LT:
                condition_res = left->number_value < right->number_value;
                break;
            case OPERATOR_GT:
                condition_res = left->number_value > right->number_value;
                break;
            case OPERATOR_EQ:
                condition_res = left->number_value == right->number_value;
                break;
            case OPERATOR_LE:
                condition_res = left->number_value <= right->number_value;
                break;
            case OPERATOR_GE:
                condition_res = left->number_value >= right->number_value;
                break;
            case OPERATOR_NE:
                condition_res = left->number_value != right->number_value;
                break;
            default:
                printf("Unsupported comparison operator: %d\n", op_type);
                exit(1);
        }

        ast_t* result_node = init_ast(ast_bool);
        result_node->bool_value = condition_res;
        return result_node;
    }

    // Handle arithmetic operations
    double result = 0.0;

    switch (op_type) {
        case OPERATOR_PLUS:
            result = left->number_value + right->number_value;
            break;
        case OPERATOR_MINUS:
            result = left->number_value - right->number_value;
            break;
        case OPERATOR_MULTIPLY:
            result = left->number_value * right->number_value;
            break;
        case OPERATOR_DIVIDE:
            if (right->number_value == 0) {
                printf("Error: Division by zero.\n");
                exit(1);
            }
            result = left->number_value / right->number_value;
            break;
        case OPERATOR_MODULO:
            if (right->number_value == 0) {
                printf("Error: Division by zero.\n");
                exit(1);
            }
            result = fmod(left->number_value, right->number_value);
            break;
        default:
            printf("Unsupported arithmetic operator: %d\n", op_type);
            exit(1);
    }

    ast_t* result_node = init_ast(ast_number);
    result_node->number_value = result;
    return result_node;
}



ast_t* visitor_visit_if_statement(visitor_t* visitor, ast_t* node) {
    // Evaluate the 'if' condition
    ast_t* condition = visitor_visit(visitor, node->if_condition);

    if (condition->type != ast_bool) {
        printf("Error: If condition must evaluate to a boolean.\n");
        exit(1);
    }

    // Check the 'if' condition result
    if (condition->bool_value) {
        return visitor_visit(visitor, node->if_body);
    }

    // Check each 'elsif' condition only if previous conditions were false
    for (size_t i = 0; i < node->elsif_count; i++) {
        // Evaluate the 'elsif' condition
        ast_t* elsif_condition = visitor_visit(visitor, node->elsif_conditions[i]);

        if (elsif_condition->type != ast_bool) {
            printf("Error: Elsif condition must evaluate to a boolean.\n");
            exit(1);
        }

        // If the current 'elsif' condition is true, execute its body and exit
        if (elsif_condition->bool_value) {
            return visitor_visit(visitor, node->elsif_bodies[i]);
        }
    }

    // Add handling for 'else' if you have implemented that
    if (node->else_branch_body) {
        return visitor_visit(visitor, node->else_branch_body);
    }

    // If neither 'if', 'elsif', nor 'else' was executed, return a 'noop'
    return init_ast(ast_noop);
}






ast_t* visitor_visit_ast_string(visitor_t* visitor, ast_t* node) {
    return node;
}

ast_t* visitor_visit_ast_number(visitor_t* visitor, ast_t* node) {

    return node;
}

ast_t* visitor_visit_ast_compound(visitor_t* visitor, ast_t* node) {
    for (int i = 0; i < node->compound_size; i++) {
        visitor_visit(visitor, node->compound_value[i]);
    }
    return init_ast(ast_noop);
}

ast_t* visitor_visit_ast_statement(visitor_t* visitor, ast_t* node) {
    // Handle statement nodes if needed
    return node;
}

ast_t* visitor_visit_ast_func_call(visitor_t* visitor, ast_t* node) {
    if (strcmp(node->func_call_name, "print") == 0) {
        return pre_defined_func_print(visitor, node->func_call_args, node->func_call_args_size);
    }

    // Lookup function definition in the current scope or its parents
    ast_t* f_def = scope_get_function_definition(visitor->current_scope, node->func_call_name);
    if (f_def == NULL) {
        printf("Undefined function '%s'\n", node->func_call_name);
        exit(1);
    }

    // Check for argument count mismatch
    int args_difference = abs(f_def->func_def_args_size - node->func_call_args_size);
    if (args_difference == 1) {
        printf("'%s' missing %d required argument\n", node->func_call_name, args_difference);
        exit(1);
    } else if (args_difference > 1) {
        printf("'%s' missing %d required arguments\n", node->func_call_name, args_difference);
        exit(1);
    }
    
    // Create a new local scope for the function
    scope_t* function_scope = init_scope_with_parent(visitor->current_scope);

    // Set the new scope as the current scope
    visitor->current_scope = function_scope;

    // Bind function parameters to arguments in the local scope
    for (int i = 0; i < f_def->func_def_args_size; i++) {
        ast_t* arg = node->func_call_args[i];
        ast_t* evaluated_arg = visitor_visit(visitor, arg);
        
        ast_t* ast_var = (ast_t*)f_def->func_def_args[i];
        ast_t* ast_vardef = init_ast(ast_variable_def);

        ast_vardef->variable_def_name = (char*)calloc(strlen(ast_var->variable_name) + 1, sizeof(char));
        ast_vardef->variable_def_var_value = evaluated_arg;
        strcpy(ast_vardef->variable_def_name, ast_var->variable_name);
        
        // Associate the parameter name with its evaluated value
         
        scope_set_variable_definition(visitor->current_scope, ast_vardef);
    }
    visitor_visit(visitor, f_def->func_def_body);
    visitor->current_scope = function_scope->parent_scope;
    free(function_scope);
}


ast_t* visitor_visit_ast_func_def(visitor_t* visitor, ast_t* node) {
    // Handle function definition
    scope_set_function_definition(visitor->current_scope,node);
    return node;
}



// Function to process escape sequences in a string
char* process_escaped_sequences(const char* str) {
    size_t len = strlen(str);
    char* result = calloc(len + 1, sizeof(char)); // Allocate memory for the result
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    size_t i, j = 0;
    for (i = 0; i < len; i++) {
        if (str[i] == '\\' && i + 1 < len) {
            // Check the next character
            switch (str[i + 1]) {
                case 'n':
                    result[j++] = '\n';
                    i++; // Skip the next character
                    break;
                case 't':
                    result[j++] = '\t';
                    i++; // Skip the next character
                    break;
                // Add more cases here for other escape sequences if needed
                default:
                    result[j++] = str[i]; // Copy the backslash as is
                    break;
            }
        } else {
            result[j++] = str[i];
        }
    }
    result[j] = '\0'; // Null-terminate the result
    return result;
}