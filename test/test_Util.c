/**
 * @file    test_Util.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestUtil Source
 *
 * @addtogroup TEST_UTIL
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"

#include "Util.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

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

void test_ArrayElementsChar(void)
{
    char arr1[1];
    char arr5[5];
    char arr1024[1024];

    TEST_ASSERT_EQUAL(1, ARRAY_ELEMENTS(arr1));
    TEST_ASSERT_EQUAL(5, ARRAY_ELEMENTS(arr5));
    TEST_ASSERT_EQUAL(1024, ARRAY_ELEMENTS(arr1024));
}

void test_ArrayElementsInt(void)
{
    int arr1[1];
    int arr5[5];
    int arr1024[1024];

    TEST_ASSERT_EQUAL(1, ARRAY_ELEMENTS(arr1));
    TEST_ASSERT_EQUAL(5, ARRAY_ELEMENTS(arr5));
    TEST_ASSERT_EQUAL(1024, ARRAY_ELEMENTS(arr1024));
}

void test_ArrayElementsDouble(void)
{
    double arr1[1];
    double arr5[5];
    double arr1024[1024];

    TEST_ASSERT_EQUAL(1, ARRAY_ELEMENTS(arr1));
    TEST_ASSERT_EQUAL(5, ARRAY_ELEMENTS(arr5));
    TEST_ASSERT_EQUAL(1024, ARRAY_ELEMENTS(arr1024));
}

void test_ArrayElementsPointer(void)
{
    void * arr1[1];
    void * arr5[5];
    void * arr1024[1024];

    TEST_ASSERT_EQUAL(1, ARRAY_ELEMENTS(arr1));
    TEST_ASSERT_EQUAL(5, ARRAY_ELEMENTS(arr5));
    TEST_ASSERT_EQUAL(1024, ARRAY_ELEMENTS(arr1024));
}

void test_ArrayElementsStruct(void)
{
    struct DummyStruct{
        int x;
        double y;
        void * next;
    };

    struct DummyStruct arr1[1];
    struct DummyStruct arr5[5];
    struct DummyStruct arr1024[1024];

    TEST_ASSERT_EQUAL(1, ARRAY_ELEMENTS(arr1));
    TEST_ASSERT_EQUAL(5, ARRAY_ELEMENTS(arr5));
    TEST_ASSERT_EQUAL(1024, ARRAY_ELEMENTS(arr1024));
}

void test_PowersOfTwo(void)
{
    TEST_ASSERT(IS_POWER_OF_TWO(1));
    TEST_ASSERT(IS_POWER_OF_TWO(2));
    TEST_ASSERT(IS_POWER_OF_TWO(4));
    TEST_ASSERT(IS_POWER_OF_TWO(1024));
    TEST_ASSERT(IS_POWER_OF_TWO(32768));
    TEST_ASSERT(IS_POWER_OF_TWO(1 << 30));
}

void test_NotPowersOfTwo(void)
{
    TEST_ASSERT(!IS_POWER_OF_TWO(0));
    TEST_ASSERT(!IS_POWER_OF_TWO(3));
    TEST_ASSERT(!IS_POWER_OF_TWO(12));
    TEST_ASSERT(!IS_POWER_OF_TWO(33));
    TEST_ASSERT(!IS_POWER_OF_TWO(32767));
    TEST_ASSERT(!IS_POWER_OF_TWO((1 << 30) + 1));
}

void test_HighestSetBitIndex_should_ReturnZero_when_PassedZero(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, HighestSetBitIndex(0));
}

void test_HighestSetBitIndex_should_ReturnOnlySetBit_when_PassedPowerOf2(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, HighestSetBitIndex(0x01));
    TEST_ASSERT_EQUAL_UINT32(1, HighestSetBitIndex(0x02));
    TEST_ASSERT_EQUAL_UINT32(2, HighestSetBitIndex(0x04));
    TEST_ASSERT_EQUAL_UINT32(4, HighestSetBitIndex(0x10));
    TEST_ASSERT_EQUAL_UINT32(63, HighestSetBitIndex(0x8000000000000000));
}

void test_HighestSetBitIndex_should_ReturnHighestSetBit_when_PassedNonPowerOf2(void)
{
    TEST_ASSERT_EQUAL_UINT32(1, HighestSetBitIndex(0x03));
    TEST_ASSERT_EQUAL_UINT32(2, HighestSetBitIndex(0x05));
    TEST_ASSERT_EQUAL_UINT32(3, HighestSetBitIndex(0x0F));
    TEST_ASSERT_EQUAL_UINT32(5, HighestSetBitIndex(0x21));
    TEST_ASSERT_EQUAL_UINT32(63, HighestSetBitIndex(0x8000143235990345));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_UTIL */
