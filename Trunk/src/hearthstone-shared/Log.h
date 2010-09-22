/*
 * Sandshroud Hearthstone
 * Copyright (C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007 - 2008 Antrix Team
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

#ifndef WOWSERVER_LOG_H
#define WOWSERVER_LOG_H

#include "Common.h"
#include "Singleton.h"

class WorldPacket;
class WorldSession;
class Database;

#ifdef WIN32

#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TPURPLE FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY

#else

#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6
#define TPURPLE 7

#endif
std::string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp);

// log define
#define OUT_DEBUG sLog.outDebug
#define DEBUG_LOG Log.Debug

class SERVER_DECL oLog : public Singleton< oLog > {
public:
	void outString( const char * str, ... );
	void outError( const char * err, ... );
	void outDetail( const char * str, ... );
	void outDebug( const char * str, ... );
	void outDebugInLine( const char * str, ... );

	void fLogText(const char *text);
	void SetLogging(bool enabled);

	void Init(int32 screenLogLevel);
	void SetScreenLoggingLevel(int32 level);
	void outColor(uint32 colorcode, const char * str, ...);
	bool IsOutDevelopement() const { return m_screenLogLevel == 4 || m_screenLogLevel == 6; }
	bool IsOutProccess() const { return m_screenLogLevel == 5 || m_screenLogLevel == 6; }

#ifdef WIN32
	HANDLE stdout_handle, stderr_handle;
#endif

	int32 m_screenLogLevel;
};

#define sLog oLog::getSingleton()

class WorldLog : public Singleton<WorldLog>
{
public:
	WorldLog();
	~WorldLog();

	void EnableXml();
	void DisableXml();
private:
	FILE * m_xml;
	Mutex mutex;
	bool bEnabledXml;
	char *onlyPlayer;
};

#define sWorldLog WorldLog::getSingleton()

#endif

