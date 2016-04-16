/**
 * @file    main.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Application Entry Point
 *
 * @defgroup MAIN Main
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Access.h"
#include "CException.h"
#include "CExceptionConfig.h"
#include "Config.h"
#include "ExceptionTypes.h"
#include "L1Cache.h"
#include "L2Cache.h"
#include "MainMem.h"
#include "Statistics.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* --- PRIVATE DATATYPES ---------------------------------------------------- */

/**@brief   The memory hierarchy */
typedef struct {
    main_mem_t main_mem;         /**< Main Memory, wherein all data may be found */
    l2_cache_t l2_cache;         /**< L2  -> Main Memory */
    l1_cache_t l1i_cache;        /**< L1i -> L2 */
    l1_cache_t l1d_cache;        /**< L1d -> L2 */
} memory_t;

/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

/**@brief   Creates the full memory hierarchy */
static void Memory_Create(memory_t * mem, stats_t * stats, config_t * config);

/**@brief   Tears down the memory hierarchy */
static void Memory_Destroy(memory_t * mem);

/**@brief   Parse command-line options*/
static void parse_args(int argc, char const * const * const argv,
                       char const * * config_file, char const * * trace_name);

/**@brief   Prints an ultra-useful usage message */
static void usage(char const * call);

/**@brief   Handle the topmost-level logic for an access */
static uint32_t do_access(memory_t * mem, access_t const * access, uint32_t * n_aligned);

/**@brief   Prints the results of a completed simulation */
static void print_results(memory_t * mem, char const * config_file, char const * trace_name,
                          config_t * config, stats_t * stats);

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Application Entry Point */
int main(int argc, char const * const * const argv)
{
    char const * config_file = NULL;
    char const * trace_name = NULL;
    parse_args(argc, argv, &config_file, &trace_name);

    config_t config;
    Config_FromFile(config_file, &config);

    stats_t stats;
    Statistics_Create(&stats);

    memory_t mem;
    Memory_Create(&mem, &stats, &config);

    CEXCEPTION_T e;
    Try {
        char line[128];
        while (fgets(line, sizeof(line), stdin)) {
            access_t access;
            Access_ParseLine(line, &access);

            uint32_t n_aligned;
            uint32_t access_cycles = do_access(&mem, &access, &n_aligned);

            Statistics_RecordAccess(&stats, access.type, access_cycles, n_aligned);
        }
    }
    Catch (e) {
        Memory_Destroy(&mem);
        UncaughtException(e);
    }

    print_results(&mem, config_file, trace_name, &config, &stats);

    Memory_Destroy(&mem);

    return 0;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void Memory_Create(memory_t * mem, stats_t * stats, config_t * config)
{
    mem->main_mem  = MainMem_Create(&(config->main_mem));
    mem->l2_cache  = L2Cache_Create(mem->main_mem,  &(stats->l2),  &(config->l2));
    mem->l1i_cache = L1Cache_Create(mem->l2_cache, &(stats->l1i), &(config->l1));
    mem->l1d_cache = L1Cache_Create(mem->l2_cache, &(stats->l1d), &(config->l1));
}

static void Memory_Destroy(memory_t * mem)
{
    L1Cache_Destroy(mem->l1i_cache);
    L1Cache_Destroy(mem->l1d_cache);
    L2Cache_Destroy(mem->l2_cache);
    MainMem_Destroy(mem->main_mem);
}

static void parse_args(int argc, char const * const * const argv,
                       char const * * config_file, char const * * trace_name)
{
    if (argc > 4) {
        printf("Too many arguments\n\n");
        usage(argv[0]);
        exit(-1);
    }

    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp("-t", argv[i]) == 0) {
            if (i == argc - 1) {
                printf("'-t' takes an argument\n\n");
                usage(argv[0]);
                exit(-1);
            }
            *trace_name = argv[i + 1];
            i++;
        }
        else {
            if (*config_file != NULL) {
                printf("extra argument '%s'\n\n", argv[i]);
                usage(argv[0]);
                exit(-1);
            }
            *config_file = argv[i];
        }
    }
}

static void usage(char const * call)
{
    printf("Usage: %s [config_file] [-t <trace_name>]\n"
            "    If only one argument is given, it is assumed to be config_file.\n", call);
}

static uint32_t do_access(memory_t * mem, access_t const * access, uint32_t * n_aligned)
{
    access_t l1_bus_aligned_access;
    Access_Align(&l1_bus_aligned_access, access, 4);

    *n_aligned = l1_bus_aligned_access.n_bytes >> 2;
    l1_bus_aligned_access.n_bytes = 4;

    l1_cache_t top_cache = mem->l1d_cache;
    uint32_t access_cycles = 0;
    if (l1_bus_aligned_access.type == TYPE_INSTR) {
        top_cache = mem->l1i_cache;
        access_cycles = 1;
    }

    uint32_t i;
    for (i = 0; i < *n_aligned; i++) {
        access_cycles += L1Cache_Access(top_cache, &l1_bus_aligned_access);
        l1_bus_aligned_access.address += 4;
    }

    return access_cycles;
}

static void print_results(memory_t * mem, char const * config_file, char const * trace_name,
                          config_t * config, stats_t * stats)
{
    // This is designed to print EXACTLY like the sample traces. It's close
    // enough that a direct diff (ignoring whitespace) can be used to compare
    // our output with the expected values, which is immensly helpful in
    // automating testing
    char const * config_name = "default";
    if (config_file != NULL) {
        config_name = strrchr(config_file, '/') + 1;
        if (config_name == NULL) {
            config_name = config_file;
        }
    }

    char case_name[128] = { '\0' };
    if (trace_name != NULL) {
        strncat(case_name, trace_name,  sizeof(case_name) - 1);
        strncat(case_name, ".",         sizeof(case_name) - strlen(case_name) - 1);
    }
    strncat(case_name, config_name, sizeof(case_name) - strlen(case_name) - 1);

    printf("\n-------------------------------------------------------------------------\n");
    printf("      %-25s Simulation Results\n", case_name);
    printf("-------------------------------------------------------------------------\n\n");

    printf("  Memory system:\n");
    Config_Print(config);
    printf("\n");

    Statistics_Print(stats);
    printf("\n");

    Config_PrintCost(config);
    printf("\n");

    printf("-------------------------------------------------------------------------\n\n");

    printf("Cache final contents - Index and Tag values are in HEX\n\n");

    printf("Memory Level: L1i\n");
    L1Cache_Print(mem->l1i_cache);
    printf("\n");

    printf("Memory Level: L1d\n");
    L1Cache_Print(mem->l1d_cache);
    printf("\n");

    printf("Memory Level: L2\n");
    L2Cache_Print(mem->l2_cache);
    printf("\n");
}

/** @} defgroup MAIN */
