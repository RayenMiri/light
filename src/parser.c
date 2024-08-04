#include "include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


parser_t* init_parser(lexer_t* lexer){
    parser_t* parser = calloc(1,sizeof(struct parser));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;

    return parser;

}

void parser_die(parser_t* parser, int token_type){
    if (parser->current_token->type == token_type) {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    } else {
        printf("Unexpected token %s with type %d\n",
            parser->current_token->value, parser->current_token->type
        );
        exit(1);
    }
}


ast_t* parser_parse(parser_t* parser){
    return parser_parse_statements(parser);
}

ast_t* parser_parse_statements(parser_t* parser) {
    ast_t* compound = init_ast(ast_compound);
    compound->compound_value = calloc(1, sizeof(struct ast*));
    compound->compound_size = 0;

    while (parser->current_token->type != TOKEN_EOF) {
        ast_t* ast_statement = parser_parse_statement(parser);

        if (ast_statement) {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct ast*)
            );
            compound->compound_value[compound->compound_size - 1] = ast_statement;
        } else {
            printf("ast_statement is NULL\n");
        }

        if (parser->current_token->type == TOKEN_SEMI) {
            parser_die(parser, TOKEN_SEMI);
        }
    }

    return compound;
}




ast_t* parser_parse_statement(parser_t* parser) {
    
    switch (parser->current_token->type) {
        case TOKEN_IDENTIFIER: return parser_parse_identifier(parser);
        
    }
    return init_ast(ast_noop);
}




ast_t* parser_parse_exp(parser_t* parser){
    switch (parser->current_token->type)
    {
        case TOKEN_STRING : return parser_parse_string(parser);
        case TOKEN_IDENTIFIER: return parser_parse_identifier(parser);

    }
}

ast_t* parser_parse_factor(parser_t* parser){

}

ast_t* parser_parse_term(parser_t* parser){

}

ast_t* parser_parse_func_call(parser_t* parser) {
    ast_t* func_call = init_ast(ast_func_call);

    // Check if the current token is an identifier (function name)
    func_call->func_call_name = parser->prev_token->value;
    
    func_call->func_call_args = NULL;  // Initialize to NULL
    func_call->func_call_args_size = 0;  // Initialize the args size

    // Ensure the next token is '('
    if (parser->current_token->type != TOKEN_LPAREN) {
        printf("Expected '(', got %s\n", parser->current_token->value);
        exit(1);
    }
    parser_die(parser, TOKEN_LPAREN);  // Advance past '('

    // Parse function arguments
    while (parser->current_token->type != TOKEN_RPAREN) {
        if (func_call->func_call_args_size > 0) {
            // Ensure there's a comma between arguments
            if (parser->current_token->type != TOKEN_COMMA) {
                printf("Expected ',', got %s\n", parser->current_token->value);
                exit(1);
            }
            parser_die(parser, TOKEN_COMMA);  // Advance past ','
        }

        // Parse the argument expression
        ast_t* ast_exp = parser_parse_exp(parser);
        func_call->func_call_args_size += 1;
        func_call->func_call_args = realloc(
            func_call->func_call_args,
            func_call->func_call_args_size * sizeof(struct ast*)
        );
        func_call->func_call_args[func_call->func_call_args_size - 1] = ast_exp;
    }

    parser_die(parser, TOKEN_RPAREN);  // Advance past ')'

    return func_call;
}



ast_t* parser_parse_variable(parser_t* parser){
    char* token_value = parser->current_token->value;

    parser_die(parser,TOKEN_IDENTIFIER);
    if(parser->current_token->type == TOKEN_LPAREN){
        
        return parser_parse_func_call(parser);
    }

    ast_t* ast_var = init_ast(ast_variable);
    ast_var->variable_name = token_value;

    return ast_var;

}

ast_t* parser_parse_variable_definition(parser_t* parser) {
    parser_die(parser, TOKEN_IDENTIFIER);  // "var"
    char* var_name = parser->current_token->value;

    parser_die(parser, TOKEN_IDENTIFIER);  // Variable name
    parser_die(parser, TOKEN_ASSIGN);      // "="
    ast_t* var_value = parser_parse_exp(parser);
    ast_t* var_def = init_ast(ast_variable_def);
    var_def->variable_def_name = var_name;
    var_def->variable_def_var_value = var_value;

    return var_def;
}


ast_t* parser_parse_string(parser_t* parser) {
    ast_t* string_node = init_ast(ast_string);
    string_node->string_value = parser->current_token->value;

    parser_die(parser, TOKEN_STRING);

    return string_node;
}


ast_t* parser_parse_identifier(parser_t* parser){
    if(strcmp(parser->current_token->value , "var") == 0){
        return parser_parse_variable_definition(parser);
    }else{
        return parser_parse_variable(parser);
    }
}
