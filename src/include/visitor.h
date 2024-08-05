#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"

typedef struct visitor
{
    ast_t ** variable_definitions;
    size_t variable_definitions_size;
    
}visitor_t;

visitor_t* init_visitor();

ast_t* visitor_visit(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_variable_def(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_variable(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_exp(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_string(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_number(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_compound(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_statement(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_func_call(visitor_t* visitor, ast_t* node);

ast_t* visitor_visit_ast_func_def(visitor_t* visitor, ast_t* node);

char* process_escaped_sequences(const char* str);

#endif