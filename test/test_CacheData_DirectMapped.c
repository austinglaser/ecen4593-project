/**
 * @file    test_CacheData.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheData Source
 *
 * @addtogroup TEST_CACHEDATA
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

static const uint32_t n_sets           = 16;
static const uint32_t block_size_bytes = 4;
static cache_data_t cache_data;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    cache_data = CacheData_Create(n_sets, 1, block_size_bytes);
}

void tearDown(void)
{
    CacheData_Destroy(cache_data);
}

void test_CacheData_Contains_should_ReturnFalse_when_CacheDataIsEmpty(void)
{
    uint64_t address = 0x7ff04300;
    TEST_ASSERT_FALSE_MESSAGE(CacheData_Contains(cache_data, address), "Found block in empty cache set");
}

void test_CacheData_Insert_should_NotKickOutBlock_when_CacheDataIsEmpty(void)
{
    uint64_t address = 0x7ff04304;
    TEST_ASSERT_EQUAL_HEX64(0, CacheData_Read(cache_data, address));
}

void test_CacheData_Contains_should_ReturnTrue_when_BlockHasBeenInserted(void)
{
    uint64_t address = 0x7ff04308;
    CacheData_Read(cache_data, address);

    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address), "Failed to find inserted block");
}

void test_CacheData_Contains_should_ReturnFalse_when_DifferentBlockHasBeenInserted(void)
{
    uint64_t address = 0x7ff0430c;
    CacheData_Read(cache_data, address);

    TEST_ASSERT_FALSE_MESSAGE(CacheData_Contains(cache_data, address + block_size_bytes), "Found wrong block");
    TEST_ASSERT_FALSE_MESSAGE(CacheData_Contains(cache_data, address - block_size_bytes), "Found wrong block");
}

void test_CacheData_Contains_should_ReportTrue_when_MultipleBlocksInDifferentSetsHaveBeenSaved(void)
{
    uint64_t address1 = 0x7ff04310;
    uint64_t address2 = address1 + block_size_bytes;
    uint64_t address3 = address1 - block_size_bytes;

    CacheData_Read(cache_data, address1);
    CacheData_Read(cache_data, address2);
    CacheData_Read(cache_data, address3);

    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address1), "Failed to find inserted block");
    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address2), "Failed to find inserted block");
    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address3), "Failed to find inserted block");
}

void test_CacheData_Insert_should_NotKickout_when_TheSameBlockIsInserted(void)
{
    uint64_t address = 0x7ff04328;

    CacheData_Read(cache_data, address);
    CacheData_Write(cache_data, address);

    TEST_ASSERT_EQUAL_HEX64(0, CacheData_Read(cache_data, address));
}

void test_CacheData_Insert_should_KickOutOldBlock_when_TwoAddressesMappedToSameBlockAreInserted(void)
{
    uint64_t address1 = 0x7ff04314;
    uint64_t address2 = address1 + block_size_bytes * n_sets;

    CacheData_Read(cache_data, address1);
    CacheData_Read(cache_data, address2);

    TEST_ASSERT_FALSE_MESSAGE(CacheData_Contains(cache_data, address1), "Found block that should have been kicked out");
    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address2), "Failed to find inserted block");
}

void test_CacheData_Write_should_AddBlock_when_BlockIsntPresent(void)
{
    uint64_t address = 0x7ff04318;

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0, CacheData_Write(cache_data, address), "Write kicked out dirty from empty cache");
}

void test_CacheData_Write_should_NotDirtyKickout_when_WritingSameBlock(void)
{
    uint64_t address = 0x7ff0431c;

    CacheData_Read(cache_data, address);

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0, CacheData_Write(cache_data, address), "Write kicked out dirty from the same block");
}

void test_CacheData_Write_should_DirtyKickout_when_ReplacingBlock(void)
{
    uint64_t address1 = 0x7ff0431c;
    uint64_t address2 = address1 + block_size_bytes * n_sets;

    CacheData_Read(cache_data, address1);
    CacheData_Write(cache_data, address1);

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(address1, CacheData_Write(cache_data, address2), "Write didn't perform dirty kickout");
}

void test_CacheData_Insert_should_KickOutDirtyBlock_when_BlockHasBeenWritten(void)
{
    uint64_t address1 = 0x7ff04320;
    uint64_t address2 = address1 + block_size_bytes * n_sets;

    CacheData_Read(cache_data, address1);
    CacheData_Write(cache_data, address1);

    TEST_ASSERT_EQUAL_HEX64(address1, CacheData_Read(cache_data, address2));
}

void test_CacheData_Contains_should_IgnoreOffsetWithinBlock(void)
{
    uint64_t address1 = 0x7ff04324;
    uint64_t address2 = address1 + 1;
    uint64_t address3 = address1 + 3;
    uint64_t address4 = address1 + block_size_bytes - 1;

    CacheData_Read(cache_data, address1);

    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address2), "Offset within block should be ignored");
    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address3), "Offset within block should be ignored");
    TEST_ASSERT_MESSAGE(CacheData_Contains(cache_data, address4), "Offset within block should be ignored");
}

void test_CacheData_Insert_should_BeAbleToFillCache(void)
{
    uint64_t base_address = 0x7000000;

    uint32_t i;
    for (i = 0; i < n_sets; i++) {
        uint64_t block_address = base_address + block_size_bytes * i;
        TEST_ASSERT_EQUAL_HEX64(0, CacheData_Read(cache_data, block_address));

        // So we see a dirty kickout if anything is overwritten
        CacheData_Write(cache_data, block_address);
    }

    uint64_t old_base_address = base_address;
    uint64_t new_base_address = 0x5000000;
    for (i = 0; i < n_sets; i++) {
        uint64_t offset = block_size_bytes * i;
        uint64_t new_block_address = new_base_address + offset;
        uint64_t old_block_address = old_base_address + offset;
        TEST_ASSERT_EQUAL_HEX64(old_block_address, CacheData_Read(cache_data, new_block_address));
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHEDATA */
