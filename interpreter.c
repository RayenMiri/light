#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/interpreter.h"
#include "include/environment.h"
#include "classes/include/object.h"
#include "classes/include/int_class.h"
#include "classes/include/float_class.h"
#include "classes/include/string_class.h"
#include "classes/include/bool_class.h"
#include "classes/include/nil_class.h"
#include "classes/include/function_class.h"
#include "classes/include/class_class.h"
#include "classes/include/instance_class.h"

static Object* evaluate(Interpreter* interpreter, Expr* expr);
static void execute(Interpreter* interpreter, Stmt* stmt);
static Object* interpret_unary_expr(Interpreter* interpreter, Expr* expr);
static Object* interpret_binary_expr(Interpreter* interpreter, Expr* expr);
static Object* interpret_call_expr(Interpreter* interpreter, Expr* expr);

void interpreter_init(Interpreter* interpreter) {
    interpreter->environment = environment_create(NULL);
    interpreter->globals = interpreter->environment;
    //init_native_functions(interpreter);
}

void interpreter_interpret(Interpreter* interpreter, Stmt** statements) {
    for (int i = 0; statements[i] != NULL; i++) {
        execute(interpreter, statements[i]);
    }
}

static Object* evaluate(Interpreter* interpreter, Expr* expr) {
    switch (expr->type) {
        case EXPR_LITERAL:
            return expr->literal.value;
        case EXPR_GROUPING:
            return evaluate(interpreter, expr->grouping.expression);
        case EXPR_UNARY:
            return interpret_unary_expr(interpreter, expr);
        case EXPR_BINARY:
            return interpret_binary_expr(interpreter, expr);
        case EXPR_VARIABLE:
            return environment_get(interpreter->environment, expr->variable.name);
        case EXPR_CALL:
            return interpret_call_expr(interpreter, expr);
        default:
            // Error: Unknown expression type
            return nil_class_create();
    }
}

static Object* interpret_unary_expr(Interpreter* interpreter, Expr* expr) {
    Object* right = evaluate(interpreter, expr->unary.right);

    switch (expr->unary.operator.type) {
        case TOKEN_MINUS:
            if (right->type == OBJ_INT) {
                return int_class_create(-((IntObject*)right)->value);
            } else if (right->type == OBJ_FLOAT) {
                return float_class_create(-((FloatObject*)right)->value);
            }
            // Error: Operand must be a number
            break;
        case TOKEN_BANG:
            return bool_class_create(!object_is_truthy(right));
        default:
            // Error: Unknown unary operator
            break;
    }

    return nil_class_create();
}

static Object* interpret_binary_expr(Interpreter* interpreter, Expr* expr) {
    Object* left = evaluate(interpreter, expr->binary.left);
    Object* right = evaluate(interpreter, expr->binary.right);

    switch (expr->binary.operator.type) {
        case TOKEN_PLUS:
            if (left->type == OBJ_INT && right->type == OBJ_INT) {
                return int_class_create(((IntObject*)left)->value + ((IntObject*)right)->value);
            }
            if (left->type == OBJ_FLOAT || right->type == OBJ_FLOAT) {
                double left_val = (left->type == OBJ_INT) ? ((IntObject*)left)->value : ((FloatObject*)left)->value;
                double right_val = (right->type == OBJ_INT) ? ((IntObject*)right)->value : ((FloatObject*)right)->value;
                return float_class_create(left_val + right_val);
            }
            if (left->type == OBJ_STRING && right->type == OBJ_STRING) {
                char* result = malloc(strlen(((StringObject*)left)->value) + strlen(((StringObject*)right)->value) + 1);
                strcpy(result, ((StringObject*)left)->value);
                strcat(result, ((StringObject*)right)->value);
                Object* str_obj = string_class_create(result);
                free(result);
                return str_obj;
            }
            // Error: Invalid operands for +
            break;
        case TOKEN_MINUS:
            // Implement subtraction
            break;
        case TOKEN_STAR:
            // Implement multiplication
            break;
        case TOKEN_SLASH:
            // Implement division
            break;
        // Implement other binary operators (>, <, >=, <=, ==, !=)
        default:
            // Error: Unknown binary operator
            break;
    }

    return nil_class_create();
}

static Object* interpret_call_expr(Interpreter* interpreter, Expr* expr) {
    Object* callee = evaluate(interpreter, expr->call.callee);

    Object** arguments = malloc(sizeof(Object*) * expr->call.arg_count);
    for (int i = 0; i < expr->call.arg_count; i++) {
        arguments[i] = evaluate(interpreter, expr->call.arguments[i]);
    }

    if (callee->type != OBJ_FUNCTION && callee->type != OBJ_NATIVE_FUNCTION && callee->type != OBJ_CLASS) {
        // Error: Can only call functions and classes
        free(arguments);
        return nil_class_create();
    }

    if (callee->type == OBJ_FUNCTION || callee->type == OBJ_NATIVE_FUNCTION) {
        FunctionObject* function = (FunctionObject*)callee;
        if (expr->call.arg_count != function->arity) {
            // Error: Expected [arity] arguments but got [arg_count]
            free(arguments);
            return nil_class_create();
        }

        Object* result = function_class_call(interpreter, function, expr->call.arg_count, arguments);
        free(arguments);
        return result;
    } else {  // OBJ_CLASS
        ClassObject* klass = (ClassObject*)callee;
        Object* instance = instance_class_create(klass);
        // Call constructor if it exists
        Object* init = table_get(klass->methods, "init");
        if (init != NULL && init->type == OBJ_FUNCTION) {
            function_class_call(interpreter, (FunctionObject*)init, expr->call.arg_count, arguments);
        }
        free(arguments);
        return instance;
    }
}

static void execute(Interpreter* interpreter, Stmt* stmt) {
    switch (stmt->type) {
        case STMT_EXPRESSION:
            evaluate(interpreter, stmt->expression.expression);
            break;
        case STMT_PRINT:
            {
                Object* value = evaluate(interpreter, stmt->print.expression);
                object_print(value);
                printf("\n");
            }
            break;
        case STMT_VAR:
            {
                Object* value = NULL;
                if (stmt->var.initializer != NULL) {
                    value = evaluate(interpreter, stmt->var.initializer);
                }
                environment_define(interpreter->environment, token_get_lexeme(&stmt->var.name), value);
            }
            break;
        case STMT_BLOCK:
            {
                Environment* previous = interpreter->environment;
                interpreter->environment = environment_create(previous);
                for (int i = 0; stmt->block.statements[i] != NULL; i++) {
                    execute(interpreter, stmt->block.statements[i]);
                }
                interpreter->environment = previous;
            }
            break;
        case STMT_IF:
            {
                if (object_is_truthy(evaluate(interpreter, stmt->if_stmt.condition))) {
                    execute(interpreter, stmt->if_stmt.then_branch);
                } else if (stmt->if_stmt.else_branch != NULL) {
                    execute(interpreter, stmt->if_stmt.else_branch);
                }
            }
            break;
        case STMT_WHILE:
            {
                while (object_is_truthy(evaluate(interpreter, stmt->while_stmt.condition))) {
                    execute(interpreter, stmt->while_stmt.body);
                }
            }
            break;
        case STMT_FUNCTION:
            {
                FunctionObject* function = function_class_create(token_get_lexeme(&stmt->function.name), stmt->function.params, stmt->function.arity, stmt->function.body, interpreter->environment);
                environment_define(interpreter->environment, token_get_lexeme(&stmt->function.name), (Object*)function);
            }
            break;
        case STMT_RETURN:
            // Implement return statement
            break;
        case STMT_CLASS:
            {
                Object* superclass = NULL;
                if (stmt->class_stmt.superclass != NULL) {
                    superclass = evaluate(interpreter, stmt->class_stmt.superclass);
                    if (superclass->type != OBJ_CLASS) {
                        // Error: Superclass must be a class
                        return;
                    }
                }

                environment_define(interpreter->environment, token_get_lexeme(&stmt->class_stmt.name), nil_class_create());

                if (stmt->class_stmt.superclass != NULL) {
                    interpreter->environment = environment_create(interpreter->environment);
                    environment_define(interpreter->environment, "super", superclass);
                }

                ClassObject* klass = class_class_create(token_get_lexeme(&stmt->class_stmt.name), (ClassObject*)superclass);

                for (int i = 0; stmt->class_stmt.methods[i] != NULL; i++) {
                    FunctionObject* method = function_class_create(
                        token_get_lexeme(&stmt->class_stmt.methods[i]->function.name),
                        stmt->class_stmt.methods[i]->function.params,
                        stmt->class_stmt.methods[i]->function.arity,
                        stmt->class_stmt.methods[i]->function.body,
                        interpreter->environment
                    );
                    table_set(klass->methods, token_get_lexeme(&stmt->class_stmt.methods[i]->function.name), (Object*)method);
                }

                if (stmt->class_stmt.superclass != NULL) {
                    interpreter->environment = interpreter->environment->enclosing;
                }

                environment_assign(interpreter->environment, stmt->class_stmt.name, (Object*)klass);
            }
            break;
    }
}

void interpreter_define_global(Interpreter* interpreter, const char* name, Object* value) {
    environment_define(interpreter->globals, name, value);
}

void interpreter_destroy(Interpreter* interpreter) {
    environment_destroy(interpreter->globals);
}