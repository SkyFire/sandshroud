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

#include "RStdAfx.h"

WServerHandler WServer::PHandlers[MSGR_NUM_TYPES];

void WServer::InitHandlers()
{
	memset(PHandlers, 0, sizeof(void*) * MSGR_NUM_TYPES);
	PHandlers[CMSGR_REGISTER_WORKER]					= &WServer::HandleRegisterWorker;
	PHandlers[CMSGR_WOW_PACKET]							= &WServer::HandleWoWPacket;
	PHandlers[CMSGR_NUMBER_REQUEST]						= &WServer::HandleNumberRequest;
	PHandlers[CMSGR_PLAYER_LOGIN_RESULT]				= &WServer::HandlePlayerLoginResult;
	PHandlers[CMSGR_PLAYER_LOGOUT]						= &WServer::HandlePlayerLogout;
	PHandlers[CMSGR_TELEPORT_REQUEST]					= &WServer::HandleTeleportRequest;
	PHandlers[CMSGR_ERROR_HANDLER]						= &WServer::HandleError;
	PHandlers[CMSGR_SWITCH_SERVER]						= &WServer::HandleSwitchServer;
	PHandlers[CMSGR_SAVE_ALL_PLAYERS]					= &WServer::HandleSaveAllPlayers;
	PHandlers[CMSGR_TRANSPORTER_MAP_CHANGE]				= &WServer::HandleTransporterMapChange;
	PHandlers[CMSGR_PLAYER_TELEPORT]					= &WServer::HandlePlayerTeleport;
	PHandlers[CMSGR_PLAYER_INFO]						= &WServer::HandlePlayerInfo;
	PHandlers[CMSGR_CHANNEL_ACTION]						= &WServer::HandleChannelAction;
	PHandlers[CMSGR_CHANNEL_UPDATE]						= &WServer::HandleChannelUpdate;
	PHandlers[CMSGR_CHANNEL_LFG_DUNGEON_STATUS_REPLY]	= &WServer::HandleChannelLFGDungeonStatusReply;
}

WServer::WServer(uint32 id, WSSocket * s) : m_id(id), m_socket(s)
{

}

void WServer::HandleRegisterWorker(WorldPacket & pck)
{
	uint32 build;
	map<uint32, uint32> mapset;
	pck >> build >> mapset;

	/* allocate initial instances for this worker */
	if(!mapset.size() || !sClusterMgr.AllocateInitialInstances(this, mapset))
	{
		Log.Notice("SlaveManager", "Rejecting worker server %u", GetID());
		WorldPacket data(SMSGR_ERROR_HANDLER, 20);
		data << uint32(1);
		SendPacket(&data);

		sClusterMgr.RemoveWServer(GetID());
		m_socket->Disconnect();
		return;
	}

	/* send a packed packet of all online players to this server */
	sClientMgr.SendPackedClientInfo(this);
}

void WServer::HandleWoWPacket(WorldPacket & pck)
{
	uint32 sessionid, size;
	uint16 opcode;

	/* get session */
	pck >> sessionid >> opcode >> size;
	DEBUG_LOG("HandleWoWPacket", "Recieved packet %u to forward.", opcode);

	Session * session = sClientMgr.GetSession(sessionid);
	if(!session)
		return;

	/* write it to that session's output buffer */
	WorldSocket * s = session->GetSocket();
	if(s)
		s->QueuePacket(opcode, size, size ? ((const void*)(pck.contents() + 10)) : 0);
}

void WServer::HandleNumberRequest(WorldPacket & pck)
{
	if(pck.size() < 5)
		return; // 1 byte for type, 4 for recall.

	uint8 type;
	pck >> type;
	uint32 number = sClientMgr.GetSharedNumber(type);
	if(number == 0)
		return;

	WorldPacket Response(SMSGR_NUMBER_RESPONSE, pck.size()+4);
	Response << number;
	Response.append(pck.contents(), pck.size());
	SendPacket(&Response);
}

void WServer::HandlePlayerLoginResult(WorldPacket & pck)
{
	uint32 guid, sessionid;
	uint8 result;
	pck >> guid >> sessionid >> result;
	if(result)
	{
		Log.Success("WServer", "Worldserver %u reports successful login of player %u", m_id, guid);
		Session * s = sClientMgr.GetSession(sessionid);
		if(s)
		{
			/* update server */
			s->SetNextServer();
			
			/* pack together a player info packet and distribute it to all the other servers */
			ASSERT(s->GetPlayer());
			
			WorldPacket data(SMSGR_PLAYER_INFO, 100);
			s->GetPlayer()->Pack(data);
			sClusterMgr.DistributePacketToAll(&data, this);
		}
	}
	else
	{
		Log.Error("WServer", "Worldserver %u reports failed login of player %u", m_id, guid);
		Session * s = sClientMgr.GetSession(sessionid);
		if(s)
		{
			s->ClearCurrentPlayer();
			s->ClearServers();
		}

		sClientMgr.DestroyRPlayerInfo(guid);
	}
}

void WServer::HandlePlayerLogout(WorldPacket & pck)
{
	uint32 sessionid, guid;
	pck >> sessionid >> guid;
	RPlayerInfo * pi = sClientMgr.GetRPlayer(guid);
	Session * s = sClientMgr.GetSession(sessionid);
	if(pi && s)
	{
		/* tell all other servers this player has gone offline */
		WorldPacket data(SMSGR_DESTROY_PLAYER_INFO, 4);
		data << sessionid << guid;
		sClusterMgr.DistributePacketToAll(&data, this);

		/* clear the player from the session */
		s->ClearCurrentPlayer();
		s->ClearServers();

		/* destroy the playerinfo struct here */
		sClientMgr.DestroyRPlayerInfo(guid);
	}
}

void WServer::HandleTeleportRequest(WorldPacket & pck)
{
	WorldPacket data(SMSGR_TELEPORT_RESULT, 100);
	RPlayerInfo * pi;
	Session * s;
	Instance * dest;
	uint32 mapid, sessionid, instanceid;

	/* this packet is only used upon changing main maps! */
	pck >> sessionid >> mapid >> instanceid;
	DEBUG_LOG("TeleportRequest", "session %u, mapid %u, instanceid %u", sessionid, mapid, instanceid);

	s = sClientMgr.GetSession(sessionid);
	if(s)
	{
		pi = s->GetPlayer();
		ASSERT(pi);
		dest = sClusterMgr.GetInstanceByMapId(mapid);

		/* server up? */
		if(!dest)
		{
			DEBUG_LOG("TeleportRequest", "INSTANCE_ABORT_NOT_FOUND");
			data.Initialize(SMSG_TRANSFER_ABORTED);
			data << uint32(0x02);	// INSTANCE_ABORT_NOT_FOUND
			s->SendPacket(&data);
		}
		else
		{
			/* server found! */
			LocationVector vec;
			pck >> vec >> vec.o;

			pi->MapId = mapid;
			pi->InstanceId = instanceid;
			pi->PositionX = vec.x;
			pi->PositionY = vec.y;

			if(dest->Server == s->GetServer())
			{
				DEBUG_LOG("TeleportRequest", "intra-server teleport");
				/* we're not changing servers, the new instance is on the same server */
				data << sessionid << uint8(1) << mapid << instanceid << vec << vec.o;
				SendPacket(&data);
			}
			else
			{
				DEBUG_LOG("TeleportRequest", "inter-server teleport");
				/* notify the old server to pack the player info together to send to the new server, and delete the player */
				data << sessionid << uint8(0) << mapid << instanceid << vec << vec.o;
				//cache this to next server and switch servers when were ready :P
				s->SetNextServer(dest->Server);
				SendPacket(&data);
			}

			data.Initialize(SMSGR_PLAYER_INFO);
			pi->Pack(data);
			sClusterMgr.DistributePacketToAll(&data, this);

			data.Initialize(SMSG_NEW_WORLD);
			data << mapid << vec << vec.o;
			s->SendPacket(&data);
		}
	}
}

void WServer::HandleError(WorldPacket & pck)
{
	uint32 sessionid;
	uint8 errorcode;

	pck >> sessionid >> errorcode;

	switch (errorcode)
	{
	case 1: //no session
		sClientMgr.DestroySession(sessionid);
		break;
	}
}

void WServer::HandleSwitchServer(WorldPacket & pck)
{
	uint32 sessionid, guid, mapid, instanceid;
	LocationVector location;
	float o;

	pck >> sessionid >> guid >> mapid >> instanceid >> location >> o;

	Session *s = sClientMgr.GetSession(sessionid);
	if (s == NULL)
		return;

	//so, switch our server, fuck the old one
	s->SetNextServer();

	WorldPacket data;
	data.SetOpcode(SMSGR_PLAYER_LOGIN);
	data << guid << mapid << instanceid;
	data << s->GetAccountId() << s->GetAccountFlags() << s->GetSessionId();
	data << s->GetAccountPermissions() << s->GetAccountName() << s->GetClientBuild();
	AccountDataEntry* acd = NULL;
	for(uint8 i = 0; i < 8; i++)
	{
		acd = s->GetAccountData(i);
		if(acd && acd->sz)
			data << acd->sz << acd->data;
		else
			data << uint32(0);
	}

	s->GetServer()->SendPacket(&data);
}

void WServer::HandleSaveAllPlayers(WorldPacket & pck)
{
	//relay this to all world servers
	WorldPacket data(SMSGR_SAVE_ALL_PLAYERS, 1);
	data << uint8(0);
	sClusterMgr.DistributePacketToAll(&data);
}

void WServer::HandleTransporterMapChange(WorldPacket & pck)
{
	Log.Debug("WServer", "Recieved CMSGR_TRANSPORTER_MAP_CHANGE");
	uint32 transporterentry, mapid, oldmapid;
	float x, y, z;

	pck >> transporterentry >> mapid >> oldmapid >> x >> y >> z;

	Instance* dest = sClusterMgr.GetInstanceByMapId(mapid);

	if (dest == NULL)
		return;

	//ok so we need to send SMSGR_TRANSPORTER_MAP_CHANGE, then world server should SafeTeleport the player across :)
	WorldPacket data(SMSGR_TRANSPORTER_MAP_CHANGE, 20);
	data << transporterentry << mapid << x << y << z;
	dest->Server->SendPacket(&data);
}

void WServer::HandlePlayerTeleport(WorldPacket & pck)
{
	//this is used to teleport to another player (gm appear/summon)
	uint8 result, method;
	uint32 sessionid, mapid, instanceid;
	LocationVector location;
	std::string player_name;
	uint32 player_guid;

	pck >> result >> method >> sessionid >> mapid >> instanceid >> location;

	Session* s=sClientMgr.GetSession(sessionid);
	if (s == NULL)
		return;

	RPlayerInfo* rp = NULL;
	if (result == 2)
	{
		//this is a callback, reserved
	}
	else if (result == 1)
	{
		pck >> player_name;
		rp = sClientMgr.GetRPlayerByName(player_name.c_str());
	}
	else if (result == 0)
	{
		pck >> player_guid;
		rp = sClientMgr.GetRPlayer(player_guid);
	}

	if (rp == NULL)
		return;

	Session* s2 = sClientMgr.GetSessionByRPInfo(rp);

	if (s2 == NULL)
		return;

	//send the info to the target's server, target's server will either handle it, or send info back (for say an appear command)
	WorldPacket data(SMSGR_PLAYER_TELEPORT);
	data << uint8(2) << method << sessionid << mapid << instanceid << location << s2->GetSessionId();
	s->GetServer()->SendPacket(&data);
}

void WServer::HandleCreatePlayerResult(WorldPacket & pck)
{
	uint32 accountid;
	uint8 result;
	pck >> accountid >> result;

	//ok, we need session by account id... gay
	Session* s = sClientMgr.GetSessionByAccountId(accountid);
	if (s == NULL)
		return;

	s->GetSocket()->OutPacket(SMSG_CHAR_CREATE, 1, &result);
}

void WServer::HandlePlayerInfo(WorldPacket & pck)
{
	uint32 guid;
	pck >> guid;
	RPlayerInfo * pRPlayer = sClientMgr.GetRPlayer(guid);
	ASSERT(pRPlayer);

	pRPlayer->Unpack(pck);
}

void WServer::Update()
{
	WorldPacket * pck;
	uint16 opcode;
	while((pck = m_recvQueue.Pop()))
	{
		opcode = pck->GetOpcode();
		if(opcode < MSGR_NUM_TYPES && WServer::PHandlers[opcode] != 0)
			(this->*WServer::PHandlers[opcode])(*pck);
		else
			Log.Error("WServer", "Unhandled packet %u\n", opcode);
	}
}

void WServer::HandleChannelAction(WorldPacket & pck)
{
/*	uint8 action;
	pck >> action;

	switch(action)
	{
	case CHANNEL_JOIN:
		{
			uint32 guid;
			uint32 cid;
			RPlayerInfo * pRPlayer;
			Channel * pChannel;

			pck >> guid;
			pck >> cid;

			pRPlayer = sClientMgr.GetRPlayer(guid);
			if(pRPlayer == NULL)
				return;

			pChannel = channelmgr.GetChannel(cid);
			if( pChannel != NULL )
			{
				pChannel->AttemptJoin(pRPlayer, "");
			}
			break;

		}
	case CHANNEL_PART:
		{
			uint32 guid;
			uint32 cid;
			RPlayerInfo * pRPlayer;
			Channel * pChannel;

			pck >> guid;
			pck >> cid;

			pRPlayer = sClientMgr.GetRPlayer(guid);
			if(pRPlayer == NULL)
				return;

			pChannel = channelmgr.GetChannel(cid);
			if( pChannel != NULL )
			{
				pChannel->Part(pRPlayer, true);
			}
			break;
		}
	case CHANNEL_SAY:
		{
			std::string channelname;
			uint32 guid;
			std::string message;
			uint32 for_gm_guid;
			bool forced;
			Channel * pChannel;
			RPlayerInfo * pRPlayer;
			RPlayerInfo * pGM_RPlayer = NULL;

			pck >> channelname;
			pck >> guid;
			pck >> message;
			pck >> for_gm_guid;
			pck >> forced;

			pRPlayer = sClientMgr.GetRPlayer(guid);
			if(pRPlayer == NULL)
				return;

			if(for_gm_guid)
				pGM_RPlayer = sClientMgr.GetRPlayer(for_gm_guid);

			pChannel = channelmgr.GetChannel(channelname.c_str(), pRPlayer);
			if(pChannel)
				pChannel->Say(pRPlayer, message.c_str(), for_gm_guid ? pGM_RPlayer : NULL, forced);
			break;
		}
	default:
		{
			DEBUG_LOG("WServer", "HandleChannelAction opcode, unhandled action %u", action);
			return;
		}
	}*/
}

void WServer::HandleChannelUpdate(WorldPacket & pck)
{
/*	uint8 updatetype;
	pck >> updatetype;

	DEBUG_LOG("WServer", "Received CMSGR_CHANNEL_UPDATE opcode, update type %u", updatetype);

	uint32 guid;
	std::vector<uint32> channels;
	RPlayerInfo * plr = NULL;
	pck >> guid;
	DEBUG_LOG("WServer", "Received CMSGR_CHANNEL_UPDATE opcode, from player guid %u", guid);
	pck >> channels;
	plr = sClientMgr.GetRPlayer(guid);
	if(plr == NULL)
		return;

	std::vector<uint32>::iterator itr = channels.begin();
	Channel *pChannel;
	uint32 cid;

	switch(updatetype)
	{
	case UPDATE_CHANNELS_ON_ZONE_CHANGE:
		{
			DEBUG_LOG("WServer", "HandleChannelUpdate opcode, UPDATE_CHANNELS_ON_ZONE_CHANGE");
			uint32 areaid, zoneid, mapid;
			pck >> areaid;
			pck >> zoneid;
			pck >> mapid;

			vector<uint32>::iterator i;
			string channelname,pass, AreaName;

			//Whats this?
			if(mapid==450)
				zoneid = 2917;
			if(mapid==449)
				zoneid = 2918;

			AreaTable * at = dbcArea.LookupEntry(areaid);
			AreaTable * at2 = dbcArea.LookupEntry(zoneid);

			//Check for instances?
			if(!zoneid || zoneid == 0xFFFF)
			{
				MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(mapid);
				if(mapid != 1 && mapid != 0 && mapid != 530)
					AreaName = pMapinfo->name;
				else
					return;//How'd we get here?
			}
			else
			{
				AreaName = at2->name;
				if(AreaName.length() < 2)
				{
					MapInfo *pMapinfo = WorldMapInfoStorage.LookupEntry(mapid);
					AreaName = pMapinfo->name;
				}
			}

			for(; itr != channels.end(); itr++)
			{
				cid = *itr;

				pChannel = channelmgr.GetChannel(cid);
				if( pChannel == NULL )
				{
					continue;
				}

				if(!pChannel->m_general || pChannel->m_name == "LookingForGroup")//Not an updatable channel.
					continue;

				if( strstr(pChannel->m_name.c_str(), "General") )
					channelname = "General";
				else if( strstr(pChannel->m_name.c_str(), "Trade") )
					channelname = "Trade";
				else if( strstr(pChannel->m_name.c_str(), "LocalDefense") )
					channelname = "LocalDefense";
				else if( strstr(pChannel->m_name.c_str(), "GuildRecruitment") )
					channelname = "GuildRecruitment";
				else
					continue;//Those 4 are the only ones we want updated.

				channelname += " - ";
				if( at && (strstr(pChannel->m_name.c_str(), "Trade") || strstr(pChannel->m_name.c_str(), "GuildRecruitment")) && at->AreaFlags &0x200 /*AREA_CITY* / )
				{
					channelname += "City";
				}
				else
					channelname += AreaName;

				Channel * chn = channelmgr.GetCreateChannel(channelname.c_str(), plr, pChannel->m_channelId);
				if( !chn->HasMember(plr) )
				{
					pChannel->Part(plr, true);
					chn->AttemptJoin(plr, pass.c_str());
				}
			};
			break;
		}
	case PART_ALL_CHANNELS:
		{
			for(; itr != channels.end(); itr++)
			{
				cid = *itr;

				pChannel = channelmgr.GetChannel(cid);
				if( pChannel != NULL )
				{
					pChannel->Part(plr, true);
				}
			}
			break;
		}
	case JOIN_ALL_CHANNELS:
		{
			for(; itr != channels.end(); itr++)
			{
				cid = *itr;

				pChannel = channelmgr.GetChannel(cid);
				if( pChannel != NULL )
				{
					pChannel->AttemptJoin(plr, "");
				}
			}
			break;
		}
	default:
		{
			DEBUG_LOG("WServer", "HandleChannelUpdate opcode, unhandled update type %u", updatetype);
			return;
		}
	}*/
}

void WServer::HandleChannelLFGDungeonStatusReply(WorldPacket& pck)
{
/*	uint8 i = 0;
	pck >> i;

	if(i == 3)
		return;

	uint32 guid;	
	pck >> guid;

	RPlayerInfo * pRplayer;
	pRplayer = sClientMgr.GetRPlayer(guid);
	if(!pRplayer)
		return;

	string channelname,pass;
	uint32 dbc_id = 0;
	uint16 unk;		// some sort of channel type?
	Channel * chn;

	pck >> dbc_id >> unk;
	pck >> channelname;
	pck >> pass;

	chn = channelmgr.GetCreateChannel(channelname.c_str(), pRplayer, dbc_id);
	if(chn == NULL)
		return;

	chn->AttemptJoin(pRplayer, pass.c_str());
	DEBUG_LOG("LfgChannelJoin", "%s, unk %u", channelname.c_str(), unk);*/
}
