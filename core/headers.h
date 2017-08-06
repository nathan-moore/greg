#ifndef headers_header
#define headers_header

#define PACKED __attribute__((packed))

#include <stdint.h>

typedef struct PACKED RIFFHeader{
	uint32_t chunkID;  //should be RIFF in ascii
	uint32_t chunkSize;//size of header chunk past this, 4 + size of waveChunks
	uint32_t format;   //should be WAVE
}RIFFHeader;

typedef struct PACKED RIFFMHeader{
	RIFFHeader header;
	size_t waveChunks;//can be variable length; 
}RIFFMHeader;

typedef struct PACKED FMTHeader{
	uint32_t subchunk1ID;//should be "fmt"
	uint32_t chunkSize;//size of the subchunk following this.16 for PCM
	uint16_t audioFormat;//PCM = 1. Others not supported yet
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
}FMTHeader;

typedef struct PACKED dataHeader{
	uint32_t subChunk2ID;//should be "data"
	uint32_t subChunk2Size;//Equal to numSamples * numChannels * bitsPerSample / 8
}dataHeader;

int readRIFF(RIFFMHeader** header,FILE* Fin);






















#endif
