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
#include "unity_ConfigHelper.h"

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
            .associative_bytes      = 1,
            .hit_time_cycles        = 1,
            .miss_time_cycles       = 1,
        },
        .l2 = {
            .block_size_bytes       = 64,
            .cache_size_bytes       = 32768,
            .associative_bytes      = 1,
            .hit_time_cycles        = 8,
            .miss_time_cycles       = 10,
            .transfer_time_cycles   = 10,
            .bus_width_bytes        = 16,
        },
    };

    config_t default_config;
    ZERO_STRUCT(default_config);

    Config_Defaults(&default_config);

    TEST_ASSERT_EQUAL_config_t(expected_config, default_config);
}

void test_BlankLineDoesntModifyConfig(void)
{
    config_t expected_config;
    ZERO_STRUCT(expected_config);

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseBlockSizeString(void)
{
    config_t expected_config = {
        .l1 = {
            .block_size_bytes = 128,
        },
        .l2 = {
            .block_size_bytes = 32,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L1_block_size=128\n", &config);
    Config_ParseLine("L2_block_size=32\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseCacheSizeString(void)
{
    config_t expected_config = {
        .l1 = {
            .cache_size_bytes = 1024,
        },
        .l2 = {
            .cache_size_bytes = 1024,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L1_cache_size=1024\n", &config);
    Config_ParseLine("L2_cache_size=1024\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseAssociativeString(void)
{
    config_t expected_config = {
        .l1 = {
            .associative_bytes = 2,
        },
        .l2 = {
            .associative_bytes = 4,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L1_assoc=2\n", &config);
    Config_ParseLine("L2_assoc=4\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseHitTime(void)
{
    config_t expected_config = {
        .l1 = {
            .hit_time_cycles = 2,
        },
        .l2 = {
            .hit_time_cycles = 12,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L1_hit_time=2\n", &config);
    Config_ParseLine("L2_hit_time=12\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseMissTime(void)
{
    config_t expected_config = {
        .l1 = {
            .miss_time_cycles = 2,
        },
        .l2 = {
            .miss_time_cycles = 12,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L1_miss_time=2\n", &config);
    Config_ParseLine("L2_miss_time=12\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseTransferTime(void)
{
    config_t expected_config = {
        .l2 = {
            .transfer_time_cycles = 12,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L2_transfer_time=12\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseBusWidth(void)
{
    config_t expected_config = {
        .l2 = {
            .bus_width_bytes = 32,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L2_bus_width=32\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CONFIG */
