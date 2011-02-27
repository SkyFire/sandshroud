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
// Map.h
//

#ifndef __MAP_H
#define __MAP_H

class MapMgr;
class TemplateMgr;
struct MapInfo;
class TerrainMgr;

struct Formation;

typedef struct EventIdInfo
{
	uint8  eventid;
	uint8  eventchangesflag;
	uint32 eventdisplayid;
	int32  eventphase;
	uint32 eventitem1;
	uint32 eventitem2;
	uint32 eventitem3;
}EventIdInfo;

typedef struct CreatureSpawn
{
	uint32	id;//spawn ID
	uint32	entry;
	float	x;
	float	y;
	float	z;
	float	o;
	Formation* form;
	uint8	movetype;
	uint32	displayid;
	uint32	factionid;
	uint32	flags;
	uint32	bytes;
	uint32	bytes1;
	uint32	bytes2;
	uint32	emote_state;
//	uint32	respawnNpcLink;
	uint16	channel_spell;
	uint32	channel_target_go;
	uint32	channel_target_creature;
	uint16	stand_state;
	uint32	MountedDisplayID;
	uint32	ItemSlot1;
	uint32	ItemSlot2;
	uint32	ItemSlot3;
	int32	phase;
	int32	vehicle;
	uint8	CanMove;
}CreatureSpawn;

typedef struct GOSpawn
{
	uint32	id;//spawn ID
	uint32	entry;
	float	x;
	float	y;
	float	z;
	float	orientation1;
	float	orientation2;
	float	orientation3;
	float	orientation4;
	float	facing;
	uint32	flags;
	uint32	state;
	uint32	faction;
	float   scale;
	int32   phase;
} GOSpawn;

typedef std::vector<CreatureSpawn*> CreatureSpawnList;
typedef std::vector<GOSpawn*> GOSpawnList;

typedef struct
{
	CreatureSpawnList CreatureSpawns;
	GOSpawnList GOSpawns;
}CellSpawns;

class SERVER_DECL Map
{
public:
	Map(uint32 mapid, MapInfo * inf);
	~Map();

	HEARTHSTONE_INLINE string GetNameString() { return name; }
	HEARTHSTONE_INLINE const char* GetName() { return name.c_str(); }
	HEARTHSTONE_INLINE MapEntry* GetDBCEntry() { return me; }

	HEARTHSTONE_INLINE CellSpawns *GetSpawnsList(uint32 cellx,uint32 celly)
	{
		ASSERT(cellx < _sizeX);
		ASSERT(celly < _sizeY);
		if(spawns[cellx]==NULL) return NULL;

		return spawns[cellx][celly];
	}
	HEARTHSTONE_INLINE CellSpawns * GetSpawnsListAndCreate(uint32 cellx, uint32 celly)
	{
		ASSERT(cellx < _sizeX);
		ASSERT(celly < _sizeY);
		if(spawns[cellx]==NULL)
		{
			spawns[cellx] = new CellSpawns*[_sizeY];
			memset(spawns[cellx],0,sizeof(CellSpawns*)*_sizeY);
		}

		if(spawns[cellx][celly] == 0)
			spawns[cellx][celly] = new CellSpawns;
		return spawns[cellx][celly];
	}

	void LoadSpawns(bool reload = false);//set to true to make clean up
	uint32 CreatureSpawnCount;
	TerrainMgr* GetMapTerrain() { return _terrain; };

	HEARTHSTONE_INLINE float  GetLandHeight(float x, float y)
	{
		if(_terrain)
		{
			return _terrain->GetLandHeight(x, y);
		}
		else
		{
			return NO_LAND_HEIGHT;
		}
	}

	HEARTHSTONE_INLINE float  GetWaterHeight(float x, float y)
	{
		if(_terrain)
		{
			return _terrain->GetWaterHeight(x, y);
		}
		else
		{
			return NO_LAND_HEIGHT;
		}
	}

	HEARTHSTONE_INLINE uint8  GetWaterType(float x, float y)
	{
		if(_terrain)
		{
			return _terrain->GetWaterType(x, y);
		}
		else
		{
			return 0;
		}
	}

	HEARTHSTONE_INLINE uint8  GetWalkableState(float x, float y)
	{
		if(_terrain)
		{
			return _terrain->GetWalkableState(x, y);
		}
		else
		{
			return 1;
		}
	}

	HEARTHSTONE_INLINE uint16 GetAreaID(float x, float y)
	{
		if(_terrain)
		{
			return _terrain->GetAreaID(x, y);
		}
		else
		{
			return 0xFFFF;
		}
	}

	HEARTHSTONE_INLINE void CellGoneActive(uint32 x, uint32 y)
	{
		if(_terrain)
		{
			_terrain->CellGoneActive(x,y);
		}
	}

	HEARTHSTONE_INLINE void CellGoneIdle(uint32 x,uint32 y)
	{
		if(_terrain)
		{
			_terrain->CellGoneIdle(x,y);
		}
	}

private:
	MapInfo* _mapInfo;
	TerrainMgr* _terrain;
	uint32 _mapId;
	string name;
	MapEntry* me;

	//new stuff
	CellSpawns **spawns[_sizeX];
public:
	CellSpawns staticSpawns;
};

#endif
