CC = gcc
CFLAGS = -Wall -Wextra -c
LFLAGS = -Wall -Wextra

raycast: main.o raycast.o vector.o
	$(CC) $(LFLAGS) main.o raycast.o vector.o -lm -o raycast

main.o: main.c raycast.h vector.h
	$(CC) $(CFLAGS) main.c

raycast.o: raycast.c raycast.h
	$(CC) $(CFLAGS) raycast.c

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) vector.c

clean:
	rm -rf raycast *~ *.o
