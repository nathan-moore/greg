#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <cstdio>

#include "readAudio.h"
#include "greg.h"

#define SAMPLEREMOVE 5/6

greg::greg(samples* sIn)
{

	in = sIn;
	tmp.open(tmpnam(NULL),std::ios::in | std::ios::out | std::ios::binary);
	prev = 0;

}

int greg::gUpdate()
{
	int32_t avg = 0;
	for(int i = 0;i <in -> n;i++)
	{
		avg += ( in -> samples)[i];
	}
	avg = avg / (in -> n);
	int32_t diff = avg - (prev);
	prev = avg;

	//size_t write = fwrite(&diff,sizeof(int32_t),1,tmp);
	tmp.write((char*) &diff,sizeof(int32_t));

	return 0;
}

int greg::finalize(std::ofstream& fout,waveHeaders* WIn)
{
	gregHeader* GHead = initGHead(WIn);
	if(WIn -> FMTHead -> sampleRate < GHead -> sampleRate)
	{
		std::cout << "Error, can't increase sampleRate" << std::endl;
		delete GHead;
		return 1;
	}
	
	uint16_t toRemove = WIn -> FMTHead -> sampleRate / (WIn -> FMTHead -> sampleRate - GHead -> sampleRate);

	//int error = fseek(tmp,0,SEEK_SET);
	tmp.seekg(0,std::ios_base::beg);
    
	fout.write((char*)GHead,sizeof(gregHeader));
	delete GHead;
	
	int counter = 0;
	int32_t sample;
	do{
		//fread(&sample,sizeof(int32_t),1,tmp);
		tmp.read((char*) &sample,sizeof(int32_t));
		if(counter % toRemove != 0 && tmp.eof() == 0)
		{
			//fwrite(&sample,GHead -> bitsPerSample / 8,1,fout);
			fout.write((char*) &sample,GHead -> bitsPerSample / 8);
		}
		++counter;
	}while(tmp.eof() == false && tmp.fail() == false);

	if(fout.eof() == 0)
	{
		std::cout << "Error with file" << std::endl;
		return 2;
	}

	return 0;
}

gregHeader* greg::initGHead(waveHeaders* WIn)
{
	gregHeader* rtn = new gregHeader;
	(rtn -> beg).headerSize = sizeof(gregHeader) - sizeof(gregsub);
	rtn -> bitsPerSample = WIn -> FMTHead -> bitsPerSample;
	rtn -> blockAlign = WIn -> FMTHead -> blockAlign;

	rtn -> sampleRate = WIn -> FMTHead -> sampleRate * SAMPLEREMOVE;
	rtn -> byteRate = rtn -> sampleRate * rtn -> bitsPerSample / 8;
	return rtn;
}

greg::~greg()
{
	tmp.close();
}
