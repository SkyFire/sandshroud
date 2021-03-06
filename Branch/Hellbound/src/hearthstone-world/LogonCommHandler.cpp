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
initialiseSingleton(LogonCommHandler);

#ifndef CLUSTERING

extern bool bServerShutdown;

LogonCommHandler::LogonCommHandler()
{
	idhigh = 1;
	next_request = 1;
	ReConCounter = 0;
	pings = !Config.RealmConfig.GetBoolDefault("LogonServer", "DisablePings", false);
	string logon_pass = Config.RealmConfig.GetStringDefault("LogonServer", "RemotePassword", "r3m0t3");
	plrLimit = sWorld.GetPlayerLimit();
	if(!plrLimit)
		plrLimit = 1;

	// sha1 hash it
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();
	memcpy(sql_passhash, hash.GetDigest(), 20);
}

LogonCommHandler::~LogonCommHandler()
{
	delete server;
	server = NULL;

	delete realm;
	realm = NULL;
}

LogonCommClientSocket * LogonCommHandler::ConnectToLogon(string Address, uint32 Port)
{
	return ConnectTCPSocket<LogonCommClientSocket>(Address.c_str(), Port);
}

void LogonCommHandler::RequestAddition(LogonCommClientSocket * Socket)
{
	WorldPacket data(RCMSG_REGISTER_REALM, 100);
	// Add realm to the packet
	data << realm->Name;
	data << realm->Address;
	data << uint16(0x042); // Six by nine. Forty two.
	data << realm->Icon;
	data << realm->WorldRegion;
	data << realm->Population;
	data << uint32(CL_BUILD_SUPPORT);
	data << realm->Lock;
	Socket->SendPacket(&data);
}

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

			HEARTHSTONE_TOUPPER(acct);
			forced_permissions.insert(make_pair(acct,perm));

		} while (result->NextRow());
		delete result;
	}
	ThreadPool.ExecuteTask(new LogonCommWatcherThread());
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
	if(bServerShutdown)
		return;

	if(ReConCounter >= 10)
	{ // Attempt to connect 5 times, if not able to, shut down.
		sWorld.QueueShutdown(5, SERVER_SHUTDOWN_TYPE_SHUTDOWN);
		bServerShutdown = true;
		return;
	}

	++ReConCounter;

	Log.Notice("LogonCommClient", "Connecting to logonserver on `%s:%u, attempt %u`", server->Address.c_str(), server->Port, ReConCounter );

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
		if((uint32)UNIXTIME >= tt || bServerShutdown)
		{
			Log.Notice("LogonCommClient", "Authentication timed out.");
			logon->Disconnect();
			logon = NULL;
			return;
		}

		Sleep(10);
	}

	if(logon->authenticated != 1)
	{
		Log.Notice("LogonCommClient","Authentication failed.");
		logon->Disconnect();
		logon = NULL;
		return;
	}
	else
		Log.Success("LogonCommClient","Authentication succeeded.");

	// Send the initial ping
	logon->SendPing();

	Log.Notice("LogonCommClient", "Registering Realms...");
	logon->_id = server->ID;

	RequestAddition(logon);

	uint32 st = (uint32)UNIXTIME + 15;

	// Wait for register ACK
	while(server->Registered == false)
	{
		// Don't wait more than.. like 15 seconds for a registration, thats our ping timeout
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

	Log.Success("LogonCommClient", "Logonserver latency is %ums.", logon->latency);

	// We have connected, reset our attempt counter.
	ReConCounter = 0;
}

void LogonCommHandler::AdditionAck(uint32 ServID)
{
	server->ServerID = ServID;
	server->Registered = true;
}

void LogonCommHandler::UpdateSockets()
{
	if(bServerShutdown)
		return;

	mapLock.Acquire();
	uint32 t = (uint32)UNIXTIME;
	if(logon != NULL)
	{
		if(logon->IsDeleted() || !logon->IsConnected())
		{
			Log.Error("LogonCommClient","Realm id %u lost connection.", (unsigned int)server->ID);
			logon->_id = 0;
			mapLock.Release();
			return;
		}

		if(pings)
		{
			if(logon->last_pong < t && ((t - logon->last_pong) > 60))
			{
				// no pong for 60 seconds -> remove the socket
				Log.Error("LogonCommClient","Realm id %u connection dropped due to pong timeout.", (unsigned int)server->ID);
				logon->_id = 0;
				logon->Disconnect();
				mapLock.Release();
				return;
			}
			if( (t - logon->last_ping) > 15 )//ping every 15 seconds when connected
				logon->SendPing();
		}
	}
	else
	{
		// Try to reconnect
		if(t >= server->RetryTime && !bServerShutdown)
			Connect();

		mapLock.Release();
	}
}

void LogonCommHandler::ConnectionDropped()
{
	if(bServerShutdown)
		return;

	if(logon != NULL)
	{
		if(!bServerShutdown)
			Log.Error("LogonCommHandler"," Realm connection was dropped unexpectedly. reconnecting next loop.");
		logon->_id = 0;
		logon->Disconnect();
		logon = NULL;
	}
	mapLock.Release();
}

uint32 LogonCommHandler::ClientConnected(string AccountName, WorldSocket * Socket)
{
	uint32 request_id = next_request++;
	size_t i = 0;
	const char * acct = AccountName.c_str();
	DEBUG_LOG( "LogonCommHandler","Sending request for account information: `%s` (request %u).", AccountName.c_str(), request_id);

	// Send request packet to server.
	if(logon == NULL)
	{
		// No valid logonserver is connected.
		return (uint32)-1;
	}

	pendingLock.Acquire();

	WorldPacket data(RCMSG_REQUEST_SESSION, 100);
	data << int32(-42);
	data << server->ID;
	data << request_id;

	// strip the shitty hash from it
	for(; acct[i] != '#' && acct[i] != '\0'; i++ )
		data.append( &acct[i], 1 );

	data.append( "\0", 1 );
	logon->SendPacket(&data, false);

	pending_logons[request_id] = Socket;
	pendingLock.Release();
	RefreshRealmPop();

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
	server = new LogonServer;
	server->ID = idhigh++;
	server->Name = Config.RealmConfig.GetStringDefault("LogonServer", "Name", "UnkLogon");
	server->Address = Config.RealmConfig.GetStringDefault("LogonServer", "Address", "127.0.0.1");
	server->Port = Config.RealmConfig.GetIntDefault("LogonServer", "Port", 8093);

	char* port = new char[10];
	itoa(Config.RealmConfig.GetIntDefault( "ServerSettings", "WorldServerPort", 8129), port, 10);
	std::string adress = string(Config.RealmConfig.GetStringDefault( "ServerSettings", "Adress", "SomeRealm" ).c_str())
		+ ":" + string(port);
	realm = new Realm();
	realm->Population = 1;
	realm->Address = adress;
	realm->Icon = Config.RealmConfig.GetIntDefault("ServerSettings", "RealmType", 1);
	realm->WorldRegion = Config.RealmConfig.GetIntDefault("ServerSettings", "WorldRegion", 1);
	realm->Name = Config.RealmConfig.GetStringDefault("ServerSettings", "RealmName", "SomeRealm");
	sWorld.IsPvPRealm = ((realm->Icon == REALMTYPE_RPPVP || realm->Icon == REALMTYPE_PVP) ? true : false);
}

void LogonCommHandler::UpdateAccountCount(uint32 account_id, int8 add)
{
	if(logon == NULL) // No valid logonserver is connected.
		return;

	logon->UpdateAccountCount(account_id, add);
}

void LogonCommHandler::TestConsoleLogon(string& username, string& password, uint32 requestnum)
{
	if(logon == NULL) // No valid logonserver is connected.
		return;

	string newuser = username;
	string newpass = password;
	string srpstr;

	HEARTHSTONE_TOUPPER(newuser);
	HEARTHSTONE_TOUPPER(newpass);

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
	if(logon == NULL) // No valid logonserver is connected.
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(1);		// 1 = ban
	data << account;
	data << banned;
	data << reason;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetGM(const char * account, const char * flags)
{
	if(logon == NULL) // No valid logonserver is connected.
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(2);		// 2 = set gm
	data << account;
	data << flags;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetMute(const char * account, uint32 muted)
{
	if(logon == NULL) // No valid logonserver is connected.
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(3);		// 3 = mute
	data << account;
	data << muted;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Add(const char * ip, uint32 duration, const char* reason)
{
	if(logon == NULL) // No valid logonserver is connected.
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(4);		// 4 = ipban add
	data << ip;
	data << duration;
	data << reason;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Remove(const char * ip)
{
	if(logon == NULL) // No valid logonserver is connected.
		return;

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(5);		// 5 = ipban remove
	data << ip;
	logon->SendPacket(&data, false);
}

void LogonCommHandler::RefreshRealmPop()
{
	// Calc pop: 0 >= low, 1 >= med, 2 >= hig, 3 >= full
	server_population = (sWorld.HordePlayers+sWorld.AlliancePlayers) * 3.0f / plrLimit;
}

#endif
