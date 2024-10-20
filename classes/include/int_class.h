#ifndef INTEGER_CLASS_H
#define INTEGER_CLASS_H

#include "object.h"

typedef struct {
    Object obj;
    int value;
} IntObject;

Object* int_class_create(int value);
void int_class_print(IntObject* self);
Object* int_class_add(IntObject* self, Object* other);
Object* int_class_subtract(IntObject* self, Object* other);
Object* int_class_multiply(IntObject* self, Object* other);
Object* int_class_divide(IntObject* self, Object* other);
bool int_class_equals(IntObject* self, Object* other);

#endif