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
	
	RIFFHeader* RIFFHead;
	int error = readRIFF(&RIFFHead,file_read);

	if(DEBUG == 1)
	{
		printRIFF(RIFFHead);
	}

	if(error != 0)
	{
		fprintf(stderr,"The error code is %i\n",error);

		free(RIFFHead);
		return EXIT_FAILURE;
	}

	FMTHeader* FMTHead;
	error = readFMT(&FMTHead,file_read);
	if(DEBUG == 1)
	{
		printFMT(FMTHead);
	}
	if(error != 0)
	{
		fprintf(stderr,"The error code for FMT is %i\n",error);
		free(FMTHead);
		free(RIFFHead);
		return EXIT_FAILURE;
	}

	error = skip(file_read,"data",4);
	if(error != 0)
	{
		fprintf(stderr,"The error code for skipping is %i\n",error);
		free(RIFFHead);
		free(FMTHead);
		return EXIT_FAILURE;
	}

	dataHeader* dataHead = malloc(sizeof(dataHeader));
	if(dataHead == NULL)
	{
		fprintf(stderr,"malloc failure\n");
		free(RIFFHead);
		free(FMTHead);
		return EXIT_FAILURE;
	}

	error = readData(&dataHead,file_read);

	if(DEBUG == 1)
	{
		printData(dataHead);
	}
	
	if(error != 0)
	{
		fprintf(stderr,"Error with readData, Error code: %i\n",error);
		free(RIFFHead);
		free(FMTHead);
		free(dataHead);
		return EXIT_FAILURE;
	}

	
	free(dataHead);
	free(RIFFHead);
	free(FMTHead);
	fclose(file_read);
}
