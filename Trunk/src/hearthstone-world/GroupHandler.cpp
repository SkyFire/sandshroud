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

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GROUP_INVITE
//////////////////////////////////////////////////////////////
void WorldSession::HandleGroupInviteOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;

	CHECK_PACKET_SIZE(recv_data, 1);
	WorldPacket data(100);
	std::string membername;
	uint32 serverid; // Sent as of 3.3 multiserver parties
	Player* player = NULLPLR;
	Group *group = NULL;

	recv_data >> membername;
	recv_data >> serverid;
	if(_player->HasBeenInvited())return;

	player = objmgr.GetPlayer(membername.c_str(), false);

	if ( player == NULL)
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		return;
	}

	if (player == _player)
	{
		return;
	}

	if ( _player->InGroup() && (!_player->IsGroupLeader() || _player->GetGroup()->HasFlag(GROUP_FLAG_BATTLEGROUND_GROUP) ) )
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	group = _player->GetGroup();
	if ( group != NULL )
	{
		if (group->IsFull())
		{
			SendPartyCommandResult(_player, 0, "", ERR_PARTY_IS_FULL);
			return;
		}
	}

	if ( player->InGroup() )
	{
		SendPartyCommandResult(_player, player->GetGroup()->GetGroupType(), membername, ERR_PARTY_ALREADY_IN_GROUP);
		data.SetOpcode(SMSG_GROUP_INVITE);
		data << uint8(0);
		data << GetPlayer()->GetName();
		player->GetSession()->SendPacket(&data);
		return;
	}

	if(player->GetTeam()!=_player->GetTeam() && _player->GetSession()->GetPermissionCount() == 0 && !sWorld.cross_faction_world)
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_WRONG_FACTION);
		return;
	}

	if ( player->HasBeenInvited() )
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_ALREADY_IN_GROUP);
		return;
	}

	if( player->Social_IsIgnoring( _player->GetLowGUID() ) )
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_IS_IGNORING_YOU);
		return;
	}

	if( player->bGMTagOn && !HasGMPermissions())
	{
		SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		return;
	}

	data.SetOpcode(SMSG_GROUP_INVITE);
	data << uint8(1);
	data << GetPlayer()->GetName();
	data << uint32(0);
	data << uint8(0);
	data << uint32(0);
	player->GetSession()->SendPacket(&data);

	SendPartyCommandResult(GetPlayer(), 0, membername, ERR_PARTY_NO_ERROR);

	// 16/08/06 - change to guid to prevent very unlikely event of a crash in deny, etc
	player->SetInviter(_player->GetLowGUID());
}

///////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_CANCEL:
///////////////////////////////////////////////////////////////
void WorldSession::HandleGroupCancelOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	DEBUG_LOG( "WORLD"," got CMSG_GROUP_CANCEL." );
}

////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_ACCEPT:
////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupAcceptOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;

	uint32 serverid; // Sent as of 3.3 multiserver parties
	recv_data >> serverid;

	Player* player = objmgr.GetPlayer(_player->GetInviter());
	if(!player)
		return;

	player->SetInviter(0);
	_player->SetInviter(0);

	Group *grp = player->GetGroup();

	if(grp)
	{
		if(grp->AddMember(_player->m_playerInfo))
		{
			if(grp->GetLeader()->m_loggedInPlayer)
			{
				_player->iInstanceType = grp->GetLeader()->m_loggedInPlayer->iInstanceType;
				_player->iRaidType = grp->GetLeader()->m_loggedInPlayer->iRaidType;
			}

			_player->GetSession()->OutPacket(MSG_SET_DUNGEON_DIFFICULTY, 4, &_player->iInstanceType);
			_player->GetSession()->OutPacket(MSG_SET_RAID_DIFFICULTY, 4, &_player->iRaidType);
			return;
		}
		else { return; }
	}

	// If we're this far, it means we have no existing group, and have to make one.
	grp = new Group(true);
	grp->AddMember(player->m_playerInfo);		// add the inviter first, therefore he is the leader
	grp->AddMember(_player->m_playerInfo);		// add us.
	grp->SetDifficulty(player->iInstanceType);	// Set our instance difficulty.
	grp->SetRaidDifficulty(player->iRaidType);	// Set our raid difficulty.
	_player->iInstanceType = player->iInstanceType;
	_player->iRaidType = player->iRaidType;
	_player->GetSession()->OutPacket(MSG_SET_DUNGEON_DIFFICULTY, 4, &player->iInstanceType);
	_player->GetSession()->OutPacket(MSG_SET_RAID_DIFFICULTY, 4, &player->iRaidType);
}

///////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_DECLINE:
//////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupDeclineOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	WorldPacket data(SMSG_GROUP_DECLINE, 100);

	Player* player = objmgr.GetPlayer(_player->GetInviter());
	if(!player) return;

	data << GetPlayer()->GetName();

	player->GetSession()->SendPacket( &data );
	player->SetInviter(0);
	_player->SetInviter(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_UNINVITE:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupUninviteOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 1);
	std::string membername;
	Player* player;
	PlayerInfo * info;

	recv_data >> membername;

	player = objmgr.GetPlayer(membername.c_str(), false);
	info = objmgr.GetPlayerInfoByName(membername.c_str());
	_player->GroupUninvite(player, info);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_UNINVITE_GUID:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupUninviteGUIDOpcode( WorldPacket & recv_data )
{
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 1);
	std::string membername;
	Player* player;
	PlayerInfo * info;
	uint64 guid;
	recv_data >> guid;

	player = objmgr.GetPlayer(guid);
	info = objmgr.GetPlayerInfo(guid);
	_player->GroupUninvite(player, info);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_SET_LEADER:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupSetLeaderOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	// important note _player->GetName() can be wrong.
	CHECK_PACKET_SIZE(recv_data, 1);
	WorldPacket data;
	uint64 MemberGuid;
	Player* player;

	recv_data >> MemberGuid;

	player = objmgr.GetPlayer((uint32)MemberGuid);

	if ( player == NULL )
	{
		//SendPartyCommandResult(_player, 0, membername, ERR_PARTY_CANNOT_FIND);
		SendPartyCommandResult(_player, 0, _player->GetName(), ERR_PARTY_CANNOT_FIND);
		return;
	}

	if(!_player->IsGroupLeader())
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	if(player->GetGroup() != _player->GetGroup())
	{
		//SendPartyCommandResult(_player, 0, membername, ERR_PARTY_IS_NOT_IN_YOUR_PARTY);
		SendPartyCommandResult(_player, 0, _player->GetName(), ERR_PARTY_IS_NOT_IN_YOUR_PARTY);
		return;
	}

	Group *pGroup = _player->GetGroup();
	if(pGroup)
		pGroup->SetLeader(player,false);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_GROUP_DISBAND:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleGroupDisbandOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;

	Group* pGroup = _player->GetGroup();
	if(!pGroup)
		return;

	if(pGroup->HasFlag(GROUP_FLAG_BATTLEGROUND_GROUP))
		return;

	//Update the group list
	pGroup->SendNullUpdate( _player );

	pGroup->RemovePlayer(_player->m_playerInfo);
}

//////////////////////////////////////////////////////////////////////////////////////////
///This function handles CMSG_LOOT_METHOD:
//////////////////////////////////////////////////////////////////////////////////////////
void WorldSession::HandleLootMethodOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 16);
	uint32 lootMethod;
	uint64 lootMaster;
	uint32 threshold;

	recv_data >> lootMethod >> lootMaster >>threshold;

	if(!_player->IsGroupLeader())
	{
		SendPartyCommandResult(_player, 0, "", ERR_PARTY_YOU_ARE_NOT_LEADER);
		return;
	}

	Group* pGroup = _player->GetGroup();
	if( pGroup != NULL)
	{
		Player* pLooter = objmgr.GetPlayer(uint32(lootMaster)) ? objmgr.GetPlayer(uint32(lootMaster)) : _player;
		pGroup->SetLooter(pLooter , lootMethod, threshold );
	}
}

void WorldSession::HandleMinimapPingOpcode( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN;
	CHECK_PACKET_SIZE(recv_data, 8);
	if(!_player->InGroup())
	return;
	Group * party= _player->GetGroup();
	if(!party)return;

	float x,y;
	recv_data >> x >>y;
	WorldPacket data;
	data.SetOpcode(MSG_MINIMAP_PING);
	data << _player->GetGUID();
	data << x << y;
	party->SendPacketToAllButOne(&data, _player);
}

void WorldSession::HandleSetPlayerIconOpcode(WorldPacket& recv_data)
{
	CHECK_INWORLD_RETURN;

	Group * pGroup = _player->GetGroup();
	if(pGroup == NULL)
		return;

	uint64 guid;
	uint8 icon;

	recv_data >> icon;
	if(icon == 0xFF)
	{
		// client request
		WorldPacket data(MSG_RAID_TARGET_UPDATE, 73);
		data << uint8(1);
		for(uint8 i = 0; i < 8; i++)
			data << i << pGroup->m_targetIcons[i];

		SendPacket(&data);
	}
	else if(_player->IsGroupLeader())
	{
		recv_data >> guid;
		if(icon > 7)
			icon = 7;	// whoops, buffer overflow :p

		// Null last icon
		for(uint8 i = 0; i < 8; i++)
		{
			if( pGroup->m_targetIcons[i] == guid )
			{
				WorldPacket data(MSG_RAID_TARGET_UPDATE, 10);
				data << uint8(0);
				data << uint64(0);
				data << uint8(i);
				data << uint64(0);
				pGroup->SendPacketToAll(&data);

				pGroup->m_targetIcons[i] = 0;
				break;
			}
		}

		// setting icon
		WorldPacket data(MSG_RAID_TARGET_UPDATE, 10);
		data << uint8(0) << uint64(GetPlayer()->GetGUID()) << icon << guid;
		pGroup->SendPacketToAll(&data);

		pGroup->m_targetIcons[icon] = guid;
	}
}

void WorldSession::SendPartyCommandResult(Player* pPlayer, uint32 p1, std::string name, uint32 err)
{
	CHECK_INWORLD_RETURN;
	// if error message do not work, please sniff it and leave me a message
	if(pPlayer)
	{
		WorldPacket data(SMSG_PARTY_COMMAND_RESULT, name.length()+4+4+4);
		data << uint32(p1);
		if(!name.length())
			data << uint8(0);
		else
			data << name.c_str();
		data << uint32(err);
		data << uint32(0);
		pPlayer->GetSession()->SendPacket(&data);
	}
}
