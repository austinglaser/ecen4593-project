/**
 * @file    test_L1DirectMapped.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestL1DirectMapped Source
 *
 * @addtogroup TEST_L1DIRECTMAPPED
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "L1Cache.h"

#include "Config.h"
#include "Access.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"
#include "ConfigDefaults.h"

#include "mock_L2Cache.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static l1_cache_t l1_cache;
static l2_cache_t l2_cache;
static config_t config;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);
    L1Cache_Create(&l1_cache, &config, &l2_cache);
}

void tearDown(void)
{
}

void test_WordReadFromEmptyCacheCausesMiss(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x7f81ce2206b0,
        .n_bytes = 4,
    };

    uint32_t l2_access_cycles = 34;
    L2Cache_Access_ExpectAndReturn(&l2_cache, &access, l2_access_cycles);

    uint32_t total_access_cycles = L1Cache_Access(&l1_cache, &access);

    TEST_ASSERT_EQUAL_UINT32(l2_access_cycles + config.l1.miss_time_cycles, total_access_cycles);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L1DIRECTMAPPED */
