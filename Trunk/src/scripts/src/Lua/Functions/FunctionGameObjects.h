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

#ifndef GAMEOBJECT_FUNCTIONS_H
#define GAMEOBJECT_FUNCTIONS_H

////////////////////////////////////
////////////GAMEOBJECT GOSSIP///////
/////////////////////////////////////
int luaGameObject_GossipCreateMenu(lua_State * L, GameObject * ptr)
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
int luaGameObject_GossipMenuAddItem(lua_State * L, GameObject * ptr)
{
	int icon = luaL_checkint(L, 1);
	const char * menu_text = luaL_checkstring(L, 2);
	int IntId = luaL_checkint(L, 3);

	Menu->AddItem(icon, menu_text, IntId);
	return 1;
}
int luaGameObject_GossipSendMenu(lua_State * L, GameObject * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	Menu->SendTo(plr);
	return 1;
}
int luaGameObject_GossipComplete(lua_State * L, GameObject * ptr)
{
	Unit* target = Lunar<Unit>::check(L, 1);
	Player * plr = (Player*)target;
	plr->Gossip_Complete();
	return 1;
}
int luaGameObject_GossipSendPOI(lua_State * L, GameObject * ptr)
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

///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////GAMEOBJECT'S COMMANDS///////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
/*int luaGameObject_EventCastSpell(lua_State * L, GameObject * ptr)
{
	uint32 guid = luaL_checkint(L,1);
	uint32 sp = luaL_checkint(L,2);
	uint32 delay = luaL_checkint(L,3);
	uint32 repeats = luaL_checkint(L,4);
	if(guid && sp && delay)
	{
		sEventMgr.AddEvent(ptr,&GameObject::EventCastSpell,guid,sp,false,EVENT_GAMEOBJECT_UPDATE,delay,repeats,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
	return 0;
}*/
int luaGameObject_GetMapId(lua_State * L, GameObject * ptr)
{
	if(ptr)
	{
		lua_pushinteger(L,ptr->GetMapId());
	}
	return 1;
}
int luaGameObject_RemoveFromWorld(lua_State * L, GameObject * ptr)
{
	if(ptr)
		ptr->RemoveFromWorld(true);
	return 0;
}

int luaGameObject_GetName(lua_State * L, GameObject * ptr)
{
	if(!ptr || ptr->GetTypeId() != TYPEID_GAMEOBJECT || !ptr->GetInfo())
		return 0;

	lua_pushstring(L, ptr->GetInfo()->Name);
	return 1;
}

int luaGameObject_TeleportPlr(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	Player* target = Lunar<Player>::check(L, 1);
	if(target == NULL)
	{
#ifdef PRINTERRORS
		printf("Incorrect target in luaGameObject_TeleportPlr, script: %s\n", "GetName"); // TODO
#endif
		return 0;
	}

	int mapId = luaL_checkint(L, 2);
	double posX = luaL_checknumber(L, 3);
	double posY = luaL_checknumber(L, 4);
	double posZ = luaL_checknumber(L, 5);
	double DirO;
	if(!(DirO = luaL_checknumber(L, 6)))
		DirO = 0;

	if(!posX || !posY || !posZ)
		return 0;


	target->SafeTeleport(uint32(mapId), 0, float(posX), float(posY), float(posZ), float(DirO));

	return 1;
}

int luaGameObject_GetCreatureNearestCoords(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
    uint32 entryid = luaL_checkint(L,4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(entryid == 0) 
        lua_pushnil(L);
    else
        Lunar<Unit>::push(L,ptr->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(x, y, z, entryid), false);
    return 1;
}

int luaGameObject_GetGameObjectNearestCoords(lua_State *L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
    uint32 entryid = luaL_checkint(L,4);
    float x = (float)luaL_checknumber(L,1);
    float y = (float)luaL_checknumber(L,2);
    float z = (float)luaL_checknumber(L,3);
    if(entryid == 0) 
        lua_pushnil(L);
    else
        Lunar<GameObject>::push(L,ptr->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(x, y, z, entryid), false);
    return 1;
}
int luaGameObject_GetClosestPlayer(lua_State * L, GameObject * ptr)
{
	if(!ptr)
		return 0;

	float dist, d2;
	Player * ret=NULL;

	for(unordered_set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr)
	{
		d2=(*itr)->GetDistanceSq(ptr);
		if(!ret||d2<dist)
		{
			dist=d2;
			ret=*itr;
		}
	}

	if(!ret)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L,TO_UNIT(ret),false);

	return 1;
}
int luaGameObject_GetDistance(lua_State * L, GameObject * ptr)
{
	if(!ptr)
		return 0;
	Unit * target = Lunar<Unit>::check(L, 1);
	lua_pushnumber(L,ptr->GetDistance2dSq(target));
	return 1;
}
int luaGameObject_IsInWorld(lua_State * L, GameObject * ptr)
{
	if (ptr)
		if(ptr->IsInWorld())
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaGameObject_GetZoneId(lua_State *L, GameObject * ptr)
{
	if(!ptr)
		return 0;
	lua_pushinteger(L,(ptr->GetZoneId()));
	return 1;
}
int luaGameObject_PlaySoundToSet(lua_State * L, GameObject * ptr)
{
	if(!ptr) return 0;
	int soundid = luaL_checkint(L,1);
	ptr->PlaySoundToSet(soundid);
	return 0;
}
int luaGameObject_SpawnCreature(lua_State * L, GameObject * ptr)
{
	if(ptr == NULL) return 0;
	uint32 entry_id = luaL_checkint(L, 1);
	float x = (float)luaL_checkint(L, 2);
	float y = (float)luaL_checkint(L, 3);
	float z = (float)luaL_checkint(L, 4);
	float o = (float)luaL_checkint(L, 5);
	uint32 faction = luaL_checkint(L, 6);
	uint32 duration = luaL_checkint(L, 7);

	if( !x || !y || !z || !entry_id || !faction /*|| !duration*/) //Shady: is it really required?
	{
		lua_pushnil(L);
		return 1;
	}
	CreatureProto *p = CreatureProtoStorage.LookupEntry(entry_id);
    
    if(p == NULL)
      return 0;

    Creature *pCreature = ptr->GetMapMgr()->CreateCreature(entry_id);
    pCreature->m_spawn = 0;
    pCreature->Load(p,x,y,z,0);
	if(faction)
		pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE,faction);
	pCreature->_setFaction();
    pCreature->SetMapId(ptr->GetMapId());
    pCreature->SetOrientation(o);
    pCreature->Despawn(duration, 0);
    pCreature->SetInstanceID(ptr->GetInstanceID());
    pCreature->PushToWorld(ptr->GetMapMgr());
	if(duration)
	{
		pCreature->Despawn(duration,0);
	}
	Lunar<Unit>::push(L,pCreature);
	return 0;
}
int luaGameObject_GetItemCount(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID_RET_INT(TYPEID_PLAYER);
	int itemid = luaL_checkint(L,1);
	lua_pushinteger(L, ((Player*)ptr)->GetItemInterface()->GetItemCount(itemid,false));
	return 1;
}
int luaGameObject_SpawnGameObject(lua_State * L, GameObject * ptr)
{
	if(ptr == NULL) return 0;
	uint32 entry_id = luaL_checkint(L, 1);
	float x = (float)luaL_checkint(L, 2);
	float y = (float)luaL_checkint(L, 3);
	float z = (float)luaL_checkint(L, 4);
	float o = (float)luaL_checkint(L, 5);
	uint32 duration = luaL_checkint(L, 6);

	if(!entry_id || !duration)
	{
		lua_pushnil(L);
		return 1;
	}

	GameObjectInfo *goi = GameObjectNameStorage.LookupEntry(entry_id);
	if (goi == NULL) return 0;
    GameObject *pC = ptr->GetMapMgr()->CreateGameObject(entry_id);
    pC->m_spawn=0;
    pC->CreateFromProto(entry_id, ptr->GetMapId(), (float)x, (float)y, (float)z, (float)o, 0.0f, 0.0f, 0.0f, 0.0f);
    pC->SetMapId(ptr->GetMapId());
    pC->SetInstanceID(ptr->GetInstanceID());
	pC->Spawn(ptr->GetMapMgr());
	if(duration)
	{
		pC->ExpireAndDelete(duration);
	}
	Lunar<GameObject>::push(L,pC);
	return 0;
}
int luaGameObject_CalcDistance(lua_State * L, GameObject * ptr)
{
	Object * ob = Lunar<Object>::check(L,1);
	if(ob)
		lua_pushnumber(L,ptr->CalcDistance(ob));
	return 1;
}
int luaGameObject_GetSpawnX(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnX());
	return 1;
}
int luaGameObject_GetSpawnY(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnY());
	return 1;
}
int luaGameObject_GetSpawnZ(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnZ());
	return 1;
}
int luaGameObject_GetSpawnO(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetSpawnO());
	return 1;
}
int luaGameObject_GetX(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetPositionX());
	return 0;
}
int luaGameObject_GetY(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetPositionY());
	return 0;
}
int luaGameObject_GetZ(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetPositionZ());
	return 0;
}
int luaGameObject_GetO(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetOrientation());
	return 0;
}

int luaGameObject_GetInRangePlayersCount(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetInRangePlayersCount());
	return 1;
}
int luaGameObject_GetEntry(lua_State * L, GameObject * ptr)
{
	if(ptr)
		lua_pushnumber(L,ptr->GetEntry());
	return 1;
}
int luaGameObject_SetOrientation(lua_State * L, GameObject * ptr)
{
	float newo = (float)luaL_checkint(L, 1);
	if(!newo|!ptr)
		return 0;
	ptr->SetOrientation(newo);
	return 0;
}
int luaGameObject_CalcRadAngle(lua_State * L, GameObject * ptr)
{
	float x = (float)luaL_checkint(L,1 );
	float y = (float)luaL_checkint(L, 2);
	float x2 = (float)luaL_checkint(L, 3);
	float y2 = (float)luaL_checkint(L, 4);
	if(!x||!y||!x2||!y2||!ptr)
		return 0;
	lua_pushnumber(L,ptr->calcRadAngle(x,y,x2,y2));
	return 0;
}
int luaGameObject_GetInstanceID(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_UNIT);
	if(ptr->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
		lua_pushnil(L);
	else
	lua_pushinteger(L,ptr->GetInstanceID());
	return 1;
}
int luaGameObject_GetInRangePlayers(lua_State * L, GameObject * ptr)
{
	uint32 count = 0;
	lua_newtable(L);
	for(unordered_set<Player*>::iterator itr = ptr->GetInRangePlayerSetBegin(); itr != ptr->GetInRangePlayerSetEnd(); itr++)
	{
		if( (*itr) ->IsUnit())
		{
			count++,
			lua_pushinteger(L,count);
			Lunar<Unit>::push(L,TO_UNIT(*itr),false);
			lua_rawset(L,-3);
		}
	}
	return 1;
}

int luaGameObject_GetInRangeGameObjects(lua_State * L, GameObject * ptr)
{
	uint32 count = 0;
	lua_newtable(L);
	for (unordered_set<Object*>::iterator itr = ptr->GetInRangeSetBegin();itr!= ptr->GetInRangeSetEnd();itr++)
	{
		if( (*itr) ->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			count++,
			lua_pushinteger(L,count);
			Lunar<GameObject>::push(L,((GameObject*)*itr),false);
			lua_rawset(L,-3);
		}
	}
	return 1;
}
int luaGameObject_IsInFront(lua_State * L, GameObject * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		if(ptr->isInFront(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaGameObject_IsInBack(lua_State * L, GameObject * ptr)
{
	Unit * target = Lunar<Unit>::check(L, 1);
	if(ptr && target)
		if(ptr->isInBack(target))
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
	return 1;
}
int luaGameObject_GetUInt32Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	if (ptr)
		lua_pushinteger(L,ptr->GetUInt32Value(field));
	return 1;
}
int luaGameObject_GetUInt64Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	if (ptr && field)
	{
		lua_pushinteger(L,(int)ptr->GetUInt64Value(field));
	}
	return 1;
}
int luaGameObject_SetUInt32Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	int value = luaL_checkint(L,2);
	if(ptr && field && value)
	{
		ptr->SetUInt32Value(field,value);
	}
	return 1;
}
int luaGameObject_SetUInt64Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	int value = luaL_checkint(L,2);
	if(ptr && field && value)
	{
		ptr->SetUInt64Value(field,value);
	}
	return 1;
}
int luaGameObject_SetFloatValue(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	float value = (float)luaL_checkint(L,2);
	if( ptr && value)
	{
		ptr->SetFloatValue(field,value);
	}
	return 1;
}
int luaGameObject_GetFloatValue(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	if( ptr && field)
	{
		lua_pushnumber(L,ptr->GetFloatValue(field));
	}
	return 1;
}
int luaGameObject_ModUInt32Value(lua_State * L, GameObject * ptr)
{
	int field = luaL_checkint(L,1);
	int value = luaL_checkint(L,2);
	if( ptr && field && value)
	{
		ptr->ModSignedInt32Value(field,value);
	}
	return 1;
}
int luaGameObject_CastSpell(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	if( !ptr || !sp|| sp == 0) return 0;
	
	Spell * spp = new Spell( ptr, dbcSpell.LookupEntry(sp), true, NULL);
	SpellCastTargets tar(ptr->GetGUID());
	spp->prepare(&tar);
	return 0;
}
int luaGameObject_FullCastSpell(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	if( !ptr || !sp|| sp == 0) return 0;
	
	Spell * nspell = new Spell(ptr,dbcSpell.LookupEntry(sp),false,NULL);
	SpellCastTargets tar(ptr->GetGUID());
	nspell->prepare(&tar);
	return 0;
}
int luaGameObject_CastSpellOnTarget(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	Unit * target = Lunar<Unit>::check(L,2);
	if( !ptr || !sp || sp == 0) return 0;

	Spell * nspell = new Spell(ptr,dbcSpell.LookupEntry(sp),true,NULL);
	SpellCastTargets tar(target->GetGUID());
	nspell->prepare(&tar);
	return 0;
}
int luaGameObject_FullCastSpellOnTarget(lua_State * L, GameObject * ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);
	uint32 sp = luaL_checkint(L,1);
	Unit * target = Lunar<Unit>::check(L,2);
	if( !ptr || !sp || sp == 0) return 0;
	Spell * nspell = new Spell(ptr,dbcSpell.LookupEntry(sp),false,NULL);
	SpellCastTargets tar(target->GetGUID());
	nspell->prepare(&tar);
	return 0;
}
int luaGameObject_GetGUID(lua_State * L, GameObject* ptr)
{
	CHECK_TYPEID(TYPEID_GAMEOBJECT);

	lua_pushinteger(L,ptr->GetGUID());
	return 1;
}

#endif // GAMEOBJECT_FUNCTIONS_H
