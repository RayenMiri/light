#ifndef LEXER_H
#define LEXER_H

#include "token.h"


typedef struct {
    const char* start;
    const char* current;
    int line;
} Lexer;

void lexer_init(Lexer* lexer, const char* source);
Token lexer_scan_token(Lexer* lexer);

#endif