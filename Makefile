all: part1

part1: part1.o
	gcc -pthread -o part1 part1.c

clean:
	rm -f part1 *.o