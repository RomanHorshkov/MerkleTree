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
extern struct node_t **leave_nodes;      /* list of leaves pointers */

/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/
struct node_t ** BuildMerkleTree(void);

#endif /* MERKLE_TREE_H */