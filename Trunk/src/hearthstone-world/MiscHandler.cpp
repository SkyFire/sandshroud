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

void WorldSession::HandleRepopRequestOpcode( WorldPacket & recv_data )
{
	DEBUG_LOG( "WORLD"," Recvd CMSG_REPOP_REQUEST Message" );
	uint8 popcheck;
	recv_data >> popcheck;
	// Todo: Death checks and whatnot.

	if(_player->m_CurrentTransporter)
		_player->m_CurrentTransporter->RemovePlayer(_player);

	if(_player->GetVehicle())
		_player->GetVehicle()->RemovePassenger(_player);

	  GetPlayer()->RepopRequestedPlayer();
}

void WorldSession::HandleAutostoreLootItemOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld() || !_player->GetLootGUID()) 
		return;
	uint32 itemid = 0;
	uint32 amt = 1;
	uint8 lootSlot = 0;
	uint8 error = 0;
	SlotResult slotresult;

	Item* add;

	if(_player->isCasting())
		_player->InterruptCurrentSpell();

	GameObject* pGO = NULLGOB;
	Object* pLootObj;

	// handle item loot
	uint64 guid = _player->GetLootGUID();
	if( GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_ITEM )
		pLootObj = _player->GetItemInterface()->GetItemByGUID(guid);
	else
		pLootObj = _player->GetMapMgr()->_GetObject(guid);

	if( pLootObj == NULL )
		return;

	if( pLootObj->GetTypeId() == TYPEID_GAMEOBJECT )
		pGO = TO_GAMEOBJECT(pLootObj);

	recv_data >> lootSlot;
	if (lootSlot >= pLootObj->m_loot.items.size())
		return;

	amt = pLootObj->m_loot.items.at(lootSlot).iItemsCount;
	if( pLootObj->m_loot.items.at(lootSlot).roll != NULL )
		return;

	if (!pLootObj->m_loot.items.at(lootSlot).ffa_loot)
	{
		if (!amt)//Test for party loot
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ALREADY_LOOTED);
			return;
		}
	}
	else
	{
		//make sure this player can still loot it in case of ffa_loot
		LooterSet::iterator itr = pLootObj->m_loot.items.at(lootSlot).has_looted.find(_player->GetLowGUID());

		if (pLootObj->m_loot.items.at(lootSlot).has_looted.end() != itr)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ALREADY_LOOTED);
			return;
		}
	}

	itemid = pLootObj->m_loot.items.at(lootSlot).item.itemproto->ItemId;
	ItemPrototype* it = pLootObj->m_loot.items.at(lootSlot).item.itemproto;

	if((error = _player->GetItemInterface()->CanReceiveItem(it, 1, NULL)))
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM, error);
		return;
	}

	if(pGO)
	{
		CALL_GO_SCRIPT_EVENT(pGO, OnLootTaken)(_player, it);
	}
	else if(pLootObj->IsCreature())
		CALL_SCRIPT_EVENT(pLootObj, OnLootTaken)(_player, it);

	sHookInterface.OnLoot(_player, pLootObj, 0, itemid);

	add = GetPlayer()->GetItemInterface()->FindItemLessMax(itemid, amt, false);
	if (!add)
	{
		slotresult = GetPlayer()->GetItemInterface()->FindFreeInventorySlot(it);
		if(!slotresult.Result)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM, INV_ERR_INVENTORY_FULL);
			return;
		}

		DEBUG_LOG("HandleAutostoreItem","AutoLootItem %u",itemid);
		Item* item = objmgr.CreateItem( itemid, GetPlayer());

		item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,amt);
		if(pLootObj->m_loot.items.at(lootSlot).iRandomProperty!=NULL)
		{
			item->SetRandomProperty(pLootObj->m_loot.items.at(lootSlot).iRandomProperty->ID);
			item->ApplyRandomProperties(false);
		}
		else if(pLootObj->m_loot.items.at(lootSlot).iRandomSuffix != NULL)
		{
			item->SetRandomSuffix(pLootObj->m_loot.items.at(lootSlot).iRandomSuffix->id);
			item->ApplyRandomProperties(false);
		}

		if( GetPlayer()->GetItemInterface()->SafeAddItem(item,slotresult.ContainerSlot, slotresult.Slot) )
		{
			sQuestMgr.OnPlayerItemPickup(GetPlayer(),item);
			_player->GetSession()->SendItemPushResult(item,false,true,true,true,slotresult.ContainerSlot,slotresult.Slot,1);
		}
		else
		{
			item->DeleteMe();
			item = NULLITEM;
		}
	}
	else
	{
		add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + amt);
		add->m_isDirty = true;

		sQuestMgr.OnPlayerItemPickup(GetPlayer(),add);
		_player->GetSession()->SendItemPushResult(add, false, true, true, false, _player->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()), 0xFFFFFFFF, amt);
	}

	//in case of ffa_loot update only the player who recives it.
	if (!pLootObj->m_loot.items.at(lootSlot).ffa_loot)
	{
		pLootObj->m_loot.items.at(lootSlot).iItemsCount = 0;

		// this gets sent to all looters
		WorldPacket data(1);
		data.SetOpcode(SMSG_LOOT_REMOVED);
		data << lootSlot;
		Player* plr;
		for(LooterSet::iterator itr = pLootObj->m_loot.looters.begin(); itr != pLootObj->m_loot.looters.end(); itr++)
		{
			plr = _player->GetMapMgr()->GetPlayer((*itr));
			if( plr != NULL )
				plr->GetSession()->SendPacket(&data);
		}
	}
	else
	{
		pLootObj->m_loot.items.at(lootSlot).has_looted.insert(_player->GetLowGUID());
		WorldPacket data(1);
		data.SetOpcode(SMSG_LOOT_REMOVED);
		data << lootSlot;
		_player->GetSession()->SendPacket(&data);
	}

	/* any left yet? (for fishing bobbers) */
	if(pGO && pGO->GetEntry() ==GO_FISHING_BOBBER)
	{
		for(vector<__LootItem>::iterator itr = pLootObj->m_loot.items.begin(); itr != pLootObj->m_loot.items.end(); itr++)
		{
			if( itr->iItemsCount > 0 )
				return;
		}

		pGO->ExpireAndDelete();
	}
}

void WorldSession::HandleLootMoneyOpcode( WorldPacket & recv_data )
{
	// sanity checks
	if(!_player->IsInWorld() || !_player->GetLootGUID()) return;

	// lookup the object we will be looting
	Object* pLootObj = NULL;
	if( GET_TYPE_FROM_GUID(_player->GetLootGUID()) == HIGHGUID_TYPE_ITEM )
		pLootObj = _player->GetItemInterface()->GetItemByGUID(_player->GetLootGUID());
	else
		pLootObj = _player->GetMapMgr()->_GetObject(_player->GetLootGUID());

	Player* plr;
	if( pLootObj == NULL )
		return;

	// is there any left? :o
	if( pLootObj->m_loot.gold == 0 )
		return;

	uint32 money = pLootObj->m_loot.gold;
	for(LooterSet::iterator itr = pLootObj->m_loot.looters.begin(); itr != pLootObj->m_loot.looters.end(); itr++)
	{
		if((plr = _player->GetMapMgr()->GetPlayer(*itr)))
			plr->GetSession()->OutPacket(SMSG_LOOT_CLEAR_MONEY);
	}

	if(!_player->InGroup())
	{
		if((_player->GetUInt32Value(PLAYER_FIELD_COINAGE) + money) >= PLAYER_MAX_GOLD)
			return;

		_player->ModUnsigned32Value( PLAYER_FIELD_COINAGE , money);
		pLootObj->m_loot.gold = 0;
	}
	else
	{
		//this code is wrong mustbe party not raid!
		Group* party = _player->GetGroup();
		pLootObj->m_loot.gold = 0;

		vector<Player*  > targets;
		targets.reserve(party->MemberCount());

		GroupMembersSet::iterator itr;
		SubGroup * sgrp;
		party->getLock().Acquire();
		for(uint32 i = 0; i < party->GetSubGroupCount(); i++)
		{
			sgrp = party->GetSubGroup(i);
			for(itr = sgrp->GetGroupMembersBegin(); itr != sgrp->GetGroupMembersEnd(); itr++)
			{
				if((*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetZoneId() == _player->GetZoneId() && _player->GetInstanceID() == (*itr)->m_loggedInPlayer->GetInstanceID())
					targets.push_back((*itr)->m_loggedInPlayer);
			}
		}
		party->getLock().Release();

		if(!targets.size())
			return;

		uint32 share = money / uint32(targets.size());

		uint8 databuf[50];
		StackPacket pkt(SMSG_LOOT_MONEY_NOTIFY, databuf, 50);
		pkt << share;

		for(vector<Player*>::iterator itr = targets.begin(); itr != targets.end(); itr++)
		{
			if(((*itr)->GetUInt32Value(PLAYER_FIELD_COINAGE) + share) >= PLAYER_MAX_GOLD)
				continue;

			(*itr)->ModUnsigned32Value(PLAYER_FIELD_COINAGE, share);
			(*itr)->GetSession()->SendPacket(&pkt);
		}
	}
}

void WorldSession::HandleLootOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;

	uint64 guid;
	recv_data >> guid;

	if(_player->GetMapMgr()->GetCreature(guid) && _player->GetMapMgr()->GetCreature(guid)->IsVehicle())
		return;

	if(_player->isCasting())
		_player->InterruptCurrentSpell();

	if(_player->InGroup() && !_player->m_bg)
	{
		Group * party = _player->GetGroup();
		if(party)
		{
			if(party->GetMethod() == PARTY_LOOT_MASTER)
			{
				uint8 databuf[1000];
				StackPacket data(SMSG_LOOT_MASTER_LIST, databuf, 1000);

				data << (uint8)party->MemberCount();
				uint32 real_count = 0;
				SubGroup *s;
				GroupMembersSet::iterator itr;
				party->Lock();
				for(uint32 i = 0; i < party->GetSubGroupCount(); i++)
				{
					s = party->GetSubGroup(i);
					for(itr = s->GetGroupMembersBegin(); itr != s->GetGroupMembersEnd(); itr++)
					{
						if((*itr)->m_loggedInPlayer && _player->GetZoneId() == (*itr)->m_loggedInPlayer->GetZoneId())
						{
							data << (*itr)->m_loggedInPlayer->GetGUID();
							++real_count;
						}
					}
				}
				party->Unlock();
				data.GetBufferPointer()[0] = real_count;

				party->SendPacketToAll(&data);
			}
		}
	}

	_player->SendLoot(guid, _player->GetMapId(), LOOT_CORPSE);
}


void WorldSession::HandleLootReleaseOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	uint64 guid;
	recv_data >> guid;

	WorldPacket data(SMSG_LOOT_RELEASE_RESPONSE, 9);
	data << guid << uint8( 1 );
	SendPacket( &data );

	_player->SetLootGUID(0);
	_player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOOTING);
	_player->m_currentLoot = 0;

	// special case
	if( GET_TYPE_FROM_GUID( guid ) == HIGHGUID_TYPE_GAMEOBJECT )
	{
		GameObject* pGO = _player->GetMapMgr()->GetGameObject( GET_LOWGUID_PART(guid) );
		if( pGO == NULL )
			return;

		pGO->m_loot.looters.erase(_player->GetLowGUID());
		switch( pGO->GetType())
		{
		case GAMEOBJECT_TYPE_FISHINGNODE:
			{
				if(pGO->IsInWorld())
				{
					pGO->RemoveFromWorld(true);
				}
				delete pGO;
				pGO = NULLGOB;
			}break;
		case GAMEOBJECT_TYPE_CHEST:
			{
				pGO->m_loot.looters.erase( _player->GetLowGUID() );
				//check for locktypes

				Lock* pLock = dbcLock.LookupEntry( pGO->GetInfo()->SpellFocus );
				if( pLock )
				{
					for( uint32 i=0; i < 8; i++ )
					{
						if( pLock->locktype[i] )
						{
							if( pLock->locktype[i] == 2 ) //locktype;
							{
								//herbalism and mining;
								if( pLock->lockmisc[i] == LOCKTYPE_MINING || pLock->lockmisc[i] == LOCKTYPE_HERBALISM )
								{
									//we still have loot inside.
									if( pGO->m_loot.HasItems(_player) )
									{
										pGO->SetState(1);
										return;
									}

									if( pGO->CanMine() )
									{
										pGO->ClearLoot();
										pGO->UseMine();
										return;
									}
									else
									{
										pGO->CalcMineRemaining( true );
										pGO->Despawn( 0, 600000 + ( RandomUInt( 180000 ) ) );
										return;
									}
								}
								else //other type of locks that i dont bother to split atm ;P
								{
									if(pGO->m_loot.HasItems(_player))
									{
										pGO->SetState(1);
										return;
									}

									pGO->CalcMineRemaining(true);

									//Don't interfere with scripted GO's
									if(!sEventMgr.HasEvent(pGO, EVENT_GMSCRIPT_EVENT))
									{
										uint32 DespawnTime = 0;
										if(sQuestMgr.GetGameObjectLootQuest(pGO->GetEntry()))
											DespawnTime = 120000;	   // 5 min for quest GO,
										else
											DespawnTime = 900000;	   // 15 for else

										pGO->Despawn(0, DespawnTime);
									}
									return;
								}
							}
						}
					}
				}
				else
				{
					if( pGO->m_loot.HasItems(_player) )
					{
						pGO->SetState(1);
						return;
					}
					uint32 DespawnTime = 0;
					if(sQuestMgr.GetGameObjectLootQuest(pGO->GetEntry()))
						DespawnTime = 120000;	   // 5 min for quest GO,
					else
					{
						DespawnTime = 900000;	   // 15 for else
					}
					pGO->Despawn(0, DespawnTime);

				}
			}
		}
		return;
	}
	else if( GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_ITEM )
	{
		// if we have no items left, destroy the item.
		Item* pItem = _player->GetItemInterface()->GetItemByGUID(guid);
		if( pItem != NULL )
		{
			if( !pItem->m_loot.HasItems(_player) )
				_player->GetItemInterface()->SafeFullRemoveItemByGuid(guid);
		}

		return;
	}
	else if( GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_CREATURE )
	{
		Unit* pLootTarget = _player->GetMapMgr()->GetUnit(guid);
		if( pLootTarget != NULL )
		{
			pLootTarget->m_loot.looters.erase(_player->GetLowGUID());
			if( !pLootTarget->m_loot.HasLoot(_player) )
			{
				TO_CREATURE(pLootTarget)->UpdateLootAnimation(_player);

				// skinning
				if(!TO_CREATURE(pLootTarget)->IsPet() && !TO_CREATURE(pLootTarget)->IsSummon()
					&& lootmgr.IsSkinnable( pLootTarget->GetEntry()))
				{
					pLootTarget->BuildFieldUpdatePacket( _player, UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE );
				}
			}
		}
	}
	else if( GET_TYPE_FROM_GUID(guid) == HIGHGUID_TYPE_CORPSE )
	{
		Corpse* pCorpse = objmgr.GetCorpse((uint32)guid);
		if( pCorpse != NULL && !pCorpse->m_loot.HasLoot(_player) )
			pCorpse->Despawn();
	}
}

void WorldSession::HandleWhoOpcode( WorldPacket & recv_data )
{
	uint32 min_level;
	uint32 max_level;
	uint32 class_mask;
	uint32 race_mask;
	uint32 zone_count;
	uint32 * zones = 0;
	uint32 name_count;
	string * names = 0;
	string chatname;
	string unkstr;
	bool cname;
	uint32 i;

	if( ((uint32)UNIXTIME - m_lastWhoTime) < 10 && !GetPlayer()->bGMTagOn )
		return;

	m_lastWhoTime = (uint32)UNIXTIME;
	recv_data >> min_level >> max_level;
	recv_data >> chatname >> unkstr >> race_mask >> class_mask;
	recv_data >> zone_count;

	if(zone_count > 0 && zone_count < 10)
	{
		zones = new uint32[zone_count];

		for(i = 0; i < zone_count; i++)
			recv_data >> zones[i];
	}
	else
	{
		zone_count = 0;
	}

	recv_data >> name_count;
	if(name_count > 0 && name_count < 10)
	{
		names = new string[name_count];

		for(i = 0; i < name_count; i++)
			recv_data >> names[i];
	}
	else
	{
		name_count = 0;
	}

	if(chatname.length() > 0)
		cname = true;
	else
		cname = false;

	DEBUG_LOG( "WORLD"," Recvd CMSG_WHO Message with %u zones and %u names", zone_count, name_count );

	bool gm = false;
	uint32 team = _player->GetTeam();
	if(HasGMPermissions())
		gm = true;

	uint32 sent_count = 0;
	uint32 total_count = 0;

	PlayerStorageMap::const_iterator itr,iend;
	Player* plr;
	uint32 lvl;
	bool add;
	WorldPacket data;
	data.SetOpcode(SMSG_WHO);
	data << uint64(0);

	objmgr._playerslock.AcquireReadLock();
	iend=objmgr._players.end();
	itr=objmgr._players.begin();
	while(itr !=iend && sent_count < 50)
	{
		plr = itr->second;
		++itr;

		if(!plr->GetSession() || !plr->IsInWorld())
			continue;

		if(!sWorld.show_gm_in_who_list && !HasGMPermissions())
		{
			if(plr->GetSession()->HasGMPermissions())
				continue;
		}

		// Team check
		if(!sWorld.cross_faction_world)
			if(!gm && plr->GetTeam() != team && !plr->GetSession()->HasGMPermissions())
				continue;

		++total_count;

		// Add by default, if we don't have any checks
		add = true;

		// Chat name
		if(cname && chatname != *plr->GetNameString())
			continue;

		// Level check
		lvl = plr->m_uint32Values[UNIT_FIELD_LEVEL];
		if(min_level && max_level)
		{
			// skip players outside of level range
			if(lvl < min_level || lvl > max_level)
				continue;
		}

		// Zone id compare
		if(zone_count)
		{
			// people that fail the zone check don't get added
			add = false;
			for(i = 0; i < zone_count; i++)
			{
				if(zones[i] == plr->GetZoneId())
				{
					add = true;
					break;
				}
			}
		}

		if(!(class_mask & plr->getClassMask()) || !(race_mask & plr->getRaceMask()))
			add = false;

		// skip players that fail zone check
		if(!add)
			continue;

		// name check
		if(name_count)
		{
			// people that fail name check don't get added
			add = false;
			for(i = 0; i < name_count; i++)
			{
				if(!strnicmp(names[i].c_str(), plr->GetName(), names[i].length()))
				{
					add = true;
					break;
				}
			}
		}

		if(!add)
			continue;

		// if we're here, it means we've passed all testing
		// so add the names :)
		data << plr->GetName();
		if(plr->m_playerInfo->guild)
			data << plr->m_playerInfo->guild->GetGuildName();
		else
			data << uint8(0);	   // Guild name

		data << plr->m_uint32Values[UNIT_FIELD_LEVEL];
		data << uint32(plr->getClass());
		data << uint32(plr->getRace());
		data << plr->getGender();
		data << uint32(plr->GetZoneId());
		++sent_count;
	}
	objmgr._playerslock.ReleaseReadLock();
	data.wpos(0);
	data << sent_count;
	data << sent_count;

	SendPacket(&data);

	// free up used memory
	if(zones)
		delete [] zones;
	if(names)
		delete [] names;
}

void WorldSession::HandleLogoutRequestOpcode( WorldPacket & recv_data )
{
	Player* pPlayer = GetPlayer();
	WorldPacket data(SMSG_LOGOUT_RESPONSE, 9);

	DEBUG_LOG( "WORLD"," Recvd CMSG_LOGOUT_REQUEST Message" );

	if(pPlayer)
	{
		sHookInterface.OnLogoutRequest(pPlayer);
		if(pPlayer->m_isResting ||	  // We are resting so log out instantly
			pPlayer->GetTaxiState() ||  // or we are on a taxi
			HasGMPermissions())		   // or we are a gm
		{
			SetLogoutTimer(1);
			return;
		}

		if(pPlayer->DuelingWith != NULL || pPlayer->CombatStatus.IsInCombat())
		{
			//can't quit still dueling or attacking
			data << uint32(0x1); //Filler
			data << uint8(0); //Logout accepted
			SendPacket( &data );
			return;
		}

		data << uint32(0); //Filler
		data << uint8(0); //Logout accepted
		SendPacket( &data );

		//stop player from moving
		pPlayer->SetMovement(MOVE_ROOT,1);

		// Set the "player locked" flag, to prevent movement
		pPlayer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

		//make player sit
		pPlayer->SetStandState(STANDSTATE_SIT);
		SetLogoutTimer(PLAYER_LOGOUT_DELAY);
	}
}

void WorldSession::HandlePlayerLogoutOpcode( WorldPacket & recv_data )
{
	DEBUG_LOG( "WORLD"," Recvd CMSG_PLAYER_LOGOUT Message" );
	if(!HasGMPermissions())
	{
		// send "You do not have permission to use this"
		SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
	} 
	else
		LogoutPlayer(true);
}

void WorldSession::HandleLogoutCancelOpcode( WorldPacket & recv_data )
{

	DEBUG_LOG( "WORLD"," Recvd CMSG_LOGOUT_CANCEL Message" );

	Player* pPlayer = GetPlayer();
	if(!pPlayer || !_logoutTime)
		return;

	//Cancel logout Timer
	SetLogoutTimer(0);

	//tell client about cancel
	OutPacket(SMSG_LOGOUT_CANCEL_ACK);

	//unroot player
	pPlayer->SetMovement(MOVE_UNROOT,5);

	// Remove the "player locked" flag, to allow movement
	pPlayer->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER);

	//make player stand
	pPlayer->SetStandState(STANDSTATE_STAND);

	DEBUG_LOG( "WORLD"," sent SMSG_LOGOUT_CANCEL_ACK Message" );
}

void WorldSession::HandleZoneUpdateOpcode( WorldPacket & recv_data )
{
	uint32 newZone;
	WPAssert(GetPlayer());

	recv_data >> newZone;

	if (GetPlayer()->GetZoneId() == newZone)
		return;

	sWeatherMgr.SendWeather(GetPlayer());
	_player->ZoneUpdate(newZone);

	//clear buyback
	_player->GetItemInterface()->EmptyBuyBack();
}

void WorldSession::HandleSetTargetOpcode( WorldPacket & recv_data )
{
	// obselete?
}

void WorldSession::HandleSetSelectionOpcode( WorldPacket & recv_data )
{
	uint64 guid;
	recv_data >> guid;

	_player->SetUInt64Value(UNIT_FIELD_TARGET, guid);
	_player->SetSelection(guid);

	if(_player->m_comboPoints)
		_player->UpdateComboPoints();
}

void WorldSession::HandleStandStateChangeOpcode( WorldPacket & recv_data )
{
	uint32 animstate;
	recv_data >> animstate;

	_player->SetStandState(int8(animstate));
}

void WorldSession::HandleBugOpcode( WorldPacket & recv_data )
{
	uint32 suggestion, contentlen;
	std::string content;
	uint32 typelen;
	std::string type;

	recv_data >> suggestion >> contentlen >> content >> typelen >> type;

	if( suggestion == 0 )
		DEBUG_LOG( "WORLD"," Received CMSG_BUG [Bug Report]" );
	else
		DEBUG_LOG( "WORLD"," Received CMSG_BUG [Suggestion]" );

	OUT_DEBUG( type.c_str( ) );
	OUT_DEBUG( content.c_str( ) );
}

void WorldSession::HandleCorpseReclaimOpcode(WorldPacket& recv_data)
{
	if(_player->isAlive())
		return;

	OUT_DEBUG("WORLD: Received CMSG_RECLAIM_CORPSE");

	uint64 guid;
	recv_data >> guid;

	Corpse* pCorpse = objmgr.GetCorpse( guid );

	if( pCorpse == NULL )
	{
		QueryResult* result = CharacterDatabase.Query("SELECT * FROM corpses WHERE guid = %u AND mapId = %u",
			guid, _player->GetMapId());
		if(result == NULL)
			return;
		delete result;
		pCorpse = _player->CreateCorpse();
	}

	// Check that we're reviving from a corpse, and that corpse is associated with us.
	if( pCorpse->GetUInt32Value( CORPSE_FIELD_OWNER ) != _player->GetLowGUID() && pCorpse->GetUInt32Value( CORPSE_FIELD_FLAGS ) == 5 )
	{
		WorldPacket data( SMSG_RESURRECT_FAILED, 4 );
		data << uint32(1); // this is a real guess!
		SendPacket(&data);
		return;
	}

	// Check we are actually in range of our corpse
	if ( pCorpse->GetDistance2dSq( _player ) > CORPSE_MINIMUM_RECLAIM_RADIUS_SQ || GetPlayer()->PreventRes)
	{
		WorldPacket data( SMSG_RESURRECT_FAILED, 4 );
		data << uint32(1);
		SendPacket(&data);
		return;
	}

	// Check death clock before resurrect they must wait for release to complete
	if( pCorpse->GetDeathClock() + (_player->ReclaimCount*15) > time( NULL ) )
	{
		WorldPacket data( SMSG_RESURRECT_FAILED, 4 );
		data << uint32(1);
		SendPacket(&data);
		return;
	}

	GetPlayer()->ResurrectPlayer();
	GetPlayer()->SetUInt32Value(UNIT_FIELD_HEALTH, GetPlayer()->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2 );
}

void WorldSession::HandleResurrectResponseOpcode(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;
	OUT_DEBUG("WORLD: Received CMSG_RESURRECT_RESPONSE");

	if(GetPlayer()->isAlive() || GetPlayer()->PreventRes)
		return;

	uint64 guid;
	uint8 status;
	recv_data >> guid;
	recv_data >> status;

	// check
	if( guid != 0 && _player->resurrector != (uint32)guid )
	{
		// error
		return;
	}

	// need to check guid
	Player* pl = _player->GetMapMgr()->GetPlayer((uint32)guid);
	if(!pl)
		pl = objmgr.GetPlayer((uint32)guid);

	// reset resurrector
	_player->resurrector = 0;

	if(pl == 0 || status != 1)
	{
		_player->m_resurrectHealth = 0;
		_player->m_resurrectMana = 0;
		_player->resurrector = 0;
		return;
	}

	_player->SetMovement(MOVE_UNROOT, 1);
	_player->ResurrectPlayer(pl);
	_player->m_resurrectHealth = 0;
	_player->m_resurrectMana = 0;

}

void WorldSession::HandleUpdateAccountData(WorldPacket& recv_data)
{
	//OUT_DEBUG("WORLD: Received CMSG_UPDATE_ACCOUNT_DATA");
	if(!sWorld.m_useAccountData)
	{
		SKIP_READ_PACKET(recv_data); // Spam cleanup.
		return;
	}

	uint32 uiID;
	recv_data >> uiID;

	if(uiID > 8)
	{
		// Shit..
		sLog.outString("WARNING: Accountdata > 8 (%d) was requested to be updated by %s of account %d!", uiID, GetPlayer()->GetName(), this->GetAccountId());
		return;
	}

	uint32 _time;
	recv_data >> _time;

	uint32 uiDecompressedSize;
	recv_data >> uiDecompressedSize;
	uLongf uid = uiDecompressedSize;

	// client wants to 'erase' current entries
	if(uiDecompressedSize == 0)
	{
		SKIP_READ_PACKET(recv_data);
		SetAccountData(uiID, NULL, false,0);
		return;
	}

	if(uiDecompressedSize > 100000)
	{
		SKIP_READ_PACKET(recv_data); // Spam cleanup.
		Disconnect();
		return;
	}

	if(uiDecompressedSize >= 65534)
	{
		SKIP_READ_PACKET(recv_data); // Spam cleanup.
		// BLOB fields can't handle any more than this.
		return;
	}

	size_t ReceivedPackedSize = recv_data.size() - 12;
	char* data = new char[uiDecompressedSize+1];
	memset(data, 0, uiDecompressedSize+1);	/* fix umr here */

	if(uiDecompressedSize > ReceivedPackedSize) // if packed is compressed
	{
		int32 ZlibResult;

		ZlibResult = uncompress((uint8*)data, &uid, recv_data.contents() + 12, (uLong)ReceivedPackedSize);

		switch (ZlibResult)
		{
		case Z_OK:				  //0 no error decompression is OK
			SetAccountData(uiID, data, false, uiDecompressedSize);
			OUT_DEBUG("WORLD: Successfully decompressed account data %d for %s, and updated storage array.", uiID, GetPlayer() ? GetPlayer()->GetName() : GetAccountNameS());
			break;

		case Z_ERRNO:				//-1
		case Z_STREAM_ERROR:		//-2
		case Z_DATA_ERROR:			//-3
		case Z_MEM_ERROR:			//-4
		case Z_BUF_ERROR:			//-5
		case Z_VERSION_ERROR:		//-6
		{
			delete [] data;
			sLog.outString("WORLD WARNING: Decompression of account data %d for %s FAILED.", uiID, GetPlayer() ? GetPlayer()->GetName() : GetAccountNameS());
			break;
		}

		default:
			delete [] data;
			sLog.outString("WORLD WARNING: Decompression gave a unknown error: %x, of account data %d for %s FAILED.", ZlibResult, uiID, GetPlayer() ? GetPlayer()->GetName() : GetAccountNameS());
			break;
		}
	}
	else
	{
		memcpy(data, recv_data.contents() + 12, uiDecompressedSize);
		SetAccountData(uiID, data, false, uiDecompressedSize);
	}SKIP_READ_PACKET(recv_data); // Spam cleanup for packet size checker... Because who cares about this dataz
}

void WorldSession::HandleRequestAccountData(WorldPacket& recv_data)
{
	//OUT_DEBUG("WORLD: Received CMSG_REQUEST_ACCOUNT_DATA");
	if(!sWorld.m_useAccountData)
	{
		SKIP_READ_PACKET(recv_data); // Spam cleanup for packet size checker.
		return;
	}

	uint32 id;
	recv_data >> id;

	if(id > 8)
	{
		// Shit..
		sLog.outString("WARNING: Accountdata > 8 (%d) was requested by %s of account %d!", id, GetPlayer() ? GetPlayer()->GetName() : "UNKNOWN", this->GetAccountId());
		return;
	}

	AccountDataEntry* res = GetAccountData(id);
	uLongf destSize = compressBound(res->sz);
	ByteBuffer bbuff;
	bbuff.resize(destSize);

	if(res->sz && compress(const_cast<uint8*>(bbuff.contents()), &destSize, (uint8*)res->data, res->sz) != Z_OK)
	{
		OUT_DEBUG("Error while compressing ACCOUNT_DATA");
		SKIP_READ_PACKET(recv_data);
		return;
	}

	WorldPacket data;
	data.SetOpcode(SMSG_UPDATE_ACCOUNT_DATA);
	data << uint64(_player ? _player->GetGUID() : 0);
	data << id;
	// if red does not exists if ID == 7 and if there is no data send 0
	if(!res || !res->data) // if error, send a NOTHING packet
	{
		data << (uint32)0;
		data << (uint32)0;
	}
	else
	{
		data << uint32(res->Time);
		data << res->sz;
	}
	data.append(bbuff);
	SendPacket(&data);
}

void WorldSession::HandleSetActionButtonOpcode(WorldPacket& recv_data)
{
	DEBUG_LOG( "WORLD"," Received CMSG_SET_ACTION_BUTTON" );
	uint8 button, misc, type;
	uint16 action;
	recv_data >> button >> action >> misc >> type;
	OUT_DEBUG( "BUTTON: %u ACTION: %u TYPE: %u MISC: %u", button, action, type, misc );
	if(action==0)
	{
		OUT_DEBUG( "MISC: Remove action from button %u", button );
		GetPlayer()->setAction(button, 0, 0, 0);
	}
	else
	{
		if(button >= 0x00FFFFFF+1)
			return;

		if(type == 64 || type == 65)
		{
			OUT_DEBUG( "MISC: Added Macro %u into button %u", action, button );
			GetPlayer()->setAction(button,action,type,misc);
		}
		else if(type == 128)
		{
			OUT_DEBUG( "MISC: Added Item %u into button %u", action, button );
			GetPlayer()->setAction(button,action,type,misc);
		}
		else if(type == 0)
		{
			OUT_DEBUG( "MISC: Added Spell %u into button %u", action, button );
			GetPlayer()->setAction(button,action,type,misc);
		}
	}

#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Actions();
#endif
}

void WorldSession::HandleSetWatchedFactionIndexOpcode(WorldPacket &recvPacket)
{
	uint32 factionid;
	recvPacket >> factionid;
	GetPlayer()->SetUInt32Value(PLAYER_FIELD_WATCHED_FACTION_INDEX, factionid);

#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Misc();
#endif
}

void WorldSession::HandleTogglePVPOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	uint32 areaId = _player->GetAreaID();
	AreaTable * at = dbcArea.LookupEntryForced(areaId);
	if(sWorld.FunServerMall != -1 && areaId == (uint32)sWorld.FunServerMall)
	{
		if(at != NULL)
			sChatHandler.ColorSystemMessage(this, MSG_COLOR_WHITE, "You cannot flag for PvP while in the area: %s", at->name);
		else
			sChatHandler.ColorSystemMessage(this, MSG_COLOR_WHITE, "You cannot do that here");

		if( _player->IsFFAPvPFlagged() || _player->IsPvPFlagged() )
		{
			if( _player->IsPvPFlagged() )
				_player->RemovePvPFlag();
			if( _player->IsFFAPvPFlagged() )
				_player->RemoveFFAPvPFlag();
		}
		return;
	}

	if( at == NULL)
		_player->PvPToggle(); // Crow: Should be a delayed pvp flag
	else if(!(at->category == AREAC_SANCTUARY || at->AreaFlags & AREA_SANCTUARY))
		_player->PvPToggle();
}

void WorldSession::HandleAmmoSetOpcode(WorldPacket & recv_data)
{
#ifndef CATACLYSM
	uint32 ammoId;
	recv_data >> ammoId;

	if(!ammoId)
		return;

	ItemPrototype * xproto=ItemPrototypeStorage.LookupEntry(ammoId);
	if(!xproto)
		return;

	if(xproto->Class != ITEM_CLASS_PROJECTILE || GetPlayer()->GetItemInterface()->GetItemCount(ammoId) == 0)
	{
		sWorld.LogCheater(GetPlayer()->GetSession(), "Definately cheating. tried to add %u as ammo.", ammoId);
		GetPlayer()->GetSession()->Disconnect();
		return;
	}

	if(xproto->RequiredLevel)
	{
		if(GetPlayer()->getLevel() < xproto->RequiredLevel)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			_player->SetUInt32Value(PLAYER_AMMO_ID, 0);
			_player->CalcDamage();
			return;
		}
	}
	if(xproto->RequiredSkill)
	{
		if(!GetPlayer()->_HasSkillLine(xproto->RequiredSkill))
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ITEM_RANK_NOT_ENOUGH);
			_player->SetUInt32Value(PLAYER_AMMO_ID, 0);
			_player->CalcDamage();
			return;
		}

		if(xproto->RequiredSkillRank)
		{
			if(_player->_GetSkillLineCurrent(xproto->RequiredSkill, false) < xproto->RequiredSkillRank)
			{
				GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ITEM_RANK_NOT_ENOUGH);
				_player->SetUInt32Value(PLAYER_AMMO_ID, 0);
				_player->CalcDamage();
				return;
			}
		}
	}
	_player->SetUInt32Value(PLAYER_AMMO_ID, ammoId);
	_player->CalcDamage();

#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_Misc();
#endif
#endif
}

#define OPEN_CHEST 11437
void WorldSession::HandleGameObjectUse(WorldPacket & recv_data)
{
 	CHECK_INWORLD_RETURN;

	uint64 guid;
	recv_data >> guid;
	SpellCastTargets targets;
	Spell* spell = NULLSPELL;
	SpellEntry *spellInfo = NULL;
	OUT_DEBUG("WORLD: CMSG_GAMEOBJ_USE: [GUID %d]", guid);

	GameObject* obj = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
	if (!obj)
		return;

	GameObjectInfo *goinfo= obj->GetInfo();
	if (!goinfo)
		return;

	Player* plyr = GetPlayer();

	CALL_GO_SCRIPT_EVENT(obj, OnActivate)(_player);
	CALL_INSTANCE_SCRIPT_EVENT( _player->GetMapMgr(), OnGameObjectActivate )( obj, _player );

	uint32 type = obj->GetByte(GAMEOBJECT_BYTES_1, GAMEOBJECT_BYTES_TYPE_ID);
	switch (type)
	{
		case GAMEOBJECT_TYPE_CHAIR:
		{
			/// if players are mounted they are not able to sit on a chair
			if( plyr->IsMounted() )
				plyr->RemoveAura( plyr->m_MountSpellId );

			plyr->SafeTeleport( plyr->GetMapId(), plyr->GetInstanceID(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation() );
			plyr->SetStandState(STANDSTATE_SIT_MEDIUM_CHAIR);
		}break;
		case GAMEOBJECT_TYPE_CHEST://cast da spell
		{
			spellInfo = dbcSpell.LookupEntry( OPEN_CHEST );
			spell = (new Spell(plyr, spellInfo, true, NULLAURA));
			_player->m_currentSpell = spell;
			targets.m_unitTarget = obj->GetGUID();
			spell->prepare(&targets);
		}break;
		case GAMEOBJECT_TYPE_FISHINGNODE:
		{
			obj->UseFishingNode(plyr);
		}break;
		case GAMEOBJECT_TYPE_DOOR:
		{
			// door
			if((obj->GetState() == 1) && (obj->GetFlags() == 33))
				obj->EventCloseDoor();
			else
			{
				obj->SetFlags(33);
				obj->SetState(0);
				sEventMgr.AddEvent(obj,&GameObject::EventCloseDoor,EVENT_GAMEOBJECT_DOOR_CLOSE,20000,1,0);
			}
		}break;
		case GAMEOBJECT_TYPE_FLAGSTAND:
		{
			// battleground/warsong gulch flag
			if(plyr->m_bg)
			{
				if( plyr->m_stealth )
					plyr->RemoveAura( plyr->m_stealth );

				if( plyr->m_MountSpellId )
					plyr->RemoveAura( plyr->m_MountSpellId );

				if( plyr->GetVehicle() )
					plyr->GetVehicle()->RemovePassenger( plyr );

				if(!plyr->m_bgFlagIneligible)
					plyr->m_bg->HookFlagStand(plyr, obj);
			}
		}break;
		case GAMEOBJECT_TYPE_FLAGDROP:
		{
			// Dropped flag
			if(plyr->m_bg)
			{
				if( plyr->m_stealth )
					plyr->RemoveAura( plyr->m_stealth );

				if( plyr->m_MountSpellId )
					plyr->RemoveAura( plyr->m_MountSpellId );

				if( plyr->GetVehicle() )
					plyr->GetVehicle()->RemovePassenger( plyr );

				plyr->m_bg->HookFlagDrop(plyr, obj);
			}
		}break;
		case GAMEOBJECT_TYPE_QUESTGIVER:
		{
			// Questgiver
			if(obj->HasQuests())
				sQuestMgr.OnActivateQuestGiver(obj, plyr);
		}break;
		case GAMEOBJECT_TYPE_SPELLCASTER:
		{
			SpellEntry *info = dbcSpell.LookupEntry(goinfo->SpellFocus);
			if(!info)
				break;
			Spell* spell(new Spell(plyr, info, false, NULLAURA));
			SpellCastTargets targets;
			targets.m_unitTarget = plyr->GetGUID();
			spell->prepare(&targets);
			if(obj->charges>0 && !--obj->charges)
				obj->ExpireAndDelete();
		}break;
		case GAMEOBJECT_TYPE_RITUAL:
		{
			// store the members in the ritual, cast sacrifice spell, and summon.
			uint32 i = 0;
			if(!obj->m_ritualmembers || !obj->GetGOui32Value(GO_UINT32_RIT_SPELL) || !obj->GetGOui32Value(GO_UINT32_M_RIT_CASTER))
				return;

			for(i=0;i<goinfo->SpellFocus;++i)
			{
				if(!obj->m_ritualmembers[i])
				{
					obj->m_ritualmembers[i] = plyr->GetLowGUID();
					plyr->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, obj->GetGUID());
					plyr->SetUInt32Value(UNIT_CHANNEL_SPELL, obj->GetGOui32Value(GO_UINT32_RIT_SPELL));
					break;
				}else if(obj->m_ritualmembers[i] == plyr->GetLowGUID())
				{
					// we're deselecting :(
					obj->m_ritualmembers[i] = 0;
					plyr->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					plyr->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					return;
				}
			}

			if(i == goinfo->SpellFocus - 1)
			{
				obj->SetGOui32Value(GO_UINT32_RIT_SPELL, 0);
				Player* plr;
				for(i=0;i<goinfo->SpellFocus;++i)
				{
					plr = _player->GetMapMgr()->GetPlayer(obj->m_ritualmembers[i]);
					if(plr!=NULL)
					{
						plr->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						plr->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					}
				}

				SpellEntry *info = NULL;
				switch( goinfo->ID )
				{
				case 36727:// summon portal
					{
						if(!obj->GetGOui32Value(GO_UINT32_M_RIT_TARGET))
							return;

						info = dbcSpell.LookupEntry(goinfo->sound1);
						if(!info)
							break;
						Player* target = _player->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_TARGET));
						if(!target)
							return;

						spell = (new Spell(obj,info,true,NULLAURA));
						SpellCastTargets targets;
						targets.m_unitTarget = target->GetGUID();
						spell->prepare(&targets);
					}break;
				case 177193:// doom portal
					{
						Player* psacrifice = NULLPLR;
						Spell* spell = NULLSPELL;

						// kill the sacrifice player
						psacrifice = _player->GetMapMgr()->GetPlayer(obj->m_ritualmembers[(int)(RandomUInt(goinfo->SpellFocus-1))]);
						Player* pCaster = obj->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_CASTER));
						if(!psacrifice || !pCaster)
							return;

						info = dbcSpell.LookupEntry(goinfo->sound4);
						if(!info)
							break;
						spell = (new Spell(psacrifice, info, true, NULLAURA));
						targets.m_unitTarget = psacrifice->GetGUID();
						spell->prepare(&targets);

						// summons demon
						info = dbcSpell.LookupEntry(goinfo->sound1);
						spell = (new Spell(pCaster, info, true, NULLAURA));
						SpellCastTargets targets;
						targets.m_unitTarget = pCaster->GetGUID();
						spell->prepare(&targets);
					}break;
				case 179944:// Summoning portal for meeting stones
					{
						Player* plr = _player->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_TARGET));
						if(!plr)
							return;

						Player* pleader = _player->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_CASTER));
						if(!pleader)
							return;

						info = dbcSpell.LookupEntry(goinfo->sound1);
						Spell* spell(new Spell(pleader, info, true, NULLAURA));
						SpellCastTargets targets(plr->GetGUID());
						spell->prepare(&targets);

						/* expire the GameObject* */
						obj->ExpireAndDelete();
					}break;
				case 194108:// Ritual of Summoning portal for warlocks
					{
						Player* pleader = _player->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_CASTER));
						if(!pleader)
							return;

						info = dbcSpell.LookupEntry(goinfo->sound1);
						Spell* spell(new Spell(pleader, info, true, NULLAURA));
						SpellCastTargets targets(pleader->GetGUID());
						spell->prepare(&targets);

						obj->ExpireAndDelete();
						pleader->InterruptCurrentSpell();
					}break;
				case 186811://Ritual of Refreshment
				case 193062:
					{
						Player* pleader = _player->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_CASTER));
						if(!pleader)
							return;

						info = dbcSpell.LookupEntry(goinfo->sound1);
						Spell* spell(new Spell(pleader, info, true, NULLAURA));
						SpellCastTargets targets(pleader->GetGUID());
						spell->prepare(&targets);

						obj->ExpireAndDelete();
						pleader->InterruptCurrentSpell();
					}break;
				case 181622://Ritual of Souls
				case 193168:
					{
						Player* pleader = _player->GetMapMgr()->GetPlayer(obj->GetGOui32Value(GO_UINT32_M_RIT_CASTER));
						if(!pleader)
							return;

						info = dbcSpell.LookupEntry(goinfo->sound1);
						Spell* spell(new Spell(pleader, info, true, NULLAURA));
						SpellCastTargets targets(pleader->GetGUID());
						spell->prepare(&targets);
					}break;
				}
			}
		}break;
		case GAMEOBJECT_TYPE_GOOBER:
		{
			//Quest related mostly
		}
		case GAMEOBJECT_TYPE_CAMERA://eye of azora
		{
			SpellEntry * sp = dbcSpell.LookupEntryForced(goinfo->Unknown1);
			if(sp != NULL)
				_player->CastSpell(_player,sp,true);
			else
			{
				WorldPacket data(SMSG_TRIGGER_CINEMATIC, 4);
				data << uint32(goinfo->sound1);
				SendPacket(&data);
			}
		}break;
		case GAMEOBJECT_TYPE_MEETINGSTONE:	// Meeting Stone
		{
			/* Use selection */
			Player* pPlayer = objmgr.GetPlayer((uint32)_player->GetSelection());
			if(!pPlayer || _player->GetGroup() != pPlayer->GetGroup() || !_player->GetGroup())
				return;

			GameObjectInfo * info = GameObjectNameStorage.LookupEntry(179944);
			if(!info)
				return;

			/* Create the summoning portal */
			GameObject* pGo = _player->GetMapMgr()->CreateGameObject(179944);
			if( pGo == NULL || !pGo->CreateFromProto(179944, _player->GetMapId(), _player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
				return;

			// dont allow to spam them
			GameObject* gobj = _player->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(_player->GetPositionX(), _player->GetPositionY(), _player->GetPositionZ(), 179944);
			if( gobj )
				gobj->ExpireAndDelete();

			pGo->SetGOui32Value(GO_UINT32_M_RIT_CASTER, _player->GetLowGUID());
			pGo->SetGOui32Value(GO_UINT32_M_RIT_TARGET, pPlayer->GetLowGUID());
			pGo->SetGOui32Value(GO_UINT32_RIT_SPELL, 61994);
			pGo->PushToWorld(_player->GetMapMgr());

			/* member one: the (w00t) caster */
			pGo->m_ritualmembers[0] = _player->GetLowGUID();
			_player->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, pGo->GetGUID());
			_player->SetUInt32Value(UNIT_CHANNEL_SPELL, pGo->GetGOui32Value(GO_UINT32_RIT_SPELL));

			/* expire after 2mins*/
			sEventMgr.AddEvent(pGo, &GameObject::_Expire, EVENT_GAMEOBJECT_EXPIRE, 120000, 1,0);
		}break;
		case GAMEOBJECT_TYPE_BARBER_CHAIR:
		{
			plyr->SafeTeleport( plyr->GetMapId(), plyr->GetInstanceID(), obj->GetPositionX(), obj->GetPositionY(), obj->GetPositionZ(), obj->GetOrientation() );
			plyr->SetStandState(STANDSTATE_SIT_MEDIUM_CHAIR);
			plyr->m_lastRunSpeed = 0; //counteract mount-bug; reset speed to zero to force update SetPlayerSpeed in next line.
			plyr->SetPlayerSpeed(RUN,plyr->m_base_runSpeed);
			WorldPacket data(SMSG_ENABLE_BARBER_SHOP, 0);
			plyr->GetSession()->SendPacket(&data);
		}break;
	}
}

void WorldSession::HandleTutorialFlag( WorldPacket & recv_data )
{
	uint32 iFlag;
	recv_data >> iFlag;

	uint32 wInt = (iFlag / 32);
	uint32 rInt = (iFlag % 32);

	if(wInt >= 7)
	{
		Disconnect();
		return;
	}

	uint32 tutflag = GetPlayer()->GetTutorialInt( wInt );
	tutflag |= (1 << rInt);
	GetPlayer()->SetTutorialInt( wInt, tutflag );

	DEBUG_LOG("WorldSession","Received Tutorial Flag Set {%u}.", iFlag);
}

void WorldSession::HandleTutorialClear( WorldPacket & recv_data )
{
	for ( uint32 iI = 0; iI < 8; iI++)
		GetPlayer()->SetTutorialInt( iI, 0xFFFFFFFF );
}

void WorldSession::HandleTutorialReset( WorldPacket & recv_data )
{
	for ( uint32 iI = 0; iI < 8; iI++)
		GetPlayer()->SetTutorialInt( iI, 0x00000000 );
}

void WorldSession::HandleSetSheathedOpcode( WorldPacket & recv_data )
{
	uint32 active;
	recv_data >> active;
	_player->SetByte(UNIT_FIELD_BYTES_2,0,(uint8)active);
}

void WorldSession::HandlePlayedTimeOpcode( WorldPacket & recv_data )
{
	uint8 send;
	recv_data >> send;

	uint32 playedt = (uint32)UNIXTIME - _player->m_playedtime[2];
	if(playedt)
	{
		_player->m_playedtime[0] += playedt;
		_player->m_playedtime[1] += playedt;
		_player->m_playedtime[2] += playedt;
	}

	WorldPacket data(SMSG_PLAYED_TIME, 9);
	data << (uint32)_player->m_playedtime[1];
	data << (uint32)_player->m_playedtime[0];
	data << uint8(send);
	SendPacket(&data);
}

void WorldSession::HandleInspectOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE( recv_data, 8 );
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Player* player = _player->GetMapMgr()->GetPlayer( (uint32)guid );
	if( player == NULL )
		return;

	WorldPacket data(SMSG_INSPECT_TALENT, 1000);
	data << player->GetNewGUID();
	player->BuildPlayerTalentsInfo(&data);

	// build items inspect part. could be sent separately as SMSG_INSPECT
	uint32 slotUsedMask = 0;
	uint16 enchantmentMask = 0;
	size_t maskPos = data.wpos();
	data << uint32(slotUsedMask);	// will be replaced later
	for(uint32 slot = 0; slot < EQUIPMENT_SLOT_END; slot++)
	{
		Item* item = player->GetItemInterface()->GetInventoryItem(slot);
		if( item )
		{
			slotUsedMask |= (1 << slot);

			data << uint32(item->GetEntry());

			size_t maskPosEnch = data.wpos();
			enchantmentMask = 0;
			data << uint16(enchantmentMask);

			for(uint32 ench = 0; ench < 12; ench++)
			{
				uint16 enchId = (uint16) item->GetUInt32Value( ITEM_FIELD_ENCHANTMENT_1_1 + (ench * 3));
				if( enchId )
				{
					enchantmentMask |= (1 << ench);
					data << uint16(enchId);
				}
			}
			*(uint16*)&data.contents()[maskPosEnch] = enchantmentMask;

			data << uint16(0);	// unk

			uint64 creatorguid = item->GetUInt64Value(ITEM_FIELD_CREATOR);
			if(creatorguid)
				data << WoWGuid(creatorguid);
			else
				data << uint8(0);

			data << uint32(0);	// unk
		}
	}
	*(uint16*)&data.contents()[maskPos] = slotUsedMask;
	SendPacket( &data );
}

void WorldSession::HandleSetActionBarTogglesOpcode(WorldPacket &recvPacket)
{
	uint8 cActionBarId;
	recvPacket >> cActionBarId;
	DEBUG_LOG("WorldSession","Received CMSG_SET_ACTIONBAR_TOGGLES for actionbar id %d.", cActionBarId);
	GetPlayer()->SetByte(PLAYER_FIELD_BYTES,2, cActionBarId);
}

void WorldSession::HandleSelfResurrectOpcode(WorldPacket& recv_data)
{
	uint32 self_res_spell = _player->GetUInt32Value(PLAYER_SELF_RES_SPELL);
	if(self_res_spell)
	{
		SpellEntry * sp = dbcSpell.LookupEntry(self_res_spell);
		Spell* s(new Spell(_player,sp,false,NULLAURA));
		SpellCastTargets tgt;
		tgt.m_unitTarget=_player->GetGUID();
		s->prepare(&tgt);
	}
}

void WorldSession::HandleRandomRollOpcode(WorldPacket& recv_data)
{
	uint32 min, max;
	recv_data >> min >> max;

	OUT_DEBUG("WORLD: Received MSG_RANDOM_ROLL: %u-%u", min, max);

	WorldPacket data(20);
	data.SetOpcode(MSG_RANDOM_ROLL);
	data << min << max;

	if(max < min)
		return;

	uint32 roll;

	// generate number
	roll = RandomUInt(max - min) + min + 1;

	// append to packet, and guid
	data << roll << _player->GetGUID();

	// send to set
	if(_player->InGroup())
		_player->GetGroup()->SendPacketToAll(&data);
	else
		GetPlayer()->SendMessageToSet(&data, true, true);
}

void WorldSession::HandleLootMasterGiveOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	uint32 itemid = 0;
	uint32 amt = 1;
	uint8 error = 0;
	SlotResult slotresult;

	Creature* pCreature = NULLCREATURE;
	Loot *pLoot = NULL;
	/* struct:
	{CLIENT} Packet: (0x02A3) CMSG_LOOT_MASTER_GIVE PacketSize = 17
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|39 23 05 00 81 02 27 F0 01 7B FC 02 00 00 00 00 |9#....'..{......|
	|00											  |.			   |
	-------------------------------------------------------------------

		uint64 creatureguid
		uint8  slotid
		uint64 target_playerguid

	*/
	uint64 creatureguid, target_playerguid;
	uint8 slotid;
	recv_data >> creatureguid >> slotid >> target_playerguid;

	if(_player->GetGroup() == NULL || _player->GetGroup()->GetLooter() != _player->m_playerInfo)
		return;

	Player* player = _player->GetMapMgr()->GetPlayer((uint32)target_playerguid);
	if(!player)
		return;

	// cheaterz!
	if(_player->GetLootGUID() != creatureguid)
		return;

	//now its time to give the loot to the target player
	if(GET_TYPE_FROM_GUID(GetPlayer()->GetLootGUID()) == HIGHGUID_TYPE_CREATURE)
	{
		pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(creatureguid));
		if (!pCreature)return;
		pLoot=&pCreature->m_loot;
	}

	if(!pLoot) return;

	if (slotid >= pLoot->items.size())
	{
		OUT_DEBUG("AutoLootItem: Player %s might be using a hack! (slot %d, size %d)",
						GetPlayer()->GetName(), slotid, pLoot->items.size());
		return;
	}

	amt = pLoot->items.at(slotid).iItemsCount;

	if (!pLoot->items.at(slotid).ffa_loot)
	{
		if (!amt)//Test for party loot
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ALREADY_LOOTED);
			return;
		}
	}
	else
	{
		//make sure this player can still loot it in case of ffa_loot
		LooterSet::iterator itr = pLoot->items.at(slotid).has_looted.find(player->GetLowGUID());

		if (pLoot->items.at(slotid).has_looted.end() != itr)
		{
			GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_ALREADY_LOOTED);
			return;
		}
	}

	itemid = pLoot->items.at(slotid).item.itemproto->ItemId;
	ItemPrototype* it = pLoot->items.at(slotid).item.itemproto;

	if((error = player->GetItemInterface()->CanReceiveItem(it, 1, 0)))
	{
		_player->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM, error);
		return;
	}

	if(pCreature)
		CALL_SCRIPT_EVENT(pCreature, OnLootTaken)(player, it);

	slotresult = player->GetItemInterface()->FindFreeInventorySlot(it);
	if(!slotresult.Result)
	{
		GetPlayer()->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM, INV_ERR_INVENTORY_FULL);
		return;
	}

	Item* item = objmgr.CreateItem( itemid, player);

	item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,amt);
	if(pLoot->items.at(slotid).iRandomProperty!=NULL)
	{
		item->SetRandomProperty(pLoot->items.at(slotid).iRandomProperty->ID);
		item->ApplyRandomProperties(false);
	}
	else if(pLoot->items.at(slotid).iRandomSuffix != NULL)
	{
		item->SetRandomSuffix(pLoot->items.at(slotid).iRandomSuffix->id);
		item->ApplyRandomProperties(false);
	}

	if( player->GetItemInterface()->SafeAddItem(item,slotresult.ContainerSlot, slotresult.Slot) )
	{
		player->GetSession()->SendItemPushResult(item,false,true,true,true,slotresult.ContainerSlot,slotresult.Slot,1);
		sQuestMgr.OnPlayerItemPickup(player,item);
	}
	else
	{
		item->DeleteMe();
		item = NULLITEM;
	}

	pLoot->items.at(slotid).iItemsCount=0;

	// this gets sent to all looters
	if (!pLoot->items.at(slotid).ffa_loot)
	{
		pLoot->items.at(slotid).iItemsCount=0;

		// this gets sent to all looters
		WorldPacket data(1);
		data.SetOpcode(SMSG_LOOT_REMOVED);
		data << slotid;
		Player* plr;
		for(LooterSet::iterator itr = pLoot->looters.begin(); itr != pLoot->looters.end(); itr++)
		{
			if((plr = _player->GetMapMgr()->GetPlayer(*itr)))
				plr->GetSession()->SendPacket(&data);
		}
	}
	else
	{
		pLoot->items.at(slotid).has_looted.insert(player->GetLowGUID());
	}
}

void WorldSession::HandleLootRollOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	/* struct:

	{CLIENT} Packet: (0x02A0) CMSG_LOOT_ROLL PacketSize = 13
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|11 4D 0B 00 BD 06 01 F0 00 00 00 00 02		  |.M...........   |
	-------------------------------------------------------------------

	uint64 creatureguid
	uint21 slotid
	uint8  choice

	*/
	uint64 creatureguid;
	uint32 slotid;
	uint8 choice;
	recv_data >> creatureguid >> slotid >> choice;

	LootRoll* li = NULLROLL;

	uint32 guidtype = GET_TYPE_FROM_GUID(creatureguid);
	if (guidtype == HIGHGUID_TYPE_GAMEOBJECT)
	{
		GameObject* pGO = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(creatureguid));
		if (!pGO)
			return;
		if (slotid >= pGO->m_loot.items.size() || pGO->m_loot.items.size() == 0)
			return;
		if (pGO->GetInfo() && pGO->GetInfo()->Type == GAMEOBJECT_TYPE_CHEST)
			li = pGO->m_loot.items[slotid].roll;
	}
	else if (guidtype == HIGHGUID_TYPE_CREATURE)
	{
		// Creatures
		Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(creatureguid));
		if (!pCreature)
			return;

		if (slotid >= pCreature->m_loot.items.size() || pCreature->m_loot.items.size() == 0)
			return;

		li = pCreature->m_loot.items[slotid].roll;
	}
	else
		return;

	if(!li)
		return;

	li->PlayerRolled(_player->getPlayerInfo(), choice);
}

void WorldSession::HandleOpenItemOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 2);
	int8 slot, containerslot;
	recv_data >> containerslot >> slot;

	Item* pItem = _player->GetItemInterface()->GetInventoryItem(containerslot, slot);
	if(!pItem)
		return;

	// gift wrapping handler
	if(pItem->GetUInt32Value(ITEM_FIELD_GIFTCREATOR) && pItem->wrapped_item_id)
	{
		ItemPrototype * it = ItemPrototypeStorage.LookupEntry(pItem->wrapped_item_id);
		if(it == NULL)
			return;

		pItem->SetUInt32Value(ITEM_FIELD_GIFTCREATOR,0);
		pItem->SetUInt32Value(OBJECT_FIELD_ENTRY,pItem->wrapped_item_id);
		pItem->wrapped_item_id=0;
		pItem->SetProto(it);

		if(it->Bonding==ITEM_BIND_ON_PICKUP)
			pItem->SetUInt32Value(ITEM_FIELD_FLAGS,1);
		else
			pItem->SetUInt32Value(ITEM_FIELD_FLAGS,0);

		if(it->MaxDurability)
		{
			pItem->SetUInt32Value(ITEM_FIELD_DURABILITY,it->MaxDurability);
			pItem->SetUInt32Value(ITEM_FIELD_MAXDURABILITY,it->MaxDurability);
		}

		pItem->m_isDirty=true;
		pItem->SaveToDB(containerslot,slot, false, NULL);
		return;
	}

	Lock *lock = dbcLock.LookupEntry( pItem->GetProto()->LockId );

	uint32 removeLockItems[8] = {0,0,0,0,0,0,0,0};

	if(lock) // locked item
	{
		for(int i=0; i<8;++i)
		{
			if(lock->locktype[i] == 1 && lock->lockmisc[i] > 0)
			{
				int8 slot = _player->GetItemInterface()->GetInventorySlotById(lock->lockmisc[i]);
				if(slot != ITEM_NO_SLOT_AVAILABLE && slot >= INVENTORY_SLOT_ITEM_START && slot < INVENTORY_SLOT_ITEM_END)
				{
					removeLockItems[i] = lock->lockmisc[i];
				}
				else
				{
					_player->GetItemInterface()->BuildInventoryChangeError(pItem,NULLITEM,INV_ERR_ITEM_LOCKED);
					return;
				}
			}
			else if(lock->locktype[i] == 2 && pItem->locked)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(pItem,NULLITEM,INV_ERR_ITEM_LOCKED);
				return;
			}
		}
		for(int j=0; j<8;++j)
		{
			if(removeLockItems[j])
				_player->GetItemInterface()->RemoveItemAmt(removeLockItems[j],1);
		}
	}

	// fill loot
	_player->SetLootGUID(pItem->GetGUID());
	if( !pItem->m_looted )
	{
		// delete item from database, so we can't cheat
		pItem->DeleteFromDB();
		lootmgr.FillItemLoot(&pItem->m_loot, pItem->GetEntry());

		if(pItem->GetProto()->Lootgold > 0) // Gold can be looted from items. http://www.wowhead.com/?item=45724
			pItem->m_loot.gold = pItem->GetProto()->Lootgold;

		if(pItem->m_loot.gold)
			pItem->m_loot.gold = int32(float(pItem->m_loot.gold) * sWorld.getRate(RATE_MONEY));

		pItem->m_looted = true;
	}

	_player->SendLoot(pItem->GetGUID(), _player->GetMapId(), LOOT_GATHERING);
}

void WorldSession::HandleCompleteCinematic(WorldPacket& recv_data)
{
	// when a Cinematic is started the player is going to sit down, when its finished its standing up.
	_player->SetStandState(STANDSTATE_STAND);
};

void WorldSession::HandleResetInstanceOpcode(WorldPacket& recv_data)
{
	sInstanceMgr.ResetSavedInstances(_player);
}

void EncodeHex(const char* source, char* dest, uint32 size)
{
	char temp[5];
	for(uint32 i = 0; i < size; i++)
	{
		snprintf(temp, 5, "%02X", source[i]);
		strcat(dest, temp);
	}
}

void DecodeHex(const char* source, char* dest, uint32 size)
{
	char temp;
	char* acc = const_cast<char*>(source);
	for(uint32 i = 0; i < size; i++)
	{
		sscanf("%02X", &temp);
		acc = ((char*)&source[2]);
		strcat(dest, &temp);
	}
}

void WorldSession::HandleToggleCloakOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	//////////////////////////
	//	PLAYER_FLAGS									   = 3104 / 0x00C20 / 0000000000000000000110000100000
	//																							 ^
	// This bit, on = toggled OFF, off = toggled ON.. :S

	//uint32 SetBit = 0 | (1 << 11);

	if(_player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_NOCLOAK))
		_player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_NOCLOAK);
	else
		_player->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_NOCLOAK);
}

void WorldSession::HandleToggleHelmOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;
	//////////////////////////
	//	PLAYER_FLAGS									   = 3104 / 0x00C20 / 0000000000000000000110000100000
	//																							  ^
	// This bit, on = toggled OFF, off = toggled ON.. :S

	//uint32 SetBit = 0 | (1 << 10);

	if(_player->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_NOHELM))
		_player->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_NOHELM);
	else
		_player->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_NOHELM);
}

void WorldSession::HandleDungeonDifficultyOpcode(WorldPacket& recv_data)
{
	uint32 data;
	recv_data >> data;

	if(_player->GetGroup() && _player->IsGroupLeader())
	{
		WorldPacket pData;
		pData.Initialize(MSG_SET_DUNGEON_DIFFICULTY);
		pData << data;

		_player->iInstanceType = data;
		sInstanceMgr.ResetSavedInstances(_player);

		Group * m_Group = _player->GetGroup();

		m_Group->SetDifficulty(data);
		m_Group->Lock();
		for(uint32 i = 0; i < m_Group->GetSubGroupCount(); i++)
		{
			for(GroupMembersSet::iterator itr = m_Group->GetSubGroup(i)->GetGroupMembersBegin(); itr != m_Group->GetSubGroup(i)->GetGroupMembersEnd(); itr++)
			{
				if((*itr)->m_loggedInPlayer)
				{
					(*itr)->m_loggedInPlayer->iInstanceType = data;
					(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&pData);
				}
			}
		}
		m_Group->Unlock();
	}
	else if(!_player->GetGroup())
	{
		_player->iInstanceType = data;
		sInstanceMgr.ResetSavedInstances(_player);
	}

#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_InstanceType();
#endif
}

void WorldSession::HandleRaidDifficultyOpcode(WorldPacket& recv_data)
{
	uint32 data;
	recv_data >> data;

	if(_player->GetGroup() && _player->IsGroupLeader())
	{
		WorldPacket pData;
		pData.Initialize(MSG_SET_RAID_DIFFICULTY);
		pData << data;

		_player->iRaidType = data;
		Group * m_Group = _player->GetGroup();

		m_Group->SetRaidDifficulty(data);
		m_Group->Lock();
		for(uint32 i = 0; i < m_Group->GetSubGroupCount(); i++)
		{
			for(GroupMembersSet::iterator itr = m_Group->GetSubGroup(i)->GetGroupMembersBegin(); itr != m_Group->GetSubGroup(i)->GetGroupMembersEnd(); itr++)
			{
				if((*itr)->m_loggedInPlayer)
				{
					(*itr)->m_loggedInPlayer->iRaidType = data;
					(*itr)->m_loggedInPlayer->GetSession()->SendPacket(&pData);
				}
			}
		}
		m_Group->Unlock();
	}
	else if(!_player->GetGroup())
	{
		_player->iRaidType = data;
	}

#ifdef OPTIMIZED_PLAYER_SAVING
	_player->save_InstanceType();
#endif
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket & recv_data)
{
	uint64 summonguid;
	bool agree;
	recv_data >> summonguid;
	recv_data >> agree;

	//Do we have a summoner?
	if(!_player->m_summoner)
	{
		SendNotification("Summoner guid has changed or does not exist.");
		return;
	}

	// Summoner changed?
	if(_player->m_summoner->GetGUID() != summonguid)
	{
		SendNotification("Summoner guid has changed or does not exist.");
		return;
	}

	//not during combat
	if(_player->CombatStatus.IsInCombat())
		return;

	//Map checks.
	MapInfo * inf = WorldMapInfoStorage.LookupEntry(_player->m_summonMapId);
	if(!inf)
		return;

	//are we summoning from witin the same instance?
	if( _player->m_summonInstanceId != _player->GetInstanceID() )
	{
		// if not, are we allowed on the summoners map?
		uint8 pReason = CheckTeleportPrerequisites(NULL, this, _player, inf->mapid);
		if( pReason )
		{
			SendNotification(NOTIFICATION_MESSAGE_NO_PERMISSION);
			return;
		}
	}
	if(agree)
	{
		if(!_player->SafeTeleport(_player->m_summonMapId, _player->m_summonInstanceId, _player->m_summonPos))
			SendNotification(NOTIFICATION_MESSAGE_FAILURE);

		_player->m_summoner = NULLOBJ;
		_player->m_summonInstanceId = _player->m_summonMapId = 0;
		return;
	}
	else
	{
		// Null-out the summoner
		_player->m_summoner = NULLOBJ;
		_player->m_summonInstanceId = _player->m_summonMapId = 0;
		return;
	}
}

void WorldSession::HandleDismountOpcode(WorldPacket& recv_data)
{
	DEBUG_LOG( "WORLD"," Received CMSG_DISMOUNT"  );

	if( !_player->IsInWorld() || _player->GetTaxiState())
		return;

	if( _player->IsMounted() )
		TO_UNIT(_player)->Dismount();
}

void WorldSession::HandleSetAutoLootPassOpcode(WorldPacket & recv_data)
{
	uint32 on;
	recv_data >> on;

	if( _player->IsInWorld() )
		_player->BroadcastMessage("Auto loot passing is now %s.", on ? "on" : "off");

	_player->m_passOnLoot = (on!=0) ? true : false;
}

void WorldSession::HandleRemoveGlyph(WorldPacket & recv_data)
{
	uint32 glyphSlot;
	recv_data >> glyphSlot;
	_player->UnapplyGlyph(glyphSlot);
	if(glyphSlot < GLYPHS_COUNT)
		_player->m_specs[_player->m_talentActiveSpec].glyphs[glyphSlot] = 0;
}

void WorldSession::HandleWorldStateUITimerUpdate(WorldPacket& recv_data)
{
	WorldPacket data(SMSG_WORLD_STATE_UI_TIMER_UPDATE, 4);
	data << (uint32)UNIXTIME;
	SendPacket(&data);
}

void WorldSession::HandleReadyForAccountDataTimes(WorldPacket& recv_data)
{
	DEBUG_LOG( "WORLD","Received CMSG_READY_FOR_ACCOUNT_DATA_TIMES" );

	// account data == UI config
	WorldPacket data(SMSG_ACCOUNT_DATA_TIMES, 4+1+4+8*4);
	data << uint32(UNIXTIME) << uint8(1) << uint32(0x15);
	for (int i = 0; i < 8; i++)
	{
		if(0x15 & (1 << i))
		{
			data << uint32(0);
		}
	}
	SendPacket(&data);
}

void WorldSession::HandleFarsightOpcode(WorldPacket& recv_data)
{
	uint8 type;
	recv_data >> type;

	// TODO

	GetPlayer()->UpdateVisibility();
}

void WorldSession::HandleGameobjReportUseOpCode( WorldPacket& recv_data )
{
	if(!_player->IsInWorld()) // Teleporting? :O
	{
		SKIP_READ_PACKET(recv_data);
		return;
	}

	uint64 guid;
	recv_data >> guid;
	GameObject* gameobj = _player->GetMapMgr()->GetGameObject(GET_LOWGUID_PART(guid));
	if(gameobj != NULLGOB && gameobj->GetInfo())
	{
		// Gossip Script
		if(gameobj->GetInfo()->gossip_script)
			gameobj->GetInfo()->gossip_script->GossipHello(gameobj, _player, true);

		if(gameobj->CanActivate())
			sQuestMgr.OnGameObjectActivate(_player, gameobj);
	}
}

void WorldSession::HandleTalentWipeConfirmOpcode( WorldPacket& recv_data )
{
	uint64 guid;
	recv_data >> guid;
	CHECK_INWORLD_RETURN;

	uint32 playerGold = _player->GetUInt32Value( PLAYER_FIELD_COINAGE );
	uint32 price = _player->CalcTalentResetCost(_player->GetTalentResetTimes());

	if( playerGold < price )
		return;

	_player->SetUInt32Value( PLAYER_FIELD_COINAGE, playerGold - price );

	_player->GetAchievementInterface()->HandleAchievementCriteriaTalentResetCostTotal( price );
	_player->GetAchievementInterface()->HandleAchievementCriteriaTalentResetCount();

	_player->CastSpell(_player, 14867, true);	// Spell: "Untalent Visual Effect"
	_player->CastSpell(_player, 46331, true);	// Spell: "Trainer: Untrain Talents"

	WorldPacket data( MSG_TALENT_WIPE_CONFIRM, 12);	// You don't have any talent.
	data << uint64(0);
	data << uint32(0);
	SendPacket( &data );
	return;
}

void WorldSession::HandleCalendarGetCalendar(WorldPacket& recv_data)
{
	time_t cur_time = time(NULL);

	WorldPacket data(SMSG_CALENDAR_SEND_CALENDAR, 4+4*0+4+4*0+4+4);

	// TODO: calendar invite event output
	// TODO: calendar event output
	data << uint32(0);			//invite node count
	data << uint32(0);			//event count
	data << uint32(0);			//wtf??
	data << uint32(secsToTimeBitFields(cur_time));			// current time

	uint32 count = 0;
	size_t p_count = data.wpos();
	data << uint32(count);	// Calender shit.
	data.put<uint32>(p_count,count);
	data << uint32(1135753200);	// Unix Time for when calendars started?
	data << uint32(0);			//  unk counter 4
	data << uint32(0);			// unk counter 5
	SendPacket(&data);
}

void WorldSession::HandleCalendarGetEvent(WorldPacket& recv_data)
{

}

void WorldSession::HandleCalendarGuildFilter(WorldPacket& recv_data)
{

}

void WorldSession::HandleCalendarArenaTeam(WorldPacket& recv_data)
{

}

void WorldSession::HandleCalendarAddEvent(WorldPacket& recv_data)
{
	SKIP_READ_PACKET(recv_data);	// set to end to avoid warnings spam

	std::string unk1, unk2;
	recv_data >> unk1;
	recv_data >> unk2;

	uint8   unk3, unk4;
	uint32  unk5, unk6, unk7, unk8, unk9, count = 0;
	recv_data >> unk3;
	recv_data >> unk4;
	recv_data >> unk5;
	recv_data >> unk6;
	recv_data >> unk7;
	recv_data >> unk8;
	recv_data >> unk9;
	if(!((unk9 >> 6) & 1))
	{
		recv_data >> count;
		if (count)
		{
			uint8 unk12,unk13;
			WoWGuid guid;
			for (uint i = 0; i < count; i++)
			{
				recv_data >> guid;
				recv_data >> unk12;
				recv_data >> unk13;
			}
		}
	}
}

void WorldSession::HandleCalendarUpdateEvent(WorldPacket& recv_data)
{
	SKIP_READ_PACKET(recv_data);

	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> std::string
	//recv_data >> std::string
	//recv_data >> uint8
	//recv_data >> uint8
	//recv_data >> uint32
	//recv_data >> uint32
	//recv_data >> uint32
	//recv_data >> uint32
	//recv_data >> uint32
}

void WorldSession::HandleCalendarRemoveEvent(WorldPacket& recv_data)
{
	SKIP_READ_PACKET(recv_data);

	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint32

}

void WorldSession::HandleCalendarCopyEvent(WorldPacket& recv_data)
{
	SKIP_READ_PACKET(recv_data);

	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint32

}

void WorldSession::HandleCalendarEventInvite(WorldPacket& recv_data)
{
	recv_data.hexlike();
	SKIP_READ_PACKET(recv_data);

	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> std::string
	//recv_data >> uint8
	//recv_data >> uint8

}

void WorldSession::HandleCalendarEventRsvp(WorldPacket& recv_data)
{
	recv_data.hexlike();
	SKIP_READ_PACKET(recv_data);

	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint32

}

void WorldSession::HandleCalendarEventRemoveInvite(WorldPacket& recv_data)
{
	recv_data.hexlike();
	SKIP_READ_PACKET(recv_data);

	//recv_data >> wowguid(guid);
	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint64
}

void WorldSession::HandleCalendarEventStatus(WorldPacket& recv_data)
{
	recv_data.hexlike();
	SKIP_READ_PACKET(recv_data);

	//recv_data >> wowguid(guid);
	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint32
}

void WorldSession::HandleCalendarEventModeratorStatus(WorldPacket& recv_data)
{
	recv_data.hexlike();
	SKIP_READ_PACKET(recv_data);

	//recv_data >> wowguid(guid);
	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint32
}

void WorldSession::HandleCalendarComplain(WorldPacket& recv_data)
{
	recv_data.hexlike();
	SKIP_READ_PACKET(recv_data);

	//recv_data >> uint64
	//recv_data >> uint64
	//recv_data >> uint64
}

void WorldSession::HandleCalendarGetNumPending(WorldPacket & /*recv_data*/)
{
	WorldPacket data(SMSG_CALENDAR_SEND_NUM_PENDING, 4);
	data << uint32(0);		// 0 - no pending invites, 1 - some pending invites
	SendPacket(&data);
}

void WorldSession::HandleMeetingStoneInfo(WorldPacket& )
{
	DEBUG_LOG("WORLD"," Received CMSG_MEETINGSTONE_INFO");
	//Used for LFR/LFG updates
}

void WorldSession::HandleHearthandResurrect(WorldPacket &recv_data)
{
	CHECK_INWORLD_RETURN
	_player->ResurrectPlayer();
	_player->SafeTeleport(_player->GetBindMapId(),0,_player->GetBindPositionX(),_player->GetBindPositionY(),_player->GetBindPositionZ(),_player->GetOrientation());
}