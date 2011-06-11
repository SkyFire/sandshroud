/*
 * Sandshroud Zeon
 * Copyright (c) 2009 Mikko Mononen memon@inside.org
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "RecastTimer.h"

#if defined(WIN32)

// Win32
#include <windows.h>

rcTimeVal rcGetPerformanceTimer()
{
	__int64 count;
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
	return count;
}

int rcGetDeltaTimeUsec(rcTimeVal start, rcTimeVal end)
{
	static __int64 freq = 0;
	if (freq == 0)
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	__int64 elapsed = end - start;
	return (int)(elapsed*1000000 / freq);
}

#else

// Linux, BSD, OSX

#include <cstddef>
#include <sys/time.h>

rcTimeVal rcGetPerformanceTimer()
{
	timeval now;
	gettimeofday(&now, 0);
	return (rcTimeVal)now.tv_sec*1000000L + (rcTimeVal)now.tv_usec;
}

int rcGetDeltaTimeUsec(rcTimeVal start, rcTimeVal end)
{
	return (int)(end - start);
}

#endif
