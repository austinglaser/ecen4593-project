/**
 * @file    CExceptionConfig.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CExceptionConfig Source
 *
 * @addtogroup CEXCEPTIONCONFIG
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "CExceptionConfig.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */

const char * exception_file = NULL;
unsigned int exception_line = 0;

/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void UncaughtException(CEXCEPTION_T id) {
    if (id >= MAX_EXCEPTION_N) {
        printf("%s:%u: Unknown Exception %u!\n", exception_file,
                                                 exception_line,
                                                 id);
    }
    else {
        printf("%s:%u: %s (%u)\n", exception_file,
                                   exception_line,
                                   exception_descriptors[id],
                                   id);
    }

    exit(-id);
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} addtogroup CEXCEPTIONCONFIG */
