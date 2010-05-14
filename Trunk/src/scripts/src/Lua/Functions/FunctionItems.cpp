#include "StdAfx.h"
#include "../LUAEngine.h"


/////////////////////////////////////////////////////
////////////////UNIT COMMANDS////////////////////////
/////////////////////////////////////////////////////
// Item Gossip Functions
int luaItem_GossipCreateMenu(lua_State * L, Item * ptr)
{
	int text_id = luaL_checkint(L, 1);
	Unit* target = Lunar<Unit>::check(L, 2);
	int autosend = luaL_checkint(L, 3);

	Player* plr = (Player*)target;

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

	Menu->AddItem(icon, menu_text, IntId);
	return 1;
}

int luaItem_GossipSendMenu(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	Menu->SendTo(plr);
	return 1;
}

int luaItem_GossipComplete(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	plr->Gossip_Complete();
	return 1;
}

int luaItem_GossipSendPOI(lua_State * L, Item * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	int icon = luaL_checkint(L, 4);
	int flags = luaL_checkint(L, 5);
	int data = luaL_checkint(L, 6);
	const char * name = luaL_checkstring(L, 7);

	plr->Gossip_SendPOI(x, y, icon, flags, data, name);
	return 1;
}
