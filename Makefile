# Compiler and common flags
CC = gcc
COMMON_CFLAGS = -Wall -Werror -Iinc
COMMON_LDFLAGS = -lcrypto

# Normal Build: Debug version (for production or regular debugging)
NORMAL_CFLAGS = $(COMMON_CFLAGS) -g
NORMAL_LDFLAGS = $(COMMON_LDFLAGS)

# Test Debug Build: No optimization, debugging enabled (could include extra test macros)
TEST_DBG_CFLAGS = $(COMMON_CFLAGS) -g# -DTEST_BUILD
TEST_DBG_LDFLAGS = $(COMMON_LDFLAGS)

# Test Fast Build: Optimized for speed
TEST_FAST_CFLAGS = $(COMMON_CFLAGS) -O2# -DTEST_BUILD
TEST_FAST_LDFLAGS = $(COMMON_LDFLAGS)

# Source files for the main application and tests.
# Here, the normal build uses main.c as entry point,
# while the test builds use src/tests.c as the entry point.
MAIN_SRC = main.c src/utils.c src/node.c src/merkleTree.c
TEST_SRC = main_tests.c src/tests.c src/utils.c src/node.c src/merkleTree.c

# Executable targets
NORMAL_TARGET = merkleTree
TEST_DBG_TARGET = merkleTree_test_dbg
TEST_FAST_TARGET = merkleTree_test_fast

.PHONY: all merkleTree_test_dbg merkleTree_test_fast clean

# Default target: Normal Build.
all: $(NORMAL_TARGET)

$(NORMAL_TARGET): $(MAIN_SRC)
	$(CC) $(NORMAL_CFLAGS) $(MAIN_SRC) -o $(NORMAL_TARGET) $(NORMAL_LDFLAGS)

# Test Debug Build: compiles with debug flags and without optimization.
merkleTree_test_dbg: $(TEST_SRC)
	$(CC) $(TEST_DBG_CFLAGS) $(TEST_SRC) -o $(TEST_DBG_TARGET) $(TEST_DBG_LDFLAGS)

# Test Fast Build: compiles with aggressive optimizations.
merkleTree_test_fast: $(TEST_SRC)
	$(CC) $(TEST_FAST_CFLAGS) $(TEST_SRC) -o $(TEST_FAST_TARGET) $(TEST_FAST_LDFLAGS)

# Clean all generated executables.
clean:
	rm -f $(NORMAL_TARGET) $(TEST_DBG_TARGET) $(TEST_FAST_TARGET)
