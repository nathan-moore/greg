#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

#include "headers.h"
#include "debug.h"
#include "readAudio.h"
#include "greg.h"

#define DEBUG 1

void freeFiles(std::vector<FILE*> tofree);

int main(int argc,char** argv)
{
	if(argc != 3)
	{
		fprintf(stdout,"Error, please use ./function fileIn.wav fileOut.greg\n");
	}

	FILE* file_read = fopen(argv[1],"r");
	if(file_read == NULL)
	{
	    int error = errno;
		fprintf(stderr,"Error opening file. Error %i\n",error);
		return EXIT_FAILURE;
	}

	FILE* f_out = fopen(argv[2],"w");
	if(f_out == NULL)
	{
		int error = errno;
		fclose(file_read);
		fprintf(stderr,"Error opeing file. Error: %i\n",error);
		return EXIT_FAILURE;
	}
	
	//reads three headers in and does simple header validation
	waveHeaders* waveHead = readWHeaders(file_read);
	if(waveHead == NULL)
	{
		fclose(file_read);
		fclose(f_out);
		return EXIT_FAILURE;
	}

	if(DEBUG == 1)
	{
		printRIFF(waveHead -> RIFFHead);
		printFMT(waveHead -> FMTHead);
		printData(waveHead -> dataHead);
	}

	if(!validateHeads(waveHead,file_read))
	{
		freeWHead(waveHead);
		fclose(file_read);
		fclose(f_out);
		fprintf(stdout,"Error parsing file\n");
		return EXIT_FAILURE;
	}

	//opens up a file stream for each channel
	//allocates the containing array
	//FILE** f_channels = new FILE*[waveHead -> FMTHead -> numChannels];
	std::vector<FILE*> f_channels(waveHead -> FMTHead -> numChannels);
	
	//opens channel - 1 streams and stores them
	for(int i = 0,limit = f_channels.size() - 1;i < limit;i++)
	{
		f_channels[i] = fopen(argv[1],"r");
		if(f_channels[i] == NULL)
		{
			//gets why the file open would have failed
			int error = errno;
			for(int j = 0;j < i;j++)
			{
				fclose(f_channels[i]);
			}
			fprintf(stdout,"Error opening channel data\n");
			fprintf(stderr,"errno error: %i\n",error);
			fclose(file_read);\
			fclose(f_out);
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
		fclose(f_out);
		fprintf(stdout,"error opening audio data\n");
		return EXIT_FAILURE;
	}

	greg* Gtmp = initGreg(dataIn);
	if(Gtmp == NULL)
	{
		freeFiles(f_channels);
		freeWHead(waveHead);
		fclose(f_out);
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
				fclose(f_out);
				freeGreg(Gtmp);
				freeAudio(Astream);
				fprintf(stdout,"Error writing to tmp file");
				return EXIT_FAILURE;
			}
		}
	}while(status == 0);
	
	//deal with other status exit codes
	//check to make sure number of sample reads matches expected

	//finalize greg
	
	freeFiles(f_channels);
	freeWHead(waveHead);
	freeAudio(Astream);
	fclose(f_out);
	freeGreg(Gtmp);
	
}


void freeFiles(std::vector<FILE*> tofree)
{
	for(int i = 0;i < tofree.size();i++)
	{
		fclose(tofree[i]);
	}
	return;
}

