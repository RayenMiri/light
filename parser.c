#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/parser.h"

void parser_init(Parser* parser, Token* tokens) {
    parser->tokens = tokens;
    parser->current = 0;
}

static Token peek(Parser* parser) {
    return parser->tokens[parser->current];
}

static Token previous(Parser* parser) {
    return parser->tokens[parser->current - 1];
}

static bool is_at_end(Parser* parser) {
    return peek(parser).type == TOKEN_EOF;
}

static Token advance(Parser* parser) {
    if (!is_at_end(parser)) parser->current++;
    return previous(parser);
}

static bool check(Parser* parser, TokenType type) {
    if (is_at_end(parser)) return false;
    return peek(parser).type == type;
}

static bool match(Parser* parser, TokenType type) {
    if (check(parser, type)) {
        advance(parser);
        return true;
    }
    return false;
}

static Token consume(Parser* parser, TokenType type, const char* message) {
    if (check(parser, type)) return advance(parser);
    // Error handling should be implemented here
    exit(1);
}

static Expr* expression(Parser* parser);
static Stmt* declaration(Parser* parser);
static Stmt* statement(Parser* parser);

static Expr* binary(Parser* parser) {
    Expr* expr = expression(parser);

    while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS) ||
           match(parser, TOKEN_STAR) || match(parser, TOKEN_SLASH) ||
           match(parser, TOKEN_EQUAL_EQUAL) || match(parser, TOKEN_BANG_EQUAL) ||
           match(parser, TOKEN_LESS) || match(parser, TOKEN_LESS_EQUAL) ||
           match(parser, TOKEN_GREATER) || match(parser, TOKEN_GREATER_EQUAL)) {
        Token operator = previous(parser);
        Expr* right = expression(parser);
        expr = expr_create_binary(expr, operator, right);
    }

    return expr;
}

static Expr* unary(Parser* parser) {
    if (match(parser, TOKEN_BANG) || match(parser, TOKEN_MINUS)) {
        Token operator = previous(parser);
        Expr* right = unary(parser);
        return expr_create_unary(operator, right);
    }

    return binary(parser);
}

static Expr* primary(Parser* parser) {
    if (match(parser, TOKEN_FALSE)) return expr_create_literal(bool_class_create(false));
    if (match(parser, TOKEN_TRUE)) return expr_create_literal(bool_class_create(true));
    if (match(parser, TOKEN_NIL)) return expr_create_literal(nil_class_create());

    if (match(parser, TOKEN_NUMBER)) {
        double value = strtod(previous(parser).start, NULL);
        return expr_create_literal(float_class_create(value));
    }

    if (match(parser, TOKEN_STRING)) {
        char* value = malloc(previous(parser).length - 1);
        strcpy(value, previous(parser).start + 1);
        value[previous(parser).length - 2] = '\0'; // null-terminate the string
        return expr_create_literal(string_class_create(value));
    }

    if (match(parser, TOKEN_IDENTIFIER)) {
        return expr_create_variable(previous(parser));
    }

    if (match(parser, TOKEN_LEFT_PAREN)) {
        Expr* expr = expression(parser);
        consume(parser, TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
        return expr_create_grouping(expr);
    }

    // Error handling should be implemented here (for later)
    exit(1);
}

static Expr* expression(Parser* parser) {
    return unary(parser);
}

static Stmt* expression_statement(Parser* parser) {
    Expr* expr = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after expression.");
    return stmt_create_expression(expr);
}

static Stmt* print_statement(Parser* parser) {
    Expr* value = expression(parser);
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after value.");
    return stmt_create_print(value);
}

static Stmt* var_declaration(Parser* parser) {
    Token name = consume(parser, TOKEN_IDENTIFIER, "Expect variable name.");

    Expr* initializer = NULL;
    if (match(parser, TOKEN_EQUAL)) {
        initializer = expression(parser);
    }

    consume(parser, TOKEN_SEMICOLON, "Expect ';' after variable declaration.");
    return stmt_create_var(name, initializer);
}

static Stmt* statement(Parser* parser) {
    if (match(parser, TOKEN_PRINT)) return print_statement(parser);
    return expression_statement(parser);
}

static Stmt* declaration(Parser* parser) {
    if (match(parser, TOKEN_VAR)) return var_declaration(parser);
    return statement(parser);
}

Stmt** parser_parse(Parser* parser) {
    printf("test parser_parse");
    Stmt** statements = NULL;
    int count = 0;
    int capacity = 0;

    while (!is_at_end(parser)) {
        printf("whilelllll\n");
        if (count >= capacity) {
            capacity = capacity == 0 ? 8 : capacity * 2;
            statements = realloc(statements, sizeof(Stmt*) * capacity);
        }
        statements[count++] = declaration(parser);
    }

    statements[count] = NULL;
    printf("here\n");
    return statements;
}