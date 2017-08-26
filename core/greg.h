#ifndef gregheader
#define gregheader

#include <stdlib.h>

#include <fstream>

#include "readAudio.h"
#include "headers.h"

class greg{
public:
	std::fstream tmp;
	samples* in;
	int32_t prev;

	greg(samples* in);
	
	int gUpdate();
	gregHeader* initGHead(waveHeaders* in);
	int finalize(std::ofstream& fout,waveHeaders* WIn);
	~greg();
};


 
#endif
