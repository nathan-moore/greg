#ifndef readheader
#define readheader

#include <stdint.h>
#include "headers.h"

typedef struct samples{
	uint16_t n;//stores number of channels read from
	uint32_t* samples;//array of channel's concurrent samples
}samples;

typedef struct audio{
	samples* samples;
	FILE** channels;
	waveHeaders* headers;
	uint32_t numSamplesR;//for file integrety checking
}audio;

samples* aOpen(waveHeaders* head,FILE* channels[],audio** set);

//frees both the samples returned from aOpen and toFree
//does not close the FILE* channels opened or free array
void freeAudio(audio* toFree);
#endif
