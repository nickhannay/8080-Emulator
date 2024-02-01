CC=gcc
CFLAGS = -g -Wall -Werror

SRCS = operations.o shared.o 8080-Emulator.o registers.o emulator_disassembler.o main.o
OBJS = $(SRCS:.c=.o)

all: emu

emu: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) emu