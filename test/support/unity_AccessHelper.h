/**
 * @file    unity_AccessHelper.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   UnityAccessHelper Interface
 */

#ifndef UNITY_ACCESSHELPER_H
#define UNITY_ACCESSHELPER_H

/**@defgroup UNITY_ACCESSHELPER UnityAccessHelper
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Access.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

#define UNITY_TEST_ASSERT_EQUAL_access_t(expected, actual, line, message) \
            AssertEqual_access_t(expected, actual, line, message)

#define TEST_ASSERT_EQUAL_access_t(expected, actual) \
            UNITY_TEST_ASSERT_EQUAL_access_t(expected, actual, __LINE__, NULL)

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void AssertEqual_access_t(access_t expected,
                          access_t actual,
                          unsigned short line,
                          const char * message);

/** @} defgroup UNITY_ACCESSHELPER */

#endif /* ifndef UNITY_ACCESSHELPER_H */
