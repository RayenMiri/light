#include <stdlib.h>
#include <string.h>
#include "include/class_class.h"

Object* class_class_create(char* name, ClassObject* superclass) {
    ClassObject* obj = malloc(sizeof(ClassObject));
    obj->obj.type = OBJ_CLASS;
    obj->name = strdup(name);
    obj->superclass = superclass;
    obj->methods = table_create();
    return (Object*)obj;
}