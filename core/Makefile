CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = gcc $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)
OBJS = headers.o master.o debug.o readAudio.o 
HEADERS = headers.h debug.h readAudio.h

makeall:$(OBJS)

%.o: $(HEADERS) %.c
	$(GCC) -c $*.c

clean: 
	/bin/rm -f *.o
	/bin/rm -f *.out
