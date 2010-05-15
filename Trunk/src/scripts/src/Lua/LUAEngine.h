/*
 * Ascent MMORPG Server
 * LUA Interface
 * Copyright (C) 2007 Burlex <burlex@gmail.com>
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

#ifndef __LUAENGINE_H
#define __LUAENGINE_H

//#define PRINTERRORS

extern "C" {		// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "LUAFunctions.h"
};

class LuaEngine;
class LuaEngineMgr;

extern LuaEngineMgr g_luaMgr;
extern LuaEngine * g_engine;

GossipMenu* Menu;

/** Macros for calling lua-based events
 */
#define LUA_ON_UNIT_EVENT(unit,eventtype,miscunit,misc) if(unit->GetTypeId()==TYPEID_UNIT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->OnUnitEvent(unit,eventtype,miscunit,misc); }
#define LUA_ON_QUEST_EVENT(plr,quest,eventtype,miscobject) if(plr->IsPlayer() && plr->IsInWorld() && miscobject->IsInWorld() && !miscobject->IsPlayer()) { plr->GetMapMgr()->GetScriptEngine()->OnQuestEvent(plr,quest,eventtype,miscobject); } 
#define LUA_ON_GO_EVENT(unit,evtype,miscunit) if(unit->GetTypeId()==TYPEID_GAMEOBJECT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->OnGameObjectEvent(unit,evtype,miscunit); }
#define LUA_ON_GOSSIP_EVENT(object, evtype, player, id, intid, code) if(object->IsInWorld()) { object->GetMapMgr()->GetScriptEngine()->OnGossipEvent(object, evtype, player, id, intid, code); }
#define LUA_CALL_FUNC(unit,funcname) if(unit->GetTypeId()==TYPEID_UNIT && unit->IsInWorld()) { unit->GetMapMgr()->GetScriptEngine()->CallFunction(unit,funcname); }

/** Quest Events
 */
enum QuestEvents
{
	QUEST_EVENT_ON_COMPLETE		= 1,
	QUEST_EVENT_ON_ACCEPT		= 2,
	//QUEST_EVENT_CAN_ACCEPT		= 3, 
	QUEST_EVENT_COUNT,
};

/** Gossip Events
*/
enum GossipEvents
{
	GOSSIP_EVENT_ON_TALK			= 1,
	GOSSIP_EVENT_ON_SELECT_OPTION	= 2,
	GOSSIP_EVENT_ON_END             = 3,
	GOSSIP_EVENT_COUNT,
};

/** Creature Events
 */
enum CreatureEvents
{
	CREATURE_EVENT_ON_ENTER_COMBAT		= 1,
	CREATURE_EVENT_ON_LEAVE_COMBAT		= 2,
	CREATURE_EVENT_ON_KILLED_TARGET		= 3,
	CREATURE_EVENT_ON_DIED				= 4,
	CREATURE_EVENT_AI_TICK				= 5,
	CREATURE_EVENT_ON_SPAWN				= 6,
	CREATURE_EVENT_ON_GOSSIP_TALK		= 7,
	CREATURE_EVENT_ON_REACH_WP			= 8,
	CREATURE_EVENT_ON_LEAVE_LIMBO		= 9,
	CREATURE_EVENT_PLAYER_ENTERS_RANGE	= 10,
	CREATURE_EVENT_COUNT,
};

/** GameObject Events
 */
enum GameObjectEvents
{
	GAMEOBJECT_EVENT_ON_SPAWN			= 1,
	GAMEOBJECT_EVENT_ON_USE				= 2,
	GAMEOBJECT_EVENT_COUNT,
};


enum RandomFlags
{
	RANDOM_ANY           = 0,
	RANDOM_IN_SHORTRANGE = 1,
	RANDOM_IN_MIDRANGE   = 2,
	RANDOM_IN_LONGRANGE  = 3,
	RANDOM_WITH_MANA     = 4,
	RANDOM_WITH_RAGE     = 5,
	RANDOM_WITH_ENERGY   = 6,
	RANDOM_NOT_MAINTANK  = 7
};
class LuaEngine
{
private:
	lua_State * L;
	Mutex m_Lock;

public:
	LuaEngine();
	~LuaEngine();

	void LoadScripts();
	void Shutdown();
	void Restart();
	void RegisterCoreFunctions();
	HEARTHSTONE_INLINE Mutex& GetLock() { return m_Lock; }

	void OnUnitEvent(Unit * pUnit, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc);
	void OnQuestEvent(Player * QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Object * QuestStarter);
	void OnGameObjectEvent(GameObject * pGameObject, const char * FunctionName, uint32 EventType, Unit * pMiscUnit);
	void OnGossipEvent(Object* pObject, const char * FunctionName, uint32 EventType, Player* mPlayer, uint32 Id, uint32 IntId, const char *Code);
	void CallFunction(Unit * pUnit, const char * FuncName);
};

struct LuaUnitBinding { const char * Functions[CREATURE_EVENT_COUNT]; };
struct LuaQuestBinding { const char * Functions[QUEST_EVENT_COUNT]; };
struct LuaGameObjectBinding { const char * Functions[GAMEOBJECT_EVENT_COUNT]; };
struct LuaUnitGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };
struct LuaItemGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };
struct LuaGOGossipBinding { const char * Functions[GOSSIP_EVENT_COUNT]; };

class LuaEngineMgr
{
private:
	typedef HM_NAMESPACE::hash_map<uint32, LuaUnitBinding> UnitBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaQuestBinding> QuestBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGameObjectBinding> GameObjectBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaUnitGossipBinding> GossipUnitScriptsBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaItemGossipBinding> GossipItemScriptsBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGOGossipBinding> GossipGOScriptsBindingMap;

	UnitBindingMap m_unitBinding;
	QuestBindingMap m_questBinding;
	GameObjectBindingMap m_gameobjectBinding;
	GossipUnitScriptsBindingMap m_unit_gossipBinding;
	GossipItemScriptsBindingMap m_item_gossipBinding;
	GossipGOScriptsBindingMap m_go_gossipBinding;

public:
	LuaEngine * m_engine;
	void Startup();
	void Unload();

	void RegisterUnitEvent(uint32 Id, uint32 Event, const char * FunctionName);
	void RegisterQuestEvent(uint32 Id, uint32 Event, const char * FunctionName);
	void RegisterGameObjectEvent(uint32 Id, uint32 Event, const char * FunctionName);
	void RegisterUnitGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
    void RegisterItemGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
    void RegisterGOGossipEvent(uint32 Id, uint32 Event, const char * FunctionName);
 
	LuaUnitBinding * GetUnitBinding(uint32 Id)
	{
		UnitBindingMap::iterator itr = m_unitBinding.find(Id);
		return (itr == m_unitBinding.end()) ? NULL : &itr->second;
	}

	LuaQuestBinding * GetQuestBinding(uint32 Id)
	{
		QuestBindingMap::iterator itr = m_questBinding.find(Id);
		return (itr == m_questBinding.end()) ? NULL : &itr->second;
	}

	LuaGameObjectBinding * GetGameObjectBinding(uint32 Id)
	{
		GameObjectBindingMap::iterator itr =m_gameobjectBinding.find(Id);
		return (itr == m_gameobjectBinding.end()) ? NULL : &itr->second;
	}

    // Gossip Stuff
    LuaUnitGossipBinding * GetLuaUnitGossipBinding(uint32 Id)
	{
		GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.find(Id);
		return (itr == m_unit_gossipBinding.end()) ? NULL : &itr->second;
	}

    LuaItemGossipBinding * GetLuaItemGossipBinding(uint32 Id)
	{
		GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.find(Id);
		return (itr == m_item_gossipBinding.end()) ? NULL : &itr->second;
	}

    LuaGOGossipBinding * GetLuaGOGossipBinding(uint32 Id)
	{
		GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.find(Id);
		return (itr == m_go_gossipBinding.end()) ? NULL : &itr->second;
	}

};

template<typename T>
struct RegType
{
	const char * name;
	int(*mfunc)(lua_State*,T*);
};

RegType<Item> ItemMethods[] = {
	{ "GossipCreateMenu", &luaItem_GossipCreateMenu },
	{ "GossipMenuAddItem", &luaItem_GossipMenuAddItem },
	{ "GossipSendMenu", &luaItem_GossipSendMenu },
	{ "GossipComplete", &luaItem_GossipComplete },
	{ "GossipSendPOI", &luaItem_GossipSendPOI },
	{ "PerformWorldDBQuery", &luaItem_PerformWorldDBQuery },
	{ "PerformCharDBQuery", &luaItem_PerformCharDBQuery },
	{ "GetOwner", &luaItem_GetOwner },
	{ NULL, NULL },
};

RegType<Unit> UnitMethods[] = {
	{ "GossipCreateMenu", &luaUnit_GossipCreateMenu },
	{ "GossipMenuAddItem", &luaUnit_GossipMenuAddItem },
	{ "GossipSendMenu", &luaUnit_GossipSendMenu },
	{ "GossipComplete", &luaUnit_GossipComplete },
	{ "GossipSendPOI", &luaUnit_GossipSendPOI },
	{ "GetName", &luaUnit_GetName },
	{ "SendChatMessage", &luaUnit_SendChatMessage },
	{ "MoveTo", &luaUnit_MoveTo },
	{ "SetMovementType", &luaUnit_SetMovementType },
	{ "CastSpell", &luaUnit_CastSpell },
	{ "FullCastSpell", &luaUnit_FullCastSpell },
	{ "CastSpellOnTarget", &luaUnit_CastSpellOnTarget },
	{ "FullCastSpellOnTarget", &luaUnit_FullCastSpellOnTarget },
	{ "SpawnCreature", &luaUnit_SpawnCreature },
	{ "SpawnGameObject", &luaUnit_SpawnGameObject },
	{ "GetX", &luaUnit_GetX },
	{ "GetY", &luaUnit_GetY },
	{ "GetZ", &luaUnit_GetZ },
	{ "GetO", &luaUnit_GetO },
	{ "IsPlayer", &luaUnit_IsPlayer },
	{ "IsCreature", &luaUnit_IsCreature },
	{ "RegisterEvent", &luaUnit_RegisterEvent },
	{ "RemoveEvents", &luaUnit_RemoveEvents },
	{ "SendBroadcastMessage", &luaUnit_SendBroadcastMessage },
	{ "SendAreaTriggerMessage", &luaUnit_SendAreaTriggerMessage },
	{ "KnockBack", &luaUnit_KnockBack },
	{ "MarkQuestObjectiveAsComplete", &luaUnit_MarkQuestObjectiveAsComplete },
	{ "LearnSpell", &luaUnit_LearnSpell },
	{ "UnlearnSpell", &luaUnit_UnlearnSpell },
	{ "HasFinishedQuest", &luaUnit_HasFinishedQuest },
	{ "GetItemCount", &luaUnit_GetItemCount },
	{ "IsInCombat", &luaUnit_IsInCombat },
	{ "GetMainTank", &luaUnit_GetMainTank },
	{ "GetAddTank", &luaUnit_GetAddTank },
	{ "ClearThreatList", &luaUnit_ClearThreatList },
	{ "GetTauntedBy", &luaUnit_GetTauntedBy },
	{ "SetTauntedBy", &luaUnit_SetTauntedBy },
	{ "ChangeTarget", &luaUnit_ChangeTarget },
	{ "GetHealthPct", &luaUnit_GetHealthPct },
	{ "SetHealthPct", &luaUnit_SetHealthPct },
	{ "GetManaPct", &luaUnit_GetManaPct },
	{ "Despawn", &luaUnit_Despawn },
	{ "GetUnitBySqlId", &luaUnit_GetUnitBySqlId },
	{ "PlaySoundToSet", &luaUnit_PlaySoundToSet },
	{ "RemoveAura", &luaUnit_RemoveAura },
	{ "StopMovement", &luaUnit_StopMovement },
	{ "Emote", &luaUnit_Emote },
	{ "GetInstanceID", &luaUnit_GetInstanceID },
	{ "GetClosestPlayer", &luaUnit_GetClosestPlayer },
	{ "GetRandomPlayer", &luaUnit_GetRandomPlayer },
	{ "GetRandomFriend", &luaUnit_GetRandomFriend },
	{ "AddItem", &luaUnit_AddItem },
	{ "RemoveItem", &luaUnit_RemoveItem },
	{ "CreateCustomWaypointMap", &luaUnit_CreateCustomWaypointMap },
	{ "CreateWaypoint", &luaUnit_CreateWaypoint },
	{ "MoveToWaypoint", &luaUnit_MoveToWaypoint },
	{ "DestroyCustomWaypointMap", &luaUnit_DestroyCustomWaypointMap },
	{ "SetCombatCapable", &luaUnit_SetCombatCapable },
	{ "SetCombatMeleeCapable", &luaUnit_SetCombatMeleeCapable },
	{ "SetCombatRangedCapable", &luaUnit_SetCombatRangedCapable },
	{ "SetCombatSpellCapable", &luaUnit_SetCombatSpellCapable },
	{ "SetCombatTargetingCapable", &luaUnit_SetCombatTargetingCapable },
	{ "SetNPCFlags", &luaUnit_SetNPCFlags },
	{ "SetModel", &luaUnit_SetModel },
	{ "SetScale", &luaUnit_SetScale },
	{ "SetFaction", &luaUnit_SetFaction },
	{ "SetStandState",&luaUnit_SetStandState },
	{ "Teleport" , &luaUnit_TeleportUnit },
	{ "GetPlayerClass", &luaUnit_GetPlayerClass },
	{ "ClearThreatList", &luaUnit_ClearHateList },
	{ "WipeThreatList", &luaUnit_WipeHateList },
	{ "WipeTargetList", &luaUnit_WipeTargetList },
	{ "WipeCurrentTarget", &luaUnit_WipeCurrentTarget },
	{ "GetHealth", &luaUnit_GetHealth },
	{ "GetMaxHealth", &luaUnit_GetMaxHealth },
	{ "SetHealth", &luaUnit_SetHealth },
	{ "SetMaxHealth", &luaUnit_SetMaxHealth },
	{ "HasAura", &luaUnit_HasAura },
	{ "Land", &luaUnit_Land },
	{ "SetFlying", &luaUnit_SetFlying },
	{ "SetMana", &luaUnit_SetMana },
	{ "SetMaxMana", &luaUnit_SetMaxMana },
	{ "GetDistance", &luaUnit_GetDistance },
	{ "GetGUID", &luaUnit_GetGUID },
	{ "GetCreatureNearestCoords", &luaUnit_GetCreatureNearestCoords },
    { "CastSpellAoF", &luaUnit_CastSpellAoF },
	{ "GetGameObjectNearestCoords", &luaUnit_GetGameObjectNearestCoords },
	{ "SetInFront", &luaUnit_SetInFront },
	{ "RemoveAllAuras", &luaUnit_RemoveAllAuras },
	{ "ReturnToSpawnPoint", &luaUnit_ReturnToSpawnPoint },
	{ "CancelSpell", &luaUnit_CancelSpell },
	{ "IsAlive", &luaUnit_IsAlive },
	{ "IsDead", &luaUnit_IsDead },
	{ "IsInWorld", &luaUnit_IsInWorld },
	{ "GetZoneId", &luaUnit_GetZoneId },
	{ "GetMana", &luaUnit_GetMana },
	{ "GetMaxMana", &luaUnit_GetMaxMana },
	{ "Root", &luaUnit_Root },
	{ "Unroot", &luaUnit_Unroot },
	{ "IsCreatureMoving", &luaUnit_IsCreatureMoving },
	{ "SetOutOfCombatRange", &luaUnit_SetOutOfCombatRange },
	{ "ModifyRunSpeed", &luaUnit_ModifyRunSpeed },
	{ "ModifyWalkSpeed", &luaUnit_ModifyWalkSpeed },
	{ "ModifyFlySpeed" , &luaUnit_ModifyFlySpeed },
	{ "GetCurrentSpell", &luaUnit_GetCurrentSpell },
	{ "IsFlying", &luaUnit_IsFlying },
	{ "SetRotation", &luaUnit_SetRotation },
	{ "SetOrientation", &luaUnit_SetOrientation },
	{ "GetSpawnX", &luaUnit_GetSpawnX },
	{ "GetSpawnY", &luaUnit_GetSpawnY },
	{ "GetSpawnZ", &luaUnit_GetSpawnZ },
	{ "GetSpawnO", &luaUnit_GetSpawnO },
	{ "GetInRangePlayersCount", &luaUnit_GetInRangePlayersCount },
	{ "GetEntry", &luaUnit_GetEntry },
	{ "SetMoveRunFlag", &luaUnit_SetMoveRunFlag },
	{ "GetAIState", &luaUnit_GetAIState },
	{ "ModUInt32Value", &luaUnit_ModUInt32Value },
	{ "ModFloatValue", &luaUnit_ModFloatValue },
	{ "SetUInt32Value", &luaUnit_SetUInt32Value },
	{ "SetUInt64Value", &luaUnit_SetUInt64Value },
	{ "SetFloatValue", &luaUnit_SetFloatValue },
	{ "GetUInt32Value", &luaUnit_GetUInt32Value },
	{ "GetUInt64Value", &luaUnit_GetUInt64Value },
	{ "GetFloatValue", &luaUnit_GetFloatValue },
	{ "InitPacket", &luaUnit_InitPacket },
	{ "AddDataToPacket", &luaUnit_AddDataToPacket },
	{ "AddGuidDataToPacket", &luaUnit_AddGuidDataToPacket },
	{ "SendData", &luaUnit_SendData },
	{ "AdvanceQuestObjective", &luaUnit_AdvanceQuestObjective },
	{ "Heal", &luaUnit_Heal },
	{ "Energize", &luaUnit_Energize },
	{ "SendChatMessageAlternateEntry", &luaUnit_SendChatMessageAlternateEntry },
	{ "SendChatMessageToPlayer", &luaUnit_SendChatMessageToPlayer },
	{ "SetPowerType", &luaUnit_SetPowerType },
	{ "Strike", &luaUnit_Strike },
	{ "SetAttackTimer", &luaUnit_SetAttackTimer },
	{ "Kill", &luaUnit_Kill },
	{ "DealDamage", &luaUnit_DealDamage },
	{ "IsInFront", &luaUnit_IsInFront },
	{ "IsInBack", &luaUnit_IsInBack },
	{ "IsPacified", &luaUnit_IsPacified },
	{ "IsStunned", &luaUnit_IsStunned },
	{ "IsFeared", &luaUnit_IsFeared },
	{ "CreateGuardian", &luaUnit_CreateGuardian },
	{ "HandleEvent", &luaUnit_HandleEvent },
	{ "HasInRangeObjects", &luaUnit_HasInRangeObjects },
	{ "IsInWater", &luaUnit_IsInWater },
	{ "GetInRangeObjects", &luaUnit_GetInRangeGameObjects },
	{ "GetInRangeObjectsCount", &luaUnit_GetInRangeObjectsCount },
	{ "GetAITargetsCount", &luaUnit_GetAITargetsCount },
	{ "SetUnitToFollow", &luaUnit_SetUnitToFollow },
	{ "GetUnitToFollow", &luaUnit_GetUnitToFollow },
	{ "DismissPet", &luaUnit_DismissPet },
	{ "IsPet", &luaUnit_IsPet },
	{ "SetNextTarget", &luaUnit_SetNextTarget },
	{ "GetNextTarget", &luaUnit_GetNextTarget },
	{ "SetPetOwner", &luaUnit_SetPetOwner },
	{ "GetPetOwner", &luaUnit_GetPetOwner },
	{ "CalcToDistance", &luaUnit_CalcToDistance },
	{ "CalcAngle", &luaUnit_CalcAngle },
	{ "CalcRadAngle", &luaUnit_CalcRadAngle },
	{ "SetFacing", &luaUnit_SetFacing },
	{ "SetDeathState", &luaUnit_SetDeathState },
	{ "IsInvisible", &luaUnit_IsInvisible },
	{ "IsInvincible", &luaUnit_IsInvincible },
	{ "ResurrectPlayer", &luaUnit_ResurrectPlayer },
	{ "KickPlayer", &luaUnit_KickPlayer },
	{ "CanCallForHelp", &luaUnit_CanCallForHelp },
	{ "CallForHelpHp", &luaUnit_CallForHelpHp },
	{ "SetCreatureNameById", &luaUnit_SetCreatureName },
	{ "GetInRangePlayers", &luaUnit_GetInRangePlayers },
	{ "GetUnitByGUID", &luaUnit_GetUnitByGUID },
	{ "RemoveFromWorld", &luaUnit_RemoveFromWorld },
	{ "GetFaction", &luaUnit_GetFaction },
	{ "EnableMoveFly", &luaUnit_MoveFly },
	{ "SpellNonMeleeDamageLog", &luaUnit_SpellNonMeleeDamageLog },
	{ "DisableRespawn", &luaUnit_NoRespawn },
	{ "ModThreat", &luaUnit_ModThreat },
	{ "GetThreat", &luaUnit_GetThreatByPtr },
	{ "GetInRangeFriends", &luaUnit_GetInRangeFriends },
	{ "GetPowerType", &luaUnit_GetPowerType },
	{ "GetMapId", &luaUnit_GetMapId },
	{ "CalcDistance" , &luaUnit_CalcDistance },
	{ "AttackReaction", &luaUnit_AttackReaction },
	{ "EventCastSpell", &luaUnit_EventCastSpell },
	{ "IsPlayerMoving", &luaUnit_IsPlayerMoving },
	{ "IsPlayerAttacking", &luaUnit_IsPlayerAttacking },
	{ "RemoveThreat", &luaUnit_RemoveThreatByPtr },
	{ "SetPlayerAtWar", &luaUnit_SetPlayerAtWar },
	{ "GetFactionStanding", &luaUnit_GetFactionStanding },
	{ "PlaySpellVisual", &luaUnit_PlaySpellVisual },
	{ "GetPlayerLevel", &luaUnit_GetPlayerLevel },
	{ "SetPlayerLevel", &luaUnit_SetPlayerLevel },
	{ "SetStanding", &luaUnit_SetPlayerStanding },
	{ "GetStanding", &luaUnit_GetStanding },
	{ "HasItem", &luaUnit_HasItem },
	{ "AdvanceSkill", &luaUnit_AdvanceSkill },
	{ "AddSkill", &luaUnit_AddSkill },
	{ "RemoveSkill", &luaUnit_RemoveSkill },
	{ "EnableFlyCheat", &luaUnit_FlyCheat },
	{ "AddAssistTarget", &luaUnit_AddAssistTargets },
	{ "GetCurrentSpellId", &luaUnit_GetCurrentSpellId },
	{ "GetPlayerRace", &luaUnit_GetPlayerRace },
	{ "RemoveAurasByMechanic", &luaUnit_RemoveAurasByMechanic },
	{ "RemoveAurasType", &luaUnit_RemoveAurasType },

	{ NULL, NULL },
};


RegType<GameObject> GOMethods[] = {
	{ "GossipCreateMenu", &luaGameObject_GossipCreateMenu },
	{ "GossipSendMenu", &luaGameObject_GossipSendMenu },
	{ "GossipComplete", &luaGameObject_GossipComplete },
	{ "GossipSendPOI", &luaGameObject_GossipSendPOI },
	{ "GetX", &luaGameObject_GetX },
	{ "GetY", &luaGameObject_GetY },
	{ "GetZ", &luaGameObject_GetZ },
	{ "GetO", &luaGameObject_GetO },
	{ "PlaySoundToSet", &luaGameObject_PlaySoundToSet },
 	{ "GetName", &luaGameObject_GetName },
	{ "Teleport" , &luaGameObject_TeleportPlr },
	{ NULL, NULL },
};

template<typename T> RegType<T>* GetMethodTable() { return NULL; }
template<>
RegType<Item>* GetMethodTable<Item>() { return ItemMethods; }

template<>
RegType<Unit>* GetMethodTable<Unit>() { return UnitMethods; }

template<>
RegType<GameObject>* GetMethodTable<GameObject>() { return GOMethods; }

template<typename T> const char * GetTClassName() { return "UNKNOWN"; };
template<> const char * GetTClassName<Item>() { return "Item"; };
template<> const char * GetTClassName<Unit>() {	return "Unit"; };
template<> const char * GetTClassName<GameObject>() { return "GameObject"; };

template <typename T> class Lunar
{
	typedef struct { T *pT; } userdataType;
public:
	typedef int (*mfp)(lua_State *L, T* ptr);
	typedef struct { const char *name; mfp mfunc; } RegType;

	static void Register(lua_State *L)
	{
		lua_newtable(L);
		int methods = lua_gettop(L);

		luaL_newmetatable(L, GetTClassName<T>());
		int metatable = lua_gettop(L);

		// store method table in globals so that
		// scripts can add functions written in Lua.
		lua_pushvalue(L, methods);
		set(L, LUA_GLOBALSINDEX, GetTClassName<T>());

		// hide metatable from Lua getmetatable()
		lua_pushvalue(L, methods);
		set(L, metatable, "__metatable");

		lua_pushvalue(L, methods);
		set(L, metatable, "__index");

		lua_pushcfunction(L, tostring_T);
		set(L, metatable, "__tostring");

		lua_pushcfunction(L, gc_T);
		set(L, metatable, "__gc");

		lua_newtable(L);                // mt for method table
		lua_pushcfunction(L, new_T);
		lua_pushvalue(L, -1);           // dup new_T function
		set(L, methods, "new");         // add new_T to method table
		set(L, -3, "__call");           // mt.__call = new_T
		lua_setmetatable(L, methods);

		// fill method table with methods from class T
		for (RegType *l = ((RegType*)GetMethodTable<T>()); l->name; l++) {
			lua_pushstring(L, l->name);
			lua_pushlightuserdata(L, (void*)l);
			lua_pushcclosure(L, thunk, 1);
			lua_settable(L, methods);
		}

		lua_pop(L, 2);  // drop metatable and method table
	}

	// call named lua method from userdata method table
	static int call(lua_State *L, const char *method,
		int nargs=0, int nresults=LUA_MULTRET, int errfunc=0)
	{
		int base = lua_gettop(L) - nargs;  // userdata index
		if (!luaL_checkudata(L, base, T::className)) {
			lua_settop(L, base-1);           // drop userdata and args
			lua_pushfstring(L, "not a valid %s userdata", T::className);
			return -1;
		}

		lua_pushstring(L, method);         // method name
		lua_gettable(L, base);             // get method from userdata
		if (lua_isnil(L, -1)) {            // no method?
			lua_settop(L, base-1);           // drop userdata and args
			lua_pushfstring(L, "%s missing method '%s'", T::className, method);
			return -1;
		}
		lua_insert(L, base);               // put method under userdata, args

		int status = lua_pcall(L, 1+nargs, nresults, errfunc);  // call method
		if (status) {
			const char *msg = lua_tostring(L, -1);
			if (msg == NULL) msg = "(error with no message)";
			lua_pushfstring(L, "%s:%s status = %d\n%s",
				T::className, method, status, msg);
			lua_remove(L, base);             // remove old message
			return -1;
		}
		return lua_gettop(L) - base + 1;   // number of results
	}

	// push onto the Lua stack a userdata containing a pointer to T object
	static int push(lua_State *L, T *obj, bool gc=false) {
		if (!obj) { lua_pushnil(L); return 0; }
		luaL_getmetatable(L, GetTClassName<T>());  // lookup metatable in Lua registry
		if (lua_isnil(L, -1)) luaL_error(L, "%s missing metatable", GetTClassName<T>());
		int mt = lua_gettop(L);
		subtable(L, mt, "userdata", "v");
		userdataType *ud =
			static_cast<userdataType*>(pushuserdata(L, obj, sizeof(userdataType)));
		if (ud) {
			ud->pT = obj;  // store pointer to object in userdata
			lua_pushvalue(L, mt);
			lua_setmetatable(L, -2);
			if (gc == false) {
				lua_checkstack(L, 3);
				subtable(L, mt, "do not trash", "k");
				lua_pushvalue(L, -2);
				lua_pushboolean(L, 1);
				lua_settable(L, -3);
				lua_pop(L, 1);
			}
		}
		lua_replace(L, mt);
		lua_settop(L, mt);
		return mt;  // index of userdata containing pointer to T object
	}

	// get userdata from Lua stack and return pointer to T object
	static T *check(lua_State *L, int narg) {
		userdataType *ud =
			static_cast<userdataType*>(luaL_checkudata(L, narg, GetTClassName<T>()));
		if(!ud) { luaL_typerror(L, narg, GetTClassName<T>()); return NULL; }
		return ud->pT;  // pointer to T object
	}

private:
	Lunar();  // hide default constructor

	// member function dispatcher
	static int thunk(lua_State *L) {
		// stack has userdata, followed by method args
		T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
		lua_remove(L, 1);  // remove self so member function args start at index 1
		// get member function from upvalue
		RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
		//return (obj->*(l->mfunc))(L);  // call member function
		return l->mfunc(L,obj);
	}

	// create a new T object and
	// push onto the Lua stack a userdata containing a pointer to T object
	static int new_T(lua_State *L) {
		lua_remove(L, 1);   // use classname:new(), instead of classname.new()
		T *obj = NULL/*new T(L)*/;  // call constructor for T objects
		assert(false);
		push(L, obj, true); // gc_T will delete this object
		return 1;           // userdata containing pointer to T object
	}

	// garbage collection metamethod
	static int gc_T(lua_State *L) {
		if (luaL_getmetafield(L, 1, "do not trash")) {
			lua_pushvalue(L, 1);  // dup userdata
			lua_gettable(L, -2);
			if (!lua_isnil(L, -1)) return 0;  // do not delete object
		}
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		if (obj) delete obj;  // call destructor for T objects
		return 0;
	}

	static int tostring_T (lua_State *L) {
		char buff[32];
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		T *obj = ud->pT;
		sprintf(buff, "%p", obj);
		lua_pushfstring(L, "%s (%s)", GetTClassName<T>(), buff);
		return 1;
	}

	static void set(lua_State *L, int table_index, const char *key) {
		lua_pushstring(L, key);
		lua_insert(L, -2);  // swap value and key
		lua_settable(L, table_index);
	}

	static void weaktable(lua_State *L, const char *mode) {
		lua_newtable(L);
		lua_pushvalue(L, -1);  // table is its own metatable
		lua_setmetatable(L, -2);
		lua_pushliteral(L, "__mode");
		lua_pushstring(L, mode);
		lua_settable(L, -3);   // metatable.__mode = mode
	}

	static void subtable(lua_State *L, int tindex, const char *name, const char *mode) {
		lua_pushstring(L, name);
		lua_gettable(L, tindex);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_checkstack(L, 3);
			weaktable(L, mode);
			lua_pushstring(L, name);
			lua_pushvalue(L, -2);
			lua_settable(L, tindex);
		}
	}

	static void *pushuserdata(lua_State *L, void *key, size_t sz) {
		void *ud = 0;
		lua_pushlightuserdata(L, key);
		lua_gettable(L, -2);     // lookup[key]
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);         // drop nil
			lua_checkstack(L, 3);
			ud = lua_newuserdata(L, sz);  // create new userdata
			lua_pushlightuserdata(L, key);
			lua_pushvalue(L, -2);  // dup userdata
			lua_settable(L, -4);   // lookup[key] = userdata
		}
		return ud;
	}
};

/************************************************************************/
/* SCRIPT FUNCTION IMPLEMENTATION                                       */
/************************************************************************/
#define CHECK_TYPEID(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { return 0; }
#define CHECK_TYPEID_RET(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { lua_pushboolean(L,0); return 0; }
#define CHECK_TYPEID_RET_INT(expected_type) if(!ptr || !ptr->IsInWorld() || ptr->GetTypeId() != expected_type) { lua_pushinteger(L,0); return 0; }

#endif

