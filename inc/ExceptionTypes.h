/**
 * @file    ExceptionTypes.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   ExceptionTypes Interface
 */

#ifndef EXCEPTIONTYPES_H
#define EXCEPTIONTYPES_H

/**@defgroup EXCEPTIONTYPES ExceptionTypes
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

/**@brief   Exception values */
enum EXCEPTION_TYPES {
    NO_EXCEPTION = 0,           /**< Another way to say there wasn't an exception */
    ARGUMENT_ERROR,             /**< Bad argument to function */
    BAD_CONFIG_PARAM,           /**< Bad value in configuration */
    BAD_CONFIG_FILE,            /**< Invalid configuration file */
    MAX_EXCEPTION_N,            /**< Total number of exception types */
    INVALID_EXCEPTION           /**< An invalid exception */
};

/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */

/**@brief Descriptor strings for exceptions */
extern char const * const exception_descriptors[MAX_EXCEPTION_N];

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/** @} defgroup EXCEPTIONTYPES */

#endif /* ifndef EXCEPTIONTYPES_H */
