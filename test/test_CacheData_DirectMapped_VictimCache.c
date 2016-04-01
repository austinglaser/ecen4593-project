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

static const uint32_t n_sets           = 16;
static const uint32_t victim_cache_len = 8;
static const uint32_t block_size_bytes = 4;
static cache_data_t cache_data;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    cache_data = CacheData_Create(n_sets, 1, block_size_bytes, 0);
}

void tearDown(void)
{
    CacheData_Destroy(cache_data);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CACHEDATA_DIRECTMAPPED_VICTIMCACHE */
