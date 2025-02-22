#ifndef TESTS_H
#define TESTS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Runs the performance and functionality tests for the Merkle tree engine.
 *
 * This function executes a series of tests using different datasets (folders) and prints
 * performance metrics such as elapsed time, user and system CPU time, and maximum resident set size.
 */
void RunMerkleTreeTests(void);

#ifdef __cplusplus
}
#endif

#endif // TESTS_H
