#ifndef gregheader
#define gregheader

#include <stdlib.h>

#include <fstream>

#include "readAudio.h"
#include "headers.h"

class greg{
public:
	std::FILE* tmp;
	samples* in;
	int32_t prev;

	greg(samples* in);
	
	int gUpdate(greg* g);
	void freeGreg(greg* toFree);
	gregHeader* initGHead(waveHeaders* in);
	int finalize(greg* g,std::ifstream* fout,waveHeaders* in);
};


 
#endif
