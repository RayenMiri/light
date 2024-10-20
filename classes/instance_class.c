#include <stdlib.h>
#include "include/instance_class.h"

Object* instance_class_create(ClassObject* klass) {
    InstanceObject* obj = malloc(sizeof(InstanceObject));
    obj->obj.type = OBJ_INSTANCE;
    obj->klass = klass;
    obj->fields = table_create();
    return (Object*)obj;
}