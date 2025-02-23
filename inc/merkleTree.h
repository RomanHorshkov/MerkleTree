/**
 * @file merkleTree.h
 * @author Roman Horshkov
 * @date 13 Feb 2025
 * @brief merkle node header file
 */

 #ifndef MERKLE_TREE_H
 #define MERKLE_TREE_H

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/utils.h"               /* utilities */

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
extern struct node_t ***nodes;                 /* nodes of the tree */

/* Root Node definition: 
 * ptr to the root node, head */
extern struct node_t *root_node;

/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/

/**
 * @brief builds the merkleTree
 * does all the memory management
*/
void BuildMerkleTree(const char *filename);

#endif /* MERKLE_TREE_H */