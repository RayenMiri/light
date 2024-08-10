#include "include/lexer.h"
#include "include/token.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

lexer_t* init_lexer(char* contents) {
    lexer_t* lexer = calloc(1, sizeof(lexer_t));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->line = 1; // Initialize line number to 1
    lexer->pos = 0;  // Initialize position to 0
    lexer->c = contents[lexer->i];
    return lexer;
}

void lexer_advance(lexer_t* lexer) {
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1;
        lexer->pos += 1; // Increment position in the line
        lexer->c = lexer->contents[lexer->i];

        if (lexer->c == '\n') { // Handle new lines
            lexer->line += 1;
            lexer->pos = 0;
        }
    }
}

void lexer_skip_ws(lexer_t* lexer) {
    while (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' || lexer->c == '\r') {
        lexer_advance(lexer);
    }
}

token_t* lexer_get_next_token(lexer_t* lexer) {
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        if (isspace(lexer->c)) {
            lexer_skip_ws(lexer);
            continue;
        }

        if (isalpha(lexer->c) || lexer->c == '_') {
            return lexer_collect_id(lexer);
        }

        if (isdigit(lexer->c)) {
            return lexer_collect_number(lexer); // Assuming you have a function to collect numbers
        }


        switch (lexer->c) {
            case '"': return lexer_collect_string(lexer);
            case '(': return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case ')': return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '{': return lexer_advance_with_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '}': return lexer_advance_with_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case ';': return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case ',': return lexer_advance_with_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '=': 
                if (lexer_peek(lexer, 1) == '=') {
                    
                    lexer_advance(lexer);
                    
                    return lexer_advance_with_token(lexer, init_token(TOKEN_EQ, "==", lexer->line, lexer->pos));
                }
                return lexer_advance_with_token(lexer, init_token(TOKEN_ASSIGN, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '<': 
                if (lexer_peek(lexer, 1) == '=') {
                    
                    lexer_advance(lexer);
                    
                    return lexer_advance_with_token(lexer, init_token(TOKEN_LE, "<=", lexer->line, lexer->pos));
                }
                return lexer_advance_with_token(lexer, init_token(TOKEN_LT, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '>': 
                if (lexer_peek(lexer, 1) == '=') {
                    
                    lexer_advance(lexer);
                    
                    return lexer_advance_with_token(lexer, init_token(TOKEN_GE, ">=", lexer->line, lexer->pos));
                }
                return lexer_advance_with_token(lexer, init_token(TOKEN_GT, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '!': 
                if (lexer_peek(lexer, 1) == '=') {
                    
                    lexer_advance(lexer);
                    
                    return lexer_advance_with_token(lexer, init_token(TOKEN_NE, "!=", lexer->line, lexer->pos));
                }
                return lexer_advance_with_token(lexer, init_token(TOKEN_NOT, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '+': return lexer_advance_with_token(lexer, init_token(TOKEN_PLUS, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '-': return lexer_advance_with_token(lexer, init_token(TOKEN_MINUS, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '*': return lexer_advance_with_token(lexer, init_token(TOKEN_MUL, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            case '/': return lexer_advance_with_token(lexer, init_token(TOKEN_DIV, lexer_get_current_char_as_string(lexer), lexer->line, lexer->pos));
            
        }

        // If we reach here, we encountered an unexpected character
        char* unexpected_char = lexer_get_current_char_as_string(lexer);
        printf("Unexpected character '%s' at line %d, position %d\n", unexpected_char, lexer->line, lexer->pos);
        free(unexpected_char);
        exit(1);
    }

    return init_token(TOKEN_EOF, "", lexer->line, lexer->pos);
}

token_t* lexer_collect_string(lexer_t* lexer) {
    lexer_advance(lexer); // Skip the opening quote
    char* value = calloc(1, sizeof(char));
    while (lexer->c != '"' && lexer->c != '\0') {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        free(s);
        lexer_advance(lexer);
    }

    lexer_advance(lexer); // Skip the closing quote
    return init_token(TOKEN_STRING, value, lexer->line, lexer->pos);
}

token_t* lexer_collect_number(lexer_t* lexer) {
    char* value = calloc(1, sizeof(char)); // Allocate initial memory
    int is_float = 0; // Flag to indicate if we encounter a decimal point
    
    while (isdigit(lexer->c) || lexer->c == '.') {
        if (lexer->c == '.') {
            if (is_float) {
                // Multiple decimal points, invalid number
                printf("Unexpected character '.' at line %d, position %d\n", lexer->line, lexer->pos);
                free(value);
                exit(1);
            }
            is_float = 1; // Set flag for floating point number
        }
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        free(s);
        lexer_advance(lexer);
    }
    // Return the token for number
    return init_token(TOKEN_NUMBER, value, lexer->line, lexer->pos);
}

/*token_t* lexer_collect_bool(lexer_t* lexer) {
    return init_token(TOKEN_BOOL, value, lexer->line, lexer->pos);
}*/

token_t* lexer_collect_id(lexer_t* lexer) {
    char* value = calloc(1, sizeof(char));

    // Ensure the identifier does not start with a digit
    if (isdigit(lexer->c)) {
        printf("Error: Identifiers cannot start with a digit at line %d, position %d\n", lexer->line, lexer->pos);
        exit(1);
    }

    while (isalnum(lexer->c) || lexer->c == '_') {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        free(s);
        lexer_advance(lexer);
    }
    if (strcmp(value, "if") == 0) return init_token(TOKEN_IF, value, lexer->line, lexer->pos);
    if (strcmp(value, "elsif") == 0) return init_token(TOKEN_ELSIF, value, lexer->line, lexer->pos);
    if (strcmp(value, "else") == 0) return init_token(TOKEN_ELSE, value, lexer->line, lexer->pos);
    if (strcmp(value, "while") == 0) return init_token(TOKEN_WHILE, value, lexer->line, lexer->pos);
    if (strcmp(value, "return") == 0) return init_token(TOKEN_RETURN, value, lexer->line, lexer->pos);
    if (strcmp(value, "true") == 0) return init_token(TOKEN_BOOL, value, lexer->line, lexer->pos);
    if (strcmp(value, "false") == 0) return init_token(TOKEN_BOOL, value, lexer->line, lexer->pos);

    // Additional checks can be added here if needed

    return init_token(TOKEN_IDENTIFIER, value, lexer->line, lexer->pos);
}

token_t* lexer_advance_with_token(lexer_t* lexer, token_t* token) {
    lexer_advance(lexer);
    return token;
}

char* lexer_get_current_char_as_string(lexer_t* lexer) {
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';
    return str;
}


// Peek ahead in the input buffer
char lexer_peek(lexer_t* lexer, int offset) {
    int peek_pos = lexer->i + offset;
    // Check if the peek position is within the bounds of the input
    if (peek_pos >= strlen(lexer->contents)) {
        return '\0'; // End of input
    }

    // Return the character at the peek position
    return lexer->contents[peek_pos];
}