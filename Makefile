CC = gcc
CCFLAGS = -W -Wall

BINARY = stm32-uart-bootloader
INCLUDES = -ISerial_Port_Library/Includes
SOURCES = Main.c Serial_Port_Library/Sources/Serial_Port_Linux.c Serial_Port_Library/Sources/Serial_Port_Windows.c

all:
	$(CC) $(CCFLAGS) $(INCLUDES) $(SOURCES) -o $(BINARY)

clean:
	rm -f $(BINARY)
