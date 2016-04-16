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
 * @brief   Data structures for storing configuration data, and utilities for
 *          parsing config files
 *
 * If you have a look at the source, you'll see we went just a touch crazy. The
 * goal was to reduce code duplication to an absolute minimum, In this
 * part of the project, however, it resulted in a bizzare structure with lookup
 * tables and function pointers and... well you'll see.
 *
 * The upshot is that it works very reliably!
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   Structure storing configuration for a cache */
typedef struct cache_param {
    uint32_t block_size_bytes;      /**< Block size [bytes] */
    uint32_t cache_size_bytes;      /**< Total cache size [bytes] */
    uint32_t associativity;         /**< Associativity value. 1 is
                                         equivalent to direct mapped */
    uint32_t hit_time_cycles;       /**< Time consumed for a hit [cycles] */
    uint32_t miss_time_cycles;      /**< Time consumed for a miss [cycles] (not
                                         counting time to access next memory
                                         level) */
    uint32_t transfer_time_cycles;  /**< Time to transfer a busload of data
                                         [cycles] */
    uint32_t bus_width_bytes;       /**< Data per busload [bytes] */
} cache_param_t;

/**@brief   Structure storing configuration info for main memory */
typedef struct memory_param_t {
    uint32_t send_address_cycles;   /**< Time to send an address to memory
                                         [cycles] */
    uint32_t ready_cycles;          /**< Time for data to be read [cycles] */
    uint32_t send_chunk_cycles;     /**< Time to send a chunkload [cycles] */
    uint32_t chunk_size_bytes;      /**< Size of a chunkload [bytes] */
} memory_param_t;

/**@brief   Configuration describing a full memory hierarchy */
typedef struct config {
    cache_param_t   l1;             /**< L1 configuration
                                         (both instruction and data) */
    cache_param_t   l2;             /**< L2 configuration */
    memory_param_t  main_mem;       /**< Main memory configuration */
} config_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Sets the default values in the config structure
 *
 * @apram[in] config:      The struct to modify
 */
void Config_Defaults(config_t * config);

/**@brief   Parses the given line, and modifies the appropriate value in config
 *
 * @param[in] line:         The configuration line to parse
 * @param[out] config:      Configuration structure to modify
 *
 * @throws  ARGUMENT_ERROR:         If passed a NULL pointer
 * @throws  SYNTAX_ERROR:           If @p line contains invalid syntax
 */
void Config_ParseLine(const char * line, config_t * config);

/**@brief   Opens the file, and reads in all the config lines.
 *
 * If a value isn't specified, uses the default configuration value
 *
 * @param[in] filename:         The file to be read
 * @param[out] config:          Configuration to populate
 *
 * @throws  BAD_CONFIG_FILE:    If the configuration file could not be opened
 * @throws  BAD_CONFIG_CACHE:   If the config line refers to an invalid memory level
 * @throws  BAD_CONFIG_PARAM:   If the config line refers to an unrecognized parameter
 * @throws  BAD_CONFIG_VALUE:   If a value is out of the allowable range
 */
void Config_FromFile(const char * filename, config_t * config);

/**@brief   Prints configuration values
 *
 * @param[in] config:           Configuration to print
 */
void Config_Print(config_t const * config);

/**@brief   Prints the cost of a memory configuration
 *
 * @param[in] config:           Configuration to print
 */
void Config_PrintCost(config_t const * config);

/** @} defgroup CONFIG */

#endif /* ifndef CONFIG_H */
