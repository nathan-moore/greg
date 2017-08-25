#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cstdio>

#include "readAudio.h"
#include "greg.h"

#define SAMPLEREMOVE 5/6

greg::greg(samples* sIn)
{

	in = sIn;

	std::FILE* temp = std::tmpfile();

	prev = 0;

}

int greg::gUpdate(greg* g)
{
	int32_t avg = 0;
	for(int i = 0;i < g -> in -> n;i++)
	{
		avg += (g -> in -> samples)[i];
	}
	avg = avg / (g -> in -> n);
	int32_t diff = avg - (g -> prev);
	g -> prev = avg;

	size_t write = fwrite(&diff,sizeof(int32_t),1,g -> tmp);
	if(write != 1)
	{
		return 1;
	}

	return 0;
}

int greg::finalize(greg* g,std::ifstream* fout,waveHeaders* in)
{
	gregHeader* GHead = g->initGHead(in);
	if(in -> FMTHead -> sampleRate < GHead -> sampleRate)
	{
		std::cout << "Error, can't increase sampleRate" << std::endl;
		delete GHead;
		return 1;
	}
	
	uint16_t toRemove = in -> FMTHead -> sampleRate / (in -> FMTHead -> sampleRate - GHead -> sampleRate);

	int error = fseek(g -> tmp,0,SEEK_SET);
	if(error != 0)
	{
		delete GHead;
		return 2;
	}	
	size_t read = fwrite(GHead,sizeof(GHead),1,fout);
	delete GHead;
	
	if(read != 1)
	{
		std::cout << "Error writing to file" << std::endl;
		return 2;
	}
	int counter = 0;
	int32_t sample;
	do{
		read = fread(&sample,sizeof(int32_t),1,g -> tmp);
		if(counter % toRemove != 0)
		{
			fwrite(&sample,GHead -> bitsPerSample / 8,1,fout);
		}
		++counter;
	}while(error == 1);

	if(feof(fout) == 0)
	{
		std::cout << "Error with file" << std::endl;
		return 2;
	}

	return 0;
}

gregHeader* greg::initGHead(waveHeaders* in)
{
	gregHeader* rtn = new gregHeader;
	(rtn -> beg).headerSize = sizeof(gregHeader) - sizeof(gregsub);
	rtn -> bitsPerSample = in -> FMTHead -> bitsPerSample;
	rtn -> blockAlign = in -> FMTHead -> blockAlign;

	rtn -> sampleRate = in -> FMTHead -> sampleRate * SAMPLEREMOVE;
	rtn -> byteRate = rtn -> sampleRate * rtn -> bitsPerSample / 8;
	return rtn;
}

void greg::~greg()
{
	tmp.close();
}
