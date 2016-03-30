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

typedef struct block_t {
    bool valid;
    bool dirty;
    uint64_t address;
} block_t;

struct _cache_sets_t {
    uint32_t n_sets;
    uint32_t set_len_blocks;
    uint32_t block_size_bytes;
    uint64_t block_mask;
    block_t blocks[];
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static block_t * CacheSet_GetBlock(cache_sets_t sets, uint64_t address);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_sets_t CacheSet_Create_Sets(uint32_t n_sets, uint32_t set_len_blocks, uint32_t block_size_bytes)
{
    cache_sets_t sets = (cache_sets_t) malloc(sizeof(*sets) + sizeof(block_t) * n_sets);
    if (sets == NULL) {
        return NULL;
    }

    sets->n_sets = n_sets;
    sets->set_len_blocks = set_len_blocks;
    sets->block_size_bytes = block_size_bytes;
    sets->block_mask = n_sets - 1;

    uint32_t i;
    for (i = 0; i < n_sets; i++) {
        sets->blocks[i].valid   = false;
        sets->blocks[i].dirty   = false;
        sets->blocks[i].address = 0;
    }

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
    block_t * block = CacheSet_GetBlock(sets, address);
    return (block->address) == address;
}

bool CacheSet_Write(cache_sets_t sets, uint64_t address)
{
    block_t * block = CacheSet_GetBlock(sets, address);

    bool data_present = block->valid;
    if (data_present) {
        block->dirty = true;
    }

    return data_present;
}

uint64_t CacheSet_Insert(cache_sets_t sets, uint64_t address)
{
    block_t * block = CacheSet_GetBlock(sets, address);

    uint64_t old_address = 0;
    if (block->valid && block->dirty) {
        old_address = block->address;
    }

    block->valid   = true;
    block->dirty   = false;
    block->address = address;

    return old_address;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static block_t * CacheSet_GetBlock(cache_sets_t sets, uint64_t address)
{
    uint32_t block_index = address & sets->block_mask;
    return &(sets->blocks[block_index]);
}

/** @} addtogroup CACHESET */
