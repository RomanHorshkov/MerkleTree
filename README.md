# Merkle Tree Project

## Overview
This project implements a Merkle tree to efficiently verify the integrity of a dataset. A Merkle tree is a cryptographic data structure that provides a secure and efficient way to validate large data sets by storing hashes in a hierarchical structure.

## Features

- Builds a Merkle Tree from multiple transaction files.
- Calculates SHA-256 hashes using [OpenSSL](https://www.openssl.org/) (libcrypto).
- Uses a hierarchical structure of nodes to aggregate file hashes into a single root hash.
- Provides two different entry points:
  1. `main.c` for interactive menu usage.
  2. `main_tests.c` for performance and functionality tests (logs to `tests_results.txt`).


## Usage
### Compilation
## Compilation

You can build three different versions:

- **Default / Debug**:
Produces `merkleTree` (from `main.c`).

- **Test Debug**:  
Produces `merkleTree_test_dbg` (from `main_tests.c`) with debug symbols.

- **Test Fast**:  
Produces `merkleTree_test_fast` (from `main_tests.c`) but optimized (`-O2`).

### Cleaning Up
To remove compiled files, use:
```
make clean
```

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

### Test Mode

If you build `merkleTree_test_dbg` or `merkleTree_test_fast`, run: `./merkleTree_test_dbg` (or `./merkleTree_test_fast`) to exercise the automated tests. The steps are:
1. **Generate test folders & files** by reading from `test_spec.txt`.
2. **Run Merkle Tree building** for each test dataset.
3. **Record** performance/memory metrics in `tests_results.txt`.
4. **Remove** the test folders.

Check `tests_results.txt` for timing and resource usage (RSS, page faults, etc.).

## Future Improvements

- Enhanced memory management for extremely large data sets.
- Parallel hashing for multi-core performance.
- On-demand node allocation to handle partial trees or streaming data.
- Detailed verification paths (e.g., proofs for a single file’s membership).

## License

This project is licensed under the terms of the MIT license.


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
│   ├── merkleTree.h
│   ├── node.h
|   ├── tests.h
|   └── utils.h
│
├── src/                 # Source files
│   ├── merkleTree.c     # Implements Merkle tree operations
│   ├── node.c           # Implements node-related functions
│   ├── tests.c          # Implements tests
│   └── utils.c          # Implements node-related functions
│
├── main.c               # Main program to build and test the Merkle tree
├── Makefile             # Compilation instructions
├── test_spec.txt        # Tests specifications
├── tests_results.txt    # Tests outcomes
└── README.md            # Documentation
```

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

## Dependencies
- OpenSSL (libcrypto) for SHA-256 hashing.
- GCC for compilation.

## Future Improvements
- Improve memory management.
- Implement tree verification methods.

### Concluding Notes


## Tests metrics:
1.1 Elapsed time:
The “wall-clock” time (in milliseconds, here) from when you start building the tree until it completes. Includes any time spent blocked (sleeping, I/O wait, etc.) plus actual CPU execution time. A direct measure of how long an end user actually waits for the operation to finish. If you want the entire build process or test suite to be faster overall, lowering this is usually the main goal.

1.2 User CPU time:
The amount of time (in milliseconds) that the CPU actually spent executing instructions in user space. “User space” means this program code and libraries, not operating system kernel code. A measure of how CPU-heavy this application logic is, ignoring OS overhead. If you have large computations, heavy loops, or hashing tasks, user CPU time will reflect that load.

1.3 System CPU time:
The time (in milliseconds) spent in kernel space on behalf of this process.
For instance, if you call malloc, do file I/O, or system("cls") / system("clear")`, the OS itself may do some work. If this code triggers a lot of syscalls (filesystem, memory management, etc.), you can see system CPU time spike. Minimizing frequent small syscalls or excessive memory allocations can reduce system time.

1.4 MAX RSS (KB):
Resident Set Size (RSS) is how much memory (in kilobytes) is actually held in physical RAM for the process. This is the peak (maximum) resident set size over the lifetime of the test, as reported by getrusage. Indicates how memory-hungry this code is at its peak. If you see very large RSS usage, you might have allocated data structures bigger than necessary or in a less efficient way.

1.5 /proc/self/status output (Linux) 
VmPeak: The peak virtual memory size used by the process.
VmSize: The current total virtual memory size. This can include memory mapped files, shared libraries, etc.
VmHWM (“high water mark”): The peak physical memory usage—similar to Max RSS but read from a different kernel interface.
VmRSS: The current physical memory usage (“Resident Set Size”) at this instant.
VmData: The size of the process’s data segment (heap, global/static vars).
VmStk: The stack usage.
VmExe: The size of the text segment (executable code).
VmLib: Memory used by loaded shared libraries.
All these help gauge how this program’s memory usage changes during runtime.

1.6 Minor page faults:
The count of “soft” page faults this process encountered since you started measuring. A minor fault usually happens when the process tries to access a page that’s in memory but not “mapped” to the process yet (so the OS just has to fix up page tables). Minor faults can indicate frequent small allocations or expansions of memory that cause the OS to update page tables. Usually less severe for performance than major faults, but if they’re extremely high, you could see overhead.

1.7 Major page faults:
“Hard” page faults requiring data to be fetched from disk (i.e., swapped out pages or not in memory at all). Can severely impact performance if you’re hitting swap. Zero major faults is great—it means this working set is fully in RAM.

1.8 Blocks input / Blocks output:
The number of block I/O operations, e.g., reading or writing from a file or device block. getrusage tracks these as ru_inblock and ru_oublock. If this code does heavy file I/O (or is forced to swap memory in/out from disk), you might see large block I/O numbers.
