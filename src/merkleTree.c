/**
 * @file merkleTree.c
 * @author Roman Horshkov
 * @date 13 Feb 2025
 * @brief merkle tree engine
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/merkleTree.h"

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
struct node_t **leave_nodes = NULL;  // Actual definition

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/

/* Define file reading buffer size */
#define BUFFER_SIZE 4096

/* Define transaction data folder */
#define TRANSACTIONS_FOLDER "data/transactions/"
#define FILE_NAME_MAX_LENGTH 50

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
 * @brief Hash a file
*/
bool HashFile(unsigned char hash[SHA256_DIGEST_LENGTH], const char *filename);

/**
 * @brief generates leaf node from input
*/
void GenerateAndStoreLeaveNode(uint8_t fileNo);

/**
 * @brief builds the merkleTree
 * does all the memory management
*/
struct node_t ** BuildMerkleTree(void);

/**
 * @brief generates the correct filename
*/
int8_t GenerateFilename(char *ret, uint8_t fileNo);

/**
 * @brief counts the number of transaction files
*/
uint8_t CountTransactionFiles(void);

/**
 * @brief prints a node info
*/
void PrintNode(struct node_t* node);

/**
 * @brief prints the leaves hashes
*/
void PrintLeavesHashes(uint8_t n_files);

/**
 * @brief prints a hash
*/
void PrintHash(const unsigned char hash[SHA256_DIGEST_LENGTH]);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/

bool HashFile(unsigned char output[SHA256_DIGEST_LENGTH], const char *filename)
{
    bool ret = false;
    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;
    /* Clean output */
    memset(output, 0, SHA256_DIGEST_LENGTH);
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

    	printf("\nObtained file hash from %s:", filename);
		PrintHash(output);
        fclose(file);
        ret = true;
    }
    else
    {
        /* do nothing */
    }

    return ret;
}

void GenerateAndStoreLeaveNode(uint8_t fileNo)
{
    /* store the file output hash */
    unsigned char output_hash[SHA256_DIGEST_LENGTH];
    /* store the filename */
    char filename[FILE_NAME_MAX_LENGTH];
    /* Allocate memory for a new node */
    leave_nodes[fileNo] = malloc(sizeof(struct node_t));
    /* Check malloc execution */
    if (!leave_nodes[fileNo])
    {
        perror("malloc failed for a leaf node");
        exit(EXIT_FAILURE);
    }
    /* create the filename */
    else if (GenerateFilename(filename, fileNo) < 0)
    {
        perror("GenerateFilename in GenerateAndStoreLeaveNode failed");
        exit(EXIT_FAILURE);
    }
    /* Hash the file */
    else if (!HashFile(output_hash, filename))
    {
        perror("HashFile in GenerateAndStoreLeaveNode failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* if the hash have been successfully generated 
        store it in the leave_nodes */
        memcpy(leave_nodes[fileNo]->hash, output_hash, SHA256_DIGEST_LENGTH);
        //memcpy(leave_nodes[fileNo]->number, fileNo, sizeof(uint8_t));
        leave_nodes[fileNo]->parent = NULL;
        leave_nodes[fileNo]->rchild = NULL;
        leave_nodes[fileNo]->lchild = NULL;
        leave_nodes[fileNo]->number = fileNo;
    }
}

struct node_t ** BuildMerkleTree(void)
{
    /* count the total amount of files */
    uint8_t n_files = CountTransactionFiles();
    /* allocate space in memory for the leave_nodes
    array of struct node_t pointers */
    leave_nodes = malloc(n_files * sizeof(struct node_t *));
    if (leave_nodes == NULL)
    {
        perror("malloc failed for leave_nodes");
        exit(EXIT_FAILURE);
    }
    for (uint8_t fileNo = 0; fileNo < n_files; fileNo++)
    {
        /* generate and store the leave node */
        GenerateAndStoreLeaveNode(fileNo);
    }

    printf("\n\n\nMerkle Tree Successfully Built! \n\n\n");
    PrintLeavesHashes(n_files);
    return leave_nodes;
}

int8_t GenerateFilename(char *ret, uint8_t fileNo)
{
    return snprintf(ret, FILE_NAME_MAX_LENGTH, "%sblock_%u.txt", TRANSACTIONS_FOLDER, fileNo);
}

uint8_t CountTransactionFiles()
{
    uint8_t count = 0;
    DIR *dir = opendir(TRANSACTIONS_FOLDER);
    if (!dir) {
        perror("opendir");
        return 0;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file and matches "block_" prefix.
        if (entry->d_type == DT_REG && strstr(entry->d_name, "block_") != NULL) {
            count++;
        }
    }
    closedir(dir);
    return count;
}

void PrintNode(struct node_t* node)
{
    printf("Node N: %d\n", node->number);
    printf("Hash:");
    printf("parent: %p", node->parent);
}

void PrintLeavesHashes(uint8_t n_files)
{
    for (uint8_t i = 0; i < n_files; i++)
    {
        PrintNode(leave_nodes[i]);
        PrintHash(leave_nodes[i]->hash);
    }
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