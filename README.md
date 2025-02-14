# Merkle Tree Project

## Overview
This project implements a Merkle tree to efficiently verify the integrity of a dataset. A Merkle tree is a cryptographic data structure that provides a secure and efficient way to validate large data sets by storing hashes in a hierarchical structure.

## Directory Structure
```
merkle_tree/
│
├── data/                # Folder to store data files
│   ├── root_hash.txt    # Stores the Merkle tree root hash
│   └── transactions/    # Directory containing transaction files
│       ├── block1.txt
│       ├── block2.txt
│       ├── block3.txt
│       └── block4.txt
│
├── inc/                 # Header files
│   ├── node.h           # Header for node structure and related functions
│   ├── merkleTree.h     # Header for Merkle tree functionality
│
├── src/                 # Source files
│   ├── node.c           # Implements node-related functions
│   ├── merkleTree.c     # Implements Merkle tree operations
│
├── main.c               # Main program to build and test the Merkle tree
├── Makefile             # Compilation instructions
└── README.md            # Documentation
```

## Features
- Builds a Merkle tree from transaction files.
- Computes and stores the root hash.
- Provides a command-line menu for interaction.
- Uses OpenSSL for secure SHA-256 hashing.

## File Descriptions
### main.c
- Includes all the necessary header files.
- Initializes the Merkle tree.
- Provides a user interface to interact with the tree.

### src/node.c
- Implements node-related operations such as combining two hashes to create parent nodes.

### src/merkleTree.c
- Reads transaction files and hashes them.
- Constructs the Merkle tree from the hashed transactions.
- Computes the root hash and prints it.

### inc/node.h
- Defines the structure of a Merkle tree node.
- Declares functions for node manipulation.

### inc/merkleTree.h
- Declares functions for constructing and interacting with the Merkle tree.

### Makefile
- Defines rules for compiling the project using `gcc`.

## Usage
### Compilation
To compile the project, run:
```
make
```
This will generate the executable `merkleTree`.

### Running the Program
Execute the compiled program:
```
./merkleTree
```
This will display the interactive menu:
```
===== Merkle Tree Menu =====
1 - Check if root hash exists
2 - Generate and compare root hashes
x - Regenerate root hash
q - Quit
```
Select an option by entering the corresponding number or letter.

### Cleaning Up
To remove compiled files, use:
```
make clean
```

## Dependencies
- OpenSSL (libcrypto) for SHA-256 hashing.
- GCC for compilation.

## Future Improvements
- Improve memory management.
- Add support for larger datasets.
- Implement tree verification methods.

This project demonstrates how to construct and verify a Merkle tree, providing a solid foundation for secure data verification techniques.

