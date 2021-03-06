/**
 * @file    test_Config_FromFile.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestConfigFromFile Source
 *
 * @addtogroup TEST_CONFIG_FROMFILE
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
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
}

void tearDown(void)
{
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
    Config_FromFile("test/support/empty", &actual_config);

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
    Config_FromFile("test/support/l2_hit_time_10", &actual_config);

    TEST_ASSERT_EQUAL_config_t(expected_config, actual_config);
}

void test_ExceptionPointsToBadConfigLine(void)
{
    CEXCEPTION_T e;

    config_t dummy_config;

    const char * bad_config_file = "test/support/config_with_error_line_3";

    Try {
        Config_FromFile(bad_config_file, &dummy_config);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_MESSAGE(BAD_CONFIG_PARAM, e, "Should have caught bad config file");
    TEST_ASSERT_EQUAL_STRING(bad_config_file, exception_file);
    TEST_ASSERT_EQUAL(3, exception_line);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_CONFIG_FROMFILE */
