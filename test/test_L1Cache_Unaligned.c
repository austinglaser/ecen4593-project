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
#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"
#include "ConfigDefaults.h"

#include "mock_L2Cache.h"
#include "mock_CacheData.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static l1_cache_t l1_cache;
static l2_cache_t dummy_l2_cache;
static cache_data_t dummy_cache_data;
static config_t config;
static uint32_t n_blocks;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);

    n_blocks = config.l1.cache_size_bytes / config.l1.block_size_bytes;
    CacheData_Create_ExpectAndReturn(n_blocks,
                                     1,
                                     config.l1.block_size_bytes,
                                     8,
                                     dummy_cache_data);

    l1_cache = L1Cache_Create(dummy_l2_cache, &config);
}

void tearDown(void)
{
    CacheData_Destroy_Expect(dummy_cache_data);
    L1Cache_Destroy(l1_cache);
}

void test_UnalignedWordReadAccessHit_should_CauseTwoL1Operations(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x00001,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, 0, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    CacheData_Read_ExpectAndReturn(dummy_cache_data, 4, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles * 2;

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_UnalignedWordWriteAccessHit_should_CauseTwoL1Operations(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x00001,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, 0, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    CacheData_Write_ExpectAndReturn(dummy_cache_data, 4, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles * 2;

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L1DIRECTMAPPED */
