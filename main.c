#include "inc/hash.h"
#include "stdio.h"

/* PRIVATE FUNCTIONS DECLARATIONS */

/* brief
 * displays the user menu to
 * manage the merkle tree.
 * params:
 * [OUT] float true to keep displaying
*/
float DisplayMenu(void);

/* brief
 * initializes the merkle tree
 * params: 
 * NONE 
*/
void GenerateMerkleTree(void);

int main()
{
	GenerateMerkleTree();
	while(DisplayMenu()){};	
	return 0;
}

/* PRIVATE FUNCTIONS DEFINITIONS */
float DisplayMenu()
{
}

void GenerateMerkleTree()
