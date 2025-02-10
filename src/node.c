#include "node.h"
#include <stdio.h>
#include <dirent.h> // for counting files
#include <string.h>

//node .c file for general functionality

// PRIVATE VARIABLES
int files_number = 0;


// PRIVATE FUNCTIONS 
// brief
// function that reads the data files and stores 
// them in nodes
void ReadFileData(void);
void ReadFilesData(void);
void UpdateFilesNumber(void);

void GenerateNodesFromData();
void GenerateNodesFromHashes


void ReadFilesData()
{
	char filename[20]; // store the file name
	for (int i = 1; i <= files_number; i++)
	{
        	snprintf(filename, sizeof(filename), "block_%d.txt", i);
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
}
void UpdateFilesNumber(void)
{

	DIR *dir = opendir("/home/roman/C_book_KimNKing/merkle_tree/data");
	char filepath[512];
    	
	if (dir == NULL)
	{
        	perror("Unable to open directory");
        	return;
    	}
	
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

