#include <stdio.h>
#include <stdlib.h>
#include "include/float_class.h"
#include "include/int_class.h"

Object* float_class_create(double value) {
    FloatObject* float_obj = malloc(sizeof(FloatObject));
    float_obj->obj.type = OBJ_FLOAT;
    float_obj->value = value;
    return (Object*)float_obj;
}

void float_class_print(FloatObject* self) {
    printf("%f", self->value);
}

Object* float_class_add(FloatObject* self, Object* other) {
    if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return float_class_create(self->value + float_other->value);
    } else if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return float_class_create(self->value + int_other->value);
    }
    // Error: Unsupported operand type for +
    return NULL;
}

Object* float_class_subtract(FloatObject* self, Object* other) {
    if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return float_class_create(self->value - float_other->value);
    } else if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return float_class_create(self->value - int_other->value);
    }
    // Error: Unsupported operand type for -
    return NULL;
}

Object* float_class_multiply(FloatObject* self, Object* other) {
    if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return float_class_create(self->value * float_other->value);
    } else if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return float_class_create(self->value * int_other->value);
    }
    // Error: Unsupported operand type for *
    return NULL;
}

Object* float_class_divide(FloatObject* self, Object* other) {
    if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        if (float_other->value == 0.0) {
            // Error: Division by zero
            return NULL;
        }
        return float_class_create(self->value / float_other->value);
    } else if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        if (int_other->value == 0) {
            // Error: Division by zero
            return NULL;
        }
        return float_class_create(self->value / int_other->value);
    }
    // Error: Unsupported operand type for /
    return NULL;
}

bool float_class_equals(FloatObject* self, Object* other) {
    if (other->type == OBJ_FLOAT) {
        FloatObject* float_other = (FloatObject*)other;
        return self->value == float_other->value;
    } else if (other->type == OBJ_INT) {
        IntObject* int_other = (IntObject*)other;
        return self->value == int_other->value;
    }
    return false;
}