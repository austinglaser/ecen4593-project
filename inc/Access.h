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
    TYPE_INSTR  = 'I',
    TYPE_WRITE  = 'W',
    TYPE_READ   = 'R',
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

void Access_Align(access_t * aligned_access, access_t const * unaligned_access, uint32_t block_size);

void Access_Print(access_t const * access);

/** @} defgroup ACCESS */

#endif /* ifndef ACCESS_H */
