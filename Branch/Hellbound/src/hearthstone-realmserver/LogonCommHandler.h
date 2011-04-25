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

#ifndef RLOGONCOMMHANDLER_H
#define RLOGONCOMMHANDLER_H

typedef struct
{
	uint32 ID;
	string Name;
	string Address;
	uint32 Port;
	uint32 ServerID;
	uint32 RetryTime;
	bool   Registered;
}LogonServer;

typedef struct
{
	string Name;
	string Address;
	uint32 Colour;
	uint32 Icon;
	uint32 WorldRegion;
	float Population;
}Realm;

enum RealmType
{
	REALMTYPE_NORMAL	= 0,
	REALMTYPE_PVP		= 1,
	REALMTYPE_RP		= 6,
	REALMTYPE_RPPVP		= 8,
};

class SocketLoadBalancer;

class LogonCommHandler : public Singleton<LogonCommHandler>
{
	map<LogonServer*, LogonCommClientSocket*> logons;
	map<uint32, ClientSocket*> pending_logons;
	set<Realm*> realms;
	set<LogonServer*> servers;
	uint32 idhigh;
	uint32 next_request;
	Mutex mapLock;
	Mutex pendingLock;
	bool pings;
	uint32 _realmType;

public:
	uint8 sql_passhash[20];

	LogonCommHandler();
	~LogonCommHandler();

	LogonCommClientSocket * ConnectToLogon(string Address, uint32 Port);
	void UpdateAccountCount(uint32 account_id, int8 add);
	void RequestAddition(LogonCommClientSocket * Socket);
	void CheckAllServers();
	void Startup();
	void ConnectionDropped(uint32 ID);
	void AdditionAck(uint32 ID, uint32 ServID);
	void UpdateSockets();
	void Connect(LogonServer * server);
	void LogonDatabaseSQLExecute(const char* str, ...);
	void LogonDatabaseReloadAccounts();

	void LoadRealmConfiguration();
	void AddServer(string Name, string Address, uint32 Port);

	HEARTHSTONE_INLINE uint32 GetRealmType() { return _realmType; }
	void SetRealmType(uint32 type) { _realmType = type; }

	/////////////////////////////
	// ClientSocket stuff
	///////

	uint32 ClientConnected(string AccountName, ClientSocket * Socket);
	void UnauthedSocketClose(uint32 id);
	void RemoveUnauthedSocket(uint32 id);
	HEARTHSTONE_INLINE ClientSocket* GetSocketByRequest(uint32 id)
	{
		pendingLock.Acquire();

		ClientSocket * sock;
		map<uint32, ClientSocket*>::iterator itr = pending_logons.find(id);
		sock = (itr == pending_logons.end()) ? 0 : itr->second;

		pendingLock.Release();
		return sock;
	}
	HEARTHSTONE_INLINE Mutex & GetPendingLock() { return pendingLock; }		
};

#define sLogonCommHandler LogonCommHandler::getSingleton()

#endif

