/**
 * @file    test_CacheSet_TwoWay.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheSetTwoWay Source
 *
 * @addtogroup TEST_CACHESET_TWOWAY
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
static const uint32_t set_len          = 2;
static const uint32_t block_size_bytes = 4;
static cache_sets_t cache_sets;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

void setUp(void)
{
    cache_sets = CacheSet_Create_Sets(n_sets, set_len, block_size_bytes);
}

void tearDown(void)
{
    CacheSet_Destroy_Sets(cache_sets);
}

void test_CacheSet_Insert_should_SaveTwoAddressesMappedToTheSameSet(void)
{
    uint64_t address1 = 0x7ff38200;
    uint64_t address2 = 0x7ff38300;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Insert(cache_sets, address2);

    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address1), "Failed to store first address");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address2), "Failed to store second address");
}

void test_CacheSet_Insert_should_KickOutOldestAddressFromSet(void)
{
    uint64_t address1 = 0x7ff38200;
    uint64_t address2 = 0x7ff38300;
    uint64_t address3 = 0x7ff38400;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Insert(cache_sets, address2);
    CacheSet_Insert(cache_sets, address3);

    TEST_ASSERT_FALSE_MESSAGE(CacheSet_Contains(cache_sets, address1), "Failed to kick out first address");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address2), "Failed to store second address");
    TEST_ASSERT_MESSAGE(CacheSet_Contains(cache_sets, address3), "Failed to store second address");
}

void test_CacheSet_Write_should_CauseDirtyKickout(void)
{
    uint64_t address1 = 0x7ff38200;
    uint64_t address2 = 0x7ff38300;
    uint64_t address3 = 0x7ff38400;

    CacheSet_Insert(cache_sets, address1);
    CacheSet_Insert(cache_sets, address2);

    CacheSet_Write(cache_sets, address1);
    TEST_ASSERT_EQUAL_HEX64(address1, CacheSet_Insert(cache_sets, address3));
}

void test_CacheSet_Insert_should_BeAbleToFillCache(void)
{
    uint64_t base_address = 0x7000000;

    uint32_t i;
    for (i = 0; i < n_sets; i++) {
        uint32_t j;
        for (j = 0; j < set_len; j++) {
            uint64_t block_address = (base_address) +
                                     (i * block_size_bytes) +
                                     (j * block_size_bytes * n_sets / set_len);
            TEST_ASSERT_EQUAL_HEX64(0, CacheSet_Insert(cache_sets, block_address));

            // So we see a dirty kickout if anything is overwritten
            CacheSet_Write(cache_sets, block_address);
        }
    }

    uint64_t old_base_address = base_address;
    uint64_t new_base_address = 0x5000000;
    for (i = 0; i < n_sets; i++) {
        uint64_t offset = i * block_size_bytes;
        uint64_t new_block_address = new_base_address + offset;
        uint64_t old_block_address = old_base_address + offset;
        TEST_ASSERT_EQUAL_HEX64(old_block_address, CacheSet_Insert(cache_sets, new_block_address));
    }
}

/** @} addtogroup TEST_CACHESET_TWOWAY */
