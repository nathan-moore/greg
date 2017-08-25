#ifndef headers_header
#define headers_header

#define PACKED __attribute__((packed))

#include <stdint.h>
#include <stdbool.h>

#include <fstream>

typedef struct PACKED RIFFHeader{
	uint32_t chunkID;  //should be RIFF in ascii
	uint32_t chunkSize;//size of header chunk past this, 4 + size of waveChunks
	uint32_t format;   //should be WAVE
}RIFFHeader;

typedef struct PACKED FMTHeader{
	uint32_t subchunk1ID;//should be "fmt "
	uint32_t chunkSize;//size of the subchunk following this.16 for PCM
	uint16_t audioFormat;//PCM = 1. Others not supported yet
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
}FMTHeader;

//various other headers

typedef struct PACKED dataHeader{
	uint32_t subChunk2ID;//should be "data"
	uint32_t subChunk2Size;//Equal to numSamples * numChannels * bitsPerSample / 8
}dataHeader;

typedef struct waveHeaders{
	RIFFHeader* RIFFHead;
	FMTHeader* FMTHead;
	dataHeader* dataHead;
}waveHeaders;

typedef struct PACKED gregsub{
	uint32_t magic;
	uint32_t fileSize;
	uint32_t headerSize;
}gregsub;

typedef struct PACKED gregHeader{
	gregsub beg;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint32_t blockAlign;
	uint32_t bitsPerSample;
	uint32_t dataSize;
}gregHeader;
	
//reads all wavefile format headers
waveHeaders* readWHeaders(std::ifstream* fin);

//reads in their respective header. If it reads the header in, it sets
//*header/head to the header. Then returns 0 if the header matches basic
//header marchings, 1 if not
int readRIFF(RIFFHeader** header,std::ifstream* Fin);
int readFMT(FMTHeader** header,std::ifstream* fin);
int readData(dataHeader** head,std::ifstream* fin);

//skips the data stream fin until the next n chars read match match
int skip(std::ifstream* fin, char const* match,unsigned int n);

//initiates struct to hold all the headers
waveHeaders* initWHead();

//frees WHead struct and all initialized headers
void freeWHead(waveHeaders* head);

//returns true if correctly formated
bool validateHeads(waveHeaders* head,std::ifstream* fin);



















#endif
