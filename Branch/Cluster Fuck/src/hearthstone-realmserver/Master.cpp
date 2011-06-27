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

#include "RStdAfx.h"

createFileSingleton( Master );
Database * Database_Character;
Database * Database_World;

bool _StartCharacterDatabase()
{
	string host = Config.ClusterConfig.GetStringDefault("CharacterDatabase", "Hostname", "localhost");
	string user = Config.ClusterConfig.GetStringDefault("CharacterDatabase", "Username", "root");
	string pw = Config.ClusterConfig.GetStringDefault("CharacterDatabase", "Password", "");
	string database = Config.ClusterConfig.GetStringDefault("CharacterDatabase", "Name", "Character");
	uint32 port = Config.ClusterConfig.GetIntDefault("CharacterDatabase", "Port", 3306);
	return Database_Character->Initialize(host.c_str(), port, user.c_str(), pw.c_str(), database.c_str(), 5, 16384);
}

bool _StartWorldDatabase()
{
	string host = Config.ClusterConfig.GetStringDefault("WorldDatabase", "Hostname", "localhost");
	string user = Config.ClusterConfig.GetStringDefault("WorldDatabase", "Username", "root");
	string pw = Config.ClusterConfig.GetStringDefault("WorldDatabase", "Password", "");
	string database = Config.ClusterConfig.GetStringDefault("WorldDatabase", "Name", "World");
	uint32 port = Config.ClusterConfig.GetIntDefault("WorldDatabase", "Port", 3306);
	return Database_World->Initialize(host.c_str(), port, user.c_str(), pw.c_str(), database.c_str(), 5, 16384);
}

void Master::_OnSignal(int s)
{
	switch (s)
	{
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		Master::m_stopEvent = true;
		break;
	}

	signal(s, _OnSignal);
}

volatile bool Master::m_stopEvent = false;

Master::Master()
{

}

Master::~Master()
{

}

bool Master::_StartDB()
{
	Database_Character = Database::Create();
	Database_World = Database::Create();
	Log.Success("Database", "Interface Created.");
	Log.Notice("Database", "Connecting to databases...");
	if(!_StartCharacterDatabase() ||
		!_StartWorldDatabase() )
	{
		Log.Error("Database", "One or more errors occured while connecting to databases.");
		return false;
	}
	else
		Log.Success("Database", "Connections established successfully.");

	return true;
}

bool StartConsoleListener();
void CloseConsoleListener();
ThreadContext * GetConsoleListener();

bool Master::Run(int argc, char ** argv)
{
	//sLog.outString("TexT");
	/* Initialize global timestamp */
	time_t curTime;
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	ThreadPool.Startup();

	//use these log_level until we are fully started up.
#ifdef _DEBUG
	sLog.Init(3);
#else
	sLog.Init(1);
#endif // _DEBUG

	/* Print Banner */
	Log.Notice("Server", "==============================================================");
	Log.Notice("Server", "| Hearthstone - Realm Server                     |");
	Log.Notice("Server", "| Revision %04u                                 |", BUILD_REVISION);
	Log.Notice("Server", "==============================================================");
	Log.Line();
	Config.ClusterConfig.SetSource(default_cluster_config_file);
	Config.RealmConfig.SetSource(default_realm_config_file);

	if(!_StartDB())
	{
		Database::CleanupLibs();
		return false;
	}

	if(!LoadRSDBCs(Config.ClusterConfig.GetStringDefault("Data", "DBCPath", "dbc").c_str()))
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "One or more of the DBC files are missing.", "These are absolutely necessary for the server to function.", "The server will not start without them.", NULL);
		return false;
	}
	Log.Success("Storage", "DBC Files Loaded...");

	Log.Success("ClusterMgr", "Starting Cluster Manager.");
	ThreadPool.ExecuteTask(new ClusterMgr);
	Log.Success("ClientMgr", "Starting Client Manager.");
	ThreadPool.ExecuteTask(new ClientMgr);

	_HookSignals();

	Log.Success("Storage", "Loading Storage...");
	Storage_Load();

	new iocpEngine;
	sSocketEngine.SpawnThreads();
	Log.Success("Network", "Network Subsystem Started.");

	if( StartConsoleListener() )
	{
#ifdef WIN32
		ThreadPool.ExecuteTask( GetConsoleListener() );
#endif
		Log.Success("RemoteConsole", "Started and listening on port %i",Config.MainConfig.GetIntDefault("RemoteConsole", "Port", 8092));
	}
	else
		DEBUG_LOG("RemoteConsole", "Not enabled or failed listen.");

	uint32 start;
	uint32 diff;
	uint32 etime;
	uint32 last_time = now();
	uint32 loopcounter = 0;

	/* connect to LS */
	new LogonCommHandler();

	Log.Notice("Network", "Opening Client Port...");
	ListenSocket<WorldSocket> * wsl = new ListenSocket<WorldSocket>();
	bool lsc = wsl->Open(Config.ClusterConfig.GetStringDefault("Listen", "Host", "0.0.0.0").c_str(),
		Config.RealmConfig.GetIntDefault("Listen", "WorldServerPort", 8129));

	Log.Notice("Network", "Opening Server Port...");
	ListenSocket<WSSocket> * isl = new ListenSocket<WSSocket>();
	bool ssc = isl->Open("0.0.0.0", 11010);
	if(!ssc)
	{
		Log.Error("Network", "Could not open one of the sockets.");
		return 1;
	}

	sLogonCommHandler.Startup();

	//Update sLog to obey config setting
	sLog.Init(Config.ClusterConfig.GetIntDefault("LogLevel", "Screen", 1));

	Log.Success("Console", "Starting Console Thread.");
	ConsoleThread* console = new ConsoleThread();
	ThreadPool.ExecuteTask(console);

#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif
	/* main loop */
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

	CloseConsoleListener();
	console->terminate();
	delete console;

	_UnhookSignals();
	return true;
}

void Master::_HookSignals()
{
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
#ifdef _WIN32
	signal( SIGBREAK, _OnSignal );
#else
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
		sLogonCommHandler.ConnectionDropped();
		sLog.outString( "Waiting for all database queries to finish..." );
		WorldDatabase.EndThreads();
		CharacterDatabase.EndThreads();

		sLog.outString( "All pending database operations cleared.\n" );
		sLog.outString( "Data saved." );
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
