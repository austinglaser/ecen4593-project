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
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Access_ParseLine(const char * line, access_t * access)
{
    char type_c;

    sscanf(line, "%c %" SCNx64 " %" SCNu32 "\n", &type_c, &access->address, &access->n_bytes);

    switch (type_c) {
    case 'I':
        access->type = TYPE_INSTR;
        break;

    case 'W':
        access->type = TYPE_WRITE;
        break;

    case 'R':
        access->type = TYPE_READ;
        break;

    default:
        ThrowHere(INVALID_OPERATION);
        break;
    }
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup ACCESS */
