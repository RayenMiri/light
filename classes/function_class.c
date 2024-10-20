#include <stdlib.h>
#include <string.h>
#include "include/function_class.h"

Object* function_class_create(char* name, Token* params, int arity, Stmt** body, Environment* closure) {
    FunctionObject* obj = malloc(sizeof(FunctionObject));
    obj->obj.type = OBJ_FUNCTION;
    obj->name = strdup(name);
    obj->arity = arity;
    obj->body = body;
    obj->closure = closure;
    return (Object*)obj;
}

Object* function_class_call(Interpreter* interpreter, FunctionObject* function, int arg_count, Object** arguments){
    // This function should be implemented in the interpreter
    // as it needs access to the interpreter's state
    return NULL;
}