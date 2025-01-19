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
    //this should take consideration of the args_size and \n solo printing 
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
        case ast_while : 
            return visitor_visit_while_statement(visitor,node);
        case ast_for : 
            return visitor_visit_for_statement(visitor,node);
        case ast_string:
            return visitor_visit_ast_string(visitor, node);
        case ast_number:
            return visitor_visit_ast_number(visitor, node);
        case ast_bool:
            return visitor_visit_ast_bool(visitor, node);
        case ast_compound:
            return visitor_visit_ast_compound(visitor, node);
        case ast_statement:
            return visitor_visit_ast_statement(visitor, node);
        case ast_func_call:
            return visitor_visit_ast_func_call(visitor, node);
        case ast_func_def:
            return visitor_visit_ast_func_def(visitor, node);
        case ast_return:
            return visitor_visit_ast_return(visitor,node);
        default:
            printf("Uncaught statement of type %d\n", node->type);
            exit(1);
    }
}

ast_t* visitor_visit_ast_variable_def(visitor_t* visitor, ast_t* node) {
    
    // Check if the variable already exists in the scope
    ast_t* existing_var_def = scope_get_variable_definition(visitor->current_scope, node->variable_def_name);
    
    if (existing_var_def != NULL) {
        // Evaluate the new value before updating the variable definition
         
        ast_t* new_value = visitor_visit(visitor, node->variable_def_var_value);
        
        // Update its value in the existing variable definition
        existing_var_def->variable_def_var_value = new_value;
    } else {
        
        // Variable does not exist, so add a new definition with the evaluated value
   
        ast_t* new_value = visitor_visit(visitor, node->variable_def_var_value);
        node->variable_def_var_value = new_value;
        scope_set_variable_definition(visitor->current_scope, node);
    }

    return node;
}



ast_t* visitor_visit_ast_variable(visitor_t* visitor, ast_t* node) {
    
    ast_t* v_def = scope_get_variable_definition(visitor->current_scope, node->variable_name);
    
    if (v_def != NULL) {
        // Check if the variable's value is not the variable itself or causing recursion
        if (v_def->variable_def_var_value->type == ast_variable && 
            strcmp(v_def->variable_def_var_value->variable_name, node->variable_name) == 0) {
            printf("Error: Infinite recursion detected in variable %s\n", node->variable_name);
            exit(1);
        }
        // Evaluate the variable's value only if it's not a recursive call
        return visitor_visit(visitor, v_def->variable_def_var_value);
    }

    printf("Undefined variable %s\n", node->variable_name);
    exit(1);

    return node; // Unreachable but included
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

        // Determine if the operands are of the same type
        if (left->type != right->type) {
            printf("Error: Cannot compare operands of different types.\n");
            exit(1);
        }

        // Perform the comparison based on the type of the operands
        switch (left->type) {
            case ast_number:
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
                        printf("Unsupported comparison operator for numbers: %d\n", op_type);
                        exit(1);
                }
                break;

            case ast_string:
                switch (op_type) {
                    case OPERATOR_EQ:
                        condition_res = strcmp(left->string_value, right->string_value) == 0;
                        break;
                    case OPERATOR_NE:
                        condition_res = strcmp(left->string_value, right->string_value) != 0;
                        break;
                    default:
                        printf("Unsupported comparison operator for strings: %d\n", op_type);
                        exit(1);
                }
                break;

            case ast_bool:
                switch (op_type) {
                    case OPERATOR_EQ:
                        condition_res = left->bool_value == right->bool_value;
                        break;
                    case OPERATOR_NE:
                        condition_res = left->bool_value != right->bool_value;
                        break;
                    default:
                        printf("Unsupported comparison operator for booleans: %d\n", op_type);
                        exit(1);
                }
                break;

            default:
                printf("Unsupported operand type for comparison: %d\n", left->type);
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
       
        for (int i = 0; i < node->if_body->compound_size; i++) {
            ast_t* result = visitor_visit(visitor, node->if_body->compound_value[i]);
            
            // If a return statement is encountered, stop and return its value
            if (result->type == ast_return) {
                return result;
            }
        }
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
            for (int i = 0; i < node->elsif_bodies[i]->compound_size; i++) {
            ast_t* result = visitor_visit(visitor, node->elsif_bodies[i]->compound_value[i]);
            
            // If a return statement is encountered, stop and return its value
            if (result->type == ast_return) {
                return result;
            }
        }
            return visitor_visit(visitor, node->elsif_bodies[i]);
        }
    }

    // Add handling for 'else' 
    if (node->else_branch_body) {
        for (int i = 0; i < node->else_branch_body->compound_size; i++) {
            ast_t* result = visitor_visit(visitor, node->else_branch_body->compound_value[i]);
            
            // If a return statement is encountered, stop and return its value
            if (result->type == ast_return) {
                return result;
            }
        }
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

ast_t* visitor_visit_ast_bool(visitor_t* visitor, ast_t* node) {
   
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
    if (args_difference > 0) {
        printf("'%s' called with incorrect number of arguments: expected %d, got %d\n",
               node->func_call_name, f_def->func_def_args_size, node->func_call_args_size);
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
        strcpy(ast_vardef->variable_def_name, ast_var->variable_name);
        
        ast_vardef->variable_def_var_value = evaluated_arg;

        scope_set_variable_definition(function_scope, ast_vardef);
    }

    ast_t* result = init_ast(ast_noop); // Default return value

    for (int i = 0; i < f_def->func_def_body->compound_size; i++) {
        ast_t* statement = f_def->func_def_body->compound_value[i];
        
        // Visit the statement
        result = visitor_visit(visitor, statement);

        // If a return statement is encountered, stop execution and return its value
        if (result->type == ast_return) {
           
            visitor->current_scope = visitor->current_scope->parent_scope; // Exit function scope
            return result->return_value; // Return the value
        }
    }

    visitor->current_scope = visitor->current_scope->parent_scope;

    return result; // Return the result (or a default value if no return was encountered)
}




ast_t* visitor_visit_ast_func_def(visitor_t* visitor, ast_t* node) {
    // Handle function definition
    scope_set_function_definition(visitor->current_scope,node);
    return node;
}

ast_t* visitor_visit_while_statement(visitor_t* visitor, ast_t* node) {
    while (1) {
        ast_t* while_condition = visitor_visit(visitor, node->while_condition);

        if (while_condition->type != ast_bool) {
            printf("Error: while condition must evaluate to a boolean.\n");
            exit(1);
        }

        if (!while_condition->bool_value) {
            break; // Exit the loop when the condition is false
        }

        // Execute the loop body
        visitor_visit(visitor, node->while_body);
    }

    return init_ast(ast_noop);
}

ast_t* visitor_visit_for_statement(visitor_t* visitor, ast_t* node){
    // Visit the index initialization (e.g., int i = 0)
    visitor_visit(visitor,node->for_index);
    while(1){
        //Evaluate the for condition
        ast_t* for_condition = visitor_visit(visitor,node->for_condition);

        if (for_condition->type != ast_bool) {
            printf("Error: while condition must evaluate to a boolean.\n");
            exit(1);
        }

        if(!for_condition->bool_value){
            break;
        }
        //Visit the body
        visitor_visit(visitor,node->for_body);
        
        //Revisit the step (++ or --)
        visitor_visit(visitor, node->for_step);
    }

    return init_ast(ast_noop);
}

ast_t* visitor_visit_ast_return(visitor_t* visitor, ast_t* node) {
    // Visit the expression being returned
    ast_t* return_value = visitor_visit(visitor, node->return_value);
    
    // Create a return node with the evaluated value
    ast_t* return_node = init_ast(ast_return);
    return_node->return_value = return_value;
    
    return return_node; // Return this node to signal return

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
                // for now i don't think it's necessary but i can add more cases here for other escape sequences if needed
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