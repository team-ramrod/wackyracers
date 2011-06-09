/** @file   bluetooth.c
 *  @author Simon Richards
 *  @date   Created: 19 April 2011
 *
 *	Bluetooth driver
 *
 *  Status: Not tested
 */

#include "common.h"
#include "uart.h"
#include <stdint.h>
#include <stdlib.h>
//#include "usart_driver.h"
//#include "avr_compiler.h"

/*! Number of bytes to send. */
#define BLUE_RECEIVE_SIZE  3

/*! Define that selects the Usart used. */
#define BLUE_USART_BT USARTC0
#define BLUE_USART_MOTOR USARTE0

/*! USART data struct for each uart. */
USART_data_t USART_data_BT;
USART_data_t USART_data_MOTOR;

//uint8_t sendArray[NUM_BYTES];

/*! Array to put received data in. */

void blue_init(){
        /* setting up communication to bluetooth*/

  	/* PC3 (TXD0) as output. */
	PORTC.DIRSET   = PIN3_bm;
	/* PC2 (RXD0) as input. */
	PORTC.DIRCLR   = PIN2_bm;

	/* Use USARTC0 and initialize buffers. */
	USART_InterruptDriver_Initialize(&USART_data_BT, &BLUE_USART_BT, USART_DREINTLVL_LO_gc);

	/* USARTC0, 8 Data bits, No Parity, 1 Stop bit. */
	USART_Format_Set(USART_data_BT.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);

	/* Enable RXC interrupt. */
	USART_RxdInterruptLevel_Set(USART_data_BT.usart, USART_RXCINTLVL_LO_gc);

	/* Set Baudrate to 9600 bps:
	 * Use the default I/O clock frequency that is 2 MHz.
	 * Do not use the baudrate scale factor
	 *
	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
	 *                 = 12
	 */
	USART_Baudrate_Set(&BLUE_USART_BT, 12 , 0);

	/* Enable both RX and TX. */
	USART_Rx_Enable(USART_data_BT.usart);
	USART_Tx_Enable(USART_data_BT.usart);

        /* setting up communication to motor*/

  	/* PE3 (TXD0) as output. */
	PORTE.DIRSET   = PIN3_bm;
	/* PE2 (RXD0) as input. */
	PORTE.DIRCLR   = PIN2_bm;

	/* Use USARTC0 and initialize buffers. */
	USART_InterruptDriver_Initialize(&USART_data_MOTOR, &BLUE_USART_MOTOR, USART_DREINTLVL_LO_gc);

	/* USARTC0, 8 Data bits, No Parity, 1 Stop bit. */
	USART_Format_Set(USART_data_MOTOR.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);

	/* Enable RXC interrupt. */
	USART_RxdInterruptLevel_Set(USART_data_MOTOR.usart, USART_RXCINTLVL_LO_gc);

	/* Set Baudrate to 9600 bps:
	 * Use the default I/O clock frequency that is 2 MHz.
	 * Do not use the baudrate scale factor
	 *
	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
	 *                 = 12
	 */
	USART_Baudrate_Set(&BLUE_USART_MOTOR, 12 , 0);

	/* Enable both RX and TX. */
	USART_Rx_Enable(USART_data_MOTOR.usart);
	USART_Tx_Enable(USART_data_MOTOR.usart);

	/* Enable PMIC interrupt level low. */
	PMIC.CTRL |= PMIC_LOLVLEX_bm;

	/* Enable global interrupts. */
	sei();
}

/**
 * This gets an input character followed by a \r\n echos it back to the computer
 * and sends the command to the motor board
 * Based on the UART command line process by Micheal Hayes on ECE wiki
 *
 */
inline void blue_read_bluetooth(void) {
    char command;
    char input[BLUE_RECEIVE_SIZE];
    char error[] = "invalid input\n";
    bool byteToBuffer;
    int i;

    /* Fetch received data as it is received. */
    i = 0;
    while (i < BLUE_RECEIVE_SIZE) {
            if (USART_RXBufferData_Available(&USART_data_BT)) {
                    input[i] = USART_RXBuffer_GetByte(&USART_data_BT);
                    i++;
            }
    }

    /* Echo input  */
    i = 0;
    while (i < BLUE_RECEIVE_SIZE) {
            byteToBuffer = USART_TXBuffer_PutByte(&USART_data_BT, input[i]);
            if(byteToBuffer){
                    i++;
            }
    }

    //send appropriate command based on input
    switch (input[0]) {
        case 'w':
            command = CMD_FORWARD;
            break;

        case 'a':
            command = CMD_LEFT;
            break;

        case 's':
            command = CMD_STOP;
            break;

        case 'd':
            command = CMD_RIGHT;
            break;

        case 'x':
            command = CMD_BACK;
            break;

        case 'c':
            command = CMD_GET_IMAGE;
            break;

        case 'z':
            command = CMD_TOGGLE;
            break;

        default:
            command = CMD_NONE;
            break;
    }

    /* Send command to motor */
    if(command != CMD_NONE)
    {
	i = 0;
	while (i < sizeof(command)) {
		byteToBuffer = USART_TXBuffer_PutByte(&USART_data_MOTOR, command);
		if(byteToBuffer){
			i++;
		}
	}
    }
    else
    {
	i = 0;
	while (i < sizeof(error)) {
		byteToBuffer = USART_TXBuffer_PutByte(&USART_data_BT, error[i]);
		if(byteToBuffer){
			i++;
		}
	}
    }
}

/*! \brief Receive complete interrupt service routine.
 *
 *  Receive complete interrupt service routine.
 *  Calls the common receive complete handler with pointer to the correct USART
 *  as argument.
 */
ISR(USARTC0_RXC_vect)
{
	USART_RXComplete(&USART_data_BT);
}

ISR(USARTE0_RXC_vect)
{
	USART_RXComplete(&USART_data_MOTOR);
}


/*! \brief Data register empty  interrupt service routine.
 *
 *  Data register empty  interrupt service routine.
 *  Calls the common data register empty complete handler with pointer to the
 *  correct USART as argument.
 */
ISR(USARTC0_DRE_vect)
{
	USART_DataRegEmpty(&USART_data_BT);
}

ISR(USARTE0_DRE_vect)
{
	USART_DataRegEmpty(&USART_data_MOTOR);
}

