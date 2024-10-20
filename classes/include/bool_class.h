#ifndef BOOL_CLASS_H
#define BOOL_CLASS_H

#include "object.h"
#include <stdbool.h>

typedef struct {
    Object obj;
    bool value;
} BoolObject;

Object* bool_class_create(bool value);

#endif