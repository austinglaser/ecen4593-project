/**
 * @file    test_CacheSet_Create.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheSetCreate Source
 *
 * @addtogroup TEST_CACHESET_CREATE
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
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void test_CacheSet_Create_should_ReturnNonNull_when_GivenValidParameters(void)
{
    cache_sets_t cache_sets;

    cache_sets = CacheSet_Create_Sets(8, 4, 4);
    TEST_ASSERT_NOT_NULL(cache_sets);
    CacheSet_Destroy_Sets(cache_sets);

    cache_sets = CacheSet_Create_Sets(1, 4, 4);
    TEST_ASSERT_NOT_NULL(cache_sets);
    CacheSet_Destroy_Sets(cache_sets);

    cache_sets = CacheSet_Create_Sets(8, 1, 4);
    TEST_ASSERT_NOT_NULL(cache_sets);
    CacheSet_Destroy_Sets(cache_sets);
}

void test_CacheSet_Create_shouild_ReturnNull_when_GivenNonPowerOfTwoNumberOfSets(void)
{
    cache_sets_t cache_sets = CacheSet_Create_Sets(3, 4, 4);
    TEST_ASSERT_NULL(cache_sets);
}

void test_CacheSet_Create_shouild_ReturnNull_when_GivenNonPowerOfTwoSetLength(void)
{
    cache_sets_t cache_sets = CacheSet_Create_Sets(8, 3, 4);
    TEST_ASSERT_NULL(cache_sets);
}

void test_CacheSet_Create_shouild_ReturnNull_when_GivenNonPowerOfTwoBlockSize(void)
{
    cache_sets_t cache_sets = CacheSet_Create_Sets(8, 1, 5);
    TEST_ASSERT_NULL(cache_sets);
}

void test_CacheSet_Create_shouild_ReturnNull_when_GivenBlockSizeSmallerThanAWord(void)
{
    cache_sets_t cache_sets = CacheSet_Create_Sets(8, 1, 2);
    TEST_ASSERT_NULL(cache_sets);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHESET_CREATE */
