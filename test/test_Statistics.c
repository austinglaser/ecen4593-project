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
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count_aligned);
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
    uint32_t cycles = 38;
    Statistics_RecordAccess(&stats, TYPE_READ, cycles, 1);

    TEST_ASSERT_EQUAL_UINT64(1,      stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(1,      stats.read_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles, stats.read_cycles);

    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_cycles);
}

void test_Statistics_RecordAccess_should_RecordWriteAccess(void)
{
    uint32_t cycles = 21;
    Statistics_RecordAccess(&stats, TYPE_WRITE, cycles, 1);

    TEST_ASSERT_EQUAL_UINT64(1,      stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(1,      stats.write_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles, stats.write_cycles);

    TEST_ASSERT_EQUAL_UINT64(0, stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.instr_cycles);
}

void test_Statistics_RecordAccess_should_RecordInstructionAccess(void)
{
    uint32_t cycles = 111;
    Statistics_RecordAccess(&stats, TYPE_INSTR, cycles, 1);

    TEST_ASSERT_EQUAL_UINT64(1,      stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(1,      stats.instr_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles, stats.instr_cycles);

    TEST_ASSERT_EQUAL_UINT64(0, stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.read_cycles);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.write_cycles);
}

void test_Statistics_RecordAccess_should_RecordMisalignedAccess(void)
{
    uint8_t type;
    uint32_t cycles;
    uint32_t n_aligned;

    type      = TYPE_READ;
    cycles    = 38;
    n_aligned = 2;
    Statistics_RecordAccess(&stats, type, cycles, n_aligned);

    TEST_ASSERT_EQUAL_UINT64(1,         stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(n_aligned, stats.read_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles,    stats.read_cycles);

    type       = TYPE_WRITE;
    cycles     = 118;
    n_aligned  = 3;
    Statistics_RecordAccess(&stats, type, cycles, n_aligned);

    TEST_ASSERT_EQUAL_UINT64(1,         stats.write_count);
    TEST_ASSERT_EQUAL_UINT64(n_aligned, stats.write_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles,    stats.write_cycles);

    type       = TYPE_INSTR;
    cycles     = 21;
    n_aligned  = 2;
    Statistics_RecordAccess(&stats, type, cycles, n_aligned);

    TEST_ASSERT_EQUAL_UINT64(1,         stats.instr_count);
    TEST_ASSERT_EQUAL_UINT64(n_aligned, stats.instr_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles,    stats.instr_cycles);
}

void test_Statistics_RecordAccess_should_RecordMultipleAccesses(void)
{
    uint32_t cycles = 38;
    Statistics_RecordAccess(&stats, TYPE_READ, cycles, 1);
    Statistics_RecordAccess(&stats, TYPE_READ, cycles, 2);
    Statistics_RecordAccess(&stats, TYPE_READ, cycles, 2);

    TEST_ASSERT_EQUAL_UINT64(3,        stats.read_count);
    TEST_ASSERT_EQUAL_UINT64(5,        stats.read_count_aligned);
    TEST_ASSERT_EQUAL_UINT64(cycles*3, stats.read_cycles);
}

void test_Statistics_RecordCacheAccess_should_RecordCacheHit(void)
{
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_HIT);

    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.hit_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.vc_hit_count);
}

void test_Statistics_RecordCacheAccess_should_RecordVictimCacheHit(void)
{
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_HIT_VICTIM_CACHE);

    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.hit_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.transfers);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.vc_hit_count);
}

void test_Statistics_RecordCacheAccess_should_RecordCacheMiss(void)
{
    Statistics_RecordCacheAccess(&(stats.l1i), RESULT_MISS);

    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.hit_count);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1i.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1i.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1i.vc_hit_count);
}

void test_Statistics_RecordCacheAccess_should_RecordCacheKickout(void)
{
    Statistics_RecordCacheAccess(&(stats.l2), RESULT_MISS_KICKOUT);

    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.hit_count);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l2.miss_count);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l2.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l2.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l2.vc_hit_count);
}

void test_Statistics_RecordCacheAccess_should_RecordCacheDirtyyKickout(void)
{
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_MISS_DIRTY_KICKOUT);

    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.hit_count);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.miss_count);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.kickouts);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(1, stats.l1d.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.vc_hit_count);
}

void test_Statistics_RecordCacheAccess_should_IncrementForMultipleAccesses(void)
{
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_HIT);
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_HIT);
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_HIT);

    TEST_ASSERT_EQUAL_UINT64(3, stats.l1d.hit_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.miss_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.vc_hit_count);

    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_MISS_DIRTY_KICKOUT);
    Statistics_RecordCacheAccess(&(stats.l1d), RESULT_MISS_DIRTY_KICKOUT);

    TEST_ASSERT_EQUAL_UINT64(3, stats.l1d.hit_count);
    TEST_ASSERT_EQUAL_UINT64(2, stats.l1d.miss_count);
    TEST_ASSERT_EQUAL_UINT64(2, stats.l1d.kickouts);
    TEST_ASSERT_EQUAL_UINT64(2, stats.l1d.dirty_kickouts);
    TEST_ASSERT_EQUAL_UINT64(2, stats.l1d.transfers);
    TEST_ASSERT_EQUAL_UINT64(0, stats.l1d.vc_hit_count);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_STATISTICS */
