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

#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

/** ByteConverter reverse your byte order. This is use
    for cross platform where they have different endians.
 */

#include "Common.h"
#include <algorithm>

namespace ByteConverter
{
	template<size_t T>
	inline void convert(char *val)
	{
		std::swap(*val, *(val + T - 1));
		convert<T - 2>(val + 1);
	}

	template<> inline void convert<0>(char *) {}
	template<> inline void convert<1>(char *) {} // ignore central byte
	template<typename T> inline void apply(T *val)
	{
		convert<sizeof(T)>((char *)(val));
	}
}

#ifdef USING_BIG_ENDIAN

template<typename T> inline void EndianConvert(T& val) { ByteConverter::apply<T>(&val); }
template<typename T> inline void EndianConvertReverse(T&) { }

template<typename T> void EndianConvert(T*val); { ByteConverter::apply<T>(val); }
template<typename T> void EndianConvertReverse(T*) { };

#else

template<typename T> inline void EndianConvert(T&) { }
template<typename T> inline void EndianConvertReverse(T& val) { ByteConverter::apply<T>(&val); }

template<typename T> void EndianConvert(T*) { };
template<typename T> void EndianConvertReverse(T*val) { ByteConverter::apply<T>(val); };

#endif

inline void EndianConvert(uint8&) { }
inline void EndianConvert( int8&) { }
inline void EndianConvertReverse(uint8&) { }
inline void EndianConvertReverse( int8&) { }

#endif
