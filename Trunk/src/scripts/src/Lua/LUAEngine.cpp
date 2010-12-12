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

void LuaEngine::ScriptLoadDir(char* Dirname, LUALoadScripts *pak)
{
#ifdef WIN32

	HANDLE hFile;
	WIN32_FIND_DATA FindData;
	memset(&FindData,0,sizeof(FindData));

	char SearchName[MAX_PATH];

	strcpy(SearchName,Dirname);
	strcat(SearchName,"\\*.*");

	hFile = FindFirstFile(SearchName,&FindData);
	FindNextFile(hFile, &FindData);

	while( FindNextFile(hFile, &FindData) )
	{
		if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) //Credits for this 'if' go to Cebernic from ArcScripts Team. Thanks, you saved me some work ;-)
		{
			strcpy(SearchName,Dirname);
			strcat(SearchName,"\\");
			strcat(SearchName,FindData.cFileName);
			ScriptLoadDir(SearchName, pak);
		}
		else
		{
			string fname = Dirname;
			fname += "\\";
			fname += FindData.cFileName;

			int len = int(strlen(fname.c_str()));
			int i = 0;
			char ext[MAX_PATH];

			while(len > 0)
			{  
				ext[i++] = fname[--len];
				if(fname[len] == '.')
					break;
			}
			ext[i++] = '\0';
			if ( !_stricmp(ext,"aul.") ) 
				pak->luaFiles.insert(fname);
		}
	}
	FindClose(hFile);

#else

	char *pch=strrchr(Dirname,'/');
	if (strcmp(Dirname, "..")==0 || strcmp(Dirname, ".")==0) return; //Against Endless-Loop
	if (pch != NULL && (strcmp(pch, "/..")==0 || strcmp(pch, "/.")==0 || strcmp(pch, "/.svn")==0)) return;
	struct dirent ** list;
	int filecount = scandir(Dirname, &list, 0, 0);

	if(filecount <= 0 || !list)
		return;

	struct stat attributes;
	bool err;
	Log.Success("LuaEngine", "Scanning Directory %s", Dirname);
	while(filecount--)
	{
		char dottedrelpath[200];
		sprintf(dottedrelpath, "%s/%s", Dirname, list[filecount]->d_name);
		err = false;
		if(stat(dottedrelpath, &attributes) == -1)
		{
			err = true;
			Log.Error("LuaEngine","Error opening %s: %s\n", dottedrelpath, strerror(errno));
		}

		if (!err && S_ISDIR(attributes.st_mode))
		{
			ScriptLoadDir((char *)dottedrelpath, pak); //Subdirectory
		}
		else
		{
			char* ext = strrchr(list[filecount]->d_name, '.');
			if(ext != NULL && !strcmp(ext, ".lua"))
			{
				pak->luaFiles.insert(dottedrelpath);
			}
		}

		free(list[filecount]);
	}
	free(list);

#endif
}

void LuaEngine::LoadScripts()
{
	LUALoadScripts rtn;
	Log.Notice("LuaEngine", "Scanning Script-Directories...");
	ScriptLoadDir(((char*)sWorld.LuaScriptPath.c_str()), &rtn);

	unsigned int cnt_uncomp = 0;
	luaL_openlibs(L);
	RegisterCoreFunctions();
	Log.Notice("LuaEngine", "Loading Scripts...");

	char filename[200];

	for(set<string>::iterator itr = rtn.luaFiles.begin(); itr != rtn.luaFiles.end(); ++itr)
	{
		strcpy(filename, itr->c_str());
		if(luaL_loadfile(L, filename) != 0)
		{
#ifdef WIN32
				SetConsoleTextAttribute(stdout_handle, (WORD)TRED);
				report(L);
				SetConsoleTextAttribute(stdout_handle, (WORD)TWHITE);
#else
				printf("\033[22;31m");
				report(L);
				printf("\033[01;37m");
#endif
			continue;
		}
		else
		{
			if(lua_pcall(L, 0, 0, 0) != 0)
			{
#ifdef WIN32
				SetConsoleTextAttribute(stdout_handle, (WORD)TRED);
				report(L);
				SetConsoleTextAttribute(stdout_handle, (WORD)TWHITE);
#else
				printf("\033[22;31m");
				report(L);
				printf("\033[01;37m");
#endif
				continue;
			}
		}
		cnt_uncomp++;
	}
	Log.Notice("LuaEngine","Loaded %u Lua scripts.", cnt_uncomp);
}

/*******************************************************************************
	FUNCTION CALL METHODS
*******************************************************************************/
void LuaEngine::BeginCall(uint16 fReference)
{
	lua_settop(L,0); //stack should be empty
	lua_getref(L,fReference);
}

bool LuaEngine::ExecuteCall(uint8 params, uint8 res)
{
	bool ret = true;
	int top = lua_gettop(L);
	if(strcmp(luaL_typename(L,top-params),"function") )
	{
		ret = false;
		//Paroxysm : Stack Cleaning here, not sure what causes that crash in luaH_getstr, maybe due to lack of stack space. Anyway, experimental.
		if(params > 0)
		{
			for(int i = top; i >= (top-params); i--)
			{
				if(!lua_isnone(L,i) )
					lua_remove(L,i);
			}
		}
	}
	else
	{
		if(lua_pcall(L,params,res,0) )
		{
			report(L);
			ret = false;
		}
	}
	return ret;
}

void LuaEngine::EndCall(uint8 res) 
{
	for(int i = res; i > 0; i--)
	{
		if(!lua_isnone(L,res))
			lua_remove(L,res);
	}
}
/*******************************************************************************
	END FUNCTION CALL METHODS
*******************************************************************************/

/******************************************************************************
	PUSH METHODS
******************************************************************************/

void LuaEngine::PushUnit(Object * unit, lua_State * LuaS) 
{
	Unit * pUnit = NULL;
	if(unit != NULL && unit->IsUnit() ) 
		pUnit = TO_UNIT(unit);
	if(LuaS == NULL)
		Lunar<Unit>::push(L,pUnit);
	else
		Lunar<Unit>::push(LuaS,pUnit);
}

void LuaEngine::PushGo(Object *go, lua_State *LuaS)
{
	GameObject * pGo = NULL;
	if(go != NULL && go->IsGameObject() )
		pGo = TO_GAMEOBJECT(go);

	if(LuaS == NULL)
		Lunar<GameObject>::push(L, pGo);
	else
		Lunar<GameObject>::push(LuaS, pGo);
}

void LuaEngine::PushItem(Object * item, lua_State *LuaS)
{
	Item * pItem = NULL;
	if(item != NULL && (item->GetTypeId() == TYPEID_ITEM || item->GetTypeId() == TYPEID_CONTAINER))
		pItem = TO_ITEM(item);

	if(LuaS == NULL)
		Lunar<Item>::push(L, pItem);
	else
		Lunar<Item>::push(LuaS, pItem);
}

void LuaEngine::PushGuid(uint64 guid, lua_State * LuaS) 
{
	if(LuaS == NULL)
		GuidMgr::push(L, guid);
	else
		GuidMgr::push(LuaS, guid);
}

void LuaEngine::PushPacket(WorldPacket * pack, lua_State * LuaS) 
{
	if(LuaS == NULL)
		Lunar<WorldPacket>::push(L, pack, true);
	else
		Lunar<WorldPacket>::push(LuaS, pack, true);
}

void LuaEngine::PushTaxiPath(TaxiPath * tp, lua_State * LuaS) 
{
	if(LuaS == NULL)
		Lunar<TaxiPath>::push(L, tp, true);
	else
		Lunar<TaxiPath>::push(LuaS, tp, true);
}

void LuaEngine::PushSpell(Spell * sp, lua_State * LuaS) 
{
	if(LuaS == NULL)
		Lunar<Spell>::push(L, sp);
	else
		Lunar<Spell>::push(LuaS, sp);
}

void LuaEngine::PushSQLField(Field *field, lua_State *LuaS)
{
	if(LuaS == NULL)
		Lunar<Field>::push(L, field);
	else
		Lunar<Field>::push(LuaS, field);
}

void LuaEngine::PushSQLResult(QueryResult * res, lua_State * LuaS)
{
	if(LuaS == NULL)
		Lunar<QueryResult>::push(L, res, true);
	else
		Lunar<QueryResult>::push(LuaS, res, true);
}

void LuaEngine::PushAura(Aura * aura, lua_State * LuaS)
{
	if(LuaS == NULL)
		Lunar<Aura>::push(L, aura);
	else
		Lunar<Aura>::push(LuaS, aura);
}

/*******************************************************************************
	END PUSH METHODS
*******************************************************************************/

// Hyper as in Hyperactive, Jk, its Hypersniper.
void LuaEngineMgr::HyperCallFunction(const char * FuncName, int ref)
{
	CREATE_L_PTR;
	string sFuncName = string(FuncName);
	char * copy = strdup(FuncName);
	char * token = strtok(copy,".:");
	int top = 1;
	bool colon = false;
	//REMEMBER: top is always 1
	lua_settop(L,0); //stack should be empty
	if (strpbrk(FuncName,".:") == NULL ) //stack: empty
		lua_getglobal(L,FuncName);  //stack: function
	else
	{
		lua_getglobal(L, "_G"); //start out with the global table.  //stack: _G
		while (token != NULL)
		{
			lua_getfield(L, -1, token); //get the (hopefully) table/func  //stack: _G, subtable/func/nil
			if ((int)sFuncName.find(token)-1 > 0) //if it isn't the first token
			{
				if (sFuncName.at(sFuncName.find(token)-1) == ':')
					colon = true;
			}

			if (lua_isfunction(L,-1) && !lua_iscfunction(L,-1)) //if it's a Lua function //stack: _G/subt, func
			{
				if (colon) //stack: subt, func
				{
					lua_pushvalue(L, -2); //make the table the first arg //stack: subt, func, subt
					lua_remove(L,top); //stack: func, subt
				}
				else
					lua_replace(L,top); //stack: func
				break; //we don't need anything else
			}
			else if(lua_istable(L,-1) ) //stack: _G/subt, subtable
			{
				token = strtok(NULL,".:"); //stack: _G/subt, subtable
				lua_replace(L, top); //stack: subtable
			}
		}
	}
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_State * M = lua_tothread(L, -1); //repeats, args
	int thread = lua_gettop(L);
	int repeats = luaL_checkinteger(M, 1); //repeats, args
	int nargs = lua_gettop(M)-1;
	if (nargs != 0) //if we HAVE args...
	{
		for (int i = 2; i <= nargs+1; i++)
		{
			lua_pushvalue(M,i);
		}
		lua_xmove(M, L, nargs);
	}

	if(repeats != 0)
	{
		if (--repeats == 0) //free stuff, then
		{
			free((void*)FuncName);
			luaL_unref(L, LUA_REGISTRYINDEX, ref);
			hash_map<int, EventInfoHolder*>::iterator itr = g_luaMgr.m_registeredTimedEvents.find(ref);
			g_luaMgr.m_registeredTimedEvents.erase(itr);
		}
		else
		{
			lua_remove(M, 1); //args
			lua_pushinteger(M, repeats); //args, repeats
			lua_insert(M, 1); //repeats, args
		}
	}
	lua_remove(L, thread); //now we can remove the thread object
	int r = lua_pcall(L, nargs+(colon ? 1 : 0),0,0);
	if (r)
		report(L);

	free((void*)copy);
	lua_settop(L,0);
}

/*
	This version only accepts actual Lua functions and no arguments. See LCF_Extra:CreateClosure(...) to pass arguments to this function.
	*/
static int CreateLuaEvent(lua_State * L)
{
	int delay = luaL_checkinteger(L,2);
	int repeats = luaL_checkinteger(L,3);
	if(!strcmp(luaL_typename(L,1),"function") || delay > 0)
	{
		lua_settop(L,1);
		int functionRef = lua_ref(L,true);
		TimedEvent * ev = TimedEvent::Allocate(World::getSingletonPtr(), new CallbackP1<LuaEngineMgr,int>(&g_luaMgr, &LuaEngineMgr::CallFunctionByReference, functionRef), 0, delay, repeats);
		ev->eventType  = LUA_EVENTS_END+functionRef; //Create custom reference by adding the ref number to the max lua event type to get a unique reference for every function.
		sWorld.event_AddEvent(ev);
		g_luaMgr.getFunctionRefs().insert(functionRef);
		lua_pushinteger(L,functionRef);
	}
	else
		lua_pushnil(L);

	return 1;
}

void LuaEngineMgr::CallFunctionByReference(int ref)
{
	CREATE_L_PTR;
	lua_getref(L,ref);
	if(lua_pcall(L,0,0,0) )
		report(L);
}

void LuaEngineMgr::DestroyAllLuaEvents()
{
	CREATE_L_PTR;
	//Clean up for all events.
	set<int>::iterator itr = m_functionRefs.begin();
	for(; itr != m_functionRefs.end(); ++itr)
	{
		sEventMgr.RemoveEvents(World::getSingletonPtr(),(*itr)+LUA_EVENTS_END);
		lua_unref(L,(*itr));
	}
	m_functionRefs.clear();
}

static int ModifyLuaEventInterval(lua_State * L)
{
	int ref = luaL_checkinteger(L,1);
	int newinterval = luaL_checkinteger(L,2);
	ref+= LUA_EVENTS_END;
	//Easy interval modification.
	sEventMgr.ModifyEventTime(World::getSingletonPtr(),ref,newinterval);
	return 1;
}

static int DestroyLuaEvent(lua_State * L)
{
	//Simply remove the reference, CallFunctionByReference will find the reference has been freed and skip any processing.
	int ref = luaL_checkinteger(L,1);
	lua_unref(L,ref);
	g_luaMgr.getFunctionRefs().erase(ref);
	sEventMgr.RemoveEvents(World::getSingletonPtr(),ref+LUA_EVENTS_END);
	return 1;
}

static int ExtractfRefFromCString(lua_State * L,const char * functionName)
{
	int functionRef = 0;
	int top = lua_gettop(L);
	if(functionName != NULL)
	{
		char * copy = strdup(functionName);
		char * token = strtok(copy, ".:");
		if (strpbrk(functionName,".:") == NULL)
		{
			lua_getglobal(L,functionName);
			if (lua_isfunction(L,-1) && !lua_iscfunction(L,-1))
				functionRef = lua_ref(L,true);
			else
				luaL_error(L,"Reference creation failed! (%s) is not a valid Lua function.",functionName);
		}
		else
		{
			lua_getglobal(L,"_G");
			while (token != NULL)
			{
				lua_getfield(L,-1,token);
				if (lua_isfunction(L,-1) && !lua_iscfunction(L,-1))
				{
					functionRef = lua_ref(L,true);
					break;
				}
				else if (lua_istable(L,-1) )
				{
					token = strtok(NULL,".:");
					continue;
				}
				else
				{
					luaL_error(L,"Reference creation failed! (%s) is not a valid Lua function.",functionName);
					break;
				}
			}
		}
		free((void*)copy);
		lua_settop(L,top);
	}
	return functionRef;
}

static int RegisterUnitEvent(lua_State * L);
static int RegisterQuestEvent(lua_State * L);
static int RegisterGameObjectEvent(lua_State * L);
static int RegisterUnitGossipEvent(lua_State * L);
static int RegisterItemGossipEvent(lua_State * L);
static int RegisterGOGossipEvent(lua_State * L);

// Hyp Arc
static int RegisterServerHook(lua_State * L);
static int SuspendLuaThread(lua_State * L);
static int RegisterTimedEvent(lua_State * L);
static int RemoveTimedEvents(lua_State * L);
//static int RegisterDummySpell(lua_State * L);
//static int RegisterInstanceEvent(lua_State * L);
void RegisterGlobalFunctions(lua_State*);

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

	// Hyp Arc
	lua_pushcfunction(L, &RegisterServerHook);
	lua_setglobal(L, "RegisterServerHook");
	lua_pushcfunction(L, &SuspendLuaThread);
	lua_setglobal(L, "SuspendThread");
	lua_pushcfunction(L, &RegisterTimedEvent);
	lua_setglobal(L, "RegisterTimedEvent");
	lua_pushcfunction(L, &RemoveTimedEvents);
	lua_setglobal(L, "RemoveTimedEvents");
/*	lua_pushcfunction(L, &RegisterDummySpell);
	lua_setglobal(L, "RegisterDummySpell");
	lua_pushcfunction(L, &RegisterInstanceEvent);
	lua_setglobal(L, "RegisterInstanceEvent");*/
	lua_pushcfunction(L, &CreateLuaEvent);
	lua_setglobal(L, "CreateLuaEvent");
	lua_pushcfunction(L, &ModifyLuaEventInterval);
	lua_setglobal(L, "ModifyLuaEventInterval");
	lua_pushcfunction(L, &DestroyLuaEvent);
	lua_setglobal(L, "DestroyLuaEvent");

	RegisterGlobalFunctions(L);
	Lunar<Item>::Register(L);
	Lunar<Unit>::Register(L);
	Lunar<GameObject>::Register(L);
	Lunar<Aura>::Register(L);
	Lunar<WorldPacket>::Register(L);
	Lunar<TaxiPath>::Register(L);
	Lunar<Spell>::Register(L);
	Lunar<Field>::Register(L);
	Lunar<QueryResult>::Register(L);
	GuidMgr::Register(L);
}

static int RegisterUnitEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);
	if(!entry || !ev || typeName == NULL )
		return 0;

	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L, luaL_checkstring(L,3));
	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_UNIT,entry,ev,functionRef);

	lua_pop(L,3);
	return 1;
}

static int RegisterQuestEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);
	if(!entry || !ev || typeName == NULL)
		return 0;

	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_QUEST,entry,ev,functionRef);

	lua_pop(L,3);
	return 1;
}

static int RegisterGameObjectEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);
	if(!entry || !ev || typeName == NULL)
		return 0;

	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_GO,entry,ev,functionRef);

	lua_pop(L,3);
	return 1;
}

// Gossip stuff
static int RegisterUnitGossipEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);
	if(!entry || !ev || typeName == NULL)
		return 0;

	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_UNIT_GOSSIP,entry,ev,functionRef);

	lua_pop(L,3);
	return 1;
}

static int RegisterItemGossipEvent(lua_State * L)
{
 	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);
	if(!entry || !ev || typeName == NULL)
		return 0;

	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_ITEM_GOSSIP,entry,ev,functionRef);

	lua_pop(L,3);
	return 1;
}

static int RegisterGOGossipEvent(lua_State * L)
{
	lua_settop(L,3);
	uint16 functionRef = 0;
	int entry = luaL_checkint(L, 1);
	int ev = luaL_checkint(L, 2);
	const char * typeName = luaL_typename(L,3);
	if(!entry || !ev || typeName == NULL)
		return 0;

	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,3));
	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_GO_GOSSIP,entry,ev,functionRef);

	lua_pop(L,3);
	return 1;
}

static int SuspendLuaThread(lua_State * L)
{
	lua_State * thread = (lua_isthread(L,1)) ? lua_tothread(L,1) : NULL;
	if(thread == NULL)
		return luaL_error(L,"LuaEngineMgr","SuspendLuaThread expected Lua coroutine, got NULL.");

	int waitime = luaL_checkinteger(L,2);
	if(waitime <= 0)
		return luaL_error(L,"LuaEngineMgr","SuspendLuaThread expected timer > 0 instead got (%d)",waitime);

	lua_pushvalue(L,1);
	int ref = luaL_ref(L,LUA_REGISTRYINDEX);
	if(ref == LUA_REFNIL || ref == LUA_NOREF)
		return luaL_error(L,"Error in SuspendLuaThread! Failed to create a valid reference.");

	TimedEvent * evt = TimedEvent::Allocate(thread,new CallbackP1<LuaEngineMgr,int>(&g_luaMgr,&LuaEngineMgr::ResumeLuaThread,ref),0,waitime,1);
	sWorld.event_AddEvent(evt);
	lua_remove(L,1); // remove thread object
	lua_remove(L,1); // remove timer.
	//All that remains now are the extra arguments passed to this function.
	lua_xmove(L,thread,lua_gettop(L));
	g_luaMgr.getThreadRefs().insert(ref);
	return lua_yield(thread,lua_gettop(L));
}

static int RegisterTimedEvent(lua_State * L) //in this case, L == lu
{
	const char * funcName = strdup(luaL_checkstring(L,1));
	int delay = luaL_checkint(L,2);
	int repeats = luaL_checkint(L,3);
	if (!delay || repeats < 0 || !funcName)
	{
		lua_pushnumber(L, LUA_REFNIL);
		return 1;
	}

	lua_remove(L, 1); 
	lua_remove(L, 1);//repeats, args
	lua_State * thread = lua_newthread(L); //repeats, args, thread
	lua_insert(L,1); //thread, repeats, args
	lua_xmove(L,thread,lua_gettop(L)-1); //thread
	int ref = luaL_ref(L, LUA_REGISTRYINDEX); //empty
	if(ref == LUA_REFNIL || ref == LUA_NOREF)
		return luaL_error(L,"Error in RegisterTimedEvent! Failed to create a valid reference.");

	TimedEvent *te = TimedEvent::Allocate(&g_luaMgr, new CallbackP2<LuaEngineMgr, const char*, int>(&g_luaMgr, &LuaEngineMgr::HyperCallFunction, funcName, ref), EVENT_LUA_TIMED, delay, repeats);
	EventInfoHolder * ek = new EventInfoHolder;
	ek->funcName = funcName;
	ek->te = te;
	g_luaMgr.m_registeredTimedEvents.insert( make_pair<int, EventInfoHolder*>(ref, ek) );
	LuaEvent.event_AddEvent(te);
	lua_settop(L,0);
	lua_pushnumber(L, ref);
	return 1;
}

static int RemoveTimedEvents(lua_State * L) //in this case, L == lu
{
	LuaEvent.RemoveEvents();
	return 1;
}

static int RegisterServerHook(lua_State * L)
{
	uint16 functionRef = 0;
	//Maximum passed in arguments, consider rest as garbage
	lua_settop(L,2);
	uint32 ev = luaL_checkint(L, 1);
	const char * typeName = luaL_typename(L,2);
	if(!ev || typeName == NULL)
		return 0;

	// For functions directly passed in, skip all that code and register the reference.
	if(!strcmp(typeName,"function"))
		functionRef = (uint16)lua_ref(L,true);
	else if(!strcmp(typeName,"string")) //Old way of passing in functions, obsolete but left in for compatability.
		functionRef = ExtractfRefFromCString(L,luaL_checkstring(L,2));

	if(functionRef > 0)
		g_luaMgr.RegisterEvent(REGTYPE_SERVHOOK,0,ev,functionRef);

	lua_pop(L,2);
	return 1;
}

/************************************************************************/
/* Manager Stuff														*/
/************************************************************************/

class LuaGossip : public GossipScript
{
public:

	LuaGossip() : GossipScript(), m_go_gossip_binding(NULL),m_item_gossip_binding(NULL),m_unit_gossip_binding(NULL) {}
	~LuaGossip()
	{
		typedef HM_NAMESPACE::hash_map<uint32, LuaGossip*> MapType;
		MapType gMap;
		if(m_go_gossip_binding != NULL)
		{
			gMap = g_luaMgr.getGameObjectGossipInterfaceMap();
			for(MapType::iterator itr = gMap.begin(); itr != gMap.end(); ++itr)
			{
				if(itr->second == this) {
					gMap.erase(itr);
					break;
				}
			}
		}
		else if(m_unit_gossip_binding != NULL)
		{
			gMap = g_luaMgr.getUnitGossipInterfaceMap();
			for(MapType::iterator itr = gMap.begin(); itr != gMap.end(); ++itr)
			{
				if(itr->second == this)
				{
					gMap.erase(itr);
					break;
				}
			}
		}
		else if(m_item_gossip_binding != NULL)
		{
			gMap = g_luaMgr.getItemGossipInterfaceMap();
			for(MapType::iterator itr = gMap.begin(); itr != gMap.end(); ++itr)
			{
				if(itr->second == this)
				{
					gMap.erase(itr);
					break;
				}
			}
		}
	}

	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		if(pObject->GetTypeId() == TYPEID_UNIT)
		{
			if(m_unit_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK]);
			g_engine->PushUnit(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_TALK);
			g_engine->PushUnit(Plr);
			g_engine->PushBool(AutoSend);
			g_engine->ExecuteCall(4);
		}
		else if(pObject->GetTypeId() == TYPEID_ITEM)
		{
			if(m_item_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK]);
			g_engine->PushItem(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_TALK);
			g_engine->PushUnit(Plr);
			g_engine->PushBool(AutoSend);
			g_engine->ExecuteCall(4);
		}
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			if(m_go_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_TALK]);
			g_engine->PushGo(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_TALK);
			g_engine->PushUnit(Plr);
			g_engine->PushBool(AutoSend);
			g_engine->ExecuteCall(4);
		}
	}

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(pObject->GetTypeId() == TYPEID_UNIT)
		{
			if(m_unit_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION]);
			g_engine->PushUnit(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_SELECT_OPTION);
			g_engine->PushUnit(Plr);
			g_engine->PushUint(Id);
			g_engine->PushUint(IntId);
			g_engine->PushString(EnteredCode);
			g_engine->ExecuteCall(6);
		}
		else if(pObject->GetTypeId() == TYPEID_ITEM)
		{
			if(m_item_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION]);
			g_engine->PushItem(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_SELECT_OPTION);
			g_engine->PushUnit(Plr);
			g_engine->PushUint(Id);
			g_engine->PushUint(IntId);
			g_engine->PushString(EnteredCode);
			g_engine->ExecuteCall(6);
		}
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			if(m_go_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_SELECT_OPTION]);
			g_engine->PushGo(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_SELECT_OPTION);
			g_engine->PushUnit(Plr);
			g_engine->PushUint(Id);
			g_engine->PushUint(IntId);
			g_engine->PushString(EnteredCode);
			g_engine->ExecuteCall(6);
		}
	}

	void GossipEnd(Object* pObject, Player* Plr)
	{
		if(pObject->GetTypeId() == TYPEID_UNIT)
		{
			if(m_unit_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_unit_gossip_binding->Functions[GOSSIP_EVENT_ON_END]);
			g_engine->PushUnit(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_END);
			g_engine->PushUnit(Plr);
			g_engine->ExecuteCall(3);
		}
		else if(pObject->GetTypeId() == TYPEID_ITEM)
		{
			if(m_item_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_item_gossip_binding->Functions[GOSSIP_EVENT_ON_END]);
			g_engine->PushItem(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_END);
			g_engine->PushUnit(Plr);
			g_engine->ExecuteCall(3);
		}
		else if(pObject->GetTypeId() == TYPEID_GAMEOBJECT)
		{
			if(m_go_gossip_binding == NULL)
				return;

			g_engine->BeginCall(m_go_gossip_binding->Functions[GOSSIP_EVENT_ON_END]);
			g_engine->PushGo(pObject);
			g_engine->PushUint(GOSSIP_EVENT_ON_END);
			g_engine->PushUnit(Plr);
			g_engine->ExecuteCall(3);
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
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_ENTER_COMBAT]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_ENTER_COMBAT);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnCombatStop(Unit* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_LEAVE_COMBAT]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_LEAVE_COMBAT);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnTargetDied(Unit* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_KILLED_TARGET]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_KILLED_TARGET);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnDied(Unit *mKiller)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_DIED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_DIED);
		g_engine->PushUnit(mKiller);
		g_engine->ExecuteCall(3);
	}

	void OnTargetParried(Unit* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_TARGET_PARRIED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_TARGET_PARRIED);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnTargetDodged(Unit* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_TARGET_DODGED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_TARGET_DODGED);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnTargetBlocked(Unit* mTarget, int32 iAmount)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_TARGET_BLOCKED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_TARGET_BLOCKED);
		g_engine->PushUnit(mTarget);
		g_engine->PushInt(iAmount);
		g_engine->ExecuteCall(4);
	}

	void OnTargetCritHit(Unit* mTarget, int32 fAmount)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_TARGET_CRIT_HIT]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_TARGET_CRIT_HIT);
		g_engine->PushUnit(mTarget);
		g_engine->PushInt(fAmount);
		g_engine->ExecuteCall(4);
	}

	void OnParried(Unit* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_PARRY]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_PARRY);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnDodged(Unit* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_DODGED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_DODGED);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(3);
	}

	void OnBlocked(Unit* mTarget, int32 iAmount)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_BLOCKED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_BLOCKED);
		g_engine->PushUnit(mTarget);
		g_engine->PushInt(iAmount);
		g_engine->ExecuteCall(4);
	}

	void OnCritHit(Unit* mTarget, int32 fAmount)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_CRIT_HIT]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_CRIT_HIT);
		g_engine->PushUnit(mTarget);
		g_engine->PushInt(fAmount);
		g_engine->ExecuteCall(4);
	}

	void OnHit(Unit* mTarget, float fAmount)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_HIT]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_HIT);
		g_engine->PushUnit(mTarget);
		g_engine->PushFloat(fAmount);
		g_engine->ExecuteCall(4);
	}

	void OnAssistTargetDied(Unit* mAssistTarget)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_ASSIST_DIED]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_ASSIST_DIED);
		g_engine->PushUnit(mAssistTarget);
		g_engine->ExecuteCall(3);
	}

	void OnFear(Unit* mFeared, uint32 iSpellId)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_FEAR]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_FEAR);
		g_engine->PushUnit(mFeared);
		g_engine->PushUint(iSpellId);
		g_engine->ExecuteCall(4);
	}

	void OnFlee(Unit* mFlee)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_FLEE]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_FLEE);
		g_engine->PushUnit(mFlee);
		g_engine->ExecuteCall(3);
	}

	void OnCallForHelp()
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_CALL_FOR_HELP]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_CALL_FOR_HELP);
		g_engine->ExecuteCall(2);
	}

	void OnLoad()
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_SPAWN]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_SPAWN);
		g_engine->ExecuteCall(2);

		uint32 iid = _unit->GetInstanceID();
		if (_unit->GetMapMgr() == NULL || _unit->GetMapMgr()->GetMapInfo()->type == INSTANCE_NULL)
			iid = 0;

		OnLoadInfo.push_back(_unit->GetMapId());
		OnLoadInfo.push_back(iid);
		OnLoadInfo.push_back(GET_LOWGUID_PART(_unit->GetGUID()));
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_REACH_WP]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_REACH_WP);
		g_engine->PushUint(iWaypointId);
		g_engine->PushBool(bForwards);
		g_engine->ExecuteCall(4);
	}

	void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_LOOT_TAKEN]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_LOOT_TAKEN);
		g_engine->PushUnit(pPlayer);
		g_engine->PushUint(pItemPrototype->ItemId);
		g_engine->ExecuteCall(4);
	}

	void AIUpdate()
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_AIUPDATE]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_AIUPDATE);
		g_engine->ExecuteCall(2);
	}

	void OnEmote(Player * pPlayer, EmoteType Emote)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_EMOTE]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_EMOTE);
		g_engine->PushUnit(pPlayer);
		g_engine->PushInt((int32)Emote);
		g_engine->ExecuteCall(4);
	}

	void OnDamageTaken(Unit* mAttacker, uint32 fAmount)
	{
		g_engine->BeginCall(m_binding->Functions[CREATURE_EVENT_ON_DAMAGE_TAKEN]);
		g_engine->PushUnit(_unit);
		g_engine->PushInt(CREATURE_EVENT_ON_DAMAGE_TAKEN);
		g_engine->PushUnit(mAttacker);
		g_engine->PushUint(fAmount);
		g_engine->ExecuteCall(4);
	}

	void StringFunctionCall(int fRef)
	{
		g_engine->BeginCall(fRef);
		g_engine->PushUnit(_unit);
		g_engine->ExecuteCall(1);
	}

	void Destroy()
	{
/*		typedef std::multimap<uint32,LuaCreature*> CMAP;
		CMAP & cMap = g_engine->getLuCreatureMap();
		CMAP::iterator itr = cMap.find(_unit->GetEntry());
		CMAP::iterator itend = cMap.upper_bound(_unit->GetEntry());
		CMAP::iterator it;
		for(;itr != cMap.end() && itr != itend;)
		{
			it = itr++;
			if(it->second != NULL && it->second == this)
				cMap.erase(it);
		}

		//Function Ref clean up
		std::map< uint64,std::set<int> > & objRefs = g_engine->getObjectFunctionRefs();
		std::map< uint64,std::set<int> >::iterator itr = objRefs.find(_unit->GetGUID());
		if(itr != objRefs.end() )
		{
			std::set<int> & refs = itr->second;
			for(std::set<int>::iterator it = refs.begin(); it != refs.end(); ++it)
			{
				lua_unref(g_engine->getluState(),(*it));
				sEventMgr.RemoveEvents(_unit,(*it)+EVENT_LUA_CREATURE_EVENTS);
			}
			refs.clear();
		}*/
		delete this;
	}

	LuaUnitBinding * m_binding;
};

class LuaGameObject : public GameObjectAIScript
{
public:
	LuaGameObject(GameObject * go) : GameObjectAIScript(go) {}
	~LuaGameObject() {}

	void OnCreate()
	{
		g_engine->BeginCall(m_binding->Functions[GAMEOBJECT_EVENT_ON_CREATE]);
		g_engine->PushGo(_gameobject);
		g_engine->ExecuteCall(1);
	}

	void OnSpawn()
	{
		g_engine->BeginCall(m_binding->Functions[GAMEOBJECT_EVENT_ON_SPAWN]);
		g_engine->PushGo(_gameobject);
		g_engine->ExecuteCall(1);
	}

	void OnDespawn()
	{
		g_engine->BeginCall(m_binding->Functions[GAMEOBJECT_EVENT_ON_DESPAWN]);
		g_engine->PushGo(_gameobject);
		g_engine->ExecuteCall(1);
	}

	void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo)
	{
		g_engine->BeginCall(m_binding->Functions[GAMEOBJECT_EVENT_ON_LOOT_TAKEN]);
		g_engine->PushGo(_gameobject);
		g_engine->PushUint(GAMEOBJECT_EVENT_ON_LOOT_TAKEN);
		g_engine->PushUnit(pLooter);
		g_engine->PushUint(pItemInfo->ItemId);
		g_engine->ExecuteCall(4);
	}

	void OnActivate(Player * pPlayer)
	{
		g_engine->BeginCall(m_binding->Functions[GAMEOBJECT_EVENT_ON_USE]);
		g_engine->PushGo(_gameobject);
		g_engine->PushUint(GAMEOBJECT_EVENT_ON_USE);
		g_engine->PushUnit(pPlayer);
		g_engine->ExecuteCall(3);
	}

	void AIUpdate()
	{
		g_engine->BeginCall(m_binding->Functions[GAMEOBJECT_EVENT_AIUPDATE]);
		g_engine->PushGo(_gameobject);
		g_engine->ExecuteCall(1);
	}

	void Destroy ()
	{
/*		typedef std::multimap<uint32,LuaGameObject*> GMAP;
		GMAP & gMap = g_engine->getLuGameObjectMap();
		GMAP::iterator itr = gMap.find(_gameobject->GetEntry());
		GMAP::iterator itend = gMap.upper_bound(_gameobject->GetEntry());
		GMAP::iterator it;
		//uint64 guid = _gameobject->GetGUID(); Unused?
		for(; itr != itend;)
		{
			it = itr++;
			if(it->second != NULL && it->second == this)
				gMap.erase(it);
		}

		std::map< uint64,std::set<int> > & objRefs = g_engine->getObjectFunctionRefs();
		std::map< uint64,std::set<int> >::iterator itr2 = objRefs.find(_gameobject->GetGUID());
		if(itr2 != objRefs.end() )
		{
			std::set<int> & refs = itr2->second;
			for(std::set<int>::iterator it = refs.begin(); it != refs.end(); ++it)
				lua_unref( g_engine->getluState(), (*it) );
			refs.clear();
		}*/
		delete this;
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
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_ON_ACCEPT]);
		g_engine->PushUnit(mTarget);
		g_engine->PushUint(qLogEntry->GetQuest()->id);
		g_engine->ExecuteCall(2);
	}

	void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
	{
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_ON_COMPLETE]);
		g_engine->PushUnit(mTarget);
		g_engine->PushUint(qLogEntry->GetQuest()->id);
		g_engine->ExecuteCall(2);
	}

	void OnQuestCancel(Player* mTarget)
	{
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_ON_CANCEL]);
		g_engine->PushUnit(mTarget);
		g_engine->ExecuteCall(1);
	}

	void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_GAMEOBJECT_ACTIVATE]);
		g_engine->PushUint(entry);
		g_engine->PushUnit(mTarget);
		g_engine->PushUint(qLogEntry->GetQuest()->id);
		g_engine->ExecuteCall(3);
	}

	void OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_ON_CREATURE_KILL]);
		g_engine->PushUint(entry);
		g_engine->PushUnit(mTarget);
		g_engine->PushUint(qLogEntry->GetQuest()->id);
		g_engine->ExecuteCall(3);
	}

	void OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_ON_EXPLORE_AREA]);
		g_engine->PushUint(areaId);
		g_engine->PushUnit(mTarget);
		g_engine->PushUint(qLogEntry->GetQuest()->id);
		g_engine->ExecuteCall(3);
	}

	void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry *qLogEntry)
	{
		g_engine->BeginCall(m_binding->Functions[QUEST_EVENT_ON_PLAYER_ITEMPICKUP]);
		g_engine->PushUint(itemId);
		g_engine->PushUint(totalCount);
		g_engine->PushUnit(mTarget);
		g_engine->PushUint(qLogEntry->GetQuest()->id);
		g_engine->ExecuteCall(4);
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

void LuaEngineMgr::RegisterEvent(uint8 regtype, uint32 id, uint32 evt, uint16 functionRef) 
{
	switch(regtype) 
	{
		case REGTYPE_UNIT: 
			{
				if(id && evt && evt < CREATURE_EVENT_COUNT) {
					LuaUnitBinding * bind = GetUnitBinding(id);
					if(bind == NULL)
					{
						LuaUnitBinding nbind;
						memset(&nbind,0,sizeof(LuaUnitBinding));
						nbind.Functions[evt] = functionRef;
						m_unitBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->Functions[evt] > 0)
							lua_unref(GLuas(), bind->Functions[evt]);
						bind->Functions[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_GO:
			{
				if(id && evt && evt < GAMEOBJECT_EVENT_COUNT)
				{
					LuaGameObjectBinding * bind = GetGameObjectBinding(id);
					if(bind == NULL)
					{
						LuaGameObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaGameObjectBinding));
						nbind.Functions[evt] = functionRef;
						m_gameobjectBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->Functions[evt] > 0)
							lua_unref(GLuas(), bind->Functions[evt]);
						bind->Functions[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_QUEST:
			{
				if(id && evt && evt < QUEST_EVENT_COUNT)
				{
					LuaQuestBinding * bind = GetQuestBinding(id);
					if(bind == NULL)
					{
						LuaQuestBinding nbind;
						memset(&nbind,0,sizeof(LuaQuestBinding));
						nbind.Functions[evt] = functionRef;
						m_questBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->Functions[evt] > 0)
							lua_unref(GLuas(), bind->Functions[evt]);
						bind->Functions[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_SERVHOOK:
			{
				if(evt < NUM_SERVER_HOOKS)
					EventAsToFuncName[evt].push_back(functionRef);
			}break;
		case REGTYPE_DUMMYSPELL: 
			{
				if (id)
					m_luaDummySpells.insert( make_pair<uint32,uint16>(id,functionRef) );
			}break;
/*		case REGTYPE_INSTANCE: 
			{
				if(id && evt && evt < INSTANCE_EVENT_COUNT)
				{
					LuaObjectBinding * bind = getInstanceBinding(id);
					if(bind == NULL)
					{
						LuaObjectBinding nbind;
						memset(&nbind,0,sizeof(LuaObjectBinding));
						nbind.Functions[evt] = functionRef;
						m_instanceBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->Functions[evt] > 0)
							lua_unref(GLuas(), bind->Functions[evt]);
						bind->Functions[evt] = functionRef;
					}
				}
			}break;*/
		case REGTYPE_UNIT_GOSSIP:
			{
				if(id && evt && evt < GOSSIP_EVENT_COUNT)
				{
					LuaUnitGossipBinding * bind = GetLuaUnitGossipBinding(id);
					if(bind == NULL)
					{
						LuaUnitGossipBinding nbind;
						memset(&nbind,0,sizeof(LuaUnitGossipBinding));
						nbind.Functions[evt] = functionRef;
						m_unit_gossipBinding.insert(make_pair(id,nbind));
					}
					else
					{
						if(bind->Functions[evt] > 0)
							lua_unref(GLuas(), bind->Functions[evt]);
						bind->Functions[evt] = functionRef;
					}
				}
			}break;
		case REGTYPE_ITEM_GOSSIP:
		{
			if(id && evt && evt < GOSSIP_EVENT_COUNT)
			{
				LuaItemGossipBinding * bind = GetLuaItemGossipBinding(id);
				if(bind == NULL)
				{
					LuaItemGossipBinding nbind;
					memset(&nbind,0,sizeof(LuaItemGossipBinding));
					nbind.Functions[evt] = functionRef;
					m_item_gossipBinding.insert(make_pair(id,nbind));
				}
				else
				{
					if(bind->Functions[evt] > 0)
						lua_unref(GLuas(), bind->Functions[evt]);
					bind->Functions[evt] = functionRef;
				}
			}
		}break;
		case REGTYPE_GO_GOSSIP:
		{
			if(id && evt && evt < GOSSIP_EVENT_COUNT)
			{
				LuaGOGossipBinding * bind = GetLuaGOGossipBinding(id);
				if(bind == NULL)
				{
					LuaGOGossipBinding nbind;
					memset(&nbind,0,sizeof(LuaGOGossipBinding));
					nbind.Functions[evt] = functionRef;
					m_go_gossipBinding.insert(make_pair(id,nbind));
				}
				else
				{
					if(bind->Functions[evt] > 0)
						lua_unref(GLuas(), bind->Functions[evt]);
					bind->Functions[evt] = functionRef;
				}
			}
		}break;
	}
}

void LuaEngineMgr::ResumeLuaThread(int ref)
{
	CREATE_L_PTR;

	lua_State * expectedThread = NULL;
	lua_rawgeti(L,LUA_REGISTRYINDEX,ref);
	if(lua_isthread(L,-1) )
		expectedThread = lua_tothread(L,-1);

	if(expectedThread != NULL) 
	{
		//push ourself on the stack
		lua_pushthread(expectedThread);
		//move the thread to the main lu state(and pop it off)
		lua_xmove(expectedThread,L,1);
		if(lua_rawequal(L,-1,-2) )
		{
			lua_pop(L,2);
			int res = lua_resume(expectedThread,lua_gettop(expectedThread));
			if(res != LUA_YIELD && res)
				report(expectedThread);
		}
		else
			lua_pop(L,2);
		luaL_unref(L,LUA_REGISTRYINDEX,ref);
	}
}

/*void LuaEngineMgr::ReloadScripts()
{
	m_engine->GetLock().Acquire();

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
	m_engine->GetLock().Release();
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

#include "Functions/TableFunctions.h"
