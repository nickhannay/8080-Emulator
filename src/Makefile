CC=gcc
CFLAGS = -g -Wall -Werror

all: emu

emu: operations.o shared.o 8080-Emulator.o registers.o main.o
	$(CC) $(CFLAGS) operations.o shared.o 8080-Emulator.o registers.o main.o -o emu

shared.o: shared.c
	$(CC) -c shared.c

operations.o: operations.c
	$(CC) -c operations.c

8080-Emulator.o: 8080-Emulator.c
	$(CC) -c 8080-Emulator.c

registers.o: registers.c
	$(CC) -c registers.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm *.o emu