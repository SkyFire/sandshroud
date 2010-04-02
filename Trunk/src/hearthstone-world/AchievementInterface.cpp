/*
* Aspire Hearthstone
* Copyright (C) 2008 - 2010 AspireDev <http://www.aspiredev.net/>
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

set<uint32> m_completedRealmFirstAchievements;

AchievementInterface::AchievementInterface(Player* plr)
{
	m_player = plr;
	m_achievementInspectPacket = NULL;
}

AchievementInterface::~AchievementInterface()
{
	m_player = NULLPLR;

	if( m_achivementDataMap.size() > 0 )
	{
		std::map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
		for(; itr != m_achivementDataMap.end(); ++itr)
		{
			delete [] itr->second->counter;
			delete itr->second;
		}
	}

	if( m_achievementInspectPacket )
		delete m_achievementInspectPacket;
}

void AchievementInterface::LoadFromDB( QueryResult * pResult )
{
	// don't allow GMs to complete achievements
	if( m_player->GetSession()->HasGMPermissions() )
	{
		CharacterDatabase.Execute("DELETE FROM achievements WHERE player = %u;", m_player->GetGUID());
		return;
	}

	if( !pResult ) // We have no achievements yet. :)
		return;

	do 
	{
		Field * fields = pResult->Fetch();
		uint32 achievementid = fields[1].GetUInt32();
		bool completed = (fields[3].GetUInt32() > 0);

		AchievementEntry * ae = dbcAchievement.LookupEntry( achievementid );
		AchievementData * ad = new AchievementData;
		memset(ad, 0, sizeof(AchievementData));

		ad->id = achievementid;
		ad->num_criterias = ae->AssociatedCriteriaCount;
		if(!completed) // save us memory! don't allocate our counters unless we're actually still working. :D
		{
			ad->counter = new uint32[ae->AssociatedCriteriaCount];
			memset(ad->counter, 0, sizeof(uint32)*ae->AssociatedCriteriaCount);
		}
		ad->completed = completed;
		ad->criteriaprogress = fields[2].GetString();
		ad->date = fields[3].GetUInt32();
		ad->groupid = fields[4].GetUInt64();

		if( ad->completed && string(ae->name).find("Realm First!") != string::npos ||
			(ae->flags & ACHIEVEMENT_FLAG_REALM_FIRST_OBTAIN) || (ae->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
			m_completedRealmFirstAchievements.insert( ae->ID );

		vector<string> Delim = StrSplit( ad->criteriaprogress, "," );
		for( uint32 i = 0; !completed && i < ae->AssociatedCriteriaCount; ++i)
		{
			if( i >= Delim.size() )
				continue;

			string posValue = Delim[i];
			if( !posValue.size() )
				continue;

			uint32 r = atoi(posValue.c_str());
			ad->counter[i] = r;

			//printf("Loaded achievement: %u, %s\n", ae->ID, ad->completed ? "completed" : "incomplete" );
		}

		m_achivementDataMap.insert( make_pair( achievementid, ad) );
	} while ( pResult->NextRow() );
}

void AchievementInterface::SaveToDB(QueryBuffer * buffer)
{
	// don't allow GMs to save achievements
	if( m_player->GetSession()->HasGMPermissions() )
		return;

	bool NewBuffer = false;
	if( !buffer )
	{
		buffer = new QueryBuffer;
		NewBuffer = true;
	}

	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		AchievementData* ad = itr->second;
		if( !ad->m_isDirty )
			continue;

		std::stringstream ss;
		ss << "REPLACE INTO achievements (player,achievementid,progress,completed,groupid) VALUES (";
		ss << m_player->GetLowGUID() << ",";
		ss << ad->id << ",";
		ss << "'";
		for(uint32 i = 0; i < ad->num_criterias; ++i)
		{
			ss << ad->counter[i] << ",";
		}
		ss << "',";
		ss << ad->date << ",";
		ss << ad->groupid << ")";
		buffer->AddQueryStr( ss.str().c_str() );

		ad->m_isDirty = false;
	}

	if( NewBuffer )
		CharacterDatabase.AddQueryBuffer( buffer );
}

WorldPacket* AchievementInterface::BuildAchievementData(bool forInspect)
{
	if(forInspect && m_achievementInspectPacket)
		return m_achievementInspectPacket;

	WorldPacket * data = new WorldPacket(forInspect ? SMSG_RESPOND_INSPECT_ACHIEVEMENTS : SMSG_ALL_ACHIEVEMENT_DATA, 400);
	if(forInspect)
		*data << m_player->GetNewGUID();

	std::map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		if( itr->second->completed )
		{
			*data << uint32(itr->second->id);
			*data << uint32( unixTimeToTimeBitfields(itr->second->date) );
		}
	}

	*data << int32(-1);
	itr = m_achivementDataMap.begin(); // Re-loop, luls
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		if( !itr->second->completed )
		{
			AchievementEntry * ae = dbcAchievement.LookupEntry( itr->second->id );
			// Loop over the associated criteria
			for(uint32 i = 0; i < ae->AssociatedCriteriaCount; ++i)
			{
				*data << uint32( ae->AssociatedCriteria->at(i) );
				uint32 counterVar = itr->second->counter[i];
				FastGUIDPack( *data, counterVar );
				*data << m_player->GetNewGUID();
				*data << uint32(0);
				*data << uint32( unixTimeToTimeBitfields( time(NULL) ) );
				*data << uint32(0);
				*data << uint32(0);
			}
		}
	}
	*data << int32(-1);

	if(forInspect)
		m_achievementInspectPacket = data;

	return data;
}

void AchievementInterface::GiveRewardsForAchievement(AchievementEntry * ae)
{
	AchievementReward * ar = AchievementRewardStorage.LookupEntry( ae->ID );
	if(!ar) return;

	// Reward: Item
	if( ar->ItemID )
	{
		m_player->GetGUID();
		Item* pItem = objmgr.CreateItem(ar->ItemID, m_player);
		m_player->GetItemInterface()->AddItemToFreeSlot(pItem);
		if( !m_player->GetItemInterface()->AddItemToFreeSlot(pItem) )
		{
			// Inventory full? Send it by mail.
			m_player->GetSession()->SendNotification("No free slots were found in your inventory, item has been mailed.");
			sMailSystem.DeliverMessage(MAILTYPE_NORMAL, m_player->GetGUID(), m_player->GetGUID(), "Achievement Reward", "Here is your reward.", 0, 0, ar->ItemID, 1, true);
			pItem->Destructor();
		}
	}

	// Define: Alliance Title
	if(m_player->GetTeam() == ALLIANCE)
	{
		if( ar->AllianceTitle )
		{
			m_player->SetKnownTitle(ar->AllianceTitle, true);
			// Set title to Alliance Reward, forced by Blizzard.
			m_player->SetUInt32Value( PLAYER_CHOSEN_TITLE, ar->AllianceTitle);
		}
	}

	// Define: Horde Title
	if(m_player->GetTeam() == HORDE)
	{
		if( ar->HordeTitle )
		{
			m_player->SetKnownTitle(ar->HordeTitle, true);
			// Set title to Horde Reward, forced by Blizzard.
			m_player->SetUInt32Value( PLAYER_CHOSEN_TITLE, ar->HordeTitle);
		}
	}
}

void AchievementInterface::EventAchievementEarned(AchievementData * pData)
{
	pData->completed = true;
	pData->date = (uint32)time(NULL);
	AchievementEntry * ae = dbcAchievement.LookupEntry(pData->id);
	GiveRewardsForAchievement(ae);
	WorldPacket * data = BuildAchievementEarned(pData);

	if( m_player->IsInWorld() )
		m_player->GetSession()->SendPacket( data );
	else
		m_player->CopyAndSendDelayedPacket( data );

	delete data;

	HandleAchievementCriteriaRequiresAchievement(pData->id);

	// Realm First Achievements
	if( string(ae->name).find("Realm First") != string::npos  ) // flags are wrong lol
	{
		m_completedRealmFirstAchievements.insert( ae->ID );

		// Send to my team
		WorldPacket data(SMSG_SERVER_FIRST_ACHIEVEMENT, 60);
		data << m_player->GetName();
		data << m_player->GetGUID();
		data << ae->ID;
		data << uint32(1);
		sWorld.SendFactionMessage(&data, m_player->GetTeam());

		// Send to the other team (no clickable link)
		WorldPacket data2(SMSG_SERVER_FIRST_ACHIEVEMENT, 60);
		data2 << m_player->GetName();
		data2 << m_player->GetGUID();
		data2 << ae->ID;
		data2 << uint32(0);
		sWorld.SendFactionMessage(&data2, m_player->GetTeam() ? 0 : 1);
	}
}

WorldPacket* AchievementInterface::BuildAchievementEarned(AchievementData * pData)
{
	pData->m_isDirty = true;
	WorldPacket * data = new WorldPacket(SMSG_ACHIEVEMENT_EARNED, 40);
	*data << m_player->GetNewGUID();
	*data << pData->id;
	*data << uint32( unixTimeToTimeBitfields(time(NULL)) );
	*data << uint32(0);

	if( m_achievementInspectPacket )
	{
		delete m_achievementInspectPacket;
		m_achievementInspectPacket = NULL;
	}

	return data;
}

AchievementData* AchievementInterface::CreateAchievementDataEntryForAchievement(AchievementEntry * ae)
{
	AchievementData* ad = new AchievementData;
	memset(ad, 0, sizeof(AchievementData));
	ad->id = ae->ID;
	ad->num_criterias = ae->AssociatedCriteriaCount;
	ad->counter = new uint32[ae->AssociatedCriteriaCount];
	memset(ad->counter, 0, sizeof(uint32)*ae->AssociatedCriteriaCount);
	m_achivementDataMap.insert( make_pair( ad->id, ad ) );
	return ad;
}

bool AchievementInterface::CanCompleteAchievement(AchievementData * ad)
{
	// don't allow GMs to complete achievements
	if( m_player->GetSession()->HasGMPermissions() )
		return false;

	if(!m_player) // o.O bastard.
		return false;

	if( ad->completed )
		return false;

	bool hasCompleted = false;
	AchievementEntry * ach = dbcAchievement.LookupEntry(ad->id);

	if( ach->categoryId == 1 || ach->flags & ACHIEVEMENT_FLAG_COUNTER ) // We cannot complete statistics
		return false;

	// realm first achievements
	if( m_completedRealmFirstAchievements.find(ad->id) != m_completedRealmFirstAchievements.end() )
		return false;

	/* Crow: Needs work :|
	This needs to be in a check that allows updates, but does not allow the
	player to actually get the achievement, also need to add it so that based on how
	many we need and how many we have, but that seems to be a small amount of achievements,
	maybe just do those by hand.
	*/
	for(uint32 i = 0; i < ad->num_criterias; ++i)
	{
		if(ad->counter[i] == 0)
			return false;
	}

	bool failedOne = false;
	for(uint32 i = 0; i < ad->num_criterias; ++i)
	{
		bool thisFail = false;
		AchievementCriteriaEntry * ace = dbcAchievementCriteria.LookupEntry(ach->AssociatedCriteria->at(i));
		uint32 ReqCount = ace->raw.field4 ? ace->raw.field4 : 1;

		if( ace->groupFlag & ACHIEVEMENT_CRITERIA_GROUP_NOT_IN_GROUP && m_player->GetGroup() )
			return false;

		if( ace->timeLimit && ace->timeLimit < ad->completionTimeLast )
			thisFail = true;

		if( ad->counter[i] < ReqCount )
			thisFail = true;

		if( ach->factionFlag == ACHIEVEMENT_FACTION_ALLIANCE && m_player->GetTeam() == 1 )
			thisFail = true;

		if( ach->factionFlag == ACHIEVEMENT_FACTION_HORDE && m_player->GetTeam() == 0 )
			thisFail = true;

		if( thisFail && ace->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG )
			failedOne = true;
		else if(thisFail)
			return false;

		if( !thisFail && ace->completionFlag & ACHIEVEMENT_CRITERIA_COMPLETE_ONE_FLAG )
			hasCompleted = true;
	}

	if( failedOne && !hasCompleted )
		return false;

	return true;
}

bool AchievementInterface::HandleBeforeChecks(AchievementData * ad)
{
	AchievementEntry * ach = dbcAchievement.LookupEntry(ad->id);

	// Do we have the achievement that comes before?
	if(ach->Previous_achievement)
		if(!HasAchievement(ach->Previous_achievement))
			return false;
	// Difficulty checks
	if(string(ach->description).find("25-player heroic mode") != string::npos)
		if(m_player->iRaidType < MODE_25PLAYER_HEROIC)
			return false;
	if(string(ach->description).find("10-player heroic mode") != string::npos)
		if(m_player->iRaidType < MODE_10PLAYER_HEROIC)
			return false;
	if(string(ach->description).find("25-player mode") != string::npos)
		if(m_player->iRaidType < MODE_25PLAYER_NORMAL)
			return false;
	if((string(ach->description).find("Heroic Difficulty") != string::npos) || ach->ID == 4526)
		if(m_player->iInstanceType < MODE_5PLAYER_HEROIC)
			return false;
	if(m_player->getLevel() < 10) // Blizzard says no.
		return false;
	if(m_player->GetSession()->HasGMPermissions())
		return false;
	return true;
}

bool AchievementInterface::HasAchievement(uint32 ID)
{
	AchievementData* acc = GetAchievementDataByAchievementID(ID);
	if(acc)
		return acc->completed;

	return false;
}

AchievementData* AchievementInterface::GetAchievementDataByAchievementID(uint32 ID)
{
	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.find( ID );
	if( itr != m_achivementDataMap.end() )
		return itr->second;
	else
	{
		AchievementEntry * ae = dbcAchievement.LookupEntryForced(ID);
		if(!ae)
			printf("No achievement for %u!\n", ID);
		return CreateAchievementDataEntryForAchievement(ae);
	}
}

void AchievementInterface::SendCriteriaUpdate(AchievementData * ad, uint32 idx)
{
	ad->m_isDirty = true;
	ad->date = (uint32)time(NULL);
	ad->groupid = m_player->GetGroupID();
	AchievementEntry * ae = dbcAchievement.LookupEntry(ad->id);
	if(!ae)
		return;

	WorldPacket data(SMSG_CRITERIA_UPDATE, 50);
	data << uint32(ae->AssociatedCriteria->at(idx));
	FastGUIDPack( data, (uint64)ad->counter[idx] );
	data << m_player->GetNewGUID();
	data << uint32(0);
	data << uint32(unixTimeToTimeBitfields(time(NULL)));
	data << uint32(0);
	data << uint32(0);

	if( !m_player->IsInWorld() )
		m_player->CopyAndSendDelayedPacket(&data);
	else
		m_player->GetSession()->SendPacket(&data);

	if( m_achievementInspectPacket )
	{
		delete m_achievementInspectPacket;
		m_achievementInspectPacket = NULL;
	}
}

void AchievementInterface::HandleAchievementCriteriaConditionDeath()
{
	// We died, so reset all our achievements that have ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH
	if( !m_achivementDataMap.size() )
		return;

	map<uint32,AchievementData*>::iterator itr = m_achivementDataMap.begin();
	for(; itr != m_achivementDataMap.end(); ++itr)
	{
		AchievementData * ad = itr->second;
		if(ad->completed) continue;
		AchievementEntry * ae = dbcAchievement.LookupEntry( ad->id );
		for(uint32 i = 0; i < ad->num_criterias; ++i)
		{
			uint32 CriteriaID = ae->AssociatedCriteria->at(i);
			AchievementCriteriaEntry * ace = dbcAchievementCriteria.LookupEntry( CriteriaID );
			if( ad->counter[i] && ace->raw.additionalRequirement1_type & ACHIEVEMENT_CRITERIA_CONDITION_NO_DEATH )
			{
				ad->counter[i] = 0;
				SendCriteriaUpdate(ad, i); break;
			}
		}
	}
}

void AchievementInterface::HandleAchievementCriteriaKillCreature(uint32 killedMonster)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqKill = ace->kill_creature.creatureID;
		uint32 ReqCount = ace->kill_creature.creatureCount;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		// Wrong monster, continue on, kids.
		if( ReqKill != killedMonster )
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i);
				break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaWinBattleground(uint32 bgMapId, uint32 scoreMargin, uint32 time, CBattleground* bg)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_WIN_BG );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqBGMap = ace->win_bg.bgMapID;
		uint32 ReqCount = ace->win_bg.winCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		// Wrong BG, continue on, kids.
		if( ReqBGMap != bgMapId )
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		ad->completionTimeLast = time;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				if( compareCriteria->raw.additionalRequirement1_type && scoreMargin < compareCriteria->raw.additionalRequirement1_type ) // BG Score Requirement.
					continue;
				// AV stuff :P
				if( bg->GetType() == BATTLEGROUND_ALTERAC_VALLEY )
				{
					AlteracValley* pAV(TO_ALTERACVALLEY(bg));
					if( pAchievementEntry->ID == 225 ||  pAchievementEntry->ID == 1164) // AV: Everything Counts
					{
						continue; // We do not support mines yet in AV
					}
					if( pAchievementEntry->ID == 220 ) // AV: Stormpike Perfection
					{
						bool failure = false;
						// We must control all Alliance nodes and Horde nodes (towers only)
						for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
						{
							if( pAV->GetNode(i)->IsGraveyard() )
								continue;
							if( pAV->GetNode(i)->GetState() != AV_NODE_STATE_ALLIANCE_CONTROLLED )
								failure = true;
						}
						if( failure ) continue;
					}
					if( pAchievementEntry->ID == 873 ) // AV: Frostwolf Perfection
					{
						bool failure = false;
						// We must control all Alliance nodes and Horde nodes (towers only)
						for(uint32 i = 0; i < AV_NUM_CONTROL_POINTS; ++i)
						{
							if( pAV->GetNode(i)->IsGraveyard() )
								continue;

							if( pAV->GetNode(i)->GetState() != AV_NODE_STATE_HORDE_CONTROLLED )
								failure = true;
						}
						if( failure ) continue;
					}
				}
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaRequiresAchievement(uint32 achievementId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqAchievement = ace->complete_achievement.linkedAchievement;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		if( ReqAchievement != achievementId )
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaLevelUp(uint32 level)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqLevel = ace->reach_level.level;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		if( level < ReqLevel )
			continue;
		// Realm first to 80 stuff has race and class requirements. Let the hacking begin.
		if( string(pAchievementEntry->name).find("Realm First!") != string::npos )
		{
			static const char* classNames[] = { "", "Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "", "Druid" };
			static const char* raceNames[] = { "", "Human", "Orc", "Dwarf", "Night Elf", "Forsaken", "Tauren", "Gnome", "Troll", "", "Blood Elf", "Draenei" };
			uint32 ReqClass = 0;
			uint32 ReqRace = 0;
			for(uint32 i = 0; i < 12; ++i)
			{
				if(strlen(classNames[i]) > 0 && string(pAchievementEntry->name).find(classNames[i]) != string::npos )
				{
					// We require this class
					ReqClass = i;
					break;
				}
			}
			for(uint32 i = 0; i < 12; ++i)
			{
				if(strlen(raceNames[i]) > 0 && string(pAchievementEntry->name).find(raceNames[i]) != string::npos )
				{
					// We require this race
					ReqRace = i;
					break;
				}
			}
			if( ReqClass && m_player->getClass() != ReqClass )
				continue;
			if( ReqRace && m_player->getRace() != ReqRace )
				continue;
		}
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = m_player->getLevel() > ReqLevel ? ReqLevel : m_player->getLevel();
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaOwnItem(uint32 itemId, uint32 stack)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqItemId = ace->own_item.itemID;
		uint32 ReqItemCount = ace->own_item.itemCount;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		if( itemId != ReqItemId )
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if( ad->completed )
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + stack;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
	HandleAchievementCriteriaLootItem(itemId, stack);
}

void AchievementInterface::HandleAchievementCriteriaLootItem(uint32 itemId, uint32 stack)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqItemId = ace->loot_item.itemID;
		uint32 ReqItemCount = ace->loot_item.itemCount;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		if( itemId != ReqItemId )
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + stack;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaQuestCount(uint32 questCount)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqQuestCount = ace->complete_quest_count.totalQuestCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = questCount;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKillClass(uint32 classId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqClass = ace->hk_class.classID;
		uint32 ReqCount = ace->hk_class.count;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqClass != classId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKillRace(uint32 raceId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_HK_RACE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqRace = ace->hk_race.raceID;
		uint32 ReqCount = ace->hk_race.count;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if( ReqRace != raceId )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaTalentResetCostTotal(uint32 cost)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_GOLD_SPENT_FOR_TALENTS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + cost;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaTalentResetCount()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_TALENT_RESETS );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaBuyBankSlot(bool retroactive)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqSlots = ace->buy_bank_slot.numberOfSlots; // We don't actually use this. :P

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				if( retroactive )
				{
					uint32 bytes = m_player->GetUInt32Value(PLAYER_BYTES_2);
					uint32 slots = (uint8)(bytes >> 16);
					ad->counter[i] = slots > ReqSlots ? ReqSlots : slots;
				}
				else
					ad->counter[i] = ad->counter[i] + 1;

				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaFlightPathsTaken()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_FLIGHT_PATHS_TAKEN );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaExploreArea(uint32 areaId, uint32 explorationFlags)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqFlags = ace->explore_area.areaReference;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		WorldMapOverlayEntry * wmoe = dbcWorldMapOverlay.LookupEntryForced( ReqFlags );
		if(!wmoe) continue;

		AreaTable * at = dbcArea.LookupEntryForced(wmoe->AreaTableID);
		if(!at || !(ReqFlags & at->explorationFlag) )
			continue;

		uint32 offset = at->explorationFlag / 32;
		offset += PLAYER_EXPLORED_ZONES_1;

		uint32 val = (uint32)(1 << (at->explorationFlag % 32));
		uint32 currFields = m_player->GetUInt32Value(offset);

		// Not explored /sadface
		if( !(currFields & val) )
			continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaHonorableKill()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqKills = ace->honorable_kill.killCount;

		if( m_player->m_killsLifetime < ReqKills )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

#define SCRIPTOK_FALSE { scriptOk = false; break; }
void AchievementInterface::HandleAchievementCriteriaDoEmote(uint32 emoteId, Unit* pTarget)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqEmote = ace->do_emote.emoteID;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		if(ReqEmote != emoteId)
			continue; // Wrong emote, newb!

		// Target information is not stored, so we'll have to do this one by one...
		// --unless the target's name is the description of the criteria! Bahahaha
		bool scriptOk = false;
		if( pTarget && pTarget->IsCreature() )
		{
			Creature* pCreature = TO_CREATURE(pTarget);
			if( !(!ace->name || strlen(ace->name) == 0 || !pCreature->GetCreatureInfo() || stricmp(pCreature->GetCreatureInfo()->Name, ace->name) != 0) )
			{
				scriptOk = true;
			}
		}

		string name = string(pAchievementEntry->name);
		if( name.find("Total") != string::npos )
		{
			// It's a statistic, like: "Total Times /Lol'd"
			scriptOk = true;
		}

		// Script individual ones here...
		if( ace->ID == 2379 ) // Make Love, Not Warcraft
		{
			if( !pTarget || !pTarget->IsPlayer() || !pTarget->isDead() || !isHostile(pTarget, m_player) )
				SCRIPTOK_FALSE

			scriptOk = true;
		}
		else if( ace->ID == 6261 ) // Winter Veil: A Frosty Shake 
		{
			if( m_player->GetZoneId() != 4395 ) // Not in Dalaran
				SCRIPTOK_FALSE
			
			if( !pTarget || !pTarget->HasAura(21848) ) // Not a Snowman
				SCRIPTOK_FALSE

			scriptOk = true;
		}

		if( !scriptOk ) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaCompleteQuestsInZone(uint32 zoneId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqZone = ace->complete_quests_in_zone.zoneID;
		uint32 ReqCount = ace->complete_quests_in_zone.questCount;

		if( ReqZone != zoneId )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry) continue;

		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;

		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}

		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaReachSkillLevel(uint32 skillId, uint32 skillLevel)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqSkill = ace->reach_skill_level.skillID;
		uint32 ReqLevel = ace->reach_skill_level.skillLevel;
		if( ReqSkill != skillId )
			continue;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = skillLevel;
				SendCriteriaUpdate(ad, i);
				break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaWinDuel()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		//uint32 ReqDuels = ace->win_duel.duelCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaLoseDuel()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_LOSE_DUEL );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		//uint32 ReqDuels = ace->win_duel.duelCount;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i] = ad->counter[i] + 1;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaKilledByCreature(uint32 killedMonster)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE );
	if( itr == objmgr.m_achievementCriteriaMap.end() )
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 ReqCreature = ace->killed_by_creature.creatureEntry;
		if( ReqCreature != killedMonster )
			continue;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaKilledByPlayer()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_PLAYER );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}

void AchievementInterface::HandleAchievementCriteriaDeath()
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DEATH );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
	HandleAchievementCriteriaDeathAtMap(m_player->GetMapId());
}

void AchievementInterface::HandleAchievementCriteriaDeathAtMap(uint32 mapId)
{
	AchievementCriteriaMap::iterator itr = objmgr.m_achievementCriteriaMap.find( ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP );
	if(itr == objmgr.m_achievementCriteriaMap.end())
		return;

	AchievementCriteriaSet * acs = itr->second;
	if( !acs ) // We have no achievements for this criteria :(
		return;

	AchievementCriteriaSet::iterator citr = acs->begin();
	for(; citr != acs->end(); ++citr)
	{
		AchievementCriteriaEntry * ace = (*citr);
		uint32 AchievementID = ace->referredAchievement;
		uint32 MapID = ace->death_at_map.mapID;

		if( mapId != MapID )
			continue;

		AchievementEntry * pAchievementEntry = dbcAchievement.LookupEntryForced(AchievementID);
		if(!pAchievementEntry)
			continue;
		AchievementCriteriaEntry * compareCriteria = NULL;
		AchievementData * ad = GetAchievementDataByAchievementID(AchievementID);
		if(ad->completed)
			continue;
		if(!HandleBeforeChecks(ad))
			continue;
		// Figure out our associative ID.
		for(uint32 i = 0; i < pAchievementEntry->AssociatedCriteriaCount; ++i)
		{
			compareCriteria = dbcAchievementCriteria.LookupEntry( pAchievementEntry->AssociatedCriteria->at(i) );			
			if( compareCriteria == ace )
			{
				ad->counter[i]++;
				SendCriteriaUpdate(ad, i); break;
			}
		}
		if( CanCompleteAchievement(ad) )
			EventAchievementEarned(ad);
	}
}
