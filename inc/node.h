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


/* Future work:
 * Instead of using separate structs for leaves, internal nodes, and the root, you can define a generic node structure with pointers to children and a parent. This will simplify the tree traversal and memory management.
 */

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
#define HASH_SIZE 32 // hash size for sha 256

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
	uint8_t hash[HASH_SIZE];
	struct node_t *parent;
	struct node_t *rchild;
	struct node_t *lchild;
};


/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/
/**
 * @brief 
 * @param [out] d
 * @param [out] h
 * @param [out] m
 * @retval TRUE     Success
 * @retval FALSE    Error
 */
/**
 * @brief try to hash :D 
 */
void TryToHash(void);

#endif /* MERKLE_NODE_H */
