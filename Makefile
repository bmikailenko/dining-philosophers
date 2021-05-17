INCLUDE=include/
CFLAGS=-c -std=gnu99 -Wall -pedantic -I$(INCLUDE) -g

main.o: main.c
		gcc $(CFLAGS) main.c -o main.o -lm -lpthread

random.o: random.c
		gcc $(CFLAGS) random.c -o random.o -lm

all: main.o random.o
	gcc -o main main.o random.o -lm -lpthread

run:
	./main

clean:
	rm -f main.o random.o main
