#include "../inc/tests.h"
#include "merkleTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>

void generateFiles(void) {
    int ret = system("python3 genFiles.py");
    if(ret != 0) {
        printf("Error generating files.\n");
    }
}

void removeFiles(void) {
    int ret = system("python3 rmFiles.py");
    if(ret != 0) {
        printf("Error removing files.\n");
    }
}

/**
 * @brief Computes the difference in milliseconds between two timeval structures.
 *
 * @param start Pointer to the starting timeval.
 * @param end Pointer to the ending timeval.
 * @return double The time difference in milliseconds.
 */
static double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    double start_ms = (double)start->tv_sec * 1000.0 + (double)start->tv_usec / 1000.0;
    double end_ms   = (double)end->tv_sec * 1000.0 + (double)end->tv_usec / 1000.0;
    return end_ms - start_ms;
}

/**
 * @brief Runs a single test on the specified folder and prints performance metrics.
 *
 * This function measures wall-clock time, CPU time (user and system), and memory usage before and after
 * building the Merkle tree.
 *
 * @param folder The path to the folder containing the transaction files.
 */
static void run_test(const char *folder)
{
    printf("Running test on folder: %s\n", folder);

    /* 
     * Depending on your implementation, you might need to set a global variable or
     * pass the folder name to BuildMerkleTree() so that it uses the correct dataset.
     * For this example, we assume BuildMerkleTree() uses a global configuration.
     */

    struct timeval start_tv, end_tv;
    struct rusage start_ru, end_ru;
    
    gettimeofday(&start_tv, NULL);
    getrusage(RUSAGE_SELF, &start_ru);

    /* Run Merkle tree build process */
    BuildMerkleTree(folder);

    gettimeofday(&end_tv, NULL);
    getrusage(RUSAGE_SELF, &end_ru);

    double elapsed_ms   = timeval_diff_ms(&start_tv, &end_tv);
    double user_time_ms = timeval_diff_ms(&start_ru.ru_utime, &end_ru.ru_utime);
    double sys_time_ms  = timeval_diff_ms(&start_ru.ru_stime,  &end_ru.ru_stime);
    long max_rss_kb     = end_ru.ru_maxrss;  /* In kilobytes on Linux */

    printf("Elapsed time   : %.2f ms\n", elapsed_ms);
    printf("User CPU time  : %.2f ms\n", user_time_ms);
    printf("System CPU time: %.2f ms\n", sys_time_ms);
    printf("Max RSS        : %ld KB\n", max_rss_kb);
    printf("-----------------------------------\n");
}

/**
 * @brief Runs a series of tests on different datasets and prints system information.
 *
 * This function retrieves and prints basic OS information and then runs tests on multiple datasets.
 */
void RunMerkleTreeTests(void)
{
    /* Generate test files */
    generateFiles();
    /* Print OS information */
    struct utsname sys_info;
    if (uname(&sys_info) == 0)
    {
        printf("OS Information:\n");
        printf("  Sysname : %s\n", sys_info.sysname);
        printf("  Nodename: %s\n", sys_info.nodename);
        printf("  Release : %s\n", sys_info.release);
        printf("  Version : %s\n", sys_info.version);
        printf("  Machine : %s\n", sys_info.machine);
        printf("-----------------------------------\n");
    }

    /* Define an array of test folder paths (adjust these as needed) */
    const char *folders[] = {
        "data/transactions_256",
        "data/transactions_257",
        "data/transactions_4096",
        "data/transactions_4097"
    };

    int numFolders = sizeof(folders) / sizeof(folders[0]);
    for (int i = 0; i < numFolders; i++)
    {
        run_test(folders[i]);
    }
    /* Delete test files */
    removeFiles();
}