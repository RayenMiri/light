# Define variables
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:.c=.o)
FLAGS := -g
EXEC := light.exe

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
