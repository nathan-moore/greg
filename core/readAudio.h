#ifndef readheader
#define readheader

#include <stdint.h>
#include <vector>
#include "headers.h"

typedef struct samples{
	uint16_t n;//stores number of channels read from
	int32_t* samples;//array of channel's concurrent samples
}samples;

class audio{
private:
		samples* sample;
		std::ifstream* channels;
		waveHeaders* headers;
		uint32_t numSamplesR;//for file integrety checking
		int* tmp;
		struct bad_alloc{};
		unsigned int channelsN;
public:
		audio(waveHeaders* head,std::ifstream* channels,samples** set);
	int getSample();
	
//frees both the samples returned from aOpen and toFree
//does not close the FILE* channels opened or free array
	~audio();
};

#endif
