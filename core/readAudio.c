#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "readAudio.h"

samples* aOpen(waveHeaders* head,FILE* channels[],audio** set)
{
	//number of channels
	uint16_t cNum = head -> FMTHead -> numChannels;
	
	audio* toSet = malloc(sizeof(audio));
	if(toSet == NULL)
	{
		return NULL;
	}

	uint32_t* rtn = malloc(sizeof(uint32_t) * cNum);
	if(rtn == NULL)
	{
		free(toSet);
		return NULL;
	}

	samples* samp = malloc(sizeof(samples));
	if(rtn == NULL)
	{
		free(toSet);
		free(rtn);
		return NULL;
	}

	//sets and initializes everything
	toSet -> headers = head;
	toSet -> samples = samp;
	toSet -> numSamplesR = 0;
	toSet -> channels = channels;
	
	samp -> samples = rtn;
	samp -> n = cNum;

	//moves the file stream pointers in the right places
	for(int i = 0;i < cNum;i++)
	{
		long offset = (head -> RIFFHead -> chunkSize) - (head -> dataHead -> subChunk2Size)
			+ i * (head -> dataHead -> subChunk2Size) / cNum;
		int error = fseek(channels[i],offset,SEEK_SET);
		if(error != 0)
		{
			freeAudio(toSet);
			return NULL;
		}
	}

	*set = toSet;
	return samp;
}

void freeAudio(audio* toFree)
{
	free(toFree -> samples -> samples);
	free(toFree -> samples);
	free(toFree);
}
