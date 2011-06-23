/**
 * @file   commander.h
 * @author Henry Jenkins
 * @date   20110623
 */

#ifndef commander_h
#define commander_h

#include "common.h"

/**
 * Initialises the commander module.
 */
void commander_init();

/**
 * This returnes the next command to be passed to the state machine. It handels
 * which source the board should be listening too. This function also disables
 * motors if the charger is plugged in.
 * 
 * @return next command to be run by state machine
 */
cmd_t get_cmd();

#endif
