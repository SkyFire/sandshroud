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

// Use sMgr for Script Mgr.
void Lacrimi::SetupScripts()
{

}
