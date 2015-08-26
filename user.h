/*==============================================================================
	Buddy symbolic constants for user.c and main.c functions.
==============================================================================*/

// TODO Add user constant definitions for in user.c and main.c here.
// These definitions give names to all CHRP 3 I/O port devices.

// PORTA I/O devices
#define Q1		GP0		//APPARENTLY PIN 7 IS GP0
#define S1		GP1
#define LED1            GP2             
#define S2              GP3
#define S3              GP4
#define Q2              GP5

#define _XTAL_FREQ		4000000			// Set xtal frequency for time delays
#define FCY				_XTAL_FREQ/4	// Set processor cycle time


void initPorts(void);    // Hardware port configuration and user functions.
