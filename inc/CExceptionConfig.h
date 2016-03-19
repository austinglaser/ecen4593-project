/**
 * @file    CExceptionConfig.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   CExceptionConfig Interface
 */

#ifndef CEXCEPTIONCONFIG_H
#define CEXCEPTIONCONFIG_H

/**@defgroup CEXCEPTIONCONFIG CExceptionConfig
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "CException.h"
#include "ExceptionTypes.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

// TODO: Find a way not to have to do this
#define CEXCEPTION_T unsigned int

/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

#define CEXCEPTION_NO_CATCH_HANDLER(id) UncaughtException(id)

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

static inline void UncaughtException(CEXCEPTION_T id) {
    if (id >= MAX_EXCEPTION_N) {
        printf("Unknown Exception! (%u)\n", id);
    }
    else {
        printf("Exception: %s (%u)\n", exception_descriptors[id], id);
    }
    exit(-id);
}

/** @} defgroup CEXCEPTIONCONFIG */

#endif /* ifndef CEXCEPTIONCONFIG_H */
