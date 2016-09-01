#include <BWTA/RectangleArray.h>
#include <BWTA.h>

#include "ConnectedComponent.h"
#include "MapData.h"
#include "functions.h"
#include "BWTA_Result.h"

namespace BWTA
{
	void detectBaseLocations(std::set<BaseLocation*>& baseLocations);
	void attachResourcePointersToBaseLocations(std::set<BaseLocation*>& baseLocations);
	void calculateBaseLocationProperties();
}