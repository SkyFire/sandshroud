
#include "ModelContainerView.h"

#define DISABLE_MINI_DUMP

namespace VMAP
{
  std::string gMapDataDir;
  std::string gVMapDataDir;
  std::string gMMapDataDir;
  bool gMakeObjFile;
}

#define SIZE_OF_GRIDS 533.33333f

bool istilemap(uint32 mapid)
{
	switch( mapid )
	{
	case 0:		// Eastern Kingdoms
	case 1:		// Kalimdor
	case 30:	// Alterac Valley
	case 33:	// Shadowfang Keep
	case 37:	// Azshara Crater
	case 189:	// Scarlet Monastery
	case 209:	// Zul'Farrak
	case 309:	// Zul'Gurub
	case 469:	// Blackwing Lair
	case 509:	// Ruins of Ahn'Qiraj
	case 530:	// Outland
	case 532:	// Karazhan
	case 533:	// Naxxramas
	case 534:	// The Battle for Mount Hyjal
	case 543:	// Hellfire Citadel: Ramparts
	case 571:	// Northrend
	case 560:	// Coilfang: The Steamvault
	case 564:	// Black Temple
	case 568:	// Zul'Aman
	case 609:	// Ebon Hold
		return true;
		break;

	default:
		return false;
		break;
	}
}

int main (int argc, char** argv)
{
  std::string gDataDir = ".";
  VMAP::gMapDataDir = gDataDir + "/maps";
  VMAP::gMMapDataDir = gDataDir + "/mmaps";
  VMAP::gVMapDataDir = gDataDir + "/vmaps";
  VMAP::gMakeObjFile = false;
  for(uint32 map = 0; map < 750; ++map)
  {
	  if(istilemap(map))
	  {
		  for(uint32 x = 1; x < 64+1; ++x)
		  {
			  for(uint32 y = 1; y < 64+1; ++y)
			  {
				  VMAP::ModelContainerView modelContainerView(map, x, y);
				  modelContainerView.generateMoveMapForTile(map,x,y);
			  }
		  }
	  }
  }
  return 1;
}

void OnCrash(bool Terminate)
{
	printf("FUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU\n");
	Sleep(4000);
}
