#include "include/visitor.h"
#include "include/scope.h"
#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Predefined functions
static ast_t* pre_defined_func_print(visitor_t* visitor, ast_t** args, int args_size) {
    printf("print func\n");
    for (int i = 0; i < args_size; i++) {
        ast_t* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type) {
            case ast_string:
                printf("%s ", process_escaped_sequences(visited_ast->string_value));
                break;
            case ast_number:
                printf("%f ", visited_ast->number_value);
                break;
            case ast_variable_def: {
                ast_t* var_value = visitor_visit(visitor, visited_ast->variable_def_var_value);
                printf("%s", var_value->string_value);
                break;
            }
            case ast_bool:
                visited_ast->bool_value ? printf("true") : printf("false");
                break;
            default:
                printf("Unhandled type: %d\n", visited_ast->type);
        }
    }
    return init_ast(ast_noop);
}

visitor_t* init_visitor() {
    visitor_t* visitor = calloc(1, sizeof(visitor_t));
    visitor->current_scope = init_scope();
    return visitor;
}

ast_t* visitor_visit(visitor_t* visitor, ast_t* node) {
    printf("visitor visit %d\n", node->type);
    switch (node->type) {
        case ast_variable_def:
            return visitor_visit_ast_variable_def(visitor, node);
        case ast_variable:
            return visitor_visit_ast_variable(visitor, node);
        case ast_variable_assign:
            return visitor_visit_ast_variable_def(visitor, node);
        case ast_binary_op:
            return visitor_visit_ast_binary_op(visitor, node);
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

void visit_function_body(visitor_t* visitor, ast_t* function_body) {
    printf("we are visiting function body\n");
    scope_t* function_scope = init_scope_with_parent(visitor->current_scope);
    if (!function_scope) {
        printf("Failed to create function scope\n");
        exit(1);
    }
    visitor->current_scope = function_scope;
    
    // Visit the function body
    visitor_visit(visitor, function_body);
    
    // Revert back to the parent scope
    visitor->current_scope = function_scope->parent_scope;
    free(function_scope); // Free the function scope
}


ast_t* visitor_visit_ast_variable_def(visitor_t* visitor, ast_t* node) {
    printf("Check if the variable already exists in the current scope or parent scope\n");
    printf("visitor current scope %p\n",visitor->current_scope);
    ast_t* existing_var_def = scope_get_variable_definition(visitor->current_scope, node->variable_def_name);
    
    if (existing_var_def != NULL) {
        // Variable exists, so update its value
        existing_var_def->variable_def_var_value = node->variable_def_var_value;
        printf("Updated existing variable: %s\n", node->variable_def_name);
    } else {
        // Variable does not exist, so add a new definition in the current scope
        scope_set_variable_definition(visitor->current_scope, node);
        printf("Defined new variable: %s\n", node->variable_def_name);
    }

    return node;
}

ast_t* visitor_visit_ast_variable(visitor_t* visitor, ast_t* node) {
    // Attempt to resolve the variable in the current or parent scopes
    ast_t* v_def = scope_get_variable_definition(visitor->current_scope, node->variable_name);
    if (v_def != NULL) {
        return visitor_visit(visitor, v_def->variable_def_var_value);
    }
    printf("Undefined variable %s\n", node->variable_name);
    exit(1);
    return node; // It's unreachable but I'll keep it
}

// Handle expression nodes
ast_t* visitor_visit_ast_binary_op(visitor_t* visitor, ast_t* node) {
    // Visit left and right operands
    printf("Binary operation started\n");
    
    ast_t* left = visitor_visit(visitor, node->binary_op_left);
    ast_t* right = visitor_visit(visitor, node->binary_op_right);

    // Determine the type of the operation
    int node_type = node->binary_op_type - ast_binary_op - 3;

    printf("Node type: %d\n", node_type);

    // Handle equality and other comparison operations
    if (node_type >= OPERATOR_EQ) {
        bool condition_res = false;

        switch (node_type) {
            case 7:
                condition_res = left->number_value < right->number_value;
                break;
            case 8:
                condition_res = left->number_value > right->number_value;
                break;
            case 9:
                condition_res = left->number_value == right->number_value;
                break;
            case 10:
                condition_res = left->number_value <= right->number_value;
                break;
            case 11:
                condition_res = left->number_value >= right->number_value;
                break;
            case 12:
                condition_res = left->number_value != right->number_value;
                break;
            default:
                printf("Unsupported comparison operator: %d\n", node->binary_op_type);
                exit(1);
        }

        ast_t* result_node = init_ast(ast_bool);
        result_node->bool_value = condition_res;
        return result_node;
    }

    // Handle arithmetic operations
    double result = 0.0;

    switch (node_type) {
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
            printf("Unsupported arithmetic operator: %d\n", node->binary_op_type);
            exit(1);
    }

    ast_t* result_node = init_ast(ast_number);
    result_node->number_value = result;
    return result_node;
}

ast_t* visitor_visit_if_statement(visitor_t* visitor, ast_t* node) {
    printf("If statement started\n");

    // Evaluate the condition
    ast_t* condition = visitor_visit(visitor, node->if_condition);

    if (condition->type != ast_bool) {
        printf("Error: If condition must evaluate to a boolean.\n");
        exit(1);
    }

    // Check the condition result
    if (condition->bool_value) {
        // Execute the 'then' branch
        return visitor_visit(visitor, node->if_body);
    }

    // If there's no else branch and the condition is false, return a null node or empty result
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
    printf("statement visitor\n");
    return node;
}

ast_t* visitor_visit_ast_func_call(visitor_t* visitor, ast_t* node) {
    if (strcmp(node->func_call_name, "print") == 0) {
        return pre_defined_func_print(visitor, node->func_call_args, node->func_call_args_size);
    }

    // Look for the function definition in the current or parent scopes
    
    ast_t* f_def = scope_get_function_definition(visitor->current_scope, node->func_call_name);
    if (f_def == NULL) {
        printf("Undefined function '%s'\n", node->func_call_name);
        exit(1);
    }

    int args_difference = abs(f_def->func_def_args_size - node->func_call_args_size);
    if (args_difference == 1) {
        printf("'%s' missing %d required argument\n", node->func_call_name, args_difference);
    } else if (args_difference > 1) {
        printf("'%s' missing %d required arguments\n", node->func_call_name, args_difference);
    }

    for (int i = 0; i < f_def->func_def_args_size; i++) {
        ast_t* arg_name = f_def->func_def_args[i];
        ast_t* evaluated_arg = visitor_visit(visitor, node->func_call_args[i]);
        scope_set_variable_definition(f_def->scope, evaluated_arg);
    }

    visit_function_body(visitor, f_def->func_def_body);
    return init_ast(ast_noop);
}

ast_t* visitor_visit_ast_func_def(visitor_t* visitor, ast_t* node) {
    printf("we are visiting a func def \n");
    scope_set_function_definition(visitor->current_scope, node);
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