#ifndef FLOAT_CLASS_H
#define FLOAT_CLASS_H

#include "object.h"

typedef struct {
    Object obj;
    double value;
} FloatObject;

Object* float_class_create(double value);
void float_class_print(FloatObject* self);
Object* float_class_add(FloatObject* self, Object* other);
Object* float_class_subtract(FloatObject* self, Object* other);
Object* float_class_multiply(FloatObject* self, Object* other);
Object* float_class_divide(FloatObject* self, Object* other);
bool float_class_equals(FloatObject* self, Object* other);

#endif