/*
 * Sun++ Scripts for Sandshroud MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2009-2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ITEM_FUNCTIONS_H
#define ITEM_FUNCTIONS_H

/////////////////////////////////////////////////////
////////////////ITEM COMMANDS////////////////////////
/////////////////////////////////////////////////////

// Item Gossip Functions
int luaItem_GossipCreateMenu(lua_State * L, Item * ptr)
{

	int text_id = luaL_checkint(L, 1);
	Unit* target = Lunar<Unit>::check(L, 2);
	if(target == NULL)
	{
#ifdef PRINTERRORS
		printf("Incorrect target in luaItem_GossipCreateMenu, script: %s\n", "GetName"); // TODO
#endif
		return 0;
	}

	int autosend = luaL_checkint(L, 3);
	Player* plr = TO_PLAYER(target);

	objmgr.CreateGossipMenuForPlayer(&Menu, ptr->GetGUID(), text_id, plr);
	if(autosend)
		Menu->SendTo(plr);
	return 1;
}

int luaItem_GossipMenuAddItem(lua_State * L, Item * ptr)
{
	int icon = luaL_checkint(L, 1);
	const char * menu_text = luaL_checkstring(L, 2);
	int IntId = luaL_checkint(L, 3);

	if((menu_text == NULL) || Menu == NULL)
		return 0;

	Menu->AddItem(icon, menu_text, IntId);
	return 1;
}

int luaItem_GossipSendMenu(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	if(target == NULL)
	{
#ifdef PRINTERRORS
		printf("Incorrect target in luaItem_GossipSendMenu, script: %s\n", "GetName"); // TODO
#endif
		return 0;
	}

	Player * plr = TO_PLAYER(target);
	Menu->SendTo(plr);
	return 1;
}

int luaItem_GossipComplete(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	if(target == NULL)
	{
#ifdef PRINTERRORS
		printf("Incorrect target in luaItem_GossipComplete, script: %s\n", "GetName"); // TODO
#endif
		return 0;
	}

	Player * plr = TO_PLAYER(target);
	plr->Gossip_Complete();
	return 1;
}

int luaItem_GossipSendPOI(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	if(target == NULL)
	{
#ifdef PRINTERRORS
		printf("Incorrect target in luaItem_GossipSendPOI, script: %s\n", "GetName"); // TODO
#endif
		return 0;
	}

	Player * plr = TO_PLAYER(target);
	float x = float(luaL_checknumber(L, 2));
	float y = float(luaL_checknumber(L, 3));
	int icon = luaL_checkint(L, 4);
	int flags = luaL_checkint(L, 5);
	int data = luaL_checkint(L, 6);
	const char * name = luaL_checkstring(L, 7);

	if(!x || !y)
		return 0;

	plr->Gossip_SendPOI(x, y, icon, flags, data, name);
	return 1;
}

int luaItem_PerformWorldDBQuery(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	const char * qStr = luaL_checkstring(L,1);
	if(!qStr)
		return 0;

	QueryResult* result = WorldDatabase.Query(qStr);
	if(result)
		lua_pushnumber(L, lua_Number(1)); // We have data.
	else
		lua_pushnumber(L, lua_Number(2)); // No data.
	return 1;
}

int luaItem_PerformCharDBQuery(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	const char * qStr = luaL_checkstring(L,1);
	if(!qStr)
		return 0;

	QueryResult * result = CharacterDatabase.Query(qStr);
	if(result)
		lua_pushnumber(L, lua_Number(1)); // We have data.
	else
		lua_pushnumber(L, lua_Number(2)); // No data.
	return 1;
}

int luaItem_GetOwner(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	Player * owner = ptr->GetOwner();
	if(owner != NULL)
		Lunar<Unit>::push(L,owner,false);
	else
		lua_pushnil(L);
	return 1;
}

int luaItem_AddEnchantment(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	int entry = luaL_checkint(L,1);
	int duration = luaL_checkint(L,2);
	bool permanent = (duration == 0) ? true : false;
	bool temp = (luaL_checkint(L, 3) == 1) ? true : false;

	EnchantEntry *eentry = dbcEnchant.LookupEntry( entry );
	if(eentry == NULL)
		return 0;

	lua_pushinteger(L, ptr->AddEnchantment(eentry, duration, permanent, true, temp));
	return 1;
}

int luaItem_RemoveEnchantment(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	int slot = luaL_checkint(L,1);
	bool temp = (luaL_checkint(L,2) == 1 ? true : false);

	if (slot == -1)	ptr->RemoveAllEnchantments(temp);
	else if (slot == -2) ptr->RemoveProfessionEnchant();
	else if (slot == -3) ptr->RemoveSocketBonusEnchant();
	else if (slot >= 0) ptr->RemoveEnchantment(slot);

	return 1;
}

int luaItem_GetEntryId(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetEntry());
	return 1;
}

int luaItem_GetName(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	ItemPrototype * proto = NULL;
	if (!ptr || !(proto = ptr->GetProto()))
		return 0;

	lua_pushstring(L, proto->Name1);
	return 1;
}

int luaItem_GetSpellId(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	ItemPrototype * proto = NULL;
	uint32 index = luaL_checkint(L, 1);
	if (!ptr || index < 0 || index > 5 || !(proto = ptr->GetProto()))
		return 0;

	lua_pushnumber(L, proto->Spells[index].Id);
	return 1;
}

int luaItem_GetSpellTrigger(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	ItemPrototype * proto = NULL;
	uint32 index = luaL_checkint(L, 1);
	if (!ptr || index < 0 || index > 5 || !(proto = ptr->GetProto()))
		return 0;

	/*	
		USE				= 0,
		ON_EQUIP		= 1,
		CHANCE_ON_HIT	= 2,
		SOULSTONE		= 4,
		LEARNING		= 6,
	*/
	lua_pushnumber(L, proto->Spells[index].Trigger);
	return 1;
}

int luaItem_GetGUID(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	if(!ptr)
		return 0;

	lua_pushinteger(L,int(ptr->GetGUID()));
	return 1;
}

int luaItem_AddLoot(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper++
	uint32 itemid = luaL_checkint(L,1);
	uint32 mincount = luaL_checkint(L,2);
	uint32 maxcount = luaL_checkint(L,3);
	uint32 ffa_loot = luaL_checkint(L,4);
	bool perm = ((luaL_checkint(L,5) == 1) ? true : false);
	if(perm)
	{
		double chance = luaL_checknumber(L, 6);
		QueryResult* result = WorldDatabase.Query("SELECT * FROM itemloot WHERE entryid = %u, itemid = %u", ptr->GetEntry(), itemid);
		if(!result)
		{
			WorldDatabase.Execute("REPLACE INTO itemloot VALUES (%u, %u, %f, %u, %u, %u)", ptr->GetEntry(), itemid, float(chance), mincount, maxcount, ffa_loot);
			lootmgr.AddLoot(ptr->GetLoot(),itemid,mincount,maxcount,ffa_loot);
		}
	}
	else
		lootmgr.AddLoot(ptr->GetLoot(),itemid,mincount,maxcount,ffa_loot);


	return 1;
}

int luaItem_SetByteValue(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	uint32 index = luaL_checkint(L,1);
	uint32 index1 = luaL_checkint(L,2);
	uint8 value = luaL_checkint(L,3);
	ptr->SetByte(index,index1,value);
	return 1;
}

int luaItem_GetByteValue(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	uint32 index = luaL_checkint(L,1);
	uint32 index1 = luaL_checkint(L,2);
	lua_pushinteger(L,ptr->GetByte(index,index1));
	return 1;
}

int luaItem_GetItemLevel(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetProto()->ItemLevel);
	return 1;
}

int luaItem_GetRequiredLevel(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetProto()->RequiredLevel);
	return 1;
}

int luaItem_GetBuyPrice(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetProto()->BuyPrice);
	return 1;
}

int luaItem_GetSellPrice(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetProto()->SellPrice);
	return 1;
}

int luaItem_RepairItem(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	ptr->SetDurabilityToMax();
	return 1;
}

int luaItem_GetMaxDurability(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetDurabilityMax());
	return 1;
}

int luaItem_GetDurability(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	lua_pushnumber(L, ptr->GetDurability());
	return 1;
}

int luaItem_HasEnchantment(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	if(ptr->HasEnchantments())
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

int luaItem_ModifyEnchantmentTime(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	uint32 slot = luaL_checkint(L, 1);
	uint32 duration = luaL_checkint(L, 2);

	ptr->ModifyEnchantmentTime(slot, duration);
	return 1;
}

int luaItem_SetStackCount(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	uint32 count = luaL_checkint(L, 1);
	if(!count || count > 1000)
		return 0;

	ptr->SetUInt32Value(ITEM_FIELD_STACK_COUNT, count);
	return 1;
}

int luaItem_HasFlag(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	uint32 index = luaL_checkint(L,1);
	uint32 flag = luaL_checkint(L,2);
	lua_pushboolean(L, ptr->HasFlag(index,flag) ? 1 : 0);
	return 1;
}

int luaItem_IsSoulbound(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	ptr->IsSoulbound() ? lua_pushboolean(L,1) : lua_pushboolean(L,0);
	return 1;
}

int luaItem_IsAccountbound(lua_State * L, Item * ptr)
{
	if(!ptr || !ptr->GetProto())
		return 0;

	(ptr->GetProto()->ScalingStatsEntry > 0) ? lua_pushboolean(L,1) : lua_pushboolean(L,0);
	return 1;
}

int luaItem_IsContainer(lua_State * L, Item * ptr)
{
	if(!ptr)
		return 0;

	ptr->IsContainer() ? lua_pushboolean(L,1) : lua_pushboolean(L,0);
	return 1;
}

int luaItem_GetContainerItemCount(lua_State * L, Item * ptr)
{
	if(!ptr || !ptr->IsContainer() || !ptr->GetProto())
		return 0;

	uint32 itemid = (uint32)luaL_checknumber(L,1);
	if (!itemid)
		return 0;

	Container * pCont = TO_CONTAINER(ptr);
	int16 TotalSlots = pCont->GetProto()->ContainerSlots;
	if(TotalSlots < 1)
		return 0;

	int cnt = 0;
	for (int16 i = 0; i < TotalSlots; i++)
	{
		Item *item = pCont->GetItem(i);
		if (item)
		{
			if(item->GetEntry() == itemid && item->wrapped_item_id == 0)
			{
				cnt += item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) ? item->GetUInt32Value(ITEM_FIELD_STACK_COUNT) : 1; 
			}
		}
	}
	lua_pushinteger(L, cnt);
	return 1;
}

int luaItem_GetEquippedSlot(lua_State * L, Item * ptr)
{
	if(!ptr || !ptr->GetOwner() || !ptr->GetOwner()->GetItemInterface())
		return 0;

	lua_pushinteger(L, ptr->GetOwner()->GetItemInterface()->GetInventorySlotById(ptr->GetEntry()));
	return 1;
}

int luaItem_GetObjectType(lua_State * L, Item * ptr)
{
	if(!ptr)
	{
		lua_pushnil(L);
		return 0;
	}

	lua_pushstring(L, "Item");
	return 1;
}

int luaItem_Remove(lua_State * L, Item * ptr)
{
	if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_ITEM)
		return 0;

	ptr->RemoveFromWorld();
	return 0;
}

int luaItem_Create(lua_State * L, Item * ptr)
{
	uint32 id = (uint32)luaL_checknumber(L,1);
	uint32 stackcount = (uint32)luaL_checknumber(L,2);
	Item * pItem = objmgr.CreateItem(id, NULL);
	if (!pItem)
		return 0;

	pItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, stackcount);
	pItem->SaveToDB(0, 0, true, NULL);
	Lunar<Item>::push(L,pItem);
	return 1;
}

#endif // ITEM_FUNCTIONS_H
