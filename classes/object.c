#include <stdio.h>
#include <stdlib.h>
#include "include/object.h"
#include "include/int_class.h"
#include "include/float_class.h"
#include "include/string_class.h"
#include "include/bool_class.h"

void object_print(Object* object) {
    switch (object->type) {
        case OBJ_INT:
            int_class_print((IntObject*)object);
            break;
        case OBJ_FLOAT:
            float_class_print((FloatObject*)object);
            break;
        case OBJ_STRING:
            string_class_print((StringObject*)object);
            break;
        case OBJ_BOOL:

            printf(((IntObject*)object)->value ? "true" : "false");
            break;
        case OBJ_NIL:
            printf("nil");
            break;
        case OBJ_FUNCTION:
        case OBJ_NATIVE_FUNCTION:
            printf("<function>");
            break;
        case OBJ_CLASS:
            printf("<class>");
            break;
        case OBJ_INSTANCE:
            printf("<instance>");
            break;
    }
}

bool object_is_truthy(Object* object) {
    switch (object->type) {
        case OBJ_BOOL:
            return ((IntObject*)object)->value;
        case OBJ_NIL:
            return false;
        case OBJ_INT:
            return ((IntObject*)object)->value != 0;
        case OBJ_FLOAT:
            return ((FloatObject*)object)->value != 0.0;
        case OBJ_STRING:
            return ((StringObject*)object)->value[0] != '\0';
        default:
            return true;
    }
}

bool object_is_equal(Object* a, Object* b) {
    if (a->type != b->type) return false;

    switch (a->type) {
        case OBJ_INT:
            return int_class_equals((IntObject*)a, b);
        case OBJ_FLOAT:
            return float_class_equals((FloatObject*)a, b);
        case OBJ_STRING:
            return string_class_equals((StringObject*)a, b);
        case OBJ_BOOL:
            return ((IntObject*)a)->value == ((IntObject*)b)->value;
        case OBJ_NIL:
            return true;
        default:
            return a == b;
    }
}

void object_destroy(Object* object) {
    switch (object->type) {
        case OBJ_STRING:
            string_class_destroy((StringObject*)object);
            break;
        default:
            free(object);
            break;
    }
}