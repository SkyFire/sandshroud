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

void Session::HandlePlayerLogin(WorldPacket & pck)
{
	WorldPacket data(SMSG_CHARACTER_LOGIN_FAILED, 1);
	MasterServer* MasterControl = sClusterMgr.GetMasterServer();
	if(MasterControl == NULL)
	{
		data << uint8(CHAR_LOGIN_NO_WORLD);
		SendPacket(&data);
		return;
	}

	uint64 guid;
	pck >> guid;

	if(guid == NULL)
	{
		data << uint8(CHAR_LOGIN_NO_CHARACTER);
		SendPacket(&data);
		return;
	}

	uint32 playerguid = guid;
	Log.Notice("", "Building player login packet for %u/%u", guid, playerguid);
	data.Initialize(IMSG_PLAYER_TRANSFER);

	/* append info */
	data << playerguid << uint32(m_accountId) << uint32(m_accountFlags) << uint32(m_sessionId)
		<< m_GMPermissions << m_accountName << m_ClientBuild;

	AccountDataEntry* acd = NULL;
	for(uint8 i = 0; i < 8; i++)
	{
		acd = GetAccountData(i);
		if(acd && acd->sz)
			data << acd->sz << acd->data;
		else
			data << uint32(0);
	}

	MasterControl->SendPacket(&data);
}

void Session::HandlePlayerLogoutRequest(WorldPacket & pck)
{

}

void Session::HandleCharacterEnum(WorldPacket & pck)
{
	SendChars();
}

void Session::SendChars()
{
	uint32 start_time = getMSTime();
	OUT_DEBUG("Enum Build started at %u.", start_time);

	// loading characters
	QueryResult* result = CharacterDatabase.Query("SELECT guid, level, race, class, gender, \
		bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, \
		restState, deathstate, forced_rename_pending, player_flags, guild_data.guildid, \
		customizable FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid \
		WHERE acct=%u ORDER BY guid ASC LIMIT 10", GetAccountId());
	uint8 num = 0;

	//Erm, reset it here in case player deleted his DK.
	m_hasDeathKnight= false;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data((result ? result->GetRowCount() * 200 : 1));	

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
	data.SetOpcode(SMSG_CHAR_ENUM);

	data << num;
	if( result )
	{
		CreatureInfo *info = NULL;
		player_item items[19];
		ItemPrototype * proto;
//		player_item bags[4];
		QueryResult * res;
		Field *fields;
		int8 slot;
		int8 containerslot;
		uint8 Class;
		uint8 race;
		uint32 i;
		uint32 bytes2;
		uint32 flags;
		uint32 banned;
		uint64 guid;
		do
		{
			fields = result->Fetch();
			guid = fields[0].GetUInt32();
			bytes2 = fields[6].GetUInt32();
			Class = fields[3].GetUInt8();
			flags = fields[17].GetUInt32();
			race = fields[2].GetUInt8();

			/* build character enum, w0000t :p */
			data << fields[0].GetUInt64();		// guid
			data << fields[7].GetString();		// name
			data << race;						// race
			data << Class;						// class
			data << fields[4].GetUInt8();		// gender
			data << fields[5].GetUInt32();		// PLAYER_BYTES
			data << uint8(bytes2 & 0xFF);		// facial hair
			data << fields[1].GetUInt8();		// Level
			data << fields[12].GetUInt32();		// zoneid
			data << fields[11].GetUInt32();		// Mapid
			data << fields[8].GetFloat();		// X
			data << fields[9].GetFloat();		// Y
			data << fields[10].GetFloat();		// Z
			data << fields[18].GetUInt32();		// GuildID

			if( fields[1].GetUInt8() > m_highestLevel )
				m_highestLevel = fields[1].GetUInt8();

			if( Class == 6 )
				m_hasDeathKnight = true;

			banned = fields[13].GetUInt32();
			if(banned && (banned<10 || banned > (uint32)UNIXTIME))
				data << uint32(0x01A04040);
			else
			{
				if(fields[16].GetUInt32() != 0)
					data << uint32(0x00A04342);
				else if(fields[15].GetUInt32() != 0)
					data << (uint32)8704; // Dead (displaying as Ghost)
				else
					data << uint32(1);		// alive
			}

			data << uint32(fields[19].GetUInt8());	// Character Customization
			data << fields[14].GetUInt8();			// Rest State

			if( Class == WARLOCK || Class == HUNTER )
			{
				res = CharacterDatabase.Query("SELECT entry FROM playerpets WHERE ownerguid="I64FMTD" AND ( active MOD 10 ) =1", guid);

				if(res)
				{
					info = CreatureNameStorage.LookupEntry(res->Fetch()[0].GetUInt32());
					delete res;
				}
				else
					info = NULL;
			}
			else
				info = NULL;

			if(info)  //PET INFO uint32 displayid,	uint32 level,		 uint32 familyid
				data << uint32(info->Male_DisplayID) << uint32(10) << uint32(info->Family);
			else
				data << uint32(0) << uint32(0) << uint32(0);

			res = CharacterDatabase.Query("SELECT containerslot, slot, entry, enchantments FROM playeritems WHERE ownerguid=%u", GUID_LOPART(guid));

			uint32 enchantid;
			EnchantEntry * enc;
			memset(items, 0, sizeof(player_item) * 19);
			if(res)
			{
				do
				{
					containerslot = res->Fetch()[0].GetInt8();
					slot = res->Fetch()[1].GetInt8();
					if( containerslot == -1 && slot < EQUIPMENT_SLOT_END && slot >= 0 )
					{
						proto = ItemPrototypeStorage.LookupEntry(res->Fetch()[2].GetUInt32());
						if(proto)
						{
							// slot0 = head, slot14 = cloak 0x400 = no helm, 0x800 = no cloak
							if(!(slot == 0 && (flags & (uint32)0x400) != 0) && !(slot == 14 && (flags & (uint32)0x800) != 0))
							{
								items[slot].displayid = proto->DisplayInfoID;
								items[slot].invtype = proto->InventoryType;
								if( slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND )
								{
									// get enchant visual ID
									const char * enchant_field = res->Fetch()[3].GetString();
									if( sscanf( enchant_field , "%u,0,0;" , (unsigned int *)&enchantid ) == 1 && enchantid > 0 )
									{
										enc = dbcEnchant.LookupEntry( enchantid );
										if( enc != NULL )
											items[slot].enchantment = enc->visual;
										else
											items[slot].enchantment = 0;;
									}
								}
							}
						}
					}
				} while(res->NextRow());
				delete res;
			}

			for( i = 0; i < EQUIPMENT_SLOT_END; i++ )
				data << items[i].displayid << items[i].invtype << uint32(items[i].enchantment);

			for( i = 0; i < 4; i++)
				data << uint32(0)/*bags[i].displayid*/ << uint8(18) << uint32(0);
			//			Displayid						  // Bag	  // Enchant

			num++;
		}
		while( result->NextRow() );
		delete result;
	}

	data.put<uint8>(0, num);
	OUT_DEBUG("Enum Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );
}

void Session::HandleCharacterCreate(WorldPacket & pck)
{
	WorldPacket data(SMSG_CHAR_CREATE, 1);
	int error = sClientMgr.CreateNewPlayer(this, pck);
	if(error > 0)
	{
		switch(error)
		{
		case 1:
			data << uint8(CHAR_CREATE_DISABLED);
			break;
		case 2:
			data << uint8(CHAR_CREATE_FAILED);
			break;
		case 3: // Our char is being created via worldserver
			sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
			return;
			break;
		default:
			data << uint8(CHAR_CREATE_ERROR);
			break;
		}
		SendPacket(&data);
		return;
	}
}

void Session::HandleCharacterDelete(WorldPacket & pck)
{
	WorldPacket data(SMSG_CHAR_DELETE, 1);
	int error = sClientMgr.DeleteCharacter(this, pck);
	if(error)
	{
		data << uint8(CHAR_DELETE_FAILED);
		SendPacket(&data);
		return;
	}

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), -1);
}

void Session::HandleCharacterRename(WorldPacket & pck)
{
	uint64 guid;
	string name;
	pck >> guid >> name;
	pck.rpos(0);

	int error = sClientMgr.RenameCharacter(this, pck);
	if(error)
	{
		WorldPacket data(SMSG_CHAR_RENAME, pck.size() + 1);
		data << uint8(CHAR_NAME_FAILURE);
		data << guid << name;
		SendPacket(&data);
	}
}

void Session::HandleCharacterCustomize(WorldPacket & pck)
{

}

void Session::HandleUpdateAccountData(WorldPacket & pck)
{
	uint32 uiID;
	pck >> uiID;

	if(uiID > 8)
	{
		// Shit..
		sLog.outString("WARNING: Accountdata > 8 (%d) was requested to be updated by account %s(%u)!", uiID, GetAccountName().c_str(), GetAccountId());
		return;
	}

	uint32 _time;
	pck >> _time;

	uint32 uiDecompressedSize;
	pck >> uiDecompressedSize;
	uLongf uid = uiDecompressedSize;

	// client wants to 'erase' current entries
	if(uiDecompressedSize == 0)
	{
		SKIP_READ_PACKET(pck);
		SetAccountData(uiID, NULL, false,0);
		return;
	}

	if(uiDecompressedSize > 100000)
	{
		SKIP_READ_PACKET(pck); // Spam cleanup.
		Disconnect();
		return;
	}

	if(uiDecompressedSize >= 65534)
	{
		SKIP_READ_PACKET(pck); // Spam cleanup.
		// BLOB fields can't handle any more than this.
		return;
	}

	size_t ReceivedPackedSize = pck.size() - 12;
	char* data = new char[uiDecompressedSize+1];
	memset(data, 0, uiDecompressedSize+1);	/* fix umr here */

	if(uiDecompressedSize > ReceivedPackedSize) // if packed is compressed
	{
		int32 ZlibResult;

		ZlibResult = uncompress((uint8*)data, &uid, pck.contents() + 12, (uLong)ReceivedPackedSize);

		switch (ZlibResult)
		{
		case Z_OK:				  //0 no error decompression is OK
			SetAccountData(uiID, data, false, uiDecompressedSize);
			OUT_DEBUG("WORLD: Successfully decompressed account data %d for %s, and updated storage array.", uiID, GetAccountName().c_str());
			break;

		case Z_ERRNO:				//-1
		case Z_STREAM_ERROR:		//-2
		case Z_DATA_ERROR:			//-3
		case Z_MEM_ERROR:			//-4
		case Z_BUF_ERROR:			//-5
		case Z_VERSION_ERROR:		//-6
		{
			delete [] data;
			sLog.outString("WORLD WARNING: Decompression of account data %d for %s FAILED.", uiID, GetAccountName().c_str());
			break;
		}

		default:
			delete [] data;
			sLog.outString("WORLD WARNING: Decompression gave a unknown error: %x, of account data %d for %s FAILED.", ZlibResult, uiID, GetAccountName().c_str());
			break;
		}
	}
	else
	{
		memcpy(data, pck.contents() + 12, uiDecompressedSize);
		SetAccountData(uiID, data, false, uiDecompressedSize);
	}SKIP_READ_PACKET(pck); // Spam cleanup for packet size checker... Because who cares about this dataz
}

void Session::HandleRequestAccountData(WorldPacket & pck)
{
	uint32 id;
	pck >> id;

	if(id > 8)
	{
		// Shit..
		sLog.outString("WARNING: Accountdata > 8 (%d) was requested by account %s(%u)!", id, GetAccountName().c_str(), GetAccountId());
		return;
	}

	AccountDataEntry* res = GetAccountData(id);
	uLongf destSize = compressBound(res->sz);
	ByteBuffer bbuff;
	bbuff.resize(destSize);

	if(res->sz && compress(const_cast<uint8*>(bbuff.contents()), &destSize, (uint8*)res->data, res->sz) != Z_OK)
	{
		OUT_DEBUG("Error while compressing ACCOUNT_DATA");
		SKIP_READ_PACKET(pck);
		return;
	}

	WorldPacket data;
	data.SetOpcode(SMSG_UPDATE_ACCOUNT_DATA);
	data << uint64(0);
	data << id;
	// if res does not exists if ID == 7 and if there is no data send 0
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

void Session::HandleReadyForAccountDataTimes(WorldPacket & pck)
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

void Session::HandleEnableMicrophoneOpcode(WorldPacket & pck)
{
	SKIP_READ_PACKET(pck);
}

void Session::HandleVoiceChatQueryOpcode(WorldPacket & pck)
{
	SKIP_READ_PACKET(pck);
}

void Session::HandleRealmSplitQuery(WorldPacket & pck)
{
	uint32 v;
	pck >> v;

	WorldPacket data(SMSG_REALM_SPLIT, 16);
	data << v << uint32(0);
	data << "01/01/01";
	SendPacket(&data);
}
