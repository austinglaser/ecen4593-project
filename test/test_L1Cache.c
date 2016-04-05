/**
 * @file    test_L1DirectMapped.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestL1DirectMapped Source
 *
 * @addtogroup TEST_L1DIRECTMAPPED
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "L1Cache.h"

#include "Access.h"
#include "Config.h"
#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"
#include "ConfigDefaults.h"

#include "mock_CacheData.h"
#include "mock_L2Cache.h"
#include "mock_Statistics.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static l1_cache_t   l1_cache;

static l2_cache_t dummy_l2_cache;
static cache_stats_t * dummy_cache_stats;
static cache_data_t dummy_cache_data = (cache_data_t) 4;

static config_t config;
static uint32_t n_blocks;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);

    n_blocks = config.l1.cache_size_bytes / config.l1.block_size_bytes;
    CacheData_Create_ExpectAndReturn(n_blocks,
                                     1,
                                     config.l1.block_size_bytes,
                                     8,
                                     dummy_cache_data);

    l1_cache = L1Cache_Create(dummy_l2_cache, dummy_cache_stats, &(config.l1));
}

void tearDown(void)
{
    CacheData_Destroy_Expect(dummy_cache_data);
    L1Cache_Destroy(l1_cache);
}

void test_CacheData_Create_should_ReturnNULL_when_CacheDataAllocationFails(void)
{
    CacheData_Create_IgnoreAndReturn(NULL);

    TEST_ASSERT_EQUAL_PTR(NULL, L1Cache_Create(dummy_l2_cache, dummy_cache_stats, &(config.l1)));
}

void test_InstructionMiss_should_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t l2_access_cycles = 75;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_ReadMiss_should_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0xbbccfa00,
        .n_bytes = 4,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0xbbccfa00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t l2_access_cycles = 31;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_WriteMiss_should_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0xcccc00,
        .n_bytes = 4,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0xcccc00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t l2_access_cycles = 829;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_InstructionMissKickout_should_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS_KICKOUT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t l2_access_cycles = 75;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_ReadMissKickout_should_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0xbbccfa00,
        .n_bytes = 4,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0xbbccfa00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS_KICKOUT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t l2_access_cycles = 31;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_WriteMissKickout_should_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0xcccc00,
        .n_bytes = 4,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0xcccc00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS_KICKOUT;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t l2_access_cycles = 829;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_InstructionHit_should_not_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_ReadHit_should_not_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_WriteHit_should_not_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_InstructionVictimCacheHit_should_not_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT_VICTIM_CACHE;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_ReadVictimCacheHit_should_not_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT_VICTIM_CACHE;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_WriteVictimCacheHit_should_not_CauseL2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    result_t result = RESULT_HIT_VICTIM_CACHE;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_ReadDirtyKickout_should_CauseL2Write_then_L2Read(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    access_t expected_dirty_writeback = {
        .type = TYPE_WRITE,
        .address = 0xacacddf1200,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS_DIRTY_KICKOUT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL,
                                   expected_dirty_writeback.address);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t l2_writeback_cycles = 110;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_dirty_writeback, l2_writeback_cycles);

    uint32_t l2_access_cycles = 19;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_writeback_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

void test_WriteDirtyKickout_should_CauseL2Write_then_L2Read(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x4cd7f0c00,
        .n_bytes = 4,
    };

    access_t expected_dirty_writeback = {
        .type = TYPE_WRITE,
        .address = 0xacacddf1200,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x4cd7f0c00,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_MISS_DIRTY_KICKOUT;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL,
                                    expected_dirty_writeback.address);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t l2_writeback_cycles = 110;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_dirty_writeback, l2_writeback_cycles);

    uint32_t l2_access_cycles = 19;
    L2Cache_Access_ExpectAndReturn(dummy_l2_cache, &expected_memory_access, l2_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l1.miss_time_cycles +
                                      l2_writeback_cycles +
                                      l2_access_cycles +
                                      config.l1.hit_time_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L1Cache_Access(l1_cache, &access));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L1DIRECTMAPPED */
