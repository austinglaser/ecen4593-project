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
 * @brief   This module provides functions to help with parsing and managing
 *          memory accesses.
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

/**@brief   Access types.
 *
 * @note    Matches the types in input traces
 */
enum ACCESS_TYPE {
    TYPE_INSTR  = 'I',      /**< An instruction-type access */
    TYPE_WRITE  = 'W',      /**< A write-type access */
    TYPE_READ   = 'R',      /**< A read-type access */
};

/* --- PUBLIC DATATYPES ----------------------------------------------------- */

/**@brief   The type used internally to represent an access operation */
typedef struct {
    uint8_t type;       /**< The access' type. Member of @ref enum
                             ACCESS_TYPE */
    uint64_t address;   /**< The address being accessed */
    uint32_t n_bytes;   /**< The number of bytes being accessed */
} access_t;

/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**@brief   Reads a line from a trace
 *
 * The line should be of the following format:
 *
 * <I|W|R> <address> <bytes>[\n]
 *
 * <address> is a 64-bit number encoded as hex, and <bytes> is a 32-bit number
 * encoded as decimal.
 *
 * @param[in] line:     The line to be parsed
 * @param[out] access:  A structure for output
 *
 * @throws ARGUMENT_ERROR       When passed invalid (NULL) parameters
 * @throws SYNTAX_ERROR         When the line doesn't match the expected syntax
 *                              (described above)
 * @throws INVALID_OPERATION    When the operation is not one of @ref enum
 *                              ACCESS_TYPE
 * @throws INVALID_ACCESS_SIZE  When one attempts to access zero bytes
 */
void Access_ParseLine(const char * line, access_t * access);

/**@brief   Aligns an access to a block size
 *
 * Expands an access to cover all blocks of size @p block_size.
 *
 * @param[out] aligned_access:      @p unaligned_access, aligned to @p block_size
 * @param[in] unaligned_access:     An access which needs alignment
 * @param[in] block_size:           Block size for alignment
 */
void Access_Align(access_t * aligned_access,
                  access_t const * unaligned_access,
                  uint32_t block_size);

/**@brief   Prints an access
 *
 * Prints to a single line, in a format appropriate for parsing with @ref
 * Access_ParseLine()
 *
 * @param[in] access:   The access to print
 */
void Access_Print(access_t const * access);

/** @} defgroup ACCESS */

#endif /* ifndef ACCESS_H */
