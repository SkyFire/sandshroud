/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 * This file is for saving commented out code that might be useful later.
 *
 */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From: MiscHandler: HandleAcknowledgementOpcodes
/*	uint16 opcode = recv_data.GetOpcode();
	if (opcode == CMSG_FORCE_RUN_SPEED_CHANGE_ACK)
	{
 
		uint64 GUID;
		uint32 Flags, unk0, unk1, d_time;
		float X, Y, Z, O, speed;
		
		recv_data >> GUID;
		recv_data >> unk0 >> Flags;
		if (Flags & (0x2000 | 0x6000))			 //0x2000 == jumping  0x6000 == Falling
		{
			uint32 unk2, unk3, unk4, unk5;
			float OldSpeed;

			recv_data >> d_time;
			recv_data >> X >> Y >> Z >> O;
			recv_data >> unk2 >> unk3;						  //no idea, maybe unk2 = flags2
			recv_data >> unk4 >> unk5;						  //no idea
			recv_data >> OldSpeed >> speed;
		}
		else													//single check
		{
			recv_data >> d_time;
			recv_data >> X >> Y >> Z >> O;
			recv_data >> unk1 >> speed;
		}
		
		// if its not good kick player???
		if (_player->GetPlayerSpeed() != speed)
		{
			sLog.outError("SpeedChange player:%s is NOT correct, its set to: %f he seems to be cheating",_player->GetName(), speed);
		}
	}*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From: MiscHandler: HandleAcknowledgementOpcodes
   /* uint16 opcode = recv_data.GetOpcode();
	std::stringstream ss;
	ss << "Received ";
	switch( opcode )
	{
	case CMSG_MOVE_FEATHER_FALL_ACK:			ss << "Move_Feather_Fall"; break;
	case CMSG_MOVE_WATER_WALK_ACK:			  ss << "Move_Water_Walk"; break;
	case CMSG_MOVE_KNOCK_BACK_ACK:			  ss << "Move_Knock_Back"; break;
	case CMSG_MOVE_HOVER_ACK:				   ss << "Move_Hover"; break;
	case CMSG_FORCE_WALK_SPEED_CHANGE_ACK:	  ss << "Force_Walk_Speed_Change"; break;
	case CMSG_FORCE_SWIM_SPEED_CHANGE_ACK:	  ss << "Force_Swim_Speed_Change"; break;
	case CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK: ss << "Force_Swim_Back_Speed_Change"; break;
	case CMSG_FORCE_TURN_RATE_CHANGE_ACK:	   ss << "Force_Turn_Rate_Change"; break;
	case CMSG_FORCE_RUN_SPEED_CHANGE_ACK:	   ss << "Force_Run_Speed_Change"; break;
	case CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK:  ss << "Force_Run_Back_Speed_Change"; break;
	case CMSG_FORCE_MOVE_ROOT_ACK:			  ss << "Force_Move_Root"; break;
	case CMSG_FORCE_MOVE_UNROOT_ACK:			ss << "Force_Move_Unroot"; break;
	default:									ss << "Unknown"; break;
	}
	ss << " Acknowledgement. PktSize: " << recv_data.size();
	OUT_DEBUG( ss.str().c_str() );*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From: Group.cpp: HandlePartyMemberStatsOpcode

	/*
	Player* plr = _player->GetMapMgr()->GetPlayer((uint32)guid);

	if(!_player->GetGroup() || !plr)
		return;

	WorldPacket data(200);
	if(!_player->GetGroup()->HasMember(plr))
		return;			// invalid player

	if(_player->IsVisible(plr))
		return;

	_player->GetGroup()->UpdateOutOfRangePlayer(plr, GROUP_UPDATE_TYPE_FULL_CREATE | GROUP_UPDATE_TYPE_FULL_REQUEST_REPLY, false, &data);
	data.SetOpcode(SMSG_PARTY_MEMBER_STATS_FULL);
	SendPacket(&data);