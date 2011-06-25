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
 * This gets an input character followed by a \r\n echos it back to the computer
 * and sends the command to the motor board
 * Based on the UART command line process by Micheal Hayes on ECE wiki
 *
 */
void blue_read_bluetooth(FILE *stdio_blue, FILE *stream_board, FILE *stdio_cam) {
    char input;
    char error[] = "invalid input\n";

    /* Fetch received data */
    input = fgetc(stdio_blue);
    
    /* Echo input  */
    fprintf(stdio_blue, "%c", input);

    //send appropriate command based on input
    switch (input) {
        case 'w':
            fprintf(stream_board, "%i", CMD_FORWARD);
            break;

        case 'a':
            fprintf(stream_board, "%i", CMD_LEFT);
            break;

        case 's':
            fprintf(stream_board, "%i", CMD_STOP);
            break;

        case 'd':
            fprintf(stream_board, "%i", CMD_RIGHT);
            break;

        case 'x':
            fprintf(stream_board, "%i", CMD_BACK);
            break;

        case 'c':
            //would send command to get image here
            //fprintf(stdio_cam, "%i", CMD_GET_IMAGE);
            break;

        case 'z':
            fprintf(stream_board, "%i", CMD_ASSUME_CTRL);
            break;
        case '\n':
            break;
            
        case '\r':
            break;

        default:
            fprintf(stdio_blue, error);
            break;
    }
}
