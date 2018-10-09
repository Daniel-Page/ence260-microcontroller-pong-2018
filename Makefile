# File:   Makefile
# Author: Daniel Page and Caleb Smith
# Date:   8 Oct 2018
# Descr:  Makefile for pong

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: pong.out


# Compile: create object files from C source files.
pong.o: pong.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/navswitch.h ../../drivers/led.h ../../utils/pacer.h ../../drivers/button.h ../../utils/spwm.h
	$(CC) -c $(CFLAGS) $< -o $@
	
spwm.o: ../../utils/spwm.c ../../utils/spwm.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@	

button.o: ../../drivers/button.c ../../drivers/button.h ../../drivers/avr/system.h ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@
	
task.o: ../../utils/task.c ../../drivers/avr/system.h ../../utils/task.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@		
	
tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@	
	
font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@
	
ir_serial.o: ../../drivers/ir_serial.c ../../drivers/ir_serial.h ../../drivers/avr/system.h ../../drivers/ir.h
	$(CC) -c $(CFLAGS) $< -o $@

ir.o: ../../drivers/ir.c ../../drivers/ir.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
	
timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@
	
led.o: ../../drivers/led.c ../../drivers/led.h ../../drivers/avr/system.h ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@		

navswitch.o: ../../drivers/navswitch.c ../../drivers/navswitch.h ../../drivers/avr/system.h ../../drivers/avr/pio.h ../../drivers/avr/delay.h
	$(CC) -c $(CFLAGS) $< -o $@	

display.o: ../../drivers/display.c ../../drivers/display.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@	
	
ledmat.o: ../../drivers/ledmat.c ../../drivers/ledmat.h ../../drivers/avr/system.h ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@
	
pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
pong.out: pong.o spwm.o button.o task.o tinygl.o font.o ir_serial.o ir.o pacer.o timer.o led.o navswitch.o display.o ledmat.o pio.o system.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: pong.out
	$(OBJCOPY) -O ihex pong.out pong.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash pong.hex; dfu-programmer atmega32u2 start


