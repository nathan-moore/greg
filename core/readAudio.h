#ifndef readheader
#define readheader

#include <stdint.h>
#include <vector>
#include "headers.h"

typedef struct samples{
	uint16_t n;//stores number of channels read from
	int32_t* samples;//array of channel's concurrent samples
}samples;

typedef struct audio{
	samples* sample;
	std::vector<FILE*> channels;
	waveHeaders* headers;
	uint32_t numSamplesR;//for file integrety checking
	int* tmp;
}audio;

samples* aOpen(waveHeaders* head,std::vector<FILE*> channels,audio** set);

int getSample(audio* ain);

//frees both the samples returned from aOpen and toFree
//does not close the FILE* channels opened or free array
void freeAudio(audio* toFree);
#endif
