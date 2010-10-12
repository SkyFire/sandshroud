#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <deque>
#include <set>
#include <cstdlib>

#ifdef WIN32
#include "direct.h"
#else
#include <sys/stat.h>
#endif

#include "dbcfile.h"
#include "mpq_libmpq.h"

#include "loadlib/adt.h"
#include "loadlib/wdt.h"
#include <fcntl.h>

#if defined( __GNUC__ )
    #define _open   open
    #define _close close
    #ifndef O_BINARY
        #define O_BINARY 0
    #endif
#else
    #include <io.h>
#endif

#ifdef O_LARGEFILE
    #define OPEN_FLAGS  (O_RDONLY | O_BINARY | O_LARGEFILE)
#else
    #define OPEN_FLAGS (O_RDONLY | O_BINARY)
#endif
extern ArchiveSet gOpenArchives;

char output_path[128] = ".";
char input_path[128] = ".";

// This option allow use float to int conversion
bool  CONF_allow_float_to_int   = true;
float CONF_float_to_int8_limit  = 2.0f;      // Max accuracy = val/256
float CONF_float_to_int16_limit = 2048.0f;   // Max accuracy = val/65536
float CONF_flat_height_delta_limit = 0.005f; // If max - min less this value - surface is flat
float CONF_flat_liquid_delta_limit = 0.001f; // If max - min less this value - liquid surface is flat

// List MPQ for extract from
char *CONF_mpq_list[]={
    "common.MPQ",
    "common-2.MPQ",
    "lichking.MPQ",
    "expansion.MPQ",
    "patch.MPQ",
    "patch-2.MPQ",
    "patch-3.MPQ",
    "patch-4.MPQ",
    "patch-5.MPQ",
};

static char* const langs[] = {"enGB", "enUS", "deDE", "esES", "frFR", "koKR", "zhCN", "zhTW", "enCN", "enTW", "esMX", "ruRU" };
#define LANG_COUNT 12

void CreateDir( const std::string& Path )
{
    #ifdef WIN32
    _mkdir( Path.c_str());
    #else
    mkdir( Path.c_str(), 0777 );
    #endif
}

bool FileExists( const char* FileName )
{
    int fp = _open(FileName, OPEN_FLAGS);
    if(fp != -1)
    {
        _close(fp);
        return true;
    }

    return false;
}

void Usage(char* prg)
{
    printf(
        "Usage:\n"\
        "%s -[var] [value]\n"\
        "-i set input path\n"\
        "-o set output path\n"\
        "-e extract only MAP(1)/DBC(2) - standard: both(3)\n"\
        "-f height stored as int (less map size but lost some accuracy) 1 by default\n"\
        "Example: %s -f 0 -i \"c:\\games\\game\"", prg, prg);
    exit(1);
}

void ExtractDBCFiles(int locale, bool basicLocale)
{
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

    string path = output_path;
    path += "/dbc/";
    CreateDir(path);
    if(!basicLocale)
    {
        path += langs[locale];
        path += "/";
        CreateDir(path);
    }

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
}

void LoadLocaleMPQFiles(int const locale)
{
    char filename[512];

    sprintf(filename,"%s/Data/%s/locale-%s.MPQ", input_path, langs[locale], langs[locale]);
    new MPQArchive(filename);

    for(int i = 1; i < 5; ++i)
    {
        char ext[3] = "";
        if(i > 1)
            sprintf(ext, "-%i", i);

        sprintf(filename,"%s/Data/%s/patch-%s%s.MPQ", input_path, langs[locale], langs[locale], ext);
        if(FileExists(filename))
            new MPQArchive(filename);
    }
}

inline void CloseMPQFiles()
{
    for(ArchiveSet::iterator j = gOpenArchives.begin(); j != gOpenArchives.end();++j) (*j)->close();
        gOpenArchives.clear();
}

int main(int argc, char * arg[])
{
    printf("== DBC Extractor ==\n");
    printf("===================\n\n");

    int FirstLocale = -1;

    for (int i = 0; i < LANG_COUNT; i++)
    {
        char tmp1[512];
        sprintf(tmp1, "%s/Data/%s/locale-%s.MPQ", input_path, langs[i], langs[i]);
        if (FileExists(tmp1))
        {
            printf("Detected locale: %s\n", langs[i]);

            //Open MPQs
            LoadLocaleMPQFiles(i);

            //Extract DBC files
            ExtractDBCFiles(i, false);

            //Close MPQs
            CloseMPQFiles();
            break;
        }
    }

    return 0;
}

