/**
 * @file    Config.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Config Interface
 */

#ifndef CONFIG_H
#define CONFIG_H

/**@defgroup CONFIG Config
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct cache_param {
    uint32_t block_size_bytes;
    uint32_t cache_size_bytes;
    uint32_t associative_bytes;
    uint32_t hit_time_cycles;
    uint32_t miss_time_cycles;
    uint32_t transfer_time_cycles;
    uint32_t bus_width_bytes;
} cache_param_t;

typedef struct memory_param_t {
    uint32_t send_address_cycles;
    uint32_t ready_cycles;
    uint32_t send_chunk_cycles;
    uint32_t chunk_size_bytes;
} memory_param_t;

typedef struct config {
    cache_param_t   l1;
    cache_param_t   l2;
    memory_param_t  main_mem;
} config_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Sets the default values in the config structure
 *
 * @apram[in] configp:      The struct to modify
 */
void Config_Defaults(config_t * configp);

/**@brief   Parses the given line, and modifies the appropriate value in configp */
void Config_ParseLine(const char * line, config_t * configp);

/**@brief   Opens the file, and reads in all the config lines.
 *
 * If a value isn't specified, uses the default configuration value
 */
void Config_FromFile(const char * filename, config_t * configp);

/** @} defgroup CONFIG */

#endif /* ifndef CONFIG_H */
