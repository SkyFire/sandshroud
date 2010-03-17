/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef _UTIL_H
#define _UTIL_H

#include "Common.h"

#ifndef PAIR64_HIPART
#define PAIR64_HIPART(x)   (uint32)((uint64(x) >> 32) & UI64LIT(0x00000000FFFFFFFF))
#define PAIR64_LOPART(x)   (uint32)(uint64(x)         & UI64LIT(0x00000000FFFFFFFF))
#endif

///////////////////////////////////////////////////////////////////////////////
// String Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> StrSplit(const std::string &src, const std::string &sep);

// This HAS to be called outside the threads __try / __except block!
void SetThreadName(const char* format, ...);
time_t convTimePeriod ( uint32 dLength, char dType);

FEATHERMOONEMU_INLINE uint32 secsToTimeBitFields(time_t secs)
{
    tm* lt = localtime(&secs);
    return (lt->tm_year - 100) << 24 | lt->tm_mon  << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min;
}
#ifdef WIN32

	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in user addr space)
		DWORD dwThreadID; // thread ID (-1=caller thread)
		DWORD dwFlags; // reserved for future use, must be zero
	} THREADNAME_INFO;

#endif

class flag96
{
private:
	uint32 part[3];
public:
	flag96(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0)
	{
		part[0] = p1;
		part[1] = p2;
		part[2] = p3;
	}

	flag96(uint64 p1, uint32 p2)
	{
		part[0] = PAIR64_LOPART(p1);
		part[1] = PAIR64_HIPART(p1);
		part[2] = p2;
	}

	FEATHERMOONEMU_INLINE bool IsEqual(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0) const
	{
		return (
			part[0] == p1 &&
			part[1] == p2 &&
			part[2] == p3);
	};

	FEATHERMOONEMU_INLINE bool HasFlag(uint32 p1 = 0, uint32 p2 = 0, uint32 p3 = 0) const
	{
		return (
			part[0] & p1 ||
			part[1] & p2 ||
			part[2] & p3);
	};

	FEATHERMOONEMU_INLINE void Set(uint32 p1=0, uint32 p2=0, uint32 p3=0)
	{
		part[0] = p1;
		part[1] = p2;
		part[2] = p3;
	};

	template<class type>
	FEATHERMOONEMU_INLINE bool operator < (type & right)
	{
		for (uint8 i = 3; i > 0; i--)
		{
			if (part[i-1] < right.part[i-1])
				return 1;
			else if (part[i-1] > right.part[i-1])
				return 0;
		}
		return 0;
	};

	template<class type>
	FEATHERMOONEMU_INLINE bool operator < (type & right) const
	{
		for (uint8 i = 3; i > 0; i--)
		{
			if (part[i-1] < right.part[i-1])
				return 1;
			else if (part[i-1] > right.part[i-1])
				return 0;
		}
		return 0;
	};

	template<class type>
	FEATHERMOONEMU_INLINE bool operator != (type & right)
	{
		if (part[0] != right.part[0]
		|| part[1] != right.part[1]
		|| part[2] != right.part[2])
			return true;
		return false;
	}

	template<class type>
	FEATHERMOONEMU_INLINE bool operator != (type & right) const
	{
		if (part[0] != right.part[0]
		|| part[1] != right.part[1]
		|| part[2] != right.part[2])
			return true;
		return false;
	};

	template<class type>
	FEATHERMOONEMU_INLINE bool operator == (type & right)
	{
		if (part[0] != right.part[0]
		|| part[1] != right.part[1]
		|| part[2] != right.part[2])
			return false;
		return true;
	};

	template<class type>
	FEATHERMOONEMU_INLINE bool operator == (type & right) const
	{
		if (part[0] != right.part[0]
		|| part[1] != right.part[1]
		|| part[2] != right.part[2])
			return false;
		return true;
	};

	template<class type>
	FEATHERMOONEMU_INLINE void operator = (type & right)
	{
		part[0] = right.part[0];
		part[1] = right.part[1];
		part[2] = right.part[2];
	};

	template<class type>
	FEATHERMOONEMU_INLINE flag96 operator & (type & right)
	{
		flag96 ret(part[0] & right.part[0], part[1] & right.part[1], part[2] & right.part[2]);
		return
			ret;
	};
	
	template<class type>
	FEATHERMOONEMU_INLINE flag96 operator & (type & right) const
	{
		flag96 ret(part[0] & right.part[0],part[1] & right.part[1],part[2] & right.part[2]);
		return
			ret;
	};

	template<class type>
	FEATHERMOONEMU_INLINE void operator &= (type & right)
	{
		*this = *this & right;
	};

	template<class type>
	FEATHERMOONEMU_INLINE flag96 operator | (type & right)
	{
		flag96 ret(part[0] | right.part[0],part[1] | right.part[1],part[2] | right.part[2]);
		return
			ret;
	};

	template<class type>
	FEATHERMOONEMU_INLINE flag96 operator | (type & right) const
	{
		flag96 ret(part[0] | right.part[0], part[1] | right.part[1], part[2] | right.part[2]);
		return
			ret;
	};

	template<class type>
	FEATHERMOONEMU_INLINE void operator |= (type & right)
	{
		*this  =*this | right;
	};

	FEATHERMOONEMU_INLINE void operator ~ ()
	{
		part[2] =~ part[2];
		part[1] =~ part[1];
		part[0] =~ part[0];
	};

	template<class type>
	FEATHERMOONEMU_INLINE flag96 operator ^ (type & right)
	{
		flag96 ret(part[0] ^ right.part[0],part[1] ^ right.part[1],part[2] ^ right.part[2]);
		return
			ret;
	};

	template<class type>
	FEATHERMOONEMU_INLINE flag96 operator ^ (type & right) const
	{
		flag96 ret(part[0] ^ right.part[0],part[1] ^ right.part[1],part[2] ^ right.part[2]);
		return
			ret;
	};

	template<class type>
	FEATHERMOONEMU_INLINE void operator ^= (type & right)
	{
		*this = *this ^ right;
	};

	FEATHERMOONEMU_INLINE operator bool() const
	{
		return(
			part[0] != 0 ||
			part[1] != 0 ||
			part[2] != 0);
	};

	FEATHERMOONEMU_INLINE operator bool()
	{
		return(
			part[0] != 0 ||
			part[1] != 0 ||
			part[2] != 0);
	};

	FEATHERMOONEMU_INLINE bool operator ! () const
	{
		return(
			part[0] == 0 &&
			part[1] == 0 &&
			part[2] == 0);
	};

	FEATHERMOONEMU_INLINE bool operator ! ()
	{
		return(
			part[0] == 0 &&
			part[1] == 0 &&
			part[2] == 0);
	};

	FEATHERMOONEMU_INLINE uint32 & operator[](uint8 el)
	{
		return (part[el]);
	};

	FEATHERMOONEMU_INLINE const uint32 & operator[](uint8 el) const
	{
		return (part[el]);
	};
};
#endif
