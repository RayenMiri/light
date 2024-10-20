#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "../classes/include/object.h"

typedef enum {
    EXPR_BINARY,
    EXPR_GROUPING,
    EXPR_LITERAL,
    EXPR_UNARY,
    EXPR_VARIABLE,
    EXPR_CALL
} ExprType;

typedef struct Expr Expr;

struct Expr {
    ExprType type;
    union {
        struct {
            Expr* left;
            Token operator;
            Expr* right;
        } binary;
        struct {
            Expr* expression;
        } grouping;
        struct {
            Object* value;
        } literal;
        struct {
            Token operator;
            Expr* right;
        } unary;
        struct {
            Token name;
        } variable;
        struct {
            Expr* callee;
            Token paren;
            Expr** arguments;
            int arg_count;
        } call;
    };
};

Expr* expr_create_binary(Expr* left, Token operator, Expr* right);
Expr* expr_create_grouping(Expr* expression);
Expr* expr_create_literal(Object* value);
Expr* expr_create_unary(Token operator, Expr* right);
Expr* expr_create_variable(Token name);
Expr* expr_create_call(Expr* callee, Token paren, Expr** arguments, int arg_count);
void expr_destroy(Expr* expr);

#endif