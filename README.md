Overall description and files architecture:

merkle_tree/
│
├── data/                 # Folder to store data files
│   ├── block1.txt
│   ├── block2.txt
│   ├── block3.txt
│   └── block4.txt
│
├── include/              # Header files
│   ├── node.h
│   ├── merkle.h
│   └── hash.h
│
├── src/                  # Source files
│   ├── node.c
│   ├── merkle.c
│   └── hash.c
│
├── main.c                # Main program to build and test the Merkle tree
├── Makefile              # To compile everything
└── README.md             # (Optional) For documentation

Detailed Description:
main.c
- includes all the header files.
- initialises the Merkle tree.
- initialises the menu for merke tree usage.
MAIN MENU: 
    1 - check if root hash exists
    2 - generate and compare root hashes
    x - regenerate root hash
