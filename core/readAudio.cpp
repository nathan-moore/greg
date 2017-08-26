#include <stdlib.h>
#include <stdint.h>

#include <fstream>
#include "readAudio.h"

audio::audio(waveHeaders* head,std::ifstream* channelsIn,samples** set)
{
	//number of channels
	uint16_t cNum = head -> FMTHead -> numChannels;

	sample = new samples;
	sample -> samples = new int32_t[cNum];
	
	tmp = (int*)malloc(head -> FMTHead -> bitsPerSample / 8);
	if(tmp == NULL)
	{
		delete[] sample -> samples;
		delete sample;
		throw bad_alloc();
		return;
	}
	
	//sets and initializes everything
	headers = head;
	numSamplesR = 0;
	channels = channelsIn;
	
	sample -> n = cNum;

	*set = sample;
	
	//moves the file stream pointers in the right places
	for(int i = 0;i < cNum;i++)
	{
		//stores the channel offset from the beginning of the file
		long offset = (head -> RIFFHead -> chunkSize) - (head -> dataHead -> subChunk2Size)
			+ i * (head -> dataHead -> subChunk2Size) / cNum;
		//int error = fseek(channels[i],offset,SEEK_SET);
		channels[i].seekg(offset,std::ios_base::beg);
	}
}

//returns the current status of reads. Data is stored in the corresponding initialized samples struct.1 - end of file,2 - everything else. 
int audio::getSample()
{
	//inits temp size of audio bits to read
	//for every channel
	for(int i = 0;i < sample -> n;i++)
	{
		//reads in the sample
		//size_t read = fread(ain -> tmp,
			//ain -> headers -> FMTHead -> bitsPerSample / 8,
			//1,(ain -> channels)[i]);
		channels[i].read((char*)tmp,headers -> FMTHead -> bitsPerSample / 8);
		//checks if read succeeded,if not checks if we're at the end of the file
	    /**
		if(read != 1)
		{
			//if(feof((ain -> channels)[i]) != 0)
			if(channels[i].feof() == true)
				return 1;
			else
				return 2;
		}
		**/
		//sample data is stored in program as 32 bit int
		(sample -> samples)[i] = (int32_t) *tmp;
	}

	//increments count of samples read
	numSamplesR++;
	return 0;
}

audio::~audio()
{
	free (tmp);
	delete [] (sample -> samples);
	delete sample;
}
