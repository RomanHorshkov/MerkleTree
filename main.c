/**
 * @file main.c
 * @author Roman Horshkov
 * @date 12 Feb 2025
 * @brief merkle tree main menu
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "inc/merkleTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE MACROS
 *-----------------------------------*/
#define MAIN_MENU_ROWS_N 4
#define TRANSACTIONS_FOLDER "data/transactions/"

/*-----------------------------------*
 * PRIVATE TYPEDEFS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PRIVATE FUNCTION PROTOTYPES
 *-----------------------------------*/

/**
 * @brief Entry point for the Merkle Tree menu application.
 *
 * Initializes and displays the main menu, then waits
 * for user interaction to build or check a Merkle Tree.
 *
 * @return 0 on successful exit.
 */
int main();

/**
 * @brief displays the user menu to
 * manage the merkle tree.
 * @retval float true to keep displaying
*/
bool DisplayMenu(void);

/**
 * @brief initializes the merkle tree
*/
void GenerateMerkleTree(void);

/**
 * @brief clears the screen
*/
void ClearScreen(void);
/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
char *mainMenu[MAIN_MENU_ROWS_N] = 
{
	"1 check if root hash exists",
	"2 generate and compare root hashes",
	"x regenerate root hash",
	"q exit"
};
/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
/* None */
/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
int main()
{
    while(DisplayMenu())
    {

    }
	return 0;
}

bool DisplayMenu()
{
	bool ret = true;
    char cmd; /* user command */

    ClearScreen();  /* Clear screen before displaying the menu */

    printf("===== Merkle Tree Menu =====\n");
    for (int i = 0; i < MAIN_MENU_ROWS_N; i++)
    {
        printf("%s\n", mainMenu[i]);
    }
    printf("Select an option: ");
    scanf("%c", &cmd);
	getchar();
	printf("\n");

    switch (cmd)
    {
        case '1':
            printf("Checking if root hash exists...\n");
            break;
        case '2':
            printf("Generating and comparing root hashes...\n");
            break;
        case 'x':
            printf("Regenerating root hash...\n");
			GenerateMerkleTree();
            break;
        case 'q':
            printf("Exiting...\n");
            ret = false;  // Exit the loop
            break;
        default:
            printf("Invalid cmd. Please try again.\n");
    }

    if(cmd != 'q')
    {
        printf("\n press enter to continue:");
        getchar();
    }
    ClearScreen();  /* Clear screen before exiting */
    return ret;
}

void GenerateMerkleTree()
{
    printf("Initializing Merkle Tree...\n");
    BuildMerkleTree(TRANSACTIONS_FOLDER);
}

void ClearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
