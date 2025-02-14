/**
 * @file node.h
 * @author Roman Horshkov
 * @date 12 Feb 2025
 * @brief merkle node header file
 */

#ifndef MERKLE_NODE_H
#define MERKLE_NODE_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include <stdint.h> // for uin8_t
#include <openssl/sha.h>


/* Future work:
 * Instead of using separate structs for leaves, internal nodes, and the root, you can define a generic node structure with pointers to children and a parent. This will simplify the tree traversal and memory management.
 */

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
/* Node structure for the merkle tree */
struct node_t {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	uint8_t number;
	struct node_t *parent;
	struct node_t *rchild;
	struct node_t *lchild;
};


/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/
// /**
//  * @brief 
//  * @param [out] d
//  * @param [out] h
//  * @param [out] m
//  * @retval TRUE     Success
//  * @retval FALSE    Error
//  */
// /**
//  * @brief try to hash :D 
//  */
// void TryToHash(unsigned char output[SHA256_DIGEST_LENGTH]);

#endif /* MERKLE_NODE_H */
