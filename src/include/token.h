#ifndef TOKEN_H
#define TOKEN_H
typedef struct token
{
    enum {
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_STRING,
        TOKEN_ASSIGN,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_COMMA,
        TOKEN_SEMI,
        TOKEN_NL,
        TOKEN_WS,
        TOKEN_FN,
        TOKEN_EOF,
        TOKEN_UNKNOWN
    }type;
    char* value;
}token_t;

token_t* init_token(int type,char* value);

#endif