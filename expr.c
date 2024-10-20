#include <stdlib.h>
#include "include/expr.h"

Expr* expr_create_binary(Expr* left, Token operator, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_BINARY;
    expr->binary.left = left;
    expr->binary.operator = operator;
    expr->binary.right = right;
    return expr;
}

Expr* expr_create_grouping(Expr* expression) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_GROUPING;
    expr->grouping.expression = expression;
    return expr;
}

Expr* expr_create_literal(Object* value) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_LITERAL;
    expr->literal.value = value;
    return expr;
}

Expr* expr_create_unary(Token operator, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_UNARY;
    expr->unary.operator = operator;
    expr->unary.right = right;
    return expr;
}

Expr* expr_create_variable(Token name) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_VARIABLE;
    expr->variable.name = name;
    return expr;
}

Expr* expr_create_call(Expr* callee, Token paren, Expr** arguments, int arg_count) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_CALL;
    expr->call.callee = callee;
    expr->call.paren = paren;
    expr->call.arguments = arguments;
    expr->call.arg_count = arg_count;
    return expr;
}

void expr_destroy(Expr* expr) {
    switch (expr->type) {
        case EXPR_BINARY:
            expr_destroy(expr->binary.left);
            expr_destroy(expr->binary.right);
            break;
        case EXPR_GROUPING:
            expr_destroy(expr->grouping.expression);
            break;
        case EXPR_LITERAL:
            object_destroy(expr->literal.value);
            break;
        case EXPR_UNARY:
            expr_destroy(expr->unary.right);
            break;
        case EXPR_VARIABLE:
            // Nothing to free for variable
            break;
        case EXPR_CALL:
            expr_destroy(expr->call.callee);
            for (int i = 0; i < expr->call.arg_count; i++) {
                expr_destroy(expr->call.arguments[i]);
            }
            free(expr->call.arguments);
            break;
    }
    free(expr);
}