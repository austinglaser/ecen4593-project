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

static const char * l1_str                  = "L1";
static const char * l2_str                  = "L2";

/* --- PRIVATE DATATYPES ---------------------------------------------------- */

typedef void (* value_writer_t)(uint32_t, const char *, cache_param_t *);

typedef struct {
    const char * cache_str;
    const char * param_str;
    value_writer_t vw;
} config_value_t;

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static void block_size_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);
static void cache_size_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);
static void associative_size_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);
static void hit_time_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);
static void miss_time_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);
static void transfer_time_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);
static void bus_width_writer(uint32_t value, const char * cache_str, cache_param_t * cachep);

static cache_param_t * get_cache(const char * cache_str, config_t * configp);
static void ensure_cache_not_l1(const char * cache_str);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static const config_value_t config_values[] = {
    { .param_str = "block_size",    .vw = block_size_writer },
    { .param_str = "cache_size",    .vw = cache_size_writer },
    { .param_str = "assoc",         .vw = associative_size_writer },
    { .param_str = "hit_time",      .vw = hit_time_writer },
    { .param_str = "miss_time",     .vw = miss_time_writer },
    { .param_str = "transfer_time", .vw = transfer_time_writer },
    { .param_str = "bus_width",     .vw = bus_width_writer },
};

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Config_Defaults(config_t * configp)
{
    configp->l1.block_size_bytes        = 32;
    configp->l1.cache_size_bytes        = 8192;
    configp->l1.associative_bytes       = 1;
    configp->l1.hit_time_cycles         = 1;
    configp->l1.miss_time_cycles        = 1;
    configp->l1.transfer_time_cycles    = 0;    // not valid for L1
    configp->l1.bus_width_bytes         = 0;    // not valid for L1

    configp->l2.block_size_bytes        = 64;
    configp->l2.cache_size_bytes        = 32768;
    configp->l2.associative_bytes       = 1;
    configp->l2.hit_time_cycles         = 8;
    configp->l2.miss_time_cycles        = 10;
    configp->l2.transfer_time_cycles    = 10;
    configp->l2.bus_width_bytes         = 16;
}

void Config_ParseLine(const char * line, config_t * configp)
{
    char cache_str[64];
    char field_str[64];
    uint32_t value;

    if (line == NULL || configp == NULL) {
        ThrowHere(ARGUMENT_ERROR);
    }

    if (sscanf(line, "%[^_]_%[^=]=%" SCNu32 "\n", cache_str, field_str, &value) == 3) {
        unsigned int i;
        cache_param_t * cachep = get_cache(cache_str, configp);

        for (i = 0; i < ARRAY_ELEMENTS(config_values); i++) {
            config_value_t config_value = config_values[i];
            if (strcmp(config_value.param_str, field_str) == 0) {
                config_value.vw(value, cache_str, cachep);
                break;
            }
        }
        if (i == ARRAY_ELEMENTS(config_values)) {
            ThrowHere(BAD_CONFIG_PARAM);
        }
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

        CEXCEPTION_T e;
        Try {
            char line[128];
            while (fgets(line, sizeof(line), config_file)) {
                Config_ParseLine(line, configp);
            }
        }
        Catch (e) {
            fclose(config_file);
            // We use Throw instead of ThrowHere so as to preserve file/line no. info
            Throw(e);
        }

        fclose(config_file);
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static void block_size_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    (void) cache_str;

    cachep->block_size_bytes = value;
}

static void cache_size_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    (void) cache_str;

    cachep->cache_size_bytes = value;
}

static void associative_size_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    (void) cache_str;

    cachep->associative_bytes = value;
}

static void hit_time_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    (void) cache_str;

    cachep->hit_time_cycles = value;
}

static void miss_time_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    (void) cache_str;

    cachep->miss_time_cycles = value;
}

static void transfer_time_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    ensure_cache_not_l1(cache_str);

    cachep->transfer_time_cycles = value;
}

static void bus_width_writer(uint32_t value, const char * cache_str, cache_param_t * cachep)
{
    ensure_cache_not_l1(cache_str);

    cachep->bus_width_bytes = value;
}

static cache_param_t * get_cache(const char * cache_str, config_t * configp)
{
    if (strcmp(cache_str, l1_str) == 0) {
        return &configp->l1;
    }
    else if (strcmp(cache_str, l2_str) == 0) {
        return &configp->l2;
    }
    else {
        ThrowHere(BAD_CONFIG_CACHE);
        return NULL;
    }
}

static void ensure_cache_not_l1(const char * cache_str)
{
    if (strcmp(l1_str, cache_str) == 0) {
        ThrowHere(BAD_CONFIG_PARAM);
    }
}

/** @} addtogroup CONFIG */
