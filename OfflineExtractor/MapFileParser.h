#pragma once

#include "../BWTA/Source/MapData.h"

#if !defined(_MSCV) && !defined(_MSC_VER) && !defined(WIN32) && !defined(_WINDOWS)
typedef unsigned long DWORD;
#endif

namespace BWTA
{
	const std::string tileSetName[9] = { "badlands", "platform", "install", "ashworld",
										 "jungle", "desert", "ice", "twilight", "Unknown" };

	unsigned char* extractCHKfile(const char* archive, DWORD* dataSize);
	void printCHKchunks(unsigned char* CHKdata, DWORD CHKdataSize);
	unsigned char* getChunkPointer(const char* desiredChunk, DWORD& chunkSize, unsigned char* CHKdata, DWORD CHKdataSize, DWORD& offset);

	void getDimensions(unsigned char* CHKdata, DWORD CHKdataSize, unsigned int& width, unsigned int& height);
	void getUnits(unsigned char* CHKdata, DWORD CHKdataSize);
	void getDoodads(unsigned char* CHKdata, DWORD CHKdataSize);
	unsigned int getTileset(unsigned char* CHKdata, DWORD CHKdataSize);
	unsigned char* getFileBuffer(const char* filename);

	void setOfflineWalkability(RectangleArray<bool> &walkability);
	void setOfflineBuildability(RectangleArray<bool> &buildability);

	bool parseMapFile(const char* mapFilePath);
}
