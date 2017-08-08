#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers.h"


/*Given a file to read from and an emtpy reference to store the header,
 *it reads in the header and preforms obvious error validation.
 *
 *Returns 0 on success, 1 for header mismatch, 2 for file errors, 3 foreverything else
 *In the case of a header mismatch, the header is set.
 */

int readRIFF(RIFFHeader** header,FILE* Fin)
{
	
	RIFFHeader* header_first = malloc(sizeof(RIFFHeader));
	if(header_first == NULL)
	{
		return 3;
	}

	//reads header in
	size_t error = fread(header_first,sizeof(RIFFHeader),1,Fin);
	if (error != 1)
	{
		free(header_first);
		fprintf(stdout,"Error reading from file\n");
		return 2;
	}
	
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
int readFMT(FMTHeader** head,FILE* fin)
{
	FMTHeader* header = malloc(sizeof(FMTHeader));
	if(header == NULL)
	{
		return 3;
	}
	
	size_t error = fread(header,sizeof(FMTHeader),1,fin);
	if(error != 1)
	{
		fprintf(stdout,"error reading from file\n");

		free(header);
		return 2;
	}

	*head = header;
	
	if(strncmp((char*)&(header -> subchunk1ID),"fmt ",4) != 0)
	{
		fprintf(stdout,"file header error\n");
		return 1;
	}
	if(header -> audioFormat != 1)
	{
		fprintf(stdout,"audioformat not supported\n");
		return 1;
	}

	return 0;
}

//positions the FILE* pointer until the next n chars match the first n chars in match
int skip(FILE* fin,const char* match,int n)
{
	char* read = malloc(sizeof(char) * n);
	if(read == NULL)
	{
		return 1;
	}

	size_t error = fread(read,sizeof(char),n,fin);
	if(error != n)
	{
		free(read);
		return 2;
	}

	while(strncmp(match,read,n) != 0)
	{
		for(int i = 1;i < n;i++)
		{
			read[i - 1] = read[i];
		}

		error = fread(&read[n - 1],sizeof(char),1,fin);
		if(error != 1)
		{
			free(read);
			return 2;
		}
	}

	free(read);
	
	error = (size_t)fseek(fin,-n * sizeof(char),SEEK_CUR);
	if(error != 0)
	{
		return 2;
	}

	return 0;
}

int readData(dataHeader** head,FILE* fin)
{
	dataHeader* tmp = malloc(sizeof(dataHeader));
	if(tmp == NULL)
	{
		return 3;
	}

	size_t error = fread(tmp,sizeof(dataHeader),1,fin);
	if(error != 1)
	{
		free(tmp);
		return 2;
	}

	*head = tmp;
	if(strncmp((char*)&(tmp -> subChunk2ID),"data",4) != 0)
	{
		return 1;
	}

	return 0;
}
