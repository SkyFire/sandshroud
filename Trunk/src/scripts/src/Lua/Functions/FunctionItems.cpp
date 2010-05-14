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

	if((x == NULL && y == NULL))
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
