/**
 * @file    unity_Helper.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   UnityHelper Source
 *
 * @addtogroup UNITY_HELPER
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity_Helper.h"
#include "unity.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static const char * strncat_if_nonempty(char * out, unsigned int len, const char * base, const char * ext);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void AssertEqual_config_t(config_t expected,
                          config_t actual,
                          unsigned short line,
                          const char * message)
{
    char sub_msg[128];

    strncat_if_nonempty(sub_msg, 128, "L1", message);
    UNITY_TEST_ASSERT_EQUAL_cache_param_t(expected.l1, actual.l1, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "L2", message);
    UNITY_TEST_ASSERT_EQUAL_cache_param_t(expected.l2, actual.l2, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Main memory", message);
    UNITY_TEST_ASSERT_EQUAL_memory_param_t(expected.main_mem, actual.main_mem, line, sub_msg);
}

void AssertEqual_cache_param_t(cache_param_t expected,
                               cache_param_t actual,
                               unsigned short line,
                               const char * message)
{
    char sub_msg[128];

    strncat_if_nonempty(sub_msg, 128, "Field 'block_size'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.block_size_bytes, actual.block_size_bytes, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'cache_size'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.cache_size_bytes, actual.cache_size_bytes, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'associative'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.associativity, actual.associativity, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'hit_time'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.hit_time_cycles, actual.hit_time_cycles, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'miss_time'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.miss_time_cycles, actual.miss_time_cycles, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'transfer_time'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.transfer_time_cycles, actual.transfer_time_cycles, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'bus_width'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.bus_width_bytes, actual.bus_width_bytes, line, sub_msg);
}

void AssertEqual_memory_param_t(memory_param_t expected,
                                memory_param_t actual,
                                unsigned short line,
                                const char * message)
{
    char sub_msg[128];

    strncat_if_nonempty(sub_msg, 128, "Field 'send_address'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.send_address_cycles, actual.send_address_cycles, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'ready'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.ready_cycles, actual.ready_cycles, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'send_chunk'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.send_chunk_cycles, actual.send_chunk_cycles, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'chunk_size'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.chunk_size_bytes, actual.chunk_size_bytes, line, sub_msg);
}

void AssertEqual_access_t(access_t expected,
                          access_t actual,
                          unsigned short line,
                          const char * message)
{
    char sub_msg[128];

    strncat_if_nonempty(sub_msg, 128, "Field 'type'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT8(expected.type, actual.type, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'address'", message);
    UNITY_TEST_ASSERT_EQUAL_HEX64(expected.address, actual.address, line, sub_msg);

    strncat_if_nonempty(sub_msg, 128, "Field 'n_bytes'", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.n_bytes, actual.n_bytes, line, sub_msg);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static const char * strncat_if_nonempty(char * out, unsigned int len, const char * base, const char * ext)
{
    bool ext_nonempty = ext && (strcmp(ext, "") != 0);
    snprintf(out, len, "%s%s%s",
             base,
             ext_nonempty ? ". " : "",
             ext_nonempty ? ext  : "");

    return out;
}

/** @} addtogroup UNITY_HELPER */
