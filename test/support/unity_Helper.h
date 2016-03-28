/**
 * @file    unity_Helper.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   UnityHelper Interface
 */

#ifndef UNITY_HELPER_H
#define UNITY_HELPER_H

/**@defgroup UNITY_HELPER UnityHelper
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Config.h"
#include "Access.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

#define UNITY_TEST_ASSERT_EQUAL_config_t(expected, actual, line, message) \
            AssertEqual_config_t(expected, actual, line, message)

#define TEST_ASSERT_EQUAL_config_t(expected, actual) \
            UNITY_TEST_ASSERT_EQUAL_config_t(expected, actual, __LINE__, NULL)


#define UNITY_TEST_ASSERT_EQUAL_cache_param_t(expected, actual, line, message) \
            AssertEqual_cache_param_t(expected, actual, line, message)

#define TEST_ASSERT_EQUAL_cache_param_t(expected, actual) \
            UNITY_TEST_ASSERT_EQUAL_cache_param_t(expected, actual, __LINE__, NULL)


#define UNITY_TEST_ASSERT_EQUAL_memory_param_t(expected, actual, line, message) \
            AssertEqual_memory_param_t(expected, actual, line, message)

#define TEST_ASSERT_EQUAL_memory_param_t(expected, actual) \
            UNITY_TEST_ASSERT_EQUAL_memory_param_t(expected, actual, __LINE__, NULL)


#define UNITY_TEST_ASSERT_EQUAL_access_t(expected, actual, line, message) \
            AssertEqual_access_t(expected, actual, line, message)

#define TEST_ASSERT_EQUAL_access_t(expected, actual) \
            UNITY_TEST_ASSERT_EQUAL_access_t(expected, actual, __LINE__, NULL)

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void AssertEqual_config_t(config_t expected,
                          config_t actual,
                          unsigned short line,
                          const char * message);

void AssertEqual_cache_param_t(cache_param_t expected,
                               cache_param_t actual,
                               unsigned short line,
                               const char * message);

void AssertEqual_memory_param_t(memory_param_t expected,
                                memory_param_t actual,
                                unsigned short line,
                                const char * message);

void AssertEqual_access_t(access_t expected,
                          access_t actual,
                          unsigned short line,
                          const char * message);

/** @} defgroup UNITY_HELPER */

#endif /* ifndef UNITY_HELPER_H */
