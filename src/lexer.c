#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

lexer_t* init_lexer(char* contents){
    lexer_t* lexer = calloc(1,sizeof(struct lexer));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];
    return lexer;
}

void lexer_advance(lexer_t* lexer){
    if(lexer->c != '\0' && lexer->i < strlen(lexer->contents)){
        lexer->i+=1;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexer_skip_ws(lexer_t* lexer){
   
    while(lexer->c == ' ' || lexer->c == 10){
        lexer_advance(lexer);
    }
}

token_t* lexer_get_next_token(lexer_t* lexer){
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)){
        if(lexer->c == ' ' || lexer->c == 10){
            lexer_skip_ws(lexer);
        }
        
        if(isalnum(lexer->c)){
            return lexer_collect_id(lexer);
        }
       
        if(lexer->c == '"'){
            return lexer_collect_string(lexer);
        }
        switch (lexer->c)
        {
            case '=': return lexer_advace_with_token(lexer,init_token(TOKEN_ASSIGN,lexer_get_current_char_as_string(lexer))); break;
            case ';': return lexer_advace_with_token(lexer,init_token(TOKEN_SEMI,lexer_get_current_char_as_string(lexer))); break;
            case '(': return lexer_advace_with_token(lexer,init_token(TOKEN_LPAREN,lexer_get_current_char_as_string(lexer))); break;
            case ')': return lexer_advace_with_token(lexer,init_token(TOKEN_RPAREN,lexer_get_current_char_as_string(lexer))); break;
            case '{': return lexer_advace_with_token(lexer,init_token(TOKEN_LBRACE,lexer_get_current_char_as_string(lexer))); break;
            case '}': return lexer_advace_with_token(lexer,init_token(TOKEN_RBRACE,lexer_get_current_char_as_string(lexer))); break;
            case ',': return lexer_advace_with_token(lexer,init_token(TOKEN_COMMA,lexer_get_current_char_as_string(lexer))); break;

            
        }
    };
    return init_token(TOKEN_EOF,"\0");
}

token_t* lexer_advace_with_token(lexer_t* lexer,token_t* token){
    lexer_advance(lexer);
    return token;
}

/*
The lexer_collect_string function collect String Characters: A while loop is used to collect characters until 
the closing double quote ("):
For each character, it gets the current character as a string.
The value string is reallocated to accommodate the new character.
The new character is concatenated to value.

NB : this function can be optimized in the future by addin
Buffer Size Management: Preallocate a larger buffer and resize only when necessary.
Manual Concatenation: Avoid calling strcat and manage the string concatenation manually.
*/

token_t* lexer_collect_string(lexer_t* lexer){
    lexer_advance(lexer);
    char* value = calloc(1,sizeof(char));
    value[0] = '\0';
    while(lexer->c != '"'){
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value,(strlen(value)+strlen(s)+1)*sizeof(char));
        strcat(value,s);
        lexer_advance(lexer);
    }
    lexer_advance(lexer);
    return init_token(TOKEN_STRING , value);
}

token_t* lexer_collect_id(lexer_t* lexer){
    char* value = calloc(1,sizeof(char));
    value[0] = '\0';
    while(isalnum(lexer->c)){
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value,(strlen(value)+strlen(s)+1)*sizeof(char));
        strcat(value,s);
        lexer_advance(lexer);
    }
    return init_token(TOKEN_IDENTIFIER , value);
}

char* lexer_get_current_char_as_string(lexer_t* lexer){
    char* str = calloc(1,sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';
    return str;
}