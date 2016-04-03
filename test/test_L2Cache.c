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
#include "CacheData.h"
#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include "ConfigDefaults.h"
#include "mock_MainMem.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

// Hardcoded in test
static const uint32_t victim_cache_len = 8;

/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static config_t config;
static main_mem_t dummy_main_mem;
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

    l2_cache = L2Cache_Create(dummy_main_mem, &config);
}

void tearDown(void)
{
    L2Cache_Destroy(l2_cache);
}

void test_InstructionAccessToEmptyCacheShouldMiss(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = access.type,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    uint32_t main_mem_access_cycles = 75;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_ReadAccessToEmptyCacheShouldMiss(void)
{
    access_t access = {
        .type = TYPE_READ,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = access.type,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    uint32_t main_mem_access_cycles = 32;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_WriteAccessToEmptyCacheShouldMiss(void)
{
    access_t access = {
        .type = TYPE_WRITE,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = access.type,
        .address = 0x10123400,
        .n_bytes = config.l2.block_size_bytes,
    };

    uint32_t main_mem_access_cycles = 256;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    uint32_t expected_access_cycles = config.l2.miss_time_cycles +
                                      main_mem_access_cycles +
                                      config.l2.hit_time_cycles +
                                      l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, L2Cache_Access(l2_cache, &access));
}

void test_RepeatedAccessShouldMissFirstThenHit(void)
{
    access_t access = {
        .type = TYPE_INSTR,
        .address = 0x10123400,
        .n_bytes = config.l1.block_size_bytes,
    };

    uint32_t main_mem_access_cycles = 75;
    MainMem_Access_IgnoreAndReturn(main_mem_access_cycles);

    L2Cache_Access(l2_cache, &access);

    uint32_t expected_hit_cycles = config.l2.hit_time_cycles +
                                   l1_block_transfer_cycles;
    TEST_ASSERT_EQUAL_UINT32(expected_hit_cycles, L2Cache_Access(l2_cache, &access));
}

void test_SuccessiveAccessesToDifferentBlocksShouldBothMiss(void)
{
    access_t access1 = {
        .type = TYPE_READ,
        .address = 0x0000,
        .n_bytes = config.l1.block_size_bytes,
    };
    access_t expected_memory_access1 = {
        .type = TYPE_READ,
        .address = access1.address,
        .n_bytes = config.l2.block_size_bytes,
    };

    access_t access2 = {
        .type = TYPE_READ,
        .address = access1.address + config.l2.block_size_bytes,
        .n_bytes = config.l1.block_size_bytes,
    };
    access_t expected_memory_access2 = {
        .type = TYPE_READ,
        .address = access2.address,
        .n_bytes = config.l2.block_size_bytes,
    };

    uint32_t main_mem_access_cycles1 = 35;
    uint32_t main_mem_access_cycles2 = 68;

    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access1, main_mem_access_cycles1);
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access2, main_mem_access_cycles2);

    uint32_t expected_access_cycles1 = config.l2.miss_time_cycles +
                                       main_mem_access_cycles1 +
                                       config.l2.hit_time_cycles +
                                       l1_block_transfer_cycles;

    uint32_t expected_access_cycles2 = config.l2.miss_time_cycles +
                                       main_mem_access_cycles2 +
                                       config.l2.hit_time_cycles +
                                       l1_block_transfer_cycles;

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles1, L2Cache_Access(l2_cache, &access1));
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles2, L2Cache_Access(l2_cache, &access2));
}

void test_AccessToTwoL1BlocksWithinTheSameL2BlockShouldMissThenHit(void)
{
    access_t access1 = {
        .type = TYPE_READ,
        .address = 0xfac029320000,
        .n_bytes = config.l1.block_size_bytes,
    };
    access_t access2 = {
        .type = TYPE_READ,
        .address = access1.address + config.l1.block_size_bytes,
        .n_bytes = config.l1.block_size_bytes,
    };

    access_t expected_memory_access = {
        .type = TYPE_READ,
        .address = access1.address,
        .n_bytes = config.l2.block_size_bytes,
    };

    uint32_t main_mem_access_cycles = 23;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &expected_memory_access, main_mem_access_cycles);

    uint32_t expected_access_cycles1 = config.l2.miss_time_cycles +
                                       main_mem_access_cycles +
                                       config.l2.hit_time_cycles +
                                       l1_block_transfer_cycles;
    uint32_t expected_access_cycles2 = config.l2.hit_time_cycles +
                                       l1_block_transfer_cycles;

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles1, L2Cache_Access(l2_cache, &access1));
    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles2, L2Cache_Access(l2_cache, &access2));
}

void test_OverwriteOfDirtyBlockShouldDirtyKickoutFromVictimCache(void)
{
    uint64_t base_address = 0xf78d9ee40;
    access_t access = {
        .type = TYPE_WRITE,
        .address = base_address,
        .n_bytes = config.l1.block_size_bytes,
    };

    MainMem_Access_IgnoreAndReturn(0);
    L2Cache_Access(l2_cache, &access);

    uint32_t i;
    access.type = TYPE_READ;
    for (i = 1; i <= victim_cache_len; i++) {
        access.address = base_address +
                         (i * n_blocks * config.l2.block_size_bytes);
        L2Cache_Access(l2_cache, &access);
    }

    access.address = base_address +
                     ((victim_cache_len + 1) *
                      n_blocks *
                      config.l2.block_size_bytes);

    access_t dirty_kickout_access = {
        .type = TYPE_WRITE,
        .address = base_address,
        .n_bytes = config.l2.block_size_bytes,
    };
    uint32_t kickout_cycles = 38;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &dirty_kickout_access, kickout_cycles);

    access_t final_miss_access = {
        .type = TYPE_READ,
        .address = access.address,
        .n_bytes = config.l2.block_size_bytes,
    };
    uint32_t final_miss_cycles = 229;
    MainMem_Access_ExpectAndReturn(dummy_main_mem, &final_miss_access, final_miss_cycles);

    uint32_t total_access_cycles = config.l2.miss_time_cycles +
                                   kickout_cycles +
                                   final_miss_cycles +
                                   config.l2.hit_time_cycles +
                                   l1_block_transfer_cycles;

    TEST_ASSERT_EQUAL_UINT32(total_access_cycles, L2Cache_Access(l2_cache, &access));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_L2CACHE */
