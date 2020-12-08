CC = gcc
CFLAGS = -Wall -g
LC = gcc
LFLAGS = 

hencode: hencode.o functions.o files.o
	$(LC) -o hencode hencode.o functions.o files.o

hdecode: hdecode.o functions.o files.o
	$(LC) -o hdecode hdecode.o functions.o files.o

hdecode.o: hdecode.c
	$(CC) $(CFLAGS) -c -o hdecode.o hdecode.c

hencode.o: hencode.c 
	$(CC) $(CFLAGS) -c -o hencode.o hencode.c

functions.o: functions.c 
	$(CC) $(CFLAGS) -c -o functions.o functions.c 

files.o: files.c 
	$(CC) $(CFLAGS) -c -o files.o files.c

clean:
	rm *.o

all:
	make hencode
	make hdecode