#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <vector>
#include <deque>
#include <set>
#include <cstdlib>

#ifdef WIN32
#include "direct.h"
#else
#include <sys/stat.h>
#endif

#include "dbcfile.h"
#include "adt.h"
#include "mpq_libmpq.h"
#include <io.h>
#include <fcntl.h>
#include <string>
#include <map>
#include <Windows.h>
#include <mmsystem.h>
using namespace std;

extern unsigned int iRes;
bool ConvertADT(uint32 x, uint32 y, FILE * out_file, char* name);
void reset();
void CleanCache();
extern ArchiveSet gOpenArchives;

typedef struct{
	char name[64];
	unsigned int id;
}map_id;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
map_id * map_ids;

uint32 MapCount;

void CreateDir( const std::string& Path )
{
    #ifdef WIN32
    _mkdir( Path.c_str());
    #else
    mkdir( Path.c_str(), 0777 );
    #endif
}

void ExtractMapsFromMpq()
{
    bool Available_Maps[64][64];
    uint32 Offsets[512][512];
    uint32 TotalTiles = 0;
    uint32 AvailableTiles = 0;
    char output_filename[50];
    map_id * map;
	char mpq_filename[128];
	printf("\nProcessing %u maps...\n\n", MapCount);

    for(uint32 i = 0; i < MapCount; ++i)
    {	
        map = &map_ids[i];
        printf("Converting maps for mapid %u [%s]...\n", map->id, map->name);
        // Create the container file
        sprintf(output_filename, "maps\\Map_%u.bin", map->id);
        printf("  Creating output file %s.\n", output_filename);
        FILE * out_file = fopen(output_filename, "wb");
        if(!out_file)
        {
            printf("  Could not create output file!\n");
            return;
        }
        printf("  Checking which tiles are extractable...\n");

        // First, check the number of present tiles.
        for(uint32 x = 0; x < 64; ++x)
        {
            for(uint32 y = 0; y < 64; ++y)
            {
                // set up the mpq filename
                sprintf(mpq_filename, "World\\Maps\\%s\\%s_%u_%u.adt", map->name, map->name, y, x);

                // check if the file exists
                if(!mpq_file_exists(mpq_filename))
                {
                    // file does not exist
                    Available_Maps[x][y] = false;
                }
                else
                {
                    // file does exist
                    Available_Maps[x][y] = true;
                    ++AvailableTiles;
                }
                ++TotalTiles;
            }
        }


        // Calculate the estimated size.
        float Estimated_Size = 1048576.0f;
        uint32 TilesToExtract = 0;

        for(uint32 x = 0; x < 512; ++x)
        {
            for(uint32 y = 0; y < 512; ++y)
            {
                Offsets[x][y] = 0;
                if(Available_Maps[x/8][y/8] == true)
                    ++TilesToExtract;                
            }
        }
        // Write the offsets to file
        printf("  Writing empty index to the beginning of the file...\n");
        fwrite(Offsets, sizeof(Offsets), 1, out_file);

        Estimated_Size += 4124.0f * TilesToExtract;
        Estimated_Size /= 1024.0f;
        Estimated_Size /= 1024.0f;
        printf("  %u of %u tiles are available. Estimated file size will be %.4fMB.\n", AvailableTiles, TotalTiles, Estimated_Size);
        printf("  %u passes in total have to be performed, it may take a while.\n", TilesToExtract);
        printf("  Extracting data...\n");
        uint32 start_time = timeGetTime();
        reset();

        // call the extraction function.
        for(uint32 x = 0; x < 512; ++x)
        {
            for(uint32 y = 0; y < 512; ++y)
            {
                // Check if the map is available.
                if(Available_Maps[x/8][y/8])
                {
                    uint32 Offset = ftell(out_file);
                    if(ConvertADT(x, y, out_file, map->name))
                        Offsets[x][y] = Offset;
                }
            }

            // Clean the cache every 8 cells.
            if(!(x % 8))
                CleanCache();
        }
        // clean any leftover cells
        CleanCache();
        printf("  Finished extracting in %ums. Appending header to start of file...\n", timeGetTime() - start_time);
        fseek(out_file, 0, SEEK_SET);
        fwrite(Offsets, sizeof(Offsets), 1, out_file);
        printf("  Closing output file.\n");
        fclose(out_file);
        printf("  Conversion of map %u completed\n\n", map->id);
    }
}

int main(int argc, char * arg[])
{
    printf("Sandshroud map extractor for versions 3.x.x\n");
    printf("============================================================\n\n");

	FILE * tf;
	const char* localeNames[] = { "enUS", "enGB", "deDE", "frFR", "koKR", "zhCN", "zhTW", "esES", "ruRU", 0 };
	int locale = -1;
	char tmp[100];

	for( size_t i = 0; localeNames[i] != 0; i++ )
	{
		sprintf(tmp, "Data/%s/locale-%s.MPQ", localeNames[i], localeNames[i]);
		tf = fopen(tmp, "r");
		if (!tf)
			continue;

		fclose(tf);
		locale = i;
		new MPQArchive(tmp);
		break;
	}

	tf = fopen("Data/world.MPQ", "r");
	if (!tf)
	{
		printf("Could not find Data/world.MPQ\n");
		return 1;
	}
	fclose(tf);
	new MPQArchive("Data/world.MPQ");

	tf = fopen("Data/art.MPQ", "r");
	if (!tf)
	{
		printf("Could not find Data/art.MPQ\n");
		return 1;
	}
	fclose(tf);
	new MPQArchive("Data/art.MPQ");

	tf = fopen("Data/OldWorld.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/OldWorld.MPQ");
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/OldWorld-%s.MPQ", localeNames[locale], localeNames[locale]);
			new MPQArchive(tmp);
		}
	}

	tf = fopen("Data/expansion1.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/expansion1.MPQ");
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/expansion1-locale-%s.MPQ", localeNames[locale], localeNames[locale]);
			new MPQArchive(tmp);
		}
	}

	tf = fopen("Data/expansion2.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/expansion2.MPQ");
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/expansion2-locale-%s.MPQ", localeNames[locale], localeNames[locale]);
			new MPQArchive(tmp);
		}
	}

/*	tf = fopen("Data/expansion3.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/expansion3.MPQ");
		if ( -1 != locale )
		{
			sprintf(tmp, "Data/%s/expansion3-locale-%s.MPQ", localeNames[locale], localeNames[locale]);
			new MPQArchive(tmp);
		}
	}*/

	tf = fopen("Data/wow-update-13164.MPQ", "r");
	if (tf)
	{
		fclose(tf);
		new MPQArchive("Data/wow-update-13164.MPQ");
	}

    printf("Extracting dbc files...\n");

    set<string> dbcfiles;

    // get DBC file list
    for(ArchiveSet::iterator i = gOpenArchives.begin(); i != gOpenArchives.end();++i)
    {
        vector<string> files;
        (*i)->GetFileListTo(files);
        for (vector<string>::iterator iter = files.begin(); iter != files.end(); ++iter)
            if (iter->rfind(".dbc") == iter->length() - strlen(".dbc"))
                    dbcfiles.insert(*iter);
    }

    string path = ".";
    path += "/dbc/";
    CreateDir(path);

    // extract DBCs
    int count = 0;
    for (set<string>::iterator iter = dbcfiles.begin(); iter != dbcfiles.end(); ++iter)
    {
        string filename = path;
        filename += (iter->c_str() + strlen("DBFilesClient\\"));

        FILE *output = fopen(filename.c_str(), "wb");
        if(!output)
        {
            printf("Can't create the output file '%s'\n", filename.c_str());
            continue;
        }
        MPQFile m(iter->c_str());
        if(!m.isEof())
            fwrite(m.getPointer(), 1, m.getSize(), output);

        fclose(output);
        ++count;
    }
    printf("Extracted %u DBC files\n\n", count);

	//map.dbc
	DBCFile * dbc = new DBCFile("DBFilesClient\\Map.dbc");
	dbc->open();

	MapCount = dbc->getRecordCount();
	map_ids = new map_id[MapCount];
	for(unsigned int x = 0; x < MapCount; x++)
	{
		map_ids[x].id=dbc->getRecord (x).getUInt(0);
		strcpy(map_ids[x].name,dbc->getRecord(x).getString(1));
	}
	delete dbc;

	CreateDirectory("maps", NULL);
	ExtractMapsFromMpq();
	delete [] map_ids;

	return 0; // Exit The Program
}
