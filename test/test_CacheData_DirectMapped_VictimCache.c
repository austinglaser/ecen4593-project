/**
 * @file    test_CacheData_DirectMapped_VictimCache.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheDataDirectMappedVictimCache Source
 *
 * @addtogroup TEST_CACHEDATA_DIRECTMAPPED_VICTIMCACHE
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

static const uint32_t n_sets                    = 16;
static const uint32_t victim_cache_len_blocks   = 2;
static const uint32_t block_size_bytes          = 16;
static cache_data_t cache_data;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    cache_data = CacheData_Create(n_sets, 1, block_size_bytes, victim_cache_len_blocks);
}

void tearDown(void)
{
    CacheData_Destroy(cache_data);
}

void test_CacheData_Read_should_SendBlockToVictimCache(void)
{
    uint64_t base_address = 0x293800;

    CacheData_Write(cache_data, base_address);

    uint64_t address;
    uint32_t i;
    for (i = 0; i < victim_cache_len_blocks; i++) {
        address = base_address + (i + 1) *
                                 n_sets *
                                 block_size_bytes;
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(0,
                                        CacheData_Read(cache_data, address),
                                        "Didn't send block to victim cache");
    }
    address = base_address + ((victim_cache_len_blocks + 1) *
                              n_sets *
                              block_size_bytes);
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(base_address,
                                    CacheData_Read(cache_data, address),
                                    "Didn't dirty kickout from victim cache");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHEDATA_DIRECTMAPPED_VICTIMCACHE */
