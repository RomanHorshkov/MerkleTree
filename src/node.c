/**
 * @file node.c
 * @author Roman Horshkov
 * @date 12 Feb 2025
 * @brief merkle node engine
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/node.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/evp.h> // New EVP API for SHA-256

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/

/*-----------------------------------*
 * PRIVATE MACROS
 *-----------------------------------*/
/* None */
/*-----------------------------------*
 * PRIVATE TYPEDEFS
 *-----------------------------------*/
/* None */
/*-----------------------------------*
 * PRIVATE FUNCTION PROTOTYPES
 *-----------------------------------*/
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
bool HashTwoNodes(struct node_t* lch, struct node_t* rch, struct node_t *parent);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
bool HashTwoNodes(struct node_t* lch, struct node_t* rch,
					struct node_t *parent)
	{
    // Validate input parameters
    if (lch == NULL || rch == NULL || parent == NULL) {
        return false;
    }
    /* Create a buffer to hold the concatenated hashes.
    Each hash is SHA256_DIGEST_LENGTH bytes so create a
	buffer of 2 * SHA256_DIGEST_LENGTH. */
    unsigned char combined[2 * SHA256_DIGEST_LENGTH];
    memcpy(combined, lch->hash, SHA256_DIGEST_LENGTH);
    memcpy(combined + SHA256_DIGEST_LENGTH, rch->hash, SHA256_DIGEST_LENGTH);

    // Compute the SHA-256 of the combined buffer.
    unsigned char new_hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        return false;
    }
    const EVP_MD *md = EVP_sha256();
    if (EVP_DigestInit_ex(mdctx, md, NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }
    if (EVP_DigestUpdate(mdctx, combined, sizeof(combined)) != 1) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }
    if (EVP_DigestFinal_ex(mdctx, new_hash, NULL) != 1) {
        EVP_MD_CTX_free(mdctx);
        return false;
    }
    EVP_MD_CTX_free(mdctx);

    // /* Allocate a new parent node. */
    // *parent = malloc(sizeof(struct node_t));
    // if (*parent == NULL) {
    //     return false;
    // }

    // // Set the new parent's hash to the computed hash.
    // memcpy((*parent)->hash, new_hash, SHA256_DIGEST_LENGTH);

    // // Link the children to the new parent.
    // (*parent)->lchild = lch;
    // (*parent)->rchild = rch;
    // (*parent)->parent = NULL;  // Parent's parent can be set later if needed.

    // // Set the parent pointers in the child nodes.
    // lch->parent = *parent;
    // rch->parent = *parent;

    return true;
}
