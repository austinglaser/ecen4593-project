/**
 * @file    Config.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Config Source
 *
 * @addtogroup CONFIG
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Config.h"
#include "CException.h"
#include "ExceptionTypes.h"
#include "Util.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define L1_CACHE_STR        "L1"
#define L2_CACHE_STR        "L2"
#define MAIN_MEM_STR        "mem"

/* --- PRIVATE DATATYPES ---------------------------------------------------- */

typedef void (* value_writer_t)(uint32_t, void *);

typedef struct {
    const char * mem_names[2];
    const char * param_str;
    value_writer_t value_writer;
} config_value_t;

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static void write_value(const char * mem_name_str, const char * param_str, uint32_t value, config_t * configp);

static void cache_block_size_writer(uint32_t value, void * _cachep);
static void cache_cache_size_writer(uint32_t value, void * _cachep);
static void cache_associative_size_writer(uint32_t value, void * _cachep);
static void cache_hit_time_writer(uint32_t value, void * _cachep);
static void cache_miss_time_writer(uint32_t value, void * _cachep);
static void l2_cache_transfer_time_writer(uint32_t value, void * _cachep);
static void l2_cache_bus_width_writer(uint32_t value, void * _cachep);
static void main_mem_send_address_writer(uint32_t value, void * _memp);
static void main_mem_ready_writer(uint32_t value, void * _memp);
static void main_mem_send_chunk_writer(uint32_t value, void * _memp);
static void main_mem_chunk_size_writer(uint32_t value, void * _memp);

static void * get_mem(const char * mem_name_str, config_t * configp);
static const config_value_t * find_matching_config_value(const char * mem_name_str, const char * param_str);

static void ensure_value_power_of_two(uint32_t value);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static const config_value_t config_values[] = {
    { .mem_names = { L1_CACHE_STR, L2_CACHE_STR }, .param_str = "block_size",    .value_writer = cache_block_size_writer },
    { .mem_names = { L1_CACHE_STR, L2_CACHE_STR }, .param_str = "cache_size",    .value_writer = cache_cache_size_writer },
    { .mem_names = { L1_CACHE_STR, L2_CACHE_STR }, .param_str = "assoc",         .value_writer = cache_associative_size_writer },
    { .mem_names = { L1_CACHE_STR, L2_CACHE_STR }, .param_str = "hit_time",      .value_writer = cache_hit_time_writer },
    { .mem_names = { L1_CACHE_STR, L2_CACHE_STR }, .param_str = "miss_time",     .value_writer = cache_miss_time_writer },
    { .mem_names = { L2_CACHE_STR },               .param_str = "transfer_time", .value_writer = l2_cache_transfer_time_writer },
    { .mem_names = { L2_CACHE_STR },               .param_str = "bus_width",     .value_writer = l2_cache_bus_width_writer },
    { .mem_names = { MAIN_MEM_STR },               .param_str = "sendaddr",      .value_writer = main_mem_send_address_writer },
    { .mem_names = { MAIN_MEM_STR },               .param_str = "ready",         .value_writer = main_mem_ready_writer },
    { .mem_names = { MAIN_MEM_STR },               .param_str = "chunktime",     .value_writer = main_mem_send_chunk_writer },
    { .mem_names = { MAIN_MEM_STR },               .param_str = "chunksize",     .value_writer = main_mem_chunk_size_writer },
};

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Config_Defaults(config_t * configp)
{
    configp->l1.block_size_bytes            = 32;
    configp->l1.cache_size_bytes            = 8192;
    configp->l1.associative_bytes           = 1;
    configp->l1.hit_time_cycles             = 1;
    configp->l1.miss_time_cycles            = 1;
    configp->l1.transfer_time_cycles        = 0;    // not valid for L1
    configp->l1.bus_width_bytes             = 0;    // not valid for L1

    configp->l2.block_size_bytes            = 64;
    configp->l2.cache_size_bytes            = 32768;
    configp->l2.associative_bytes           = 1;
    configp->l2.hit_time_cycles             = 8;
    configp->l2.miss_time_cycles            = 10;
    configp->l2.transfer_time_cycles        = 10;
    configp->l2.bus_width_bytes             = 16;

    configp->main_mem.send_address_cycles   = 10;
    configp->main_mem.ready_cycles          = 50;
    configp->main_mem.send_chunk_cycles     = 15;
    configp->main_mem.chunk_size_bytes      = 8;
}

void Config_ParseLine(const char * line, config_t * configp)
{
    char mem_name_str[64];
    char param_str[64];
    uint32_t value;

    if (line == NULL || configp == NULL) {
        ThrowHere(ARGUMENT_ERROR);
    }

    if (sscanf(line, "%[^_]_%[^=]=%" SCNu32 "\n", mem_name_str, param_str, &value) == 3) {
        write_value(mem_name_str, param_str, value, configp);
    }
}

void Config_FromFile(const char * filename, config_t * configp)
{
    Config_Defaults(configp);

    if (filename) {
        FILE * config_file = fopen(filename, "r");
        if (config_file == NULL) {
            ThrowHere(BAD_CONFIG_FILE);
        }

        unsigned int line_no = 1;
        CEXCEPTION_T e;
        Try {
            char line[128];
            while (fgets(line, sizeof(line), config_file)) {
                Config_ParseLine(line, configp);
                line_no++;
            }
        }
        Catch (e) {
            fclose(config_file);
            // Manually set file/line number info for config file
            ThrowWithLocationInfo(e, filename, line_no);
        }

        fclose(config_file);
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void write_value(const char * mem_name_str, const char * param_str, uint32_t value, config_t * configp)
{
    void * memp = get_mem(mem_name_str, configp);
    const config_value_t * config_valuep = find_matching_config_value(mem_name_str, param_str);
    if (!config_valuep) {
        ThrowHere(BAD_CONFIG_PARAM);
    }

    config_valuep->value_writer(value, memp);
}

static void cache_block_size_writer(uint32_t value, void * _cachep)
{
    ensure_value_power_of_two(value);

    cache_param_t * cachep = _cachep;
    cachep->block_size_bytes = value;
}

static void cache_cache_size_writer(uint32_t value, void * _cachep)
{
    ensure_value_power_of_two(value);

    cache_param_t * cachep = _cachep;
    cachep->cache_size_bytes = value;
}

static void cache_associative_size_writer(uint32_t value, void * _cachep)
{
    ensure_value_power_of_two(value);

    cache_param_t * cachep = _cachep;
    cachep->associative_bytes = value;
}

static void cache_hit_time_writer(uint32_t value, void * _cachep)
{
    cache_param_t * cachep = _cachep;
    cachep->hit_time_cycles = value;
}

static void cache_miss_time_writer(uint32_t value, void * _cachep)
{
    cache_param_t * cachep = _cachep;
    cachep->miss_time_cycles = value;
}

static void l2_cache_transfer_time_writer(uint32_t value, void * _cachep)
{
    cache_param_t * cachep = _cachep;
    cachep->transfer_time_cycles = value;
}

static void l2_cache_bus_width_writer(uint32_t value, void * _cachep)
{
    ensure_value_power_of_two(value);

    cache_param_t * cachep = _cachep;
    cachep->bus_width_bytes = value;
}

static void main_mem_send_address_writer(uint32_t value, void * _memp)
{
    memory_param_t * memp = _memp;
    memp->send_address_cycles = value;
}

static void main_mem_ready_writer(uint32_t value, void * _memp)
{
    memory_param_t * memp = _memp;
    memp->ready_cycles = value;
}

static void main_mem_send_chunk_writer(uint32_t value, void * _memp)
{
    memory_param_t * memp = _memp;
    memp->send_chunk_cycles = value;
}

static void main_mem_chunk_size_writer(uint32_t value, void * _memp)
{
    ensure_value_power_of_two(value);

    memory_param_t * memp = _memp;
    memp->chunk_size_bytes = value;
}

static void * get_mem(const char * mem_name_str, config_t * configp)
{
    if (strcmp(mem_name_str, L1_CACHE_STR) == 0) {
        return &configp->l1;
    }
    else if (strcmp(mem_name_str, L2_CACHE_STR) == 0) {
        return &configp->l2;
    }
    else if (strcmp(mem_name_str, MAIN_MEM_STR) == 0) {
        return &configp->main_mem;
    }
    else {
        ThrowHere(BAD_CONFIG_CACHE);
        return NULL;
    }
}

static const config_value_t * find_matching_config_value(const char * mem_name_str, const char * param_str)
{
    unsigned int i;
    const unsigned int n_config_values = ARRAY_ELEMENTS(config_values);
    for (i = 0; i < n_config_values; i++) {
        const config_value_t * config_valuep = &config_values[i];

        if (strcmp(config_valuep->param_str, param_str) == 0) {
            unsigned int j;
            unsigned int n_mems = ARRAY_ELEMENTS(config_valuep->mem_names);
            for (j = 0; j < n_mems; j++) {
                const char * candidate_mem_name = config_valuep->mem_names[j];

                if (candidate_mem_name == NULL) {
                    break;
                }
                if (strcmp(mem_name_str, candidate_mem_name) == 0) {
                    return config_valuep;
                }
            }
        }
    }

    return NULL;
}

static void ensure_value_power_of_two(uint32_t value)
{
    if (!IS_POWER_OF_TWO(value)) {
        ThrowHere(BAD_CONFIG_VALUE);
    }
}

/** @} addtogroup CONFIG */
