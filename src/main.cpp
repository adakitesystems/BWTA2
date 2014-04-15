#include <StormLib.h>
#include <direct.h>
#include <bitset>

#include "MapData.h"
#include "offline/TileSet.h"
#include "offline/TileType.h"
#include "offline/MiniTileFlags.h"
#include "offline/MyUnitType.h"
#include "offline/sha1.h"


const std::string tileSetName[9] = {
  "badlands",
  "platform",
  "install",
  "ashworld",
  "jungle",
  "desert",
  "ice",
  "twilight",
  "Unknown"
};


// TODO we should instantiate a new Game
namespace BWAPI { Game* Broodwar; }


void printError(const char * archive, const char * message, const char * file, int errnum) {

	char * error = NULL;
	char cCurrentPath[FILENAME_MAX];
	_getcwd(cCurrentPath, sizeof(cCurrentPath));

	switch(errnum) {
		case 105:
			error = (char *)"Bad format"; break;
		case 106:
			error = (char *)"No more files"; break;
		case 107:
			error = (char *)"Handle EOF"; break;
		case 108:
			error = (char *)"Cannot compile"; break;
		case 109:
			error = (char *)"File corrupted"; break;
		default:
			char msg[128];
			strerror_s(msg, sizeof(msg), errnum);
			error = msg;
	}

	std::cout << archive << ": Error: " << message << " \"" << cCurrentPath  << "\\" << file << "\": " << error << "\n";
}


/*
	Check whether the string 'fullString' ends with the string 'ending'
*/
bool hasEnding (const char *fullString, const char *ending)
{
  int l1 = strlen(fullString);
  int l2 = strlen(ending);
  if (l1 >= l2) {
    int start = l1-l2;
    for(int idx = 0;idx<l2;idx++) {
      if (fullString[start+idx] != ending[idx]) return false;
    }
    return true;
  } else {
    return false;
  }
}


/*
	Looks for a file with the .chk extension inside of a .scx map, decompresses it, and returns:
		- a pointer to the data in the .chk file
		- the size of the .chk file (in 'dataSize')
*/
unsigned char *extractCHKfile(const char *archive, DWORD *dataSize) 
{
	HANDLE hMpq       = NULL;          // Open archive handle
	HANDLE hFileFind  = NULL;          // Archived file handle
	SFILE_FIND_DATA SFileFindData;     // Data of the archived file
	bool chkFilefound = false;		   // Whether the chk file was found in the archive
	unsigned char *CHKdata = NULL;

	// Open an archive
	if(!SFileOpenArchive(archive, 0, 0, &hMpq)) {
		printError(archive, "Cannot open archive", archive, GetLastError());
		return NULL;
	}

	// List all files in archive
	hFileFind = SFileFindFirstFile(hMpq, "*", &SFileFindData, NULL);
	while ( hFileFind ) {
		std::cout << SFileFindData.cFileName << "\n";
		if (hasEnding(SFileFindData.cFileName, ".chk")) {
			chkFilefound = true;
			break;
		}
		if ( ! SFileFindNextFile(hFileFind, &SFileFindData) )
			break;
	}

	if (chkFilefound) {
		// std::cout << "CHK file found: " << SFileFindData.cFileName << ", size: " << SFileFindData.dwFileSize << "\n";
		
		// Closing previous file handle
		if ( hFileFind != (HANDLE)0xFFFFFFFF ) 
			SFileFindClose(hFileFind);

		// Open (extract) chk file
		if(!SFileOpenFileEx(hMpq, SFileFindData.cFileName, 0, &hFileFind)) {
			printError(archive, "Cannot extract chk file", archive, GetLastError());
			return NULL;
		}

		// Read chk
		CHKdata = new unsigned char[SFileFindData.dwFileSize];
		DWORD dwBytes = 0;
		SFileReadFile(hFileFind, CHKdata, SFileFindData.dwFileSize, &dwBytes, NULL);
		// std::cout << "Read " << dwBytes << " of " << SFileFindData.dwFileSize << " bytes\n";
		*dataSize = SFileFindData.dwFileSize;
	}

	// Closing handles
	if ( hFileFind != (HANDLE)0xFFFFFFFF ) 
		SFileFindClose(hFileFind);
	if ( hMpq != (HANDLE)0xFFFFFFFF ) 
		SFileCloseArchive(hMpq);

	return CHKdata;
}


/*
	Decodes a 4 byte integer from a string of characters
*/
unsigned long decode4ByteUnsigned(unsigned char *ptr) {
	unsigned long ul = 0;
	ul +=((unsigned long) ptr[0]) << 0;
	ul +=((unsigned long) ptr[1]) << 8;
	ul +=((unsigned long) ptr[2]) << 16;
	ul +=((unsigned long) ptr[3]) << 24;
	return ul;
}


/*
	Decodes a 2 byte integer from a string of characters
*/
unsigned int decode2ByteUnsigned(unsigned char *ptr) {
	unsigned int ui = 0;
	ui +=((unsigned int) ptr[0]) << 0;
	ui +=((unsigned int) ptr[1]) << 8;
	return ui;
}



/*
	Goes over the CHK data printing the different chunk types and their lengths
	(this function is for debugging only)
*/
void printCHKchunks(unsigned char *CHKdata, DWORD size) {
	DWORD position = 0;

	while(position<size) {
		char chunkName[5];
		chunkName[0] = CHKdata[position++];
		chunkName[1] = CHKdata[position++];
		chunkName[2] = CHKdata[position++];
		chunkName[3] = CHKdata[position++];
		chunkName[4] = 0;
		unsigned long chunkLength = decode4ByteUnsigned(CHKdata+position);
		position+=4;

		std::cout << "Chunk '" << chunkName << "', size: " << chunkLength << "\n";

		position+=chunkLength;
	}
}


/*
	Finds a given chunk inside CHK data and returns a pointer to it, and its length (in 'desiredChunkLength')
*/
unsigned char *getChunkPointer(unsigned char *desiredChunk, unsigned char *CHKdata, DWORD size, DWORD *desiredChunkLength) {
	DWORD position = 0;

	while(position<size) {
		char chunkName[5];
		chunkName[0] = CHKdata[position++];
		chunkName[1] = CHKdata[position++];
		chunkName[2] = CHKdata[position++];
		chunkName[3] = CHKdata[position++];
		chunkName[4] = 0;
		unsigned long chunkLength = decode4ByteUnsigned(CHKdata+position);
		position+=4;

		if (strcmp((char *)desiredChunk, (char *)chunkName)==0) {
			*desiredChunkLength = chunkLength;
			return CHKdata + position;
		}
		position+=chunkLength;
	}
	return NULL;
}


/*
	This function gets the dimensions of a StarCraft map from the CHKdata, and returns them in 'width', and 'height'
*/
void getDimensions(unsigned char *CHKdata, DWORD size, unsigned int *width, unsigned int *height) {
	DWORD chunkSize = 0;
	unsigned char *DIMdata = getChunkPointer((unsigned char *)"DIM ", CHKdata, size, &chunkSize);
	
	if (DIMdata!=NULL) {
		*width = decode2ByteUnsigned(DIMdata);
		*height = decode2ByteUnsigned(DIMdata+2);
	}
}


/*
	*** This function is not finished ***

	This function will decode the unit information from a CHK map, and return a list of all the units with their
	types, players and coordinates.
*/
void getUnits(unsigned char *CHKdata, DWORD size) {
	DWORD chunkSize = 0;
	unsigned char *UNITdata = getChunkPointer((unsigned char *)"UNIT", CHKdata, size, &chunkSize);
	
	if (UNITdata!=NULL) {
		int bytesPerUnit = 36;
		int neutralPlayer = 16;

		int nUnits = chunkSize/bytesPerUnit;
		std::cout << "UNIT chunk successfully found, with information about " << nUnits << " units\n";
		for(int i = 0;i<nUnits;i++) {
			int position = (i*bytesPerUnit);
			unsigned long unitClass = decode4ByteUnsigned(UNITdata+position);
			position+=4;
			unsigned int x = decode2ByteUnsigned(UNITdata+position);
			position+=2;
			unsigned int y = decode2ByteUnsigned(UNITdata+position);
			position+=2;
			unsigned int ID = decode2ByteUnsigned(UNITdata+position);
			position+=2;
			position+=2;	// skip relation to another building
			position+=2;	// skip special property flags
			unsigned int mapEditorFlags = decode2ByteUnsigned(UNITdata+position);
			int playerIsValid = mapEditorFlags & 0x0001;	// If this is 0, it is a neutral unit/critter/start location/etc.
			position+=2;	
			int player = (playerIsValid==1 ? UNITdata[position] : neutralPlayer);
			//std::cout << "Unit(" << unitClass << ") ID=" << ID << " at " << x << "," << y << " player " << player << "\n";
      std::cout << "Unit(" << unitClass << ") type=" << BWTA::UnitType::getName(ID) << " at " << x << "," << y << " player " << player << "\n";

			// TODO: how do we translate from the unitClass to a unit type? I have not been able to find information online...
			// ...
		}

	}
}

/*
	This function returns the tilset ID of a StarCraft map from the CHKdata
*/
unsigned int getTileset(unsigned char *CHKdata, DWORD size) {
	DWORD chunkSize = 0;
	unsigned char *ERAdata = getChunkPointer((unsigned char *)"ERA ", CHKdata, size, &chunkSize);

	if (ERAdata!=NULL) {
    // StarCraft masks the tileset indicator's bit value, 
    // so bits after the third place (anything after the value "7") are removed. 
    // Thus, 9 (1001 in binary) is interpreted as 1 (0001), 10 (1010) as 2 (0010), etc. 
    unsigned char tileset = ERAdata[0];
    //std::cout << "ERAdata = " << (std::bitset<8>) tileset << std::endl;
    tileset &= 7;
    //std::cout << "ERAdata = " << (std::bitset<8>) tileset << std::endl;
		return (unsigned int) tileset;
	}
  return 8;
}

/*
	Finds a given chunk inside CHK data and returns a pointer to it, and its length (in 'desiredChunkLength')
*/
unsigned char *getFileBuffer(const char *filename) {
  unsigned char *buffer = NULL;
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (file.is_open()) {
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new unsigned char[size];
    if(!file.read((char *)buffer, size)) {
      printError(filename, "Error reading file", filename, GetLastError());
    }
		file.close();
  } else {
    printError(filename, "Cannot open file", filename, GetLastError());
  }
  return buffer;
}

//------------------------------------------------ GET TILE ------------------------------------------------
TileID getTile(int x, int y)
{
  if ( BWTA::MapData::TileArray )
    return *(BWTA::MapData::TileArray + x + y * BWTA::MapData::mapWidth);
	return 0;
}
//------------------------------------------- GET TILE VARIATION -------------------------------------------
u8 getTileVariation(TileID tileType)
{
  return tileType & 0xF;
}
//--------------------------------------------- GET MINITILE -----------------------------------------------
u16 getMiniTile(int x, int y)
{
	int tx = x / 4;
	int ty = y / 4;
	int mx = x % 4;
	int my = y % 4;
	TileID tileID = getTile(tx, ty);
	TileType* tile = TileSet::getTileType(tileID);
	if ( tile && BWTA::MapData::MiniTileFlags )
		return BWTA::MapData::MiniTileFlags->tile[tile->miniTile[getTileVariation(tileID)]].miniTile[mx + my*4];
	return 0;
}
//-------------------------------------------- SET BUILDABILITY --------------------------------------------
void setBuildability(BWTA::RectangleArray<bool> &buildability)
{
  u16 h = BWTA::MapData::mapHeight;
  u16 w = BWTA::MapData::mapWidth;
  for (unsigned int y = 0; y < h; ++y)
    for (unsigned int x = 0; x < w; ++x) {
      TileID tileID = getTile(x, y);
	    TileType* tile = TileSet::getTileType(tileID);
      buildability[x][y] = (tile->buildability & (1 << 7)) == 0; 
    }
}
//-------------------------------------------- SET WALKABILITY ---------------------------------------------
void setWalkability(BWTA::RectangleArray<bool> &walkability)
{
	u16 h = BWTA::MapData::mapHeight * 4;
	u16 w = BWTA::MapData::mapWidth * 4;
	for (unsigned int y = 0; y < h; ++y)
		for (unsigned int x = 0; x < w; ++x)
			walkability[x][y] = (getMiniTile(x, y) & MiniTileFlags::Walkable) != 0;
	int y = h - 1;
	for(unsigned int x = 0; x < w; ++x) {
		walkability[x][y]   = false;
		walkability[x][y-1] = false;
		walkability[x][y-2] = false;
		walkability[x][y-3] = false;
	}
	y -= 4;
	for(int x = 0; x < 20; ++x) {
		walkability[x][y]   = false;
		walkability[x][y-1] = false;
		walkability[x][y-2] = false;
		walkability[x][y-3] = false;
		walkability[w - x - 1][y]   = false;
		walkability[w - x - 1][y-1] = false;
		walkability[w - x - 1][y-2] = false;
		walkability[w - x - 1][y-3] = false;
	}
}
/*
  Given a center and a offset, it returns the buildability grid around the center
*/
BWTA::RectangleArray<int> getChokeGrid(BWAPI::TilePosition center, int offset)
{
  BWTA::RectangleArray<int> grid;
  unsigned int gridSize = (offset*2)+1;
  int BUILD_TO_WALK_TILE = 4;
  grid.resize(gridSize, gridSize);
//  std::cout << BWTA::MapData::walkability.getWidth() << "," << BWTA::MapData::buildability.getWidth() << std::endl;
  //TODO we don't check offset out of range!!
  for (unsigned int y = 0; y < gridSize; ++y) {
    unsigned int ay = center.y()-offset+y;
    for (unsigned int x = 0; x < gridSize; ++x) {
	  unsigned int ax = center.x()-offset+x;
	  if (ax >= 0 && ay >= 0 &&
		  ax < BWTA::MapData::buildability.getWidth() && 
		  ay < BWTA::MapData::buildability.getHeight()) {
	      grid[x][y] = BWTA::MapData::walkability[(center.x()-offset+x)*BUILD_TO_WALK_TILE]
												 [(center.y()-offset+y)*BUILD_TO_WALK_TILE]
												 +
					   BWTA::MapData::buildability[center.x()-offset+x]
												  [center.y()-offset+y];
	  } else {
		  grid[x][y] = 4;
	  }
    }
  }
  return grid;
}
/*
  Given a chokepoint, it returns the buildability grid around the center of the choke
  and using the width as an offset
*/
BWTA::RectangleArray<int> getChokeGrid(BWTA::Chokepoint* chokepoint)
{
  BWAPI::Position center = chokepoint->getCenter();
  double chokeWidth = chokepoint->getWidth();
//  std::cout << "Choke pixel center (" << center.x() << "," << center.y() << ") width " << chokeWidth << "\n";
  // Translate from pixel position to tile position
  BWAPI::TilePosition centerTile(center);
  int chokeWidthTiles = (int)(chokeWidth/TILE_SIZE);
//  std::cout << "Choke tile center (" << centerTile.x() << "," << centerTile.y() << ") width " << chokeWidthTiles << "\n";
  // generate grid
  return getChokeGrid(centerTile, chokeWidthTiles);
}


void generateWallStartingPointsNoRamp(BWTA::RectangleArray<int> chokeGrid, int s1x, int s1y, int s2x, int s2y, std::ofstream *out) 
{
	int centerx = (s1x + s2x)/2;
	int centery = (s1y + s2y)/2;
	int dx = abs(s2x - s1x);
	int dy = abs(s2y - s1y);
	int sx = 0;
	int sy = 0;

	// Do Bresenham towards each side until a non buildable is found:
	{
		int x = centerx;
		int y = centery;
		int oldx = centerx;
		int oldy = centery;
		if (centerx < s1x) sx = 1;
			  		  else sx = -1;
		if (centery < s1y) sy = 1;
			  		  else sy = -1;
		int err = dx - dy;
//		std::cout << "line " << centerx << "," << centery << " to " << s1x << "," << s1y << std::endl;
		do {
//			std::cout << x << "," << y << std::endl;
			// check for a wall
			if (chokeGrid[x][y]!=2) {
				// extreme 1 found:
				s1x = oldx;
				s1y = oldy;
				break;
			}
			oldx = x;
			oldy = y;
//			if (x==s1x && y==s1y) break;
			int e2 = 2*err;
			if (e2 > -dy) {
				err -= dy;
				x+=sx;
			}
			if (e2<dx) {
				err += dx;
				y+=sy;
			}
			if (x<0 || x>=(int)chokeGrid.getWidth()) break;
			if (y<0 || y>=(int)chokeGrid.getHeight()) break;
		} while(true);
	}
	{
		int x = centerx;
		int y = centery;
		int oldx = centerx;
		int oldy = centery;
		if (centerx < s2x) sx = 1;
			  		  else sx = -1;
		if (centery < s2y) sy = 1;
			  		  else sy = -1;
		int err = dx - dy;
		do {
			// check for a wall
			if (chokeGrid[x][y]!=2) {
				// extreme 2 found:
				s2x = oldx;
				s2y = oldy;
				break;
			}
			oldx = x;
			oldy = y;
//			if (x==s2x && y==s2y) break;
			int e2 = 2*err;
			if (e2 > -dy) {
				err -= dy;
				x+=sx;
			}
			if (e2<dx) {
				err += dx;
				y+=sy;
			}
			if (x<0 || x>=(int)chokeGrid.getWidth()) break;
			if (y<0 || y>=(int)chokeGrid.getHeight()) break;
		} while(true);
	}
	*out << "wall from " << s1x << "," << s1y << " to " << s2x << "," << s2y << std::endl;
}


void generateWallStartingPointsRamp(BWTA::RectangleArray<int> chokeGrid, int s1x, int s1y, int s2x, int s2y, std::ofstream *out)
{
}



void generateWallStartingPoints(BWTA::RectangleArray<int> chokeGrid, int s1x, int s1y, int s2x, int s2y, std::ofstream *out) 
{
	int centerx = (s1x + s2x)/2;
	int centery = (s1y + s2y)/2;
	int centerType = chokeGrid[centerx][centery];

	if (centerType==2) generateWallStartingPointsNoRamp(chokeGrid, s1x, s1y, s2x, s2y, out);
	if (centerType==1) generateWallStartingPointsRamp(chokeGrid, s1x, s1y, s2x, s2y, out);
}



int main (int argc, char * argv[])
{
	// Check the number of parameters
  if (argc < 2) {
    // Tell the user how to run the program
    std::cerr << "Usage: " << argv[0] << " [mapFile]" << std::endl;
    return 1;
  }

  // Save map name
  std::string strName = argv[1];
  unsigned found = strName.find_last_of("/\\");
  BWTA::MapData::mapFileName = strName.substr(found+1);

  std::cout << "=======================\n";
  std::cout << "Testing standalone BWTA\n";
  std::cout << "=======================\n";
	DWORD dataSize = 0;

	unsigned char *CHKdata = extractCHKfile(argv[1], &dataSize);
	if (CHKdata==NULL) return 0;

	std::cout << "Successfully extracted the CHK file, of size " << dataSize << "\n";
	//printCHKchunks(CHKdata, dataSize);

  // Calculate hash
  unsigned char hash[20];
  char hexstring[42];
  sha1::calc(CHKdata, dataSize, hash);
  sha1::toHexString(hash, hexstring);
  BWTA::MapData::hash = std::string(hexstring);

	// Load map dimensions
	unsigned int width = 0, height = 0;
	getDimensions(CHKdata, dataSize, &width, &height);
	std::cout << "Map is " << width << "x" << height << "\n";
	BWTA::MapData::mapWidth = width;
	BWTA::MapData::mapHeight = height;

  // TODO: Load neutral units (minerasl, gas, and starting locations)
	//getUnits(CHKdata, dataSize);

  // Get map tileset ID
  unsigned int tileset = getTileset(CHKdata, dataSize);
  if (tileset > 7) {
    std::cout << "Tileset Unknown (" << tileset << ")\n";
    return 0;
  }
  std::cout << "Map's tilset: " << tileSetName[tileset] << "\n";
  
  // Load TileSet file (tileSetName[tileset].cv5) into BWTA::MapData::TileSet
  std::string cv5FileName = "tileset/"+tileSetName[tileset]+".cv5";
  BWTA::MapData::TileSet = (TileType*)getFileBuffer(cv5FileName.c_str());

  // Load MiniTileFlags file (tileSetName[tileset].vf4) into BWTA::MapData::MiniTileFlags
  std::string vf4FileName = "tileset/"+tileSetName[tileset]+".vf4";
  BWTA::MapData::MiniTileFlags = (BWTA::MapData::MiniTileMaps_type*)getFileBuffer(vf4FileName.c_str());


  // Load Map Tiles
  DWORD chunkSize = 0;
  BWTA::MapData::TileArray = (TileID*)getChunkPointer((unsigned char *)"MTXM", CHKdata, dataSize, &chunkSize);

	// Set walkability
	BWTA::RectangleArray<bool> walkability;
	walkability.resize(BWTA::MapData::mapWidth*4, BWTA::MapData::mapHeight*4);
  setWalkability(walkability);
  BWTA::MapData::isWalkable = walkability;
  // Test walkability data
  {
    std::ofstream fileTxt("logs/walkable.txt");
    u16 h = BWTA::MapData::mapHeight * 4;
    u16 w = BWTA::MapData::mapWidth * 4;
    for (unsigned int y = 0; y < h; ++y) {
      for (unsigned int x = 0; x < w; ++x) {
        fileTxt << walkability[x][y];
      }
 	  fileTxt << std::endl;
    }
    fileTxt.close();
  }

  // Set buildability
  BWTA::RectangleArray<bool> buildability;
  buildability.resize(BWTA::MapData::mapWidth, BWTA::MapData::mapHeight);
  setBuildability(buildability);
  BWTA::MapData::buildability = buildability;
  // Test buildability data
  {
    std::ofstream fileTxt("logs/buildable.txt");
    u16 h = BWTA::MapData::mapHeight;
    u16 w = BWTA::MapData::mapWidth;
    for (unsigned int y = 0; y < h; ++y) {
      for (unsigned int x = 0; x < w; ++x) {
        fileTxt << buildability[x][y];
  	  }
 	  fileTxt << std::endl;
    }
    fileTxt.close();
  }
  delete CHKdata;

  // Normal procedure to analyze map
  std::cout << "Reading map info (low resolution walkable map, ...)\n";
  BWTA::readMap();
  std::cout << "Analyzing map...";
  BWTA::analyze();
  std::cout << "DONE\n";

  // Generatethe grids around all chokepoints:
  std::ofstream fileTxt("logs/output.txt");
  const std::set<BWTA::Chokepoint*> chokePoints = BWTA::getChokepoints();
  for(std::set<BWTA::Chokepoint*>::const_iterator c = chokePoints.begin();
	  c!=chokePoints.end();++c) {
	  BWTA::Chokepoint *cp = (*c);
	  BWTA::RectangleArray<int> chokeGrid = getChokeGrid(cp);
	   // Print grid
	  BWAPI::TilePosition center = BWAPI::TilePosition(cp->getCenter());
	  int radius = int(cp->getWidth()/TILE_SIZE);
	  BWAPI::TilePosition region1 = BWAPI::TilePosition(cp->getRegions().first->getCenter());
	  BWAPI::TilePosition region2 = BWAPI::TilePosition(cp->getRegions().second->getCenter());
	  fileTxt << "Chokepoint:\n";
	  fileTxt << "Region 1: " << radius+region1.x()-center.x() << "," << 
								  radius+region1.y()-center.y() << std::endl;
	  fileTxt << "Region 2: " << radius+region2.x()-center.x() << "," << 
								  radius+region2.y()-center.y() << std::endl;
	  BWAPI::TilePosition side1 = BWAPI::TilePosition(cp->getSides().first);
	  BWAPI::TilePosition side2 = BWAPI::TilePosition(cp->getSides().second);
	  int s1x = radius+side1.x()-center.x();
	  int s1y = radius+side1.y()-center.y();
	  fileTxt << "Side 1: " << s1x << "," << s1y << " is " << chokeGrid[s1x][s1y] << std::endl;
	  int s2x = radius+side2.x()-center.x();
	  int s2y = radius+side2.y()-center.y();
	  fileTxt << "Side 2: " << s2x << "," << s2y << " is " << chokeGrid[s2x][s2y] << std::endl;
	  generateWallStartingPoints(chokeGrid, s1x, s1y, s2x, s2y, &fileTxt);
	  for (unsigned int y = 0; y < chokeGrid.getHeight(); ++y) {
		for (unsigned int x = 0; x < chokeGrid.getWidth(); ++x) {
		  fileTxt << chokeGrid[x][y];
		}
		fileTxt << "\n";
	  }
  }
  fileTxt.close();
  std::cout << "Finished\n";
  return 0;
}
