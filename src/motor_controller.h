/**
 * @file   motor_controller.h
 * @author Wim Looman
 * @date   20110615
 */

typedef enum {
    VERTICAL_FORWARD;
    VERTICAL_BACKWARD;
    VERTICAL_STOPPED;
} motor_vertical_t;

typedef enum {
    HORIZONTAL_LEFT;
    HORIZONTAL_RIGHT;
    HORIZONTAL_STOPPED;
} motor_horizontal_t;

/**
 * Initialises this module.
 */
void motor_controller_init();

/**
 * Sets the current movement.
 */
void motor_set_movement(motor_vertical_t, motor_horizontal_t);
