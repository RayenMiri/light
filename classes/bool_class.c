#include <stdlib.h>
#include "include/bool_class.h"

Object* bool_class_create(bool value) {
    BoolObject* obj = malloc(sizeof(BoolObject));
    obj->obj.type = OBJ_BOOL;
    obj->value = value;
    return (Object*)obj;
}