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

/**@brief   An exception's type */
#define CEXCEPTION_T volatile unsigned int

/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

/**@brief   Handler to execute if an exception is thrown outside of a Try {}
 *          block
 */
#define CEXCEPTION_NO_CATCH_HANDLER(id) UncaughtException(id)

/**@brief   Store some location information with a thrown exception */
#define ThrowWithLocationInfo(e, file, line)    do {                       \
                                                    exception_file = file; \
                                                    exception_line = line; \
                                                    Throw(e);              \
                                                } while (false)

/**@brief   Store the current location with a thrown exception */
#define ThrowHere(e) ThrowWithLocationInfo(e, __FILE__, __LINE__)

/* --- PUBLIC VARIABLES ----------------------------------------------------- */

extern const char * exception_file; /**< File that caused an exception */
extern unsigned int exception_line; /**< Line that caused an exception */

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Handler to execute if an exception is thrown outside of a Try {}
 *          block
 */
void UncaughtException(CEXCEPTION_T id);

/** @} defgroup CEXCEPTIONCONFIG */

#endif /* ifndef CEXCEPTIONCONFIG_H */
