#ifndef STMT_H
#define STMT_H

#include "expr.h"
#include "token.h"

typedef enum {
    STMT_EXPRESSION,
    STMT_PRINT,
    STMT_VAR,
    STMT_BLOCK,
    STMT_IF,
    STMT_WHILE,
    STMT_FUNCTION,
    STMT_RETURN,
    STMT_CLASS
} StmtType;

typedef struct Stmt Stmt;

struct Stmt {
    StmtType type;
    union {
        struct {
            Expr* expression;
        } expression;
        struct {
            Expr* expression;
        } print;
        struct {
            Token name;
            Expr* initializer;
        } var;
        struct {
            Stmt** statements;
        } block;
        struct {
            Expr* condition;
            Stmt* then_branch;
            Stmt* else_branch;
        } if_stmt;
        struct {
            Expr* condition;
            Stmt* body;
        } while_stmt;
        struct {
            Token name;
            Token* params;
            int arity;
            Stmt** body;
        } function;
        struct {
            Expr* value;
        } return_stmt;
        struct {
            Token name;
            Expr* superclass;
            Stmt** methods;
        } class_stmt;
    };
};

Stmt* stmt_create_expression(Expr* expression);
Stmt* stmt_create_print(Expr* expression);
Stmt* stmt_create_var(Token name, Expr* initializer);
Stmt* stmt_create_block(Stmt** statements);
Stmt* stmt_create_if(Expr* condition, Stmt* then_branch, Stmt* else_branch);
Stmt* stmt_create_while(Expr* condition, Stmt* body);
Stmt* stmt_create_function(Token name, Token* params, int arity, Stmt** body);
Stmt* stmt_create_return(Expr* value);
Stmt* stmt_create_class(Token name, Expr* superclass, Stmt** methods);
void stmt_destroy(Stmt* stmt);

#endif