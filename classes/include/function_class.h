#ifndef FUNCTION_CLASS_H
#define FUNCTION_CLASS_H

#include "object.h"
#include "stmt.h"
#include "environment.h"
#include "../include/interpreter.h"
struct FunctionObject {
    Object obj;
    char* name;
    Token* params;
    int arity;
    Stmt** body;
    Environment* closure;
    Object* (*call)(Interpreter* interpreter, struct FunctionObject* function, int arg_count, Object** arguments);
};

typedef struct FunctionObject FunctionObject;

Object* function_class_create(char* name, Token* params, int arity, Stmt** body, Environment* closure);
Object* function_class_call(Interpreter* interpreter, FunctionObject* function, int arg_count, Object** arguments);

#endif