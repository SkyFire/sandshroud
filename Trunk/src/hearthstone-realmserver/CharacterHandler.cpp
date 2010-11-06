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
	WorldPacket data(SMSG_CHARACTER_LOGIN_FAILED, 30);
	LocationVector LoginCoord;
	Instance * dest;
	ASSERT(!m_currentPlayer);
	uint64 guid;
	pck >> guid;

	if(sClientMgr.GetRPlayer((uint32)guid) != NULL)
	{
		data << uint8(CHAR_LOGIN_DUPLICATE_CHARACTER);
		SendPacket(&data);
		return;
	}

	m_currentPlayer = sClientMgr.CreateRPlayer((uint32)guid);

	/* Load player data */
	QueryResult * result = CharacterDatabase.Query("SELECT acct, name, level, guild_data.guildid, \
			positionX, positionY, zoneId, mapId, race, class, gender, \
			instance_id, entrypointmap, entrypointx, entrypointy, entrypointz, \
			entrypointo FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE guid = %u", guid);
	if(result)
	{
		Field * f = result->Fetch();
		m_currentPlayer->AccountId = f[0].GetUInt32();
		m_currentPlayer->Name = f[1].GetString();
		m_currentPlayer->Level = f[2].GetUInt32();
		m_currentPlayer->GuildId = f[3].GetUInt32();
		m_currentPlayer->PositionX = f[4].GetFloat();
		m_currentPlayer->PositionY = f[5].GetFloat();
		m_currentPlayer->ZoneId = f[6].GetUInt32();
		m_currentPlayer->MapId = f[7].GetUInt32();
		m_currentPlayer->Race = f[8].GetUInt8();
		m_currentPlayer->Class = f[9].GetUInt8();
		m_currentPlayer->Gender = f[10].GetUInt8();
		m_currentPlayer->Latency = m_latency;
		m_currentPlayer->GMPermissions = m_GMPermissions;
		m_currentPlayer->Account_Flags = m_accountFlags;
		m_currentPlayer->InstanceId = f[11].GetUInt32();
		m_currentPlayer->RecoveryMapId = f[12].GetUInt32();
		m_currentPlayer->RecoveryPosition.ChangeCoords(f[13].GetFloat(), f[14].GetFloat(), f[15].GetFloat(), f[16].GetFloat());
		delete result;
	}
	else
	{
		data << uint8(CHAR_LOGIN_NO_CHARACTER);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	if(IS_MAIN_MAP(m_currentPlayer->MapId))
	{
		/* we're on a continent, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);		
	}
	else
	{
		/* we're in an instanced map, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByInstanceId(m_currentPlayer->InstanceId);

		if(!dest)
		{
			/* our instance has been deleted or no longer valid */
			m_currentPlayer->MapId = m_currentPlayer->RecoveryMapId;
			LoginCoord = m_currentPlayer->RecoveryPosition;

			/* obtain instance */
			dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);
			if(dest)
			{
				data.SetOpcode(SMSG_NEW_WORLD);
				data << m_currentPlayer->MapId << m_currentPlayer->RecoveryPosition << float(0);
				SendPacket(&data);
				data.clear();
			}
		}
	}

	if(!dest ||	!dest->Server)		// Shouldn't happen
	{
		/* world server is down */
		data << uint8(CHAR_LOGIN_NO_WORLD);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	/* log the player into that WS */
	data.SetOpcode(ISMSG_PLAYER_LOGIN);

	/* append info */
	data << uint32(guid) << uint32(dest->MapId) << uint32(dest->InstanceId);

	/* append the account information */
	data << uint32(m_accountId) << uint32(m_accountFlags) << uint32(m_sessionId)
		<< m_GMPermissions << m_accountName;

	AccountDataEntry* acd = NULL;
	for(uint8 i = 0; i < 8; i++)
	{
		acd = GetAccountData(i);
		if(acd && acd->sz)
			data << acd->sz << acd->data;
		else
			data << uint32(0);
	}

	dest->Server->SendPacket(&data);
	m_nextServer = dest->Server;
}

void Session::HandleCharacterEnum(WorldPacket & pck)
{
	uint32 start_time = getMSTime();
	OUT_DEBUG("CharacterHandler", "Enum Build started at %u.", start_time);

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

	OUT_DEBUG("CharacterHandler", "Enum Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );
}

void Session::HandleCharacterCreate(WorldPacket & pck)
{
	std::string name;
	uint8 race, class_;

	pck >> name >> race >> class_;
	pck.rpos(0);

	WorldPacket data(SMSG_CHAR_CREATE, 1);
//	if(!VerifyName(name.c_str(), name.length()))
//	{
//		data << uint8(CHAR_CREATE_NAME_IN_USE);
//		SendPacket(&data);
//		return;
//	}

	if(CharacterDatabase.Query("SELECT name FROM characters WHERE name = '%s'", name.c_str()) != NULL)
	{
		data << uint8(CHAR_CREATE_NAME_IN_USE);
		SendPacket(&data);
		return;
	}

	//reserved for console whisper
	if(HEARTHSTONE_TOLOWER_RETURN(name) == "console")
	{
		data << uint8(CHAR_CREATE_NAME_IN_USE);
		SendPacket(&data);
		return;
	}

	if(sClientMgr.GetRPlayerByName(name.c_str()) != NULL)
	{
		data << uint8(CHAR_CREATE_NAME_IN_USE);
		SendPacket(&data);
		return;
	}

/*	if( class_ == DEATHKNIGHT && (!HasFlag(ACCOUNT_FLAG_XPACK_02) || !CanCreateDeathKnight() ) )
	{
		if(CanCreateDeathKnight())
			data << uint8(CHAR_CREATE_EXPANSION);
		else
			data << uint8(CHAR_CREATE_LEVEL_REQUIREMENT);
		SendPacket(&data);
		return;
	}

	if( (race == RACE_GOBLIN || race == RACE_WORGEN) && !HasFlag(ACCOUNT_FLAG_XPACK_03) )
	{
		data << uint8(CHAR_CREATE_EXPANSION);
		SendPacket(&data);
		return;
	}*/

	QueryResult * result = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(CHAR_NAME_PROFANE);
			SendPacket(&data);
			delete result;
			return;
		}
		delete result;
	}

	int error = sClientMgr.CreateNewPlayer();
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
		default:
			data << uint8(CHAR_CREATE_ERROR);
			break;
		}
		SendPacket(&data);
		return;
	}

	// CHAR_CREATE_SUCCESS
	data << uint8(CHAR_CREATE_SUCCESS);
	SendPacket(&data);

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
}

void Session::HandleCharacterDelete(WorldPacket & pck)
{

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), -1);
}

void Session::HandleCharacterRename(WorldPacket & pck)
{
	uint64 guid;
	string name;
	pck >> guid >> name;

	QueryResult * result = CharacterDatabase.Query("SELECT forced_rename_pending FROM characters WHERE guid = %u AND acct = %u", (uint32)guid, GetAccountId());
	if(result == NULL)
		return;

	WorldPacket data(SMSG_CHAR_RENAME, pck.size() + 1);

	// Check name for rule violation.
	const char * szName=name.c_str();
	for(uint32 x = 0; x < strlen(szName); x++)
	{
		if((int)szName[x] < 65 || ((int)szName[x] > 90 && (int)szName[x] < 97) || (int)szName[x] > 122)
		{
			if((int)szName[x] < 65)
			{
				data << uint8(CHAR_NAME_TOO_SHORT); // Name is too short.
			}
			else if((int)szName[x] > 122) // Name is too long.
			{
				data << uint8(CHAR_NAME_TOO_LONG);
			}
			else
			{
				data << uint8(CHAR_NAME_FAILURE); // No clue.
			}
			data << guid << name;
			SendPacket(&data);
			return;
		}
	}

	QueryResult * result2 = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result2)
	{
		if(result2->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(CHAR_NAME_PROFANE);
			data << guid << name;
			SendPacket(&data);
			delete result2;
			return;
		}
		delete result2;
	}

	// Check if name is in use.
	if(sClientMgr.GetRPlayerByName(name.c_str()) != NULL)
	{
		data << uint8(CHAR_NAME_FAILURE);
		data << guid << name;
		SendPacket(&data);
		return;
	}

	// correct capitalization
	CapitalizeString(name);

	CharacterDatabase.Query("UPDATE characters SET name = \'%s\', forced_rename_pending \
		= 0 WHERE guid = %u AND acct = %u", name.c_str(), (uint32)guid, GetAccountId());

	RPlayerInfo* pi = sClientMgr.GetRPlayer(uint32(guid));
	if(pi != NULL)
		pi->Name = name;

	data << uint8(0) << guid << name;
	SendPacket(&data);
}

void Session::HandleCharacterCustomize(WorldPacket & pck)
{

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
