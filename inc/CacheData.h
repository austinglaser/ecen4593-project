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
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct _cache_data_t * cache_data_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_data_t CacheData_Create(uint32_t n_sets, uint32_t set_len, uint32_t block_size_bytes);
void CacheData_Destroy(cache_data_t data);

bool CacheData_Contains(cache_data_t data, uint64_t address);
uint64_t CacheData_Write(cache_data_t data, uint64_t address);
uint64_t CacheData_Read(cache_data_t data, uint64_t address);

/** @} defgroup CACHEDATA */

#endif /* ifndef CACHEDATA_H */
