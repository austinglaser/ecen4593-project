/**
 * @file    main.c
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   Application Entry Point
 *
 * @defgroup MAIN Main
 * @{
 */

/* --- PRIVATE DEPENDENCIES ------------------------------------------------- */

#include "Config.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* --- PRIVATE DATATYPES ---------------------------------------------------- */
/* --- PRIVATE FUNCTION PROTOTYPES ------------------------------------------ */
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PRIVATE VARIABLES ---------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

/**
 * @brief   Application Entry Point
 */
int main(int argc, char const * const * const argv)
{
    if (argc > 2) {
        printf("Usage: %s [config_file]\n", argv[0]);
        return -1;
    }

    char const * config_file = NULL;
    if (argc == 2) {
        config_file = argv[1];
    }

    config_t config;
    Config_FromFile(config_file, &config);

    Config_Print(&config);

    return 0;
}

/* --- PRIVATE FUNCTION DEFINITIONS ----------------------------------------- */

/** @} defgroup MAIN */
