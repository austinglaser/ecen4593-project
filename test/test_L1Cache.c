/**
 * @file    test_L1DirectMapped.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestL1DirectMapped Source
 *
 * @addtogroup TEST_L1DIRECTMAPPED
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "L1Cache.h"

#include "Config.h"
#include "Access.h"
#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"
#include "ConfigDefaults.h"

#include "mock_L2Cache.h"
#include "mock_CacheData.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static l1_cache_t l1_cache;
static l2_cache_t dummy_l2_cache;
static config_t config;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);
    l1_cache = L1Cache_Create(dummy_l2_cache, &config);
}

void tearDown(void)
{
    L1Cache_Destroy(l1_cache);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L1DIRECTMAPPED */
