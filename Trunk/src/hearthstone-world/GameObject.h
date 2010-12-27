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

#ifndef WOWSERVER_GAMEOBJECT_H
#define WOWSERVER_GAMEOBJECT_H

class Player;
class GameObjectAIScript;
class GameObjectTemplate;

#pragma pack(push,1)
struct GameObjectInfo
{
	uint32 ID;
	uint32 Type;
	uint32 DisplayID;
	char * Name;
	char * Icon;
	char * CastBarText;
	uint32 SpellFocus;
	uint32 DefaultFlags;
	uint32 sound1;
	uint32 sound2;
	uint32 sound3;
	uint32 sound4;
	uint32 sound5;
	uint32 sound6;
	uint32 sound7;
	uint32 sound8;
	uint32 sound9;
	uint32 Unknown1;
	uint32 Unknown2;
	uint32 Unknown3;
	uint32 Unknown4;
	uint32 Unknown5;
	uint32 Unknown6;
	uint32 Unknown7;
	uint32 Unknown8;
	uint32 Unknown9;
	uint32 Unknown10;
	uint32 Unknown11;
	uint32 Unknown12;
	uint32 Unknown13;
	uint32 Unknown14;

	uint32 *InvolvedQuestIds;
	uint32 InvolvedQuestCount;
};
#pragma pack(pop)

enum GAMEOBJECT_BYTES
{
	GAMEOBJECT_BYTES_STATE			= 0,
	GAMEOBJECT_BYTES_TYPE_ID		= 1,
	GAMEOBJECT_BYTES_UNK			= 2, // todo: unknown atm
	GAMEOBJECT_BYTES_ANIMPROGRESS	= 3,
};

enum GAMEOBJECT_TYPES
{
	GAMEOBJECT_TYPE_DOOR					= 0,
	GAMEOBJECT_TYPE_BUTTON					= 1,
	GAMEOBJECT_TYPE_QUESTGIVER				= 2,
	GAMEOBJECT_TYPE_CHEST					= 3,
	GAMEOBJECT_TYPE_BINDER					= 4,
	GAMEOBJECT_TYPE_GENERIC					= 5,
	GAMEOBJECT_TYPE_TRAP					= 6,
	GAMEOBJECT_TYPE_CHAIR					= 7,
	GAMEOBJECT_TYPE_SPELL_FOCUS				= 8,
	GAMEOBJECT_TYPE_TEXT					= 9,
	GAMEOBJECT_TYPE_GOOBER					= 10,
	GAMEOBJECT_TYPE_TRANSPORT				= 11,
	GAMEOBJECT_TYPE_AREADAMAGE				= 12,
	GAMEOBJECT_TYPE_CAMERA					= 13,
	GAMEOBJECT_TYPE_MAP_OBJECT				= 14,
	GAMEOBJECT_TYPE_MO_TRANSPORT			= 15,
	GAMEOBJECT_TYPE_DUEL_ARBITER			= 16,
	GAMEOBJECT_TYPE_FISHINGNODE				= 17,
	GAMEOBJECT_TYPE_RITUAL					= 18,
	GAMEOBJECT_TYPE_MAILBOX					= 19,
	GAMEOBJECT_TYPE_AUCTIONHOUSE			= 20,
	GAMEOBJECT_TYPE_GUARDPOST				= 21,
	GAMEOBJECT_TYPE_SPELLCASTER				= 22,
	GAMEOBJECT_TYPE_MEETINGSTONE			= 23,
	GAMEOBJECT_TYPE_FLAGSTAND				= 24,
	GAMEOBJECT_TYPE_FISHINGHOLE				= 25,
	GAMEOBJECT_TYPE_FLAGDROP				= 26,
	GAMEOBJECT_TYPE_MINI_GAME				= 27,
	GAMEOBJECT_TYPE_LOTTERY_KIOSK			= 28,
	GAMEOBJECT_TYPE_CAPTURE_POINT			= 29,
	GAMEOBJECT_TYPE_AURA_GENERATOR			= 30,
	GAMEOBJECT_TYPE_DUNGEON_DIFFICULTY		= 31,
	GAMEOBJECT_TYPE_BARBER_CHAIR			= 32,
	GAMEOBJECT_TYPE_DESTRUCTIBLE_BUILDING	= 33,
	GAMEOBJECT_TYPE_GUILD_BANK				= 34,
	GAMEOBJECT_TYPE_TRAPDOOR				= 35
};

enum GameObjectFlags
{
	GO_FLAG_IN_USE			= 0x001,						//disables interaction while animated
	GO_FLAG_LOCKED			= 0x002,						//require key, spell, event, etc to be opened. Makes "Locked" appear in tooltip
	GO_FLAG_INTERACT_COND	= 0x004,						//cannot interact (condition to interact)
	GO_FLAG_TRANSPORT		= 0x008,						//any kind of transport? Object can transport (elevator, boat, car)
	GO_FLAG_UNK1			= 0x010,
	GO_FLAG_NODESPAWN		= 0x020,						//never despawn, typically for doors, they just change state
	GO_FLAG_TRIGGERED		= 0x040,						//typically, summoned objects. Triggered by spell or other events
	GO_FLAG_DAMAGED			= 0x200,
	GO_FLAG_DESTROYED		= 0x400,
};

enum GameObjectDynFlags
{
	GO_DYNFLAG_QUEST		= 0x09,
};

enum GoUInt32Types
{
	GO_UINT32_HEALTH			= 0,
	GO_UINT32_MINES_REMAINING	= 1, // Used for mining to mark times it can be mined
	GO_UINT32_M_RIT_CASTER		= 2,
	GO_UINT32_M_RIT_TARGET		= 3,
	GO_UINT32_RIT_SPELL			= 4,
	GO_UINT32_MAX
};

#define CALL_GO_SCRIPT_EVENT(obj, func) if(obj->GetTypeId() == TYPEID_GAMEOBJECT && obj->GetScript() != NULL) obj->GetScript()->func

class SERVER_DECL GameObject : public Object
{
public:
	GameObject(uint64 guid);
	~GameObject( );
	virtual void Init();

	HEARTHSTONE_INLINE GameObjectInfo* GetInfo() { return pInfo; }
	HEARTHSTONE_INLINE void SetInfo(GameObjectInfo * goi) { pInfo = goi; }

	bool CreateFromProto(uint32 entry,uint32 mapid, const LocationVector vec, float orientation1 = 0.0f, float orientation2 = 0.0f, float orientation3 = 0.0f, float orientation4 = 0.0f);
	bool CreateFromProto(uint32 entry,uint32 mapid, float x, float y, float z, float ang, float orientation1, float orientation2, float orientation3, float orientation4);

	bool Load(GOSpawn *spawn);

	virtual void Update(uint32 p_time);

	void Spawn( MapMgr* m);
	void Despawn( uint32 delay, uint32 respawntime);
	void UpdateTrapState();

	// Serialization
	void SaveToDB();
	void SaveToFile(std::stringstream & ss);
	void Export(std::stringstream & name);
	void DeleteFromDB();
	void EventCloseDoor();
	void UpdateRotation(float orientation3 = 0.0f, float orientation4 = 0.0f);

	//Fishing stuff
	void UseFishingNode(Player* player);
	void EndFishing(Player* player,bool abort);
	void FishHooked(Player* player);

	// Quests
	void _LoadQuests();
	bool HasQuests() { return m_quests != NULL; };
	void AddQuest(QuestRelation *Q);
	void DeleteQuest(QuestRelation *Q);
	Quest *FindQuest(uint32 quest_id, uint8 quest_relation);
	uint16 GetQuestRelation(uint32 quest_id);
	uint32 NumOfQuests();
	std::list<QuestRelation *>::iterator QuestsBegin() { return m_quests->begin(); };
	std::list<QuestRelation *>::iterator QuestsEnd() { return m_quests->end(); };
	void SetQuestList(std::list<QuestRelation *>* qst_lst) { m_quests = qst_lst; };

	void SetSummoned(Unit* mob)
	{
		m_summoner = mob;
		m_summonedGo = true;
	}

	Unit* CreateTemporaryGuardian(uint32 guardian_entry,uint32 duration,float angle, Unit* u_caster, uint8 Slot);
	void _Expire();

	void ExpireAndDelete();
	void ExpireAndDelete(uint32 delay);

	HEARTHSTONE_INLINE bool isQuestGiver()
	{
		if( GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID) == 2)
			return true;
		else
			return false;
	};

	/// Quest data
	std::list<QuestRelation *>* m_quests;

	uint32 *m_ritualmembers;
	uint64 m_rotation;

	void InitAI();
	SpellEntry* spell;

	bool initiated;
	bool m_created;
	float range;
	uint8 checkrate;
	uint16 counter;
	int32 charges;//used for type==22,to limit number of usages.
	bool invisible;//invisible
	uint8 invisibilityFlag;
	Unit* m_summoner;
	int8 bannerslot;
	int8 bannerauraslot;
	CBattleground* m_battleground;

	void CallScriptUpdate();

	HEARTHSTONE_INLINE GameObjectAIScript* GetScript() { return myScript; }

	void TrapSearchTarget();	// Traps need to find targets faster :P

	HEARTHSTONE_INLINE bool HasAI() { return spell != 0; }
	GOSpawn * m_spawn;
	void OnPushToWorld();
	void OnRemoveInRangeObject(Object* pObj);
	void RemoveFromWorld(bool free_guid);

	HEARTHSTONE_INLINE bool CanMine(){return (m_Go_Uint32Values[GO_UINT32_MINES_REMAINING] > 0);}
	HEARTHSTONE_INLINE void UseMine(){ if(m_Go_Uint32Values[GO_UINT32_MINES_REMAINING]) m_Go_Uint32Values[GO_UINT32_MINES_REMAINING]--;}
	void CalcMineRemaining(bool force)
	{
		m_Go_Uint32Values[GO_UINT32_MINES_REMAINING] = 0; // 3.0.9
	}

	uint32 GetGOReqSkill();
	MapCell * m_respawnCell;

	HEARTHSTONE_INLINE void SetScript(GameObjectAIScript *pScript) { myScript = pScript; }

	// loooot
	void GenerateLoot();

//	custom functions for scripting
	void SetState(uint8 state) { SetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE, state); }
	uint8 GetState() { return GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_STATE); }
	void SetAnimProgress(uint32 animprogress) { SetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_ANIMPROGRESS, animprogress ); }
	uint32 GetAnimProgress() { return GetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_ANIMPROGRESS ); }
	void SetFlags(uint32 flags) { SetUInt32Value(GAMEOBJECT_FLAGS, flags ); }
	uint32 GetFlags() { return GetUInt32Value( GAMEOBJECT_FLAGS ); }
	void SetType(uint8 type) { SetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID, type ); }
	uint8 GetType() { return GetByte( GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID ); }
	void SetLevel(uint32 level) { SetUInt32Value( GAMEOBJECT_LEVEL, level ); }
	uint32 GetLevel() { return GetUInt32Value( GAMEOBJECT_LEVEL ); }
	void SetDisplayId(uint32 id) { SetUInt32Value( GAMEOBJECT_DISPLAYID, id ); }
	uint32 GetDisplayId() { return GetUInt32Value( GAMEOBJECT_DISPLAYID ); }

	//Destructable Building
	void TakeDamage(uint32 amount, Object* mcaster, Player* pcaster, uint32 spellid = 0);
	void Destroy();
	void Damage();
	void Rebuild();
	//Aura Generator
	void AuraGenSearchTarget();

	uint32 GetGOui32Value(uint32 id)
	{
		if(id < GO_UINT32_MAX)
			return m_Go_Uint32Values[id];

		return 0;
	};

	void SetGOui32Value(uint32 id, uint32 value)
	{
		if(id < GO_UINT32_MAX)
			m_Go_Uint32Values[id] = value;
	};

protected:
	bool m_summonedGo;
	bool m_deleted;
	GameObjectInfo *pInfo;
	GameObjectAIScript * myScript;
	uint32 _fields[GAMEOBJECT_END];
	uint32 m_Go_Uint32Values[GO_UINT32_MAX]; // Crow: We could probably get away with using doubles...
};

#endif
