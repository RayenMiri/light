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
        case TOKEN_IDENTIFIER: return "IDENTIFIER";//0
        case TOKEN_NUMBER: return "NUMBER";//1
        case TOKEN_STRING: return "STRING";//2
        case TOKEN_ASSIGN: return "ASSIGN";//3
        case TOKEN_LPAREN: return "LPAREN";//4
        case TOKEN_RPAREN: return "RPAREN";//5
        case TOKEN_LBRACE: return "LBRACE";//6
        case TOKEN_RBRACE: return "RBRACE";//7
        case TOKEN_COMMA: return "COMMA";//8
        case TOKEN_SEMI: return "SEMI";//9
        case TOKEN_NL: return "NEWLINE";//10
        case TOKEN_WS: return "WHITESPACE";//11
        case TOKEN_FN: return "FUNCTION";//12
        case TOKEN_EOF: return "EOF";//13
        case TOKEN_PLUS: return "PLUS";//14
        case TOKEN_MINUS: return "MINUS";//15
        case TOKEN_MUL: return "MULTIPLY";//16
        case TOKEN_DIV: return "DIVIDE";//17
        case TOKEN_IF: return "IF";//18
        case TOKEN_ELSIF: return "ELSIF";//19
        case TOKEN_ELSE: return "ELSE";//20
        case TOKEN_WHILE: return "WHILE";//21
        case TOKEN_FOR: return "FOR";//22
        case TOKEN_RETURN: return "RETURN";//23
        
        default: return "UNKNOWN";
    }
}
