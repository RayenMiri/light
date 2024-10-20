#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/interpreter.h"

void print_tokens(Token* tokens, int count) {
    printf("Tokens:\n");
    for (int i = 0; i < count; i++) {
        printf("Type: %d, Lexeme: %.*s\n", tokens[i].type, tokens[i].length, tokens[i].start);
    }
    printf("\n");
}

void run(const char* source) {
    printf("Source code:\n%s\n\n", source);

    // Lexer test
    Lexer lexer;
    lexer_init(&lexer, source);

    Token* tokens = NULL;
    int token_count = 0;
    int token_capacity = 8;
    tokens = malloc(sizeof(Token) * token_capacity);

    for (;;) {
        Token token = lexer_scan_token(&lexer);
        if (token_count >= token_capacity) {
            token_capacity *= 2;
            tokens = realloc(tokens, sizeof(Token) * token_capacity);
        }
        tokens[token_count++] = token;
        if (token.type == TOKEN_EOF) break;
    }

    print_tokens(tokens, token_count);

    // Parser test
    Parser parser;
    printf("Parsing started");
    parser_init(&parser, tokens);
    printf("parser initiayed successfully");
    Stmt** statements = parser_parse(&parser);
    printf("here");
    if (statements == NULL) {
        printf("Parsing failed.\n");
        return;
    }

    printf("Parsing successful.\n\n");

    // Interpreter test
    Interpreter interpreter;
    interpreter_init(&interpreter);
    interpreter_interpret(&interpreter, statements);

    // Clean up
    for (int i = 0; statements[i] != NULL; i++) {
        // Free statements (implement stmt_destroy if not already done)
        // stmt_destroy(statements[i]);
    }
    free(statements);
    free(tokens);
    interpreter_destroy(&interpreter);
}

int main() {
    const char* test_programs[] = {
        "var x = 5;\nvar y = 10;\nprint x + y;",
        "if (true) { print \"Hello, World!\"; } else { print \"Goodbye, World!\"; }",
        "var i = 0;\nwhile (i < 5) { print i; i = i + 1; }",
        "function add(a, b) { return a + b; }\nprint add(3, 4);",
        "class Person { init(name) { this.name = name; } sayHello() { print \"Hello, I'm \" + this.name; } }\nvar person = Person(\"Alice\");\nperson.sayHello();"
    };

    int num_tests = sizeof(test_programs) / sizeof(test_programs[0]);

    for (int i = 0; i < num_tests; i++) {
        printf("Test %d:\n", i + 1);
        printf("----------------------------------------\n");
        run(test_programs[i]);
        printf("----------------------------------------\n\n");
    }

    return 0;
}