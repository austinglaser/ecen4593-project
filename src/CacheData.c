/**
 * @file    CacheData.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CacheData Source
 *
 * @addtogroup CACHEDATA
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "CacheData.h"

#include "Util.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */

typedef struct _block_t {
    bool dirty;
    uint64_t address;
    struct _block_t * newer;
    struct _block_t * older;
} block_t;

typedef struct _set_t {
    uint32_t n_valid_blocks;
    struct _block_t * newest;
    struct _block_t * oldest;
} set_t;

struct _cache_data_t {
    uint32_t n_sets;
    uint32_t set_len_blocks;
    uint32_t block_size_bytes;
    uint64_t set_mask;
    uint64_t block_mask;
    uint32_t set_index_shift;
    block_t * all_blocks;
    block_t * next_free_block;
    set_t sets[];
};

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static uint32_t CacheData_GetSetIndex(cache_data_t data, uint64_t address);
static set_t * CacheData_GetSet(cache_data_t data, uint64_t address);
static block_t * CacheData_GetMatchingBlock(set_t * set, uint64_t address);
static uint64_t CacheData_BlockAlignAddress(cache_data_t data, uint64_t address);

static uint64_t CacheData_GetBlockForInsertion(cache_data_t data, set_t * set, block_t ** block, uint64_t address);
static void CacheData_RemoveBlock(set_t * set, block_t * block);
static void CacheData_InsertBlock(set_t * set, block_t * block);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_data_t CacheData_Create(uint32_t n_sets, uint32_t set_len_blocks, uint32_t block_size_bytes)
{
    if (!IS_POWER_OF_TWO(n_sets) ||
        !IS_POWER_OF_TWO(set_len_blocks) ||
        !IS_POWER_OF_TWO(block_size_bytes) ||
        block_size_bytes < 4) {
        return NULL;
    }

    cache_data_t data = (cache_data_t) malloc(sizeof(*data) + sizeof(set_t) * n_sets);
    if (data == NULL) {
        return NULL;
    }

    uint32_t total_cache_blocks = n_sets * set_len_blocks;
    data->all_blocks = (block_t *) malloc(sizeof(block_t) * total_cache_blocks);
    if (!data->all_blocks) {
        free(data);
        return NULL;
    }
    data->next_free_block = data->all_blocks;

    data->n_sets = n_sets;
    data->set_len_blocks = set_len_blocks;
    data->block_size_bytes = block_size_bytes;

    data->set_index_shift = HighestBitSet_Uint32(block_size_bytes);
    data->set_mask = (n_sets - 1) << (data->set_index_shift);
    data->block_mask = AlignmentMask(block_size_bytes);

    uint32_t i;
    for (i = 0; i < total_cache_blocks; i++) {
        data->all_blocks[i].dirty       = false;
        data->all_blocks[i].address     = 0;
        data->all_blocks[i].older       = NULL;
    }

    for (i = 0; i < n_sets; i++) {
        data->sets[i].n_valid_blocks    = 0;
        data->sets[i].newest            = NULL;
        data->sets[i].oldest            = NULL;
    }

    return data;
}

void CacheData_Destroy(cache_data_t cache_data)
{
    if (cache_data) {
        if (cache_data->all_blocks) {
            free(cache_data->all_blocks);
        }
        free(cache_data);
    }
}

bool CacheData_Contains(cache_data_t data, uint64_t address)
{
    uint64_t aligned_address = CacheData_BlockAlignAddress(data, address);

    set_t * set = CacheData_GetSet(data, aligned_address);
    block_t * block = CacheData_GetMatchingBlock(set, aligned_address);

    return block != NULL;
}

uint64_t CacheData_Write(cache_data_t data, uint64_t address)
{
    block_t * block;

    uint64_t aligned_address = CacheData_BlockAlignAddress(data, address);
    set_t * set = CacheData_GetSet(data, aligned_address);
    uint64_t kickout_address = CacheData_GetBlockForInsertion(data, set, &block, aligned_address);

    block->dirty     = true;
    block->address   = aligned_address;
    block->newer     = NULL;
    block->older     = NULL;

    CacheData_InsertBlock(set, block);

    return kickout_address;
}

uint64_t CacheData_Read(cache_data_t data, uint64_t address)
{
    block_t * block;

    uint64_t aligned_address = CacheData_BlockAlignAddress(data, address);
    set_t * set = CacheData_GetSet(data, aligned_address);
    uint64_t kickout_address = CacheData_GetBlockForInsertion(data, set, &block, aligned_address);

    block->address   = aligned_address;
    block->newer     = NULL;
    block->older     = NULL;

    CacheData_InsertBlock(set, block);

    return kickout_address;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static uint32_t CacheData_GetSetIndex(cache_data_t data, uint64_t address)
{
    return (address & data->set_mask) >> data->set_index_shift;
}

static set_t * CacheData_GetSet(cache_data_t data, uint64_t address)
{
    uint32_t set_index = CacheData_GetSetIndex(data, address);
    return &(data->sets[set_index]);
}

static block_t * CacheData_GetMatchingBlock(set_t * set, uint64_t address)
{
    block_t * block;
    for (block = set->newest; block != NULL; block = block->older) {
        if (block->address == address) {
            return block;
        }
    }

    return NULL;
}

static uint64_t CacheData_BlockAlignAddress(cache_data_t data, uint64_t address)
{
    return address & data->block_mask;
}

static uint64_t CacheData_GetBlockForInsertion(cache_data_t data, set_t * set, block_t ** block, uint64_t address)
{
    *block = CacheData_GetMatchingBlock(set, address);

    uint64_t kickout_address = 0;

    if (*block != NULL) {
        // Block already in set
        CacheData_RemoveBlock(set, *block);
    }
    else if (set->n_valid_blocks < data->set_len_blocks) {
        // Set not full
        *block = data->next_free_block;
        data->next_free_block += 1;
    }
    else {
        // Set full
        block_t * oldest = set->oldest;
        if (oldest->dirty) {
            kickout_address = oldest->address;
        }
        *block = oldest;

        set->oldest = oldest->newer;

        if (set->oldest) {
            set->oldest->older = NULL;
        }
        else {
            set->newest = NULL;
        }
    }

    return kickout_address;
}

static void CacheData_RemoveBlock(set_t * set, block_t * block)
{
    if (block == set->newest) {
        set->newest = block->older;
    }
    if (block == set->oldest) {
        set->oldest = block->newer;
    }

    if (block->newer) {
        block->newer->older = block->older;
    }
    if (block->older) {
        block->older->newer = block->newer;
    }

    set->n_valid_blocks -= 1;
}

static void CacheData_InsertBlock(set_t * set, block_t * block)
{
    if (set->oldest == NULL) {
        set->oldest = block;
    }

    if (set->newest != NULL) {
        set->newest->newer = block;
    }

    block->older = set->newest;
    set->newest = block;
    set->n_valid_blocks += 1;
}

/** @} addtogroup CACHEDATA */
