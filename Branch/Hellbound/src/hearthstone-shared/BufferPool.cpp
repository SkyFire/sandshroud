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

#include "SharedStdAfx.h"
#include "Common.h"
#include "Log.h"
#include "BufferPool.h"

size_t BufferPool::BufferBucket::buffer_sizes[BUFFER_BUCKET_COUNT] = {
	20,			// 20 bytes
	50,			// 50 bytes
	100,		// 100 bytes
	200,		// 200 bytes
	500,		// 500 bytes
	1000,		// 1 kbyte
	5000,		// 5 kbyte
	10000,		// 10 kbyte (shouldn't be used much)
};

BufferPool g_bufferPool;
