/**
 * @file utils.h
 * @author Roman Horshkov
 * @date 20 Feb 2025
 * @brief Utility functions for hashing and file operations in the Merkle Tree.
 */

 #ifndef UTILITIES_MERKLE_TREE_H
 #define UTILITIES_MERKLE_TREE_H
 
 /*-----------------------------------*
  * INCLUDE FILES
  *-----------------------------------*/
 #include "../inc/node.h"
 #include <openssl/sha.h>  /* SHA-256 hashing */
 #include <stdio.h>        /* File I/O operations */
 #include <stdbool.h>      /* Boolean data type */
 #include <dirent.h>       /* Directory operations */
 
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
  * PUBLIC FUNCTION PROTOTYPES
  *-----------------------------------*/
 
 /**
  * @brief Computes the SHA-256 hash of a file.
  *
  * Reads the file in chunks and calculates its SHA-256 hash using OpenSSL.
  *
  * @param filename Path to the file.
  * @param output Buffer to store the 32-byte hash result.
  * @retval true  Success (hash is stored in `output`).
  * @retval false Error opening or reading the file.
  */
 bool HashFile(const char *filename, unsigned char output[SHA256_DIGEST_LENGTH]);
 
 /**
  * @brief Computes the SHA-256 hash of two concatenated hashes.
  *
  * Given two SHA-256 hashes, this function concatenates them and computes the SHA-256 hash of the result.
  *
  * @param hashA First SHA-256 hash (32 bytes).
  * @param hashB Second SHA-256 hash (32 bytes).
  * @param output Buffer to store the resulting 32-byte hash.
  * @retval true  Success.
  * @retval false Failure in hash computation.
  */
 bool HashTwoHashes(const unsigned char hashA[SHA256_DIGEST_LENGTH], 
                    const unsigned char hashB[SHA256_DIGEST_LENGTH], 
                    unsigned char output[SHA256_DIGEST_LENGTH]);
 
 /**
  * @brief Computes the parent node's hash from its children's hashes.
  *
  * If both child nodes exist, their hashes are combined to compute the parent node's hash.
  * If only the left child exists, it copies the left child's hash.
  *
  * @param node Pointer to the node whose hash needs to be computed.
  * @retval true  Success.
  * @retval false Failure (e.g., missing child nodes).
  */
 bool HashNodeFromChildren(struct node_t **node);
 
 /**
  * @brief Counts the number of regular files in a directory.
  *
  * This function scans the specified directory and counts how many regular files it contains, 
  * ignoring `.` and `..` entries.
  *
  * @param folder Path to the directory.
  * @return The number of regular files found, or 0 on failure.
  */
 int CountFilesInDirectory(const char *folder);
 
 /**
  * @brief Computes the number of nodes per level in the Merkle tree.
  *
  * Based on the number of files (leaf nodes), this function calculates the number of nodes 
  * at each level up to the root.
  *
  * @param nodes_number_arr Pointer to store an allocated array containing the node count per level.
  * @param folder Directory containing the transaction files.
  * @return The number of levels in the Merkle tree.
  */
 int NodesNumberArrayFromFile(int **nodes_number_arr, const char *folder);
 
 /**
  * @brief Checks whether a given file exists.
  *
  * @param filename Path to the file.
  * @retval true  If the file exists.
  * @retval false If the file does not exist.
  */
 bool isValidFile(const char *filename);
 
 /* ######################################################################
  * PRINT FUNCTIONS 
  *###################################################################### */
 
 /**
  * @brief Prints the entire Merkle tree structure.
  *
  * Iterates over all levels of the Merkle tree and prints the nodes' details.
  *
  * @param nodes A pointer to the two-dimensional array (matrix) of node_t pointers.
  */
 void PrintMerkleTree(struct node_t ***nodes);
 
 /**
  * @brief Prints an array of integers.
  *
  * @param arr Pointer to the array.
  * @param size Size of the array.
  */
 void pIntArr(int *arr, int size);
 
 /**
  * @brief Prints detailed information about a single node.
  *
  * @param node Pointer to the node.
  */
 void PrintNode(struct node_t *node);
 
 /**
  * @brief Prints a SHA-256 hash in hexadecimal format.
  *
  * @param hash SHA-256 hash (32 bytes).
  */
 void PrintHashHex(const unsigned char hash[SHA256_DIGEST_LENGTH]);
 
 /**
  * @brief Prints a SHA-256 hash in binary format.
  *
  * @param hash SHA-256 hash (32 bytes).
  */
 void PrintHashBinary(const unsigned char hash[SHA256_DIGEST_LENGTH]);
 
 #endif /* UTILITIES_MERKLE_TREE_H */
 