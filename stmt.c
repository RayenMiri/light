#include <stdlib.h>
#include "include/stmt.h"

Stmt* stmt_create_expression(Expr* expression) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_EXPRESSION;
    stmt->expression.expression = expression;
    return stmt;
}

Stmt* stmt_create_print(Expr* expression) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_PRINT;
    stmt->print.expression = expression;
    return stmt;
}

Stmt* stmt_create_var(Token name, Expr* initializer) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_VAR;
    stmt->var.name = name;
    stmt->var.initializer = initializer;
    return stmt;
}

Stmt* stmt_create_block(Stmt** statements) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_BLOCK;
    stmt->block.statements = statements;
    return stmt;
}

Stmt* stmt_create_if(Expr* condition, Stmt* then_branch, Stmt* else_branch) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_IF;
    stmt->if_stmt.condition = condition;
    stmt->if_stmt.then_branch = then_branch;
    stmt->if_stmt.else_branch = else_branch;
    return stmt;
}

Stmt* stmt_create_while(Expr* condition, Stmt* body) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_WHILE;
    stmt->while_stmt.condition = condition;
    stmt->while_stmt.body = body;
    return stmt;
}

Stmt* stmt_create_function(Token name, Token* params, int arity, Stmt** body) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_FUNCTION;
    stmt->function.name = name;
    stmt->function.params = params;
    stmt->function.arity = arity;
    stmt->function.body = body;
    return stmt;
}

Stmt* stmt_create_return(Expr* value) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_RETURN;
    stmt->return_stmt.value = value;
    return stmt;
}

Stmt* stmt_create_class(Token name, Expr* superclass, Stmt** methods) {
    Stmt* stmt = malloc(sizeof(Stmt));
    stmt->type = STMT_CLASS;
    stmt->class_stmt.name = name;
    stmt->class_stmt.superclass = superclass;
    stmt->class_stmt.methods = methods;
    return stmt;
}

void stmt_destroy(Stmt* stmt) {
    switch (stmt->type) {
        case STMT_EXPRESSION:
            expr_destroy(stmt->expression.expression);
            break;
        case STMT_PRINT:
            expr_destroy(stmt->print.expression);
            break;
        case STMT_VAR:
            expr_destroy(stmt->var.initializer);
            break;
        case STMT_BLOCK:
            for (int i = 0; stmt->block.statements[i] != NULL; i++) {
                stmt_destroy(stmt->block.statements[i]);
            }
            free(stmt->block.statements);
            break;
        case STMT_IF:
            expr_destroy(stmt->if_stmt.condition);
            stmt_destroy(stmt->if_stmt.then_branch);
            if (stmt->if_stmt.else_branch != NULL) {
                stmt_destroy(stmt->if_stmt.else_branch);
            }
            break;
        case STMT_WHILE:
            expr_destroy(stmt->while_stmt.condition);
            stmt_destroy(stmt->while_stmt.body);
            break;
        case STMT_FUNCTION:
            free(stmt->function.params);
            for (int i = 0; stmt->function.body[i] != NULL; i++) {
                stmt_destroy(stmt->function.body[i]);
            }
            free(stmt->function.body);
            break;
        case STMT_RETURN:
            if (stmt->return_stmt.value != NULL) {
                expr_destroy(stmt->return_stmt.value);
            }
            break;
        case STMT_CLASS:
            if (stmt->class_stmt.superclass != NULL) {
                expr_destroy(stmt->class_stmt.superclass);
            }
            for (int i = 0; stmt->class_stmt.methods[i] != NULL; i++) {
                stmt_destroy(stmt->class_stmt.methods[i]);
            }
            free(stmt->class_stmt.methods);
            break;
    }
    free(stmt);
}