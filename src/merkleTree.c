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


void SetRelations(struct node_t ***nodes_ptr);

void HashNodes(struct node_t ***nodes_ptr);

void PrintMerkleTree(struct node_t ***nodes);

void PrintNode(struct node_t *node);
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
        printf("set relations \n");
        SetRelations(nodes);
        /* Hash all the nodes */
        printf("HashNodes \n");
        HashNodes(nodes);
    }


}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
void SetRelations(struct node_t ***nodes_ptr)
{
    bool no_children = true;
    struct node_t ***row; // point to array of ptrs node_t*
    struct node_t **col; // point to ptr node_t*

    
    if (*nodes_ptr)
    {
        row = nodes_ptr;
        /* get advantage of the null terminated array */
        while (*row != NULL)
        {
            col = row[0]; // pointer to ptr node_t* in the row array
            /* get advantage of the null terminated array */
            while (*col != NULL)
            {
                /* calculate the distance on row */
                int dist_r = col - row[0];
    
                printf("\nAnalysed node:\n");
                PrintNode(*col);
    
                /* set parent */
                /* check if anything above exists, using the
                null terminated array */
                if (row[1])
                {
                    /* the index is this distance / 2 
                    0/2 = 0, 1/2 = 0, 2/2 = 1, 3/2 = 1 etc etc
                    &row[+1][dist_r/2] -> take upper column, that node*/
                    printf("Set as parent: \n");
                    PrintNode(row[1][dist_r/2]);
                    (*col)->parent = row[1][dist_r/2];
                }
                /* set children */
    
                /* if leaves */
                if (no_children)
                {
                    (*col)->lchild = NULL;
                    (*col)->rchild = NULL;
                }
                else
                {
                    /* the index is this distance / 2 
                    0*2 = 0, 1*2 = 2, 2*2 = 4, 3*2 = 6 etc etc
                    &row[-1][dist_r/2] -> take upper column, that node*/
                    /* The left child could NOT EXIST */
                    if (row[-1][dist_r*2])
                    {
                        (*col)->lchild = row[-1][dist_r*2];
                        printf("Set as lchild: \n");
                        PrintNode(row[-1][dist_r*2]);
                        /* The right child could NOT EXIST */
                        if (row[-1][dist_r*2 + 1])
                        {
                            (*col)->rchild = row[-1][dist_r*2 + 1];
                            printf("Set as rchild: \n");
                            PrintNode(row[-1][dist_r*2 + 1]);
                        }
                        else
                        {
                            printf("NO RCHILDREN SET! \n");
                        }
                    }
                    else
                    {
                        printf("NO LCHILDREN SET! \n");
                        printf("NO RCHILDREN SET! \n");
                    }
                }

                col++;
            }

            no_children = false;
            row++;
        }
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
            printf("Node %p %d: n = %d, hash = ", nodes[level][i], i, nodes[level][i]->number);
            PrintHashHex(nodes[level][i]->hash);
        }
        printf("\n");
        level++;
    }
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