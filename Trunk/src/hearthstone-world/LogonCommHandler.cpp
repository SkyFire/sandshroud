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
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		idhigh = 1;
		next_request = 1;
		pings = !Config.MainConfig.GetBoolDefault("LogonServer", "DisablePings", false);
		string logon_pass = Config.MainConfig.GetStringDefault("LogonServer", "RemotePassword", "r3m0t3");
		plrLimit = sWorld.GetPlayerLimit();
		if(!plrLimit)
			plrLimit = 1;

		// sha1 hash it
		Sha1Hash hash;
		hash.UpdateData(logon_pass);
		hash.Finalize();
		memcpy(sql_passhash, hash.GetDigest(), 20);
		ReConCounter = 0;
	}
}

LogonCommHandler::~LogonCommHandler()
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		for(set<LogonServer*>::iterator i = servers.begin(); i != servers.end(); i++)
			delete (*i);

		for(set<Realm*>::iterator i = realms.begin(); i != realms.end(); i++)
			delete (*i);
	}
}

LogonCommClientSocket * LogonCommHandler::ConnectToLogon(string Address, uint32 Port)
{
	LogonCommClientSocket * conn = NULL;
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		conn = ConnectTCPSocket<LogonCommClientSocket>(Address.c_str(), Port);
	}
	return conn;
}

void LogonCommHandler::RequestAddition(LogonCommClientSocket * Socket)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		set<Realm*>::iterator itr = realms.begin();
		WorldPacket data(RCMSG_REGISTER_REALM, 100);
		for(; itr != realms.end(); itr++)
		{
			data.clear();

			// Add realm to the packet
			if(!(sWorld.LogonServerType & LOGON_HEARTHSTONE))
			{	// Not Hearthstone, so use Ascent ;D
				Realm * realm = *itr;
				data << realm->Name;
				data << realm->Address;
				data << uint32(0);
				data << (uint32)realm->Icon;
				data << (uint32)realm->WorldRegion;
				data << float(realm->Population);
				if(sWorld.LogonServerType & LOGON_ARCEMU) // ArcEmu, so use Lock.
					data << uint8(realm->Lock);
			}
			else // Hearthstone :D
			{
				// Add realm to the packet
				Realm * realm = *itr;
				data << realm->Name;
				data << realm->Address;
				data << realm->Icon;
				data << realm->WorldRegion;
				data << realm->Population;
				data << realm->Lock;
			}

			Socket->SendPacket(&data,false);
		}
	}
}

class LogonCommWatcherThread : public ThreadContext
{
	bool running;
#ifdef WIN32
	HANDLE hEvent;
#endif
public:

	LogonCommWatcherThread()
	{
		if(!(sWorld.LogonServerType & LOGON_MANGOS))
		{
#ifdef WIN32
			hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
#endif
			running = true;
		}
	}

	~LogonCommWatcherThread()
	{

	}

	void OnShutdown()
	{
		if(!(sWorld.LogonServerType & LOGON_MANGOS))
		{
			m_threadRunning = false;
#ifdef WIN32
			SetEvent( hEvent );
#endif
		}
	}

	bool run()
	{
		if(!(sWorld.LogonServerType & LOGON_MANGOS))
		{
			sLogonCommHandler.ConnectAll();
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
		return false;
	}
};

void LogonCommHandler::Startup()
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
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

		ThreadPool.ExecuteTask( new LogonCommWatcherThread() );
	}
}

void LogonCommHandler::ConnectAll()
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		Log.Notice("LogonCommClient", "Attempting to connect to logon server...");
		for(set<LogonServer*>::iterator itr = servers.begin(); itr != servers.end(); itr++)
			Connect(*itr);
	}
}

const string* LogonCommHandler::GetForcedPermissions(string& username)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		ForcedPermissionMap::iterator itr = forced_permissions.find(username);
		if(itr == forced_permissions.end())
			return NULL;

		return &itr->second;
	}
	return NULL;
}

void LogonCommHandler::Connect(LogonServer * server)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		if(bServerShutdown)
			return;

		if(ReConCounter >= 5)
		{ // Attempt to connect 5 times, if not able to, shut down.
			sWorld.QueueShutdown(5, SERVER_SHUTDOWN_TYPE_SHUTDOWN);
			bServerShutdown = true;
			return;
		}

		++ReConCounter;

		Log.Notice("LogonCommClient", "Connecting to logonserver on `%s:%u, attempt %u`", server->Address.c_str(), server->Port, ReConCounter );

		server->RetryTime = (uint32)UNIXTIME + 10;
		server->Registered = false;

		LogonCommClientSocket * conn = ConnectToLogon(server->Address, server->Port);
		logons[server] = conn;
		if(conn == NULL)
		{
			Log.Notice("LogonCommClient", "Connection failed. Will try again in 10 seconds.");
			return;
		}
		Log.Notice("LogonCommClient", "Authenticating...");
		uint32 tt = (uint32)UNIXTIME + 10;
		conn->SendChallenge();
		while(!conn->authenticated)
		{
			if((uint32)UNIXTIME >= tt || bServerShutdown)
			{
				Log.Notice("LogonCommClient", "Authentication timed out.");
				conn->_id = 0;
				conn->Disconnect();
				logons[server] = NULL;
				return;
			}

			Sleep(50);
		}

		if(conn->authenticated != 1)
		{
			Log.Notice("LogonCommClient","Authentication failed.");
			conn->_id = 0;
			conn->Disconnect();
			logons[server] = NULL;
			return;
		}
		else
			Log.Notice("LogonCommClient","Authentication succeeded.");

		// Send the initial ping
		conn->SendPing();

		Log.Notice("LogonCommClient", "Registering Realms...");
		conn->_id = server->ID;

		RequestAddition(conn);

		uint32 st = (uint32)UNIXTIME + 10;

		// Wait for register ACK
		while(server->Registered == false)
		{
			// Don't wait more than.. like 10 seconds for a registration
			if((uint32)UNIXTIME >= st)
			{
				Log.Notice("LogonCommClient", "Realm registration timed out.");
				conn->_id = 0;
				conn->Disconnect();
				logons[server] = NULL;
				break;
			}
			Sleep(50);
		}

		if(!server->Registered)
			return;

		// Wait for all realms to register
		Sleep(200);

		Log.Notice("LogonCommClient", "Logonserver latency is %ums.", conn->latency);

		// We have connected, reset our attempt counter.
		ReConCounter = 0;
	}
}

void LogonCommHandler::AdditionAck(uint32 ID, uint32 ServID)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		for(; itr != logons.end(); itr++)
		{
			if(itr->first->ID == ID)
			{
				itr->first->ServerID = ServID;
				itr->first->Registered = true;
				return;
			}
		}
	}
}

void LogonCommHandler::UpdateSockets()
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		mapLock.Acquire();

		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		LogonCommClientSocket * cs = NULL;
		uint32 t = (uint32)UNIXTIME;
		for(; itr != logons.end(); itr++)
		{
			cs = itr->second;
			if(cs != NULL)
			{
				if(cs->IsDeleted() || !cs->IsConnected())
				{
					Log.Error("LogonCommClient","Realm id %u lost connection.", (unsigned int)itr->first->ID);
					cs->_id = 0;
					itr->second = 0;
					continue;
				}

				if(pings)
				{
					if(cs->last_pong < t && ((t - cs->last_pong) > 60))
					{
						// no pong for 60 seconds -> remove the socket
						Log.Error("LogonCommClient","Realm id %u connection dropped due to pong timeout.", (unsigned int)itr->first->ID);
						cs->_id = 0;
						cs->Disconnect();
						itr->second = 0;
						continue;
					}
					if( (t - cs->last_ping) > 15 )//ping every 15 seconds when connected
						cs->SendPing();
				}
			}
			else
			{
				// Try to reconnect
				if(t >= itr->first->RetryTime && !bServerShutdown)
				{
					Connect(itr->first);
				}
			}
		}
		mapLock.Release();
	}
}

void LogonCommHandler::ConnectionDropped(uint32 ID)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		if(bServerShutdown)
			return;
		mapLock.Acquire();
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		for(; itr != logons.end(); itr++)
		{
			if(itr->first->ID == ID && itr->second != 0)
			{
				if(!bServerShutdown)
					Log.Error("LogonCommHandler"," Realm id %u connection was dropped unexpectedly. reconnecting next loop.", ID);
				itr->second->_id = 0;
				itr->second->Disconnect();
				itr->second = NULL;
				break;
			}
		}
		mapLock.Release();
	}
}

uint32 LogonCommHandler::ClientConnected(string AccountName, WorldSocket * Socket)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		uint32 request_id = next_request++;
		size_t i = 0;
		const char * acct = AccountName.c_str();
		DEBUG_LOG( "LogonCommHandler","Sending request for account information: `%s` (request %u).", AccountName.c_str(), request_id);

		// Send request packet to server.
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0)
		{
			// No valid logonserver is connected.
			return (uint32)-1;
		}

		LogonCommClientSocket * s = itr->second;
		if( s == NULL )
			return (uint32)-1;

		pendingLock.Acquire();

		WorldPacket data(RCMSG_REQUEST_SESSION, 100);
		data << request_id;

		// strip the shitty hash from it
		for(; acct[i] != '#' && acct[i] != '\0'; i++ )
			data.append( &acct[i], 1 );

		data.append( "\0", 1 );
		s->SendPacket(&data,false);

		pending_logons[request_id] = Socket;
		pendingLock.Release();
		RefreshRealmPop();

		return request_id;
	}
	return (uint32)-1;
}

void LogonCommHandler::UnauthedSocketClose(uint32 id)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		pendingLock.Acquire();
		pending_logons.erase(id);
		pendingLock.Release();
	}
}

void LogonCommHandler::RemoveUnauthedSocket(uint32 id)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		pending_logons.erase(id);
	}
}

void LogonCommHandler::LoadRealmConfiguration()
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		LogonServer * ls = NULL;
		ls = new LogonServer;
		ls->ID = idhigh++;
		ls->Name = Config.RealmConfig.GetStringDefault("LogonServer", "Name", "UnkLogon");
		ls->Address = Config.RealmConfig.GetStringDefault("LogonServer", "Address", "127.0.0.1");
		ls->Port = Config.RealmConfig.GetIntDefault("LogonServer", "Port", 8093);
		servers.insert(ls);

		uint32 realmcount = Config.RealmConfig.GetIntDefault("LogonServer", "RealmCount", 1);
		if(realmcount == 0)
		{
			Log.Error("LogonCommHandler","No realms found. this server will not be online anywhere!");
		}
		else
		{
			for(uint32 i = 1; i < realmcount+1; i++)
			{
				Realm * realm = NULL;
				realm = new Realm;
				realm->Name = Config.RealmConfig.GetStringVA("Name", "SomeRealm", "Realm%u", i);
				realm->Address = Config.RealmConfig.GetStringVA("Address", "127.0.0.1:8129", "Realm%u", i);
				realm->WorldRegion = Config.RealmConfig.GetIntVA("WorldRegion", 1, "Realm%u", i);
				realm->Population = Config.RealmConfig.GetFloatVA("Population", 0, "Realm%u", i);
				string rt = Config.RealmConfig.GetStringVA("Icon", "Normal", "Realm%u", i);
				uint32 type;

				// process realm type
				if( stricmp(rt.c_str(), "pvp")==0 )
					type = REALMTYPE_PVP;
				else if( stricmp(rt.c_str(), "rp")==0 )
					type = REALMTYPE_RP;
				else if( stricmp(rt.c_str(), "rppvp")==0 )
					type = REALMTYPE_RPPVP;
				else
					type = REALMTYPE_NORMAL;

				_realmType = type;

				realm->Icon = type;
				realms.insert(realm);
			}
		}
	}
}

void LogonCommHandler::UpdateAccountCount(uint32 account_id, uint8 add)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		// Send request packet to server.
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}
		itr->second->UpdateAccountCount(account_id, add);
	}
}

void LogonCommHandler::TestConsoleLogon(string& username, string& password, uint32 requestnum)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		string newuser = username;
		string newpass = password;
		string srpstr;

		HEARTHSTONE_TOUPPER(newuser);
		HEARTHSTONE_TOUPPER(newpass);

		srpstr = newuser + ":" + newpass;

		// Send request packet to server.
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}

		Sha1Hash hash;
		hash.UpdateData(srpstr);
		hash.Finalize();

		WorldPacket data(RCMSG_TEST_CONSOLE_LOGIN, 100);
		data << requestnum;
		data << newuser;
		data.append(hash.GetDigest(), 20);

		itr->second->SendPacket(&data, false);
	}
}

// db funcs
void LogonCommHandler::Account_SetBanned(const char * account, uint32 banned, const char* reason)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}

		WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
		data << uint32(1);		// 1 = ban
		data << account;
		data << banned;
		data << reason;
		itr->second->SendPacket(&data, false);
	}
}

void LogonCommHandler::Account_SetGM(const char * account, const char * flags)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}

		WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
		data << uint32(2);		// 2 = set gm
		data << account;
		data << flags;
		itr->second->SendPacket(&data, false);
	}
}

void LogonCommHandler::Account_SetMute(const char * account, uint32 muted)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}

		WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
		data << uint32(3);		// 3 = mute
		data << account;
		data << muted;
		itr->second->SendPacket(&data, false);
	}
}

void LogonCommHandler::IPBan_Add(const char * ip, uint32 duration)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}

		WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
		data << uint32(4);		// 4 = ipban add
		data << ip;
		data << duration;
		itr->second->SendPacket(&data, false);
	}
}

void LogonCommHandler::IPBan_Remove(const char * ip)
{
	if(!(sWorld.LogonServerType & LOGON_MANGOS))
	{
		map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
		if(logons.size() == 0 || itr->second == 0)
		{
			// No valid logonserver is connected.
			return;
		}

		WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
		data << uint32(5);		// 5 = ipban remove
		data << ip;
		itr->second->SendPacket(&data, false);
	}
}

void LogonCommHandler::RefreshRealmPop()
{
	// Calc pop: 0 >= low, 1 >= med, 2 >= hig, 3 >= full
	server_population = (sWorld.HordePlayers+sWorld.AlliancePlayers) * 3.0f / plrLimit;
}

#endif
