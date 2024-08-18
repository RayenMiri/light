#include "include/token.h"
#include <stdlib.h>

token_t* init_token(int type, char* value, unsigned int line, unsigned int pos) {
    token_t* token = calloc(1, sizeof(struct token));
    token->type = type;
    token->value = value;
    token->line = line;
    token->pos = pos;

    return token;
}

const char* token_type_to_string(int type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMI: return "SEMI";
        case TOKEN_NL: return "NEWLINE";
        case TOKEN_WS: return "WHITESPACE";
        case TOKEN_FN: return "FUNCTION";
        case TOKEN_EOF: return "EOF";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MUL: return "MULTIPLY";
        case TOKEN_DIV: return "DIVIDE";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSIF: return "ELSIF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_RETURN: return "RETURN";
        
        default: return "UNKNOWN";
    }
}
