/**
 * @file   motor_controller.h
 * @author Wim Looman
 * @date   20110615
 */

typedef enum {
    VERTICAL_FORWARD   = 1,
    VERTICAL_BACKWARD  = 2,
    VERTICAL_STOPPED   = 4,
} motor_vertical_t;

typedef enum {
    HORIZONTAL_LEFT    = 8,
    HORIZONTAL_RIGHT   = 16,
    HORIZONTAL_STOPPED = 32
} motor_horizontal_t;

/**
 * Initialises this module.
 */
void motor_controller_init();

/**
 * Sets the current movement.
 */
void motor_set_movement(motor_vertical_t, motor_horizontal_t);
