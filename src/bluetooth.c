/** @file   bluetooth.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 *	Bluetooth driver
 *
 *  Status: Not tested
 */

#include "common.h"
#include "uart_comms.h"
#include <stdint.h>
#include <stdlib.h>


/**
 * This gets an input character followed by a \r\n echoes it back to the computer
 * and sends the command to the motor board
 * Based on the UART command line process by Michael Hayes on ECE wiki
 *
 */
void blue_read_bluetooth(FILE *stream_bt, FILE *stream_board, FILE *stdio_cam) {
    char input;
    char error[] = "invalid input";

    /* Fetch received data */
    input = fgetc(stream_bt);
    
    /* Echo input  */
    fprintf(stream_bt, "%c", input);

    //send appropriate command based on input
    switch (input) {
        case 'w':
            fprintf(stream_board, "%i", CMD_FORWARD);
            fprintf(stream_bt, "%c", input);
            break;

        case 'a':
            fprintf(stream_board, "%i", CMD_LEFT);
            fprintf(stream_bt, "%c", input);
            break;

        case 's':
            fprintf(stream_board, "%i", CMD_STOP);
            fprintf(stream_bt, "%c", input);
            break;

        case 'd':
            fprintf(stream_board, "%i", CMD_RIGHT);
            fprintf(stream_bt, "%c", input);
            break;

        case 'x':
            fprintf(stream_board, "%i", CMD_BACK);
            fprintf(stream_bt, "%c", input);
            break;

        case 'c':
            //would send command to get image here
            //fprintf(stdio_cam, "%i", CMD_GET_IMAGE);
            break;

        case 'z':
            fprintf(stream_board, "%i", CMD_ASSUME_CTRL);
            fprintf(stream_bt, "%c", input);
            break;
        case '\n':
            break;
            
        case '\r':
            break;

        default:
            fprintf(stream_bt, "%s: %s\n", error, input);
            break;
    }
}

void blue_init(void) {
    // Set reset and CTS as outputs
    PORTD.DIRSET = bit(2) | bit(1);

    // Hold reset high
    PORTD.OUTSET = bit(2);
    
    // Set Tx as an output
    PORTC.DIRSET = bit(3);
}

bool blue_send_byte(uint8_t byte) {
    return false;
}
