#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "stmt.h"
#include "environment.h"
#include "../classes/include/object.h"

typedef struct {
    Environment* environment;
    Environment* globals;
} Interpreter;

void interpreter_init(Interpreter* interpreter);
void interpreter_interpret(Interpreter* interpreter, Stmt** statements);
Object* interpreter_visit_expr(Interpreter* interpreter, Expr* expr);
void interpreter_visit_stmt(Interpreter* interpreter, Stmt* stmt);
void interpreter_destroy(Interpreter* interpreter);
void interpreter_define_global(Interpreter* interpreter, const char* name, Object* value);

#endif