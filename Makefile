CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = gcc $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)
PARTS = core
PARTNAME = headers.o master.o

make:
	$(GCC) $(PARTS) -o FileChanger

core:
	+$(MAKE) -C core

clean: 
	/bin/rm -f *.o
	/bin/rm -f *.out
