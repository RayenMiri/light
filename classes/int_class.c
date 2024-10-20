#include <stdio.h>
#include <stdlib.h>
#include "include/int_class.h"
#include "include/float_class.h"

Object* int_class_create(int value) {
    IntObject* int_obj = malloc(sizeof(IntObject));
    int_obj->obj.type = OBJ_INT;
    int_obj->value = value;
    return (Object*)int_obj;
}

void int_class_print(IntObject* self) {
    printf("%d", self->value);
}

Object* int_class_add(IntObject* self, Object* other) {
    if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return int_class_create(self->value + int_other->value);
    } else if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return float_class_create(self->value + float_other->value);
    }
    // Error: Unsupported operand type for +
    return NULL;
}

Object* int_class_subtract(IntObject* self, Object* other) {
    if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return int_class_create(self->value - int_other->value);
    } else if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return float_class_create(self->value - float_other->value);
    }
    // Error: Unsupported operand type for -
    return NULL;
}

Object* int_class_multiply(IntObject* self, Object* other) {
    if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return int_class_create(self->value * int_other->value);
    } else if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return float_class_create(self->value * float_other->value);
    }
    // Error: Unsupported operand type for *
    return NULL;
}

Object* int_class_divide(IntObject* self, Object* other) {
    if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        if (int_other->value == 0) {
            // Error: Division by zero
            return NULL;
        }
        return int_class_create(self->value / int_other->value);
    } else if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        if (float_other->value == 0.0) {
            // Error: Division by zero
            return NULL;
        }
        return float_class_create(self->value / float_other->value);
    }
    // Error: Unsupported operand type for /
    return NULL;
}

bool int_class_equals(IntObject* self, Object* other) {
    if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return self->value == int_other->value;
    } else if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return self->value == float_other->value;
    }
    return false;
}