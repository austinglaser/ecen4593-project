/**
 * @file    test_utilites.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   TestUtilites Interface
 */

#ifndef TEST_UTILITES_H
#define TEST_UTILITES_H

/**@defgroup TEST_UTILITES TestUtilites
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

#define ZERO_STRUCT(s) \
    memset(&(s), 0, sizeof(s))

/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/** @} defgroup TEST_UTILITES */

#endif /* ifndef TEST_UTILITES_H */
