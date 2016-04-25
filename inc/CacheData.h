/**
 * @file    CacheData.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CacheData Interface
 */

#ifndef CACHEDATA_H
#define CACHEDATA_H

/**@defgroup CACHEDATA CacheData
 * @{
 *
 * @brief   Handles all data bookkeeping; i.e. keeping track of which blocks are
 *          present in a cache, which are dirty, etc.
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

/**@brief   Possible results from accessing cache data */
typedef enum {
    /**@brief   The block was found in the cache */
    RESULT_HIT = 0,

    /**@brief   The block was found in the victim cache
     *
     * @note    The block will have been moved back into the cache proper,
     *          exchanged with the oldest member of the set it maps to
     */
    RESULT_HIT_VICTIM_CACHE,

    /**@brief   The block was inserted into the cache, displacing no data */
    RESULT_MISS,

    /**@brief   The block was inserted into the cache, displacing a clean block */
    RESULT_MISS_KICKOUT,

    /**@brief   The block was inserted into the cache, displacing a dirty block */
    RESULT_MISS_DIRTY_KICKOUT,
} result_t;

/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   Anonymous definition of the cache data type */
typedef struct _cache_data_t * cache_data_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Create a new instance of cache_data_t
 *
 * @param[in] n_sets:                   The total number of cache sets
 * @param[in] set_len:                  The number of blocks allowed to be
 *                                      stored in a set.
 *                                      The cache's associativity
 * @param[in] block_size_bytes:         Size of a cache block. Used in tag
 *                                      generation and address alignment
 * @param[in] victim_cache_len_blocks:  The number of blocks kept in the victim
 *                                      cache
 *
 * @return:     A pointer to the newly-created data structure, or NULL if
 *              memory allocation failed
 */
cache_data_t CacheData_Create(uint32_t n_sets,
                              uint32_t set_len,
                              uint32_t block_size_bytes,
                              uint32_t victim_cache_len_blocks);

/**@brief   De-allocate memory used by @p data
 *
 * @warning @p data should NOT be accessed after a call to this function. It
 *          will not point to valid memory.
 *
 * @param[in,out] data:     The structure to be freed
 */
void CacheData_Destroy(cache_data_t data);

/**@brief   Determine whether a cache contains a block with the given address
 *
 * @param[in] data:         The cache data to search
 * @param[in] address:      The address to search for
 *
 * @return:     Whether @p address is within a block contained in the data
 */
bool CacheData_Contains(cache_data_t data, uint64_t address);

/**@brief   Simulate a write access to the given cache data
 *
 * @param[in,out] data:     The cache data to write to
 * @param[in] address:      The address being accessed
 * @param[out] result:      The operation's result (whether a hit, miss, etc)
 *
 * @return                  The address of the dirty block being kicked out (if
 *                          any), or 0 if no dirty block was kicked out
 */
uint64_t CacheData_Write(cache_data_t data, uint64_t address, result_t * result);

/**@brief   Simulate a write access to the given cache data
 *
 * @param[in,out] data:     The cache data to read from
 * @param[in] address:      The address being accessed
 * @param[out] result:      The operation's result (whether a hit, miss, etc)
 *
 * @return                  The address of the dirty block being kicked out (if
 *                          any), or 0 if no dirty block was kicked out
 */
uint64_t CacheData_Read(cache_data_t data, uint64_t address, result_t * result);

/**@brief   Print the contents of the cache
 *
 * @param[in] data:         The cache data to dump
 */
void CacheData_Print(cache_data_t data);

/** @} defgroup CACHEDATA */

#endif /* ifndef CACHEDATA_H */
