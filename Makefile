# Define variables
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:.c=.o)
FLAGS := -g
EXEC := light.exe

# Default target to build and run the executable with a specified file
run: $(EXEC)
	@if not exist "$(file)" ( \
		echo "Error: The file '$(file)' does not exist"; \
		exit 1; \
	)
	@powershell -Command "if ('$(file)'.Substring('$(file)'.Length - 6) -ne '.light') { exit 1 }" || ( \
		echo "Error: Only .light files are supported for execution"; \
		exit 1; \
	)
	.\$(EXEC) $(file)

# Target to build the executable
$(EXEC): $(OBJECTS)
	gcc $(OBJECTS) $(FLAGS) -o $(EXEC)

# Rule to build object files
%.o: %.c include/%.h
	gcc -c $(FLAGS) $< -o $@

# Clean up object files and executables
clean:
	del /Q src\*.o
	del /Q $(EXEC)
