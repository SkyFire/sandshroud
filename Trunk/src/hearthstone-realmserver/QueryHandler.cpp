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

//////////////////////////////////////////////////////////////
/// This function handles CMSG_CREATURE_QUERY:
//////////////////////////////////////////////////////////////
void Session::HandleCreatureQueryOpcode(WorldPacket & pck)
{
	WorldPacket data(SMSG_CREATURE_QUERY_RESPONSE, 150);
	CreatureInfo * ci;
	uint32 entry;
	pck >> entry;

	if(entry == 300000)
	{
		data << (uint32)entry;
		data << "WayPoint";
		data << uint8(0) << uint8(0) << uint8(0);
		data << "Level is WayPoint ID";
		for(uint32 i = 0; i < 8;i++)
		{
			data << uint32(0);
		}
		data << uint8(0);  
	}
	else
	{
		ci = CreatureNameStorage.LookupEntry(entry);
		if(ci == NULL)
			return;
		DEBUG_LOG("World", "CMSG_CREATURE_QUERY '%s'", ci->Name);

		data << uint32(entry);
		data << ci->Name;
		data << uint8(0) << uint8(0) << uint8(0);	// 3 CStrings
		data << ci->SubName;
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
	}

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_GAMEOBJECT_QUERY:
//////////////////////////////////////////////////////////////
void Session::HandleGameObjectQueryOpcode(WorldPacket & pck)
{
	WorldPacket data(SMSG_GAMEOBJECT_QUERY_RESPONSE, 300);
	uint32 entryID;
	GameObjectInfo *goinfo;

	pck >> entryID;

	DEBUG_LOG("World", "CMSG_GAMEOBJECT_QUERY '%u'", entryID);

	goinfo = GameObjectNameStorage.LookupEntry(entryID);
	if(goinfo == 0)
		return;

	data << entryID;
	data << goinfo->Type;
	data << goinfo->DisplayID;
	data << goinfo->Name;
	data << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0) << uint8(0);   // new string in 1.12
	data << goinfo->SpellFocus;
	data << goinfo->sound1;
	data << goinfo->sound2;
	data << goinfo->sound3;
	data << goinfo->sound4;
	data << goinfo->sound5;
	data << goinfo->sound6;
	data << goinfo->sound7;
	data << goinfo->sound8;
	data << goinfo->sound9;
	data << goinfo->Unknown1;
	data << goinfo->Unknown2;
	data << goinfo->Unknown3;
	data << goinfo->Unknown4;
	data << goinfo->Unknown5;
	data << goinfo->Unknown6;
	data << goinfo->Unknown7;
	data << goinfo->Unknown8;
	data << goinfo->Unknown9;
	data << goinfo->Unknown10;
	data << goinfo->Unknown11;
	data << goinfo->Unknown12;
	data << goinfo->Unknown13;
	data << goinfo->Unknown14;
	/*data << goinfo->UnknownFloat;
	data << goinfo->Unknown15;
	data << goinfo->Unknown16;
	data << goinfo->Unknown17;
	data << goinfo->Unknown18;*/

	SendPacket( &data );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_ITEM_QUERY_SINGLE:
//////////////////////////////////////////////////////////////
void Session::HandleItemQuerySingleOpcode(WorldPacket & pck)
{
	int i;
	int32 statcount = 0;
	uint32 itemid = 0;
	pck >> itemid;

	DEBUG_LOG("World", "CMSG_ITEM_QUERY_SINGLE for item id %d",	itemid );

	ItemPrototype *itemProto = ItemPrototypeStorage.LookupEntry(itemid);
	if(!itemProto)
		return;

	for(int i = 0; i < 10; i++)
	{
		if(itemProto->Stats[i].Type)
			statcount = i + 1; // Crow Classic.
	}

	pck << itemProto->ItemId;
	pck << itemProto->Class;
	pck << itemProto->SubClass;
	pck << itemProto->unknown_bc;
	pck << itemProto->Name1;
	pck << uint8(0) << uint8(0) << uint8(0); // name 2,3,4
	pck << itemProto->DisplayInfoID;
	pck << itemProto->Quality;
	pck << itemProto->Flags;
	pck << uint32(0);
	pck << itemProto->BuyPrice;
	pck << itemProto->SellPrice;
	pck << itemProto->InventoryType;
	pck << itemProto->AllowableClass;
	pck << itemProto->AllowableRace;
	pck << itemProto->ItemLevel;
	pck << itemProto->RequiredLevel;
	pck << itemProto->RequiredSkill;
	pck << itemProto->RequiredSkillRank;
	pck << itemProto->RequiredSpell;
	pck << itemProto->RequiredPlayerRank1;
	pck << itemProto->RequiredPlayerRank2;
	pck << itemProto->RequiredFaction;
	pck << itemProto->RequiredFactionStanding;
	pck << itemProto->Unique;
	pck << itemProto->MaxCount;
	pck << itemProto->ContainerSlots;
	pck << uint32(statcount);
	for(i = 0; i < statcount; i++)
	{
		pck << itemProto->Stats[i].Type;
		pck << itemProto->Stats[i].Value;
	}
	pck << uint32(itemProto->ScalingStatsEntry);
	pck << uint32(itemProto->ScalingStatsFlag);
	for(i = 0; i < 2; i++)
	{
		pck << itemProto->Damage[i].Min;
		pck << itemProto->Damage[i].Max;
		pck << itemProto->Damage[i].Type;
	}
	// 7 resistances
	pck << itemProto->Armor;
	pck << itemProto->HolyRes;
	pck << itemProto->FireRes;
	pck << itemProto->NatureRes;
	pck << itemProto->FrostRes;
	pck << itemProto->ShadowRes;
	pck << itemProto->ArcaneRes;
	pck << itemProto->Delay;
	pck << itemProto->AmmoType;
	pck << itemProto->Range;
	for(i = 0; i < 5; i++)
	{
		pck << itemProto->Spells[i].Id;
		pck << itemProto->Spells[i].Trigger;
		pck << itemProto->Spells[i].Charges;
		pck << itemProto->Spells[i].Cooldown;
		pck << itemProto->Spells[i].Category;
		pck << itemProto->Spells[i].CategoryCooldown;
	}
	pck << itemProto->Bonding;
	pck << itemProto->Description;
	pck << itemProto->PageId;
	pck << itemProto->PageLanguage;
	pck << itemProto->PageMaterial;
	pck << itemProto->QuestId;
	pck << itemProto->LockId;
	pck << int32(itemProto->LockMaterial);
	pck << itemProto->SheathId;
	pck << itemProto->RandomPropId;
	pck << itemProto->RandomSuffixId;
	pck << itemProto->Block;
	pck << itemProto->ItemSet;
	pck << itemProto->MaxDurability;
	pck << itemProto->ZoneNameID;
	pck << itemProto->MapID;
	pck << itemProto->BagFamily;
	pck << itemProto->TotemCategory;
	// 3 sockets
	for(i = 0; i < 3; i++)
	{
		pck << itemProto->Sockets[i].SocketColor;
		pck << itemProto->Sockets[i].Unk;
	}
	pck << itemProto->SocketBonus;
	pck << itemProto->GemProperties;
	pck << itemProto->DisenchantReqSkill;
	pck << float(itemProto->ArmorDamageModifier);	// should be a float?
	pck << uint32(0);								// 2.4.2 Item duration in seconds
	pck << uint32(0);								// ItemLimitCategory
	pck << uint32(0);								// HolidayId.
	SendPacket( &pck );
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_PAGE_TEXT_QUERY
/*/////////////////////////////////////////////////////////////
void Session::HandlePageTextQueryOpcode( WorldPacket & recv_data )
{
	//CHECK_PACKET_SIZE(recv_data, 4);
	uint32 pageid = 0;
	uint8 buffer[10000];
	StackPacket data(SMSG_PAGE_TEXT_QUERY_RESPONSE,buffer, 10000);
	recv_data >> pageid;

	while(pageid)
	{
		ItemPage * page = ItemPageStorage.LookupEntry(pageid);
		if(page == NULL)
			return;

		LocalizedItemPage * lpi = (language>0) ? sLocalizationMgr.GetLocalizedItemPage(pageid,language):NULL;
		data.Clear();
		data << pageid;
		if(lpi)
			data.Write((uint8*)lpi->Text, strlen(lpi->Text) + 1);
		else
			data.Write((uint8*)page->text, strlen(page->text) + 1);

		data << page->next_page;
		pageid = page->next_page;
		SendPacket(&data);
	}
}*/

//////////////////////////////////////////////////////////////
/// This function handles CMSG_QUERY_TIME:
//////////////////////////////////////////////////////////////
void Session::HandleQueryTimeOpcode( WorldPacket & recv_data )
{
	uint32 t = (uint32)UNIXTIME;
	OutPacket(SMSG_QUERY_TIME_RESPONSE, 4, &t);
}

//////////////////////////////////////////////////////////////
/// This function handles CMSG_NAME_QUERY:
//////////////////////////////////////////////////////////////
void Session::HandleNameQueryOpcode( WorldPacket & recv_data )
{
	//CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;

	RPlayerInfo *pn = sClientMgr.GetRPlayer( (uint32)guid );
	if(pn == NULL)
		return;

	DEBUG_LOG("WorldSession","Received CMSG_NAME_QUERY for: %s", pn->Name );
	WorldPacket data(SMSG_NAME_QUERY_RESPONSE, 5000);
	data << WoWGuid(guid);
	data << uint8(0);
	data << pn->Name;
//	if(blablabla)
//		data << std::string("");
//	else
		data << uint8(0);
	data << uint8(pn->Race);
	data << uint8(pn->Gender);
	data << uint8(pn->Class);
	data << uint8(0);
	SendPacket( &data );
}
