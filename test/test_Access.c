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

static void lineShouldCauseException(const char * line, CEXCEPTION_T expected_e, const char * message);

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
    lineShouldCauseException(NULL, ARGUMENT_ERROR, "Should catch null line");

    // Explicit because we need to pass null pointer
    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        Access_ParseLine("R 7f81ce441b80 8\n", NULL);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(ARGUMENT_ERROR, e, "Should catch null access struct");
}

void test_InvalidOperationThrowsException(void)
{
    lineShouldCauseException("D 7f81ce441b80 8\n", INVALID_OPERATION, "Should catch invalid operation");
}

void test_SyntaxErrorThrowsException(void)
{
    lineShouldCauseException("can't even handle all these syntax errors", SYNTAX_ERROR, "Should catch invalid syntax");
    lineShouldCauseException("R 948nothexffc 10", SYNTAX_ERROR, "Should catch invalid syntax");
    lineShouldCauseException("R 7fff5a8487d8 woooord", SYNTAX_ERROR, "Should catch invalid syntax");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void lineShouldCauseException(const char * line, CEXCEPTION_T expected_e, const char * message)
{
    access_t dummy_access;

    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        Access_ParseLine(line, &dummy_access);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected_e, e, message);
}

/** @} addtogroup TEST_ACCESS */
