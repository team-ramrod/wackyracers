/*
 * Defines a keyboard pseudo controller that communicates via the debug uart.
 *
 * Simple connect the debug uart to your computer then use the following keymap
 * to pretend to be the remote:
 *
 *     q  =>  CMD_ASSUME_CTRL
 *     w  =>  CMD_FORWARD
 *     e  =>  CMD_GET_IMAGE
 *     a  =>  CMD_LEFT
 *     s  =>  CMD_BACK
 *     d  =>  CMD_RIGHT
 *     x  =>  CMD_STOP
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef void (*keyboard_callback) (cmd_t);

/**
 * Initialises the keyboard module.
 */
void key_init( void );

/**
 * Returns a command that has been recieved by the keyboard module.  If there
 * was no command recieved then this returns CMD_NONE.  If multiple commands
 * are recieved between invocations they are buffered up until an undefined
 * limit.
 *
 * If the callback is set this only returns CMD_NONE.
 *
 * @return Command recieved from the keyboard.
 */
cmd_t key_get_cmd( void );

/**
 * Sets the callback to trigger when a command is received from the keyboard.
 *
 * @param callback The callback to trigger, the cmd_t recieved is passed in as
 *                 the only parameter.
 */
void key_set_callback( keyboard_callback );

/**
 * Clears the callback to allow polling again
 */
void key_clear_callback( void );

#endif /* KEYBOARD_H */
