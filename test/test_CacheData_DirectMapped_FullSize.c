/**
 * @file    test_CacheData_DirectMapped_FullSize.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheDataDirectMappedFullSize Source
 *
 * @addtogroup TEST_CACHEDATA_DIRECTMAPPED_FULLSIZE
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

static const uint32_t block_size_bytes = 32;
static const uint32_t n_blocks         = 8192 / 32;
static cache_data_t cache_data;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    cache_data = CacheData_Create(n_blocks, 1, block_size_bytes, 0);
}

void tearDown(void)
{
    CacheData_Destroy(cache_data);
}

void test_FillItUp(void)
{
    uint64_t base_address = 0x3213120;

    uint32_t i;
    for (i = 0; i < n_blocks; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        TEST_ASSERT_EQUAL_HEX64(0, CacheData_Write(cache_data, address));
    }
}

void test_KickItAllBackOut(void)
{
    uint64_t base_address = 0x3213120;

    uint32_t i;
    for (i = 0; i < n_blocks; i++) {
        uint64_t address = base_address + i * block_size_bytes;
        CacheData_Write(cache_data, address);
    }

    for (i = 0; i < n_blocks; i++) {
        uint64_t new_address = base_address + (i + n_blocks) * block_size_bytes;
        uint64_t old_address = base_address + i * block_size_bytes;
        TEST_ASSERT_EQUAL_HEX64(old_address, CacheData_Read(cache_data, new_address));
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHEDATA_DIRECTMAPPED_FULLSIZE */
