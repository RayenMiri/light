#ifndef CLASS_CLASS_H
#define CLASS_CLASS_H

#include "object.h"
#include "table.h"

typedef struct ClassObject ClassObject;

struct ClassObject {
    Object obj;
    char* name;
    ClassObject* superclass;
    Table* methods;
};

Object* class_class_create(char* name, ClassObject* superclass);

#endif