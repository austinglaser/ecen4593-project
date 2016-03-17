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
#include "CException.h"

#include "Config.h"

#include "test_utilities.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */

#define TEST_ASSERT_EQUAL_CONFIG(expected, actual)                           \
    do {                                                                     \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l1.block_size_bytes,     \
                                         (actual).l1.block_size_bytes,       \
                                         "l1.block_size_bytes");             \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l1.cache_size_bytes,     \
                                         (actual).l1.cache_size_bytes,       \
                                         "l1.cache_size_bytes");             \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l1.associative,          \
                                         (actual).l1.associative,            \
                                         "l1.associative");                  \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l1.hit_time_cycles,      \
                                         (actual).l1.hit_time_cycles,        \
                                         "l1.hit_time_cycles");              \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l1.miss_time_cycles,     \
                                         (actual).l1.miss_time_cycles,       \
                                         "l1.miss_time_cycles");             \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.block_size_bytes,     \
                                         (actual).l2.block_size_bytes,       \
                                         "l2.block_size_bytes");             \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.cache_size_bytes,     \
                                         (actual).l2.cache_size_bytes,       \
                                         "l2.cache_size_bytes");             \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.associative,          \
                                         (actual).l2.associative,            \
                                         "l2.associative");                  \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.hit_time_cycles,      \
                                         (actual).l2.hit_time_cycles,        \
                                         "l2.hit_time_cycles");              \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.miss_time_cycles,     \
                                         (actual).l2.miss_time_cycles,       \
                                         "l2.miss_time_cycles");             \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.transfer_time_cycles, \
                                         (actual).l2.transfer_time_cycles,   \
                                         "l2.transfer_time_cycles");         \
        TEST_ASSERT_EQUAL_UINT32_MESSAGE((expected).l2.bus_width_bytes,      \
                                         (actual).l2.bus_width_bytes,        \
                                         "l2.bus_width_bytes");              \
    } while (false)

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
            .associative            = 1,
            .hit_time_cycles        = 1,
            .miss_time_cycles       = 1,
        },
        .l2 = {
            .block_size_bytes       = 64,
            .cache_size_bytes       = 32768,
            .associative            = 1,
            .hit_time_cycles        = 8,
            .miss_time_cycles       = 10,
            .transfer_time_cycles   = 10,
            .bus_width_bytes        = 16,
        },
    };

    config_t default_config;
    ZERO_STRUCT(default_config);

    Config_Defaults(&default_config);

    TEST_ASSERT_EQUAL_CONFIG(expected_config, default_config);
}

void test_BlankLineDoesntModifyConfig(void)
{
    config_t expected_config;
    ZERO_STRUCT(expected_config);

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("\n", &config);

    TEST_ASSERT_EQUAL_CONFIG(expected_config, config);
}

void test_ParseBlockSizeString(void)
{
    config_t expected_config = {
        .l1 = {
            .block_size_bytes = 32,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L1_block_size=32\n", &config);

    TEST_ASSERT_EQUAL_CONFIG(expected_config, config);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CONFIG */
