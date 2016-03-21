/**
 * @file    Access.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Access Interface
 */

#ifndef ACCESS_H
#define ACCESS_H

/**@defgroup ACCESS Access
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

enum {
    TYPE_INSTR,
    TYPE_WRITE,
    TYPE_READ,
};

/* --- PUBLIC DATATYPES ----------------------------------------------------- */

typedef struct {
    uint8_t type;
    uint64_t address;
    uint32_t n_bytes;
} access_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void Access_ParseLine(const char * line, access_t * access);

/** @} defgroup ACCESS */

#endif /* ifndef ACCESS_H */
