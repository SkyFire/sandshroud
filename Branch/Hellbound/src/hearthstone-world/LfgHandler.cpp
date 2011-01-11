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

void WorldSession::HandleLFDPlrLockOpcode( WorldPacket& recv_data )
{
/*	// Crow: Confirmed structure below
	WorldPacket data(SMSG_LFD_PLAYER_LOCK_INFO_RESPONSE, 400);
	uint8 cnt = 1;
	data << cnt;
	for(uint8 i = 0; i < cnt; i++)
	{
		data << uint32(0) << uint8(0);
		data << uint64(0) << uint64(0);

		uint8 cnt2 = 3;
		data << cnt2;
		for(uint8 i = 0; i < cnt2; i++)
		{
			data << uint64(6) << uint32(7);
		}
	}
	uint32 cnt3 = 2;
	data << cnt3;
	for(uint32 i = 0; i < cnt3; i++)
	{
		data << uint32(8) << uint32(9);
	}
	SendPacket(&data);*/
}

void WorldSession::HandleLFDPartyLockOpcode( WorldPacket& recv_data )
{
	// Crow: Confirmed structure below
/*	WorldPacket data(SMSG_LFD_PARTY_LOCK_INFO_UPDATE, 400);
	uint8 cnt = 2;
	data << uint8(cnt);
	for(uint8 i = 0; i < cnt; i++)
	{
		data << uint64(49426 << 2);
	}
	SendPacket(&data);*/
}
