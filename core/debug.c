#include <stdio.h>

#include "debug.h"
#include "headers.h"

void printRIFF(RIFFHeader* head)
{
	fprintf(stdout,"The chunk id is %i\n",(head -> chunkID));
	fprintf(stdout,"The chunkSize is %i\n",(head -> chunkSize));
	fprintf(stdout,"The format is %i\n\n",(head -> format));
}

void printFMT(FMTHeader* head)
{
	fprintf(stdout,"The subchunk1ID is %08x\n",head -> subchunk1ID);
	fprintf(stdout,"The chunkSize is %i\n",head -> chunkSize);
	fprintf(stdout,"The audio format is %i\n",head -> audioFormat);
	fprintf(stdout,"The number of Channel is %i\n",head -> numChannels);
	fprintf(stdout,"The sample rate is %i\n",head -> sampleRate);
	fprintf(stdout,"The byte rate is %i\n",head -> byteRate);
	fprintf(stdout,"The block Align is %i\n",head -> blockAlign);
	fprintf(stdout,"The bits per sample is %i\n\n",head -> bitsPerSample);
}

void printData(dataHeader* head)
{
	fprintf(stdout,"The subChunk2ID is %08x\n",head -> subChunk2ID);
	fprintf(stdout,"The subChunk2Size is %i\n",head -> subChunk2Size);
}
