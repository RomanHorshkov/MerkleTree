/**
 * @file tests.h
 * @author Roman Horshkov
 * @date 22 Feb 2025
 * @brief Merkle tree test header file
 */

#ifndef TESTS_H
#define TESTS_H

/*-----------------------------------*
* INCLUDE FILES
*-----------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif
 

/*-----------------------------------*
 * PUBLIC DEFINES
 *-----------------------------------*/
#define RESULTS_FILE "tests_results.txt"
/* Maximum number of folders to be read from config file */
#define MAX_TESTING_FOLDERS 10
 
/* Some color codes */
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BOLD    "\x1b[1m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

/*-----------------------------------*
 * PUBLIC MACROS
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC TYPEDEFS
 *-----------------------------------*/
typedef struct {
    char folder[256];
    int num_files;
} FolderConfig;

/*-----------------------------------*
 * PUBLIC VARIABLE DECLARATIONS
 *-----------------------------------*/
extern FolderConfig folders[MAX_TESTING_FOLDERS];
extern int numFolders;

/*-----------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *-----------------------------------*/
/**
 * @brief Runs performance and functionality tests for the Merkle tree engine.
 *
 * This function executes a series of tests using different
 * datasets (folders) and prints performance metrics.
 */
void RunMerkleTreeTests(void);

#ifdef __cplusplus
}
#endif

#endif /* TESTS_H */
 
