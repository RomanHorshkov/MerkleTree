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
#include "../inc/utils.h"

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
/* Tree nodes definition:
 * a ptr to a matrix of ptrs */
 struct node_t ***nodes = NULL;

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
/* None */

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
void AllocateAllNodes(struct node_t ****nodes_ptr,
                    int *nodes_arr, int tree_levels);

                    
void PrintMerkleTree(struct node_t ***nodes);

void SetRelations(struct node_t ***nodes_ptr);

void HashNodes(struct node_t ***nodes_ptr);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void BuildMerkleTree(void)
{
    /* count the total number of files */
    int n_files = CountFilesInFolder(TRANSACTIONS_FOLDER);
    printf("\nN FILES: %d in folder" TRANSACTIONS_FOLDER "\n", n_files);

    /* prepare the tree:
       set an int array each with number of nodes in levels */
    int *nodes_number_arr = NULL;
    int tree_levels = NodesNumberArrayFromFile(&nodes_number_arr, n_files);
    pIntArr(nodes_number_arr, tree_levels);

    /* Allocate space for all the nodes */
    AllocateAllNodes(&nodes, nodes_number_arr, tree_levels);
    PrintMerkleTree(nodes);

    if(nodes)
    {
        /* Set nodes relations */
        SetRelations(nodes);
        /* Hash all the nodes */
        HashNodes(nodes);
    }


}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
void SetRelations(struct node_t ***nodes_ptr)
{
    struct node_t **parents = nodes_ptr[1];
    struct node_t *idx = (*nodes_ptr)[0];
    int dist = idx - (*nodes_ptr)[0];
    while (idx)
    {
        idx->lchild = NULL;
        idx->rchild = NULL;
        idx->parent = parents[dist/2];
        dist = idx - (*nodes_ptr)[0];
    }
    
}
void HashNodes(struct node_t ***nodes_ptr)
{   

}

void AllocateAllNodes(struct node_t ****nodes_ptr, int *nodes_arr, int tree_levels)
{
    /* helper block to allocate struct node_t */
    struct node_t *helper_arr = NULL;
    int node_number_idx = 0;
    /* allocate the array of pointers to arrays of pointers */
    *nodes_ptr = (struct node_t ***)malloc((tree_levels + 1) * sizeof(struct node_t **));
    if (*nodes_ptr)
    {
        /* clean the memory */
        memset(*nodes_ptr, 0, (tree_levels + 1) * sizeof(struct node_t **));
        /* Set last ptr to arr to null */
        (*nodes_ptr)[tree_levels] = NULL;
        for (int i = 0; i < tree_levels; i++)
        {
            /* allocate a block of ptrs for these nodes */
            (*nodes_ptr)[i] = (struct node_t**)malloc((nodes_arr[i] + 1) * sizeof(struct node_t*));
            
            /* check if malloc succeeded */
            if((*nodes_ptr)[i])
            {
                /* Set last ptr to null */
                (*nodes_ptr)[i][nodes_arr[i]] = NULL;

                /* allocate the helper array with the correct amount */
                helper_arr = malloc(nodes_arr[i] * sizeof(struct node_t));
                /* check if malloc succeeded */
                if (helper_arr)
                {
                    /* clean the memory */
                    memset(helper_arr, 0, nodes_arr[i] * sizeof(struct node_t));
                    /* assign the memory location to nodes */
                    for (int j = 0; j < nodes_arr[i]; j++)
                    {
                        /* set the arr content */
                        helper_arr[j].number = node_number_idx++;
                        /* connect the pointers */
                        (*nodes_ptr)[i][j] = &helper_arr[j];
                    }
                    helper_arr = NULL;
                }
                else
                {
                    free((*nodes_ptr)[i]);
                    free(*nodes_ptr);
                    *nodes_ptr = NULL;
                }
            }
            else
            {
                free(*nodes_ptr);
                *nodes_ptr = NULL;
            }
        }
    }
}

void PrintMerkleTree(struct node_t ***nodes)
{
    if (nodes == NULL)
    {
        printf("Merkle tree is empty.\n");
        return;
    }

    int level = 0;
    // Iterate over each level until a NULL pointer is found.
    while (nodes[level] != NULL)
    {
        // Determine how many nodes are in this level.
        int count = 0;
        while (nodes[level][count] != NULL)
        {
            count++;
        }
        
        // Print a header for the current level.
        printf("Level %d (%d node%s):\n", level, count, count == 1 ? "" : "s");

        // Print each node in this level.
        for (int i = 0; i < count; i++)
        {
            printf("  Node %d: number = %d, hash = ", i, nodes[level][i]->number);
            PrintHashHex(nodes[level][i]->hash);
        }
        printf("\n");
        level++;
    }
}
