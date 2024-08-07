#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "ast.h"
#include "lexer.h"
#include "scope.h"



typedef struct parser
{
    lexer_t* lexer;
    token_t* current_token; 
    token_t* prev_token;
    scope_t* scope;

}parser_t;

parser_t* init_parser(lexer_t* lexer);

void syntax_error(parser_t* parser, const char* message);

void parser_consume(parser_t* parser, int token_type);//method that excepts a certain type of token

ast_t* parser_parse(parser_t* parser,scope_t* scope);//return the whole AST

ast_t* parser_parse_statement(parser_t* parser,scope_t* scope);

ast_t* parser_parse_statements(parser_t* parser,scope_t* scope);

ast_t* parser_parse_exp(parser_t* parser,scope_t* scope);

ast_t* parser_parse_factor(parser_t* parser,scope_t* scope);

ast_t* parser_parse_term(parser_t* parser,scope_t* scope);

ast_t* parser_parse_expression(parser_t* parser, scope_t* scope);

ast_t* parser_parse_func_call(parser_t* parser,scope_t* scope);

ast_t* parser_parse_func_definition(parser_t* parser,scope_t* scope);

ast_t* parser_parse_variable(parser_t* parser,scope_t* scope);

ast_t* parser_parse_variable_definition(parser_t* parser,scope_t* scope);

ast_t* parser_parse_identifier(parser_t* parser,scope_t* scope);

ast_t* parser_parse_string(parser_t* parser,scope_t* scope);

ast_t* parser_parse_number(parser_t* parser,scope_t* scope);






#endif