/**
 * @file    test_Access.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestAccess Source
 *
 * @addtogroup TEST_ACCESS
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "Access.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"
#include "unity_AccessHelper.h"

#include "test_utilities.h"

#include <stdbool.h>
#include <stdint.h>

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

void test_InstrAccess(void)
{
    access_t expected_access = {
        .type       = TYPE_INSTR,
        .address    = 0x7F81CE2206B0,
        .n_bytes    = 3,
    };

    access_t access;
    ZERO_STRUCT(access);

    Access_ParseLine("I 7f81ce2206b0 3\n", &access);

    TEST_ASSERT_EQUAL_access_t(expected_access, access);
}

void test_WriteAccess(void)
{
    access_t expected_access = {
        .type       = TYPE_WRITE,
        .address    = 0x7FFF5A8487D8,
        .n_bytes    = 8,
    };

    access_t access;
    ZERO_STRUCT(access);

    Access_ParseLine("W 7fff5a8487d8 8\n", &access);

    TEST_ASSERT_EQUAL_access_t(expected_access, access);
}

void test_ReadAccess(void)
{
    access_t expected_access = {
        .type       = TYPE_READ,
        .address    = 0x7F81CE441B80 ,
        .n_bytes    = 8,
    };

    access_t access;
    ZERO_STRUCT(access);

    Access_ParseLine("R 7f81ce441b80 8\n", &access);

    TEST_ASSERT_EQUAL_access_t(expected_access, access);
}

void test_InvalidOperationThrowsException(void)
{
    access_t dummy_access;

    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        Access_ParseLine("D 7f81ce441b80 8\n", &dummy_access);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(INVALID_OPERATION, e, "Should catch invalid operation");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_ACCESS */
