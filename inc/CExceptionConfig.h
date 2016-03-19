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

#define ThrowWithLocationInfo(e, file, line)    do {                       \
                                                    exception_file = file; \
                                                    exception_line = line; \
                                                    Throw(e);              \
                                                } while (false)
#define ThrowHere(e) ThrowWithLocationInfo(e, __FILE__, __LINE__)

/* --- PUBLIC VARIABLES ----------------------------------------------------- */

extern const char * exception_file;
extern unsigned int exception_line;

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void UncaughtException(CEXCEPTION_T id);

/** @} defgroup CEXCEPTIONCONFIG */

#endif /* ifndef CEXCEPTIONCONFIG_H */
