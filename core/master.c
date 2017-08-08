#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "headers.h"
#include "debug.h"

#define DEBUG 1

int main(int argc,char** argv)
{
	if(argc != 3)
	{
		fprintf(stdout,"Error, please use ./function fileIn.wav fileOut.greg\n");
	}

	FILE* file_read = fopen(argv[1],"r");
	if(file_read == NULL)
	{
	    int error = errno;
		fprintf(stderr,"Error opening file. Error %i\n",error);
		return EXIT_FAILURE;
	}

	//reads three headers in and does simple header validation
	waveHeaders* waveHead = readWHeaders(file_read);
	if(waveHead == NULL)
	{
		return EXIT_FAILURE;
	}

	if(DEBUG == 1)
	{
		printRIFF(waveHead -> RIFFHead);
		printFMT(waveHead -> FMTHead);
		printData(waveHead -> dataHead);
	}
   
	freeWHead(waveHead);
	fclose(file_read);
}
