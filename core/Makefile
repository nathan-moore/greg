CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = gcc $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)
OBJS = headers.o master.o debug.o
HEADERS = headers.h debug.h

makeall:$(OBJS)

.c.o: $(HEADERS)
	$(GCC) -c $*.c

clean: 
	/bin/rm -f *.o
	/bin/rm -f *.out
