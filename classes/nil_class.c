#include <stdlib.h>
#include "include/nil_class.h"

Object* nil_class_create(void) {
    NilObject* obj = malloc(sizeof(NilObject));
    obj->obj.type = OBJ_NIL;
    return (Object*)obj;
}