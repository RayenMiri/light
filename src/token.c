#include "include/token.h"
#include <stdlib.h>

token_t* init_token(int type,char* value,unsigned int  line,unsigned int pos){
    token_t* token = calloc(1,sizeof(struct token));
    token->type = type;
    token->value = value;
    token->line = line;
    token->pos = pos;
    
    
    return token;
}


const char* token_type_to_string(int type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMI: return "SEMI";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EOF: return "EOF";
        
        // Add other token types as necessary
        default: return "UNKNOWN";
    }
}

