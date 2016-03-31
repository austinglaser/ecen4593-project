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

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

typedef struct _block_t {
    bool dirty;
    uint64_t address;
    struct _block_t * newer;
    struct _block_t * older;
} block_t;

typedef struct _set_head_t {
    uint32_t n_valid_blocks;
    struct _block_t * newest;
    struct _block_t * oldest;
} set_head_t;

struct _cache_sets_t {
    uint32_t n_sets;
    uint32_t set_len_blocks;
    uint32_t block_size_bytes;
    uint64_t set_mask;
    uint64_t block_mask;
    uint32_t set_index_shift;
    block_t * all_blocks;
    block_t * next_free_block;
    set_head_t sets[];
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static uint32_t CacheSet_GetSetIndex(cache_sets_t sets, uint64_t address);
static set_head_t * CacheSet_GetSet(cache_sets_t sets, uint64_t address);
static block_t * CacheSet_GetMatchingBlock(set_head_t * set, uint64_t address);
static uint64_t CacheSet_BlockAlignAddress(cache_sets_t sets, uint64_t address);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_sets_t CacheSet_Create_Sets(uint32_t n_sets, uint32_t set_len_blocks, uint32_t block_size_bytes)
{
    if (!IS_POWER_OF_TWO(n_sets) ||
        !IS_POWER_OF_TWO(set_len_blocks) ||
        !IS_POWER_OF_TWO(block_size_bytes) ||
        block_size_bytes < 4) {
        return NULL;
    }

    cache_sets_t sets = (cache_sets_t) malloc(sizeof(*sets) + sizeof(set_head_t) * n_sets);
    if (sets == NULL) {
        return NULL;
    }

    uint32_t total_cache_blocks = n_sets * set_len_blocks;
    sets->all_blocks = (block_t *) malloc(sizeof(block_t) * total_cache_blocks);
    if (!sets->all_blocks) {
        free(sets);
        return NULL;
    }
    sets->next_free_block = sets->all_blocks;

    sets->n_sets = n_sets;
    sets->set_len_blocks = set_len_blocks;
    sets->block_size_bytes = block_size_bytes;

    sets->set_index_shift = HighestBitSet_Uint32(block_size_bytes);
    sets->set_mask = (n_sets - 1) << (sets->set_index_shift);
    sets->block_mask = AlignmentMask(block_size_bytes);

    uint32_t i;
    for (i = 0; i < total_cache_blocks; i++) {
        sets->all_blocks[i].dirty       = false;
        sets->all_blocks[i].address     = 0;
        sets->all_blocks[i].older       = NULL;
    }

    for (i = 0; i < n_sets; i++) {
        sets->sets[i].n_valid_blocks    = 0;
        sets->sets[i].newest            = NULL;
        sets->sets[i].oldest            = NULL;
    }

    return sets;
}

void CacheSet_Destroy_Sets(cache_sets_t cache_sets)
{
    if (cache_sets) {
        if (cache_sets->all_blocks) {
            free(cache_sets->all_blocks);
        }
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
    uint64_t aligned_address = CacheSet_BlockAlignAddress(sets, address);

    set_head_t * set = CacheSet_GetSet(sets, aligned_address);
    block_t * block = CacheSet_GetMatchingBlock(set, aligned_address);

    return block != NULL;
}

bool CacheSet_Write(cache_sets_t sets, uint64_t address)
{
    uint64_t aligned_address = CacheSet_BlockAlignAddress(sets, address);

    set_head_t * set = CacheSet_GetSet(sets, aligned_address);
    block_t * block = CacheSet_GetMatchingBlock(set, aligned_address);

    bool data_present = (block != NULL);
    if (data_present) {
        block->dirty = true;
    }

    return data_present;
}

uint64_t CacheSet_Insert(cache_sets_t sets, uint64_t address)
{
    uint64_t aligned_address = CacheSet_BlockAlignAddress(sets, address);
    set_head_t * set = CacheSet_GetSet(sets, aligned_address);

    block_t * insert_block;
    uint64_t old_address = 0;

    if (set->n_valid_blocks < sets->set_len_blocks) {
        insert_block = sets->next_free_block;

        sets->next_free_block += 1;
        set->n_valid_blocks += 1;
    }
    else {
        block_t * oldest = set->oldest;
        if (oldest->dirty) {
            old_address = oldest->address;
        }
        insert_block = oldest;

        set->oldest = oldest->newer;

        if (set->oldest) {
            oldest->older = NULL;
        }
    }

    insert_block->dirty     = false;
    insert_block->address   = aligned_address;
    insert_block->newer     = NULL;
    insert_block->older     = NULL;

    if (sets->set_len_blocks == 1) {
        set->oldest = insert_block;
    }
    else {
        insert_block->older = set->newest;
    }

    set->newest = insert_block;

    return old_address;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static uint32_t CacheSet_GetSetIndex(cache_sets_t sets, uint64_t address)
{
    return (address & sets->set_mask) >> sets->set_index_shift;
}

static set_head_t * CacheSet_GetSet(cache_sets_t sets, uint64_t address)
{
    uint32_t set_index = CacheSet_GetSetIndex(sets, address);
    return &(sets->sets[set_index]);
}

static block_t * CacheSet_GetMatchingBlock(set_head_t * set, uint64_t address)
{
    block_t * block;
    for (block = set->newest; block != NULL; block = block->older) {
        if (block->address == address) {
            return block;
        }
    }

    return NULL;
}

static uint64_t CacheSet_BlockAlignAddress(cache_sets_t sets, uint64_t address)
{
    return address & sets->block_mask;
}

/** @} addtogroup CACHESET */
