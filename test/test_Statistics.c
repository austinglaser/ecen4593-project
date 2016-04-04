/**
 * @file    test_Statistics.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestStatistics Source
 *
 * @addtogroup TEST_STATISTICS
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Statistics.h"
#include "unity.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static stats_t stats;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    Statistics_Create(&stats);
}

void tearDown(void)
{
}

void test_Statistics_Create_should_InitializeAllStatFields(void)
{
    memset(&stats, 0xFF, sizeof(stats));

    Statistics_Create(&stats);

    TEST_ASSERT_EQUAL_UINT64(0, stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_cycles);

    TEST_ASSERT_EQUAL_STRING("L1i", stats.l1i.name);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.hit_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.vc_hit_count);

    TEST_ASSERT_EQUAL_STRING("L1d", stats.l1d.name);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.hit_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.vc_hit_count);

    TEST_ASSERT_EQUAL_STRING("L2", stats.l2.name);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.hit_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.vc_hit_count);
}

void test_Statistics_RecordAccess_should_RecordReadAccess(void)
{
    access_t access = {
        .type       = TYPE_READ,
        .address    = 0x0000,
        .n_bytes    = 4,
    };

    uint32_t cycles = 38;
    Statistics_RecordAccess(&stats, &access, cycles);

    TEST_ASSERT_EQUAL_UINT64(1,      stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(cycles, stats.read_cycles);

    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_cycles);
}

void test_Statistics_RecordAccess_should_RecordWriteAccess(void)
{
    access_t access = {
        .type       = TYPE_WRITE,
        .address    = 0xfcddfbc,
        .n_bytes    = 4,
    };

    uint32_t cycles = 21;
    Statistics_RecordAccess(&stats, &access, cycles);

    TEST_ASSERT_EQUAL_UINT64(1,      stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(cycles, stats.write_cycles);

    TEST_ASSERT_EQUAL_UINT64(0, stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_cycles);
}

void test_Statistics_RecordAccess_should_RecordInstructionAccess(void)
{
    access_t access = {
        .type       = TYPE_INSTR,
        .address    = 0xfcddfb4,
        .n_bytes    = 4,
    };

    uint32_t cycles = 111;
    Statistics_RecordAccess(&stats, &access, cycles);

    TEST_ASSERT_EQUAL_UINT64(1,      stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(cycles, stats.instr_cycles);

    TEST_ASSERT_EQUAL_UINT64(0, stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_cycles);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_STATISTICS */
