CC = gcc
CFLAGS = -Wall -Wextra -c
LFLAGS = -Wall -Wextra

raycast: main.o raycast.o file_io.o
	$(CC) $(LFLAGS) main.o raycast.o file_io.o -lm -o raycast

main.o: main.c raycast.h file_io.h
	$(CC) $(CFLAGS) main.c

raycast.o: raycast.c raycast.h
	$(CC) $(CFLAGS) raycast.c

file_io.o: file_io.c file_io.h
	$(CC) $(CFLAGS) file_io.c

clean:
	rm -rf raycast *~ *.o
