/**
 * @file tests.c
 * @author Roman Horshkov
 * @date 22 Feb 2025
 * @brief Merkle tree test implementation
 */

/*-----------------------------------*
 * INCLUDE FILES
 *-----------------------------------*/
#include "../inc/tests.h"
#include "merkleTree.h"
#include <stdio.h>
#include <unistd.h>         /* sysconf() */
#include <sys/time.h>       /* timeval */
#include <sys/resource.h>   /* rusage */
#include <sys/sysinfo.h>    /* sysinfo */
#include <sys/utsname.h>    /* utsname */

/*-----------------------------------*
 * PUBLIC VARIABLE DEFINITIONS
 *-----------------------------------*/
FolderConfig folders[MAX_TESTING_FOLDERS];
int numFolders = 0;

/*-----------------------------------*
 * PRIVATE DEFINES
 *-----------------------------------*/
#define RESULTS_FILE "tests_results.txt"

/*-----------------------------------*
 * PRIVATE FUNCTION PROTOTYPES
 *-----------------------------------*/
/**
 * @brief Measures execution time and system resource usage for a test run.
 *
 * This function constructs a Merkle tree from a specified folder, tracks
 * elapsed time, CPU usage, and memory consumption, then logs the results.
 *
 * @param fp File pointer for logging test results.
 * @param folder Directory containing transaction files for the test.
 */
static void run_test(FILE *fp, const char *folder);

/**
 * @brief Prints process memory usage statistics from `/proc/self/status`.
 *
 * This function extracts memory usage metrics such as peak virtual memory,
 * resident set size, and swap usage, then logs them to the results file.
 *
 * @param fp File pointer for logging results.
 */
static void printProcSelfStatus(FILE *fp);

/**
 * @brief Logs system hardware and OS information.
 *
 * This function records details such as CPU model, available RAM, system uptime,
 * and kernel version to provide context for performance analysis.
 *
 * @param fp File pointer for logging system details.
 */
static void PrintSysInfo(FILE *fp);

/**
 * @brief Prints a formatted banner for test results.
 *
 * This function writes a visually distinct header to the results file,
 * marking the start of the Merkle tree test results section.
 *
 * @param fp File pointer to the results file.
 */
static void PrintBanner(FILE *fp);

/**
 * @brief Computes the time difference in milliseconds between two `timeval` structures.
 *
 * @param start Pointer to the start time.
 * @param end Pointer to the end time.
 * @return The time difference in milliseconds.
 */
static double timeval_diff_ms(struct timeval *start, struct timeval *end);

/*-----------------------------------*
 * PRIVATE VARIABLES
 *-----------------------------------*/
/* None */

/*-----------------------------------*
 * PUBLIC FUNCTION DEFINITIONS
 *-----------------------------------*/
void RunMerkleTreeTests(void)
{
    FILE *fp = fopen(RESULTS_FILE, "w");
    /* if the file is successfully opened */
    if (fp)
    {
        /* Print the banner to file  */
        PrintBanner(fp);
        /* Print system info to file */
        PrintSysInfo(fp);
        /* Run the tests */
        for (int i = 0; i < (numFolders); i++)
        {
            run_test(fp, folders[i].folder);
        }
        fclose(fp);
    }
    else
    {
        perror("Failed to open results file");
        return;
    }
}

/*-----------------------------------*
 * PRIVATE FUNCTION DEFINITIONS
 *-----------------------------------*/
static void run_test(FILE *fp, const char *folder)
{
    struct timeval start_tv, end_tv;
    struct rusage start_ru, end_ru;

    /* Record start times */
    gettimeofday(&start_tv, NULL);
    getrusage(RUSAGE_SELF, &start_ru);

    /* Actual building of the Merkle Tree */
    BuildMerkleTree(folder);

    /* Record end times */
    gettimeofday(&end_tv, NULL);
    getrusage(RUSAGE_SELF, &end_ru);

    /* Print process memory usage */
    printProcSelfStatus(fp);

    /* Calculate metrics */
    double elapsed_ms   = timeval_diff_ms(&start_tv, &end_tv);
    double user_time_ms = timeval_diff_ms(&start_ru.ru_utime, &end_ru.ru_utime);
    double sys_time_ms  = timeval_diff_ms(&start_ru.ru_stime, &end_ru.ru_stime);
    long max_rss_kb     = end_ru.ru_maxrss;  /* In kilobytes on Linux */

    /* Print to file */
    fprintf(fp, "%-30s %12s %12s %12s %12s\n",
        "FOLDER", "ELAPSED (ms)", "USER CPU", "SYS CPU", "MAX RSS (KB)");
    fprintf(fp, "--------------------------------------------------------------------------------------------\n");

    fprintf(fp, "%-30s %12.2f %12.2f %12.2f %12ld\n",
           folder, elapsed_ms, user_time_ms, sys_time_ms, max_rss_kb);

    fprintf(fp, "--------------------------------------------------------------------------------------------\n");
    fprintf(fp, "%-20s %15s %15s %15s\n",
        "Min Page Faults", "Maj Page Faults", "Blocks In", "Blocks Out");
    fprintf(fp, "%-20ld %15ld %15ld %15ld\n",
        end_ru.ru_minflt - start_ru.ru_minflt,
        end_ru.ru_majflt - start_ru.ru_majflt,
        end_ru.ru_inblock - start_ru.ru_inblock,
        end_ru.ru_oublock - start_ru.ru_oublock
    );
    fprintf(fp, "--------------------------------------------------------------------------------------------\n");

}

static void printProcSelfStatus(FILE *fp)
{
    FILE *status_fp = fopen("/proc/self/status", "r");
    if (!status_fp)
    {
        perror("Failed to open /proc/self/status");
        return;
    }

    fprintf(fp, "--------------------------------------------------------------------------------------------\n");
    fprintf(fp, "%-15s %10s %-40s\n", "Memory Metric", "Value (kB)", "Description");
    fprintf(fp, "--------------------------------------------------------------------------------------------\n");

    char line[256];
    while (fgets(line, sizeof(line), status_fp))
    {
        if (strncmp(line, "Vm", 2) == 0)
        {
            char label[20];
            long value;
            char unit[10];

            if (sscanf(line, "%19s %ld %9s", label, &value, unit) == 3)
            {
                const char *description = "";

                if (strcmp(label, "VmPeak:") == 0) description = "Peak virtual memory size";
                else if (strcmp(label, "VmSize:") == 0) description = "Total virtual memory size";
                else if (strcmp(label, "VmLck:") == 0) description = "Locked memory (not swapped)";
                else if (strcmp(label, "VmPin:") == 0) description = "Pinned memory (for real-time processes)";
                else if (strcmp(label, "VmHWM:") == 0) description = "Peak resident set size (high-water mark)";
                else if (strcmp(label, "VmRSS:") == 0) description = "Resident Set Size (actual RAM usage)";
                else if (strcmp(label, "VmData:") == 0) description = "Size of data segment (heap, malloc)";
                else if (strcmp(label, "VmStk:") == 0) description = "Size of stack segment";
                else if (strcmp(label, "VmExe:") == 0) description = "Size of text (code) segment";
                else if (strcmp(label, "VmLib:") == 0) description = "Size of shared libraries";
                else if (strcmp(label, "VmPTE:") == 0) description = "Page table entries size";
                else if (strcmp(label, "VmSwap:") == 0) description = "Swapped-out memory size";

                fprintf(fp, "%-15s %10ld %s\n", label, value, description);
            }
        }
    }

    fprintf(fp, "--------------------------------------------------------------------------------------------\n");
    fclose(status_fp);
}

static void PrintSysInfo(FILE *fp)
{
    struct utsname sys_info;
    struct sysinfo info;

    /* Print OS Information */
    if (uname(&sys_info) == 0)
    {
        fprintf(fp, "OS Information\n");
        fprintf(fp, "  Sysname  : %s\n", sys_info.sysname);
        fprintf(fp, "  Nodename : %s\n", sys_info.nodename);
        fprintf(fp, "  Release  : %s\n", sys_info.release);
        fprintf(fp, "  Version  : %s\n", sys_info.version);
        fprintf(fp, "  Machine  : %s\n", sys_info.machine);
    }

    fprintf(fp, "-----------------------------------\n");
    printf("-----------------------------------\n");

    /* Get CPU & Memory Info */
    long num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    long page_size_kb = sysconf(_SC_PAGESIZE) / 1024;
    long total_ram_kb = sysconf(_SC_PHYS_PAGES) * page_size_kb;
    long free_ram_kb = sysconf(_SC_AVPHYS_PAGES) * page_size_kb;

    fprintf(fp, "Hardware Information\n");
    fprintf(fp, "  CPU Cores : %ld\n", num_processors);
    fprintf(fp, "  RAM Total : %ld MB\n", total_ram_kb / 1024);
    fprintf(fp, "  RAM Free  : %ld MB\n", free_ram_kb / 1024);

    /* Get Exact CPU Model */
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo)
    {
        char line[256], processor_model[128] = "Unknown";
        while (fgets(line, sizeof(line), cpuinfo))
        {
            if (strncmp(line, "model name", 10) == 0)
            {
                sscanf(line, "model name : %[^\n]", processor_model);
                break;
            }
        }
        fclose(cpuinfo);

        fprintf(fp, "  Processor : %s\n", processor_model);
    }

    /* Get RAM Speed */
    fprintf(fp, "  RAM Speed : Run `sudo dmidecode -t memory`\n");

    fprintf(fp, "-----------------------------------\n");

    /* System Uptime & Load */
    if (sysinfo(&info) == 0)
    {
        fprintf(fp, "System Uptime & Load\n");
        fprintf(fp, "  Uptime    : %ld hours %ld min %ld sec\n",
                info.uptime / 3600, (info.uptime % 3600) / 60, info.uptime % 60);
        fprintf(fp, "  Load Avg  : %.2f, %.2f, %.2f\n",
                info.loads[0] / 65536.0, info.loads[1] / 65536.0, info.loads[2] / 65536.0);
    }

    fprintf(fp, "-----------------------------------\n\n");
}

static void PrintBanner(FILE *fp)
{
    /* Print the banner to file */
    fprintf(fp, "=======================================================================\n");
    fprintf(fp, "                  M E R K L E   T R E E   T E S T S                    \n");
    fprintf(fp, "=======================================================================\n\n");
}

static double timeval_diff_ms(struct timeval *start, struct timeval *end)
{
    double start_ms = (double)start->tv_sec * 1000.0 + (double)start->tv_usec / 1000.0;
    double end_ms   = (double)end->tv_sec * 1000.0 + (double)end->tv_usec / 1000.0;
    return end_ms - start_ms;
}
