#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/string_class.h"

Object* string_class_create(const char* value) {
    StringObject* string_obj = malloc(sizeof(StringObject));
    string_obj->obj.type = OBJ_STRING;
    string_obj->value = strdup(value);
    return (Object*)string_obj;
}

void string_class_print(StringObject* self) {
    printf("%s", self->value);
}

Object* string_class_concatenate(StringObject* self, Object* other) {
    if (other->type == OBJ_STRING) {
        StringObject* string_other = (StringObject*)other;
        size_t new_length = strlen(self->value) + strlen(string_other->value) + 1;
        char* new_value = malloc(new_length);
        strcpy(new_value, self->value);
        strcat(new_value, string_other->value);
        Object* result = string_class_create(new_value);
        free(new_value);
        return result;
    }
    // Error: Unsupported operand type for +
    return NULL;
}

bool string_class_equals(StringObject* self, Object* other) {
    if (other->type == OBJ_STRING) {
        StringObject* string_other = (StringObject*)other;
        return strcmp(self->value, string_other->value) == 0;
    }
    return false;
}

void string_class_destroy(StringObject* self) {
    free(self->value);
    free(self);
}