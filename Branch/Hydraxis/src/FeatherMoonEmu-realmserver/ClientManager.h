/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#define MAX_SESSIONS 3000

class ClientMgr : public Singleton<ClientMgr>
{
public:
	typedef HM_NAMESPACE::hash_map<uint32, RPlayerInfo*> ClientMap;

protected:
	ClientMap m_clients;
	uint32 m_maxSessionId;
	Session * m_sessions[MAX_SESSIONS];

public:
	ClientMgr();
	~ClientMgr();
	
	/* create rplayerinfo struct */
	RPlayerInfo * CreateRPlayer(uint32 guid);

	/* destroy rplayerinfo struct */
	void DestroyRPlayerInfo(uint32 guid);

	/* get rplayer */
	FEATHERMOONEMU_INLINE RPlayerInfo * GetRPlayer(uint32 guid)
	{
		ClientMap::iterator itr = m_clients.find(guid);
		return (itr != m_clients.end()) ? itr->second : 0;
	}
	/* send "mini" client data to all servers */
	void SendPackedClientInfo(WServer * server);

	/* get session by id */
	FEATHERMOONEMU_INLINE Session * GetSession(uint32 Id) { return (Id < MAX_SESSIONS) ? m_sessions[Id] : 0; }

	/* create a new session, returns null if the player is already logged in */
	Session * CreateSession(uint32 AccountId);

	void RemoveSession(Session * sess);

	/* updates sessions */
	void Update();
};

#define sClientMgr ClientMgr::getSingleton()


