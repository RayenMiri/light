#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

typedef enum {
    OBJ_INT,
    OBJ_FLOAT,
    OBJ_STRING,
    OBJ_BOOL,
    OBJ_NIL,
    OBJ_FUNCTION,
    OBJ_NATIVE_FUNCTION,
    OBJ_CLASS,
    OBJ_INSTANCE
} ObjectType;

typedef struct Object Object;
typedef struct Class Class;

struct Object {
    ObjectType type;
};

void object_print(Object* object);
bool object_is_truthy(Object* object);
bool object_is_equal(Object* a, Object* b);
void object_destroy(Object* object);

#endif