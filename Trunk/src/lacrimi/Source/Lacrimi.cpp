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
	config = false;
	LacrimiDB = NULL;
	database = false;
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

	// Load our configs
#ifdef WIN32
	if(LacrimiConfig.SetSource("configs/Lacrimi.conf", true))
		config = true;
#else
	if(LacrimiConfig.SetSource((char*)CONFDIR "/Lacrimi.conf", true))
		config = true;
#endif

	// Load our DBs
	if(_StartDB())
		database = true;

	SetupScripts();
	while(m_threadRunning)
	{
		Delay(15000);
		if(!m_threadRunning)
			break;
		Delay(15000);
		if(!m_threadRunning)
			break;
		Delay(15000);
		if(!m_threadRunning)
			break;
		Delay(15000);
		if(!m_threadRunning)
			break;

		Log.Debug("Lacrimi", "Running cleanup...");
	}

	if(database)
		_StopDB();
	return true;
}

bool Lacrimi::_StartDB()
{
	if(!config)
		return false;

	string hostname, username, password, database;
	int port = 0;
	// Configure Main Database
	bool result = LacrimiConfig.GetString( "LacrimiDatabase", "Username", &username );
	LacrimiConfig.GetString( "LacrimiDatabase", "Password", &password );
	result = !result ? result : LacrimiConfig.GetString( "LacrimiDatabase", "Hostname", &hostname );
	result = !result ? result : LacrimiConfig.GetString( "LacrimiDatabase", "Name", &database );
	result = !result ? result : LacrimiConfig.GetInt( "LacrimiDatabase", "Port", &port );
	if(result == false)
	{
		OUT_DEBUG( "sql: One or more parameters were missing from LacrimiDatabase directive." );
		return false;
	}

	// Create the DB
	LacrimiDB = Database::Create();

	// Initialize it
	if( !LacrimiDatabase.Initialize(hostname.c_str(), uint(port), username.c_str(),
		password.c_str(), database.c_str(), LacrimiConfig.GetIntDefault( "LacrimiDatabase", "ConnectionCount", 3 ), 16384 ) )
	{
		OUT_DEBUG( "sql: Main database initialization failed. Exiting." );
		_StopDB(); // Kekeke
		return false;
	}

	return true;
}

void Lacrimi::_StopDB()
{
	delete LacrimiDB;
	LacrimiDB = NULL;
}

// Use sMgr for Script Mgr.
void Lacrimi::SetupScripts()
{
	SetupZoneScripts();
	SetupCityScripts();
}

void Lacrimi::SetupZoneScripts()
{
	SetupOutlandScripts();
	SetupKalimdorScripts();
	SetupEbonHoldScripts();
	SetupNorthrendScripts();
	SetupEasternKingdomScripts();
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
}

void Lacrimi::SetupKalimdorScripts()
{
	SetupAshenvale();
	SetupAzshara();
	SetupAzuremystIsle();
	SetupBloodmystIsle();
	SetupDarkshore();
	SetupDesolace();
	SetupDustwallowMarsh();
	SetupFelwood();
	SetupFeralas();
	SetupMoonglade();
	SetupMulgore();
	SetupSilithus();
	SetupStonetalonMountains();
	SetupTanaris();
	SetupTeldrassil();
	SetupTheBarrens();
	SetupThousandNeedles();
	SetupUngoroCrater();
	SetupWinterspring();
}

void Lacrimi::SetupOutlandScripts()
{
	SetupBladesEdgeMountains();
	SetupHellfirePeninsula();
	SetupNagrand();
	SetupNetherstorm();
	SetupShadowmoonValley();
	SetupTerrokarForest();
	SetupZangarmarsh();
}

void Lacrimi::SetupNorthrendScripts()
{
	SetupBoreanTundra();
	SetupDragonblight();
	SetupGrizzlyHills();
	SetupHowlingFjord();
	SetupIcecrown();
	SetupSholazarBasin();
	SetupStormPeaks();
	SetupZulDrak();
}

void Lacrimi::SetupCityScripts()
{
	SetupIronforge();
	SetupSilvermoon();
	SetupStormwind();
	SetupUndercity();
	SetupDarnassus();
	SetupExodar();
	SetupOrgrimmar();
	SetupThunderbluff();
	SetupShattrath();
	SetupDalaran();
}
