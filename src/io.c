#include "include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_file_content(const char* file_path) {
    char* buffer = NULL;
    long length;
    FILE* f = fopen(file_path, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = calloc(length + 1, sizeof(char));  // Allocate memory + 1 for null terminator
        if (buffer) {
            fread(buffer, 1, length, f);
            buffer[length] = '\0';  // Null-terminate the string
            // Normalize newlines
            char* p = buffer;
            while (*p) {
                if (*p == '\r' && *(p + 1) == '\n') {
                    memmove(p, p + 1, strlen(p));
                } else if (*p == '\r') {
                    *p = '\n';
                }
                p++;
            }
        }
        fclose(f);
    } else {
        printf("Error reading file %s\n", file_path);
        exit(2);
    }

    return buffer;
}
