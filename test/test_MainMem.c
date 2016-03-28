/**
 * @file    test_MainMem.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestMainMem Source
 *
 * @addtogroup TEST_MAINMEM
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "MainMem.h"

#include "Config.h"

#include "CException.h"
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

static config_t config;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    config.main_mem.send_address_cycles = 10;
    config.main_mem.ready_cycles        = 50;
    config.main_mem.send_chunk_cycles   = 15;
    config.main_mem.chunk_size_bytes    = 8;

    MainMem_Create(&config);
}

void tearDown(void)
{
}

void test_AlignedBusWidthAccess(void)
{
    access_t access = {
        .type       = TYPE_WRITE,
        .address    = 0x08004580,
        .n_bytes    = 8,
    };

    TEST_ASSERT_MESSAGE( (access.address % config.main_mem.chunk_size_bytes == 0) &&
                         (access.n_bytes == config.main_mem.chunk_size_bytes),
                         "Test parameters need attention");

    uint32_t expected_access_cycles = config.main_mem.send_address_cycles +
                                      config.main_mem.ready_cycles +
                                      config.main_mem.send_chunk_cycles;

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, MainMem_Access(&access));
}

void test_AlignedSmallerThanBusAccess(void)
{
    access_t access = {
        .type       = TYPE_READ,
        .address    = 0x08004580,
        .n_bytes    = 4,
    };

    TEST_ASSERT_MESSAGE( (access.address % config.main_mem.chunk_size_bytes == 0) &&
                         (access.n_bytes < config.main_mem.chunk_size_bytes),
                         "Test parameters need attention");

    uint32_t expected_access_cycles = config.main_mem.send_address_cycles +
                                      config.main_mem.ready_cycles +
                                      config.main_mem.send_chunk_cycles;

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, MainMem_Access(&access));
}

void test_UnalignedBusWidthAccess(void)
{
    access_t access = {
        .type       = TYPE_INSTR,
        .address    = 0x08004584,
        .n_bytes    = 8,
    };

    TEST_ASSERT_MESSAGE( (access.address % config.main_mem.chunk_size_bytes != 0) &&
                         (access.n_bytes == config.main_mem.chunk_size_bytes),
                         "Test parameters need attention");

    uint32_t expected_access_cycles = config.main_mem.send_address_cycles +
                                      config.main_mem.ready_cycles +
                                      (2 * config.main_mem.send_chunk_cycles);

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, MainMem_Access(&access));
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_MAINMEM */
