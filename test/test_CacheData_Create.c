/**
 * @file    test_CacheData_Create.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestCacheDataCreate Source
 *
 * @addtogroup TEST_CACHEDATA_CREATE
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
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void test_CacheData_Create_should_ReturnNonNull_when_GivenValidParameters(void)
{
    cache_data_t cache_data;

    cache_data = CacheData_Create(8, 4, 4, 0);
    TEST_ASSERT_NOT_NULL(cache_data);
    CacheData_Destroy(cache_data);

    cache_data = CacheData_Create(1, 4, 4, 0);
    TEST_ASSERT_NOT_NULL(cache_data);
    CacheData_Destroy(cache_data);

    cache_data = CacheData_Create(8, 1, 4, 0);
    TEST_ASSERT_NOT_NULL(cache_data);
    CacheData_Destroy(cache_data);
}

void test_CacheData_Create_shouild_ReturnNull_when_GivenNonPowerOfTwoNumberOfSets(void)
{
    cache_data_t cache_data = CacheData_Create(3, 4, 4, 0);
    TEST_ASSERT_NULL(cache_data);
}

void test_CacheData_Create_shouild_ReturnNull_when_GivenNonPowerOfTwoSetLength(void)
{
    cache_data_t cache_data = CacheData_Create(8, 3, 4, 0);
    TEST_ASSERT_NULL(cache_data);
}

void test_CacheData_Create_shouild_ReturnNull_when_GivenNonPowerOfTwoBlockSize(void)
{
    cache_data_t cache_data = CacheData_Create(8, 1, 5, 0);
    TEST_ASSERT_NULL(cache_data);
}

void test_CacheData_Create_shouild_ReturnNull_when_GivenBlockSizeSmallerThanAWord(void)
{
    cache_data_t cache_data = CacheData_Create(8, 1, 2, 0);
    TEST_ASSERT_NULL(cache_data);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHEDATA_CREATE */
