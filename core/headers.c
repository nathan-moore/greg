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
