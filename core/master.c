#include <stdio.h>
#include <stdlib.h>

#include "headers.h"
#include "debug.h"

#define DEBUG 1

int main(int argc,char** argv)
{
	if(argc != 3)
	{
		fprintf(stdout,"Error, please use ./function fileIn.wav fileOut.greg\n");
	}

	FILE* file_read = fopen(argv[2],"r");
	if(file_read == NULL)
	{
		fprintf(stdout,"Error opening file\n");
		return EXIT_FAILURE;
	}
	
	RIFFMHeader* header_first;
	int error = readRIFF(&header_first,file_read);

	if(DEBUG == 1 && error == 1)
	{
		printRIFF(header_first);
	}

	if(error != 0)
	{
		fprintf(stdout,"The error code is %i\n",error);
		return EXIT_FAILURE;
	}

	free(header_first);

}
