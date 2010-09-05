/*
 * Sandshroud Hearthstone
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

#include "Common.h"
#include "Config/ConfigEnv.h"
#include "Log.h"
#include "NGLog.h"
#include <stdarg.h>

string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp)
{

	char p[MAX_PATH];
	p[0] = 0;
	time_t t = time(NULL);
	tm * a = gmtime(&t);
	strcat(p, Prefix);
	strcat(p, "/");
	strcat(p, Description);
	if(useTimeStamp)
	{
		char ftime[100];
		snprintf(ftime, 100, "-%-4d-%02d-%02d %02d-%02d-%02d", a->tm_year+1900, a->tm_mon+1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec);
		strcat(p, ftime);
	}

	strcat(p, ".log");
	return string(p);
}

createFileSingleton( oLog );
createFileSingleton(CLog);
initialiseSingleton( WorldLog );

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;
#ifndef WIN32
static const char* colorstrings[TBLUE+1] = {
"",
"\033[22;31m",
"\033[22;32m",
"\033[01;33m",
//"\033[22;37m",
"\033[0m",
"\033[01;37m",
"\033[22;34m",
};
#endif

void oLog::outTime()
{
#ifndef WIN32
	char buf[256];
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	if (tm)
	{
		strftime(buf, 256, "[%Y-%m-%d %T] ", tm);
		fprintf(m_file, buf);
	}
#endif
}

void oLog::outString( const char * str, ... )
{
	va_list ap;
	char buf[32768];

	if(m_screenLogLevel < 0)
		return;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);
	
	if(m_screenLogLevel >= 0)
	{
		printf(buf);
		putc('\n', stdout);
	}
}

void oLog::outError( const char * err, ... )
{
	va_list ap;
	char buf[32768];

	if(m_screenLogLevel < 1)
		return;

	va_start(ap, err);
	vsnprintf(buf, 32768, err, ap);
	va_end(ap);

	if(m_screenLogLevel >= 1)
	{
#ifdef WIN32
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
		puts(colorstrings[TRED]);
#endif
		fprintf(stderr, buf);
		putc('\n', stderr);
#ifdef WIN32
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#else
		puts(colorstrings[TNORMAL]);
#endif
	}
}

void oLog::outDetail( const char * str, ... )
{
	va_list ap;
	char buf[32768];

	if(m_screenLogLevel < 2)
		return;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	if(m_screenLogLevel >= 2)
	{
		printf(buf);
		putc('\n', stdout);
	}
}

void oLog::outDebug( const char * str, ... )
{
	va_list ap;
	char buf[32768];

	if(m_screenLogLevel < 3)
		return;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	if(m_screenLogLevel == 3 || m_screenLogLevel == 6)
	{
		printf(buf);
		putc('\n', stdout);
	}
}

void oLog::outDebugInLine(const char * str, ...)
{
	if(!str)
		return;

	va_list ap;
	char buf[32768];

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	if(m_screenLogLevel == 5 || m_screenLogLevel == 6)
	{
		printf(buf);
	}
}

void oLog::Init(int32 screenLogLevel)
{
	m_screenLogLevel = screenLogLevel;
	Log.log_level = screenLogLevel;

	// get error handle
#ifdef WIN32
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void oLog::SetScreenLoggingLevel(int32 level)
{
	m_screenLogLevel = level;
}

WorldLog::WorldLog()
{
	bEnabledXml = false;
	onlyPlayer = NULL;
	m_xml=NULL;

	if (Config.MainConfig.GetBoolDefault("LogLevel", "WorldXml", false))
	{
		Log.Notice("WorldLog", "Enabling packetlog output to \"world.xml\"");
		EnableXml();
	} else {
		DisableXml();
	}

}

void WorldLog::EnableXml()
{
	if(bEnabledXml)
		return;

	bEnabledXml = true;
	if(m_xml != NULL)
	{
		DisableXml();
		bEnabledXml=true;
	}
	m_xml = fopen("world.xml", "w");
	if (m_xml)
	{
		fprintf(m_xml, "<?xml version=\"1.0\" ?><log>");
	}
}

void WorldLog::DisableXml()
{
	if(!bEnabledXml)
		return;

	bEnabledXml = false;
	if(!m_xml)
		return;

	fprintf(m_xml, "</log>");
	fflush(m_xml);
	fclose(m_xml);
	m_xml=NULL;
}

WorldLog::~WorldLog()
{
	if (m_xml)
	{
		fprintf(m_xml, "</log>");
		fclose(m_xml);
		m_xml = NULL;
	}
}

void oLog::outColor(uint32 colorcode, const char * str, ...)
{
	if( !str ) return;
	va_list ap;
	va_start(ap, str);
#ifdef WIN32
	SetConsoleTextAttribute(stdout_handle, colorcode);
#else
	printf(colorstrings[colorcode]);
#endif
	vprintf( str, ap );
	fflush(stdout);
	va_end(ap);
}
