#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "readAudio.h"
#include "greg.h"

greg* initGreg(samples* in)
{
	greg* rtn = new greg;
	if(rtn == NULL)
	{
		return NULL;
	}

	rtn -> in = in;

	rtn -> tmp = fopen("tmp","w+");
	if(rtn -> tmp == NULL)
	{
		free(rtn);
		return NULL;
	}
	rtn -> prev = 0;

	return rtn;
}

int gUpdate(greg* g)
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

void freeGreg(greg* toFree)
{
	fclose(toFree -> tmp);
	delete toFree;
}
