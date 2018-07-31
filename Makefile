CC = gcc
CCFLAGS = -W -Wall

BINARY = stm32-uart-bootloader
SOURCES = Main.c

all:
	$(CC) $(CCFLAGS) $(SOURCES) -o $(BINARY)

clean:
	rm -f $(BINARY)
