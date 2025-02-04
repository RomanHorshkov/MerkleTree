/* Node structure for the merkle tree */

/* Future work:
 * Instead of using separate structs for leaves, internal nodes, and the root, you can define a generic node structure with pointers to children and a parent. This will simplify the tree traversal and memory management.
 */

#define HASH_SIZE 32 // hash size for sha 256

struct node_t {
	uint8_t hash[HASH_SIZE];
	node_t *parent;
	node_t *rchild;
	node_t *lchild;
}

