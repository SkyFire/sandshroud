/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"

initialiseSingleton( ChannelMgr );

void WorldSession::HandleChannelJoin(WorldPacket& recvPacket)
{
	string channelname,pass;
	uint32 dbc_id = 0;
	uint16 crap;		// crap = some sort of channel type?
	uint32 i;
	Channel * chn;

	recvPacket >> dbc_id >> crap;
	recvPacket >> channelname;
	recvPacket >> pass;

	if(!stricmp(channelname.c_str(), "LookingForGroup") && !sWorld.m_lfgForNonLfg)
	{
		// make sure we have lfg dungeons
		for(i = 0; i < 3; ++i)
		{
			if(_player->LfgDungeonId[i] != 0)
				break;
		}

		if(i == 3)
			return;		// don't join lfg
	}

	if( sWorld.GmClientChannel.size() && !stricmp(sWorld.GmClientChannel.c_str(), channelname.c_str()) && !GetPermissionCount())
		return;
	
	chn = channelmgr.GetCreateChannel(channelname.c_str(), _player, dbc_id);
	if(chn == NULL)
		return;

	chn->AttemptJoin(_player, pass.c_str());
	DEBUG_LOG("ChannelJoin", "%s", channelname.c_str());
}

void WorldSession::HandleChannelLeave(WorldPacket& recvPacket)
{
	if(_player)
	{
		string channelname;
		uint32 code = 0;
		Channel * chn;

		recvPacket >> code;
		recvPacket >> channelname;

		chn = channelmgr.GetChannel(channelname.c_str(), _player);
		if( chn != NULL )
			chn->Part(_player, false);
	}
	else
		SKIP_READ_PACKET(recvPacket);
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
	string channelname;
	Channel * chn;

	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	recvPacket >> pass;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if( chn != NULL )
		chn->Password(_player, pass.c_str());
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->SetOwner(_player, plr);
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if( chn != NULL )
		chn->GetOwner(_player);
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->GiveModerator(_player, plr);
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->TakeModerator(_player, plr);
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Mute(_player, plr);
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Unmute(_player, plr);
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Invite(_player, plr);
}
void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Kick(_player, plr, false);
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	Player* plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if( chn != NULL && plr != NULL )
		chn->Kick(_player, plr, true);
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
	string channelname,newp;
	Channel * chn;
	PlayerInfo * plr;

	recvPacket >> channelname;
	recvPacket >> newp;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayerInfoByName(newp.c_str());
	if( chn != NULL && plr != NULL )
		chn->Unban(_player, plr);
}

void WorldSession::HandleChannelAnnounce(WorldPacket& recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->Announce(_player);
}

void WorldSession::HandleChannelModerate(WorldPacket& recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->Moderate(_player);
}

void WorldSession::HandleChannelRosterQuery(WorldPacket & recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if( chn != NULL )
		chn->List(_player);
}

void WorldSession::HandleChannelNumMembersQuery(WorldPacket & recvPacket)
{
	string channel_name;
	WorldPacket data(SMSG_CHANNEL_MEMBER_COUNT, recvPacket.size() + 4);
	Channel *chn;
	recvPacket >> channel_name;
	chn = channelmgr.GetChannel(channel_name.c_str(), _player);
	if( chn != NULL )
	{
		data << channel_name;
		data << uint8(chn->m_flags);
		data << uint32(chn->GetNumMembers());
		SendPacket(&data);
	}
}
