#ifndef STRING_CLASS_H
#define STRING_CLASS_H

#include "object.h"

typedef struct {
    Object obj;
    char* value;
} StringObject;

Object* string_class_create(const char* value);
void string_class_print(StringObject* self);
Object* string_class_concatenate(StringObject* self, Object* other);
bool string_class_equals(StringObject* self, Object* other);
void string_class_destroy(StringObject* self);

#endif