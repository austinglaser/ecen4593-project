/**
 * @file    test_L2Cache.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestL2Cache Source
 *
 * @addtogroup TEST_L2CACHE
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "L2Cache.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include "ConfigDefaults.h"
#include "mock_MainMem.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static config_t config;
static main_mem_t dummy_main_mem;
static l2_cache_t l2_cache;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);

    L2Cache_Create(&l2_cache, &dummy_main_mem, &config);
}

void tearDown(void)
{
}

void test_InstructionAccessToEmptyCacheShouldMiss(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123420,
        .n_bytes = 8,
    };

    uint32_t main_mem_access_cycles = 75;
    MainMem_Access_ExpectAndReturn(&dummy_main_mem, &access, main_mem_access_cycles);

    uint32_t expected_access_cycles = main_mem_access_cycles + config.l2.miss_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(&l2_cache, &access));
}

void test_ReadAccessToEmptyCacheShouldMiss(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123420,
        .n_bytes = 2,
    };

    uint32_t main_mem_access_cycles = 32;
    MainMem_Access_ExpectAndReturn(&dummy_main_mem, &access, main_mem_access_cycles);

    uint32_t expected_access_cycles = main_mem_access_cycles + config.l2.miss_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(&l2_cache, &access));
}

void test_WriteAccessToEmptyCacheShouldMiss(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123420,
        .n_bytes = 32,
    };

    uint32_t main_mem_access_cycles = 256;
    MainMem_Access_ExpectAndReturn(&dummy_main_mem, &access, main_mem_access_cycles);

    uint32_t expected_access_cycles = main_mem_access_cycles + config.l2.miss_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(&l2_cache, &access));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L2CACHE */
