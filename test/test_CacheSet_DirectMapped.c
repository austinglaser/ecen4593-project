/**
 * @file    test_CacheSet.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheSet Source
 *
 * @addtogroup TEST_CACHESET
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "CacheSet.h"
#include "unity.h"

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

const uint32_t n_sets           = 16;
const uint32_t block_size_bytes = 4;
cache_sets_t cache_sets;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    cache_sets = CacheSet_Create_Sets(n_sets, 1, block_size_bytes);
}

void tearDown(void)
{
    CacheSet_Destroy_Sets(cache_sets);
}

void test_CacheSet_Get_NSets_should_ReturnNumberOfSets(void)
{
    TEST_ASSERT_EQUAL(n_sets, CacheSet_Get_NSets(cache_sets));
}

void test_CacheSet_Get_SetLength_should_ReturnLengthOfSets(void)
{
    TEST_ASSERT_EQUAL(1, CacheSet_Get_SetLength(cache_sets));
}

void test_CacheSet_Get_BlockSize_should_ReturnSizeOfBlocks(void)
{
    TEST_ASSERT_EQUAL(block_size_bytes, CacheSet_Get_BlockSize(cache_sets));
}

void test_CacheSet_Contains_should_ReturnFalse_when_CacheSetIsEmpty(void)
{
    uint64_t address = 0x7ff04300;
    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address), "Found block in empty cache set");
}

void test_CacheSet_Insert_should_NotKickOutBlock_when_CacheSetIsEmpty(void)
{
    uint64_t address = 0x7ff04300;
    TEST_ASSERT_EQUAL_HEX64(0, CacheSet_Insert(cache_sets, address));
}

void test_CacheSet_Contains_should_ReturnTrue_when_BlockHasBeenInserted(void)
{
    uint64_t address = 0x7ff04300;
    CacheSet_Insert(cache_sets, address);

    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address), "Failed to find inserted block");
}

void test_CacheSet_Contains_should_ReturnFalse_when_DifferentBlockHasBeenInserted(void)
{
    uint64_t address = 0x7ff04300;
    CacheSet_Insert(cache_sets, address);

    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address + block_size_bytes), "Found wrong block");
    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address - block_size_bytes), "Found wrong block");
}

void test_CacheSet_Contains_should_ReportTrue_when_MultipleBlocksInDifferentSetsHaveBeenSaved(void)
{
    uint64_t address1 = 0x7ff04300;
    uint64_t address2 = address1 + block_size_bytes;
    uint64_t address3 = address1 - block_size_bytes;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Insert(cache_sets, address2);
    CacheSet_Insert(cache_sets, address3);

    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address1), "Failed to find inserted block");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address2), "Failed to find inserted block");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address3), "Failed to find inserted block");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHESET */
