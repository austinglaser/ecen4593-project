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
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include "Config.h"
#include "ConfigDefaults.h"

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
    config_t expected_config;
    SetDefaultConfigValues(&expected_config);

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

void test_NewlineDoesntMatter(void)
{
    config_t expected_config = {
        .l2 = {
            .bus_width_bytes = 32,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("L2_bus_width=32", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_RejectsL1InvalidParameters(void)
{
    CEXCEPTION_T e = CEXCEPTION_NONE;

    config_t config;

    Try {
        Config_ParseLine("L1_transfer_time=3\n", &config);
    }
    Catch (e) {
    }
    TEST_ASSERT_MESSAGE(e == BAD_CONFIG_PARAM, "L1 has no transfer time parameter");

    e = CEXCEPTION_NONE;
    Try {
        Config_ParseLine("L1_bus_width=32\n", &config);
    }
    Catch (e) {
    }
    TEST_ASSERT_MESSAGE(e == BAD_CONFIG_PARAM, "L1 has no bus width parameter");
}

void test_RejetcsInvalidCacheName(void)
{
    CEXCEPTION_T e = CEXCEPTION_NONE;

    config_t config;

    Try {
        Config_ParseLine("L3_cache_size=1024\n", &config);
    }
    Catch (e) {
    }
    TEST_ASSERT_MESSAGE(e == BAD_CONFIG_CACHE, "There is no L3 cache");
}

void test_RejectsInvalidParameterName(void)
{
    CEXCEPTION_T e = CEXCEPTION_NONE;

    config_t config;

    Try {
        Config_ParseLine("L1_doesnt_exist=1024\n", &config);
    }
    Catch (e) {
    }
    TEST_ASSERT_MESSAGE(e == BAD_CONFIG_PARAM, "Should catch doesnt_exist parameter");
}

void test_ThrowsExceptionWithNullPointers(void)
{
    CEXCEPTION_T e = CEXCEPTION_NONE;

    config_t config;

    Try {
        Config_ParseLine(NULL, &config);
    }
    Catch (e) {
    }
    TEST_ASSERT_MESSAGE(e == ARGUMENT_ERROR, "Should catch null string");

    Try {
        Config_ParseLine("L1_hit_time=1\n", NULL);
    }
    Catch (e) {
    }
    TEST_ASSERT_MESSAGE(e == ARGUMENT_ERROR, "Should catch null config struct");
}

void test_SetsDefaultsWithoutConfigFile(void)
{
    config_t expected_config;
    SetDefaultConfigValues(&expected_config);

    config_t actual_config;
    Config_FromFile(NULL, &actual_config);

    TEST_ASSERT_EQUAL_config_t(expected_config, actual_config);
}

void test_SetsDefaultsWithoutEmptyConfigFile(void)
{
    config_t expected_config;
    SetDefaultConfigValues(&expected_config);

    config_t actual_config;
    Config_FromFile("config/empty", &actual_config);

    TEST_ASSERT_EQUAL_config_t(expected_config, actual_config);
}

void test_ThrowsExceptionWithNonexistentConfigFile(void)
{
    CEXCEPTION_T e;

    config_t dummy_config;

    Try {
        Config_FromFile("doesnt/exist/at/all", &dummy_config);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_MESSAGE(BAD_CONFIG_FILE, e, "Should have caught nonexistent config file");
}

void test_AnyConfigFileOptionsOverrideDefaults(void)
{
    config_t expected_config;
    SetDefaultConfigValues(&expected_config);
    expected_config.l2.hit_time_cycles = 10;

    config_t actual_config;
    Config_FromFile("config/l2_hit_time_10", &actual_config);

    TEST_ASSERT_EQUAL_config_t(expected_config, actual_config);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CONFIG */
