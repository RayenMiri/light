#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct lexer
{
    char c; //current caracter that we are on
    unsigned int i ; //current caaracter index that we are on 
    char* contents;

} lexer_t;

lexer_t* init_lexer(char* contents);

void lexer_advance(lexer_t* lexer);

void lexer_skip_ws(lexer_t* lexer);

token_t* lexer_get_next_token(lexer_t* lexer);

token_t* lexer_collect_string(lexer_t* lexer);

token_t* lexer_collect_id(lexer_t* lexer);

token_t* lexer_advance_with_token(lexer_t* lexer,token_t* token);

char* lexer_get_current_char_as_string(lexer_t* lexer);


#endif