/**
 * @file   motor_controller.h
 * @author Wim Looman
 * @date   20110615
 */

typedef enum {
    VERT_STOPPED   = 0,
    VERT_FORWARD   = 3,
    VERT_BACKWARD  = 6,
} motor_vert_t;

typedef enum {
    HORIZ_STOPPED = 0,
    HORIZ_LEFT    = 1,
    HORIZ_RIGHT   = 2,
} motor_horiz_t;

/**
 * Initialises this module.
 */
void motor_controller_init();

/**
 * Sets the current movement.
 */
void motor_set_movement(motor_vert_t, motor_horiz_t);
