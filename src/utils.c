/**
 * @file utils.c
 * @brief Implementation of hashing utilities
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/utils.h"
#include <openssl/evp.h>
#include <string.h>
#include <dirent.h>

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
    }

    /* Close the file before returning */
    fclose(file);

    return ret; /* Return whether the hashing was successful */
}

bool HashTwoHashes(const unsigned char hashA[SHA256_DIGEST_LENGTH], 
                   const unsigned char hashB[SHA256_DIGEST_LENGTH], 
                   unsigned char output[SHA256_DIGEST_LENGTH])
{
    bool ret = false;

    unsigned char combined[2 * SHA256_DIGEST_LENGTH];  // Buffer to hold concatenated hashes
    memcpy(combined, hashA, SHA256_DIGEST_LENGTH);     // Copy first hash into buffer
    memcpy(combined + SHA256_DIGEST_LENGTH, hashB, SHA256_DIGEST_LENGTH); // Copy second hash

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();  // Create a new message digest context
    if (mdctx)
    {
        EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);           // Initialize SHA-256 hashing
        EVP_DigestUpdate(mdctx, combined, sizeof(combined));    // Update hash with concatenated data
        EVP_DigestFinal_ex(mdctx, output, NULL);                // Finalize and store result in output
        EVP_MD_CTX_free(mdctx);                                 // Free allocated memory for digest context
        ret = true;
    }
    return ret;
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

int CountFilesInFolder(char *folder_path)
{
    /* files counter */
    int count = 0;
    /* open the directory */
    DIR *dir = opendir(folder_path);
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

    closedir(dir);
    return count;
}

int NodesNumberArrayFromFile(int **nodes_number_arr, int n_files)
{
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
            printf("n_row = %d, n_nodes = %d\n", n_row, n_nodes);
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
