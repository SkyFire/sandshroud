/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
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
	map<uint32, WorldSocket*> pending_logons;
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
	void UpdateAccountCount(uint32 account_id, uint8 add);
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

	FEATHERMOONEMU_INLINE uint32 GetRealmType() { return _realmType; }
	void SetRealmType(uint32 type) { _realmType = type; }

	/////////////////////////////
	// Worldsocket stuff
	///////

	uint32 ClientConnected(string AccountName, WorldSocket * Socket);
	void UnauthedSocketClose(uint32 id);
	void RemoveUnauthedSocket(uint32 id);
	FEATHERMOONEMU_INLINE WorldSocket* GetSocketByRequest(uint32 id)
	{
		pendingLock.Acquire();

		WorldSocket * sock;
		map<uint32, WorldSocket*>::iterator itr = pending_logons.find(id);
		sock = (itr == pending_logons.end()) ? 0 : itr->second;

		pendingLock.Release();
		return sock;
	}
	FEATHERMOONEMU_INLINE Mutex & GetPendingLock() { return pendingLock; }		
};

#define sLogonCommHandler LogonCommHandler::getSingleton()

#endif

