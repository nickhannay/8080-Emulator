CC=gcc
CFLAGS = -g -Wall -Werror 

SRCS = operations.o cpu.o 8080-Emulator.o registers.o Disassembler.o main.o memory.o devices.o display.o
OBJS = $(SRCS:.c=.o)

all: emu

emu: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ `sdl2-config --cflags --libs`

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) emu