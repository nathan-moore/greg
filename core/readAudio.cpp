#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "readAudio.h"

samples* aOpen(waveHeaders* head,std::vector<FILE*> channels,audio** set)
{
	//number of channels
	uint16_t cNum = head -> FMTHead -> numChannels;
	
	audio* toSet = new audio;

	int32_t* rtn = new int32_t[cNum];

	samples* samp = new samples;
	
	int * temp = (int*)malloc(head -> FMTHead -> bitsPerSample / 8);
	if(temp == NULL)
	{
		free(toSet);
		free(rtn);
		free(samp);
		return NULL;
	}
	
	//sets and initializes everything
	toSet -> headers = head;
	toSet -> sample = samp;
	toSet -> numSamplesR = 0;
	toSet -> channels = channels;
	toSet -> tmp = temp;
	
	samp -> samples = rtn;
	samp -> n = cNum;

	//moves the file stream pointers in the right places
	for(int i = 0;i < cNum;i++)
	{
		//stores the channel offset from the beginning of the file
		long offset = (head -> RIFFHead -> chunkSize) - (head -> dataHead -> subChunk2Size)
			+ i * (head -> dataHead -> subChunk2Size) / cNum;
		int error = fseek(channels[i],offset,SEEK_SET);
		if(error != 0)
		{
			freeAudio(toSet);
			return NULL;
		}
	}

	//since everything succeeded, sets the return values
	*set = toSet;
	return samp;
}

//returns the current status of reads. Data is stored in the corresponding initialized samples struct.1 - end of file,2 - everything else. 
int getSample(audio* ain)
{
	//inits temp size of audio bits to read
	//for every channel
	for(int i = 0;i < ain -> sample -> n;i++)
	{
		//reads in the sample
		size_t read = fread(ain -> tmp,
							ain -> headers -> FMTHead -> bitsPerSample / 8,
							1,(ain -> channels)[i]);
		//checks if read succeeded,if not checks if we're at the end of the file
		if(read != 1)
		{
			if(feof((ain -> channels)[i]) != 0)
				return 1;
			else
				return 2;
		}
		//sample data is stored in program as 32 bit int
		(ain -> sample -> samples)[i] = (int32_t) *(ain -> tmp);
	}

	//increments count of samples read
	(ain -> numSamplesR)++;
	return 0;
}

void freeAudio(audio* toFree)
{
	free (toFree -> tmp);
	delete [] (toFree -> sample -> samples);
	delete toFree -> sample;
	delete toFree;
}