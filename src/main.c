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

/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static void DestroyAll(void);

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static main_mem_t main_mem;
static l2_cache_t l2_cache;
static l1_cache_t l1i_cache;
static l1_cache_t l1d_cache;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**
 * @brief   Application Entry Point
 */
int main(int argc, char const * const * const argv)
{
    if (argc > 2) {
        printf("Usage: %s [config_file]\n", argv[0]);
        return -1;
    }

    char const * config_file = NULL;
    if (argc == 2) {
        config_file = argv[1];
    }

    config_t config;
    Config_FromFile(config_file, &config);

    printf("\nMemory system:\n");
    Config_Print(&config);
    printf("\n");

    stats_t stats;
    Statistics_Create(&stats);

    main_mem_t main_mem = MainMem_Create(&config);
    l2_cache_t l2_cache = L2Cache_Create(main_mem, &(stats.l2), &(config.l2));
    l1_cache_t l1i_cache = L1Cache_Create(l2_cache, &(stats.l1i), &(config.l1));
    l1_cache_t l1d_cache = L1Cache_Create(l2_cache, &(stats.l1d), &(config.l1));

    CEXCEPTION_T e;
    Try {
        char line[128];
        while (fgets(line, sizeof(line), stdin)) {
            access_t access;
            Access_ParseLine(line, &access);

            access_t l1_bus_aligned_access;
            Access_Align(&l1_bus_aligned_access, &access, 4);

            uint32_t n_aligned = l1_bus_aligned_access.n_bytes >> 2;
            l1_bus_aligned_access.n_bytes = 4;

            l1_cache_t top_cache = (l1_bus_aligned_access.type == TYPE_INSTR) ?
                                   l1i_cache :
                                   l1d_cache;

            uint32_t access_cycles = 0;
            uint32_t i;
            for (i = 0; i < n_aligned; i++) {
                access_cycles += L1Cache_Access(top_cache, &l1_bus_aligned_access);
                l1_bus_aligned_access.address += 4;
            }

            Statistics_RecordAccess(&stats, access.type, access_cycles, n_aligned);
        }
    }
    Catch (e) {
        DestroyAll();
        UncaughtException(e);
    }

    DestroyAll();

    Statistics_Print(&stats);
    printf("\n");

    return 0;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void DestroyAll(void)
{
    L1Cache_Destroy(l1i_cache);
    L1Cache_Destroy(l1d_cache);
    L2Cache_Destroy(l2_cache);
    MainMem_Destroy(main_mem);
}

/** @} defgroup MAIN */
