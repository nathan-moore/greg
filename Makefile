CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = gcc $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)
#PARTNAME = ./core/headers.o ./core/master.o ./core/debug.o
PARTNAME = $(wildcard */*.o)

make:
	cd core; make makeall
	$(GCC) $(PARTNAME) -o FileChanger

clean:
	/bin/rm -f *.o
	/bin/rm -f *.out
	/bin/rm -f FileChanger
	cd core;make clean
