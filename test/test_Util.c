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

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static void BlockAlignmentMask_ShouldThrowExceptionWithInput(CEXCEPTION_T expected_e,
                                                             uint32_t block_size,
                                                             const char * message);

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

void test_CeilDivide_should_BeRegularDivision_when_DivisorIsDivisible(void)
{
    TEST_ASSERT_EQUAL(1,    CEIL_DIVIDE(3, 3));
    TEST_ASSERT_EQUAL(2,    CEIL_DIVIDE(24, 12));
    TEST_ASSERT_EQUAL(5,    CEIL_DIVIDE(5, 1));
    TEST_ASSERT_EQUAL(321,  CEIL_DIVIDE(642, 2));
}

void test_CeilDivide_should_ReturnZero_when_DividentIsZero(void)
{
    TEST_ASSERT_EQUAL(0, CEIL_DIVIDE(0, 1));
    TEST_ASSERT_EQUAL(0, CEIL_DIVIDE(0, 12));
    TEST_ASSERT_EQUAL(0, CEIL_DIVIDE(0, INT_MAX));
}

void test_CeilDivide_should_RoundUp_when_DivisorIsNotDivisible(void)
{
    TEST_ASSERT_EQUAL(2, CEIL_DIVIDE(4, 3));
    TEST_ASSERT_EQUAL(3, CEIL_DIVIDE(25, 12));
    TEST_ASSERT_EQUAL(3, CEIL_DIVIDE(35, 12));
    TEST_ASSERT_EQUAL(4, CEIL_DIVIDE(37, 12));
}

void test_HighestBitSet_Uint64_should_ReturnZero_when_PassedZero(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, HighestBitSet_Uint64(0));
}

void test_HighestBitSet_Uint64_should_ReturnOnlySetBit_when_PassedPowerOf2(void)
{
    TEST_ASSERT_EQUAL_UINT32(0, HighestBitSet_Uint64(0x01));
    TEST_ASSERT_EQUAL_UINT32(1, HighestBitSet_Uint64(0x02));
    TEST_ASSERT_EQUAL_UINT32(2, HighestBitSet_Uint64(0x04));
    TEST_ASSERT_EQUAL_UINT32(4, HighestBitSet_Uint64(0x10));
    TEST_ASSERT_EQUAL_UINT32(63, HighestBitSet_Uint64(0x8000000000000000));
}

void test_HighestBitSet_Uint64_should_ReturnHighestSetBit_when_PassedNonPowerOf2(void)
{
    TEST_ASSERT_EQUAL_UINT32(1, HighestBitSet_Uint64(0x03));
    TEST_ASSERT_EQUAL_UINT32(2, HighestBitSet_Uint64(0x05));
    TEST_ASSERT_EQUAL_UINT32(3, HighestBitSet_Uint64(0x0F));
    TEST_ASSERT_EQUAL_UINT32(5, HighestBitSet_Uint64(0x21));
    TEST_ASSERT_EQUAL_UINT32(63, HighestBitSet_Uint64(0x8000143235990345));
}

void test_BlockAlignmentMask_should_CalculateMask_when_BlockSizeIsAPowerOfTwo(void)
{
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFF, BlockAlignmentMask(1));
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFE, BlockAlignmentMask(2));
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFFC, BlockAlignmentMask(4));
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFF8, BlockAlignmentMask(8));
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFF0, BlockAlignmentMask(16));
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFFFFFFFFC0, BlockAlignmentMask(64));
    TEST_ASSERT_EQUAL_HEX64(0xFFFFFFFF80000000, BlockAlignmentMask(UINT32_C(1) << 31));
}

void test_BlockAlignmentMask_should_ThrowException_when_BlockSizeIsNotAPowerOfTwo(void)
{
    BlockAlignmentMask_ShouldThrowExceptionWithInput(ARGUMENT_ERROR, 0, "Failed to catch non-power-of-two block size");
    BlockAlignmentMask_ShouldThrowExceptionWithInput(ARGUMENT_ERROR, 3, "Failed to catch non-power-of-two block size");
    BlockAlignmentMask_ShouldThrowExceptionWithInput(ARGUMENT_ERROR, 5, "Failed to catch non-power-of-two block size");
    BlockAlignmentMask_ShouldThrowExceptionWithInput(ARGUMENT_ERROR, 7, "Failed to catch non-power-of-two block size");
    BlockAlignmentMask_ShouldThrowExceptionWithInput(ARGUMENT_ERROR, (UINT32_C(1) << 31) + 1, "Failed to catch non-power-of-two block size");
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void BlockAlignmentMask_ShouldThrowExceptionWithInput(CEXCEPTION_T expected_e, uint32_t block_size, const char * message)
{
    CEXCEPTION_T e = CEXCEPTION_NONE;
    Try {
        BlockAlignmentMask(block_size);
    }
    Catch (e) {
    }
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected_e, e, message);
}

/** @} addtogroup TEST_UTIL */
