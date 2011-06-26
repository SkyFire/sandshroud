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

#include "StdAfx.h"

#define BANNER "Sandshroud Hearthstone r%u/%s-%s-%s :: World Server\n"

createFileSingleton( Master );
std::string LogFileName;
bool bLogChat;
bool crashed = false;

volatile bool Master::m_stopEvent;

// Database defines.
SERVER_DECL Database* Database_Character;
SERVER_DECL Database* Database_World;
SERVER_DECL Database* Database_Log;

void Master::_OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		sWorld.Rehash(true);
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		remove( "hearthstone-world.pid" );
		Master::m_stopEvent = true;
		break;
	}

	signal(s, _OnSignal);
}

Master::Master()
{

}

Master::~Master()
{

}

struct Addr
{
	unsigned short sa_family;
	/* sa_data */
	unsigned short Port;
	unsigned long IP; // inet_addr
	unsigned long unusedA;
	unsigned long unusedB;
};

#define DEF_VALUE_NOT_SET 0xDEADBEEF

#ifdef WIN32
static const char* default_config_file = "configs/hearthstone-world.conf";
static const char* default_realm_config_file = "configs/hearthstone-realms.conf";
static const char* default_options_config_file = "configs/hearthstone-options.conf";
#else
static const char* default_config_file = CONFDIR "/hearthstone-world.conf";
static const char* default_realm_config_file = CONFDIR "/hearthstone-realms.conf";
static const char* default_options_config_file = CONFDIR "/hearthstone-options.conf";
#endif

bool bServerShutdown = false;
bool StartConsoleListener();
void CloseConsoleListener();
ThreadContext * GetConsoleListener();

bool Master::Run(int argc, char ** argv)
{
	m_stopEvent = false;
	char * config_file = (char*)default_config_file;
	char * realm_config_file = (char*)default_realm_config_file;
	char * options_config_file = (char*)default_options_config_file;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;
	int do_cheater_check = 0;
	int do_database_clean = 0;
	time_t curTime;

	struct hearthstone_option longopts[] =
	{
		{ "checkconf",			hearthstone_no_argument,			&do_check_conf,			1		},
		{ "screenloglevel",		hearthstone_required_argument,		&screen_log_level,		1		},
		{ "version",			hearthstone_no_argument,			&do_version,			1		},
		{ "cheater",			hearthstone_no_argument,			&do_cheater_check,		1		},
		{ "cleandb",			hearthstone_no_argument,			&do_database_clean,		1		},
		{ "conf",				hearthstone_required_argument,		NULL,					'c'		},
		{ "realmconf",			hearthstone_required_argument,		NULL,					'r'		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = hearthstone_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch (c)
		{
		case 'c':
			config_file = new char[strlen(hearthstone_optarg)];
			strcpy(config_file, hearthstone_optarg);
			break;

		case 'r':
			realm_config_file = new char[strlen(hearthstone_optarg)];
			strcpy(realm_config_file, hearthstone_optarg);
			break;

		case 'o':
			options_config_file = new char[strlen(hearthstone_optarg)];
			strcpy(options_config_file, hearthstone_optarg);
			break;

		case 0:
			break;
		default:
			sLog.m_screenLogLevel = 3;
			printf("Usage: %s [--checkconf] [--conf <filename>] [--realmconf <filename>] [--version]\n", argv[0]);
			return true;
		}
	}
	/* set new log levels if used as argument*/
	if( screen_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetScreenLoggingLevel(screen_log_level);

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	printf(BANNER, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
	printf("Built at %s on %s by %s@%s\n", BUILD_TIME, BUILD_DATE, BUILD_USER, BUILD_HOST);
	Log.Line();

	if( do_check_conf )
	{
		Log.Notice( "Config", "Checking config file: %s", config_file );
		if( Config.MainConfig.SetSource(config_file, true ) )
			Log.Success( "Config", "Passed without errors." );
		else
			Log.Warning( "Config", "Encountered one or more errors." );

		Log.Notice( "Config", "Checking config file: %s\n", realm_config_file );
		if( Config.RealmConfig.SetSource( realm_config_file, true ) )
			Log.Success( "Config", "Passed without errors.\n" );
		else
			Log.Warning( "Config", "Encountered one or more errors.\n" );

		Log.Notice( "Config", "Checking config file: %s\n", options_config_file );
		if( Config.OptionalConfig.SetSource( options_config_file, true ) )
			Log.Success( "Config", "Passed without errors.\n" );
		else
			Log.Warning( "Config", "Encountered one or more errors.\n" );
		return true;
	}

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time.\n" );
	Log.Line();

	//use these log_level until we are fully started up.
#ifdef _DEBUG
	sLog.Init(3);
#else
	sLog.Init(1);
#endif // _DEBUG

#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
		Log.LargeErrorMessage( LARGERRORMESSAGE_WARNING, "You are running Hearthstone as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
#endif

	InitRandomNumberGenerators();
	Log.Success( "Rnd", "Initialized Random Number Generators." );

	ThreadPool.Startup();
	uint32 LoadingTime = getMSTime();

	Log.Notice( "Config", "Loading Config Files..." );
	if( Config.MainConfig.SetSource( config_file ) )
		Log.Success( "Config", ">> hearthstone-world.conf" );
	else
	{
		Log.Error( "Config", ">> hearthstone-world.conf" );
		return false;
	}

	if(Config.RealmConfig.SetSource(realm_config_file))
		Log.Success( "Config", ">> hearthstone-realms.conf" );
	else
	{
		Log.Error( "Config", ">> hearthstone-realms.conf" );
		return false;
	}

	if(Config.OptionalConfig.SetSource(options_config_file))
		Log.Success( "Config", ">> hearthstone-options.conf" );
	else
	{
		Log.Error( "Config", ">> hearthstone-options.conf" );
		return false;
	}

	if(!_StartDB())
	{
		Database::CleanupLibs();
		return false;
	}

	Log.Line();
	sLog.outString( "" );

	new EventMgr;
	new World;

	/* load the config file */
	sWorld.Rehash(true);

	// Because of our log DB system, these have to be initialized different then rehash.
	sWorld.LogCheaters = Config.MainConfig.GetBoolDefault("Log", "Cheaters", false);
	sWorld.LogCommands = Config.MainConfig.GetBoolDefault("Log", "GMCommands", false);
	sWorld.LogPlayers = Config.MainConfig.GetBoolDefault("Log", "Player", false);
	sWorld.LogChats = Config.MainConfig.GetBoolDefault("Log", "Chat", false);

	//Update sLog to obey config setting
	sLog.Init(Config.MainConfig.GetIntDefault("LogLevel", "Screen", 1));

	// Initialize Opcode Table
	WorldSession::InitPacketHandlerTable();

	string host = Config.RealmConfig.GetStringDefault( "Listen", "Host", DEFAULT_HOST );
	int wsport = Config.RealmConfig.GetIntDefault( "ServerSettings", "WorldServerPort", DEFAULT_WORLDSERVER_PORT );

	new ScriptMgr;

	if( !sWorld.SetInitialWorldSettings() )
	{
		Log.Error( "Server", "SetInitialWorldSettings() failed. Something went wrong? Exiting." );
		return false;
	}

	sWorld.SetStartTime(uint32(UNIXTIME));

	WorldRunnable * wr = new WorldRunnable();
	ThreadPool.ExecuteTask(wr);

	_HookSignals();

	ConsoleThread* console = new ConsoleThread();
	ThreadPool.ExecuteTask(console);

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();

	// Socket loop!
	uint32 start;
	uint32 diff;
	uint32 last_time = now();
	uint32 etime;

	// Start Network Subsystem
	DEBUG_LOG("Server","Starting network subsystem..." );
	CreateSocketEngine();
	sSocketEngine.SpawnThreads();

	if( StartConsoleListener() )
	{
#ifdef WIN32
		ThreadPool.ExecuteTask( GetConsoleListener() );
#endif
		Log.Success("RemoteConsole", "Started and listening on port %i",Config.MainConfig.GetIntDefault("RemoteConsole", "Port", 8092));
	}
	else
		DEBUG_LOG("RemoteConsole", "Not enabled or failed listen.");

	LoadingTime = getMSTime() - LoadingTime;
	Log.Success("Server","Ready for connections. Startup time: %ums\n", LoadingTime );

	/* write pid file */
	FILE * fPid = fopen( "hearthstone-world.pid", "w" );
	if( fPid )
	{
		uint32 pid;
#ifdef WIN32
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf( fPid, "%u", uint(pid) );
		fclose( fPid );
	}
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif

	uint32 loopcounter = 0;
	//ThreadPool.Gobble();

	new ClusterInterface;
	sClusterInterface.ConnectToRealmServer();
	while(!m_stopEvent)
	{
		start = now();
		diff = start - last_time;
		if(! ((++loopcounter) % 10000) )		// 5mins
		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();//Checks if THREAD_RESERVE is met
		}

		/* since time() is an expensive system call, we only update it once per server loop */
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&curTime);
		}

		sClusterInterface.Update();
		sSocketDeleter.Update();

		/* UPDATE */
		last_time = now();
		etime = last_time - start;
		if( 50 > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, 50 - etime );
#else
			Sleep( 50 - etime );
#endif
		}
	}
	// begin server shutdown
	Log.Notice( "Shutdown", "Initiated at %s", ConvertTimeStampToDataTime( (uint32)UNIXTIME).c_str() );
	bServerShutdown = true;

	wr->Terminate();

	/* Shut down console system */
	CloseConsoleListener();
	console->terminate();
	delete console;

	if( lootmgr.is_loading )
	{
		Log.Notice( "Shutdown", "Waiting for loot to finish loading..." );
		while( lootmgr.is_loading )
			Sleep( 100 );
	}

	Log.Notice( "CharacterLoaderThread", "Exiting..." );
	sCLT.Terminate();

	sWorld.LogoutPlayers(); //(Also saves players).
	CharacterDatabase.Execute("UPDATE characters SET online = 0");

	// send a query to wake it up if its inactive
	Log.Notice( "Database", "Clearing all pending queries..." );

	// kill the database thread first so we don't lose any queries/data
	CharacterDatabase.EndThreads();
	WorldDatabase.EndThreads();

	if(Config.MainConfig.GetBoolDefault("Log", "Cheaters", false) || Config.MainConfig.GetBoolDefault("Log", "GMCommands", false)
		|| Config.MainConfig.GetBoolDefault("Log", "Player", false) || Config.MainConfig.GetBoolDefault("Log", "Chat", false))
		LogDatabase.EndThreads();

	Log.Notice("Server", "Shutting down random generator.");
	CleanupRandomNumberGenerators();

	if(wintergrasp)
	{
		Log.Notice( "WintergraspInternal", "Exiting..." );
		sWintergraspI.terminate();
	}

	Log.Notice( "DayWatcherThread", "Exiting..." );
	sDayWatcher.terminate();

	Log.Notice( "Network", "Shutting down network subsystem." );
	sSocketEngine.Shutdown();

	sAddonMgr.SaveToDB();
	Log.Notice("AddonMgr", "~AddonMgr()");
	delete AddonMgr::getSingletonPtr();

	Log.Notice("LootMgr", "~LootMgr()");
	delete LootMgr::getSingletonPtr();

	Log.Notice("MailSystem", "~MailSystem()");
	delete MailSystem::getSingletonPtr();

	ThreadPool.Shutdown();

	Log.Notice("CharacterLoaderThread", "~CharacterLoaderThread()");
	delete CharacterLoaderThread::getSingletonPtr();

	if(wintergrasp)
	{
		Log.Notice("WintergraspInternal", "~WintergraspInternal()");
		delete WintergraspInternal::getSingletonPtr();
	}

	Log.Notice("LogonComm", "~LogonCommHandler()");
	delete LogonCommHandler::getSingletonPtr();

	Log.Notice( "World", "~World()" );
	delete World::getSingletonPtr();

	Log.Notice( "ScriptMgr", "~ScriptMgr()" );
	sScriptMgr.UnloadScripts();
	delete ScriptMgr::getSingletonPtr();

	Log.Notice( "EventMgr", "~EventMgr()" );
	delete EventMgr::getSingletonPtr();

	Log.Notice( "Database", "Closing Connections..." );
	_StopDB();

	_UnhookSignals();

#ifdef WIN32
	WSACleanup();
#endif

	// remove pid
	remove( "hearthstone-world.pid" );

	Log.Notice( "Shutdown", "Shutdown complete." );
	Sleep(1000);
	return true;
}

bool Master::_StartDB()
{
	string hostname, username, password, database;
	int port = 0;
	// Configure Main Database

	bool result = Config.MainConfig.GetString( "WorldDatabase", "Username", &username );
	Config.MainConfig.GetString( "WorldDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "WorldDatabase", "Port", &port );
	Database_World = Database::Create();

	if(result == false)
	{
		OUT_DEBUG( "sql: One or more parameters were missing from WorldDatabase directive." );
		return false;
	}

	// Initialize it
	if( !WorldDatabase.Initialize(hostname.c_str(), uint(port), username.c_str(),
		password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 3 ), 16384 ) )
	{
		OUT_DEBUG( "sql: Main database initialization failed. Exiting." );
		return false;
	}

	result = Config.MainConfig.GetString( "CharacterDatabase", "Username", &username );
	Config.MainConfig.GetString( "CharacterDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "CharacterDatabase", "Port", &port );
	Database_Character = Database::Create();

	if(result == false)
	{
		OUT_DEBUG( "sql: One or more parameters were missing from Database directive." );
		return false;
	}

	// Initialize it
	if( !CharacterDatabase.Initialize( hostname.c_str(), (uint)port, username.c_str(),
		password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "CharacterDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		OUT_DEBUG( "sql: Main database initialization failed. Exiting." );
		return false;
	}

	if(Config.MainConfig.GetBoolDefault("Log", "Cheaters", false) || Config.MainConfig.GetBoolDefault("Log", "GMCommands", false)
		|| Config.MainConfig.GetBoolDefault("Log", "Player", false) || Config.MainConfig.GetBoolDefault("Log", "Chat", false))
	{
		result = Config.MainConfig.GetString( "LogDatabase", "Username", &username );
		Config.MainConfig.GetString( "LogDatabase", "Password", &password );
		result = !result ? result : Config.MainConfig.GetString( "LogDatabase", "Hostname", &hostname );
		result = !result ? result : Config.MainConfig.GetString( "LogDatabase", "Name", &database );
		result = !result ? result : Config.MainConfig.GetInt( "LogDatabase", "Port", &port );
		Database_Log = Database::Create();

		if(result == false)
		{
			OUT_DEBUG( "sql: One or more parameters were missing from Database directive." );
			return false;
		}

		// Initialize it
		if( !(LogDatabase.Initialize( hostname.c_str(), (uint)port, username.c_str(),
			password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "LogDatabase", "ConnectionCount", 5 ), 16384 )) )
		{
			OUT_DEBUG( "sql: Log database initialization failed. Exiting." );
			return false;
		}
	}

	return true;
}

void Master::_StopDB()
{
	delete Database_World;
	delete Database_Character;

	if(Config.MainConfig.GetBoolDefault("Log", "Cheaters", false) || Config.MainConfig.GetBoolDefault("Log", "GMCommands", false)
		|| Config.MainConfig.GetBoolDefault("Log", "Player", false) || Config.MainConfig.GetBoolDefault("Log", "Chat", false))
		delete Database_Log;
}

#ifndef WIN32
// Unix crash handler :oOoOoOoOoOo
volatile bool m_crashed = false;
void segfault_handler(int c)
{
	if( m_crashed )
	{
		abort();
		return;		// not reached
	}

	m_crashed = true;

	printf ("Segfault handler entered...\n");
	try
	{
		if( World::getSingletonPtr() != 0 )
		{
			sLog.outString( "Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			if(Config.MainConfig.GetBoolDefault("Log", "Cheaters", false) || Config.MainConfig.GetBoolDefault("Log", "GMCommands", false)
				|| Config.MainConfig.GetBoolDefault("Log", "Player", false) || Config.MainConfig.GetBoolDefault("Log", "Chat", false))
				LogDatabase.EndThreads();

			if(sWorld.LogonServerType & LOGON_MANGOS)
				if(Database_Account != NULL)
					AccountDatabase.EndThreads();
			sLog.outString( "All pending database operations cleared.\n" );
			sWorld.SaveAllPlayers();
			sLog.outString( "Data saved." );
		}
	}
	catch(...)
	{
		sLog.outString( "Threw an exception while attempting to save all data." );
	}

	printf("Writing coredump...\n");
	abort();
}
#endif


void Master::_HookSignals()
{/*
	const uint32 fieldSize = 81;
	const char* prefix = "SPELL_HASH_";
	uint32 prefixLen = uint32(strlen(prefix));
	DBCFile dbc;

	if( !dbc.open( "DBC/Spell.dbc" ) )
	{
		Log.Error("World", "Cannot find file ./DBC/Spell.dbc" );
		return;
	}
	uint32 cnt = (uint32)dbc.getRecordCount();
	uint32 namehash = 0;
	FILE * f = fopen("SpellNameHashes.h", "w");
	char spaces[fieldSize], namearray[fieldSize];
	strcpy(namearray, prefix);
	char* name = &namearray[prefixLen];
	for(int i=0;i<fieldSize-1;++i)
		spaces[i] = ' ';
	spaces[fieldSize-1] = 0;
	uint32 nameTextLen = 0, nameLen = 0;
	for(uint32 x=0; x < cnt; x++)
	{
		const char* nametext = dbc.getRecord(x).getString(139);
		nameTextLen = (unsigned int)strlen(nametext);
		strncpy(name, nametext, fieldSize-prefixLen-2);	// Cut it to fit in field size
		name[fieldSize-prefixLen-2] = 0; // in case nametext is too long and strncpy didn't copy the null
		nameLen = (unsigned int)strlen(name);
		for(uint32 i = 0;i<nameLen;++i)
		{
			if(name[i] >= 'a' && name[i] <= 'z')
				name[i] = toupper(name[i]);
			else if(!(name[i] >= '0' && name[i] <= '9') &&
				!(name[i] >= 'A' && name[i] <= 'Z'))
				name[i] = '_';
		}
		namehash = crc32((const unsigned char*)nametext, nameTextLen);
		int32 numSpaces = fieldSize-prefixLen-nameLen-1;
		if(numSpaces < 0)
			fprintf(f, "WTF");
		spaces[numSpaces] = 0;
		fprintf(f, "#define %s%s0x%08X\n", namearray, spaces, namehash);
		spaces[numSpaces] = ' ';
	}
	fclose(f);*/
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
#ifdef _WIN32
	signal( SIGBREAK, _OnSignal );
#else
	signal( SIGHUP, _OnSignal );
	signal(SIGUSR1, _OnSignal);

	// crash handler
	signal(SIGSEGV, segfault_handler);
	signal(SIGFPE, segfault_handler);
	signal(SIGILL, segfault_handler);
	signal(SIGBUS, segfault_handler);
#endif
}

void Master::_UnhookSignals()
{
	signal( SIGINT, 0 );
	signal( SIGTERM, 0 );
	signal( SIGABRT, 0 );
#ifdef _WIN32
	signal( SIGBREAK, 0 );
#else
	signal( SIGHUP, 0 );
#endif
}

#ifdef WIN32

Mutex m_crashedMutex;

// Crash Handler
void OnCrash( bool Terminate )
{
	sLog.outString( "Advanced crash handler initialized." );

	if( !m_crashedMutex.AttemptAcquire() )
		TerminateThread( GetCurrentThread(), 0 );

	try
	{
		if( World::getSingletonPtr() != 0 )
		{
			sLog.outString( "Waiting for all database queries to finish..." );
			sWorld.SaveAllPlayers();
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();

			if(Config.MainConfig.GetBoolDefault("Log", "Cheaters", false) || Config.MainConfig.GetBoolDefault("Log", "GMCommands", false)
				|| Config.MainConfig.GetBoolDefault("Log", "Player", false) || Config.MainConfig.GetBoolDefault("Log", "Chat", false))
				LogDatabase.EndThreads();

			sLog.outString( "All pending database operations cleared.\n" );
			sLog.outString( "Data saved." );
		}
	}
	catch(...)
	{
		sLog.outString( "Threw an exception while attempting to save all data." );
	}

	sLog.outString( "Closing." );

	// Terminate Entire Application
	if( Terminate )
	{
		HANDLE pH = OpenProcess( PROCESS_TERMINATE, TRUE, GetCurrentProcessId() );
		TerminateProcess( pH, 1 );
		CloseHandle( pH );
	}
}

#endif
