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

void test_CacheData_Read_should_SendBlockToVictimCache_when_WritingAnotherBlock(void)
{
    result_t result;
    uint64_t base_address = 0x293800;

    CacheData_Write(cache_data, base_address, &result);

    uint64_t address;
    uint32_t i;
    for (i = 0; i < victim_cache_len_blocks; i++) {
        address = base_address + (i + 1) *
                                 n_sets *
                                 block_size_bytes;
        TEST_ASSERT_EQUAL_HEX32_MESSAGE(0,
                                        CacheData_Read(cache_data, address, &result),
                                        "Didn't send block to victim cache");
        TEST_ASSERT_EQUAL(RESULT_MISS, result);
    }
    address = base_address + ((victim_cache_len_blocks + 1) *
                              n_sets *
                              block_size_bytes);
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(base_address,
                                    CacheData_Read(cache_data, address, &result),
                                    "Didn't dirty kickout from victim cache");
    TEST_ASSERT_EQUAL(RESULT_MISS_DIRTY_KICKOUT, result);
}

void test_CacheData_Read_should_PullBlockBackFromVictimCache(void)
{
    result_t result;
    uint64_t base_address = 0x823233300;

    CacheData_Write(cache_data, base_address, &result);
    uint64_t address;
    uint32_t i;
    for (i = 0; i < victim_cache_len_blocks; i++) {
        address = base_address + (i + 1) *
                                 n_sets *
                                 block_size_bytes;
        CacheData_Write(cache_data, address, &result);
    }

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0,
                                    CacheData_Read(cache_data, base_address, &result),
                                    "Kicked out when should have just reshuffled");
    TEST_ASSERT_EQUAL(RESULT_HIT_VICTIM_CACHE, result);

    address = base_address + (victim_cache_len_blocks + 1) * n_sets * block_size_bytes;
    uint64_t expected_kickout_address = base_address + n_sets * block_size_bytes;
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(expected_kickout_address,
                                    CacheData_Read(cache_data, address, &result),
                                    "Kicked out wrong block from victim cache");
    TEST_ASSERT_EQUAL(RESULT_MISS_DIRTY_KICKOUT, result);
}

void test_DifferentSets_should_AllWriteToVictimCache(void)
{
    result_t result;

    TEST_ASSERT_EQUAL_MESSAGE(2, victim_cache_len_blocks, "Test parameters need attention");
    uint64_t base_address = 0x9820200;
    uint64_t insertion_sequence[] = {
        base_address,
        base_address + block_size_bytes,
        base_address + n_sets * block_size_bytes,
        base_address + (n_sets + 1) * block_size_bytes,
    };
    uint32_t n_insertions = ARRAY_ELEMENTS(insertion_sequence);

    uint32_t i;
    for (i = 0; i < n_insertions; i++) {
        TEST_ASSERT_EQUAL_HEX64_MESSAGE(0,
                                        CacheData_Write(cache_data, insertion_sequence[i], &result),
                                        "Kicked out early");
        TEST_ASSERT_EQUAL(RESULT_MISS, result);
    }

    uint64_t address = base_address + (2 * n_sets + 1) * block_size_bytes;
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(insertion_sequence[0], CacheData_Read(cache_data, address, &result), "Wrong kickout order");
    TEST_ASSERT_EQUAL(RESULT_MISS_DIRTY_KICKOUT, result);

    address = base_address + 2 * n_sets * block_size_bytes;
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(insertion_sequence[1], CacheData_Read(cache_data, address, &result), "Wrong kickout order");
    TEST_ASSERT_EQUAL(RESULT_MISS_DIRTY_KICKOUT, result);

    address = base_address + 3 * n_sets * block_size_bytes;
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(insertion_sequence[3], CacheData_Read(cache_data, address, &result), "Wrong kickout order");
    TEST_ASSERT_EQUAL(RESULT_MISS_DIRTY_KICKOUT, result);

    address = base_address + (3 * n_sets + 1) * block_size_bytes;
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(insertion_sequence[2], CacheData_Read(cache_data, address, &result), "Wrong kickout order");
    TEST_ASSERT_EQUAL(RESULT_MISS_DIRTY_KICKOUT, result);
}

void test_CacheData_Write_should_FillUpSetsAndVictimCache(void)
{
    result_t result;
    uint64_t base_address = 0x7340F0;

    uint64_t address;
    uint32_t i;
    for (i = 0; i < n_sets; i++) {
        address = base_address + i * block_size_bytes;
        TEST_ASSERT_EQUAL_HEX64_MESSAGE(0, CacheData_Write(cache_data, address, &result), "Kickout while filling cache");
        TEST_ASSERT_EQUAL(RESULT_MISS, result);
    }

    for (i = 0; i < victim_cache_len_blocks; i++) {
        address = base_address + (n_sets + i) * block_size_bytes;
        TEST_ASSERT_EQUAL_HEX64_MESSAGE(0, CacheData_Write(cache_data, address, &result), "Kickout while filling victim cache");
        TEST_ASSERT_EQUAL(RESULT_MISS, result);
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHEDATA_DIRECTMAPPED_VICTIMCACHE */
