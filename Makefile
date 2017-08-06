CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = gcc $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)

make:
	make -C core makeall
	FILES="$(shell echo ./*/*.[o])";$(GCC) $$FILES -o FileChanger


clean:
	/bin/rm -f *.o
	/bin/rm -f *.out
	cd core;make clean
