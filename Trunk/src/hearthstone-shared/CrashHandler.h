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

#ifndef _CRASH_HANDLER_H
#define _CRASH_HANDLER_H

bool HookCrashReporter(bool logon);
void OutputCrashLogLine(const char* format, ...);

#ifdef WIN32

#include "Common.h"
#include <DbgHelp.h>
#include "StackWalker.h"
#include "CircularQueue.h"

extern CircularQueue<uint32, 30> last_spells;

class SERVER_DECL CStackWalker : public StackWalker
{
public:
	void OnOutput(LPCSTR szText);
	void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion);
	void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
	void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
};

void StartCrashHandler();
void OnCrash(bool Terminate);

typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
int __cdecl HandleCrash(PEXCEPTION_POINTERS pExceptPtrs);

#define THREAD_TRY_EXECUTION __try 
#define THREAD_HANDLE_CRASH  __except( HandleCrash( GetExceptionInformation() ) ) {}

#define THREAD_TRY_EXECUTION2 __try {
#define THREAD_HANDLE_CRASH2  } __except(HandleCrash(GetExceptionInformation())) {}

#else

// We dont wanna confuse nix ;p
#define THREAD_TRY_EXECUTION 
#define THREAD_HANDLE_CRASH 

#define THREAD_TRY_EXECUTION2 ;
#define THREAD_HANDLE_CRASH2 ;

#endif

#endif

