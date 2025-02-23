/**
 * @file main.c
 * @author Roman Horshkov
 * @date 22 Feb 2025
 * @brief Main entry point for Merkle tree testing.
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "tests.h"
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system() */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
#define TEST_SPEC_FILE "test_spec.txt"
#define FILE_NAME_FORMAT "block_%d.txt"

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
 * @brief Reads test specifications from a configuration file.
 *
 * This function reads the number of test files from a predefined configuration
 * file and initializes folder structures accordingly.
 *
 * @retval true if the configuration file is successfully read.
 * @retval false if the file cannot be opened or parsed.
 */
static bool readTestSpecifications(void);

/**
 * @brief Generates test files for Merkle tree testing.
 *
 * This function creates dummy transaction files in specified folders
 * to simulate real-world data input for Merkle tree construction.
 */
static void generateFiles(void);

/**
 * @brief Removes generated test files after testing.
 *
 * This function deletes the generated transaction files and directories
 * to ensure a clean environment after test execution.
 */
static void removeFiles(void);

// static void PrintResultsFromFile(void);
// static void PrintBanner(void);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
int main(int argc, char **argv)
{
    /* read test specifications */
    if(readTestSpecifications())
    {
        /* generate test files */
        generateFiles();
    
        /* RUN TESTS */
        RunMerkleTreeTests();
        
        /* Print results from file */
        // PrintResultsFromFile();
    
        /* remove test files */
        removeFiles();
    }

    return 0;
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
static bool readTestSpecifications(void)
{
    bool ret = false;
    /* Open the test specification file */
    FILE *fp = fopen(TEST_SPEC_FILE, "r");
    if (fp)
    {
        numFolders = 0;
        while (numFolders < MAX_TESTING_FOLDERS &&
               fscanf(fp, "%d", &folders[numFolders].num_files) == 1)
        {
            snprintf(folders[numFolders].folder,
                     sizeof(folders[numFolders].folder),
                     "data/transactions_%d/",
                     folders[numFolders].num_files);
            numFolders++;
        }
        fclose(fp);
        ret = true;
    }
    return ret;
}

static void generateFiles(void)
{
    char filename[256];

    for (int i = 0; i < numFolders; i++)
    {
        mkdir(folders[i].folder, 0777);

        for (int j = 0; j < folders[i].num_files; j++)
        {
            int written = snprintf(filename, sizeof(filename), "%s/" FILE_NAME_FORMAT, folders[i].folder, j);
            if (written < 0 || written >= sizeof(filename))
            {
                fprintf(stderr, "Error: Filename truncated for folder: %s\n", folders[i].folder);
                continue;
            }
            FILE *fp = fopen(filename, "w");
            if (fp)
            {
                fprintf(fp, "This is dummy content for file %d in folder %s.\n",
                        j, folders[i].folder);
                fclose(fp);
            }
            else
            {
                perror("Error creating file");
            }
        }
    }

    printf("Folders and dummy files generated successfully.\n");
}

static void removeFiles(void)
{
    char command[256];

    for (int i = 0; i < numFolders; i++)
    {
        int written = snprintf(command, sizeof(command), "rm -rf %s", folders[i].folder);
        if (written < 0 || written >= sizeof(command))
        {
            fprintf(stderr, "Error: Command truncated for folder: %s\n", folders[i].folder);
            continue;
        }
        if(!system(command))
        {
            perror("removeFiles: system() call failed\n");
        }
    }

    printf("All test folders removed.\n");
}


/*-----------------------------------*
 * COMMENTED OUT FUNCTIONS
 *-----------------------------------*/
// static void PrintResultsFromFile(void)
// {
    // /* print ASCII banner */
    // PrintBanner();
//     FILE *fp = fopen(RESULTS_FILE, "r");
//     if (!fp)
//     {
//         perror("Failed to open results file for reading");
//         return;
//     }

//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), fp))
//     {
//         printf("%s", buffer);  // Print line-by-line
//     }

//     fclose(fp);
// }

// static void PrintBanner(void)
// {
//     printf(COLOR_CYAN
//         "=======================================================================\n"
//         COLOR_RESET);
//     printf(COLOR_CYAN
//         "                  M E R K L E   T R E E   T E S T S                    \n"
//         COLOR_RESET);
//     printf(COLOR_CYAN
//         "=======================================================================\n\n"
//         COLOR_RESET);
// }
