/**
 * @file    Access.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Access Source
 *
 * @addtogroup ACCESS
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Access.h"

#include "Util.h"

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static inline bool isValidType(uint8_t type);

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Access_ParseLine(const char * line, access_t * access)
{
    if (line == NULL || access == NULL) {
        ThrowHere(ARGUMENT_ERROR);
    }

    if (sscanf(line, "%c %" SCNx64 " %" SCNu32 "\n", &access->type, &access->address, &access->n_bytes) != 3) {
        ThrowHere(SYNTAX_ERROR);
    }

    if (!isValidType(access->type)) {
        ThrowHere(INVALID_OPERATION);
    }

    if (access->n_bytes == 0) {
        ThrowHere(INVALID_ACCESS_SIZE);
    }
}

void Access_Align(access_t * aligned_access, access_t const * unaligned_access, uint32_t block_size)
{
    aligned_access->type = unaligned_access->type;

    uint64_t alignment_mask = AlignmentMask(block_size);


    uint64_t start_block = unaligned_access->address & alignment_mask;
    uint64_t end_block = ( ( unaligned_access->address +
                             unaligned_access->n_bytes - 1 ) &
                           alignment_mask ) +
                         block_size;
    aligned_access->address = start_block;
    aligned_access->n_bytes = end_block - start_block;
}

void Access_Print(access_t * access)
{
    printf("%c %16" PRIx64 " %" PRIu32 "\n",
           access->type,
           access->address,
           access->n_bytes);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static inline bool isValidType(uint8_t type)
{
    return (type == TYPE_INSTR) || (type == TYPE_WRITE) || (type == TYPE_READ);
}

/** @} addtogroup ACCESS */
