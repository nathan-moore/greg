CXXFLAGS = -g -Wall -Wshadow  -Wvla -Werror -pedantic
COVFLAGS = -fprofile-arcs -ftest-coverage
PROFFLAG = -pg
GCC = g++ $(CFLAGS) #$(COVFLAGS) $(PROFFLAG)
OBJS = headers.o master.o debug.o readAudio.o greg.o
HEADERS = headers.h debug.h readAudio.h greg.h

makeall:$(OBJS)

%.o: $(HEADERS) %.c
	$(GCC) -c $*.c

clean: 
	/bin/rm -f *.o
	/bin/rm -f *.out
