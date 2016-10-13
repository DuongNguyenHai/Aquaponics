SOURCE_DIR = $(HOME)/ti/msp430_gcc

GCC_DIR = $(SOURCE_DIR)/bin

SUPPORT_FILE_DIRECTORY = $(SOURCE_DIR)/include

DEVICE  = msp430g2553
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb

CFLAGS 		= -mmcu=$(DEVICE) -O2 -g
INCLUDES 	= -I $(SUPPORT_FILE_DIRECTORY)
LFLAGS 		= -L $(SUPPORT_FILE_DIRECTORY)

# source file
SOURCES = main.c

# convert name.c to name.o and name.cpp to name.o
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)) )

EXECUTABLE = main

all: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LFLAGS) $(INCLUDES)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS) $(INCLUDES)

debug: all
	$(GDB) $(EXECUTABLE)

clean:
	$(RM) *.o $(EXECUTABLE)