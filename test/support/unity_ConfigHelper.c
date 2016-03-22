/**
 * @file    unity_ConfigHelper.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   UnityConfigHelper Source
 *
 * @addtogroup UNITY_CONFIGHELPER
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "unity.h"
#include "unity_ConfigHelper.h"
#include "Config.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static void sncat_if_nonnull(char * out, unsigned int len, const char * base, const char * ext);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void AssertEqual_config_t(config_t expected,
                          config_t actual,
                          unsigned short line,
                          const char * message)
{
    char sub_msg[128];

    sncat_if_nonnull(sub_msg, 128, "L1", message);
    UNITY_TEST_ASSERT_EQUAL_cache_param_t(expected.l1, actual.l1, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "L2", message);
    UNITY_TEST_ASSERT_EQUAL_cache_param_t(expected.l2, actual.l2, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "Main memory", message);
    UNITY_TEST_ASSERT_EQUAL_memory_param_t(expected.main_mem, actual.main_mem, line, sub_msg);
}

void AssertEqual_cache_param_t(cache_param_t expected,
                               cache_param_t actual,
                               unsigned short line,
                               const char * message)
{
    char sub_msg[128];

    sncat_if_nonnull(sub_msg, 128, "block_size", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.block_size_bytes, actual.block_size_bytes, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "cache_size", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.cache_size_bytes, actual.cache_size_bytes, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "associative", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.associativity, actual.associativity, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "hit_time", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.hit_time_cycles, actual.hit_time_cycles, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "miss_time", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.miss_time_cycles, actual.miss_time_cycles, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "transfer_time", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.transfer_time_cycles, actual.transfer_time_cycles, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "bus_width", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.bus_width_bytes, actual.bus_width_bytes, line, sub_msg);
}

void AssertEqual_memory_param_t(memory_param_t expected,
                                memory_param_t actual,
                                unsigned short line,
                                const char * message)
{
    char sub_msg[128];

    sncat_if_nonnull(sub_msg, 128, "send_address", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.send_address_cycles, actual.send_address_cycles, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "ready", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.ready_cycles, actual.ready_cycles, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "send_chunk", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.send_chunk_cycles, actual.send_chunk_cycles, line, sub_msg);

    sncat_if_nonnull(sub_msg, 128, "chunk_size", message);
    UNITY_TEST_ASSERT_EQUAL_UINT32(expected.chunk_size_bytes, actual.chunk_size_bytes, line, sub_msg);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void sncat_if_nonnull(char * out, unsigned int len, const char * base, const char * ext)
{
    bool ext_nonempty = ext && (strcmp(ext, "") != 0);
    snprintf(out, len, "%s%s%s",
             base,
             ext_nonempty ? ". " : "",
             ext_nonempty ? ext  : "");
}

/** @} addtogroup UNITY_CONFIGHELPER */
