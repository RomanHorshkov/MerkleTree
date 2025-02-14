# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -Iinc
LDFLAGS = -lcrypto  # Add OpenSSL library

# Source and object files
SRC_FILES = main.c src/node.c src/merkleTree.c
OBJ_FILES = main.o src/node.o src/merkleTree.o

TARGET = merkleTree

# Default rule: Build the executable
all: $(TARGET)

# Compile object files from their respective source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Explicit rule for node.o because it's inside src/
src/node.o: src/node.c
	$(CC) $(CFLAGS) -c src/node.c -o src/node.o

# Explicit rule for merkleTree.o because it's inside src/
src/merkleTree.o: src/merkleTree.c
	$(CC) $(CFLAGS) -c src/merkleTree.c -o src/merkleTree.o

# Link object files to create the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)
