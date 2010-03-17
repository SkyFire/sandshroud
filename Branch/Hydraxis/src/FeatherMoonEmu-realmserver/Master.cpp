/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "RStdAfx.h"
#include "../FeatherMoonEmu-shared/svn_revision.h"

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

int main(int argc, char *argv[])
{
	//sLog.outString("TexT");
	/* Initialize global timestamp */
	UNIXTIME = time(NULL);

	ThreadPool.Startup();

    /* Print Banner */
	Log.Notice("Server", "==============================================================");
	Log.Notice("Server", "| FeatherMoonEmu - Realm Server									|");
	Log.Notice("Server", "| Revision %04u												|", BUILD_REVISION);
	Log.Notice("Server", "==============================================================");
	Log.Line();

	

	Database_Character = Database::Create();
	Database_World = Database::Create();
	Log.Success("Database", "Interface Created.");

	new ClusterMgr;
	new ClientMgr;
	
	Log.Line();
	Config.ClusterConfig.SetSource("./FeatherMoonEmu-realmserver.conf");
	Config.RealmConfig.SetSource("./FeatherMoonEmu-realms.conf");
	Log.Notice("Database", "Connecting to databases...");
	if(!_StartCharacterDatabase() ||
	   !_StartWorldDatabase() )
	{
		Log.Error("Database", "One or more errors occured while connecting to databases.");
		exit(-1);
	}
	else
	{
		Log.Success("Database", "Connections established successfully.");
	}

	ThreadPool.ShowStats();
	Log.Line();

	Log.Success("Storage", "DBC Files Loaded...");
	Storage_Load();

	Log.Line();

	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	/* connect to LS */
	new LogonCommHandler;
	sLogonCommHandler.Startup();

	Log.Success("Network", "Network Subsystem Started.");

	Log.Notice("Network", "Opening Client Port...");
	ListenSocket<WorldSocket> * wsl = new ListenSocket<WorldSocket>("0.0.0.0", 8129);
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

	/* main loop */
	for(;;)
	{
        sLogonCommHandler.UpdateSockets();
		//wsl->Update();
		//isl->Update();
		sClientMgr.Update();
		sClusterMgr.Update();
		Sleep(10);
	}

}

void OnCrash(bool Terminate)
{

}


