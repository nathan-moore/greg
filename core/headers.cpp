#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <fstream>

#include "headers.h"

#define DEBUG 0

/*Given a file to read from and an emtpy reference to store the header,
 *it reads in the header and preforms obvious error validation.
 *
 *Returns 0 on success, 1 for header mismatch, 2 for file errors, 3 for
 *everything else
 *In the case of a header mismatch, the header is set.
 */

int readRIFF(RIFFHeader** header,std::ifstream& Fin)
{
	
	RIFFHeader* header_first = new RIFFHeader;

	//reads header in
	//size_t error = fread(header_first,sizeof(RIFFHeader),1,Fin);
	Fin.read((char*)header_first,sizeof(RIFFHeader));
	
	//sets header to header, checks for any mismatches
	*header = header_first;

	if(strncmp((char*)&(header_first -> chunkID),"RIFF",4) != 0)
	{
		return(1);
	}
	if(strncmp((char*)&(header_first -> format),"WAVE",4) != 0)
	{
		return(1);
	}

	return(0);
}


//same codes as previous
int readFMT(FMTHeader** head,std::ifstream& fin)
{
	FMTHeader* header = new FMTHeader;
	
	//size_t error = fread(header,sizeof(FMTHeader),1,fin);
	fin.read((char*) header,sizeof(FMTHeader);
	/**
	if(error != 1)
	{
		fprintf(stdout,"error reading from file\n");

		free(header);
		return 2;
	}
	**/

	*head = header;
	
	if(strncmp((char*)&(header -> subchunk1ID),"fmt ",4) != 0)
	{
		fprintf(stdout,"file header error\n");
		return 1;
	}

	if(header -> chunkSize != (sizeof(FMTHeader) - 8))
	{
		fprintf(stdout,"file header error;file corrupted\n");
		return 1;
	}
	
	if(header -> audioFormat != 1)
	{
		fprintf(stdout,"audioformat not supported\n");
		return 1;
	}

	return 0;
}

//moves the FILE* pointer until the next n chars match the first n chars in match
		int skip(std::ifstream& fin,char const* match,unsigned int n)
{
	char* read = new char[n];
	
	//size_t error = fread(read,sizeof(char),n,fin);
	fin.read(read,sizeof(char) * n);
	
	if(fin.bad = true)
	{
		delete[] read;
		return 2;
	}
	
	
	while(strncmp(match,read,n) != 0)
	{
		for(unsigned int i = 1;i < n;i++)
		{
			read[i - 1] = read[i];
		}

		fin.read(&read[n - 1],sizeof(char));
		if(fin.bad() == true)
		{
			delete[] read;
			return 2;
		}
	}

	delete[] read;
	
	//error = (size_t)fseek(fin,-n * sizeof(char),SEEK_CUR);
	fin.seegk(-n * sizeof(char),std::ios_base::beg);
	if(fin.bad() == true)
	{
		return 2;
	}

	return 0;
}

//reads in Data header form fin and stores it in *head.
//Same error codes as above
int readData(dataHeader** head,std::ifstream& fin)
{
	dataHeader* tmp = new dataHeader;

	//size_t error = fread(tmp,sizeof(dataHeader),1,fin);
	fin.read((char*) tmp,sizeof(dataHeader));
	if(fin.bad() == true)
	{
		delete tmp;
		return 2;
	}

	//if read is successful, we return the data header
	*head = tmp;
	if(strncmp((char*)&(tmp -> subChunk2ID),"data",4) != 0)
	{
		return 1;
	}

	return 0;
}

waveHeaders* initWHead()
{
	waveHeaders* rtn = new waveHeaders;

	rtn -> RIFFHead = NULL;
	rtn -> FMTHead = NULL;
	rtn -> dataHead = NULL;

	return rtn;
}

//frees head and every header in it
void freeWHead(waveHeaders* head)
{
	if(head == NULL)
	{
		return;
	}
	
	if(head -> RIFFHead != NULL)
	{
		delete head -> RIFFHead;
	}
	
	if(head -> FMTHead != NULL)
	{
		delete head -> FMTHead;
	}
	
	if(head -> dataHead != NULL)
	{
		delete head -> dataHead;
	}

	delete head;
	
	return;
}

//reads the RIFF,FMT,and data headers and skips over any header besides those 3
waveHeaders* readWHeaders(std::ifstream fin)
{
	waveHeaders* waveHead = initWHead();
	if(waveHead == NULL)
	{
		return NULL;

	}

	int error = readRIFF(&(waveHead -> RIFFHead),fin);
	if(error != 0)
	{
		//fprintf(stderr,"The error code for RIFF is %i\n",error);
		cerr << "The error code for RIFF is" << error << std:endl;
		freeWHead(waveHead);
		return NULL;
	}

	
	error = readFMT(&(waveHead -> FMTHead),fin);
	if(error != 0)
	{
		fprintf(stderr,"The error code for FMT is %i\n",error);
		freeWHead(waveHead);
		return NULL;
	}

	
	error = skip(fin,"data",4);
	if(error != 0)
	{
		fprintf(stderr,"The error code for skipping is %i\n",error);
		freeWHead(waveHead);
		return NULL;
	}

	
	error = readData(&(waveHead -> dataHead),fin);
	if(error != 0)
	{
		fprintf(stderr,"The error code for data is %i\n",error);
	    freeWHead(waveHead);
		return NULL;
	}

	return waveHead;
}

bool validateHeads(waveHeaders* head,FILE* fin)
{
	uint16_t channels = head -> FMTHead -> numChannels;
	uint16_t bitsPerSample = head -> FMTHead -> bitsPerSample;
	uint32_t dataSize = head -> dataHead -> subChunk2Size;

	//makes sure samples are a multiple of bytes
	if(bitsPerSample % 8 != 0)
	{
		fprintf(stdout,"Error,file format not supported\n");
		fprintf(stderr,"Reason: non int bytes per sample\n");
		return false;
	}

	//makes sure there is an integer number of samples
	if(dataSize % (channels * bitsPerSample / 8) != 0)
	{
		fprintf(stdout,"Error,file is corrupted\n");
		return false;
	}

	//makes sure sample rate and byte rate are correctly related
	if(head -> FMTHead -> byteRate !=
	   (head -> FMTHead -> sampleRate) * (float)channels * bitsPerSample / 8)
	{
		fprintf(stdout,"Error,file is corrupted\n");
		return false;
	}

	//checks to make sure the file cursor is in the right place
	//plus 8 due to the 8 bytes before the total size
	long place = ftell(fin);
	if(place != head -> RIFFHead -> chunkSize - dataSize + 8)
	{
		fprintf(stdout,"Error,   file is corrupted\n");
		if(DEBUG == 1)
		{
			fprintf(stdout,"place: %li Guessed place: %i\n",place,head -> RIFFHead -> chunkSize - dataSize + 8);
		}

		return false;
	}

	return true;
}
