#ifndef NIL_CLASS_H
#define NIL_CLASS_H

#include "object.h"

typedef struct {
    Object obj;
} NilObject;

Object* nil_class_create(void);

#endif