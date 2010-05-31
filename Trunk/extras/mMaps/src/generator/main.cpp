
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
int
main (int argc, char** argv)
{
  std::string gDataDir = ".";
  VMAP::gMapDataDir = gDataDir + "/maps";
  VMAP::gMMapDataDir = gDataDir + "/mmaps";
  VMAP::gVMapDataDir = gDataDir + "/vmaps";
  VMAP::gMakeObjFile = false;
  int map = 0;
  for(uint32 x = 1; x < 64+1; ++x)
  {
	  for(uint32 y = 1; y < 64+1; ++y)
	  {
		  VMAP::ModelContainerView modelContainerView(map, x, y);
		  modelContainerView.generateMoveMapForTile(map,x,y);
	  }
  }
  return 1;
}

void OnCrash(bool Terminate)
{
	printf("FUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU\n");
	Sleep(4000);
}
