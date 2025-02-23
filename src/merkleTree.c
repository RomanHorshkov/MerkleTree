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
#include "../inc/node.h"
#include "../inc/utils.h"

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
/* Tree nodes definition:
 * a ptr to a matrix of ptrs */
 struct node_t ***nodes = NULL;

/* Root Node definition: 
 * ptr to the root node, head */
struct node_t *root_node = NULL;

char *BASE_FOLDER = NULL;

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
 * @brief Frees allocated memory for all nodes in the Merkle tree.
 * 
 * This function releases the dynamically allocated memory used for the nodes
 * in the Merkle tree, ensuring no memory leaks occur.
 * 
 * @param tree_levels Total number of levels in the Merkle tree.
 */
void FreeAllNodes(int tree_levels);

/**
 * @brief Establishes the relationships between nodes in the Merkle tree.
 *
 * This function sets up the parent-child connections among nodes in the Merkle tree.
 * It assumes that the nodes are stored in a matrix where each row represents a level of the tree.
 * The parent of each node is determined by the node's position, and the corresponding child pointers
 * (left and right) are set accordingly.
 */
void SetRelations(void);

/**
 * @brief Computes the cryptographic hash for each node in the Merkle tree.
 *
 * This function iterates through the Merkle tree and computes the hash for every node,
 * typically by combining the hashes of its child nodes. The resulting hash is stored in the
 * node's hash field. This process is essential for ensuring the integrity and security of the tree.
 */
void HashNodes(void);

/**
 * @brief Hashes only the leaf nodes.
 *
 * This function reads transaction files and computes SHA-256 hashes
 * for the leaf nodes, which serve as the base of the Merkle tree.
 */
void HashLeaves(void);

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
/* define folder/file information */
int n_files = 0;

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void BuildMerkleTree(const char *transactions_folder)
{
    BASE_FOLDER = (char *)transactions_folder;
    /* prepare the tree:
    int array with number of nodes for each level */
    int *nodes_number_arr = NULL;
    int tree_levels = NodesNumberArrayFromFile(&nodes_number_arr, BASE_FOLDER);

    /* Allocate space for all the nodes */
    AllocateAllNodes(&nodes, nodes_number_arr, tree_levels);

    if(nodes)
    {
        /* Set nodes relations */
        SetRelations();
        /* Hash all the nodes */
        HashNodes();

        printf("tree_levels: %d\n", tree_levels);
        printf("Root hash hex: \n");
        PrintHashHex(root_node->hash);

        /* Free the tree */
        FreeAllNodes(tree_levels);
    }

}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
void SetRelations()
{
    struct node_t ***row; // point to array of ptrs node_t*
    struct node_t **col; // point to ptr node_t*

    
    if (*nodes)
    {
        row = nodes;
        /* get advantage of the null terminated array */
        while (*row != NULL)
        {
            /* pointer to ptr node_t* in the row array */
            col = row[0];
            /* get advantage of the null terminated array */
            while (*col != NULL)
            {
                /* calculate the distance on row */
                int dist_r = col - row[0];
    
                // printf("\nAnalysed node:\n");
                // PrintNode(*col);
    
                /* SET PARENT */
                /* check if anything above exists, using the
                null terminated array */
                if (row[1])
                {
                    /* the index is this distance / 2 
                    0/2 = 0, 1/2 = 0, 2/2 = 1, 3/2 = 1 etc etc
                    &row[+1][dist_r/2] -> take upper column, that node*/
                    (*col)->parent = row[1][dist_r/2];
                }
                else
                {
                    /* if no parent exists, this is the root node */
                    root_node = (*col);
                }
    
                /* SET CHILDREN */
                /* if leaves */
                if (row == nodes)
                {
                    (*col)->lchild = NULL;
                    (*col)->rchild = NULL;
                }
                else
                {
                    /* the index is this distance / 2 
                    0*2 = 0, 1*2 = 2, 2*2 = 4, 3*2 = 6 etc etc
                    &row[-1][dist_r/2] -> take upper column, that node*/

                    /* left and right children could NOT EXISTS 
                    for the nodes added with copied hash to complete row */
                    if (row[-1][dist_r*2])
                    {
                        (*col)->lchild = row[-1][dist_r*2];

                        /* The right child could NOT EXIST */
                        if (row[-1][dist_r*2 + 1])
                        {
                            (*col)->rchild = row[-1][dist_r*2 + 1];
                        }
                    }
                }
                col++;
            }
            row++;
        }
    }
}

void HashNodes()
{

    struct node_t ***row = nodes;
    struct node_t **col;

    /* Hash first the leaves */
    HashLeaves();

    /* continue hashing nodes at levels 1+ */
    row++;
    while (*row)
    {
        col = row[0];
        /* check if node is valid*/
        while (*col != NULL)
        {
            /* If the hashing from childre succeeds go to next node */
            if(HashNodeFromChildren(col))
            {
                col++;
            }
            else
            {
                fprintf(stderr, "HashNodes: nodes not allocated \n");
                break;
            }
        }
        row++;
    }
}

void HashLeaves(void)
{
    struct node_t ***row = nodes;
    struct node_t **col;

    int file_counter = 0;
    char filename[256];

    /* Check inputs */
    if (row[0])
    {
        col = row[0];
        while (col)
        {
            /* Generate filename */
            snprintf(filename, sizeof(filename),
                     "%sblock_%d.txt", BASE_FOLDER,
                      file_counter);

            /* Check if file exists */
            if (isValidFile(filename))
            {
                HashFile(filename, (*col)->hash);
                /* update the node counter */
                file_counter++;
                /* update the node */
                col++;
            }
            /* Check if last node */
            else if (!col[1])
            {
                /* Set the same hash of previous node */
                memcpy(col[0]->hash, col[-1]->hash, sizeof(col[-1]->hash));
                break;
            }
            else
            {
                fprintf(stderr, "HashLeaves: file not valid, not last node \n");
                break;
            }
        }
    }
    else
    {
        fprintf(stderr, "HashLeaves: nodes not allocated \n");
    }
}

void AllocateAllNodes(struct node_t ****nodes_ptr, int *nodes_arr, int tree_levels)
{
    /* Allocate a helper array of pointers (block) for each level */
    struct node_t *helper_arr = NULL;
    /* Node number counter, for debugging purpose */
    int node_number_idx = 0;
    /* Allocate memory for the top-level pointer array with +1 NULL node */
    *nodes_ptr = (struct node_t ***)malloc((tree_levels + 1) * sizeof(struct node_t **));

    if (*nodes_ptr)
    {
        /* clean the memory */
        memset(*nodes_ptr, 0, (tree_levels + 1) * sizeof(struct node_t **));
        /* Set last ptr to arr to null */
        (*nodes_ptr)[tree_levels] = NULL;
        for (int i = 0; i < tree_levels; i++)
        {
            /* Allocate memory for each level with +1 NULL node */
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
                    
                    /* Assign pointers to nodes */
                    for (int j = 0; j < nodes_arr[i]; j++)
                    {
                        /* set the node's number */
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

void FreeAllNodes(int tree_levels)
{
    if (nodes)
    {
        for (int i = 0; i < tree_levels; i++)
        {
            if (nodes[i])
            {
                /* Free the struct node_t array */
                free(nodes[i][0]); // helper_arr was allocated as a single block
                /* Free the array of pointers */
                free(nodes[i]);
            }
        }
    }
    /* Free the top-level pointer array */
    free(nodes);
    nodes = NULL;
}