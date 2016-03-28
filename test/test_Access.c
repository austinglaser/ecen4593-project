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

static void shouldCauseException(const char * line, access_t * access, CEXCEPTION_T expected_e, const char * message);

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

void test_NewlineDoesntMatter(void)
{
    access_t expected_access = {
        .type       = TYPE_READ,
        .address    = 0x7F81CE441B80 ,
        .n_bytes    = 8,
    };

    access_t access;
    ZERO_STRUCT(access);

    Access_ParseLine("R 7f81ce441b80 8", &access);

    TEST_ASSERT_EQUAL_access_t(expected_access, access);
}

void test_NullPointerThrowsException(void)
{
    access_t dummy_access;
    shouldCauseException(NULL,
                         &dummy_access,
                         ARGUMENT_ERROR,
                         "No exception on null line");
    shouldCauseException("R 7f81ce441b80 8\n",
                         NULL,
                         ARGUMENT_ERROR,
                         "No exception on null access struct");
}

void test_InvalidOperationThrowsException(void)
{
    access_t dummy_access;
    shouldCauseException("D 7f81ce441b80 8\n",
                         &dummy_access,
                         INVALID_OPERATION,
                         "No exception on invalid operation");
}

void test_InvalidAccessSizeThrowsException(void)
{
    access_t dummy_access;
    shouldCauseException("I 7f81ce441b80 9\n",
                         &dummy_access,
                         INVALID_ACCESS_SIZE,
                         "No exception on invalid access size");
}

void test_SyntaxErrorThrowsException(void)
{
    access_t dummy_access;
    shouldCauseException("can't even handle all these syntax errors",
                         &dummy_access,
                         SYNTAX_ERROR,
                         "No exception on invalid syntax");
    shouldCauseException("R 948nothexffc 10",
                         &dummy_access,
                         SYNTAX_ERROR,
                         "No exception on invalid syntax");
    shouldCauseException("R 7fff5a8487d8 woooord",
                         &dummy_access,
                         SYNTAX_ERROR,
                         "No exception on invalid syntax");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void shouldCauseException(const char * line, access_t * access, CEXCEPTION_T expected_e, const char * message)
{
    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        Access_ParseLine(line, access);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected_e, e, message);
}

/** @} addtogroup TEST_ACCESS */
