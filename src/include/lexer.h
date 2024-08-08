#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct lexer {
    char c; // Current character
    unsigned int i; // Current character index
    unsigned int line; // Current line number
    unsigned int pos; // Current position in the line
    char* contents;
} lexer_t;

lexer_t* init_lexer(char* contents);

void lexer_advance(lexer_t* lexer);

void lexer_skip_ws(lexer_t* lexer);

token_t* lexer_get_next_token(lexer_t* lexer);

token_t* lexer_collect_string(lexer_t* lexer);

token_t* lexer_collect_number(lexer_t* lexer);

token_t* lexer_collect_id(lexer_t* lexer);

token_t* lexer_advance_with_token(lexer_t* lexer, token_t* token);

char* lexer_get_current_char_as_string(lexer_t* lexer);

char lexer_peek(lexer_t* lexer, int offset);


#endif
