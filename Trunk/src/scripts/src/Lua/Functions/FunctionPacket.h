/*
 * ArcScript Scripts for BurningEmu By Crow@Sandshroud(WorldX)
 * Copyright (C) 2008-2009 Burning Heavens Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#ifndef _LUAPACKET_H
#define _LUAPACKET_H

#include "StdAfx.h"
#include "LUAEngine.h"

/*
	NORMAL OPERATIONS
	*/
int LuaPacket_CreatePacket(lua_State * L, WorldPacket * packet)
{
	int opcode = luaL_checkint(L,1);
	int size = luaL_checkint(L,2);
	if(opcode >= NUM_MSG_TYPES)
		luaL_error(L,"CreatePacket got opcode %d greater than max opcode %d.",opcode,NUM_MSG_TYPES);
	else
	{
		WorldPacket * npacket = new WorldPacket(opcode,size);
		Lunar<WorldPacket>::push(L,npacket);
	}
	return 1;
}

int LuaPacket_GetOpcode(lua_State *L, WorldPacket * packet)
{
	if(packet == NULL)
		lua_pushnil(L);
	else
		lua_pushinteger(L,packet->GetOpcode());
	return 1;
}

int LuaPacket_GetSize(lua_State *L, WorldPacket * packet)
{
	if(packet == NULL)
		lua_pushnil(L);
	else
		lua_pushinteger(L,packet->size());
	return 1;
}

/*
	READ OPERATIONS
	*/
int LuaPacket_ReadUByte(lua_State * L, WorldPacket * packet)
{
	if(packet == NULL)
		lua_pushnil(L);
	else
	{
		uint8 byte;
		(*packet) >> byte;
		lua_pushinteger(L,byte);
	}
	return 1;
}

int LuaPacket_ReadByte(lua_State * L, WorldPacket * packet)
{
	if(packet == NULL)
		lua_pushnil(L);
	else
	{
		int8 byte;
		(*packet) >> byte;
		lua_pushinteger(L,byte);
	}
	return 1;
}

int LuaPacket_ReadShort(lua_State * L, WorldPacket * packet)
{
	int16 val;
	if(packet != NULL)
	{
		(*packet) >> val;
		lua_pushinteger(L,val);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadUShort(lua_State * L, WorldPacket * packet)
{
	uint16 val;
	if(packet != NULL)
	{
		(*packet) >> val;
		lua_pushinteger(L,val);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadLong(lua_State * L, WorldPacket * packet)
{
	int32 val;
	if(packet != NULL)
	{
		(*packet) >> val;
		lua_pushinteger(L,val);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadULong(lua_State * L, WorldPacket * packet)
{
	uint32 val;
	if(packet != NULL)
	{
		(*packet) >> val;
		lua_pushinteger(L,val);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadFloat(lua_State * L, WorldPacket * packet)
{
	float val;
	if(packet != NULL)
	{
		(*packet) >> val;
		lua_pushnumber(L,val);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadDouble(lua_State * L, WorldPacket * packet)
{
	double val;
	if(packet != NULL)
	{
		(*packet) >> val;
		lua_pushnumber(L,val);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadGUID(lua_State * L, WorldPacket * packet)
{
	uint64 guid;
	if(packet != NULL)
	{
		(*packet) >> guid;
		GuidMgr::push(L,guid);
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadWoWGuid(lua_State * L, WorldPacket * packet)
{
	WoWGuid nGuid;
	if(packet != NULL)
	{
		(*packet) >> nGuid;
		GuidMgr::push(L,nGuid.GetOldGuid());
	}
	else
		lua_pushnil(L);
	return 1;
}

int LuaPacket_ReadString(lua_State * L, WorldPacket * packet)
{
	string str;
	if(packet != NULL)
	{
		(*packet) >> str;
		lua_pushstring(L, str.c_str());
	}
	else
		lua_pushnil(L);
	return 1;
}

/*
	WRITE OPERATIONS
	*/
int LuaPacket_WriteByte(lua_State * L, WorldPacket * packet)
{
	int8 byte = (int8)luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << byte;
	return 1;
}

int LuaPacket_WriteUByte(lua_State * L, WorldPacket * packet)
{
	uint8 byte = (uint8)luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << byte;
	return 1;
}

int LuaPacket_WriteShort(lua_State * L, WorldPacket * packet)
{
	int16 val = (int16)luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << val;
	return 1;
}

int LuaPacket_WriteUShort(lua_State * L, WorldPacket * packet)
{
	uint16 val = (uint16)luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << val;
	return 1;
}

int LuaPacket_WriteLong(lua_State * L, WorldPacket * packet)
{
	int32 val = (int32)luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << val;
	return 1;
}

int LuaPacket_WriteULong(lua_State * L, WorldPacket * packet)
{
	uint32 val = (uint32)luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << val;
	return 1;
}

int LuaPacket_WriteFloat(lua_State * L, WorldPacket * packet)
{
	float val = (float)luaL_checknumber(L,1);
	if (packet != NULL)
		(*packet) << val;
	return 1;
}

int LuaPacket_WriteDouble(lua_State * L, WorldPacket * packet)
{
	double val = luaL_checknumber(L,1);
	if (packet != NULL)
		(*packet) << val;
	return 1;
}

int LuaPacket_WriteGUID(lua_State * L, WorldPacket * packet)
{
	uint64 guid = luaL_checkint(L,1);
	if (packet != NULL)
		(*packet) << guid;
	return 1;
}

int LuaPacket_WriteWoWGuid(lua_State * L, WorldPacket * packet)
{
	Object * target = CHECK_OBJECT(L,1);
	if (packet != NULL)
		(*packet) << target->GetNewGUID();
	return 1;
}

int LuaPacket_WriteString(lua_State * L, WorldPacket * packet)
{
	string str = string(luaL_checkstring(L,1));
	if (packet != NULL)
		(*packet) << str;
	return 1;
}

int LuaPacket_GetObjectType(lua_State * L, WorldPacket * packet)
{
	if (!packet) 
	{ 
		lua_pushnil(L); 
		return 0; 
	}
	lua_pushstring(L, "Packet");
	return 1;
}

#endif


		
