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
	//sLog.outString("TexT");
	/* Initialize global timestamp */
	UNIXTIME = time(NULL);

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

	new ClusterMgr;
	new ClientMgr;

	Log.Line();
	Config.ClusterConfig.SetSource("./hearthstone-realmserver.conf");
	Config.RealmConfig.SetSource("./hearthstone-realms.conf");

	if(!_StartDB())
	{
		Database::CleanupLibs();
		return false;
	}

	_HookSignals();

	ThreadPool.ShowStats();
	Log.Line();

	Log.Success("Storage", "DBC Files Loaded...");
	Storage_Load();

	Log.Line();

	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	Log.Notice("Network", "Opening Client Port...");
	ListenSocket<WorldSocket> * wsl = new ListenSocket<WorldSocket>(Config.ClusterConfig.GetStringDefault("Listen", "Host", "0.0.0.0").c_str(), Config.RealmConfig.GetIntDefault("Listen", "WorldServerPort", 8129));
	bool lsc = wsl->IsOpen();

	Log.Notice("Network", "Opening Server Port...");
	ListenSocket<WSSocket> * isl = new ListenSocket<WSSocket>("0.0.0.0", 11010);
	bool ssc = isl->IsOpen();

	if(!lsc || !ssc)
	{
		Log.Error("Network", "Could not open one of the sockets.");
		return 1;
	}

	ThreadPool.ExecuteTask( isl );
	ThreadPool.ExecuteTask( wsl );

	/* connect to LS */
	new LogonCommHandler;
	sLogonCommHandler.Startup();

	Log.Success("Network", "Network Subsystem Started.");

#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif
	/* main loop */
	while(!m_stopEvent)
	{
		sLogonCommHandler.UpdateSockets();
		//wsl->Update();
		//isl->Update();
		sClientMgr.Update();
		sClusterMgr.Update();
#ifdef WIN32
			WaitForSingleObject( hThread, 10);
#else
			Sleep(10);
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

void OnCrash(bool Terminate)
{

}
