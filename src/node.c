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
#include <dirent.h> // for counting files
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
#define BUFFER_SIZE 4096  // Define the missing BUFFER_SIZE

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
 * @brief function that reads the data
 * files and stores them in nodes
*/
void ReadFilesData(unsigned char output[SHA256_DIGEST_LENGTH]);
bool HashFile(unsigned char output[SHA256_DIGEST_LENGTH]);
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
bool HashTwoNodes(struct node_t* lch, struct node_t* rch);
void BuildMerkleTree(void);
struct node_t* GenerateLeaveNode(unsigned char output[SHA256_DIGEST_LENGTH]);
void PrintHash(const unsigned char hash[SHA256_DIGEST_LENGTH]);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
struct node_t* leaves_ptrs[100]; /* list of leaves pointers */

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void TryToHash(unsigned char output[SHA256_DIGEST_LENGTH])
{
	/* fills the leaves_ptrs array */
	ReadFilesData(output);
	BuildMerkleTree();
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/

void ReadFilesData(unsigned char output[SHA256_DIGEST_LENGTH])
{
    while (HashFile(output))
    {
		*leaves_ptrs = GenerateLeaveNode(output);
    	printf("\nObtained hash: \n");
		PrintHash(**leaves_ptrs.hash);
		leaves_ptrs++; /* increment leaves count */
    }
}

struct node_t* GenerateLeaveNode(unsigned char output[SHA256_DIGEST_LENGTH])
{
	/* Allocate space on heap for a node */
	struct node_t *node = malloc(sizeof(struct node_t));
	/* Check the node */
	if (!node)
	{
    	fprintf(stderr, "Memory allocation failed\n");
    	exit(EXIT_FAILURE);
	}
	else
	{
		/* add its hash */
		memcpy(node->hash, output, SHA256_DIGEST_LENGTH);
		// Initialize pointers to NULL (indicating no children/parent yet)
		node->parent = NULL;
		node->lchild = NULL;
		node->rchild = NULL;
	}
	return node;
}

void BuildMerkleTree(void)
{

}

bool HashFile(unsigned char output[SHA256_DIGEST_LENGTH])
{
	bool ret = false;
	unsigned char buffer[BUFFER_SIZE];
	size_t bytesRead;

    static int files_number = 1;
    char filename[100]; // Store the file name
    /* Fill the filename */
    snprintf(filename, sizeof(filename), "data/transactions/block_%d.txt", files_number++);

    /* Open the file */
    FILE *file = fopen(filename, "rb");

	if(file)
	{
		EVP_MD_CTX *mdctx = EVP_MD_CTX_new(); // Create a new EVP_MD context
		const EVP_MD *md = EVP_sha256(); // Get SHA-256 method
		EVP_DigestInit_ex(mdctx, md, NULL);

		while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
		{
			EVP_DigestUpdate(mdctx, buffer, bytesRead);
		}
		EVP_DigestFinal_ex(mdctx, output, NULL);
		EVP_MD_CTX_free(mdctx); // Clean up

		fclose(file);
		ret = true;
	}
	else
	{
		/* do nothing */
	}

	return ret;
}

bool HashTwoNodes(struct node_t* lch, struct node_t* rch,
					struct node_t **parent)
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

    /* Allocate a new parent node. */
    *parent = malloc(sizeof(struct node_t));
    if (*parent == NULL) {
        return false;
    }

    // Set the new parent's hash to the computed hash.
    memcpy((*parent)->hash, new_hash, SHA256_DIGEST_LENGTH);

    // Link the children to the new parent.
    (*parent)->lchild = lch;
    (*parent)->rchild = rch;
    (*parent)->parent = NULL;  // Parent's parent can be set later if needed.

    // Set the parent pointers in the child nodes.
    lch->parent = *parent;
    rch->parent = *parent;

    return true;
}

void PrintHash(const unsigned char hash[SHA256_DIGEST_LENGTH])
{
    printf("SHA-256 Hash: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
        printf("%02x", hash[i]);  // Print each byte as two hex characters
    }
    printf("\n");
}