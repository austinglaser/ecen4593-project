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

/** @} addtogroup TEST_CACHESET_TWOWAY */
