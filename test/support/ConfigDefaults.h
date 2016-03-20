/**
 * @file    ConfigDefaults.h
 * @author  Austin Glaser <austin@boulderes.com>
 * @brief   ConfigDefaults Interface
 */

#ifndef CONFIGDEFAULTS_H
#define CONFIGDEFAULTS_H

/**@defgroup CONFIGDEFAULTS ConfigDefaults
 * @{ 
 *
 * @brief
 */

/* --- PUBLIC DEPENDENCIES -------------------------------------------------- */

#include "Config.h"

#include <stdbool.h>
#include <stdint.h>

/* --- PUBLIC CONSTANTS ----------------------------------------------------- */
/* --- PUBLIC DATATYPES ----------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */
/* --- PUBLIC VARIABLES ----------------------------------------------------- */
/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */

void SetDefaultConfigValues(config_t * configp);

/** @} defgroup CONFIGDEFAULTS */

#endif /* ifndef CONFIGDEFAULTS_H */
