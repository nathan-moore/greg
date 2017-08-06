#include <stdio.h>

#include "debug.h"
#include "headers.h"

void printRIFF(RIFFHeader* head)
{
	fprintf(stdout,"The chunk id is %i\n",(head -> chunkID));
	fprintf(stdout,"The chunkSize is %i\n",(head -> chunkID));
	fprintf(stdout,"The format is %i\n",(head -> format));
}
