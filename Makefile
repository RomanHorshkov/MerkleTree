# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -Iinc
LDFLAGS = -lcrypto  # OpenSSL library

# Source and object files
SRC_FILES = main.c src/node.c src/merkleTree.c src/utils.c
OBJ_FILES = main.o src/node.o src/merkleTree.o src/utils.o

TARGET = merkleTree

# Default rule: Build the executable
all: $(TARGET)

# Compile object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Explicit rules for object files in src/
src/node.o: src/node.c
	$(CC) $(CFLAGS) -c src/node.c -o src/node.o

src/merkleTree.o: src/merkleTree.c
	$(CC) $(CFLAGS) -c src/merkleTree.c -o src/merkleTree.o

src/utils.o: src/utils.c
	$(CC) $(CFLAGS) -c src/utils.c -o src/utils.o

# Link object files to create the final executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(TARGET) $(LDFLAGS)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)
