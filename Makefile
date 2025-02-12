# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -Iinc

# Source and object files
SRC_FILES = main.c src/node.c
OBJ_FILES = main.o src/node.o  # Make sure node.o is inside src/

TARGET = merkle_tree

# Default rule: Build the executable
all: $(TARGET)

# Compile object files from their respective source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Explicit rule for node.o because it's inside src/
src/node.o: src/node.c
	$(CC) $(CFLAGS) -c src/node.c -o src/node.o

# Link object files to create the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)
