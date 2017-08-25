#include <stdio.h>
#include <errno.h>

#include <vector>
#include <iostream>
#include <fstream>

#include "headers.h"
#include "debug.h"
#include "readAudio.h"
#include "greg.h"

#define DEBUG 1

void freeFiles(std::vector<std::ifstream> tofree);

int main(int argc,char** argv)
{
	if(argc != 3)
	{
		fprintf(stdout,"Error, please use ./function fileIn.wav fileOut.greg\n");
		return EXIT_FAILURE;
	}

	std::ifstream file_read;
	file_read.open(argv[1],std::ios::binary);
	if(file_read.is_open() != true)
	{
	    int error = errno;
		fprintf(stderr,"Error opening file. Error %i\n",error);
		return EXIT_FAILURE;
	}

	
	std::ifstream f_out;
	f_out.open(argv[2],std::ios::binary);
	if(f_out.is_open() != true)
	{
		int error = errno;
		file_read.close();
		fprintf(stderr,"Error opeing file. Error: %i\n",error);
		return EXIT_FAILURE;
	}
	
	//reads three headers in and does simple header validation
	waveHeaders* waveHead = readWHeaders(&file_read);
	if(waveHead == NULL)
	{
		file_read.close();
		f_out.close();
		return EXIT_FAILURE;
	}

	if(DEBUG == 1)
	{
		printRIFF(waveHead -> RIFFHead);
		printFMT(waveHead -> FMTHead);
		printData(waveHead -> dataHead);
	}

	if(!validateHeads(waveHead,&file_read))
	{
		freeWHead(waveHead);
		file_read.close();
		f_out.close();
		fprintf(stdout,"Error parsing file\n");
		return EXIT_FAILURE;
	}

	//opens up a file stream for each channel
	//allocates the containing array
	//FILE** f_channels = new FILE*[waveHead -> FMTHead -> numChannels];
	std::vector<std::ifstream> f_channels(waveHead -> FMTHead -> numChannels);
	
	//opens channel - 1 streams and stores them
	for(int i = 0,limit = f_channels.size() - 1;i < limit;i++)
	{
		f_channels[i].open(argv[1],std::ios::binary);
		if(f_channels[i] == NULL)
		{
			//gets why the file open would have failed
			int error = errno;
			for(int j = 0;j < i;j++)
			{
				f_channels[i].close();
			}
			fprintf(stdout,"Error opening channel data\n");
			fprintf(stderr,"errno error: %i\n",error);
			file_read.close();
			f_out.close();
			freeWHead(waveHead);
			return EXIT_FAILURE;
		}
	}
	//uses the already open file stream for the last stream
	f_channels[waveHead -> FMTHead -> numChannels - 1] = file_read;

	//opens up a stream for the audio
	audio* Astream;
	samples* dataIn = aOpen(waveHead,f_channels,&Astream);
	if(dataIn == NULL)
	{
		freeFiles(f_channels);
		freeWHead(waveHead);
		f_out.close();
		fprintf(stdout,"error opening audio data\n");
		return EXIT_FAILURE;
	}

	greg* Gtmp = initGreg(dataIn);
	if(Gtmp == NULL)
	{
		freeFiles(f_channels);
		freeWHead(waveHead);
		f_out.close();
		freeAudio(Astream);
		fprintf(stdout,"error converting to .greg\n");
		return EXIT_FAILURE;
	}
	
	int status;//used to store the status of the audio stream, eg file done
	//proccess the audio
	do{
		status = getSample(Astream);
		if(status == 0)
		{
			int error = gUpdate(Gtmp);
			if(error != 0)
			{
				freeFiles(f_channels);
				freeWHead(waveHead);
				f_out.close();
				freeGreg(Gtmp);
				freeAudio(Astream);
				fprintf(stdout,"Error writing to tmp file");
				return EXIT_FAILURE;
			}
		}
	}while(status == 0);
	
	if(status != 1)
	{
		std::cout << "Error reading file" << std::endl;
	}

    status = finalize(Gtmp,&f_out,waveHead);
	if(status != 0)
	{
		freeFiles(f_channels);
		freeWHead(waveHead);
		freeAudio(Astream);
		f_out.close();
		freeGreg(Gtmp);
	    return EXIT_FAILURE;
	}
	
	freeFiles(f_channels);
	freeWHead(waveHead);
	freeAudio(Astream);
	f_out.close();
	freeGreg(Gtmp);
	return EXIT_SUCCESS;
}


void freeFiles(std::vector<std::ifstream> tofree)
{
	for(unsigned int i = 0;i < tofree.size();i++)
	{
		tofree[i].close();
	}
	return;
}

