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

#include "Access.h"
#include "Config.h"
#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include "ConfigDefaults.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static config_t config;
static main_mem_t main_mem;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void setUp(void)
{
    SetDefaultConfigValues(&config);

    main_mem = MainMem_Create(&(config.main_mem));
}

void tearDown(void)
{
    MainMem_Destroy(main_mem);
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

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, MainMem_Access(main_mem, &access));
}

void test_LargeAccess(void)
{
    access_t access = {
        .type       = TYPE_INSTR,
        .address    = 0x08004580,
        .n_bytes    = 64,
    };

    TEST_ASSERT_MESSAGE( (access.address % config.main_mem.chunk_size_bytes == 0) &&
                         (access.n_bytes > config.main_mem.chunk_size_bytes),
                         "Test parameters need attention");

    uint32_t expected_access_cycles = config.main_mem.send_address_cycles +
                                      config.main_mem.ready_cycles +
                                      (8 * config.main_mem.send_chunk_cycles);

    TEST_ASSERT_EQUAL_UINT32(expected_access_cycles, MainMem_Access(main_mem, &access));
}

void test_AccessTypeDoesntMatter(void)
{
    access_t access = {
        .address    = 0x08004580,
        .n_bytes    = 8,
    };

    access.type = TYPE_INSTR;
    uint32_t instruction_access_time    = MainMem_Access(main_mem, &access);
    access.type = TYPE_READ;
    uint32_t read_access_time           = MainMem_Access(main_mem, &access);
    access.type = TYPE_WRITE;
    uint32_t write_access_time          = MainMem_Access(main_mem, &access);

    TEST_ASSERT_EQUAL_UINT32(instruction_access_time, read_access_time);
    TEST_ASSERT_EQUAL_UINT32(instruction_access_time, write_access_time);
    TEST_ASSERT_EQUAL_UINT32(read_access_time, write_access_time);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup TEST_MAINMEM */
