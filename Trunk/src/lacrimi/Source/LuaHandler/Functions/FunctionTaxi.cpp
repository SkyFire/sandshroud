/*
 * Lacrimi Scripts Copyright 2010 - 2011
 *
 * ############################################################
 * # ##            #       ####### ####### ##    #    #    ## #
 * # ##           ###      ##      ##   ## ##   ###  ###   ## #
 * # ##          ## ##     ##      ##   ## ##   ###  ###   ## #
 * # ##         #######    ##      ####### ##  ## #### ##  ## #
 * # ##        ##     ##   ##      #####   ##  ## #### ##  ## #
 * # ##       ##       ##  ##      ##  ##  ## ##   ##   ## ## #
 * # ####### ##         ## ####### ##   ## ## ##   ##   ## ## #
 * # :::::::.::.........::.:::::::.::...::.::.::...::...::.:: #
 * ############################################################
 *
 */

#include "LacrimiStdAfx.h"

int LuaTaxi_CreateTaxi(lua_State * L, TaxiPath * tp)
{
	TaxiPath * ntp = new TaxiPath();
	Lunar<TaxiPath>::push(L, ntp);
	return 1;
}

int LuaTaxi_GetNodeCount(lua_State * L, TaxiPath * tp)
{
	if (!tp) return 0;
	lua_pushinteger(L, tp->GetNodeCount());
	return 1;
}

int LuaTaxi_AddPathNode(lua_State * L, TaxiPath * tp)
{
	if (!tp) return 0;
	uint32 mapid = luaL_checkint(L, 1);
	float x = CHECK_FLOAT(L, 2);
	float y = CHECK_FLOAT(L, 3);
	float z = CHECK_FLOAT(L, 4);
	uint32 index = luaL_optnumber(L, 5, tp->GetNodeCount());

	TaxiPathNode* tpn = new TaxiPathNode();
	tpn->mapid = mapid;
	tpn->x = x;
	tpn->y = y;
	tpn->z = z;
	tp->AddPathNode(index, tpn);
	return 0;
}

int LuaTaxi_GetId(lua_State * L, TaxiPath * tp)
{
	if (!tp) return 0;
	lua_pushinteger(L, tp->GetID());
	return 1;
}

int LuaTaxi_GetObjectType(lua_State * L, TaxiPath * tp)
{
	if (!tp) 
	{ 
		lua_pushnil(L); 
		return 1; 
	}
	lua_pushstring(L, "TaxiPath");
	return 1;
}