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
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	PlayerInfo *pn = objmgr.GetPlayerInfo( (uint32)guid );
	if(pn == NULL)
		return;

	DEBUG_LOG("WorldSession","Received CMSG_NAME_QUERY for: %s", pn->name );
	WorldPacket data(SMSG_NAME_QUERY_RESPONSE, 10000);
	data << WoWGuid(guid);
	data << uint8(0);
	data << pn->name;
//	if(blablabla)
//		data << std::string("");
//	else
		data << uint8(0);
	data << uint8(pn->race);
	data << uint8(pn->gender);
	data << uint8(pn->cl);
	data << uint8(0);
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void WorldSession::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	uint32 t = (uint32)UNIXTIME;
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCreatureQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	uint32 entry;
	uint64 guid;

	recv_data >> entry;
	recv_data >> guid;

	WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
	if(entry == 300000)
	{
		data << (uint32)entry;
		data << "WayPoint";
		data << uint8(0) << uint8(0) << uint8(0);
		data << "Level is WayPoint ID";
		for(uint32 i = 0; i < 8; i++)
			data << uint32(0);
		data << uint8(0);
		SendPacket( &data );
		return;
	}

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
	if(ci == NULL)
		return;

	LocalizedCreatureName * lcn = (language > 0) ? sLocalizationMgr.GetLocalizedCreatureName(entry, language) : NULL;

	if(lcn)
		DEBUG_LOG("WORLD","HandleCreatureQueryOpcode CMSG_CREATURE_QUERY '%s' (localized to %s)", ci->Name, lcn->Name);
	else
		DEBUG_LOG("WORLD","HandleCreatureQueryOpcode CMSG_CREATURE_QUERY '%s'", ci->Name);
	data << (uint32)entry;
	data << (lcn ? lcn->Name : ci->Name);
	data << uint8(0) << uint8(0) << uint8(0);
	data << (lcn ? lcn->SubName : ci->SubName);
	data << ci->info_str; //!!! this is a string in 2.3.0 Example: stormwind guard has : "Direction"
	data << ci->Flags1;
	data << ci->Type;
	data << ci->Family;
	data << ci->Rank;
	data << ci->Unknown1;
	data << ci->SpellDataID;
	data << ci->Male_DisplayID;
	data << ci->Female_DisplayID;
	data << ci->Male_DisplayID2;
	data << ci->Female_DisplayID2;
	data << ci->unkfloat1;
	data << ci->unkfloat2;
	data << ci->Leader;
	for(uint32 i = 0; i < 6; i++)
		data << uint32(0);	//QuestItems
	data << uint32(0);	// CreatureMovementInfo.dbc
	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleGameObjectQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 12);
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 150);

	uint32 entryID;
	uint64 guid;
	recv_data >> entryID;
	recv_data >> guid;

	DEBUG_LOG("WORLD","HandleGameObjectQueryOpcode CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	GameObjectInfo* goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == NULL)
		return;

	LocalizedGameObjectName * lgn = (language>0) ? sLocalizationMgr.GetLocalizedGameObjectName(entryID, language) : NULL;

	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	data << (lgn ? lgn->Name : goinfo->Name);
	data << uint8(0);
	data << uint8(0);
	data << uint8(0);
	data << goinfo->Icon;
	data << goinfo->CastBarText;
	data << uint8(0);
	for(uint32 d = 0; d < 24; d++)
		data << goinfo->RawData.ListedData[d];
	data << float(1);
	for(uint32 i = 0; i < 6; i++)
		data << uint32(0);			// itemId[6], quest drop

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles MSG_CORPSE_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleCorpseQueryOpcode(WorldPacket &recv_data)
{
	DEBUG_LOG("WORLD","HandleCorpseQueryOpcode Received MSG_CORPSE_QUERY");

	Corpse* pCorpse;
	WorldPacket data(MSG_CORPSE_QUERY, 18);

	if(_player->isDead())
		_player->BuildPlayerRepop();

	pCorpse = objmgr.GetCorpseByOwner(_player->GetLowGUID());
	if(pCorpse)
	{
		MapInfo *pPMapinfo = NULL;
		pPMapinfo = WorldMapInfoStorage.LookupEntry(pCorpse->GetMapId());
		if(pPMapinfo == NULL)
			data.Initialize(MSG_CORPSE_QUERY);

		data << uint8(0x01); //show ?

		if(pPMapinfo != NULL && !(pPMapinfo->type == INSTANCE_NULL || pPMapinfo->type == INSTANCE_PVP))
		{
			data << pPMapinfo->repopmapid; // mapid (that tombstones shown on)
			data << pPMapinfo->repopx;
			data << pPMapinfo->repopy;
			data << pPMapinfo->repopz;
		}
		else
		{
			data << pCorpse->GetMapId(); // mapid (that tombstones shown on)
			data << pCorpse->GetPositionX();
			data << pCorpse->GetPositionY();
			data << pCorpse->GetPositionZ();
		}
		data << pCorpse->GetMapId(); //instance mapid (needs to be same as mapid to be able to recover corpse)

		SendPacket(&data);
	}
}

void WorldSession::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	uint64 itemguid;
	recv_data >> pageid;
	recv_data >> itemguid;

	WorldPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE, 300);
	while(pageid)
	{
		data.clear();
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(page == NULL)
			return;

		LocalizedItemPage * lpi = (language>0) ? sLocalizationMgr.GetLocalizedItemPage(pageid,language):NULL;
		char* text = (lpi ? lpi->Text : page->text);
		if(text == NULL || *text == NULL)
			return;

		data << pageid;
		data << text;
		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_NAME_QUERY:
//////////////////////////////////////////////////////////////
void WorldSession::HandleItemNameQueryOpcode( WorldPacket & recv_data )
{
	CHECK_PACKET_SIZE(recv_data, 4);
	uint32 itemid;
	uint64 guid;
	recv_data >> itemid;
	recv_data >> guid;

	ItemEntry* itemE = dbcItem.LookupEntry(itemid);
	WorldPacket reply(SMSG_ITEM_NAME_QUERY_RESPONSE, 1000);
	ItemPrototype *proto = ItemPrototypeStorage.LookupEntry(itemid);
	LocalizedItem* li = sLocalizationMgr.GetLocalizedItem(itemid, language);

	reply << itemid;
	if(proto)
	{
		reply << (li ? li->Name ? li->Name : proto->Name1 : proto->Name1);
		reply << uint32(proto->InventoryType);
	}
	else
	{
		reply << (li ? li->Name ? li->Name : "Unknown Item" : "Unknown Item");
		reply << (itemE ? itemE->InventoryType : uint32(0));
	}

	SendPacket(&reply);
}

void WorldSession::HandleInrangeQuestgiverQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	WorldPacket data(SMSG_QUESTGIVER_STATUS_MULTIPLE, 1000);
	Object::InRangeSet::iterator itr;
	Creature* pCreature;
	uint32 count = 0;
	data << count;
	for( itr = _player->m_objectsInRange.begin(); itr != _player->m_objectsInRange.end(); itr++ )
	{
		pCreature = TO_CREATURE(*itr);
		if( pCreature->GetTypeId() != TYPEID_UNIT )
			continue;

		if( pCreature->isQuestGiver() )
		{
			data << pCreature->GetGUID();
			data << sQuestMgr.CalcStatus( pCreature, _player );
			++count;
		}
	}

	data.put<uint32>(0, count);
	SendPacket(&data);
}