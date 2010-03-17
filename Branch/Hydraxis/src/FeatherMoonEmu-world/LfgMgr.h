/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

enum LFGTypes 
{
	LFG_NONE                 = 0,
	LFG_DUNGEON              = 1,
	LFG_RAID                 = 2,
	LFG_QUEST                = 3,
	LFG_ZONE                 = 4,
	LFG_HEROIC_DUNGEON       = 5,
	LFG_ANY_DUNGEON          = 6,
	LFG_ANY_HEROIC_DUNGEON   = 7,
	LFG_DAILY_DUNGEON        = 8,
	LFG_DAILY_HEROIC_DUNGEON = 9
};

enum LfgRoles
{
    LEADER  = 0x01,
    TANK    = 0x02,
    HEALER  = 0x04,
    DAMAGE  = 0x08
};

struct LookingForGroupSlot
{
    LookingForGroupSlot() : entry(0), type(0) {}
    bool Empty() const { return !entry && !type; }
    void Clear() { entry = 0; type = 0; }
    void Set(uint32 _entry, uint32 _type ) { entry = _entry; type = _type; }
    bool Is(uint32 _entry, uint32 _type) const { return entry == _entry && type == _type; }
    bool canAutoJoin() const { return entry && (type == LFG_DUNGEON || type == LFG_HEROIC_DUNGEON); }

    uint32 entry;
    uint32 type;
};

#define MAX_LOOKING_FOR_GROUP_SLOT 3

#define MAX_DUNGEONS 280+1 // check max entrys +1 on lfgdungeons.dbc
#define MAX_LFG_QUEUE_ID 3
#define LFG_MATCH_TIMEOUT 30		// in seconds

class LfgMatch;
class LfgMgr : public Singleton < LfgMgr >, EventableObject
{
public:	
	
	typedef list<Player*  > LfgPlayerList;

	LfgMgr();
	~LfgMgr();
	
	bool AttemptLfgJoin(Player* pl, uint32 LfgDungeonId);
	void SetPlayerInLFGqueue(Player* pl,uint32 LfgDungeonId);
	void SetPlayerInLfmList(Player* pl, uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueue(Player* pl,uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueues(Player* pl);
	void RemovePlayerFromLfmList(Player* pl, uint32 LfmDungeonId);
	void UpdateLfgQueue(uint32 LfgDungeonId);
	void SendLfgList(Player* plr, uint32 Dungeon);
	void EventMatchTimeout(LfgMatch * pMatch);

	int32 event_GetInstanceId() { return -1; }
	
protected:
	
	LfgPlayerList m_lookingForGroup[MAX_DUNGEONS];
	LfgPlayerList m_lookingForMore[MAX_DUNGEONS];
	Mutex m_lock;
	
	
};

class LfgMatch
{
public:
	set<Player*  > PendingPlayers;
	set<Player*  > AcceptedPlayers;
	Mutex lock;
	uint32 DungeonId;
    Group * pGroup;

	LfgMatch(uint32 did) : DungeonId(did),pGroup(NULL) { }
};

extern uint32 LfgDungeonTypes[MAX_DUNGEONS];

#define sLfgMgr LfgMgr::getSingleton()
