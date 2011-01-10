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

#ifndef __LUAENGINE_H
#define __LUAENGINE_H

#include "StdAfx.h"

//#define PRINTERRORS

extern "C" {		// we're C++, and LUA is C, so the compiler needs to know to use C function names.
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
};

class LuaEngine;
class LuaEngineMgr;
class LuaCreature;
class LuaGameObject;
class LuaQuest;
//class LuaInstance;
class LuaGossip;

#ifdef WIN32
	HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

extern LuaEngine * g_engine;
extern LuaEngineMgr g_luaMgr;
#define LuaEvent g_luaMgr.LuaEventMgr

GossipMenu* Menu;

/** Quest Events
 */
enum QuestEvents
{
	QUEST_EVENT_ON_ACCEPT = 1,
	QUEST_EVENT_ON_COMPLETE = 2,
	QUEST_EVENT_ON_CANCEL = 3,
	QUEST_EVENT_GAMEOBJECT_ACTIVATE = 4,
	QUEST_EVENT_ON_CREATURE_KILL  = 5,
	QUEST_EVENT_ON_EXPLORE_AREA = 6,
	QUEST_EVENT_ON_PLAYER_ITEMPICKUP = 7,
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
	CREATURE_EVENT_ON_TARGET_PARRIED	= 5,
	CREATURE_EVENT_ON_TARGET_DODGED		= 6,
	CREATURE_EVENT_ON_TARGET_BLOCKED	= 7,
	CREATURE_EVENT_ON_TARGET_CRIT_HIT	= 8,
	CREATURE_EVENT_ON_PARRY				= 9,
	CREATURE_EVENT_ON_DODGED			= 10,
	CREATURE_EVENT_ON_BLOCKED			= 11,
	CREATURE_EVENT_ON_CRIT_HIT			= 12,
	CREATURE_EVENT_ON_HIT				= 13,
	CREATURE_EVENT_ON_ASSIST_DIED		= 14,
	CREATURE_EVENT_ON_FEAR				= 15,
	CREATURE_EVENT_ON_FLEE				= 16,
	CREATURE_EVENT_ON_CALL_FOR_HELP		= 17,
	CREATURE_EVENT_ON_LOAD				= 18,
	CREATURE_EVENT_ON_REACH_WP			= 19,
	CREATURE_EVENT_ON_LOOT_TAKEN		= 20,
	CREATURE_EVENT_ON_AIUPDATE			= 21,
	CREATURE_EVENT_ON_EMOTE				= 22,
	CREATURE_EVENT_ON_DAMAGE_TAKEN		= 23,
	CREATURE_EVENT_AI_TICK				= 24,
	CREATURE_EVENT_ON_SPAWN				= 25,
	CREATURE_EVENT_ON_GOSSIP_TALK		= 26,
	CREATURE_EVENT_ON_LEAVE_LIMBO		= 27,
	CREATURE_EVENT_PLAYER_ENTERS_RANGE	= 28,
	CREATURE_EVENT_COUNT,
};

/** GameObject Events
 */
enum GameObjectEvents
{
	GAMEOBJECT_EVENT_ON_CREATE			= 1,
	GAMEOBJECT_EVENT_ON_SPAWN			= 2,
	GAMEOBJECT_EVENT_ON_LOOT_TAKEN		= 3,
	GAMEOBJECT_EVENT_ON_USE				= 4,
	GAMEOBJECT_EVENT_AIUPDATE			= 5,
	GAMEOBJECT_EVENT_ON_DESPAWN			= 6,
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

//reg type defines
#define REGTYPE_UNIT (1 << 0)
#define REGTYPE_GO (1 << 1)
#define REGTYPE_QUEST (1 << 2)
#define REGTYPE_SERVHOOK (1 << 3)
#define REGTYPE_ITEM (1 << 4)
#define REGTYPE_GOSSIP (1 << 5)
#define REGTYPE_DUMMYSPELL (1 << 6)
#define REGTYPE_INSTANCE (1 << 7)
#define REGTYPE_UNIT_GOSSIP (REGTYPE_UNIT | REGTYPE_GOSSIP)
#define REGTYPE_GO_GOSSIP (REGTYPE_GO | REGTYPE_GOSSIP)
#define REGTYPE_ITEM_GOSSIP (REGTYPE_ITEM | REGTYPE_GOSSIP)

enum CustomLuaEvenTypes
{
	LUA_EVENT_START = NUM_EVENT_TYPES, // Used as a placeholder
	EVENT_LUA_TIMED,
	EVENT_LUA_CREATURE_EVENTS,
	EVENT_LUA_GAMEOBJ_EVENTS,
	LUA_EVENTS_END
};

struct LUALoadScripts
{
	set<string> luaFiles;
};

struct EventInfoHolder
{
	const char * funcName;
	TimedEvent * te;
};

std::vector<uint32> OnLoadInfo;

struct LuaUnitBinding
{
	uint16 Functions[CREATURE_EVENT_COUNT];
};

struct LuaGameObjectBinding
{
	uint16 Functions[GAMEOBJECT_EVENT_COUNT];
};

struct LuaQuestBinding
{
	uint16 Functions[QUEST_EVENT_COUNT];
};

struct LuaUnitGossipBinding
{
	uint16 Functions[GOSSIP_EVENT_COUNT];
};

struct LuaItemGossipBinding
{
	uint16 Functions[GOSSIP_EVENT_COUNT];
};

struct LuaGOGossipBinding
{
	uint16 Functions[GOSSIP_EVENT_COUNT];
};

std::vector<uint16> EventAsToFuncName[NUM_SERVER_HOOKS];
std::map<uint32, uint16> m_luaDummySpells;

template<typename T> const char * GetTClassName() { return "UNKNOWN"; };
template<typename T> struct RegType { const char * name; int(*mfunc)(lua_State*,T*); };
template<typename T> RegType<T>* GetMethodTable() { return NULL; }

class LuaEngine
{
private:
	lua_State * L;
	Mutex m_Lock;

public:
	LuaEngine();
	~LuaEngine();

	lua_State* GetMainLuaState() { return L; }
	void LoadScripts();
	void ScriptLoadDir(char* Dirname, LUALoadScripts *pak);
	void Shutdown();
	void Restart();
	void RegisterCoreFunctions();
	HEARTHSTONE_INLINE Mutex& GetLock() { return m_Lock; }

	void BeginCall(uint16);
	HEARTHSTONE_INLINE bool ExecuteCall(uint8 params = 0,uint8 res = 0);
	HEARTHSTONE_INLINE void EndCall(uint8 res = 0);

	void PushUnit(Object * unit, lua_State * LuaS = NULL);
	void PushGo(Object * go, lua_State * LuaS = NULL);
	void PushItem(Object * item, lua_State * LuaS = NULL);
	void PushGuid(uint64 guid, lua_State * LuaS = NULL);
	void PushPacket(WorldPacket * packet, lua_State * LuaS = NULL);
	void PushTaxiPath(TaxiPath * tp, lua_State * LuaS = NULL);
	void PushSpell(Spell * sp, lua_State * LuaS = NULL);
	void PushSQLField(Field * field, lua_State * LuaS = NULL);
	void PushSQLResult(QueryResult * res, lua_State * LuaS = NULL);
	void PushAura(Aura * aura, lua_State * LuaS = NULL);

	HEARTHSTONE_INLINE void PushBool(bool bewl)
	{
		if(bewl) 
			lua_pushboolean(L,1);
		else 
			lua_pushboolean(L,0);
	}

	HEARTHSTONE_INLINE void PushNil(lua_State * LuaS = NULL)
	{
		if(LuaS == NULL)
			lua_pushnil(L);
		else
			lua_pushnil(LuaS);
	}

	HEARTHSTONE_INLINE void PushInt(int32 value) { lua_pushinteger(L, value); }
	HEARTHSTONE_INLINE void PushUint(uint32 value) { lua_pushnumber(L, value); }
	HEARTHSTONE_INLINE void PushFloat(float value) { lua_pushnumber(L, value); }
	HEARTHSTONE_INLINE void PushString(const char * str) { lua_pushstring(L, str); }
};

class LuaEngineMgr
{
private:
	typedef HM_NAMESPACE::hash_map<uint32, LuaUnitBinding> UnitBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaQuestBinding> QuestBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGameObjectBinding> GameObjectBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaUnitGossipBinding> GossipUnitScriptsBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaItemGossipBinding> GossipItemScriptsBindingMap;
	typedef HM_NAMESPACE::hash_map<uint32, LuaGOGossipBinding> GossipGOScriptsBindingMap;

	//maps to creature, & go script interfaces
	std::multimap<uint32, LuaCreature*> m_cAIScripts;
	std::multimap<uint32, LuaGameObject*> m_gAIScripts;
	HM_NAMESPACE::hash_map<uint32, LuaQuest*> m_qAIScripts;
//	HM_NAMESPACE::hash_map<uint32, LuaInstance*> m_iAIScripts;
	HM_NAMESPACE::hash_map<uint32, LuaGossip*> m_unitgAIScripts;
	HM_NAMESPACE::hash_map<uint32, LuaGossip*> m_itemgAIScripts;
	HM_NAMESPACE::hash_map<uint32, LuaGossip*> m_gogAIScripts;

	std::set<int> m_pendingThreads;
	std::set<int> m_functionRefs;
	std::map< uint64,std::set<int> > m_objectFunctionRefs;

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
//	void ReloadScripts();
	lua_State* GLuas() { return m_engine->GetMainLuaState(); }
	void RegisterEvent(uint8, uint32, uint32 , uint16);
	void ResumeLuaThread(int);
	void HyperCallFunction(const char *, int);
	void CallFunctionByReference(int);
	void DestroyAllLuaEvents();

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

	HEARTHSTONE_INLINE std::set<int> & getThreadRefs() { return m_pendingThreads; }
	HEARTHSTONE_INLINE std::set<int> & getFunctionRefs() { return m_functionRefs; }
	HEARTHSTONE_INLINE hash_map<uint32, LuaGossip*> & getUnitGossipInterfaceMap() { return m_unitgAIScripts; }
	HEARTHSTONE_INLINE hash_map<uint32, LuaGossip*> & getItemGossipInterfaceMap() { return m_itemgAIScripts; }
	HEARTHSTONE_INLINE hash_map<uint32, LuaGossip*> & getGameObjectGossipInterfaceMap() { return m_gogAIScripts; }
	HEARTHSTONE_INLINE std::map< uint64,std::set<int> > & getObjectFunctionRefs() { return m_objectFunctionRefs; }
	hash_map<int, EventInfoHolder*> m_registeredTimedEvents;

	class luEventMgr : public EventableObject
	{
	public:
		bool HasEvent(int ref)
		{
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.find(ref);
			return (itr != g_luaMgr.m_registeredTimedEvents.end());
		}

		bool HasEventInTable(const char * table)
		{
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.begin();
			for (; itr != g_luaMgr.m_registeredTimedEvents.end(); ++itr)
			{
				if (strncmp(itr->second->funcName, table, strlen(table)) == 0)
				{
					return true;
				}
			}
			return false;
		}

		bool HasEventWithName(const char * name)
		{
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.begin();
			for (; itr != g_luaMgr.m_registeredTimedEvents.end(); ++itr)
			{
				if (strcmp(itr->second->funcName, name) == 0)
				{
					return true;
				}
			}
			return false;
		}

		void RemoveEventsInTable(const char * table)
		{
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.begin(), itr2;
			for (; itr != g_luaMgr.m_registeredTimedEvents.end(); )
			{
				itr2 = itr++;
				if (strncmp(itr2->second->funcName, table, strlen(table)) == 0)
				{
					event_RemoveByPointer(itr2->second->te);
					free((void*)itr2->second->funcName);
					luaL_unref(g_luaMgr.GLuas(), LUA_REGISTRYINDEX, itr2->first);
					g_luaMgr.m_registeredTimedEvents.erase(itr2);
				}
			}
		}

		void RemoveEventsByName(const char * name)
		{
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.begin(), itr2;
			for (; itr != g_luaMgr.m_registeredTimedEvents.end(); )
			{
				itr2 = itr++;
				if (strcmp(itr2->second->funcName, name) == 0)
				{
					event_RemoveByPointer(itr2->second->te);
					free((void*)itr2->second->funcName);
					luaL_unref(g_luaMgr.GLuas(), LUA_REGISTRYINDEX, itr2->first);
					g_luaMgr.m_registeredTimedEvents.erase(itr2);
				}
			}
		}

		void RemoveEventByRef(int ref)
		{
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.find(ref);
			if (itr != g_luaMgr.m_registeredTimedEvents.end())
			{
				event_RemoveByPointer(itr->second->te);
				free((void*)itr->second->funcName);
				luaL_unref(g_luaMgr.GLuas(), LUA_REGISTRYINDEX, itr->first);
				g_luaMgr.m_registeredTimedEvents.erase(itr);
			}
		}

		void RemoveEvents()
		{
			event_RemoveEvents(EVENT_LUA_TIMED);
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.begin();
			for (; itr != g_luaMgr.m_registeredTimedEvents.end(); ++itr)
			{
				free((void*)itr->second->funcName);
				luaL_unref(g_luaMgr.GLuas(), LUA_REGISTRYINDEX, itr->first);
			}
			g_luaMgr.m_registeredTimedEvents.clear();
		}

	} LuaEventMgr;
};

template<> const char * GetTClassName<Item>() { return "Item"; };
template<> const char * GetTClassName<Unit>() {	return "Unit"; };
template<> const char * GetTClassName<GameObject>() { return "GameObject"; };
template<> const char * GetTClassName<WorldPacket>() { return "LuaPacket"; }
template<> const char * GetTClassName<TaxiPath>() { return "LuaTaxi"; }
template<> const char * GetTClassName<Spell>() { return "Spell"; }
template<> const char * GetTClassName<Field>() { return "SQL_Field"; }
template<> const char * GetTClassName<QueryResult>() { return "SQL_QResult"; }
template<> const char * GetTClassName<Aura>() { return "LuaAura"; }

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

// LuaHypArc commands(With additions)
#define TEST_ITEM() if(ptr == NULL || !ptr->IsInWorld() || (ptr->GetProto() == NULL) || (ptr->GetTypeId() != TYPEID_ITEM && ptr->GetTypeId() != TYPEID_CONTAINER)) { return 0; }
#define TEST_ITEM_RET() if(ptr == NULL || !ptr->IsInWorld() || (ptr->GetProto() == NULL) || (ptr->GetTypeId() != TYPEID_ITEM && ptr->GetTypeId() != TYPEID_CONTAINER)) { lua_pushboolean(L,0); return 1; }

#define TEST_UNIT() if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_UNIT) { return 0; }
#define TEST_UNIT_RET() if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_UNIT) { lua_pushboolean(L,0); return 1; }

#define TEST_PLAYER() if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_PLAYER) { return 0; }
#define TEST_PLAYER_RET() if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_PLAYER) { lua_pushboolean(L,0); return 1; }

#define TEST_UNITPLAYER() if(ptr == NULL || !ptr->IsInWorld() || ( ptr->GetTypeId() != TYPEID_PLAYER && ptr->GetTypeId() != TYPEID_UNIT)) { return 0; }
#define TEST_UNITPLAYER_RET() if(ptr == NULL || !ptr->IsInWorld() || ( ptr->GetTypeId() != TYPEID_PLAYER && ptr->GetTypeId() != TYPEID_UNIT)) { lua_pushboolean(L,0); return 1; }

#define TEST_GO() if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_GAMEOBJECT || !ptr->GetInfo()) { return 0; }
#define TEST_GO_RET() if(ptr == NULL || !ptr->IsInWorld() || ptr->GetTypeId() != TYPEID_GAMEOBJECT) { lua_pushboolean(L,0); return 1; }

#define RET_NIL( ){ lua_pushnil(L); return 1; }
#define RET_BOOL(exp) { (exp) ? lua_pushboolean(L,1) : lua_pushboolean(L,0); return 1; }
#define RET_STRING(str) { lua_pushstring(L,(str)); return 1; }
#define RET_NUMBER(number) { lua_pushnumber(L,(number)); return 1; }
#define RET_INT(integer) { lua_pushinteger(L,(integer)); return 1; }

#define CHECK_ITEM(L,narg) Lunar<Item>::check(L,narg)
#define CHECK_UNIT(L,narg) Lunar<Unit>::check(L,narg)
#define CHECK_PLAYER(L,narg) Lunar<Player>::check(L,narg)
#define CHECK_GO(L,narg) Lunar<GameObject>::check(L,narg)
#define CHECK_PACKET(L,narg) Lunar<WorldPacket>::check(L,narg)
#define CHECK_OBJECT(L,narg) Lunar<Object>::check(L,narg)

#define CHECK_FLOAT(L,narg) (lua_isnoneornil(L,(narg)) ) ? 0.00f : (float)luaL_checknumber(L,(narg)); 
#define CHECK_ULONG(L,narg) (uint32)luaL_checknumber((L),(narg))
#define CHECK_USHORT(L, narg) (uint16)luaL_checkinteger((L),(narg))
#define CHECK_BOOL(L,narg) (lua_toboolean((L),(narg)) > 0) ? true : false
#define CREATE_L_PTR lua_State* L = GLuas();

class GuidMgr
{
	static const char * GetName() { return "GuidMgr"; }
public:

	static void Register(lua_State * L)
	{
		luaL_newmetatable(L,GetName());
		int mt = lua_gettop(L);
		//Hide metatable.
		lua_pushnil(L);
		lua_setfield(L,mt,"__metatable");
		//nil gc method
		lua_pushnil(L);
		lua_setfield(L,mt,"__gc");
		//set our tostring method
		lua_pushcfunction(L,_tostring);
		lua_setfield(L,mt,"__tostring");
		//nil __index field
		lua_pushnil(L);
		lua_setfield(L,mt,"__index");
		//set __newindex method
		lua_pushcfunction(L,_newindex);
		lua_setfield(L,mt,"__newindex");
		//no call method
		lua_pushnil(L);
		lua_setfield(L,mt,"__call");
		//pop metatable
		lua_pop(L,1);
	}

	static uint64 check(lua_State * L, int narg) 
	{
		uint64 GUID = 0;
		uint64 * ptrHold = (uint64*)lua_touserdata(L,narg);
		if(ptrHold != NULL)
			GUID = *ptrHold;
		return GUID;
	}

	static int push(lua_State *L, uint64 guid)
	{
		int index = 0;
		if(guid == 0) 
		{
			lua_pushnil(L);
			index = lua_gettop(L);
		}
		else
		{
			luaL_getmetatable(L,GetName());
			if(lua_isnoneornil(L,-1) )
				luaL_error(L,"%s metatable not found!.",GetName());
			else 
			{
				int mt = lua_gettop(L);
				uint64* guidHold = (uint64*)lua_newuserdata(L,sizeof(uint64));
				int ud = lua_gettop(L);
				if(guidHold == NULL)
					luaL_error(L,"Lua tried to allocate size %d of memory and failed!",sizeof(uint64*));
				else
				{
					(*guidHold) = guid;
					lua_pushvalue(L,mt);
					lua_setmetatable(L,ud);
					lua_replace(L,mt);
					lua_settop(L,mt);
					index = mt;
				}
			}
		}
		return index;
	}

private:
	GuidMgr() {}

	//This method prints formats the GUID in hexform and pushes to the stack.
	static int _tostring(lua_State * L) 
	{
		uint64 GUID = GuidMgr::check(L,1);
		if(GUID == 0)
			lua_pushnil(L);
		else {
			char buff[32];
			sprintf(buff,"%X",GUID);
			lua_pushfstring(L,"%s",buff);
		}
		return 1;
	}

	static int _newindex(lua_State *L) 
	{
		//Remove table, key, and value
		lua_remove(L,1);
		lua_remove(L,1);
		lua_remove(L,1);
		luaL_error(L,"OPERATION PROHIBITED!");
		return 0;
	}
};

#endif

