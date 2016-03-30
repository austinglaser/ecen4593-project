/**
 * @file    CacheSet.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CacheSet Source
 *
 * @addtogroup CACHESET
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "CacheSet.h"

#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

struct _cache_sets_t {
    uint32_t n_sets;
    uint32_t set_len_blocks;
    uint32_t block_size_bytes;
    bool has_been_inserted;
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_sets_t CacheSet_Create_Sets(uint32_t n_sets, uint32_t set_len_blocks, uint32_t block_size_bytes)
{
    cache_sets_t sets = (cache_sets_t) malloc(sizeof(*sets));
    if (sets == NULL) {
        return NULL;
    }

    sets->n_sets = n_sets;
    sets->set_len_blocks = set_len_blocks;
    sets->block_size_bytes = block_size_bytes;
    sets->has_been_inserted = false;

    return sets;
}

void CacheSet_Destroy_Sets(cache_sets_t cache_sets)
{
    if (cache_sets) {
        free(cache_sets);
    }
}

uint32_t CacheSet_Get_NSets(cache_sets_t sets)
{
    return sets->n_sets;
}

uint32_t CacheSet_Get_SetLength(cache_sets_t sets)
{
    return sets->set_len_blocks;
}

uint32_t CacheSet_Get_BlockSize(cache_sets_t sets)
{
    return sets->block_size_bytes;
}

bool CacheSet_Contains(cache_sets_t sets, uint64_t address)
{
    UNUSED_VARIABLE(address);

    return sets->has_been_inserted;
}

uint64_t CacheSet_Insert(cache_sets_t sets, uint64_t address)
{
    UNUSED_VARIABLE(address);

    sets->has_been_inserted = true;

    return 0;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup CACHESET */
