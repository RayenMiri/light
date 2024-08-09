#ifndef TOKEN_H
#define TOKEN_H

typedef struct token {
    enum {
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_STRING,
        TOKEN_BOOL,
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
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_MUL,
        TOKEN_DIV,
        TOKEN_IF,
        TOKEN_WHILE,
        TOKEN_RETURN,
        TOKEN_LT,   // <
        TOKEN_GT,   // >
        TOKEN_EQ,   // ==
        TOKEN_LE,   // <=
        TOKEN_GE,   // >=
        TOKEN_NE,   // !=
        TOKEN_NOT,
        TOKEN_UNKNOWN,
    } type;
    char* value;
    unsigned int line;
    unsigned int pos;
} token_t;

token_t* init_token(int type, char* value, unsigned int line, unsigned int pos);
const char* token_type_to_string(int type);

#endif
