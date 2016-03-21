/**
 * @file    unity_AccessHelper.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   UnityAccessHelper Source
 *
 * @addtogroup UNITY_ACCESSHELPER
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity_AccessHelper.h"
#include "unity.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void AssertEqual_access_t(access_t expected,
                          access_t actual,
                          unsigned short line,
                          const char * message)
{
    UNITY_TEST_ASSERT_EQUAL_UINT8(expected.type, actual.type, line, message);
    UNITY_TEST_ASSERT_EQUAL_HEX64(expected.address, actual.address, line, message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.n_bytes, actual.n_bytes, line, message);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup UNITY_ACCESSHELPER */
