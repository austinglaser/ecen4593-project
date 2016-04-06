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
#include "CException.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define VICTIM_SET          (UINT32_MAX)

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
    uint32_t victim_set_len_blocks;
    uint32_t block_size_bytes;
    uint64_t set_mask;
    uint64_t block_mask;
    uint32_t set_index_shift;
    block_t * all_blocks;
    block_t * next_free_block;
    block_t * last_block;
    set_t victim_set;
    set_t sets[];
};

/* --- PRIVATE MACROS ------------------------------------------------------- */

#define for_block_in_set(block, set) for (block = set->newest; block != NULL; block = block->older)

/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static uint64_t CacheData_AccessBlock(cache_data_t data, uint64_t address, bool write_access, result_t * result);

static uint32_t CacheData_GetSetIndex(cache_data_t data, uint64_t address);
static set_t * CacheData_GetSet(cache_data_t data, uint64_t address);
static uint64_t CacheData_BlockAlignAddress(cache_data_t data, uint64_t address);

static block_t * CacheData_AllocateBlock(cache_data_t data);
static block_t * CacheData_Set_GetMatchingBlock(set_t * set, uint64_t address);
static uint64_t CacheData_Set_RemoveBlock(set_t * set, block_t * block);
static void CacheData_Set_InsertBlockAsNewest(set_t * set, block_t * block);

static void CacheData_Set_Print(cache_data_t data, set_t * set, uint32_t set_index);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

cache_data_t CacheData_Create(uint32_t n_sets,
                              uint32_t set_len_blocks,
                              uint32_t block_size_bytes,
                              uint32_t victim_set_len_blocks)
{
    if (!IS_POWER_OF_TWO(n_sets) ||
        !IS_POWER_OF_TWO(set_len_blocks) ||
        !IS_POWER_OF_TWO(block_size_bytes) ||
        (!IS_POWER_OF_TWO(victim_set_len_blocks) && victim_set_len_blocks != 0) ||
        block_size_bytes < 4) {
        return NULL;
    }

    cache_data_t data = (cache_data_t) malloc(sizeof(*data) + sizeof(set_t) * n_sets);
    if (data == NULL) {
        return NULL;
    }

    uint32_t total_cache_blocks = n_sets * set_len_blocks + victim_set_len_blocks;
    data->all_blocks = (block_t *) malloc(sizeof(block_t) * total_cache_blocks);
    if (data->all_blocks == NULL) {
        free(data);
        return NULL;
    }
    data->next_free_block               = data->all_blocks;
    data->last_block                    = data->all_blocks + total_cache_blocks;

    data->n_sets                        = n_sets;
    data->set_len_blocks                = set_len_blocks;
    data->victim_set_len_blocks         = victim_set_len_blocks;
    data->block_size_bytes              = block_size_bytes;

    data->set_index_shift               = HighestBitSet(block_size_bytes);
    data->set_mask                      = (n_sets - 1) << (data->set_index_shift);
    data->block_mask                    = AlignmentMask(block_size_bytes);

    data->victim_set.n_valid_blocks     = 0;
    data->victim_set.newest             = NULL;
    data->victim_set.oldest             = NULL;

    uint32_t i;
    for (i = 0; i < n_sets; i++) {
        data->sets[i].n_valid_blocks    = 0;
        data->sets[i].newest            = NULL;
        data->sets[i].oldest            = NULL;
    }

    for (i = 0; i < total_cache_blocks; i++) {
        data->all_blocks[i].dirty       = false;
        data->all_blocks[i].address     = 0;
        data->all_blocks[i].older       = NULL;
        data->all_blocks[i].newer       = NULL;
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
    block_t * block = CacheData_Set_GetMatchingBlock(set, aligned_address);

    if (block != NULL) {
        return true;
    }

    block_t * victim_block = CacheData_Set_GetMatchingBlock(&(data->victim_set), aligned_address);
    return victim_block != NULL;
}

uint64_t CacheData_Write(cache_data_t data, uint64_t address, result_t * result)
{
    uint64_t aligned_address = CacheData_BlockAlignAddress(data, address);
    return CacheData_AccessBlock(data, aligned_address, true, result);
}

uint64_t CacheData_Read(cache_data_t data, uint64_t address, result_t * result)
{
    uint64_t aligned_address = CacheData_BlockAlignAddress(data, address);
    return CacheData_AccessBlock(data, aligned_address, false, result);
}

void CacheData_Print(cache_data_t data)
{
    uint32_t i;
    for (i = 0; i < data->n_sets; i++) {
        CacheData_Set_Print(data, &(data->sets[i]), i);
    }
    CacheData_Set_Print(data, &(data->victim_set), 0);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static uint64_t CacheData_AccessBlock(cache_data_t data, uint64_t address, bool write_access, result_t * result)
{
    uint64_t dirty_kickout_address = 0;
    set_t * set = CacheData_GetSet(data, address);
    block_t * block = CacheData_Set_GetMatchingBlock(set, address);

    if (block != NULL) {
        // Block already in set
        CacheData_Set_RemoveBlock(set, block);
        *result = RESULT_HIT;
    }
    else if (set->n_valid_blocks < data->set_len_blocks) {
        // Set not full
        block = CacheData_AllocateBlock(data);
        *result = RESULT_MISS;
    }
    else {
        // Set full
        if (data->victim_set_len_blocks == 0) {
            block = set->oldest;
            dirty_kickout_address = CacheData_Set_RemoveBlock(set, block);
            *result = (dirty_kickout_address != 0) ?
                      RESULT_MISS_DIRTY_KICKOUT :
                      RESULT_MISS_KICKOUT;
        }
        else {
            set_t * victim_set = &(data->victim_set);
            block = CacheData_Set_GetMatchingBlock(victim_set, address);

            if (block != NULL) {
                // Block in victim cache
                CacheData_Set_RemoveBlock(victim_set, block);
                *result = RESULT_HIT_VICTIM_CACHE;
            }
            else if (victim_set->n_valid_blocks < data->victim_set_len_blocks) {
                // Victim cache not full
                block = CacheData_AllocateBlock(data);
                *result = RESULT_MISS;
            }
            else {
                // Victim cache full
                block = victim_set->oldest;
                dirty_kickout_address = CacheData_Set_RemoveBlock(victim_set, block);
                *result = (dirty_kickout_address != 0) ?
                          RESULT_MISS_DIRTY_KICKOUT :
                          RESULT_MISS_KICKOUT;
            }

            block_t * oldest = set->oldest;
            CacheData_Set_RemoveBlock(set, oldest);
            CacheData_Set_InsertBlockAsNewest(victim_set, oldest);
        }
    }

    if (write_access) {
        block->dirty = true;
    }
    block->address = address;
    CacheData_Set_InsertBlockAsNewest(set, block);

    return dirty_kickout_address;
}

static uint32_t CacheData_GetSetIndex(cache_data_t data, uint64_t address)
{
    return (address & data->set_mask) >> data->set_index_shift;
}

static set_t * CacheData_GetSet(cache_data_t data, uint64_t address)
{
    uint32_t set_index = CacheData_GetSetIndex(data, address);
    return &(data->sets[set_index]);
}

static uint64_t CacheData_BlockAlignAddress(cache_data_t data, uint64_t address)
{
    return address & data->block_mask;
}

static block_t * CacheData_Set_GetMatchingBlock(set_t * set, uint64_t address)
{
    block_t * block;
    for_block_in_set(block, set) {
        if (block->address == address) {
            return block;
        }
    }

    return NULL;
}

static block_t * CacheData_AllocateBlock(cache_data_t data)
{
    block_t * block = NULL;
    if (data->next_free_block < data->last_block) {
        block = data->next_free_block;
        data->next_free_block += 1;
    }
    else {
        // Could possibly just malloc here. However, this would require special
        // handling of free, and has performance implications.
        ThrowHere(ALLOCATION_FAILURE);
    }
    return block;
}

static uint64_t CacheData_Set_RemoveBlock(set_t * set, block_t * block)
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

    return block->dirty ? block->address : 0;
}

static void CacheData_Set_InsertBlockAsNewest(set_t * set, block_t * block)
{
    if (set->oldest == NULL) {
        set->oldest = block;
    }

    if (set->newest != NULL) {
        set->newest->newer = block;
    }

    block->older = set->newest;
    block->newer = NULL;
    set->newest = block;

    set->n_valid_blocks += 1;
}

static void CacheData_Set_Print(cache_data_t data, set_t * set, uint32_t set_index)
{
    bool is_victim_set = set == &(data->victim_set);
    if (!is_victim_set && set->newest == NULL) {
        return;
    }

    uint32_t n_blocks = data->set_len_blocks;
    if (is_victim_set) {
        printf("Victim cache:\n           ");
        n_blocks = data->victim_set_len_blocks;
    }
    else {
        printf("Index: %4" PRIx32, set_index);
    }

    uint32_t block_index = 0;
    block_t * block;
    const char * addr_str = is_victim_set ? "Addr:" : "Tag: ";
    for_block_in_set(block, set) {
        uint64_t address = block->address;
        if (!is_victim_set) {
            address >>= HighestBitSet(data->n_sets * data->block_size_bytes);
        }
        printf(" | V:%" PRIu32 " D:%" PRIu32 " %s %16" PRIx64 " |",
               1,
               block->dirty ? (uint32_t) 1 : (uint32_t) 0,
               addr_str,
               address);
        if ((block_index % 2) == 1 && block_index != n_blocks - 1) {
            printf("\n           ");
        }
        block_index += 1;
    }
    for (; block_index < n_blocks; block_index++) {
        printf(" | V:0 D:0 %s                - |", addr_str);
        if ((block_index % 2) == 1 && block_index != n_blocks - 1) {
            printf("\n           ");
        }
    }
    printf("\n");
}

/** @} addtogroup CACHEDATA */
