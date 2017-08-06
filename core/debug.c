#include <stdio.h>

#include "debug.h"
#include "headers.h"

void printRIFF(RIFFMHeader* head)
{
	fprintf(stdout,"The chunk id is %i\n",*(head -> header.chunkID));
	fprintf(stdout,"The chunkSize is %i\n",*(head -> header.chunkID));
	fprintf(stdout,"The format is %i\n",*(head -> header.format));
	fprintf(stdout,"The number of wave chunks is %i\n",*(head -> waveChunks));
}
