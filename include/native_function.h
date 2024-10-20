#ifndef NATIVE_FUNCTION_H
#define NATIVE_FUNCTION_H

#include "../classes/include/object.h"
#include "interpreter.h"

typedef Object* (*NativeFunction)(Interpreter* interpreter, int arg_count, Object** arguments);

typedef struct {
    Object obj;
    NativeFunction function; // Ensure this is used properly
    int (*arity)(void);
    Object* (*call)(Interpreter*, Object**);
} NativeFunctionObject;

// Declare all functions used
NativeFunctionObject* native_function_create(int (*arity)(void), Object* (*call)(Interpreter*, Object**));
void define_native_functions(Environment* environment); // Ensure this function is declared

#endif
