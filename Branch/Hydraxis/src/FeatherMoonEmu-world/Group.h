/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef _GROUP_H_
#define _GROUP_H_

enum PartyErrors
{
	ERR_PARTY_NO_ERROR					= 0,
	ERR_PARTY_CANNOT_FIND				= 1,
	ERR_PARTY_IS_NOT_IN_YOUR_PARTY		= 2,
	ERR_PARTY_UNK						= 3,
	ERR_PARTY_IS_FULL					= 4,
	ERR_PARTY_ALREADY_IN_GROUP			= 5,
	ERR_PARTY_YOU_ARENT_IN_A_PARTY		= 6,
	ERR_PARTY_YOU_ARE_NOT_LEADER		= 7,
	ERR_PARTY_WRONG_FACTION				= 8,
	ERR_PARTY_IS_IGNORING_YOU			= 9,
};

enum GroupTypes
{
	GROUP_TYPE_NORMAL	= 0x00,
	GROUP_TYPE_BG		= 0x01,
	GROUP_TYPE_RAID		= 0x02,
	GROUP_TYPE_BGRAID	= GROUP_TYPE_BG | GROUP_TYPE_RAID,
	GROUP_TYPE_LFD		= 0x08,
	// 0x10 Leaving battleground, going to normal group
};

enum MaxGroupCount
{
	MAX_GROUP_SIZE_PARTY				= 5,
	MAX_GROUP_SIZE_RAID					= 40,
};

enum QuickGroupUpdateFlags
{
	PARTY_UPDATE_FLAG_POSITION			= 1,
	PARTY_UPDATE_FLAG_ZONEID			= 2,
};

enum GroupFlags
{
	GROUP_FLAG_DONT_DISBAND_WITH_NO_MEMBERS			= 1,
	GROUP_FLAG_REMOVE_OFFLINE_PLAYERS				= 2,
	GROUP_FLAG_BATTLEGROUND_GROUP					= 4,
};

enum PartyUpdateFlags
{
	GROUP_UPDATE_FLAG_NONE						= 0,		// 0x00000000
	GROUP_UPDATE_FLAG_ONLINE					= 1,		// 0x00000001  uint8
	GROUP_UPDATE_FLAG_HEALTH					= 2,		// 0x00000002  uint16
	GROUP_UPDATE_FLAG_MAXHEALTH					= 4,		// 0x00000004  uint16
	GROUP_UPDATE_FLAG_POWER_TYPE				= 8,		// 0x00000008  uint16
	GROUP_UPDATE_FLAG_POWER						= 16,		// 0x00000010  uint16
	GROUP_UPDATE_FLAG_MAXPOWER					= 32,		// 0x00000020  uint16
	GROUP_UPDATE_FLAG_LEVEL						= 64,		// 0x00000040  uint16
	GROUP_UPDATE_FLAG_ZONEID					= 128,		// 0x00000080  uint16
	GROUP_UPDATE_FLAG_POSITION					= 256,		// 0x00000100  uint16, uint16
	GROUP_UPDATE_FLAG_PLAYER_AURAS				= 512,		// 0x00000200  uint64, uint16 for each uint64
	GROUP_UPDATE_FLAG_PET_GUID					= 1024,		// 0x00000400  uint64
	GROUP_UPDATE_FLAG_PET_NAME					= 2048,		// 0x00000800  string
	GROUP_UPDATE_FLAG_PET_DISPLAYID				= 4096,		// 0x00001000  uint16
	GROUP_UPDATE_FLAG_PET_HEALTH				= 8192,		// 0x00002000  uint16
	GROUP_UPDATE_FLAG_PET_MAXHEALTH				= 16384,	// 0x00004000  uint16
	GROUP_UPDATE_FLAG_PET_POWER_TYPE			= 32768,	// 0x00008000  uint8
	GROUP_UPDATE_FLAG_PET_POWER					= 65535,	// 0x00010000  uint16
	GROUP_UPDATE_FLAG_PET_MAXPOWER				= 131070,	// 0x00020000  uint16
	GROUP_UPDATE_FLAG_PET_AURAS					= 262144,	// 0x00040000  uint64, uint16 for each uint64
	GROUP_UPDATE_FLAG_VEHICLE_SEAT				= 524288,	// 0x00080000, uint32 vehicle_seat_id (index from VehicleSeat.dbc)
};

enum PartyUpdateFlagGroups
{
	GROUP_UPDATE_TYPE_FULL_CREATE				=	GROUP_UPDATE_FLAG_ONLINE | GROUP_UPDATE_FLAG_HEALTH | GROUP_UPDATE_FLAG_MAXHEALTH |
													GROUP_UPDATE_FLAG_POWER | GROUP_UPDATE_FLAG_LEVEL |
													GROUP_UPDATE_FLAG_ZONEID | GROUP_UPDATE_FLAG_MAXPOWER | GROUP_UPDATE_FLAG_POSITION,
	GROUP_UPDATE_TYPE_FULL_REQUEST_REPLY		=   0x7FFC0BFF,
};

struct PlayerInfo;
typedef struct
{
	PlayerInfo * player_info;
	Player* player;
}GroupMember;

class Group;
class Player;

typedef std::set<PlayerInfo*> GroupMembersSet;

class SERVER_DECL SubGroup	  // Most stuff will be done through here, not through the "Group" class.
{
public:
	friend class Group;

	SubGroup(Group* parent, uint32 id):m_Parent(parent),m_Id(id)
	{
	}

	~SubGroup();

	FEATHERMOONEMU_INLINE GroupMembersSet::iterator GetGroupMembersBegin(void) { return m_GroupMembers.begin(); }
	FEATHERMOONEMU_INLINE GroupMembersSet::iterator GetGroupMembersEnd(void)   { return m_GroupMembers.end();   }

	bool AddPlayer(PlayerInfo * info);
	void RemovePlayer(PlayerInfo * info);
	
	FEATHERMOONEMU_INLINE bool IsFull(void)				{ return m_GroupMembers.size() >= MAX_GROUP_SIZE_PARTY; }
	FEATHERMOONEMU_INLINE size_t GetMemberCount(void)		{ return m_GroupMembers.size(); }
	
	FEATHERMOONEMU_INLINE uint32 GetID(void)			   { return m_Id; }
	FEATHERMOONEMU_INLINE void SetID(uint32 newid)		 { m_Id = newid; }

	FEATHERMOONEMU_INLINE void   SetParent(Group* parent)  { m_Parent = parent; }
	FEATHERMOONEMU_INLINE Group* GetParent(void)		   { return m_Parent; }

	void   Disband();
	bool HasMember(uint32 guid);

protected:

	GroupMembersSet	 m_GroupMembers;
	Group*			  m_Parent;
	uint32			  m_Id;

};

class Arena;
class SERVER_DECL Group
{
public:
	friend class SubGroup;

	static Group* Create();

	Group(bool Assign);
	~Group();

	// Adding/Removal Management
	bool AddMember(PlayerInfo * info, int32 subgroupid=-1);
	void RemovePlayer(PlayerInfo * info);

	// Leaders and Looting
	void SetLeader(Player* pPlayer,bool silent);
	void SetLooter(Player* pPlayer, uint8 method, uint16 threshold);

	// Transferring data to clients
	void Update();

	FEATHERMOONEMU_INLINE void SendPacketToAll(WorldPacket *packet) { SendPacketToAllButOne(packet, NULLPLR); }
	void SendPacketToAllButOne(WorldPacket *packet, Player* pSkipTarget);

	FEATHERMOONEMU_INLINE void SendPacketToAll(StackPacket *packet) { SendPacketToAllButOne(packet, NULLPLR); }
	void SendPacketToAllButOne(StackPacket *packet, Player* pSkipTarget);

	FEATHERMOONEMU_INLINE void OutPacketToAll(uint16 op, uint16 len, const void* data) { OutPacketToAllButOne(op, len, data, NULLPLR); }
	void OutPacketToAllButOne(uint16 op, uint16 len, const void* data, Player* pSkipTarget);

	void SendNullUpdate(Player* pPlayer);

	// Group Combat
	void SendPartyKillLog(Object* player, Object* Unit);

	// Destroying/Converting
	void Disband();
	Player* FindFirstPlayer();
	
	// Accessing functions
	FEATHERMOONEMU_INLINE SubGroup* GetSubGroup(uint32 Id)
	{
		if(Id >= 8)
			return 0;

		return m_SubGroups[Id];
	}

	FEATHERMOONEMU_INLINE uint32 GetSubGroupCount(void) { return m_SubGroupCount; }

	FEATHERMOONEMU_INLINE uint8 GetMethod(void) { return m_LootMethod; }
	FEATHERMOONEMU_INLINE uint16 GetThreshold(void) { return m_LootThreshold; }
	FEATHERMOONEMU_INLINE PlayerInfo* GetLeader(void) { return m_Leader; }
	FEATHERMOONEMU_INLINE PlayerInfo* GetLooter(void) { return m_Looter; }

	void MovePlayer(PlayerInfo* info, uint8 subgroup);

	bool HasMember(Player* pPlayer);
	bool HasMember(PlayerInfo * info);
	FEATHERMOONEMU_INLINE uint32 MemberCount(void) { return m_MemberCount; }
	FEATHERMOONEMU_INLINE bool IsFull() { return ((m_GroupType == GROUP_TYPE_NORMAL && m_MemberCount >= MAX_GROUP_SIZE_PARTY) || (m_GroupType == GROUP_TYPE_RAID && m_MemberCount >= MAX_GROUP_SIZE_RAID)); }

	SubGroup* FindFreeSubGroup();

	void ExpandToRaid();

	void SaveToDB();
	void LoadFromDB(Field *fields);

	FEATHERMOONEMU_INLINE uint8 GetGroupType() { return m_GroupType; }
	FEATHERMOONEMU_INLINE uint32 GetID() { return m_Id; }

	void UpdateOutOfRangePlayer(Player* pPlayer, uint32 Flags, bool Distribute, WorldPacket * Packet);
	void UpdateAllOutOfRangePlayersFor(Player* pPlayer);
	void HandleUpdateFieldChange(uint32 Index, Player* pPlayer);
	void HandlePartialChange(uint32 Type, Player* pPlayer);

	uint64 m_targetIcons[8];
	FEATHERMOONEMU_INLINE Mutex& getLock() { return m_groupLock; }
	FEATHERMOONEMU_INLINE void Lock() { m_groupLock.Acquire(); }
	FEATHERMOONEMU_INLINE void Unlock() { return m_groupLock.Release(); }
	bool m_isqueued;

	void SetAssistantLeader(PlayerInfo * pMember);
	void SetMainTank(PlayerInfo * pMember);
	void SetMainAssist(PlayerInfo * pMember);

	FEATHERMOONEMU_INLINE PlayerInfo * GetAssistantLeader() { return m_assistantLeader; }
	FEATHERMOONEMU_INLINE PlayerInfo * GetMainTank() { return m_mainTank; }
	FEATHERMOONEMU_INLINE PlayerInfo * GetMainAssist() { return m_mainAssist; }

	void SetDifficulty(uint8 difficulty);
	
	/************************************************************************/
	/* Voicechat                                                            */
	/************************************************************************/
#ifdef VOICE_CHAT
	void AddVoiceMember(PlayerInfo * pPlayer);
	void RemoveVoiceMember(PlayerInfo * pPlayer);
	void SendVoiceUpdate();
	void CreateVoiceSession();
	void VoiceChannelCreated(uint16 id);
	void VoiceSessionDropped();
	void VoiceSessionReconnected();

public:
	bool m_voiceChannelRequested;
	int16 m_voiceChannelId;
	uint32 m_voiceMemberCount;
#endif	// VOICE_CHAT

	FEATHERMOONEMU_INLINE void SetFlag(uint8 groupflag) { m_groupFlags |= groupflag; }
	FEATHERMOONEMU_INLINE void RemoveFlag(uint8 groupflag) { m_groupFlags &= ~groupflag; }
	FEATHERMOONEMU_INLINE bool HasFlag(uint8 groupflag) { return (m_groupFlags & groupflag) > 0 ? true : false; }

protected:
	PlayerInfo* m_voiceMembersList[41];
protected:
	PlayerInfo * m_Leader;
	PlayerInfo * m_Looter;
	PlayerInfo * m_assistantLeader;
	PlayerInfo * m_mainTank;
	PlayerInfo * m_mainAssist;

	uint8 m_LootMethod;
	uint16 m_LootThreshold;
	uint8 m_GroupType;
	uint32 m_Id;

	SubGroup* m_SubGroups[8];
	uint8 m_SubGroupCount;
	uint32 m_MemberCount;
	Mutex m_groupLock;
	bool m_dirty;
	bool m_updateblock;
	uint8 m_difficulty;
	uint8 m_raiddifficulty;
	uint8 m_groupFlags;

	// Evil prayer of mending stuff
public:
	uint8 m_prayerOfMendingCount;
	uint32 m_prayerOfMendingTarget;
};

#endif  // _GROUP_H_
