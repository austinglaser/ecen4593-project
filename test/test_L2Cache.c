/**
 * @file    test_L2Cache.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestL2Cache Source
 *
 * @addtogroup TEST_L2CACHE
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "L2Cache.h"

#include "Access.h"
#include "CacheInternals.h"
#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include "ConfigDefaults.h"

#include "mock_CacheData.h"
#include "mock_MainMem.h"
#include "mock_Statistics.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

static const uint32_t victim_cache_len = 8;

/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static config_t config;

static main_mem_t dummy_main_mem;
static cache_stats_t * dummy_cache_stats;
static cache_data_t dummy_cache_data = (cache_data_t)8;

static l2_cache_t l2_cache;
static uint32_t l1_block_transfer_cycles;
static uint32_t n_blocks;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);

    l1_block_transfer_cycles = config.l2.transfer_time_cycles *
                               (config.l1.block_size_bytes /
                                config.l2.bus_width_bytes);
    n_blocks = config.l2.cache_size_bytes / config.l2.block_size_bytes;

    CacheData_Create_ExpectAndReturn(n_blocks,
                                     1,
                                     config.l2.block_size_bytes,
                                     8,
                                     dummy_cache_data);

    l2_cache = L2Cache_Create(dummy_main_mem, dummy_cache_stats, &(config.l2));
}

void tearDown(void)
{
    CacheData_Destroy_Expect(dummy_cache_data);
    L2Cache_Destroy(l2_cache);
}

void test_CacheData_Create_should_ReturnNULL_when_CacheDataAllocationFails(void)
{
    CacheData_Create_IgnoreAndReturn(NULL);

    TEST_ASSERT_EQUAL_PTR(NULL, L2Cache_Create(dummy_main_mem, dummy_cache_stats, &(config.l2)));
}

void test_InstructionMiss_should_CauseMainMemReadAccess(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t main_mem_access_cycles = 75;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_ReadMiss_should_CauseMainMemReadAccess(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t main_mem_access_cycles = 32;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_WriteMiss_should_CauseMainMemReadAccess(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t main_mem_access_cycles = 256;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_InstructionHit_should_not_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_HIT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_ReadHit_should_not_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_HIT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_WriteHit_should_not_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_HIT;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_InstructionVictimCacheHit_should_not_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_HIT_VICTIM_CACHE;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_ReadVictimCacheHit_should_not_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_HIT_VICTIM_CACHE;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_WriteVictimCacheHit_should_not_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    result_t result = RESULT_HIT_VICTIM_CACHE;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_InstructionKickout_should_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS_KICKOUT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t main_mem_access_cycles = 12;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_ReadKickout_should_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS_KICKOUT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t main_mem_access_cycles = 12;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_WriteKickout_should_CauseMainMemAccess(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS_KICKOUT;
    CacheData_Write_ExpectAndReturn(dummy_cache_data, access.address, NULL, 0);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t main_mem_access_cycles = 12;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_ReadDirtyKickout_should_CauseWriteback(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_dirty_writeback = {
        .type = TYPE_WRITE,
        .address = 0xf90dfca8500,
        .n_bytes = config.l2.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS_DIRTY_KICKOUT;
    CacheData_Read_ExpectAndReturn(dummy_cache_data,
                                   access.address,
                                   NULL,
                                   expected_dirty_writeback.address);
    CacheData_Read_IgnoreArg_result();
    CacheData_Read_ReturnThruPtr_result(&result);

    uint32_t main_mem_writeback_cycles = 32;
    uint32_t main_mem_access_cycles = 181;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_dirty_writeback, main_mem_writeback_cycles);
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access,   main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_writeback_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_WriteDirtyKickout_should_CauseWriteback(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_dirty_writeback = {
        .type = TYPE_WRITE,
        .address = 0xf90dfca8500,
        .n_bytes = config.l2.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    result_t result = RESULT_MISS_DIRTY_KICKOUT;
    CacheData_Write_ExpectAndReturn(dummy_cache_data,
                                    access.address,
                                    NULL,
                                    expected_dirty_writeback.address);
    CacheData_Write_IgnoreArg_result();
    CacheData_Write_ReturnThruPtr_result(&result);

    uint32_t main_mem_writeback_cycles = 32;
    uint32_t main_mem_access_cycles = 181;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_dirty_writeback, main_mem_writeback_cycles);
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access,   main_mem_access_cycles);

    Statistics_RecordCacheAccess_Expect(dummy_cache_stats, result);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_writeback_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L2CACHE */
