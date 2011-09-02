/*
 * onyx MMORPG Server
 * Copyright (C) 2005-2008 onyx Team <http://www.onyxemu.com/>
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

initialiseSingleton(LogonCommHandler);

LogonCommHandler::LogonCommHandler()
{
	realm = NULL;
	logon = NULL;
	server = NULL;

	idhigh = 1;
	next_request = 1;
	pings = !Config.RealmConfig.GetBoolDefault("LogonServer", "DisablePings", false);
	logon_pass = Config.RealmConfig.GetStringDefault("LogonServer", "RemotePassword", "r3m0t3");

	// sha1 hash it
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();
	memcpy(sql_passhash, hash.GetDigest(), 20);
}

LogonCommHandler::~LogonCommHandler()
{
	delete server;
	delete realm;
}

LogonCommClientSocket * LogonCommHandler::ConnectToLogon(string Address, uint32 Port)
{
	LogonCommClientSocket * conn = ConnectTCPSocket<LogonCommClientSocket>(Address.c_str(), Port);	
	return conn;
}

void LogonCommHandler::RequestAddition()
{
	if(logon == NULL)
		return;

	WorldPacket data(RCMSG_REGISTER_REALM, 100);
	data << realm->Name;
	data << realm->Address;
	data << uint16(0x042); // Six by nine. Forty two.
	data << realm->Icon;
	data << realm->WorldRegion;
	data << uint32(sWorld.GetPlayerLimit());
	data << uint8(2) << uint8(4) << uint8(3);
	data << uint32(CL_BUILD_SUPPORT);
	data << uint8(realm->Lock);
	logon->SendPacket(&data,false);
}

class LogonCommWatcherThread : public ThreadContext
{
#ifdef WIN32
	HANDLE hEvent;
#endif
public:

	LogonCommWatcherThread()
	{
#ifdef WIN32
		hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
#endif
	}

	~LogonCommWatcherThread()
	{

	}

	void OnShutdown()
	{
		m_threadRunning = false;
#ifdef WIN32
		SetEvent( hEvent );
#endif
	}

	bool run()
	{
		sLogonCommHandler.Connect();
		while( m_threadRunning )
		{
			sLogonCommHandler.UpdateSockets();
#ifdef WIN32
			WaitForSingleObject( hEvent, 3000 );
#else
			Sleep( 3000 );
#endif
		}

		return true;
	}
};

void LogonCommHandler::Startup()
{
	// Try to connect to all logons.
	LoadRealmConfiguration();

	Log.Notice("LogonCommClient", "Loading forced permission strings...");
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM account_forced_permissions");
	if( result != NULL )
	{
		do 
		{
			string acct = result->Fetch()[0].GetString();
			string perm = result->Fetch()[1].GetString();

			onyx_TOUPPER(acct);
			forced_permissions.insert(make_pair(acct,perm));

		} while (result->NextRow());
		delete result;
	}

	ThreadPool.ExecuteTask( new LogonCommWatcherThread() );
}

const string* LogonCommHandler::GetForcedPermissions(string& username)
{
	ForcedPermissionMap::iterator itr = forced_permissions.find(username);
	if(itr == forced_permissions.end())
		return NULL;

	return &itr->second;
}

void LogonCommHandler::Connect()
{
	Log.Notice("LogonCommClient", "Attempting to connect to logon server...");
	if(server == NULL)
		return;

	Log.Notice("LogonCommClient", "Connecting to logonserver on `%s:%u`...", server->Address.c_str(), server->Port );
	server->RetryTime = (uint32)UNIXTIME + 10;
	server->Registered = false;
	logon = ConnectToLogon(server->Address, server->Port);
	if(logon == NULL)
	{
		Log.Notice("LogonCommClient", "Connection failed. Will try again in 10 seconds.");
		return;
	}

	Log.Notice("LogonCommClient", "Authenticating...");
	uint32 tt = (uint32)UNIXTIME + 10;
	logon->SendChallenge();
	while(!logon->authenticated)
	{
		if((uint32)UNIXTIME >= tt)
		{
			Log.Notice("LogonCommClient", "Authentication timed out.");
			logon->Disconnect();
			logon = NULL;
			return;
		}

		Sleep(50);
	}

	if(logon->authenticated != 1)
	{
		Log.Notice("LogonCommClient","Authentication failed.");
		logon->Disconnect();
		logon = NULL;
		return;
	}
	else
		Log.Notice("LogonCommClient","Authentication succeeded.");

	// Send the initial ping
	logon->SendPing();

	Log.Notice("LogonCommClient", "Registering Realms...");
	logon->_id = server->ID;

	RequestAddition();

	uint32 st = (uint32)UNIXTIME + 10;

	// Wait for register ACK
	while(server->Registered == false)
	{
		// Don't wait more than.. like 10 seconds for a registration
		if((uint32)UNIXTIME >= st)
		{
			Log.Notice("LogonCommClient", "Realm registration timed out.");
			logon->Disconnect();
			logon = NULL;
			break;
		}
		Sleep(50);
	}

	if(!server->Registered)
		return;

	// Wait for all realms to register
	Sleep(200);

	Log.Notice("LogonCommClient", "Logonserver latency is %ums.", logon->latency);
}

void LogonCommHandler::AdditionAck(uint32 ID, uint32 ServID)
{
	if(server == NULL)
		return;

	server->ServerID = ServID;
	server->Registered = true;
}

void LogonCommHandler::UpdateSockets()
{
	if(server == NULL)
		return;

	uint32 t = (uint32)UNIXTIME;
	if(logon != 0)
	{
		if(!pings)
			return;

		if(logon->IsDeleted() || !logon->IsConnected())
		{
			logon->_id = 0;
			logon = NULL;
			return;
		}

		if(logon->last_pong < t && ((t - logon->last_pong) > 60))
		{
			// no pong for 60 seconds -> remove the socket
			printf(" >> realm id %u connection dropped due to pong timeout.\n", (unsigned int)server->ID);
			logon->_id = 0;
			logon->Disconnect();
			logon = NULL;
			return;
		}

		if( (t - logon->last_ping) > 15 )
		{
			// send a ping packet.
			logon->SendPing();
		}
	}
	else
	{
		// check retry time
		if(t >= server->RetryTime)
			Connect();
	}
}

void LogonCommHandler::ConnectionDropped(uint32 ID)
{
	if(server == NULL || logon == NULL)
		return;

	if(server->ID == ID)
	{
		sLog.outColor(TNORMAL, " >> realm id %u connection was dropped unexpectedly. reconnecting next loop.", ID);
		sLog.outColor(TNORMAL, "\n");
		logon = NULL;
	}
}

uint32 LogonCommHandler::ClientConnected(string AccountName, WorldSocket * Socket)
{
	uint32 request_id = next_request++;
	size_t i = 0;
	const char * acct = AccountName.c_str();
	DEBUG_LOG ( " >> sending request for account information: `%s` (request %u).", AccountName.c_str(), request_id);
  //  sLog.outColor(TNORMAL, "\n");
	
	// Send request packet to server.
	if(server == NULL)
		return uint32(-1);

	if( logon == NULL )
		return uint32(-1);

	pendingLock.Acquire();

	WorldPacket data(RCMSG_REQUEST_SESSION, 100);
	data << int32(-42);
	data << server->ID;
	data << request_id;

	// strip the shitty hash from it
	for(; acct[i] != '#' && acct[i] != '\0'; i++ )
		data.append( &acct[i], 1 );
	
	data.append( "\0", 1 );
	logon->SendPacket(&data,false);

	pending_logons[request_id] = Socket;
	pendingLock.Release();

	return request_id;
}

void LogonCommHandler::UnauthedSocketClose(uint32 id)
{
	pendingLock.Acquire();
	pending_logons.erase(id);
	pendingLock.Release();
}

void LogonCommHandler::RemoveUnauthedSocket(uint32 id)
{
	pending_logons.erase(id);
}

void LogonCommHandler::LoadRealmConfiguration()
{
	if(server != NULL)
	{
		delete server;
		server = NULL;
	}

	server = new LogonServer();
	ZeroMemory(server, sizeof(LogonServer*));
	server->ID = idhigh++;
	server->Name = Config.RealmConfig.GetStringDefault("LogonServer", "Name", "UnkLogon");
	server->Address = Config.RealmConfig.GetStringDefault("LogonServer", "Address", "127.0.0.1");
	server->Port = Config.RealmConfig.GetIntDefault("LogonServer", "Port", 8093);

	char* port = new char[10];
	itoa(Config.RealmConfig.GetIntDefault( "Listen", "Port", 8129), port, 10);
	std::string adress = string(Config.RealmConfig.GetStringDefault( "ServerSettings", "Adress", "Localhost" ).c_str())
		+ ":" + string(port);

	realm = new Realm();
	ZeroMemory(realm, sizeof(Realm*));
	realm->Address = adress;
	realm->Icon = Config.RealmConfig.GetIntDefault("ServerSettings", "RealmType", 1);
	realm->Name = Config.RealmConfig.GetStringDefault("ServerSettings", "Name", "SomeRealm");
	realm->WorldRegion = Config.RealmConfig.GetIntDefault("ServerSettings", "WorldRegion", 1);
	sWorld.IsPvPRealm = ((realm->Icon == REALMTYPE_RPPVP || realm->Icon == REALMTYPE_PVP) ? true : false);
}

void LogonCommHandler::UpdateAccountCount(uint32 account_id, uint8 add)
{
	if(logon == NULL)
		return;
	logon->UpdateAccountCount(account_id, add);
}

void LogonCommHandler::TestConsoleLogon(string& username, string& password, uint32 requestnum)
{
	if(logon == NULL)
		return;

	string newuser = username;
	string newpass = password;
	string srpstr;

	onyx_TOUPPER(newuser);
	onyx_TOUPPER(newpass);

	srpstr = newuser + ":" + newpass;

	Sha1Hash hash;
	hash.UpdateData(srpstr);
	hash.Finalize();

	WorldPacket data(RCMSG_TEST_CONSOLE_LOGIN, 100);
	data << requestnum;
	data << newuser;
	data.append(hash.GetDigest(), 20);

	logon->SendPacket(&data, false);
}

// db funcs
void LogonCommHandler::Account_SetBanned(const char * account, uint32 banned, const char* reason)
{
	if(logon == NULL)
		return; // Should QUEUE our packet.

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(1);		// 1 = ban
	data << account;
	data << banned;
	data << reason;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetGM(const char * account, const char * flags)
{
	if(logon == NULL)
		return; // Should QUEUE our packet.

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(2);		// 2 = set gm
	data << account;
	data << flags;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetMute(const char * account, uint32 muted)
{
	if(logon == NULL)
		return; // Should QUEUE our packet.

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(3);		// 3 = mute
	data << account;
	data << muted;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Add(const char * ip, uint32 duration)
{
	if(logon == NULL)
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(4);		// 4 = ipban add
	data << ip;
	data << duration;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Remove(const char * ip)
{
	if(logon == NULL)
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(5);		// 5 = ipban remove
	data << ip;
	logon->SendPacket(&data, false);
}
