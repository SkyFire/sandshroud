/*
 * Sandshroud Hearthstone
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

#include "SharedStdAfx.h"
#include <stdlib.h>
#include <string.h>
#include "RecastAlloc.h"

static void *rcAllocDefault(int size, rcAllocHint)
{
	return malloc(size);
}

static void rcFreeDefault(void *ptr)
{
	free(ptr);
}

static rcAllocFunc* sRecastAllocFunc = rcAllocDefault;
static rcFreeFunc* sRecastFreeFunc = rcFreeDefault;

void rcAllocSetCustom(rcAllocFunc *allocFunc, rcFreeFunc *freeFunc)
{
	sRecastAllocFunc = allocFunc ? allocFunc : rcAllocDefault;
	sRecastFreeFunc = freeFunc ? freeFunc : rcFreeDefault;
}

void* rcAlloc(int size, rcAllocHint hint)
{
	return sRecastAllocFunc(size, hint);
}

void rcFree(void* ptr)
{
	if (ptr)
		sRecastFreeFunc(ptr);
}


void rcIntArray::resize2(int n)
{
	if (n > m_cap)
	{
		if (!m_cap) m_cap = n;
		while (m_cap < n) m_cap *= 2;
		int* newData = (int*)rcAlloc(m_cap*sizeof(int), RC_ALLOC_TEMP);
		if (m_size && newData) memcpy(newData, m_data, m_size*sizeof(int));
		rcFree(m_data);
		m_data = newData;
	}
	m_size = n;
}

