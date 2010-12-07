/*
 * Lacrimi Scripts Copyright 2010 - 2011
 *
 * ############################################################
 * # ##            #       ####### ####### ##    #    #    ## #
 * # ##           ###      ##      ##   ## ##   ###  ###   ## #
 * # ##          ## ##     ##      ##   ## ##   ###  ###   ## #
 * # ##         #######    ##      ####### ##  ## #### ##  ## #
 * # ##        ##     ##   ##      #####   ##  ## #### ##  ## #
 * # ##       ##       ##  ##      ##  ##  ## ##   ##   ## ## #
 * # ####### ##         ## ####### ##   ## ## ##   ##   ## ## #
 * # :::::::.::.........::.:::::::.::...::.::.::...::...::.:: #
 * ############################################################
 *
 */

#include "LacrimiStdAfx.h"

#ifdef WIN32
static HANDLE m_abortLacrimi = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcondLacrimi;
static pthread_mutex_t abortmutex;
#endif

Lacrimi::Lacrimi(ScriptMgr* mgr)
{
	m_threadRunning = true;
	sMgr = mgr;
}

Lacrimi::~Lacrimi()
{
#ifdef WIN32
	CloseHandle(m_abortLacrimi);
#else
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
#endif
}

void Lacrimi::terminate()
{
	m_threadRunning = false;
#ifdef WIN32
	SetEvent(m_abortLacrimi);
#else
	pthread_cond_signal(&abortcondLacrimi);
#endif
}

void Lacrimi::Delay(uint32 time)
{
#ifdef WIN32
	WaitForSingleObject(m_abortLacrimi, time);
#else
	Sleep(time);
#endif
}

bool Lacrimi::run()
{
	SetThreadName("Lacrimi");
	Log.Success("Lacrimi", "Lacrimi Engine Started");
	SetupScripts();

	while(m_threadRunning)
	{
		Delay(60000);

		if(!m_threadRunning)
			break;
		Log.Debug("Lacrimi", "Running cleanup...");
	}

	return true;
}

void SetThreadName(const char* format, ...)
{
	// This isn't supported on nix?
	va_list ap;
	va_start(ap, format);

#ifdef WIN32
	char thread_name[200];
	vsnprintf(thread_name, 200, format, ap);

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;
	info.szName = thread_name;
	DEBUG_LOG("ThreadMaker", "Creating thread %u with name %s", info.dwThreadID, info.szName);
	ThreadPool.SetThreadInfo(info.dwThreadID, info);

	__try
	{
#ifdef _WIN64
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info);
#else
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
#endif
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{

	}
#endif

	va_end(ap);
}

// Use sMgr for Script Mgr.
void Lacrimi::SetupScripts()
{
	SetupZoneScripts();
}

void Lacrimi::SetupZoneScripts()
{
	SetupOutlandScripts();
	SetupKalimdorScripts();
	SetupEbonHoldScripts();
	SetupNorthrendScripts();
	SetupEasternKingdomScripts();
}

void Lacrimi::SetupOutlandScripts()
{

}

void Lacrimi::SetupKalimdorScripts()
{

}

void Lacrimi::SetupEbonHoldScripts()
{

}

void Lacrimi::SetupNorthrendScripts()
{

}

void Lacrimi::SetupEasternKingdomScripts()
{
	SetupAlteracMountains();
	SetupArathiHighlands();
	SetupBlastedLands();
	SetupBurningSteppes();
	SetupDunMorogh();
	SetupDuskwood();
	SetupEasternPlaguelands();
	SetupElwynnForest();
	SetupEversongWoods();
	SetupGhostlands();
	SetupHinterlands();
	SetupIsleOfQuelDanas();
	SetupLochModan();
	SetupSearingGorge();
	SetupSilverpineForest();
	SetupStranglethornVale();
	SetupTrisfalGlades();
	SetupWesternPlaguelands();
	SetupWestfall();
	SetupWetlands();
	SetupIronforge();
	SetupSilvermoon();
	SetupStormwind();
	SetupUndercity();
}
