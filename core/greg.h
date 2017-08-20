#ifndef gregheader
#define gregheader

#include <stdlib.h>
#include <stdio.h>

#include "readAudio.h"
#include "headers.h"

typedef struct greg{
	FILE* tmp;
	samples* in;
	int32_t prev;
}greg;

greg* initGreg(samples* in);
int gUpdate(greg* g);
void freeGreg(greg* toFree);
 
#endif
