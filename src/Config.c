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

/**@brief   A function which knows how to write a particular config value to a
 *          structure */
typedef void (* value_writer_t)(uint32_t, void *);

/**@brief   Structure encoding a single config value */
typedef struct {
    const char * mem_names[2];      /**< The memory levels the value is valid for */
    const char * param_str;         /**< The parameter string to match in the config */
    value_writer_t value_writer;    /**< A function which knows how to write it
                                         to the appropriate structure */
} config_value_t;

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

/**@brief   Takes a line that's been split into a memory name, parameter, and
 *          value, and writes it to the config structure if it's valid
 *
 * @param[in] mem_name_str:     The memory level's name
 * @param[in] param_str:        The name of the parameter
 * @param[in] value:            The raw value
 * @param[out] config:          The config that will be modified
 */
static void write_value(const char * mem_name_str, const char * param_str, uint32_t value, config_t * config);

/**@brief   Writes a cache's block size to the cache config */
static void cache_block_size_writer(uint32_t value, void * _cache);

/**@brief   Writes a cache's total size to the cache config */
static void cache_cache_size_writer(uint32_t value, void * _cache);

/**@brief   Writes a cache's associativity value to the cache config */
static void cache_associative_size_writer(uint32_t value, void * _cache);

/**@brief   Writes a caches hit-time value to the cache config */
static void cache_hit_time_writer(uint32_t value, void * _cache);

/**@brief   Writes a caches miss-time value to the cache config */
static void cache_miss_time_writer(uint32_t value, void * _cache);

/**@brief   Writes a caches bus transfer time value to the cache config */
static void l2_cache_transfer_time_writer(uint32_t value, void * _cache);

/**@brief   Writes a caches bus width value to the cache config */
static void l2_cache_bus_width_writer(uint32_t value, void * _cache);

/**@brief   Writes main memory's send address time to the memory config */
static void main_mem_send_address_writer(uint32_t value, void * _memp);

/**@brief   Writes main memory's data ready time to the memory config */
static void main_mem_ready_writer(uint32_t value, void * _memp);

/**@brief   Writes main memory's chunk transmission time to the memory config */
static void main_mem_send_chunk_writer(uint32_t value, void * _memp);

/**@brief   Writes main memory's chunk size time to the memory config */
static void main_mem_chunk_size_writer(uint32_t value, void * _memp);

/**@brief   Gets an abstracted pointer to the appropriate sub-structure in @p config
 *
 * @return  A pointer to the structure, or NULL if @p mem_name_str is an invalid value
 */
static void * get_mem(const char * mem_name_str, config_t * config);

/**@brief   Finds a value matching the memory/parameter pair given
 *
 * @see     config_values
 */
static const config_value_t * find_matching_config_value(const char * mem_name_str, const char * param_str);

/**@brief   Determines whether a given configuration value matches the provided memory/parameter pair */
static bool is_matching_config_value(const config_value_t * config_valuep, const char * mem_name_str, const char * param_str);

/**@brief   Throws an exception of @p value is not a power of two */
static void ensure_value_power_of_two(uint32_t value);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

/**@brief   Array of all possible configuration values, the memory they're
 *          valid for, and their writers
 */
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

void Config_Defaults(config_t * config)
{
    config->l1.block_size_bytes            = 32;
    config->l1.cache_size_bytes            = 8192;
    config->l1.associativity               = 1;
    config->l1.hit_time_cycles             = 1;
    config->l1.miss_time_cycles            = 1;
    config->l1.transfer_time_cycles        = 0;    // not valid for L1
    config->l1.bus_width_bytes             = 0;    // not valid for L1

    config->l2.block_size_bytes            = 64;
    config->l2.cache_size_bytes            = 32768;
    config->l2.associativity               = 1;
    config->l2.hit_time_cycles             = 8;
    config->l2.miss_time_cycles            = 10;
    config->l2.transfer_time_cycles        = 10;
    config->l2.bus_width_bytes             = 16;

    config->main_mem.send_address_cycles   = 10;
    config->main_mem.ready_cycles          = 50;
    config->main_mem.send_chunk_cycles     = 15;
    config->main_mem.chunk_size_bytes      = 8;
}

void Config_ParseLine(const char * line, config_t * config)
{
    char mem_name_str[64];
    char param_str[64];
    uint32_t value;

    if (line == NULL || config == NULL) {
        ThrowHere(ARGUMENT_ERROR);
    }

    if (strcmp("", line) == 0 || strcmp("\n", line) == 0) {
        return;
    }
    else if (sscanf(line, "%[^_]_%[^=]=%" SCNu32 "\n", mem_name_str, param_str, &value) == 3) {
        write_value(mem_name_str, param_str, value, config);
    }
    else {
        ThrowHere(SYNTAX_ERROR);
    }
}

void Config_FromFile(const char * filename, config_t * config)
{
    Config_Defaults(config);

    if (filename) {
        FILE * config_file = fopen(filename, "r");
        if (config_file == NULL) {
            ThrowHere(BAD_CONFIG_FILE);
        }

        volatile unsigned int line_no = 1;
        CEXCEPTION_T e;
        Try {
            char line[128];
            while (fgets(line, sizeof(line), config_file)) {
                Config_ParseLine(line, config);
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

void Config_Print(config_t const * config)
{
    printf("    Dcache size   = %6" PRIu32 " : ways = %3" PRIu32 " : block size = %3" PRIu32 "\n",
           config->l1.cache_size_bytes,
           config->l1.associativity,
           config->l1.block_size_bytes);
    printf("    Icache size   = %6" PRIu32 " : ways = %3" PRIu32 " : block size = %3" PRIu32 "\n",
           config->l1.cache_size_bytes,
           config->l1.associativity,
           config->l1.block_size_bytes);
    printf("    L2-cache size = %6" PRIu32 " : ways = %3" PRIu32 " : block size = %3" PRIu32 "\n",
           config->l2.cache_size_bytes,
           config->l2.associativity,
           config->l2.block_size_bytes);

    printf("    Memory ready time = %" PRIu32 " : chunksize = %" PRIu32 " : chunktime = %" PRIu32 "\n",
           config->main_mem.ready_cycles,
           config->main_mem.chunk_size_bytes,
           config->main_mem.send_chunk_cycles);
}

void Config_PrintCost(config_t const * config)
{
    uint32_t l1_size_factor  = (config->l1.cache_size_bytes / 4096);
    uint32_t l1_assoc_factor = HighestBitSet(config->l1.associativity) * l1_size_factor;
    uint32_t l1_cost         = 100 * l1_size_factor +
                               100 * l1_assoc_factor;

    uint32_t l2_size_factor  = (config->l2.cache_size_bytes / 16384);
    uint32_t l2_assoc_factor = HighestBitSet(config->l2.associativity) * l2_size_factor;
    uint32_t l2_cost         = 50 * l2_size_factor +
                               50 * l2_assoc_factor;

    uint32_t mem_latency_factor   = HighestBitSet(CEIL_DIVIDE(50, config->main_mem.ready_cycles));
    uint32_t mem_bandwidth_factor = HighestBitSet(config->main_mem.chunk_size_bytes / 8);
    uint32_t memory_cost          = 50 + 200 * mem_latency_factor +
                                    25 + 100 * mem_bandwidth_factor;

    uint32_t total_cost = l1_cost*2 + l2_cost + memory_cost;

    printf("  L1 cache cost (Icache $%"PRIu32") + (Dcache $%"PRIu32") = $%"PRIu32"\n", l1_cost, l1_cost, l1_cost*2);
    printf("  L2 cache cost = $%"PRIu32";  Memory cost = $%"PRIu32"  Total cost = $%"PRIu32"\n",
           l2_cost, memory_cost, total_cost);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void write_value(const char * mem_name_str, const char * param_str, uint32_t value, config_t * config)
{
    void * memp = get_mem(mem_name_str, config);
    const config_value_t * config_valuep = find_matching_config_value(mem_name_str, param_str);
    if (!config_valuep) {
        ThrowHere(BAD_CONFIG_PARAM);
    }

    config_valuep->value_writer(value, memp);
}

static void cache_block_size_writer(uint32_t value, void * _cache)
{
    ensure_value_power_of_two(value);

    cache_param_t * cache = _cache;
    cache->block_size_bytes = value;
}

static void cache_cache_size_writer(uint32_t value, void * _cache)
{
    ensure_value_power_of_two(value);

    cache_param_t * cache = _cache;
    cache->cache_size_bytes = value;
}

static void cache_associative_size_writer(uint32_t value, void * _cache)
{
    ensure_value_power_of_two(value);

    cache_param_t * cache = _cache;
    cache->associativity = value;
}

static void cache_hit_time_writer(uint32_t value, void * _cache)
{
    cache_param_t * cache = _cache;
    cache->hit_time_cycles = value;
}

static void cache_miss_time_writer(uint32_t value, void * _cache)
{
    cache_param_t * cache = _cache;
    cache->miss_time_cycles = value;
}

static void l2_cache_transfer_time_writer(uint32_t value, void * _cache)
{
    cache_param_t * cache = _cache;
    cache->transfer_time_cycles = value;
}

static void l2_cache_bus_width_writer(uint32_t value, void * _cache)
{
    ensure_value_power_of_two(value);

    cache_param_t * cache = _cache;
    cache->bus_width_bytes = value;
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

static void * get_mem(const char * mem_name_str, config_t * config)
{
    if (strcmp(mem_name_str, L1_CACHE_STR) == 0) {
        return &config->l1;
    }
    else if (strcmp(mem_name_str, L2_CACHE_STR) == 0) {
        return &config->l2;
    }
    else if (strcmp(mem_name_str, MAIN_MEM_STR) == 0) {
        return &config->main_mem;
    }
    else {
        ThrowHere(BAD_CONFIG_CACHE);
        // Pedantic; this is unreachable
        return NULL;
    }
}

static const config_value_t * find_matching_config_value(const char * mem_name_str, const char * param_str)
{
    unsigned int i;
    const unsigned int n_config_values = ARRAY_ELEMENTS(config_values);
    for (i = 0; i < n_config_values; i++) {
        const config_value_t * config_valuep = &config_values[i];
        if (is_matching_config_value(config_valuep, mem_name_str, param_str)) {
            return config_valuep;
        }
    }

    return NULL;
}

static bool is_matching_config_value(const config_value_t * config_valuep, const char * mem_name_str, const char * param_str)
{
    if (strcmp(config_valuep->param_str, param_str) == 0) {
        unsigned int i;
        unsigned int n_mems = ARRAY_ELEMENTS(config_valuep->mem_names);
        for (i = 0; i < n_mems; i++) {
            const char * candidate_mem_name = config_valuep->mem_names[i];

            if (candidate_mem_name == NULL) {
                break;
            }
            if (strcmp(mem_name_str, candidate_mem_name) == 0) {
                return true;
            }
        }
    }
    return false;
}

static void ensure_value_power_of_two(uint32_t value)
{
    if (!IS_POWER_OF_TWO(value)) {
        ThrowHere(BAD_CONFIG_VALUE);
    }
}

/** @} addtogroup CONFIG */
