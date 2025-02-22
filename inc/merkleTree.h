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
#include "../inc/node.h"                /* Node structure */
#include <stdbool.h>                    /* booleans */
#include <dirent.h>                     /* for counting files */
#include <stdint.h>                     /* for uint8_t*/
#include <string.h>                     /* for strings */
#include <openssl/evp.h>                /* EVP API for SHA-256 */

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

/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/

/**
 * @brief builds the merkleTree
 * does all the memory management
*/
void BuildMerkleTree(const char *folder);

#endif /* MERKLE_TREE_H */