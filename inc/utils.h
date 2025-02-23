/**
 * @file utils.h
 * @author Roman Horshkov
 * @date 20 Feb 2025
 * @brief utilities: hashing
 */


#ifndef UTILITIES_MERKLE_TREE_H
#define UTILITIES_MERKLE_TREE_H


/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/node.h"
#include <openssl/sha.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC VARIABLE DECLARATIONS
 *-----------------------------------*/
/* None */
 
/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/
/**
 * @brief Computes the SHA-256 hash of a file.
 * @param filename Path to the file.
 * @param output Buffer to store the 32-byte hash result.
 * @retval 0 Success
 * @retval -1 Error opening or reading the file
 */
bool HashFile(const char *filename, unsigned char output[SHA256_DIGEST_LENGTH]);

/**
 * @brief Computes SHA-256 of two concatenated SHA-256 hashes.
 * @param hashA First SHA-256 hash (32 bytes).
 * @param hashB Second SHA-256 hash (32 bytes).
 * @param output Buffer to store the resulting 32-byte hash.
 */
bool HashTwoHashes(const unsigned char hashA[SHA256_DIGEST_LENGTH], 
                const unsigned char hashB[SHA256_DIGEST_LENGTH], 
                unsigned char output[SHA256_DIGEST_LENGTH]);
/**
 * @brief Combines the hashes of two child nodes into a parent node.
 *
 * This function concatenates the hash values from the left and right children,
 * computes the SHA-256 digest of concatenated data, creates a new parent node
 * with that digest, and sets the child-parent pointers.
 *
 * @param lch Pointer to the left child node.
 * @param rch Pointer to the right child node.
 * @param parent Pointer to a pointer where the newly created parent node will be stored.
 * @retval true  On success.
 * @retval false On failure.
 */
bool HashNodeFromChildren(struct node_t **parent);

int CountFilesInDirectory(const char *file_name);
int NodesNumberArrayFromFile(int **nodes_number_arr, int n_files);
bool isValidFile(const char *filename);

/* ######################################################################
 * PRINT FUNCTIONS 
###################################################################### */

void PrintMerkleTree(struct node_t ***nodes);
void pIntArr(int *arr, int size);
void PrintNode(struct node_t *node);
/**
 * @brief Prints a SHA-256 hash in hexadecimal format.
 * @param hash SHA-256 hash (32 bytes).
 */
void PrintHashHex(const unsigned char hash[SHA256_DIGEST_LENGTH]);

/**
 * @brief Prints a SHA-256 hash in binary format.
 * @param hash SHA-256 hash (32 bytes).
 */
void PrintHashBinary(const unsigned char hash[SHA256_DIGEST_LENGTH]);


#endif /* MERKLE_TREE_H */