# File:   Makefile
# Author: J.P.Brazier, jbr185
# Date:   8 Oct 2018
# Descr:  Makefile for PaperScissorsRock

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I./drivers -I./fonts -I./drivers/avr -I./utils -I./extra
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: PaperScissorsRock.out


# Compile: create object files from C source files.
PaperScissorsRock.o: ./game.c ./graphics.h ./gameLogic.h ./basicIO.h ./sound.h ./drivers/avr/system.h ./utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

graphics.o: ./graphics.c ./drivers/ledmat.h ./utils/tinygl.h ./drivers/avr/pio.h ./drivers/display.h ./fonts/font3x5_1.h ./graphics.h ./utils/uint8toa.h
	$(CC) -c $(CFLAGS) $< -o $@

gameLogic.o: ./gameLogic.c ./gameLogic.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
	
sound.o: ./sound.c ./sound.h ./drivers/avr/pio.h ./extra/tweeter.h ./extra/mmelody.h ./utils/task.c ./utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@
	
basicIO.o: ./basicIO.c ./basicIO.h ./drivers/button.h ./drivers/navswitch.h ./drivers/led.h ./drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@
	
pio.o: ./drivers/avr/pio.c ./drivers/avr/system.h ./drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ./drivers/avr/system.c ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ./drivers/avr/timer.c ./drivers/avr/system.h ./drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

button.o: ./drivers/button.c ./drivers/avr/system.h ./drivers/avr/pio.h ./drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ./drivers/display.c ./drivers/avr/system.h ./drivers/ledmat.h ./drivers/display.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ./drivers/ledmat.c ./drivers/avr/pio.h ./drivers/avr/system.h ./drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ./drivers/navswitch.c ./drivers/avr/system.h ./drivers/avr/delay.h ./drivers/avr/pio.h ./drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@
	
led.o: ./drivers/led.c ./drivers/avr/system.h ./drivers/avr/pio.h ./drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ./utils/font.c ./drivers/avr/system.h ./utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ./utils/pacer.c ./drivers/avr/system.h ./drivers/avr/timer.h ./utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ./utils/tinygl.c ./drivers/avr/system.h ./drivers/display.h ./utils/font.h ./utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@
	
uint8toa.o: ./utils/uint8toa.c ./drivers/avr/system.h ./utils/uint8toa.h
	$(CC) -c $(CFLAGS) $< -o $@
	
mmelody.o: ./extra/mmelody.c ./extra/mmelody.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
	
tweeter.o: ./extra/tweeter.c ./extra/tweeter.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
	
ir_uart.o: ./drivers/avr/ir_uart.c ./drivers/avr/ir_uart.h ./drivers/avr/system.h ./drivers/avr/usart1.h ./drivers/avr/timer0.h ./drivers/avr/pio.h ./drivers/avr/delay.h
	$(CC) -c $(CFLAGS) $< -o $@
	
timer0.o: ./drivers/avr/timer0.c ./drivers/avr/timer0.h ./drivers/avr/system.h ./drivers/avr/bits.h ./drivers/avr/prescale.h ./drivers/avr/timer.o
	$(CC) -c $(CFLAGS) $< -o $@
	
usart1.o: ./drivers/avr/usart1.c ./drivers/avr/usart1.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
	
prescale.o: ./drivers/avr/prescale.c ./drivers/avr/prescale.h ./drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
	
task.o: ./utils/task.c ./utils/task.h ./drivers/avr/system.h ./drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@
	

# Link: create ELF output file from object files.
PaperScissorsRock.out: PaperScissorsRock.o basicIO.o graphics.o gameLogic.o sound.o system.o button.o navswitch.o led.o pacer.o uint8toa.o ledmat.o tinygl.o timer.o display.o font.o mmelody.o tweeter.o ir_uart.o timer0.o usart1.o prescale.o task.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o
	
# Target: clean project.
.PHONY: clean_all
clean_all: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: PaperScissorsRock.out
	$(OBJCOPY) -O ihex PaperScissorsRock.out PaperScissorsRock.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash PaperScissorsRock.hex; dfu-programmer atmega32u2 start


