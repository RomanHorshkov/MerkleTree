/**
 * @file node.c
 * @author Roman Horshkov
 * @date 12 Feb 2025
 * @brief merkle node engine
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/node.h"
#include <stdio.h>
#include <dirent.h> // for counting files
#include <string.h>

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
/* None */
/*-----------------------------------*
 * PRIVATE TYPEDEFS
 *-----------------------------------*/
/* None */
/*-----------------------------------*
 * PRIVATE FUNCTION PROTOTYPES
 *-----------------------------------*/
/**
 * @brief function that reads the data
 * files and stores them in nodes
*/
void ReadFileData(char *filename);

/**
 * @brief function that reads the data
 * files and stores them in nodes
*/
void ReadFilesData(void);

/**
 * @brief function that reads the data
 * files and return their amount 
*/
int UpdateFilesNumber(void);

void GenerateNodesFromData(char *filename);
// void GenerateNodesFromHashes();

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
/* None */
/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void TryToHash(void)
{
	ReadFilesData();
}
/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/

void ReadFilesData()
{
	int files_number = UpdateFilesNumber();
	char filename[100]; // store the file name
	for (int i = 1; i <= files_number; i++)
	{
        snprintf(filename, sizeof(filename), "/home/roman/C_book_KimNKing/merkle_tree/data/transactions/block_%d.txt", i);
		ReadFileData(filename);
	}
}

void ReadFileData(char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		perror(filename);  // Print specific file error
		return;
	}
  	char buffer[256];
	printf("Contents of %s:\n", filename);

	// Read and print each line
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		printf("%s", buffer);
	}
	printf("\n");
	printf("\n GENERATING NODE AND ITS RELATIVE HASH: \n");
	GenerateNodesFromData(filename);
}

int UpdateFilesNumber(void)
{
    int files_number = 0;
    DIR *dir = opendir("/home/roman/C_book_KimNKing/merkle_tree/data/transactions/");
    	
    if (dir == NULL)
    {
        perror("Unable to open directory");
    }
	else
	{
		struct dirent *entry;  // Declare entry
		// Loop through directory entries
		while ((entry = readdir(dir)) != NULL)
		{
			// Check if file matches "block_*.txt"
			if (strncmp(entry->d_name, "block_", 6) == 0
				&& strstr(entry->d_name, ".txt"))
			{
				files_number++;
			}
		}
	}

    closedir(dir); // Close the directory to free resources
	printf("\n Calculated files number: %d\n", files_number);
	return files_number;
}

void GenerateNodesFromData(char *filename)
{
	printf("GOT THE FILE %s:\n", filename);
}