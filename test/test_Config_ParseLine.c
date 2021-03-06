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
#include "unity_Helper.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static void lineShouldCauseException(const char * line, CEXCEPTION_T exception_e, const char * message);

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
            .associativity = 2,
        },
        .l2 = {
            .associativity = 4,
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

void test_ParseMemSendAddressTime(void)
{
    config_t expected_config = {
        .main_mem = {
            .send_address_cycles = 13,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("mem_sendaddr=13\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseMemReadyTime(void)
{
    config_t expected_config = {
        .main_mem = {
            .ready_cycles = 20,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("mem_ready=20\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseMemChunkTime(void)
{
    config_t expected_config = {
        .main_mem = {
            .send_chunk_cycles = 23,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("mem_chunktime=23\n", &config);

    TEST_ASSERT_EQUAL_config_t(expected_config, config);
}

void test_ParseMemChunkSize(void)
{
    config_t expected_config = {
        .main_mem = {
            .chunk_size_bytes = 16,
        },
    };

    config_t config;
    ZERO_STRUCT(config);

    Config_ParseLine("mem_chunksize=16\n", &config);

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

void test_RejectsSyntaxError(void)
{
    lineShouldCauseException("syntax errors all over da place", SYNTAX_ERROR, "Should detect syntax error");
}

void test_RejectsL1InvalidParameters(void)
{
    lineShouldCauseException("L1_transfer_time=3\n", BAD_CONFIG_PARAM, "L1 has no transfer time parameter");
    lineShouldCauseException("L1_bus_width=32\n", BAD_CONFIG_PARAM, "L1 has no bus width parameter");
}

void test_RejetcsInvalidCacheName(void)
{
    lineShouldCauseException("L3_cache_size=1024\n", BAD_CONFIG_CACHE, "There is no L3 cache");
}

void test_RejectsInvalidParameterName(void)
{
    lineShouldCauseException("L1_doesnt_exist=1024\n", BAD_CONFIG_PARAM, "Should catch doesnt_exist parameter");
}

void test_ThrowsExceptionWithNullPointers(void)
{
    lineShouldCauseException(NULL, ARGUMENT_ERROR, "Should catch null string");

    // Broken out specially because we need to pass a null config
    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        Config_ParseLine("L1_hit_time=1\n", NULL);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(ARGUMENT_ERROR, e, "Should catch null config struct");
}

void test_WidthsMustBePowersOfTwo(void)
{
    lineShouldCauseException("L1_block_size=31\n", BAD_CONFIG_VALUE, "Should catch non power-of-two block size");
    lineShouldCauseException("L2_cache_size=1000\n", BAD_CONFIG_VALUE, "Should catch non power-of-two cache size");
    lineShouldCauseException("L1_assoc=5\n", BAD_CONFIG_VALUE, "Should catch non power-of-two associative value");
    lineShouldCauseException("L2_bus_width=9\n", BAD_CONFIG_VALUE, "Should catch non power-of-two bus width");
    lineShouldCauseException("mem_chunksize=15\n", BAD_CONFIG_VALUE, "Should catch non power-of-two chunk size");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void lineShouldCauseException(const char * line, CEXCEPTION_T expected_e, const char * message)
{
    config_t dummy_config;

    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        Config_ParseLine(line, &dummy_config);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected_e, e, message);
}

/** @} addtogroup TEST_CONFIG */
