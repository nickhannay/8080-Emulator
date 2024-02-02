CC=gcc
CFLAGS = -g -Wall -Werror

SRCS = operations.o cpu.o 8080-Emulator.o registers.o Disassembler.o main.o memory.o devices.o
OBJS = $(SRCS:.c=.o)

all: emu

emu: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) emu