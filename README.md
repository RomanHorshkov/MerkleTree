Overall description and files architecture:

merkle_tree/
│
├── data/                # Folder to store data files
│   ├─── root_hash.txt/  # Root hash 
│   └─── tranasactions/  # Transactions list to hash
│       ├── block1.txt
│       ├── block2.txt
│       ├── block3.txt
│       └── block4.txt
│
├── inc/                  # Header files
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
└── README.md             # Documentation

Detailed Description:
main.c
- includes all the header files.
- initialises the Merkle tree.
- initialises the menu for merke tree usage.
MAIN MENU: 
    1 - check if root hash exists
    2 - generate and compare root hashes
    x - regenerate root hash
    q - quit

Compilation:
For now:
<!-- gcc -Wall -Werror -g -Iinc main.c src/node.c -o merkle_tree  with debugging -->
gcc -Wall -Werror -Iinc main.c src/node.c -o merkle_tree
