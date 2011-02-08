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

initialiseSingleton( LfgMgr );

LfgMgr::LfgMgr()
{
	MaxDungeonID = 0;
	uint32 levelgroup[2];
	levelgroup[0] = LFG_LEVELGROUP_NONE;
	levelgroup[1] = LFG_LEVELGROUP_NONE;
	DBCStorage<LookingForGroup>::iterator itr;
	for(itr = dbcLookingForGroup.begin(); itr != dbcLookingForGroup.end(); ++itr)
	{
		if((*itr)->ID > MaxDungeonID)
			MaxDungeonID = (*itr)->ID;

		levelgroup[0] = GetPlayerLevelGroup((*itr)->minlevel);
		if(levelgroup[0] != LFG_LEVELGROUP_NONE)
			DungeonsByLevel[levelgroup[0]].insert((*itr)->ID);

		levelgroup[1] = GetPlayerLevelGroup((*itr)->maxlevel);
		if(levelgroup[1] != LFG_LEVELGROUP_NONE)
			if(levelgroup[0] != levelgroup[1])
				DungeonsByLevel[levelgroup[1]].insert((*itr)->ID);

		levelgroup[0] = LFG_LEVELGROUP_NONE;
		levelgroup[1] = LFG_LEVELGROUP_NONE;
	}
}

LfgMgr::~LfgMgr()
{

}

void LfgMgr::LoadRandomDungeonRewards()
{
	QueryResult* result = WorldDatabase.Query("SELECT * FROM lfd_rewards ORDER BY dungeonid");
	if(result == NULL)
	{
		Log.Error("LfgMgr", "Dungeon Rewards table(lfd_rewards) is empty");
		return;
	}

	do
	{
		Field* fields = result->Fetch();
		uint32 dungeonid = fields[0].GetUInt32();
		if(GetLFGReward(dungeonid))
			continue;

		uint32 questid[2], moneyreward[2], XPreward[2];
		questid[0] = questid[1] = moneyreward[0] = moneyreward[1] = XPreward[0] = XPreward[1] = 0;

		// First Reward
		questid[0] = fields[2].GetUInt32();
		moneyreward[0] = fields[3].GetUInt32();
		XPreward[0] = fields[4].GetUInt32();

		// Second reward
		questid[1] = fields[5].GetUInt32();
		moneyreward[1] = fields[6].GetUInt32();
		XPreward[1] = fields[7].GetUInt32();

		LfgReward* reward = new LfgReward(questid[0], moneyreward[0], XPreward[0], questid[1], moneyreward[1], XPreward[1]);
		DungeonRewards[dungeonid] = reward;
	}while(result->NextRow());
}

bool LfgMgr::AttemptLfgJoin(Player* pl, uint32 LfgDungeonId)
{
	return false;
}

uint32 LfgMgr::GetPlayerLevelGroup(uint32 level)
{
	if(level > 80)
		return LFG_LEVELGROUP_80_UP;
	else if(level == 80)
		return LFG_LEVELGROUP_80;
	else if(level >= 70)
		return LFG_LEVELGROUP_70_UP;
	else if(level >= 60)
		return LFG_LEVELGROUP_60_UP;
	else if(level >= 50)
		return LFG_LEVELGROUP_50_UP;
	else if(level >= 40)
		return LFG_LEVELGROUP_40_UP;
	else if(level >= 30)
		return LFG_LEVELGROUP_30_UP;
	else if(level >= 20)
		return LFG_LEVELGROUP_20_UP;
	else if(level >= 10)
		return LFG_LEVELGROUP_10_UP;

	return LFG_LEVELGROUP_NONE;
}

void LfgMgr::SetPlayerInLFGqueue(Player* pl,uint32 LfgDungeonId)
{

}

void LfgMgr::RemovePlayerFromLfgQueues(Player* pl)
{

}

void LfgMgr::RemovePlayerFromLfgQueue( Player* plr, uint32 LfgDungeonId )
{

}

void LfgMgr::UpdateLfgQueue(uint32 LfgDungeonId)
{

}

void LfgMgr::SendLfgList( Player* plr, uint32 Dungeon )
{

}

void LfgMgr::SetPlayerInLfmList(Player* pl, uint32 LfgDungeonId)
{

}

void LfgMgr::RemovePlayerFromLfmList(Player* pl, uint32 LfmDungeonId)
{

}
