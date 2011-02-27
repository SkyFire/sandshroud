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

//
// mapMgr->h
//

#ifndef __MAPMGR_H
#define __MAPMGR_H

class MapCell;
class Map;
class Object;
#ifdef MULTI_THREADED_CELLS
class ObjectUpdateThread;
#endif
class MapScriptInterface;
class WorldSession;
class GameObject;
class Creature;
class Player;
class Pet;
class Vehicle;
class Transporter;
class Corpse;
class CBattleground;
class Instance;
class InstanceScript;


enum MapMgrTimers
{
	MMUPDATE_OBJECTS = 0,
	MMUPDATE_SESSIONS = 1,
	MMUPDATE_FIELDS = 2,
	MMUPDATE_IDLE_OBJECTS = 3,
	MMUPDATE_ACTIVE_OBJECTS = 4,
	MMUPDATE_COUNT = 5
};

enum ObjectActiveState
{
	OBJECT_STATE_NONE	 = 0,
	OBJECT_STATE_INACTIVE = 1,
	OBJECT_STATE_ACTIVE   = 2,
};

typedef unordered_set<Object* > ObjectSet;
typedef unordered_set<Object* > UpdateQueue;
typedef unordered_set<Player*  > PUpdateQueue;
typedef unordered_set<Player*  > PlayerSet;
typedef HM_NAMESPACE::hash_map<uint32, Object* > StorageMap;
typedef unordered_set<uint64> CombatProgressMap;
typedef unordered_set<Vehicle*> VehicleSet;
typedef unordered_set<Creature*> CreatureSet;
typedef unordered_set<GameObject* > GameObjectSet;
typedef HM_NAMESPACE::hash_map<uint32, Vehicle*> VehicleSqlIdMap;
typedef HM_NAMESPACE::hash_map<uint32, Creature*> CreatureSqlIdMap;
typedef HM_NAMESPACE::hash_map<uint32, GameObject* > GameObjectSqlIdMap;

#define MAX_TRANSPORTERS_PER_MAP 25

class Transporter;
#define RESERVE_EXPAND_SIZE 1024

#define CALL_INSTANCE_SCRIPT_EVENT( Mgr, Func ) if ( Mgr != NULL && Mgr->GetScript() != NULL ) Mgr->GetScript()->Func

class SERVER_DECL MapMgr : public CellHandler <MapCell>, public EventableObject, public ThreadContext
{
	friend class UpdateObjectThread;
	friend class ObjectUpdaterThread;
	friend class MapCell;
	friend class MapScriptInterface;
public:

	//This will be done in regular way soon

	Mutex m_objectinsertlock;
	ObjectSet m_objectinsertpool;
	void AddObject(Object*);
	Object* GetObjectClosestToCoords(uint32 entry, float x, float y, float z, float ClosestDist, int32 forcedtype = -1);

////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of GameObjects
/////////////////////////////////////////////
	typedef HM_NAMESPACE::hash_map<const uint32, GameObject* > GameObjectMap;
	GameObjectMap m_gameObjectStorage;
	uint32 m_GOHighGuid;
	GameObject* CreateGameObject(uint32 entry);

	HEARTHSTONE_INLINE const uint32 GenerateGameobjectGuid()
	{
		m_GOHighGuid &= 0x00FFFFFF;
		return ++m_GOHighGuid;
	}

	HEARTHSTONE_INLINE GameObject* GetGameObject(const uint32 guid)
	{
		GameObjectMap::iterator itr = m_gameObjectStorage.find(guid);
		return (itr != m_gameObjectStorage.end()) ? m_gameObjectStorage[guid] : NULLGOB;
	}

/////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of Vehicles
/////////////////////////////////////////////
	uint32 m_VehicleArraySize;
	uint32 m_VehicleHighGuid;
	HM_NAMESPACE::hash_map<const uint32,Vehicle*> m_VehicleStorage;
	Vehicle* CreateVehicle(uint32 entry);

	__inline Vehicle* GetVehicle(const uint32 guid)
	{
		return guid <= m_VehicleHighGuid ? m_VehicleStorage[guid] : NULLVEHICLE;
	}
/////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of Creatures
/////////////////////////////////////////////
	uint32 m_CreatureArraySize;
	uint32 m_CreatureHighGuid;
	HM_NAMESPACE::hash_map<const uint32,Creature*> m_CreatureStorage;
	Creature* CreateCreature(uint32 entry);

	HEARTHSTONE_INLINE Creature* GetCreature(const uint32 guid)
	{
		return guid <= m_CreatureHighGuid ? m_CreatureStorage[guid] : NULLCREATURE;
	}
//////////////////////////////////////////////////////////
// Local (mapmgr) storage/generation of DynamicObjects
////////////////////////////////////////////
	uint32 m_DynamicObjectHighGuid;
	typedef HM_NAMESPACE::hash_map<const uint32, DynamicObject*> DynamicObjectStorageMap;
	DynamicObjectStorageMap m_DynamicObjectStorage;
	DynamicObject* CreateDynamicObject();

	HEARTHSTONE_INLINE DynamicObject* GetDynamicObject(const uint32 guid)
	{
		DynamicObjectStorageMap::iterator itr = m_DynamicObjectStorage.find(guid);
		return (itr != m_DynamicObjectStorage.end()) ? m_DynamicObjectStorage[guid] : NULLDYN;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of pets
///////////////////////////////////////////
	typedef HM_NAMESPACE::hash_map<const uint32, Pet*> PetStorageMap;
	PetStorageMap m_PetStorage;
	__inline Pet* GetPet(const uint32 guid)
	{
		PetStorageMap::iterator itr = m_PetStorage.find(guid);
		return (itr != m_PetStorage.end()) ? m_PetStorage[guid] : NULLPET;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of players for faster lookup
////////////////////////////////

    // double typedef lolz// a compile breaker..
	typedef HM_NAMESPACE::hash_map<const uint32, Player*> PlayerStorageMap;

	PlayerStorageMap m_PlayerStorage;
	__inline Player* GetPlayer(const uint32 guid)
	{
		PlayerStorageMap::iterator itr = m_PlayerStorage.find(guid);
		return (itr != m_PlayerStorage.end()) ? m_PlayerStorage[guid] : NULLPLR;
	}

//////////////////////////////////////////////////////////
// Local (mapmgr) storage of combats in progress
////////////////////////////////
	CombatProgressMap _combatProgress;
	void AddCombatInProgress(uint64 guid)
	{
		_combatProgress.insert(guid);
	}
	void RemoveCombatInProgress(uint64 guid)
	{
		_combatProgress.erase(guid);
	}
	HEARTHSTONE_INLINE bool IsCombatInProgress()
	{
		//if all players are out, list should be empty.
		if(!HasPlayers())
			_combatProgress.clear();
		return (_combatProgress.size() > 0);
	}

//////////////////////////////////////////////////////////
// Lookup Wrappers
///////////////////////////////////
	Unit* GetUnit(const uint64 & guid);
	Object* _GetObject(const uint64 & guid);

	bool run();
	bool Do();

#ifdef MULTI_THREADED_CELLS
	ObjectUpdateThread* ObjectPusherThread;
#endif
	MapMgr(Map *map, uint32 mapid, uint32 instanceid);
	~MapMgr();
	void Init();

	void PushObject(Object* obj);
	void PushStaticObject(Object* obj);
	void RemoveObject(Object* obj, bool free_guid);
	void ChangeObjectLocation(Object* obj); // update inrange lists
	void ChangeFarsightLocation(Player* plr, Unit* farsight, bool apply);
	void ChangeFarsightLocation(Player* plr, float X, float Y, bool apply);
	bool IsInRange(float fRange, Object* obj, Object* currentobj)
	{
		// First distance check, are we in range?
		if(currentobj->GetDistance2dSq( obj ) > fRange )
			return false;

		// Second distance Check, we are in range, but are we in distance?
		if(currentobj->CalcDistance(obj) > fRange)
		{	// We aren't in distance, but since we are in range, check if we are in the same Area.
			if(currentobj->GetAreaID() != obj->GetAreaID())
				return false;	// We are not in the same area, fuck it.
		}
		return true;
	}

	//! Mark object as updated
	void ObjectUpdated(Object* obj);
	void UpdateCellActivity(uint32 x, uint32 y, int radius);

	// Terrain Functions
	HEARTHSTONE_INLINE float  GetLandHeight(float x, float y) { return GetBaseMap()->GetLandHeight(x, y); }
	HEARTHSTONE_INLINE float  GetWaterHeight(float x, float y) { return GetBaseMap()->GetWaterHeight(x, y); }
	HEARTHSTONE_INLINE uint8  GetWaterType(float x, float y) { return GetBaseMap()->GetWaterType(x, y); }
	HEARTHSTONE_INLINE uint8  GetWalkableState(float x, float y) { return GetBaseMap()->GetWalkableState(x, y); }
	HEARTHSTONE_INLINE uint16 GetAreaID(float x, float y, float z = 0.0f)
	{
		uint16 aid = GetBaseMap()->GetAreaID(x, y);
		if(_mapId == 571)
		{
			if((y < 1003.5413 && y > 269.7706) && (x < 6125.6840 && x > 5453.6235) && z > 546.0f)
			{
				if(z > 639.0f && z < 740.0f) // Better dirty fix for Dalaran
					return 4395;
				else if(z < 639.0f) // Dalaran Sewers: The Underbelly
					return 4560;
				else if(z > 740.0f) // Dalaran: The Violet Citadel
					return 4619;
			}
		}
		return aid;
	}

	HEARTHSTONE_INLINE uint32 GetMapId() { return _mapId; }

	void AddForcedCell(MapCell * c);
	void RemoveForcedCell(MapCell * c);

	void PushToProcessed(Player* plr);

	HEARTHSTONE_INLINE bool HasPlayers() { return (m_PlayerStorage.size() > 0); }
	void TeleportPlayers();

	HEARTHSTONE_INLINE uint32 GetInstanceID() { return m_instanceID; }
	HEARTHSTONE_INLINE MapInfo *GetMapInfo() { return pMapInfo; }
	HEARTHSTONE_INLINE MapEntry *GetdbcMap() { return pdbcMap; }

	HEARTHSTONE_INLINE void SetCollision(bool enable) { collision = enable; }
	HEARTHSTONE_INLINE bool IsCollisionEnabled() { return collision; }
	bool CanUseCollision(Object* obj);

	HEARTHSTONE_INLINE MapScriptInterface * GetInterface() { return ScriptInterface; }
	virtual int32 event_GetInstanceID() { return m_instanceID; }

	void LoadAllCells();
	HEARTHSTONE_INLINE size_t GetPlayerCount() { return m_PlayerStorage.size(); }

	void _PerformObjectDuties();
	uint32 mLoopCounter;
	uint32 lastGameobjectUpdate;
	uint32 lastUnitUpdate;
	void EventCorpseDespawn(uint64 guid);

	time_t InactiveMoveTime;
	uint32 iInstanceMode;

	void UnloadCell(uint32 x,uint32 y);
	void EventRespawnVehicle(Vehicle* v, MapCell * p);
	void EventRespawnCreature(Creature* c, MapCell * p);
	void EventRespawnGameObject(GameObject* o, MapCell * c);
	void SendMessageToCellPlayers(Object* obj, WorldPacket * packet, uint32 cell_radius = 2);
	void SendChatMessageToCellPlayers(Object* obj, WorldPacket * packet, uint32 cell_radius, uint32 langpos, int32 lang, WorldSession * originator);

	Instance * pInstance;
	void BeginInstanceExpireCountdown();
	void HookOnAreaTrigger(Player* plr, uint32 id);

	// better hope to clear any references to us when calling this :P
	void InstanceShutdown()
	{
		pInstance = NULL;
		Terminate();
	}

	// kill the worker thread only
	void KillThread()
	{
		pInstance=NULL;
		thread_kill_only = true;
		Terminate();
		while(thread_running)
		{
			Sleep(100);
		}
	}

protected:
	//! Collect and send updates to clients
	void _UpdateObjects();

private:
	//! Objects that exist on map

	uint32 _mapId;
	set<Object* > _mapWideStaticObjects;

	bool _CellActive(uint32 x, uint32 y);
	void UpdateInRangeSet(Object* obj, Player* plObj, MapCell* cell);
	void UpdateInRangeSet(uint64 guid, MapCell* cell);

public:
	void UpdateInrangeSetOnCells(uint64 guid, uint32 startX, uint32 endX, uint32 startY, uint32 endY);

	// Distance a Player can "see" other objects and receive updates from them (!! ALREADY dist*dist !!)
	float m_UpdateDistance;

private:
	/* Map Information */
	bool collision;
	MapInfo *pMapInfo;
	MapEntry* pdbcMap;
	uint32 m_instanceID;

	MapScriptInterface * ScriptInterface;

	/* Update System */
	FastMutex m_updateMutex;		// use a user-mode mutex for extra speed
	UpdateQueue _updates;
	PUpdateQueue _processQueue;

	/* Sessions */
	SessionSet MapSessions;

public:
#ifdef WIN32
	DWORD threadid;
#endif

	GameObjectSet activeGameObjects;
	CreatureSet activeCreatures;
	VehicleSet activeVehicles;
	EventableObjectHolder eventHolder;
	CBattleground* m_battleground;
	unordered_set<Corpse* > m_corpses;
	VehicleSqlIdMap _sqlids_vehicles;
	CreatureSqlIdMap _sqlids_creatures;
	GameObjectSqlIdMap _sqlids_gameobjects;

	Vehicle* GetSqlIdVehicle(uint32 sqlid);
	Creature* GetSqlIdCreature(uint32 sqlid);
	GameObject* GetSqlIdGameObject(uint32 sqlid);
	deque<uint32> _reusable_guids_creature;
	deque<uint32> _reusable_guids_vehicle;

	bool forced_expire;
	bool thread_kill_only;
	bool thread_running;

	// world state manager stuff
	WorldStateManager* m_stateManager;

	// bytebuffer caching
	ByteBuffer m_updateBuffer;
	ByteBuffer m_createBuffer;
	ByteBuffer m_updateBuildBuffer;
	ByteBuffer m_compressionBuffer;

public:

	// get!
	HEARTHSTONE_INLINE WorldStateManager& GetStateManager() { return *m_stateManager; }

	// send packet functions for state manager
	void SendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, WorldPacket *pData);
	void SendPacketToPlayers(int32 iZoneMask, int32 iFactionMask, StackPacket *pData);
	void SendPvPCaptureMessage(int32 iZoneMask, uint32 ZoneId, const char * Format, ...);

	// auras :< (world pvp)
	void RemoveAuraFromPlayers(int32 iFactionMask, uint32 uAuraId);
	void RemovePositiveAuraFromPlayers(int32 iFactionMask, uint32 uAuraId);
	void CastSpellOnPlayers(int32 iFactionMask, uint32 uSpellId);

public:

	// stored iterators for safe checking
	PetStorageMap::iterator __pet_iterator;
	PlayerStorageMap::iterator __player_iterator;

	VehicleSet::iterator __vehicle_iterator;
	CreatureSet::iterator __creature_iterator;
	GameObjectSet::iterator __gameobject_iterator;

	SessionSet::iterator __session_iterator_1;
	SessionSet::iterator __session_iterator_2;

	// Script related
	InstanceScript* GetScript() { return mInstanceScript; };
	void LoadInstanceScript();
	void CallScriptUpdate();

protected:

	InstanceScript* mInstanceScript;
};

class PushInfoContainer
{
public:
	PushInfoContainer(uint32 i1, uint32 i2, uint32 i3, uint32 i4)
	{ info[0] = i1; info[1] = i2; info[2] = i3; info[3] = i4; };

	~PushInfoContainer() {};
	uint32 info[4];
};

#ifdef MULTI_THREADED_CELLS

class SERVER_DECL ObjectUpdateThread :  public ThreadContext
{
public:
	ObjectUpdateThread(MapMgr* mgr) { Manager = mgr; PushMap.clear(); };
	~ObjectUpdateThread() { Manager = NULL; m_threadRunning = false; };
	void PushDataToObject(uint64 guid, uint32 startx, uint32 endx, uint32 starty, uint32 endy);
	bool m_threadRunning;

	bool run();
	void terminate()
	{
		PushMap.clear();
	};

private:
	MapMgr* Manager;
	std::map<uint64, set<PushInfoContainer*> > PushMap;
};

#endif

#endif
