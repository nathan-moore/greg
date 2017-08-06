CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = gcc $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)
OBJS = headers.o master.o
HEADERS = headers.h

makeall: $(OBJS)
	$(GCC) -c $(OBJS) -o core.o

.c.o: $(HEADERS)
	$(GCC) -c $*.c

clean: 
	/bin/rm -f *.o
	/bin/rm -f *.out
