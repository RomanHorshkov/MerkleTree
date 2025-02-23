/**
 * @file utils.c
 * @brief Implementation of hashing utilities
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/

 #include "../inc/utils.h"

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
#define BUFFER_SIZE_FILE_READ 4096      /* File reading buffer size (4KB) */

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
/* None */

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
bool HashFile(const char *filename, unsigned char output[SHA256_DIGEST_LENGTH])
{
    bool ret = false;  /* Return status, initialized to false */

    /* Open file in binary mode for reading */
    FILE *file = fopen(filename, "rb");
    if (file)
    {
        /* Create a hashing context for EVP (Message Digest) */
        EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
        if (mdctx)
        {
            /* Get SHA-256 digest method */
            const EVP_MD *md = EVP_sha256();
            if (md)
            {
                /* Initialize SHA-256 hashing process */
                EVP_DigestInit_ex(mdctx, md, NULL);

                /* Declare a buffer to read the file in chunks */
                unsigned char buffer[BUFFER_SIZE_FILE_READ];
                size_t bytesRead;

                /* Read file in chunks and feed data to the hashing function */
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
                {
                    EVP_DigestUpdate(mdctx, buffer, bytesRead);
                }

                /* Finalize the hashing process and store result in 'output' */
                EVP_DigestFinal_ex(mdctx, output, NULL);

                /* Free the hashing context */
                EVP_MD_CTX_free(mdctx);

                /* Indicate success */
                ret = true;
            }
        }
        /* Close the file before returning */
        fclose(file);
    }
    else
    {
        perror("HashFile: Unable to open file");
        printf("file failed: %s\n", filename);
    }


    return ret; /* Return whether the hashing was successful */
}

bool HashTwoHashes(const unsigned char hashA[SHA256_DIGEST_LENGTH], 
                   const unsigned char hashB[SHA256_DIGEST_LENGTH], 
                   unsigned char output[SHA256_DIGEST_LENGTH])
{
    bool ret = false;

    /* Buffer to hold concatenated hashes */
    unsigned char combined[2 * SHA256_DIGEST_LENGTH];
    /* Copy first and second hash into buffer */
    memcpy(combined, hashA, SHA256_DIGEST_LENGTH);
    memcpy(combined + SHA256_DIGEST_LENGTH, hashB, SHA256_DIGEST_LENGTH);
    /* Create a new message digest context */
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx)
    {
        unsigned int output_length = 0;  // Store actual hash length

        if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) &&
            EVP_DigestUpdate(mdctx, combined, sizeof(combined)) &&
            EVP_DigestFinal_ex(mdctx, output, &output_length))
        {
            /* Ensure correct hash length */
            ret = (output_length == SHA256_DIGEST_LENGTH);
        }

        /* Free allocated memory */
        EVP_MD_CTX_free(mdctx);
    }
    return ret;
}

bool HashNodeFromChildren(struct node_t** node)
{
    bool ret = false;

    /* Validate input parameters */
    if (!node) 
    {
        fprintf(stderr, "HashNodeFromChildren: node == NULL ");
    }
    /* If the left node is null copy prev hash*/
    else if ((*node)->lchild == NULL)
    {
        /* copy the left brother's hash */
        memcpy((*node)->hash, (*node)->parent->lchild->hash, SHA256_DIGEST_LENGTH);
    }
    /* If ONLY right node is null something has gone
    terribly wrong */
    else if ((*node)->rchild == NULL)
    {
        fprintf(stderr, "HashNodeFromChildren: Rchild == NULL ");
    }
    else if (HashTwoHashes((*node)->lchild->hash,
                           (*node)->rchild->hash,
                           (*node)->hash))
    {
        ret = true;
    }

    return ret;
}

int CountFilesInDirectory(const char *file_name)
{
    /* open the directory */
    DIR *dir = opendir(file_name);

    /* files counter */
    int count = 0;
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            // Skip "." and ".." entries.
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            // If d_type is available and indicates a regular file.
            if (entry->d_type == DT_REG)
            {
                count++;
            }
        }
    }

    return count;
}

int NodesNumberArrayFromFile(int **nodes_number_arr, const char *folder)
{
    int n_files = CountFilesInDirectory(folder);
    printf("\nN FILES: %d in folder %s\n", n_files, folder);
    int ret = 0;
    int n_row = 0;

    if (n_files >= 1)
    {
        int n_nodes = n_files;

        /* while the number of nodes to process is > 1*/
        while (n_nodes > 1)
        {
            n_row++;
            n_nodes = (n_nodes + 1) / 2;  // if odd, round up before halving
            // printf("n_row = %d, n_nodes = %d\n", n_row, n_nodes);
        }
        /* account for the root node */
        n_row++;
    
        /* allocate the array of ints */
        *nodes_number_arr = malloc(n_row * sizeof(int));
        /* check if malloc succeeded */
        if (*nodes_number_arr)
        {
            /* set return to total number of rows */
            ret = n_row;
            /* restore the nodes counter */
            n_nodes = n_files;
            /* put in the nodes number for each row */
            for (int i = 0; i < n_row; i++)
            {
                
                /* double the nodes number if odd */
                if(n_nodes & 1 && n_nodes != 1)
                {
                    n_nodes++;
                }
                /* assign n_nodes to the array */
                (*nodes_number_arr)[i] = n_nodes;
                /* halve the nodes counter */
                n_nodes /= 2;
            }
        }
    }
    
    return ret;
}

bool isValidFile(const char *filename)
{
    bool ret = false;
    struct stat buffer;

    if (stat(filename, &buffer) == 0)
    {
        ret = true;
    }
    else
    {
        printf("File does not exist: %s\n", filename);
    }

    return ret;
}

/* ######################################################################
 * PRINT FUNCTIONS 
###################################################################### */

void PrintMerkleTree(struct node_t ***nodes)
{
    if (nodes == NULL)
    {
        printf("Merkle tree is empty.\n");
        return;
    }

    int level = 0;
    while (nodes[level] != NULL)
    {
        // Count nodes at this level.
        int count = 0;
        while (nodes[level][count] != NULL)
        {
            count++;
        }
        
        // Print level header.
        printf("Level %d (%d node%s):\n", level, count, count == 1 ? "" : "s");

        // Print each node in the current level.
        for (int i = 0; i < count; i++)
        {
            struct node_t *node = nodes[level][i];
            printf("  Node[%d] at %p:\n", i, (void*)node);
            printf("    Number     : %d\n", node->number);
            printf("    Hash       : ");
            PrintHashHex(node->hash);
            printf("\n");
            if (node->parent)
                printf("    Parent     : %p\n", (void*)node->parent);
            if (node->lchild)
                printf("    Left Child : %p\n", (void*)node->lchild);
            if (node->rchild)
                printf("    Right Child: %p\n", (void*)node->rchild);
        }
        printf("\n");
        level++;
    }
}

void PrintHashHex(const unsigned char hash[SHA256_DIGEST_LENGTH])
{
    /* Iterate through hash bytes */
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        /* Print each byte in two-digit hex format */
        printf("%02x", hash[i]);
    }
    /* New line for readability */
    printf("\n");
}

void PrintHashBinary(const unsigned char hash[SHA256_DIGEST_LENGTH])
{
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {  // Iterate through hash bytes
        for (int j = 7; j >= 0; j--) {  // Iterate over each bit (MSB to LSB)
            printf("%d", (hash[i] >> j) & 1);  // Print each bit
        }
        printf(" ");  // Space between bytes for readability
    }
    printf("\n");  // New line for readability
}

void PrintNode(struct node_t *node)
{
    if (node)
    {
        printf("Node %p: number = %d, hash = ", node, node->number);
        PrintHashHex(node->hash);
        printf("rchild = %p, lchild = %p, parent = %p\n",
                node->rchild, node->lchild, node->parent);
    }
}

void pIntArr(int *arr, int size)
{
    printf("IN: int *arr: %p, size: %d\n", arr, size);
    
    // Create a pointer for iteration over the array.
    int *idx = arr;
    
    // Print the size of the pointer variable 'arr'. This is the size of a pointer, not the array.
    printf("Size of arr: %ld, Size of arr/int*: %ld\n", sizeof(arr), (sizeof(arr) / sizeof(int*)));
    // The size of 'idx' is the same as 'arr' since both are pointers.
    printf("Size of idx: %ld, Size of idx/int*: %ld\n", sizeof(idx), (sizeof(idx) / sizeof(int*)));
    
    printf("# \tPointer \tvalue idx++\n");
    // Iterate over the array elements using pointer arithmetic.
    do
    {
        // 'idx - arr' gives the current index.
        // 'idx' is the current pointer, and '*idx' is the integer value at that pointer.
        printf("%ld \t%p\t%d\n", idx - arr, idx, *idx);
        idx++;        
    } while (idx - arr < size);
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
/* None */
