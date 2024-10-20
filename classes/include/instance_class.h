#ifndef INSTANCE_CLASS_H
#define INSTANCE_CLASS_H

#include "object.h"
#include "class_class.h"
#include "table.h"

typedef struct {
    Object obj;
    ClassObject* klass;
    Table* fields;
} InstanceObject;

Object* instance_class_create(ClassObject* klass);

#endif