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
struct node_t ***nodes = NULL;                           /* Tree nodes definition */

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
uint8_t AllocateSpace(void);
void BuildTree(void);
void GenerateLeaveNodes(void);
void GenerateAllOtherNodes(void);
/**
 * @brief Hash a file
*/
bool HashFile(unsigned char hash[SHA256_DIGEST_LENGTH], const char *filename);

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
 * @brief prints a hash
*/
void PrintHash(const unsigned char hash[SHA256_DIGEST_LENGTH]);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void BuildMerkleTree(void)
{
    AllocateSpace();
    BuildTree();
    printf("\nMerkle Tree Successfully Built!\n");
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/

uint8_t AllocateSpace(void)
{
    /* count the total amount of files */
    uint8_t n_nodes = CountTransactionFiles();
    /* reteurn total files */
    uint8_t ret = n_nodes;
    /* count the number of rows */
    uint8_t row = 0;


    while (n_nodes >= 1)
    {
        /* check if nodes odd */
        if(n_nodes & 1)
        {
            /* incremen nodes counter */
            printf("Number of node incremented from: %d to", n_nodes);
            n_nodes++;
            printf("%d\n", n_nodes);
        }
        printf("Number of row: %d\n", row);
        printf("Number of nodes: %d\n", n_nodes);

        /* struct node_t ***nodes, nodes[i] is struct node_t ** */
        /* Allocate space for an array of pointers to
        struct node_t of size n_nodes at the row */
        nodes[row] = malloc(n_nodes * sizeof(struct node_t *));
        if(nodes[row])
        {
            /* Allocate space for each node in the row */
            for (uint8_t i = 0; i < n_nodes; i++)
            {
                nodes[row][i] = malloc(sizeof(struct node_t));
                /* check node validity */
                if (!nodes[row][i])
                {
                    perror("malloc failed for node allocation");
                    exit(EXIT_FAILURE);
                }
            }
            /* increment rows counter */
            row++;
            /* half the n_nodes counter */
            n_nodes /= 2;
        }
        else
        {
            perror("malloc failed for row allocation");
            exit(EXIT_FAILURE);
        }
    }
    return ret;
}

void BuildTree(void)
{

    GenerateLeaveNodes();
    GenerateAllOtherNodes();
}

void GenerateLeaveNodes(void)
{
    /* store the filename */
    char filename[FILE_NAME_MAX_LENGTH];

    /* store the file output hash */
    unsigned char output_hash[SHA256_DIGEST_LENGTH];

    uint8_t tot_files = CountTransactionFiles();

    for (size_t fileNo = 0; fileNo < (size_t)tot_files; fileNo++)
    {
        /* create the filename */
        if (GenerateFilename(filename, fileNo) < 0)
        {
            perror("GenerateLeaveNodes(): GenerateFilename failed");
            exit(EXIT_FAILURE);
        }
        /* Hash the file */
        else if (!HashFile(output_hash, filename))
        {
            perror("GenerateLeaveNodes(): HashFile failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* if the hash have been successfully generated 
            store it in the nodes */
            memcpy(nodes[0][fileNo]->hash, output_hash, SHA256_DIGEST_LENGTH);
            nodes[0][fileNo]->parent = NULL;
            nodes[0][fileNo]->rchild = NULL;
            nodes[0][fileNo]->lchild = NULL;
            nodes[0][fileNo]->number = fileNo;
        }
    }
    /* Check if nodes odd */
    if (tot_files & 1)
    {
        /* copy the node to the end */
        printf("GenerateLeaveNodes(): Adding extra node to the end\n");
        memcpy(nodes[0][tot_files], nodes[0][tot_files - 1], sizeof(struct node_t));
    }
    
}

void GenerateAllOtherNodes(void)
{
    /* 
    Start with row 0
    
    Start with nodes i = 0 and j = i + 1;
    */

    /* Save starting index */
    struct node_t ***start_idx = nodes;

    /* *nodes points to the first row of nodes_t * */
    /* ** nodes points to the first element of first row */
    while (!**nodes)
    {
        PrintNode(**nodes++);
    }
    
    /* restore nodes index */
    nodes = start_idx;
    
}

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
    PrintHash(node->hash);
    printf("parent: %p\n", node->parent);
    printf("lchild: %p\n", node->rchild);
    printf("rchild: %p\n", node->lchild);
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