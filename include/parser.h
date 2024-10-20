#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "expr.h"
#include "stmt.h"

typedef struct {
    Token* tokens;
    int current;
} Parser;

void parser_init(Parser* parser, Token* tokens);
Stmt** parser_parse(Parser* parser);

#endif