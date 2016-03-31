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

static const uint32_t n_sets           = 16;
static const uint32_t block_size_bytes = 4;
static cache_sets_t cache_sets;

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
    uint64_t address = 0x7ff04304;
    TEST_ASSERT_EQUAL_HEX64(0, CacheSet_Insert(cache_sets, address));
}

void test_CacheSet_Contains_should_ReturnTrue_when_BlockHasBeenInserted(void)
{
    uint64_t address = 0x7ff04308;
    CacheSet_Insert(cache_sets, address);

    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address), "Failed to find inserted block");
}

void test_CacheSet_Contains_should_ReturnFalse_when_DifferentBlockHasBeenInserted(void)
{
    uint64_t address = 0x7ff0430c;
    CacheSet_Insert(cache_sets, address);

    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address + block_size_bytes), "Found wrong block");
    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address - block_size_bytes), "Found wrong block");
}

void test_CacheSet_Contains_should_ReportTrue_when_MultipleBlocksInDifferentSetsHaveBeenSaved(void)
{
    uint64_t address1 = 0x7ff04310;
    uint64_t address2 = address1 + block_size_bytes;
    uint64_t address3 = address1 - block_size_bytes;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Insert(cache_sets, address2);
    CacheSet_Insert(cache_sets, address3);

    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address1), "Failed to find inserted block");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address2), "Failed to find inserted block");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address3), "Failed to find inserted block");
}

void test_CacheSet_Insert_should_KickOutOldBlock_when_TwoAddressesMappedToSameBlockAreInserted(void)
{
    uint64_t address1 = 0x7ff04314;
    uint64_t address2 = address1 + block_size_bytes * n_sets;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Insert(cache_sets, address2);

    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address1), "Found block that should have been kicked out");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address2), "Failed to find inserted block");
}

void test_CacheSet_Write_should_Fail_when_BlockIsntPresent(void)
{
    uint64_t address = 0x7ff04318;

    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Write(cache_sets, address), "Write succeeded to a non-present block");
}

void test_CacheSet_Write_should_Succeed_when_BlockIsPresent(void)
{
    uint64_t address = 0x7ff0431c;

    CacheSet_Insert(cache_sets, address);

    TEST_ASSERT_MESSAGE(CacheSet_Write(cache_sets, address), "Write failed to a present block");
}

void test_CacheSet_Insert_should_KickOutDirtyBlock_when_BlockHasBeenWritten(void)
{
    uint64_t address1 = 0x7ff04320;
    uint64_t address2 = address1 + block_size_bytes * n_sets;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Write(cache_sets, address1);

    TEST_ASSERT_EQUAL_HEX64(address1, CacheSet_Insert(cache_sets, address2));
}

void test_CacheSet_Contains_should_IgnoreOffsetWithinBlock(void)
{
    uint64_t address1 = 0x7ff04324;
    uint64_t address2 = address1 + 1;
    uint64_t address3 = address1 + 3;
    uint64_t address4 = address1 + block_size_bytes - 1;

    CacheSet_Insert(cache_sets, address1);

    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address2), "Offset within block should be ignored");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address3), "Offset within block should be ignored");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address4), "Offset within block should be ignored");
}

void test_CacheSet_Insert_should_BeAbleToFillCache(void)
{
    uint64_t base_address = 0x7000000;

    uint32_t i;
    for (i = 0; i < n_sets; i++) {
        uint64_t block_address = base_address + block_size_bytes * i;
        TEST_ASSERT_EQUAL_HEX64(0, CacheSet_Insert(cache_sets, block_address));

        // So we see a dirty kickout if anything is overwritten
        CacheSet_Write(cache_sets, block_address);
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHESET */
