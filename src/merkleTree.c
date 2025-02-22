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
#include "../inc/tests.h"
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

/* Define file reading buffer size */
#define BUFFER_SIZE 4096

/* Define transaction data folder */
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
 * @brief Allocates memory for all nodes in the Merkle tree.
 *
 * This function creates a two-dimensional (matrix) array of pointers to node_t structures,
 * based on the provided number of nodes per level and the total number of levels in the tree.
 * The memory for each row (level) is allocated and initialized, and a NULL pointer is placed at
 * the end of each row for easy iteration.
 *
 * @param nodes_ptr Pointer to the pointer that will hold the allocated tree nodes matrix.
 * @param nodes_arr An array containing the number of nodes at each level.
 * @param tree_levels The total number of levels in the Merkle tree.
 */
void AllocateAllNodes(struct node_t ****nodes_ptr, int *nodes_arr, int tree_levels);


/**
 * @brief Establishes the relationships between nodes in the Merkle tree.
 *
 * This function sets up the parent-child connections among nodes in the Merkle tree.
 * It assumes that the nodes are stored in a matrix where each row represents a level of the tree.
 * The parent of each node is determined by the node's position, and the corresponding child pointers
 * (left and right) are set accordingly.
 *
 * @param nodes_ptr A pointer to the two-dimensional array (matrix) of node_t pointers.
 */
void SetRelations(struct node_t ***nodes_ptr);


/**
 * @brief Computes the cryptographic hash for each node in the Merkle tree.
 *
 * This function iterates through the Merkle tree and computes the hash for every node,
 * typically by combining the hashes of its child nodes. The resulting hash is stored in the
 * node's hash field. This process is essential for ensuring the integrity and security of the tree.
 *
 * @param nodes_ptr A pointer to the two-dimensional array (matrix) of node_t pointers.
 */
void HashNodes(struct node_t ***nodes_ptr);


/**
 * @brief Prints the entire Merkle tree structure.
 *
 * This function outputs the details of each node in the Merkle tree, organized by level.
 * For every node, it prints the node's number, hash, and pointers to its parent and children.
 * This is useful for debugging and verifying the tree's structure.
 *
 * @param nodes A pointer to the two-dimensional array (matrix) of node_t pointers representing the tree.
 */
void PrintMerkleTree(struct node_t ***nodes);


/**
 * @brief Prints the details of a single node.
 *
 * This function outputs the node's address, number, hash, and the addresses of its parent,
 * left child, and right child. It is intended to provide detailed information about a specific node,
 * which can be helpful during debugging.
 *
 * @param node The node to be printed.
 */
void PrintNode(struct node_t *node);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void BuildMerkleTree(const char *folder)
{
    /* count the total number of files */
    int n_files = CountFilesInFolder(folder);
    // printf("\nN FILES: %d in folder %s\n", n_files, folder);

    /* prepare the tree:
       set an int array each with number of nodes in levels */
    int *nodes_number_arr = NULL;
    int tree_levels = NodesNumberArrayFromFile(&nodes_number_arr, n_files);
    // pIntArr(nodes_number_arr, tree_levels);

    /* Allocate space for all the nodes */
    AllocateAllNodes(&nodes, nodes_number_arr, tree_levels);
    // PrintMerkleTree(nodes);

    if(nodes)
    {
        /* Set nodes relations */
        // printf("set relations \n");
        SetRelations(nodes);
        // PrintMerkleTree(nodes);
        /* Hash all the nodes */
        // printf("HashNodes \n");
        // HashNodes(nodes);
    }


}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
void SetRelations(struct node_t ***nodes_ptr)
{
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
    
                // printf("\nAnalysed node:\n");
                // PrintNode(*col);
    
                /* set parent */
                /* check if anything above exists, using the
                null terminated array */
                if (row[1])
                {
                    /* the index is this distance / 2 
                    0/2 = 0, 1/2 = 0, 2/2 = 1, 3/2 = 1 etc etc
                    &row[+1][dist_r/2] -> take upper column, that node*/
                    // printf("Set as parent: \n");
                    // PrintNode(row[1][dist_r/2]);
                    (*col)->parent = row[1][dist_r/2];
                }
                /* set children */
    
                /* if leaves */
                if (row == nodes_ptr)
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
                        // printf("Set as lchild: \n");
                        // PrintNode(row[-1][dist_r*2]);
                        /* The right child could NOT EXIST */
                        if (row[-1][dist_r*2 + 1])
                        {
                            (*col)->rchild = row[-1][dist_r*2 + 1];
                            // printf("Set as rchild: \n");
                            // PrintNode(row[-1][dist_r*2 + 1]);
                        }
                        else
                        {
                            // printf("NO RCHILDREN SET! \n");
                        }
                    }
                    else
                    {
                        // printf("NO LCHILDREN SET! \n");
                        // printf("NO RCHILDREN SET! \n");
                    }
                }
                col++;
            }
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