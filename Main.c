/** @file Main.c
 * Allow to flash STM32 firmware using the protocol described in ST application note AN3155.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <Serial_Port.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The serial port identifier. */
TSerialPortID Serial_Port_ID;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Gracefully release the opened serial port on program exit. */
static void MainProgramExit(void)
{
	SerialPortClose(Serial_Port_ID);
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Check parameters
	if (argc != 3)
	{
		printf("Usage : %s Serial_Port Firmware_File\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	// Try to open serial port
	if (SerialPortOpen(argv[1], 1200, SERIAL_PORT_PARITY_EVEN, &Serial_Port_ID) != 0)
	{
		printf("Error : could not open '%s' serial port.\n", argv[1]);
		return EXIT_FAILURE;
	}
	// Automatically close serial port on program exit
	atexit(MainProgramExit);
	
	// Configure device for using UART
	printf("Connecting to microcontroller...\n");
	SerialPortWriteByte(Serial_Port_ID, 0x7F);
	if (SerialPortReadByte(Serial_Port_ID) != 0x79)
	{
		printf("Error : bad microcontroller answer. Make sure microcontroller is in bootloader mode and baud rate is not too fast.\n");
		return EXIT_FAILURE;
	}
	printf("Connection successful.\n");
	
	return EXIT_SUCCESS;
}
