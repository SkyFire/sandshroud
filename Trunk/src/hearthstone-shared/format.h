/*
 * Sandshroud Hearthstone
 * Copyright (C) 2004 - 2005 Antrix Team
 * Copyright (C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 - 2009 AspireDev <http://www.aspiredev.org/>
 * Copyright (C) 2009 - 2010 Sandshroud <http://www.sandshroud.org/>
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

#ifndef FORMAT_H
#define FORMAT_H

#include "Collision/g3dlite/platform.h"
#include <string>
#include <stdio.h>
#include <cstdarg>

// If your platform does not have vsnprintf, you can find a
// implementation at http://www.ijs.si/software/snprintf/

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
// Both MSVC seems to use the non-standard vsnprintf
// so we are using vscprintf to determine buffer size, however
// only MSVC7 and up headers include vscprintf for some reason.
HEARTHSTONE_INLINE std::string vformat(const char *fmt, va_list argPtr)
{
	// We draw the line at a 1MB string.
	const int maxSize = 1000000;

	// If the string is less than 161 characters,
	// allocate it on the stack because this saves
	// the malloc/free time.
	const int bufSize = 161;
	char stackBuffer[bufSize];

	// MSVC does not support va_copy
	int actualSize = _vscprintf(fmt, argPtr) + 1;

	if (actualSize > bufSize)
	{
		// Now use the heap.
		char* heapBuffer = NULL;

		if (actualSize < maxSize)
		{
			heapBuffer = (char*)malloc(maxSize + 1);
			_vsnprintf(heapBuffer, maxSize, fmt, argPtr);
			heapBuffer[maxSize] = '\0';
		}
		else
		{
			heapBuffer = (char*)malloc(actualSize);
			vsprintf(heapBuffer, fmt, argPtr);
		}

		std::string formattedString(heapBuffer);
		free(heapBuffer);
		return formattedString;
	}
	else
	{
		vsprintf(stackBuffer, fmt, argPtr);
		return std::string(stackBuffer);
	}
}

#elif defined(_MSC_VER) && (_MSC_VER < 1300)

std::string vformat(const char *fmt, va_list argPtr) {
	// We draw the line at a 1MB string.
	const int maxSize = 1000000;

	// If the string is less than 161 characters,
	// allocate it on the stack because this saves
	// the malloc/free time.
	const int bufSize = 161;
	char stackBuffer[bufSize];

	// MSVC6 doesn't support va_copy, however it also seems to compile
	// correctly if we just pass our argument list along.  Note that
	// this whole code block is only compiled if we're on MSVC6 anyway
	int actualWritten = _vsnprintf(stackBuffer, bufSize, fmt, argPtr);

	// Not a big enough buffer, bufSize characters written
	if (actualWritten == -1) {

		int heapSize = 512;
		double powSize = 1.0;
		char* heapBuffer = (char*)malloc(heapSize);

		while ((_vsnprintf(heapBuffer, heapSize, fmt, argPtr) == -1) &&
			(heapSize  < maxSize)) {

			heapSize = iCeil(heapSize * ::pow((double)2.0, powSize++));
			heapBuffer = (char*)realloc(heapBuffer, heapSize);
		}

		heapBuffer[heapSize-1] = '\0';

		std::string heapString(heapBuffer);
		free(heapBuffer);

		return heapString;
	} else {

		return std::string(stackBuffer);
	}
}

#else

// glibc 2.1 has been updated to the C99 standard
std::string vformat(const char* fmt, va_list argPtr) {
	// If the string is less than 161 characters,
	// allocate it on the stack because this saves
	// the malloc/free time.  The number 161 is chosen
	// to support two lines of text on an 80 character
	// console (plus the null terminator).
	const int bufSize = 161;
	char stackBuffer[bufSize];

	va_list argPtrCopy;
	va_copy(argPtrCopy, argPtr);
	int numChars = vsnprintf(stackBuffer, bufSize, fmt, argPtrCopy);
	va_end(argPtrCopy);

	if (numChars >= bufSize) {
	  // We didn't allocate a big enough string.
	  char* heapBuffer = (char*)malloc((numChars + 1) * sizeof(char));

	  ASSERT(heapBuffer);
	  int numChars2 = vsnprintf(heapBuffer, numChars + 1, fmt, argPtr);
	  ASSERT(numChars2 == numChars);
	  (void)numChars2;

	  std::string result(heapBuffer);

	  free(heapBuffer);

	  return result;

	} else {

	  return std::string(stackBuffer);

	}
}

#endif

HEARTHSTONE_INLINE std::string format(const char* fmt,...)
{
	va_list argList;
	va_start(argList,fmt);
	std::string result = vformat(fmt, argList);
	va_end(argList);

	return result;
}

#endif
