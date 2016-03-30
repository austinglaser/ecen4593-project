/**
 * @file    CacheSet.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CacheSet Interface
 */

#ifndef CACHESET_H
#define CACHESET_H

/**@defgroup CACHESET CacheSet
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct _cache_sets_t * cache_sets_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_sets_t CacheSet_Create_Sets(uint32_t n_sets, uint32_t set_len, uint32_t block_size_bytes);
void CacheSet_Destroy_Sets(cache_sets_t sets);
uint32_t CacheSet_Get_NSets(cache_sets_t sets);
uint32_t CacheSet_Get_SetLength(cache_sets_t sets);
uint32_t CacheSet_Get_BlockSize(cache_sets_t sets);
bool CacheSet_Contains(cache_sets_t sets, uint64_t address);
bool CacheSet_Write(cache_sets_t sets, uint64_t address);
uint64_t CacheSet_Insert(cache_sets_t sets, uint64_t address);

/** @} defgroup CACHESET */

#endif /* ifndef CACHESET_H */
