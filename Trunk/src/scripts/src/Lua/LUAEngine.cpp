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

#include "StdAfx.h"
#include "LUAEngine.h"
#include <ScriptSetup.h>

#if PLATFORM != PLATFORM_WIN32
#include <dirent.h>
#endif

bool lua_is_starting_up = false;
ScriptMgr * m_scriptMgr;
LuaEngineMgr g_luaMgr;
LuaEngine * g_engine;

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_SCRIPT_ENGINE | SCRIPT_TYPE_SCRIPT_ENGINE_LUA;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	m_scriptMgr = mgr;
	g_luaMgr.Startup();
}

void report(lua_State * L)
{
	const char * msg= lua_tostring(L,-1);
	while(msg)
	{
		lua_pop(L,-1);
		printf("\t%s\n", msg);
		msg=lua_tostring(L,-1);
	}
}

LuaEngine::LuaEngine()
{
	this->L = lua_open();
}

LuaEngine::~LuaEngine()
{
	lua_close(L);
}

void LuaEngine::LoadScripts()
{
	unordered_set<string> luaFiles;
	unordered_set<string> luaBytecodeFiles;

#ifdef WIN32
	WIN32_FIND_DATA fd;
	HANDLE h;

	h = FindFirstFile("scripts\\*.*", &fd);
	if(h == INVALID_HANDLE_VALUE)
		return;

	do 
	{
		if(fd.dwFileAttributes  & FILE_ATTRIBUTE_DIRECTORY)
			continue;
		char * fn = strrchr(fd.cFileName, '\\');
		if(!fn)
			fn=fd.cFileName;
        char * ext = strrchr(fd.cFileName, '.');
		if(!stricmp(ext, ".lua"))
			luaFiles.insert(string(fn));
		else if(!stricmp(ext, ".luc"))
			luaBytecodeFiles.insert(string(fn));
	} while(FindNextFile(h, &fd));
	FindClose(h);
#else
	struct dirent ** list;
	int filecount = scandir("./scripts", &list, 0, 0);
	if(filecount <= 0 || !list)
		return;

	while(filecount--)
	{
		char* ext = strrchr(list[filecount]->d_name, '.');
		if(ext != NULL && !strcmp(ext, ".lua"))
			{
				string full_path = string(list[filecount]->d_name);
				luaFiles.insert(string(full_path.c_str()));
		}
		else if(!stricmp(ext, ".luc"))
		{
		string full_path = string(list[filecount]->d_name);
		luaBytecodeFiles.insert(string(full_path.c_str()));
		}

		free(list[filecount]);
	}
	free(list);
#endif

	// we prefer precompiled code.
	for(unordered_set<string>::iterator itr = luaBytecodeFiles.begin(); itr != luaBytecodeFiles.end(); ++itr)
	{
		unordered_set<string>::iterator it2 = luaFiles.find(*itr);
		if(it2 == luaFiles.end())
			luaFiles.erase(it2);
	}

	luaL_openlibs(L);
	RegisterCoreFunctions();

	if(lua_is_starting_up)
		Log.Notice("LuaEngine", "Loading Scripts...");

	char filename[200];

	for(unordered_set<string>::iterator itr = luaFiles.begin(); itr != luaFiles.end(); ++itr)
	{
#ifdef WIN32
			snprintf(filename, 200, "scripts\\%s", itr->c_str());
#else
			snprintf(filename, 200, "scripts/%s", itr->c_str());
#endif
		if(lua_is_starting_up)
			Log.Notice("LuaEngine", "%s...", itr->c_str());

		if(luaL_loadfile(L, filename) != 0)
		{
			printf("failed. (could not load)\n");
			const char * msg = lua_tostring(L, -1);
			if(msg!=NULL&&lua_is_starting_up)
				printf("\t%s\n",msg);
		}
		else
		{
			if(lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
			{
				printf("failed. (could not run)\n");
				const char * msg = lua_tostring(L, -1);
				if(msg!=NULL&&lua_is_starting_up)
					printf("\t%s\n",msg);
			}
		}
	}
}

void LuaEngine::OnGossipEvent(Object* pObject, const char * FunctionName, uint32 EventType, Player* mPlayer, uint32 Id, uint32 IntId, const char * Code)
{
    if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L, -1))
	{
		printf("Tried to call invalid LUA function '%s' from Sandshroud (Gossip)!\n", FunctionName);
		m_Lock.Release();
		return;
	}

    if(pObject->GetTypeId() == TYPEID_UNIT)
        Lunar<Unit>::push(L, TO_UNIT(pObject));

    else if(pObject->GetTypeId() == TYPEID_ITEM)
        Lunar<Item>::push(L, TO_ITEM(pObject));

    else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
        Lunar<GameObject>::push(L, TO_GAMEOBJECT(pObject));

    lua_pushinteger(L, EventType);
	Lunar<Unit>::push(L, mPlayer);

    lua_pushinteger(L, Id);
    lua_pushinteger(L, IntId);

    lua_pushstring(L, Code);

	int r = lua_pcall(L, 6, LUA_MULTRET, 0);
	if(r)
		report(L);

	m_Lock.Release();
}

void LuaEngine::OnUnitEvent(Unit * pUnit, const char * FunctionName, uint32 EventType, Unit * pMiscUnit, uint32 Misc)
{
	if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Sandshroud (Unit)!\n", FunctionName);
		m_Lock.Release();
		return;
	}

	Lunar<Unit>::push(L, pUnit);
	lua_pushinteger(L,EventType);
	if(pMiscUnit!=NULL)
		Lunar<Unit>::push(L, pMiscUnit);
	else
		lua_pushnil(L);
	lua_pushinteger(L,Misc);
	
	int r = lua_pcall(L,4,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

void LuaEngine::OnQuestEvent(Player * QuestOwner, const char * FunctionName, uint32 QuestID, uint32 EventType, Object * QuestStarter)
{
	if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Sandshroud (Quest)!\n", FunctionName);
		m_Lock.Release();
		return;
	}

	if (QuestOwner)
		Lunar<Unit>::push(L, (Unit*)QuestOwner);
	else
		lua_pushnil(L);

	lua_pushinteger(L,EventType);

	if(QuestStarter!=NULL && QuestStarter->GetTypeId() == TYPEID_UNIT)
		Lunar<Unit>::push(L, (Unit*)QuestStarter);
	else
		lua_pushnil(L);

	int r = lua_pcall(L,3,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
	
}
void LuaEngine::CallFunction(Unit * pUnit, const char * FuncName)
{
	m_Lock.Acquire();
	lua_pushstring(L, FuncName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Sandshroud (Unit)!\n", FuncName);
		m_Lock.Release();
		return;
	}

	Lunar<Unit>::push(L, pUnit);
	int r = lua_pcall(L,1,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

void LuaEngine::OnGameObjectEvent(GameObject * pGameObject, const char * FunctionName, uint32 EventType, Unit * pMiscUnit)
{
	if(FunctionName==NULL)
		return;

	m_Lock.Acquire();
	lua_pushstring(L, FunctionName);
	lua_gettable(L, LUA_GLOBALSINDEX);
	if(lua_isnil(L,-1))
	{
		printf("Tried to call invalid LUA function '%s' from Sandshroud! (GO)\n", FunctionName);
		m_Lock.Release();
		return;
	}

	Lunar<GameObject>::push(L, pGameObject);
	lua_pushinteger(L,EventType);
	if(!pMiscUnit)
		lua_pushnil(L);
	else
		Lunar<Unit>::push(L, pMiscUnit);

	int r = lua_pcall(L,3,LUA_MULTRET,0);
	if(r)
		report(L);

	m_Lock.Release();
}

static int RegisterUnitEvent(lua_State * L);
static int RegisterQuestEvent(lua_State * L);
static int RegisterGameObjectEvent(lua_State * L);
static int RegisterUnitGossipEvent(lua_State * L);
static int RegisterItemGossipEvent(lua_State * L);
static int RegisterGOGossipEvent(lua_State * L);

void LuaEngine::RegisterCoreFunctions()
{
	lua_pushcfunction(L, &RegisterUnitEvent);
	lua_setglobal(L, "RegisterUnitEvent");

	lua_pushcfunction(L, &RegisterGameObjectEvent);
	lua_setglobal(L, "RegisterGameObjectEvent");

	lua_pushcfunction(L, &RegisterQuestEvent);
	lua_setglobal(L, "RegisterQuestEvent");

	// Unit, Item, GO gossip stuff
	lua_pushcfunction(L, &RegisterUnitGossipEvent);
	lua_setglobal(L, "RegisterUnitGossipEvent"); 
	
	lua_pushcfunction(L, &RegisterItemGossipEvent);
	lua_setglobal(L, "RegisterItemGossipEvent"); 
 
	lua_pushcfunction(L, &RegisterGOGossipEvent);
	lua_setglobal(L, "RegisterGOGossipEvent");

	Lunar<Item>::Register(L);
	Lunar<Unit>::Register(L);
	Lunar<GameObject>::Register(L);
	//Lunar<Quest>::Register(L); quest isn't a class
}

static int RegisterUnitEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterUnitEvent(entry,ev,str);
	return 0;
}

static int RegisterQuestEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterQuestEvent(entry,ev,str);
	return 0;
}

static int RegisterGameObjectEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

	g_luaMgr.RegisterGameObjectEvent(entry,ev,str);
	return 0;
}

// Gossip stuff
static int RegisterUnitGossipEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);
 
	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

    g_luaMgr.RegisterUnitGossipEvent(entry, ev, str);
	return 0;
}

static int RegisterItemGossipEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);
	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

    g_luaMgr.RegisterItemGossipEvent(entry, ev, str);
	return 0;
}

static int RegisterGOGossipEvent(lua_State * L)
{
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * str = luaL_checkstring(L, 3);

	if(!entry || !ev || !str || !lua_is_starting_up)
		return 0;

    g_luaMgr.RegisterGOGossipEvent(entry, ev, str);
	return 0;
}


/************************************************************************/
/* Manager Stuff                                                        */
/************************************************************************/

class LuaGossip : public GossipScript
{
public:
	LuaGossip() : GossipScript() {}
	~LuaGossip() {}

	void GossipHello(Object* pObject, Player* Plr, bool AutoSend) // Dont need GossipHello for GO's
	{
        if(pObject->GetTypeId() == TYPEID_UNIT)
        {
            if( m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK] != NULL )
			    g_engine->OnGossipEvent( pObject, m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK], GOSSIP_EVENT_ON_TALK, Plr, NULL, NULL, NULL );
        }
        else if(pObject->GetTypeId() == TYPEID_ITEM)
        {
            if( m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK] != NULL )
			    g_engine->OnGossipEvent( pObject, m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK], GOSSIP_EVENT_ON_TALK, Plr, NULL, NULL, NULL );
        }
	}

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
        if(pObject->GetTypeId() == TYPEID_UNIT)
        {
            if( m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION] != NULL )
			    g_engine->OnGossipEvent( pObject, m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION], GOSSIP_EVENT_ON_SELECT_OPTION, Plr, Id, IntId, EnteredCode);
        }
        else if(pObject->GetTypeId() == TYPEID_ITEM)
        {
            if( m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION] != NULL )
                g_engine->OnGossipEvent( pObject, m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION], GOSSIP_EVENT_ON_SELECT_OPTION, Plr, Id, IntId, EnteredCode);
        }
        else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
       {
            if( m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION] != NULL )
                g_engine->OnGossipEvent( pObject, m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION], GOSSIP_EVENT_ON_SELECT_OPTION, Plr, Id, IntId, EnteredCode);
        }
	}

	void GossipEnd(Object* pObject, Player* Plr)
	{
        if(pObject->GetTypeId() == TYPEID_UNIT)
        {
		    if( m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_END] != NULL )
			    g_engine->OnGossipEvent( pObject, m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_END], GOSSIP_EVENT_ON_END, Plr, NULL, NULL, NULL );
       }
        else if(pObject->GetTypeId() == TYPEID_ITEM)
        {
            if( m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_END] != NULL )
			    g_engine->OnGossipEvent( pObject, m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_END], GOSSIP_EVENT_ON_END, Plr, NULL, NULL, NULL );
        }
       else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
        {
            if( m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_END] != NULL )
			    g_engine->OnGossipEvent( pObject, m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_END], GOSSIP_EVENT_ON_END, Plr, NULL, NULL, NULL );
       }
	}

	LuaUnitGossipBinding * m_unit_gossip_binding;
	LuaItemGossipBinding * m_item_gossip_binding;
    LuaGOGossipBinding * m_go_gossip_binding;
};

class LuaCreature : public CreatureAIScript
{
public:
	LuaCreature(Creature* creature) : CreatureAIScript(creature) {};
	~LuaCreature() {};

	void OnCombatStart(Unit* mTarget)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_ENTER_COMBAT] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_ENTER_COMBAT], CREATURE_EVENT_ON_ENTER_COMBAT, mTarget, 0 );
	}

	void OnCombatStop(Unit* mTarget)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_LEAVE_COMBAT] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_LEAVE_COMBAT], CREATURE_EVENT_ON_LEAVE_COMBAT, mTarget, 0 );
	}

	void OnTargetDied(Unit* mTarget)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_KILLED_TARGET] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_KILLED_TARGET], CREATURE_EVENT_ON_KILLED_TARGET, mTarget, 0 );
	}

	void OnDied(Unit *mKiller)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_DIED] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_DIED], CREATURE_EVENT_ON_DIED, mKiller, 0 );
	}

	void OnLoad()
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_SPAWN] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_SPAWN], CREATURE_EVENT_ON_SPAWN, NULL, 0 );
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		if( m_binding->Functions[CREATURE_EVENT_ON_REACH_WP] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_ON_REACH_WP], CREATURE_EVENT_ON_REACH_WP, NULL, iWaypointId );
	}

	void AIUpdate()
	{
		if( m_binding->Functions[CREATURE_EVENT_AI_TICK] != NULL )
			g_engine->OnUnitEvent( _unit, m_binding->Functions[CREATURE_EVENT_AI_TICK], CREATURE_EVENT_AI_TICK, NULL, 0 );
	}

	void StringFunctionCall(const char * pFunction)
	{
		g_engine->CallFunction( _unit, pFunction );
	}

	void Destroy()
	{
		delete this;
	}

	LuaUnitBinding * m_binding;
};

class LuaGameObject : public GameObjectAIScript
{
public:
	LuaGameObject(GameObject * go) : GameObjectAIScript(go) {}
	~LuaGameObject() {}

	void OnSpawn()
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN] != NULL )
			g_engine->OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN], GAMEOBJECT_EVENT_ON_SPAWN, NULL );
	}

	void OnActivate(Player * pPlayer)
	{
		if( m_binding->Functions[GAMEOBJECT_EVENT_ON_USE] != NULL )
			g_engine->OnGameObjectEvent( _gameobject, m_binding->Functions[GAMEOBJECT_EVENT_ON_USE], GAMEOBJECT_EVENT_ON_USE, pPlayer );
	}

	LuaGameObjectBinding * m_binding;
};

class LuaQuest : public QuestScript
{
public:
	LuaQuest() : QuestScript() {}
	~LuaQuest() {}

	void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_ACCEPT] != NULL )
			g_engine->OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_ACCEPT], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_ACCEPT, mTarget );
	}

	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		if( m_binding->Functions[QUEST_EVENT_ON_COMPLETE] != NULL )
			g_engine->OnQuestEvent( mTarget, m_binding->Functions[QUEST_EVENT_ON_COMPLETE], qLogEntry->GetQuest()->id, QUEST_EVENT_ON_COMPLETE, mTarget );
	}

	LuaQuestBinding * m_binding;
};

CreatureAIScript * CreateLuaCreature(Creature * src)
{
	LuaUnitBinding * pBinding = g_luaMgr.GetUnitBinding( src->GetEntry() );
	if( pBinding == NULL )
		return NULL;

	LuaCreature * pLua = new LuaCreature( src );
	pLua->m_binding = pBinding;
	return pLua;
}

GameObjectAIScript * CreateLuaGameObject(GameObject * src)
{
	LuaGameObjectBinding * pBinding = g_luaMgr.GetGameObjectBinding( src->GetEntry() );
	if( pBinding == NULL )
		return NULL;

	LuaGameObject * pLua = new LuaGameObject( src );
	pLua->m_binding = pBinding;
	return pLua;
}

QuestScript * CreateLuaQuestScript(uint32 id)
{
	LuaQuestBinding * pBinding = g_luaMgr.GetQuestBinding( id );
	if( pBinding == NULL )
		return NULL;

	LuaQuest * pLua = new LuaQuest();
	pLua->m_binding = pBinding;
	return pLua;
}
// Gossip stuff
GossipScript * CreateLuaUnitGossipScript(uint32 id)
{
    LuaUnitGossipBinding * pBinding = g_luaMgr.GetLuaUnitGossipBinding( id );
	if( pBinding == NULL )
		return NULL;
 
	LuaGossip * pLua = new LuaGossip();
	pLua->m_unit_gossip_binding = pBinding;
	return pLua;
}

GossipScript * CreateLuaItemGossipScript(uint32 id)
{
    LuaItemGossipBinding * pBinding = g_luaMgr.GetLuaItemGossipBinding( id );
	if( pBinding == NULL )
		return NULL;

	LuaGossip * pLua = new LuaGossip();
	pLua->m_item_gossip_binding = pBinding;
	return pLua;
}

GossipScript * CreateLuaGOGossipScript(uint32 id)
{
    LuaGOGossipBinding * pBinding = g_luaMgr.GetLuaGOGossipBinding( id );
	if( pBinding == NULL )
		return NULL;

	LuaGossip * pLua = new LuaGossip();
    pLua->m_go_gossip_binding = pBinding;
	return pLua;
}

void LuaEngineMgr::Startup()
{
	Log.Notice("LuaEngineMgr", "Spawning Lua Engine...");
	m_engine = new LuaEngine();
	lua_is_starting_up = true;
	m_engine->LoadScripts();
	g_engine = m_engine;
	lua_is_starting_up = false;

	// stuff is registered, so lets go ahead and make our emulated C++ scripted lua classes.
	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		m_scriptMgr->register_creature_script( itr->first, CreateLuaCreature );
	}

	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		m_scriptMgr->register_gameobject_script( itr->first, CreateLuaGameObject );
	}

	for(QuestBindingMap::iterator itr = m_questBinding.begin(); itr != m_questBinding.end(); ++itr)
	{
		QuestScript * qs = CreateLuaQuestScript( itr->first );
		if( qs != NULL )
			m_scriptMgr->register_quest_script( itr->first, qs );
	}
    // Register Gossip Stuff
    for(GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.begin(); itr != m_unit_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaUnitGossipScript( itr->first );
	if( gs != NULL )
			m_scriptMgr->register_gossip_script( itr->first, gs );
    }

    for(GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.begin(); itr != m_item_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaItemGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_item_gossip_script( itr->first, gs );
    }

    for(GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.begin(); itr != m_go_gossipBinding.end(); ++itr)
	{
		GossipScript * gs = CreateLuaGOGossipScript( itr->first );
		if( gs != NULL )
			m_scriptMgr->register_go_gossip_script( itr->first, gs );
    }
}

void LuaEngineMgr::RegisterUnitEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	UnitBindingMap::iterator itr = m_unitBinding.find(Id);
	if(itr == m_unitBinding.end())
	{
		LuaUnitBinding ub;
		memset(&ub,0,sizeof(LuaUnitBinding));
		ub.Functions[Event] = strdup(FunctionName);
		m_unitBinding.insert(make_pair(Id,ub));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

void LuaEngineMgr::RegisterQuestEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	QuestBindingMap::iterator itr = m_questBinding.find(Id);
	if(itr == m_questBinding.end())
	{
		LuaQuestBinding qb;
		memset(&qb,0,sizeof(LuaQuestBinding));
		qb.Functions[Event] = strdup(FunctionName);
		m_questBinding.insert(make_pair(Id,qb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}
void LuaEngineMgr::RegisterGameObjectEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
	GameObjectBindingMap::iterator itr = m_gameobjectBinding.find(Id);
	if(itr == m_gameobjectBinding.end())
	{
		LuaGameObjectBinding ub;
		memset(&ub,0,sizeof(LuaGameObjectBinding));
		ub.Functions[Event] = strdup(FunctionName);
		m_gameobjectBinding.insert(make_pair(Id,ub));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}
// Gossip Events
void LuaEngineMgr::RegisterUnitGossipEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
    GossipUnitScriptsBindingMap::iterator itr = m_unit_gossipBinding.find(Id);
 
    if(itr == m_unit_gossipBinding.end())
	{
		LuaUnitGossipBinding gb;
		memset(&gb, 0, sizeof(LuaUnitGossipBinding));
		gb.Functions[Event] = strdup(FunctionName);
		m_unit_gossipBinding.insert(make_pair(Id, gb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

void LuaEngineMgr::RegisterItemGossipEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
    GossipItemScriptsBindingMap::iterator itr = m_item_gossipBinding.find(Id);

    if(itr == m_item_gossipBinding.end())
	{
		LuaItemGossipBinding gb;
		memset(&gb, 0, sizeof(LuaItemGossipBinding));
		gb.Functions[Event] = strdup(FunctionName);
		m_item_gossipBinding.insert(make_pair(Id, gb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

void LuaEngineMgr::RegisterGOGossipEvent(uint32 Id, uint32 Event, const char * FunctionName)
{
    GossipGOScriptsBindingMap::iterator itr = m_go_gossipBinding.find(Id);

    if(itr == m_go_gossipBinding.end())
	{
		LuaGOGossipBinding gb;
		memset(&gb, 0, sizeof(LuaGOGossipBinding));
		gb.Functions[Event] = strdup(FunctionName);
		m_go_gossipBinding.insert(make_pair(Id, gb));
	}
	else
	{
		if(itr->second.Functions[Event]!=NULL)
			free((void*)itr->second.Functions[Event]);

		itr->second.Functions[Event]=strdup(FunctionName);
	}
}

/*void LuaEngineMgr::ReloadScripts()
{
	m_lock.Acquire();

	// acquire the locks on all the luaengines so they don't do anything.
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
		itr->first->GetLock().Acquire();

	// remove all the function name bindings
	for(UnitBindingMap::iterator itr = m_unitBinding.begin(); itr != m_unitBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < CREATURE_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}
	
	for(GameObjectBindingMap::iterator itr = m_gameobjectBinding.begin(); itr != m_gameobjectBinding.end(); ++itr)
	{
		for(uint32 i = 0; i < GAMEOBJECT_EVENT_COUNT; ++i)
			if(itr->second.Functions[i] != NULL)
				free((void*)itr->second.Functions[i]);
	}

	// clear the maps
	m_gameobjectBinding.clear();
	m_unitBinding.clear();

	// grab the first lua engine in the list, use it to re-create all the binding names.
	LuaEngine * l = m_engines.begin()->first;
	lua_is_starting_up = true;
	l->Restart();
	lua_is_starting_up = false;

	// all our bindings have been re-created, go through the lua engines and restart them all, and then release their locks.
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
	{
		if(itr->first != l)		// this one is already done
		{
			itr->first->Restart();
			itr->first->GetLock().Release();
		}
	}

	// release the big lock
	m_lock.Release();
}*/

void LuaEngineMgr::Unload()
{
	/*m_lock.Acquire();
	for(LuaEngineMap::iterator itr = m_engines.begin(); itr != m_engines.end(); ++itr)
	{
		delete itr->first;
	}
	m_lock.Release();*/
}

void LuaEngine::Restart()
{
	m_Lock.Acquire();
	lua_close(L);
	L = lua_open();
	LoadScripts();
	m_Lock.Release();
}

// Crow: We could do compile with, but...
#include "Functions/FunctionItems.cpp"
#include "Functions/FunctionUnits.cpp"
#include "Functions/FunctionGameObjects.cpp"
