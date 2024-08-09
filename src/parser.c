#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

parser_t* init_parser(lexer_t* lexer) {
    parser_t* parser = calloc(1, sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;
    parser->scope = init_scope();
    
    return parser;
}

void syntax_error(parser_t* parser, const char* message) {
    fprintf(stderr, "Syntax error at line %d, position %d: %s\n",
            parser->lexer->line, parser->lexer->pos, message);
    exit(1);
}

void parser_consume(parser_t* parser, int token_type) {
    if (parser->current_token->type == token_type) {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    } else {
        char error_message[200];
        snprintf(error_message, sizeof(error_message),
                 "Expected token type %d (%s) but got type %d (%s) with value '%s'",
                 token_type, token_type_to_string(token_type),
                 parser->current_token->type, token_type_to_string(parser->current_token->type),
                 parser->current_token->value);
        syntax_error(parser, error_message);
    }
}

ast_t* parser_parse(parser_t* parser, scope_t* scope) {
    return parser_parse_statements(parser, scope);
}

ast_t* parser_parse_statements(parser_t* parser, scope_t* scope) {
    ast_t* compound = init_ast(ast_compound);
    compound->scope = scope;
    compound->compound_value = calloc(1, sizeof(struct ast*));
    compound->compound_size = 0;

    while (parser->current_token->type != TOKEN_EOF && parser->current_token->type != TOKEN_RBRACE) {
        ast_t* ast_statement = parser_parse_statement(parser, scope);
        ast_statement->scope = scope;
        if (ast_statement) {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct ast*)
            );
            compound->compound_value[compound->compound_size - 1] = ast_statement;
        } else {
            syntax_error(parser, "Statement parsing returned NULL");
        }

        if (parser->current_token->type == TOKEN_SEMI) {
            parser_consume(parser, TOKEN_SEMI);
        }
    }

    return compound;
}

ast_t* parser_parse_statement(parser_t* parser, scope_t* scope) {
    switch (parser->current_token->type) {
        case TOKEN_IDENTIFIER: return parser_parse_identifier(parser, scope);
        case TOKEN_IF: return parser_parse_if_statement(parser,scope);
        default: {
            char error_message[100];
            snprintf(error_message, sizeof(error_message),
                     "Unexpected token '%s' of type %d (%s) in statement",
                     parser->current_token->value, parser->current_token->type,
                     token_type_to_string(parser->current_token->type));
            syntax_error(parser, error_message);
        }
    }
    return init_ast(ast_noop);
}

ast_t* parser_parse_exp(parser_t* parser, scope_t* scope) {
    switch (parser->current_token->type) {
        case TOKEN_STRING: return parser_parse_string(parser, scope);
        case TOKEN_NUMBER : return parser_parse_number(parser,scope);
        case TOKEN_IDENTIFIER: return parser_parse_identifier(parser, scope);
        default: {
            char error_message[100];
            snprintf(error_message, sizeof(error_message),
                     "Unexpected token '%s' of type %d (%s) in expression",
                     parser->current_token->value, parser->current_token->type,
                     token_type_to_string(parser->current_token->type));
            syntax_error(parser, error_message);
        }
    }
    return init_ast(ast_noop);
}

ast_t* parser_parse_func_call(parser_t* parser, scope_t* scope) {
    ast_t* func_call = init_ast(ast_func_call);
    func_call->func_call_name = parser->prev_token->value;
    func_call->func_call_args = NULL;
    func_call->func_call_args_size = 0;
    
    parser_consume(parser, TOKEN_LPAREN);
   
    bool is_first_arg = true;

    while (parser->current_token->type != TOKEN_RPAREN) {
        if (!is_first_arg) {
            if (parser->current_token->type == TOKEN_COMMA) {
                parser_consume(parser, TOKEN_COMMA);
            } 
        }
        ast_t* ast_exp = NULL;
        switch (parser->current_token->type) {
            
            case TOKEN_IDENTIFIER:
            case TOKEN_NUMBER:
            case TOKEN_LPAREN:
                ast_exp = parser_parse_expression(parser, scope);
                break;
            default:
                ast_exp = parser_parse_exp(parser, scope);
                break;
        }

        if (ast_exp == NULL) {
            fprintf(stderr, "Error: Failed to parse argument expression.\n");
            return NULL; // Handle the error accordingly (later because it's not buggy and not a priority for now)
        }

        func_call->func_call_args_size += 1;
        func_call->func_call_args = realloc(
            func_call->func_call_args,
            func_call->func_call_args_size * sizeof(struct ast*)
        );
        func_call->func_call_args[func_call->func_call_args_size - 1] = ast_exp;

        is_first_arg = false;
    }

    parser_consume(parser, TOKEN_RPAREN);
    
    func_call->scope = scope;
    return func_call;
}


ast_t* parser_parse_func_definition(parser_t* parser, scope_t* scope) {
    ast_t* ast_function_def = init_ast(ast_func_def);
    parser_consume(parser, TOKEN_IDENTIFIER);  // "function"

    char* function_name = parser->current_token->value;
    ast_function_def->func_def_name = function_name;
    parser_consume(parser, TOKEN_IDENTIFIER);
    parser_consume(parser, TOKEN_LPAREN);
    
    if (parser->current_token->type != TOKEN_RPAREN) {
        ast_function_def->func_def_args = calloc(1, sizeof(struct ast*));
        ast_t* arg = parser_parse_variable(parser, scope);
        ast_function_def->func_def_args_size += 1;
        ast_function_def->func_def_args[ast_function_def->func_def_args_size - 1] = arg;

        while (parser->current_token->type == TOKEN_COMMA) {
            parser_consume(parser, TOKEN_COMMA);
            ast_function_def->func_def_args_size += 1;
            ast_function_def->func_def_args = realloc(
                ast_function_def->func_def_args,
                ast_function_def->func_def_args_size * sizeof(struct ast*)
            );
            ast_t* arg = parser_parse_variable(parser, scope);
            ast_function_def->func_def_args[ast_function_def->func_def_args_size - 1] = arg;
        }
    }
    
    parser_consume(parser, TOKEN_RPAREN);
    parser_consume(parser, TOKEN_LBRACE);

    ast_function_def->func_def_body = parser_parse_statements(parser, scope);
    parser_consume(parser, TOKEN_RBRACE);

    // Store function definition in the scope
    ast_function_def->scope = scope;
    return ast_function_def;
}

ast_t* parser_parse_variable(parser_t* parser, scope_t* scope) {
    char* token_value = parser->current_token->value;
    parser_consume(parser, TOKEN_IDENTIFIER);

    if(parser->current_token->type == TOKEN_ASSIGN) {
        parser_consume(parser,TOKEN_ASSIGN);
        
        ast_t* var_def = init_ast(ast_variable_assign);
        var_def->variable_def_name = token_value;
        var_def->variable_def_var_value =parser_parse_exp(parser,scope);
        var_def->scope = scope;
        return var_def;
    } else if (parser->current_token->type == TOKEN_LPAREN) {
        return parser_parse_func_call(parser, scope);
    }else{
        ast_t* ast_var = init_ast(ast_variable);
        ast_var->variable_name = token_value;
        ast_var->scope = scope;
        return ast_var;
    }
    
    
}

ast_t* parser_parse_variable_definition(parser_t* parser, scope_t* scope) {
    parser_consume(parser, TOKEN_IDENTIFIER);  // "var"
    char* var_name = parser->current_token->value;
    parser_consume(parser, TOKEN_IDENTIFIER);  // Variable name
    parser_consume(parser, TOKEN_ASSIGN);      // "="

    ast_t* var_value;
    
    // Determine if the value is a mathematical expression or a direct value
    if (parser->current_token->type == TOKEN_NUMBER 
    || parser->current_token->type == TOKEN_IDENTIFIER 
    || parser->current_token->type == TOKEN_LPAREN) {
        // Mathematical expression or complex expression
        var_value = parser_parse_expression(parser, scope);
       
    } else if (parser->current_token->type == TOKEN_STRING) {
        var_value = parser_parse_exp(parser, scope);
    } else {
        // Handle unexpected tokens
        char error_message[100];
        snprintf(error_message, sizeof(error_message),
                 "Unexpected token '%s' in variable definition",
                 parser->current_token->value);
        syntax_error(parser, error_message);
    }

    ast_t* var_def = init_ast(ast_variable_def);
    var_def->variable_def_name = var_name;
    var_def->variable_def_var_value = var_value;
    var_def->scope = scope;

    return var_def;
}


ast_t* parser_parse_string(parser_t* parser, scope_t* scope) {
    ast_t* string_node = init_ast(ast_string);
    string_node->string_value = parser->current_token->value;

    parser_consume(parser, TOKEN_STRING);
    string_node->scope = scope;

    return string_node;
}

ast_t* parser_parse_number(parser_t* parser, scope_t* scope) {
    
    ast_t* number_node = init_ast(ast_number);

    // Convert the token value to a double
    number_node->number_value = atof(parser->current_token->value);
    parser_consume(parser, TOKEN_NUMBER);

    number_node->scope = scope;

    return number_node;
}


ast_t* parser_parse_identifier(parser_t* parser, scope_t* scope) {
    if (strcmp(parser->current_token->value, "var") == 0) {
        return parser_parse_variable_definition(parser, scope);
    } else if (strcmp(parser->current_token->value, "function") == 0) {
        return parser_parse_func_definition(parser, scope);
    }else if(strcmp(parser->current_token->value, "if") == 0) {
        return parser_parse_if_statement(parser,scope);
    }else {
        return parser_parse_variable(parser, scope);
    }
}

ast_t* parser_parse_factor(parser_t* parser, scope_t* scope) {
    // Handle numbers, strings, identifiers, etc.
    
    switch (parser->current_token->type) {
        case TOKEN_NUMBER:
            return parser_parse_number(parser, scope);
        case TOKEN_IDENTIFIER:
            return parser_parse_identifier(parser, scope);
        case TOKEN_LPAREN:
            parser_consume(parser, TOKEN_LPAREN);
            ast_t* expr = parser_parse_expression(parser, scope);
            parser_consume(parser, TOKEN_RPAREN);
            return expr;
        default:
            syntax_error(parser, "Unexpected token in factor");
            return NULL;
    }
}

ast_t* parser_parse_term(parser_t* parser, scope_t* scope) {
    
    ast_t* left = parser_parse_factor(parser, scope);

    while (parser->current_token->type == TOKEN_MUL || parser->current_token->type == TOKEN_DIV) {
        int operator_type = parser->current_token->type;
        parser_consume(parser, operator_type);

        ast_t* right = parser_parse_factor(parser, scope);
        ast_t* new_left = init_ast(ast_binary_op);
        new_left->binary_op_left = left;
        new_left->binary_op_right = right;
        new_left->binary_op_type = operator_type;

        left = new_left;
    }

    return left;
}

ast_t* parser_parse_expression(parser_t* parser, scope_t* scope) {
    
    ast_t* left = parser_parse_term(parser, scope);
    
    while (parser->current_token->type == TOKEN_PLUS
        || parser->current_token->type == TOKEN_MINUS 
        || parser->current_token->type == TOKEN_EQ
        || parser->current_token->type == TOKEN_NE
        || parser->current_token->type == TOKEN_GT
        || parser->current_token->type == TOKEN_LT
        || parser->current_token->type == TOKEN_LE
        || parser->current_token->type == TOKEN_GE
        || parser->current_token->type == TOKEN_NE
        ) {
        int operator_type = parser->current_token->type;
        parser_consume(parser, operator_type);

        ast_t* right = parser_parse_term(parser, scope);
        ast_t* new_left = init_ast(ast_binary_op);
        new_left->binary_op_left = left;
        new_left->binary_op_right = right;
        new_left->binary_op_type = operator_type;

        left = new_left;
    }

    return left;
}

ast_t* parser_parse_condition(parser_t* parser , scope_t* scope){
     
}

ast_t* parser_parse_if_statement(parser_t* parser, scope_t* scope) {

    parser_consume(parser, TOKEN_IF);          // Consume "if"
    parser_consume(parser, TOKEN_LPAREN);      // Consume "("
    ast_t* condition = parser_parse_expression(parser, scope); // Parse the condition expression
    parser_consume(parser, TOKEN_RPAREN);      // Consume ")"
    // Expect a block or single statement here
    parser_consume(parser, TOKEN_LBRACE);      // Consume "{"
    ast_t* if_body = parser_parse_statements(parser, scope); // Parse the body of the if statement
    parser_consume(parser, TOKEN_RBRACE);      // Consume "}"

    ast_t* if_ast = init_ast(ast_if);          // Initialize the AST node for an "if" statement
    if_ast->if_condition = condition;
    if_ast->if_body = if_body;
    if_ast->scope = scope;

    /*Handle optional "else" part
    if (parser->current_token->type == TOKEN_ELSE) {
        parser_consume(parser, TOKEN_ELSE);
        if (parser->current_token->type == TOKEN_LBRACE) {
            // If there is a block after "else"
            parser_consume(parser, TOKEN_LBRACE);
            ast_t* else_branch = parser_parse_statements(parser, scope);
            parser_consume(parser, TOKEN_RBRACE);
            if_ast->if_else_branch = else_branch;
        } else {
            // If it's a single statement after "else"
            ast_t* else_branch = parser_parse_statement(parser, scope);
            if_ast->if_else_branch = else_branch;
        }
    } else {
        if_ast->if_else_branch = NULL;
    }
    */
    return if_ast;
}

