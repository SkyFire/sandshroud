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

bool Master::Run(int argc, char ** argv)
{
	time_t curTime;

	/* Initialize global timestamp */
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	/* Print Banner */
	sLog.outString("Hearthstone r%u/%s-%s(%s)::Realm Server\n", BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
	printf("Built at %s on %s by %s@%s\n", BUILD_TIME, BUILD_DATE, BUILD_USER, BUILD_HOST);
	sLog.outString("==============================================================================");
	Log.Line();

	//use these log_level until we are fully started up.
#ifdef _DEBUG
	sLog.Init(3);
#else
	sLog.Init(1);
#endif // _DEBUG

	Config.ClusterConfig.SetSource(default_cluster_config_file);
	Config.OptionalConfig.SetSource(default_optional_config_file);
	Config.RealmConfig.SetSource(default_realm_config_file);
	Config.MainConfig.SetSource(default_world_config_file);

	InitRandomNumberGenerators();
	ThreadPool.Startup();

	if(!_StartDB())
	{
		Database::CleanupLibs();
		return false;
	}

	if(!LoadRSDBCs(Config.MainConfig.GetStringDefault("Data", "DBCPath", "dbc").c_str()))
	{
		Log.LargeErrorMessage(LARGERRORMESSAGE_ERROR, "One or more of the DBC files are missing.", "These are absolutely necessary for the server to function.", "The server will not start without them.", NULL);
		return false;
	}

	new ClusterMgr;
	new ClientMgr;

	_HookSignals();

	ThreadPool.ShowStats();

	Log.Success("Storage", "DBC Files Loaded...");
	Storage_Load();

	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	uint32 port = Config.RealmConfig.GetIntDefault("Listen", "WorldServerPort", 8129);
	const char* host = Config.RealmConfig.GetStringDefault("Listen", "Host", DEFAULT_HOST).c_str();

	Log.Notice("Network", "Opening Client Port...");
	ListenSocket<ClientSocket> * wsl = new ListenSocket<ClientSocket>(host, port);
	bool lsc = wsl->IsOpen();

	Log.Notice("Network", "Opening Server Port...");
	ListenSocket<MasterServerSocket> * isl = new ListenSocket<MasterServerSocket>(DEFAULT_HOST, 11010);
	bool ssc = isl->IsOpen();

	if(!lsc || !ssc)
	{
		Log.Error("Network", "Could not open %s socket.", lsc ? "Server" : "Client");
		Sleep(3000);
		return 1;
	}

	ThreadPool.ExecuteTask( isl );
	ThreadPool.ExecuteTask( wsl );

	/* connect to LS */
	new LogonCommHandler;
	sLogonCommHandler.Startup();
	Log.Success("Network", "Network Subsystem Started.");

	//Update sLog to obey config setting
	sLog.Init(Config.ClusterConfig.GetIntDefault("LogLevel", "Screen", 1));

#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif
	/* main loop */
	while(!m_stopEvent)
	{
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&curTime);
		}

		sLogonCommHandler.UpdateSockets();
		sClientMgr.Update();
		sClusterMgr.Update();
#ifdef WIN32
		WaitForSingleObject(hThread, 50);
#else
		Sleep(50);
#endif
	}

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

void OnCrash(bool Terminate)
{

}
