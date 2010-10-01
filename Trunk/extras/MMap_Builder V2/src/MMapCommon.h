#ifndef _MMAP_COMMON_H
#define _MMAP_COMMON_H

// stop warning spam from ACE includes
#pragma warning(disable : 4996)

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
#  define _WIN32_WINNT 0x0500
#  define NOMINMAX
#  include <windows.h>
#else
#  include <string.h>
#  define MAX_PATH 1024
#endif

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#endif

#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>

#ifndef WIN32
#ifndef X64
#  if defined (__GNUC__)
#	if GCC_VERSION >= 30400
#         ifdef HAVE_DARWIN
#	      define __fastcall
#         else
#    	      define __fastcall __attribute__((__fastcall__))
#         endif
#	else
#	  define __fastcall __attribute__((__regparm__(3)))
#	endif
#  else
#	define __fastcall __attribute__((__fastcall__))
#  endif
#else
#define __fastcall  
#endif
#endif

#if COMPILER == COMPILER_GNU && __GNUC__ >= 4
#include <tr1/memory>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 3
#include <ext/hash_map>
#include <ext/hash_set>
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1500 && _HAS_TR1   // VC9.0 SP1 and later
#include <memory>
#include <unordered_map>
#include <unordered_set>
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1500 && !_HAS_TR1
#pragma message ("FATAL ERROR: Please install Service Pack 1 for Visual Studio 2008")
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER < 1500
#pragma message ("FATAL ERROR: Please install a newer version of visual studio, VS2008 or above.")
#else
#include <memory>
#include <hash_map>
#include <hash_set>
#endif

#ifdef _STLPORT_VERSION
#define HM_NAMESPACE std
using std::hash_map;
using std::hash_set;
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1500 && _HAS_TR1
using namespace std::tr1;
using std::tr1::shared_ptr;
#undef HM_NAMESPACE
#define HM_NAMESPACE tr1
#define hash_map unordered_map
#define TRHAX 1
#elif COMPILER == COMPILER_MICROSOFT && (_MSC_VER < 1500 || !_HAS_TR1)
using namespace std::tr1;
using std::tr1::shared_ptr;
#undef HM_NAMESPACE
#define HM_NAMESPACE tr1
#define hash_map unordered_map
#define ENABLE_SHITTY_STL_HACKS 1

// hacky stuff for vc++
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#elif COMPILER == COMPILER_INTEL
#define HM_NAMESPACE std
using std::hash_map;
using std::hash_set;
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 4
using namespace std::tr1;
using std::tr1::shared_ptr;
#undef HM_NAMESPACE
#define HM_NAMESPACE tr1
#define shared_ptr std::tr1::shared_ptr
#define hash_map unordered_map
#define TRHAX 1
namespace std
{
	namespace tr1
	{
		template<> struct hash<const long long unsigned int> : public std::unary_function<const long long unsigned int, std::size_t>
		{
			std::size_t operator()(const long long unsigned int val) const
			{
				return static_cast<std::size_t>(val);
			}
		};
		template<> struct hash<const unsigned int> : public std::unary_function<const unsigned int, std::size_t>
		{
			std::size_t operator()(const unsigned int val) const
			{
				return static_cast<std::size_t>(val);
			}
		};
	}
}
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 3
#define HM_NAMESPACE __gnu_cxx
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

namespace __gnu_cxx
{
	template<> struct hash<unsigned long long>
	{
		size_t operator()(const unsigned long long &__x) const { return (size_t)__x; }
	};
	template<typename T> struct hash<T *>
	{
		size_t operator()(T * const &__x) const { return (size_t)__x; }
	};

};
#else
#define HM_NAMESPACE std
using std::hash_map;
#endif
#if COMPILER == COMPILER_GNU && __GNUC__ >=4 && __GNUC_MINOR__ == 1 && __GNUC_PATCHLEVEL__ == 2
//GCC I HATE YOU!
namespace std
{
	namespace tr1
	{
		template<> struct hash<long long unsigned int> : public std::unary_function<long long unsigned int, std::size_t>
		{
			std::size_t operator()(const long long unsigned int val) const
			{
				return static_cast<std::size_t>(val);
			}
		};
	}
}
#endif

namespace std
{
	namespace tr1
	{
		template<typename T> struct hash<shared_ptr<T> >
		{
			size_t operator()(shared_ptr<T> const &__x) const { return (size_t)__x.get(); }
		};
	}
}

/* Use correct types for x64 platforms, too */
typedef unsigned int uint;
typedef unsigned long ulong;
#define UNORDERED_MAP std::tr1::unordered_map

typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;

//#include "platform/Define.h"
#include "DebugAlloc.h"

using namespace std;

namespace MMAP
{
#ifndef WIN32
    inline bool matchWildcardFilter(const char* filter, const char* str)
    {
        if(!filter || !str)
            return false;

        // end on null character
        while(*filter && *str)
        {
            if(*filter == '*')
            {
                if(*++filter == '\0')   // wildcard at end of filter means all remaing chars match
                    return true;

                while(true)
                {
                    if(*filter == *str)
                        break;
                    if(*str == '\0')
                        return false;   // reached end of string without matching next filter character
                    str++;
                }
            }
            else if(*filter != *str)
                return false;           // mismatch

            filter++;
            str++;
        }

        return ((*filter == '\0' || (*filter == '*' && *++filter == '\0')) && *str == '\0');
    }
#endif

    enum ListFilesResult : int
    {
        LISTFILE_DIRECTORY_NOT_FOUND = -1,
        LISTFILE_OK = 1
    };

    inline ListFilesResult getDirContents(vector<string> &fileList, string dirpath = ".", string filter = "*", bool includeSubDirs = false)
    {
    #ifdef WIN32
        HANDLE hFind;
        WIN32_FIND_DATA findFileInfo;
        string directory;

        directory = dirpath + "/" + filter;

        hFind = FindFirstFile(directory.c_str(), &findFileInfo);

        if(hFind == INVALID_HANDLE_VALUE)
            return LISTFILE_DIRECTORY_NOT_FOUND;

        do
        {
            if(includeSubDirs || (findFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
                fileList.push_back(string(findFileInfo.cFileName));
        }
        while (FindNextFile(hFind, &findFileInfo));

        FindClose(hFind);

    #else
        const char *p = dirpath.c_str();
        DIR * dirp;
        struct dirent * dp;
        dirp = opendir(p);

        while (dirp)
        {
            errno = 0;
            if ((dp = readdir(dirp)) != NULL)
            {
                if(matchWildcardFilter(filter.c_str(), dp->d_name))
                    fileList.push_back(string(dp->d_name));
            }
            else
                break;
        }

        if(dirp)
            closedir(dirp);
        else
            return LISTFILE_DIRECTORY_NOT_FOUND;
    #endif

        return LISTFILE_OK;
    }
}

#include "Collision/VmapDefinitions.h"

#endif
