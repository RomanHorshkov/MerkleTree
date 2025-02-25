cmake_minimum_required(VERSION 3.10)
project(MerkleTree)

set(CMAKE_C_STANDARD 99)

# Enable compile_commands.json and link_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_LINK_COMMANDS ON)

# Source files
set(SRC_FILES
    src/utils.c
    src/node.c
    src/merkleTree.c
)

# Normal build (default)
add_executable(merkleTree main.c ${SRC_FILES})
target_include_directories(merkleTree PRIVATE inc)
target_link_libraries(merkleTree crypto)

# Test builds (only built if explicitly requested)
add_executable(merkleTree_test_dbg main_tests.c src/tests.c ${SRC_FILES})
target_include_directories(merkleTree_test_dbg PRIVATE inc)
target_link_libraries(merkleTree_test_dbg crypto)
target_compile_options(merkleTree_test_dbg PRIVATE -g)  # Debug flags

add_executable(merkleTree_test_fast main_tests.c src/tests.c ${SRC_FILES})
target_include_directories(merkleTree_test_fast PRIVATE inc)
target_link_libraries(merkleTree_test_fast crypto)
target_compile_options(merkleTree_test_fast PRIVATE -O2)  # Optimization
