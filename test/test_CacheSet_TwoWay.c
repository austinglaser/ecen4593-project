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

/** @} addtogroup TEST_CACHESET_TWOWAY */
