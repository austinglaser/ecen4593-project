/**
 * @file    test_CacheData_FullyAssociative_FullSize.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheDataFullyAssociativeFullSize Source
 *
 * @addtogroup TEST_CACHEDATA_FULLYASSOCIATIVE_FULLSIZE
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "CacheData.h"
#include "unity.h"

#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static const uint32_t n_sets           = 1;
static const uint32_t set_len          = 8192 / 32;
static const uint32_t block_size_bytes = 32;
static cache_data_t cache_data;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

void setUp(void)
{
    cache_data = CacheData_Create(n_sets, set_len, block_size_bytes);
}

void tearDown(void)
{
    CacheData_Destroy(cache_data);
}

void test_CacheData_Read_should_StoreAllBlocks(void)
{
    uint64_t base_address = 0x71234200;

    uint32_t i;
    for (i = 0; i < set_len; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        CacheData_Read(cache_data, address);
    }

    for (i = 0; i < set_len; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        TEST_ASSERT(CacheData_Contains(cache_data, address));
    }
}

void test_CacheData_Read_should_KickoutOldestBlock(void)
{
    uint64_t base_address = 0x71234200;

    uint32_t i;
    for (i = 0; i < set_len + 1; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        CacheData_Read(cache_data, address);
    }

    TEST_ASSERT_FALSE(CacheData_Contains(cache_data, base_address));

    for (i = 1; i < set_len + 1; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        TEST_ASSERT(CacheData_Contains(cache_data, address));
    }
}

void test_CacheData_Read_should_KickoutOldestBlockDirty_when_BlockHasBeenWritten(void)
{
    uint64_t base_address = 0x71234200;

    CacheData_Write(cache_data, base_address);

    uint32_t i;
    for (i = 1; i < set_len; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        CacheData_Read(cache_data, address);
    }

    TEST_ASSERT_EQUAL_HEX64(base_address, CacheData_Read(cache_data, base_address + set_len * block_size_bytes));
}

void test_CacheData_Write_should_BringBlockToFrontOfLRU(void)
{
    uint64_t base_address = 0x71234200;

    uint32_t i;
    for (i = 0; i < set_len; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        CacheData_Read(cache_data, address);
    }

    CacheData_Write(cache_data, base_address);

    for (i = 0; i < set_len - 1; i++) {
        uint64_t address = base_address + (i + set_len) * block_size_bytes;
        CacheData_Read(cache_data, address);
    }
    TEST_ASSERT_EQUAL_HEX(base_address, CacheData_Read(cache_data, base_address + 2 * set_len * i * block_size_bytes));
}

/** @} addtogroup TEST_CACHEDATA_FULLYASSOCIATIVE_FULLSIZE */
