/*==============================================================================
	CHRP 3.1 user functions and hardware initialization/set-up.

	Initialize user constants in user.h and add user function code here.
==============================================================================*/

#include	"xc.h"				// XC compiler general include file

#include	"stdint.h"			// Integer definition
#include	"stdbool.h"			// Boolean (true/false) definition

#include	"user.h"			// For optional user variable definitions

void initPorts(void)
{
	// TODO Initialize ports and insert user functions here.

	// Initialize user ports and peripherals.

        CMCON0 = 7;
        ANSEL = 0b00000000;			// Make all PORTA pins digital I/O
	OPTION_REG = 0b00010111;	// PORTB pull-ups off, TMR0 internal div 256

	// Set port directions for I/O pins: 0 = Output, 1 = Input

	TRISIO = 0b00011010;			// Set pins 1,3 and 4 as inputs in PORTA
        //TRISIO = 0b00110100;	                                   // Battery pins as inputs?

	// Set starting I/O conditions.

	GPIO = 0;					// Turn off all PORTA outputs, turn on run LED

}
