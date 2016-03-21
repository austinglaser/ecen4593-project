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

#include "CException.h"
#include "CExceptionConfig.h"
#include "ExceptionTypes.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */

static bool isValidType(uint8_t type);

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
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

static bool isValidType(uint8_t type)
{
    return (type == TYPE_INSTR) || (type == TYPE_WRITE) || (type == TYPE_READ);
}

/** @} addtogroup ACCESS */
