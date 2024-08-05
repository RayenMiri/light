#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ast_t* pre_defined_func_print(visitor_t* visitor, ast_t** args, int args_size) {
    for (int i = 0; i < args_size; i++) {
        ast_t* visited_ast = visitor_visit(visitor, args[i]);
        
        switch (visited_ast->type) {
            case ast_string:
                printf("%s",process_escaped_sequences(visited_ast->string_value));
                break; 
            default:
                printf("Unhandled type: %d\n", visited_ast->type);
        }
    }
    return init_ast(ast_noop);
}

visitor_t* init_visitor() {
    visitor_t* visitor = calloc(1, sizeof(visitor_t));
    visitor->variable_definitions = NULL;
    visitor->variable_definitions_size = 0;
    visitor->symbol_table = NULL; // Initialize symbol table

    return visitor;
}

ast_t* visitor_visit(visitor_t* visitor, ast_t* node) {
    switch (node->type) {
        case ast_variable_def:
            return visitor_visit_ast_variable_def(visitor, node);
        case ast_variable:
            return visitor_visit_ast_variable(visitor, node);
        case ast_exp:
            return visitor_visit_ast_exp(visitor, node);
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
    visitor->variable_definitions_size += 1;
    if (visitor->variable_definitions == NULL) {
        visitor->variable_definitions = calloc(1, sizeof(ast_t*));
    } else {
        visitor->variable_definitions = realloc(visitor->variable_definitions, visitor->variable_definitions_size * sizeof(ast_t*));
    }
    visitor->variable_definitions[visitor->variable_definitions_size - 1] = node;
    return node;
}

ast_t* visitor_visit_ast_variable(visitor_t* visitor, ast_t* node) {
    for (int i = 0; i < visitor->variable_definitions_size; i++) {
        ast_t* var_def = visitor->variable_definitions[i];
        if (strcmp(var_def->variable_def_name, node->variable_name) == 0) {
            return visitor_visit(visitor, var_def->variable_def_var_value);
        }
    }
    printf("Undefined variable %s\n", node->variable_name);
    return node;
}

ast_t* visitor_visit_ast_exp(visitor_t* visitor, ast_t* node) {
    // Handle expression nodes if needed
    return node;
}

ast_t* visitor_visit_ast_string(visitor_t* visitor, ast_t* node) {
    return node;
}

ast_t* visitor_visit_ast_number(visitor_t* visitor, ast_t* node) {
    // Handle number nodes if needed
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
    // Handle predefined functions like "print"
    if (strcmp(node->func_call_name, "print") == 0) {
        return pre_defined_func_print(visitor, node->func_call_args, node->func_call_args_size);
    }

    ast_t* function = symbol_table_lookup(visitor->symbol_table, node->func_call_name);
   
    if (!function) {
        printf("Undefined function %s\n", node->func_call_name);
        exit(0); // You might want to use a better error handling mechanism
    }

    // Ensure the function definition is indeed a function
    if (function->type != ast_func_def) {
        printf("%s is not a function\n", node->func_call_name);
        exit(1);
    }
    // Visit the function body
    ast_t* result = visitor_visit(visitor, function->func_def_body);

    return result;
}



ast_t* visitor_visit_ast_func_def(visitor_t* visitor, ast_t* node) {

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


