# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude -Iclasses/include

# Source files
SRC = $(wildcard *.c) $(wildcard classes/*.c)

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
EXEC = your_project.exe

# Default target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	del /Q $(OBJ) $(EXEC)

# Phony targets
.PHONY: all clean
