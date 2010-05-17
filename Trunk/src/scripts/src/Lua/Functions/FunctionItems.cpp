#include "StdAfx.h"
#include "../LUAEngine.h"

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

int luaItem_GetGUID(lua_State * L, Item* ptr)
{
	// Crow: HyperSniper
	if(!ptr)
		return 0;

	lua_pushinteger(L,int(ptr->GetGUID()));
	return 1;
}
