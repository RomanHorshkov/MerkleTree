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

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/

/* Define file reading buffer size */
#define BUFFER_SIZE 4096

/* Define transaction data folder */
#define FILE_NAME_MAX_LENGTH 50
#define TRANSACTIONS_FOLDER "data/transactions/"

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

void FreeAllNodes(int tree_levels);


/**
 * @brief Establishes the relationships between nodes in the Merkle tree.
 *
 * This function sets up the parent-child connections among nodes in the Merkle tree.
 * It assumes that the nodes are stored in a matrix where each row represents a level of the tree.
 * The parent of each node is determined by the node's position, and the corresponding child pointers
 * (left and right) are set accordingly.
 */
void SetRelations();


/**
 * @brief Computes the cryptographic hash for each node in the Merkle tree.
 *
 * This function iterates through the Merkle tree and computes the hash for every node,
 * typically by combining the hashes of its child nodes. The resulting hash is stored in the
 * node's hash field. This process is essential for ensuring the integrity and security of the tree.
 *
 * @param nodes_ptr A pointer to the two-dimensional array (matrix) of node_t pointers.
 */
void HashNodes();

void HashLeaves();

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
DIR *transactions_dir = NULL;
int n_files = 0;

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void BuildMerkleTree()
{

    /* count the total number of files */
    n_files = CountFilesInDirectory(TRANSACTIONS_FOLDER);
    printf("\nN FILES: %d in folder %s\n", n_files, TRANSACTIONS_FOLDER);

    /* prepare the tree:
    int array with number of nodes for each level */
    int *nodes_number_arr = NULL;
    int tree_levels = NodesNumberArrayFromFile(&nodes_number_arr, n_files);

    /* Allocate space for all the nodes */
    AllocateAllNodes(&nodes, nodes_number_arr, tree_levels);

    if(nodes)
    {
        /* Set nodes relations */
        SetRelations();
        /* Hash all the nodes */
        HashNodes(TRANSACTIONS_FOLDER);
        PrintMerkleTree(nodes);

        FreeAllNodes(tree_levels);
    }


    /* close the directory */
    closedir(transactions_dir);
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
    
                printf("\nAnalysed node:\n");
                PrintNode(*col);
    
                /* SET PARENT */
                /* check if anything above exists, using the
                null terminated array */
                if (row[1])
                {
                    /* the index is this distance / 2 
                    0/2 = 0, 1/2 = 0, 2/2 = 1, 3/2 = 1 etc etc
                    &row[+1][dist_r/2] -> take upper column, that node*/
                    PrintNode(row[1][dist_r/2]);
                    (*col)->parent = row[1][dist_r/2];
                    printf("Set as parent: \n");
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
                        printf("Set as lchild: \n");
                        PrintNode(row[-1][dist_r*2]);
                        (*col)->lchild = row[-1][dist_r*2];
                        /* The right child could NOT EXIST */
                        if (row[-1][dist_r*2 + 1])
                        {
                            printf("Set as rchild: \n");
                            PrintNode(row[-1][dist_r*2 + 1]);
                            (*col)->rchild = row[-1][dist_r*2 + 1];
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

    if (row[0])
    {
        /* continue hashing the other nodes */
        row++;
        while (*row)
        {
            col = row[0];
            while (*col != NULL)
            {
                printf("HashNodes :parent before hash:\n");
                PrintNode(*col);
                HashNodeFromChildren(col);
                col++;
            }
            row++;
        }
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
                     "%sblock_%d.txt", TRANSACTIONS_FOLDER,
                      file_counter);

            /* Check if file exists */
            if (isValidFile(filename))
            {
                printf("HashNodes: Node before HashFile():\n");
                PrintNode(*col);
                HashFile(filename, (*col)->hash);
                printf("HashNodes: Node after HashFile():\n");
                PrintNode(*col);
                /* update the node counter */
                file_counter++;
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
                fprintf(stderr, "HashNodes: readdir() failure \n");
            }
        }
        printf("HashNodes:  TREE \n");
        PrintMerkleTree(nodes);
    }
    else
    {
        fprintf(stderr, "HashNodes: unable to open dir \n");
    }
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