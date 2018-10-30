###############################################################################
#
# Simple Makefile for Arduino Mega 2560 C projects
#
###############################################################################

# Default programs names and locations
# Use shell command export to define alternatives
# Example: export AVR-CC=/usr/bin/avr-gcc-5.4.0
AVR-CC ?= avr-gcc
AVR-OBJCOPY ?= avr-objcopy
AVRDUDE ?= avrdude
CODE_FORMATTER ?= tooling/format-code.sh

# Microcontroller type and CPU frequency
MCU = atmega2560
MCU_CPU_FREQ = 16000000UL

# Built directory and files
BINDIR = bin
TARGET = $(BINDIR)/$(MCU)-user-code.ihx
ELF = $(BINDIR)/$(MCU)-user-code.elf

# Blank firmware which can be used to "erase" user code
BLANK_FW = tooling/atemega2560-blank-user-code/bin/atmega2560-user-code-blank.ihx

# Arduino device file. Defaults to /dev/ttyACM0
# Use shell command export to define alternative device file
# Example: export ARDUINO=/dev/ttyACM1
ARDUINO ?= /dev/ttyACM0

# Firmware version from git describe
GIT_DESCR=\"$(shell git describe --abbrev=8 --dirty --always --tags --long)\"

# Source files. wildcard "uses" all .c files in src and lib directory
SRCDIR = src
BUILD_LIBS_DIR = lib
SRC = $(wildcard $(SRCDIR)/*.c $(BUILD_LIBS_DIR)/*/*.c)

# Define object files from .c files defined above
OBJ = $(SRC:.c=.o)

# Compiler flags
# Note that those beginning with -D are actually pre-processor definitions.
# -Wall ... -Wfatal-errors almost all possible warning options.
# -Os Optimize code. The special option -Os is meant to turn on all -O2.
# optimisations that are not expected to increase code size.
# -std=c11 use C11 standard.
CFLAGS =	-Wall \
			-Wextra \
			-Wpedantic \
			-Wformat \
			-pedantic-errors \
			-Werror \
			-Wfatal-errors \
			-Os \
			-std=c11 \
			-ffreestanding \
			-mmcu=$(MCU) \
			-DF_CPU=$(MCU_CPU_FREQ) \
			-DFW_VERSION=$(GIT_DESCR)

# Linker flags
LDFLAGS =	-mmcu=$(MCU)

# Object copy arguments
# Do not copy EEPROM section content
OBJCOPYARGS =	-O ihex \
				-R .eeprom

# FIXME Find out why some Arduinos require -D to write user code.
# Do not check device signature (-F)
AVRDUDEARGS =	-p $(MCU) \
				-c wiring \
				-F \
				-P $(ARDUINO) \
				-b 115200 \
				-D

# Default target. Build firmware
all: $(ELF) $(TARGET)

# Build object files from source
%.o : %.c
	$(AVR-CC) -c $(CFLAGS) -o $*.o $<

# Link object files to elf file
$(ELF): $(OBJ)
	$(AVR-CC) $(LDFLAGS) -o $@ $^

# Copy elf to ihx format
$(TARGET):
	$(AVR-OBJCOPY) $(OBJCOPYARGS) $(ELF) $(TARGET)

# Delete all built object files and built firmware files
# Do not remove .gitkeep in BINDIR
# because this directory is needed in repository for built artefacts
clean:
	find $(BINDIR) -type f -not -name '.gitkeep' -print0 | xargs -0 rm -f --
	rm -f $(SRCDIR)/*.o
	rm -fr $(BUILD_LIBS_DIR)/*/*.o

# Delete also code formatter .orig files
dist-clean: clean
	rm -f $(SRCDIR)/*.orig

# Load built firmware to Arduino
install:
	$(AVRDUDE) $(AVRDUDEARGS) -U flash:w:$(TARGET)

# "Erase" user code part by loading blank firmware
erase:
	$(AVRDUDE) $(AVRDUDEARGS) -U flash:w:$(BLANK_FW)

# Format code using code formatter script
format:
	$(CODE_FORMATTER) $(SRCDIR)/*.c

.PHONY: all clean dist-clean install erase format
