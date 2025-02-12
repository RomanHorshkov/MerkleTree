# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -g -Iinc

# Source files
SRC_FILES = main.c src/node.c
TARGET = merkle_tree

# Default rule: Compile everything
all:
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET)
