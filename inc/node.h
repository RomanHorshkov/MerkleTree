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
#include <openssl/sha.h>                /* SHA-256 hashing */
#include <stdint.h>                     /* for uint8_t */

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
/* None */

#endif /* MERKLE_NODE_H */
