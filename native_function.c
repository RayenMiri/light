#include <stdlib.h>
#include <time.h>
#include "include/native_function.h"

NativeFunctionObject* native_function_create(int (*arity)(void), Object* (*call)(Interpreter*, Object**)) {
    NativeFunctionObject* function = malloc(sizeof(NativeFunctionObject));  // Correct size
    if (function == NULL) {
        // Handle memory allocation failure if necessary
        return NULL;
    }
    function->obj.type = OBJ_NATIVE_FUNCTION;
    function->arity = arity;
    function->call = call;
    return function;
}

static int clock_arity() {
    return 0;
}

static Object* clock_call(Interpreter* interpreter, Object** arguments) {
    return float_class_create((double)time(NULL));
}

void define_native_functions(Environment* environment) {
    // Ensure 'environment' is defined and used properly
    environment_define(environment, "clock", (Object*)native_function_create(clock_arity, clock_call));
}
