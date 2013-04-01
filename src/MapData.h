#pragma once

#include <BWTA.h>

namespace BWTA
{
	typedef std::list<Chokepoint*> ChokePath;
	typedef std::set< std::pair<Chokepoint*, int> > ChokeCost;
	typedef std::map<Chokepoint*, ChokeCost> ChokepointGraph;

	namespace MapData
	{
		extern std::set<BWAPI::Unit*> minerals;
		extern std::set<BWAPI::Unit*> rawMinerals;
		extern std::set<BWAPI::Unit*> geysers;
		extern RectangleArray<bool> walkability;
		extern RectangleArray<bool> rawWalkability;
		extern RectangleArray<bool> lowResWalkability;
		extern RectangleArray<bool> buildability;
		extern RectangleArray<int> distanceTransform;
		extern std::set<BWAPI::TilePosition> startLocations;
		extern std::string hash;
		extern int mapWidth;
		extern int mapHeight;
		extern int maxDistanceTransform;
		// data for HPA*
		extern ChokepointGraph chokeNodes;
	}
}