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
#include "RecastLog.h"
#include <stdio.h>
#include <stdarg.h>

static rcLog* g_log = 0;
static rcBuildTimes* g_btimes = 0;

rcLog::rcLog() :
	m_messageCount(0),
	m_textPoolSize(0)
{
}

rcLog::~rcLog()
{
	if (g_log == this)
		g_log = 0;
}

void rcLog::log(rcLogCategory category, const char* format, ...)
{
	if (m_messageCount >= MAX_MESSAGES)
		return;
	char* dst = &m_textPool[m_textPoolSize];
	int n = TEXT_POOL_SIZE - m_textPoolSize;
	if (n < 2)
		return;
	// Store category
	*dst = (char)category;
	n--;
	// Store message
	va_list ap;
	va_start(ap, format);
	int ret = vsnprintf(dst+1, n-1, format, ap);
	va_end(ap);
	if (ret > 0)
		m_textPoolSize += ret+2;
	m_messages[m_messageCount++] = dst;
}

void rcSetLog(rcLog* log)
{
	g_log = log;
}

rcLog* rcGetLog()
{
	return g_log;
}

void rcSetBuildTimes(rcBuildTimes* btimes)
{
	g_btimes = btimes;
}

rcBuildTimes* rcGetBuildTimes()
{
	return g_btimes;
}
