/**
 * @file    test_Config.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Config Test
 *
 * @addtogroup TEST_CONFIG
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"

#include "Config.h"

#include "test_utilities.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
}

void tearDown(void)
{
}

void test_ConfigDefaultValues(void)
{
    config_t expected_config = {
        .l1 = {
            .block_size_bytes       = 32,
            .cache_size_bytes       = 8192,
            .is_associative         = true,
            .hit_time_cycles        = 1,
            .miss_time_cycles       = 1,
        },
        .l2 = {
            .block_size_bytes       = 64,
            .cache_size_bytes       = 32768,
            .is_associative         = true,
            .hit_time_cycles        = 8,
            .miss_time_cycles       = 10,
            .transfer_time_cycles   = 10,
            .bus_width_bytes        = 16,
        },
    };

    config_t default_config;
    ZERO_STRUCT(default_config);

    Config_Defaults(&default_config);

    TEST_ASSERT_EQUAL_UINT32(expected_config.l1.block_size_bytes,       default_config.l1.block_size_bytes);
    TEST_ASSERT_EQUAL_UINT32(expected_config.l1.cache_size_bytes,       default_config.l1.cache_size_bytes);
    TEST_ASSERT_EQUAL_UINT32(expected_config.l1.cache_size_bytes,       default_config.l1.cache_size_bytes);
    TEST_ASSERT_MESSAGE(expected_config.l1.is_associative            == default_config.l1.is_associative, "Default L1 associative value wrong");
    TEST_ASSERT_EQUAL_UINT16(expected_config.l1.hit_time_cycles,        default_config.l1.hit_time_cycles);
    TEST_ASSERT_EQUAL_UINT16(expected_config.l1.miss_time_cycles,       default_config.l1.miss_time_cycles);

    TEST_ASSERT_EQUAL_UINT32(expected_config.l2.block_size_bytes,       default_config.l2.block_size_bytes);
    TEST_ASSERT_EQUAL_UINT32(expected_config.l2.cache_size_bytes,       default_config.l2.cache_size_bytes);
    TEST_ASSERT_EQUAL_UINT32(expected_config.l2.cache_size_bytes,       default_config.l2.cache_size_bytes);
    TEST_ASSERT_MESSAGE(expected_config.l2.is_associative            == default_config.l2.is_associative, "Default L2 associative value wrong");
    TEST_ASSERT_EQUAL_UINT16(expected_config.l2.hit_time_cycles,        default_config.l2.hit_time_cycles);
    TEST_ASSERT_EQUAL_UINT16(expected_config.l2.miss_time_cycles,       default_config.l2.miss_time_cycles);
    TEST_ASSERT_EQUAL_UINT16(expected_config.l2.transfer_time_cycles,   default_config.l2.transfer_time_cycles);
    TEST_ASSERT_EQUAL_UINT16(expected_config.l2.bus_width_bytes,        default_config.l2.bus_width_bytes);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CONFIG */
