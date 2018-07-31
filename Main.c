/** @file Main.c
 * Allow to flash STM32 firmware through a serial port using the protocol described in ST application note AN3155.
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

/** Send a command and its payload.
 * @param Command_Code The command code (see application note for available commands).
 * @param Pointer_Buffer_Payload Contain the payload data.
 * @param Payload_Size How many bytes of payload to send.
 */
void MainSendCommand(unsigned char Command_Code, unsigned char *Pointer_Buffer_Payload, int Payload_Size)
{
	unsigned char Checksum = 0;
	
	// Send command code
	SerialPortWriteByte(Serial_Port_ID, Command_Code);
	SerialPortWriteByte(Serial_Port_ID, ~Command_Code); // A command code is followed by the complemented code
	
	// Send payload (if any)
	if (Payload_Size > 0)
	{
		while (Payload_Size > 0)
		{
			SerialPortWriteByte(Serial_Port_ID, *Pointer_Buffer_Payload);
			
			// Update checksum
			Checksum ^= *Pointer_Buffer_Payload;
			
			Pointer_Buffer_Payload++;
			Payload_Size--;
		}
		
		// Send checksum
		SerialPortWriteByte(Serial_Port_ID, Checksum);
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	unsigned char Buffer[512];
	
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
	printf("Waiting for microcontroller...\n");
	SerialPortWriteByte(Serial_Port_ID, 0x7F);
	if (SerialPortReadByte(Serial_Port_ID) != 0x79)
	{
		printf("Error : bad microcontroller answer. Make sure microcontroller is in bootloader mode and baud rate is not too fast.\n");
		return EXIT_FAILURE;
	}
	printf("Successfully connected to microcontroller.\n");
	
	// Display bootloader version
	MainSendCommand(0x01, NULL, 0);
	SerialPortReadBuffer(Serial_Port_ID, Buffer, 5);
	printf("Bootloader version : %d.%d\n", Buffer[1] >> 4, Buffer[1] & 0x0F);
	
	return EXIT_SUCCESS;
}
