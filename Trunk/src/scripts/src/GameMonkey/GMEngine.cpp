/**************************************
 *		   GameMonkey Engine		  *
 *									  *
 *		Imported and improved from	  *
 *		   original Ascent svn		  *
 **************************************/

#include "GMEngine.h"
#include "StdAfx.h"
#include <ScriptSetup.h>

#ifndef WIN32
#include <dirent.h>
#endif

GMEngine * GMSystem;
GMEngine gmscript;
ScriptMgr * m_scriptMgr;

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_SCRIPT_ENGINE | SCRIPT_TYPE_SCRIPT_ENGINE_GM;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	m_scriptMgr = mgr;
	gmscript.Reload();
}

GMEngine::GMEngine()
{
	m_playerType = m_unitType = m_gameObjectType = m_questType = m_spellType = m_auraType = m_areaTriggerType = m_scriptEngineType = -1;
	m_machine = 0;
}

GMEngine::~GMEngine()
{
	if(m_machine)
	{
		m_machine->GetGC()->DestructAll();
		
		delete m_machine;
	}
}

void GMEngine::Reload()
{
	if(m_machine)
	{
		m_playerType = m_unitType = m_gameObjectType = m_questType = m_spellType = m_auraType = m_areaTriggerType = m_scriptEngineType = -1;
		for(int i = 0; i < 10; ++i)
			m_userObjects[i] = 0;

		m_PlayerStorageMap.clear();
		m_unitMap.clear();
		m_gameObjectMap.clear();
		m_questMap.clear();
		m_auraMap.clear();
		m_areaTriggerMap.clear();
		
		m_machine->ResetAndFreeMemory();
		delete m_machine;

		m_machine = 0;
	}

	m_machine = new gmMachine;
	
	/* create our types */
	m_playerType		= m_machine->CreateUserType("Player");
	m_unitType			= m_machine->CreateUserType("Unit");
	m_gameObjectType	= m_machine->CreateUserType("GameObject");
	m_questType			= m_machine->CreateUserType("Quest");
	m_spellType			= m_machine->CreateUserType("Spell");
	m_auraType			= m_machine->CreateUserType("Aura");
	m_areaTriggerType	= m_machine->CreateUserType("AreaTrigger");
	m_scriptEngineType	= m_machine->CreateUserType("ScriptEngine");

	m_allowedTypes.push_back(m_playerType);
	m_allowedTypes.push_back(m_unitType);
	m_allowedTypes.push_back(m_gameObjectType);
	m_allowedTypes.push_back(m_questType);
	m_allowedTypes.push_back(m_spellType);
	m_allowedTypes.push_back(m_auraType);
	m_allowedTypes.push_back(m_areaTriggerType);
	m_allowedTypes.push_back(m_scriptEngineType);

	/* register all our functions inside the machine */
	SetPlayerFunctionTable();
	SetUnitFunctionTable();
	SetGameObjectFunctionTable();
	SetQuestFunctionTable();
	SetSpellFunctionTable();
	SetAuraFunctionTable();
	SetAreaTriggerFunctionTable();
	SetScriptEngineFunctionTable();

	/* allocate our user data variables */
	for(int i = 0; i < 10; ++i)
	{
		m_userObjects[i] = m_machine->AllocUserObject(this, m_scriptEngineType);
		m_machine->AddCPPOwnedGMObject(m_userObjects[i]);
	}

	Log.Notice("GMEngine", "Compiling GameMonkey Scripts...");

#ifdef WIN32
	/* compile the scripts */
	WIN32_FIND_DATA fd;
	HANDLE f = FindFirstFile("scripts\\*.gm", &fd);
	if(f != INVALID_HANDLE_VALUE)
	{
		do 
		{
			string fname = "scripts/";
			fname += fd.cFileName;

			ExecuteScriptFile(fname.c_str());
		} while(FindNextFile(f, &fd));
		FindClose(f);
	}
#else
	/* compile scripts */
	struct dirent ** list;
	int filecount = scandir("scripts/", &list, 0, 0);
	if(!filecount || !list || filecount < 0)
		return;

	char * ext;
	while(filecount--)
	{
		ext = strrchr(list[filecount]->d_name, '.');
		if(ext != NULL && !strcmp(ext, ".gm"))
		{
			string full_path = "scripts/" + string(list[filecount]->d_name);
			ExecuteScriptFile(full_path.c_str());
		}
		free(list[filecount]);
	}
	free(list);
#endif

	Log.Notice("GMEngine", "Scripts compiled.");
}

void GMEngine::ExecuteScriptFile(const char *filename)
{
	char * data;
	FILE * f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);

	data = new char[size+1];
	fread(data, 1, size, f);
	data[size] = 0;

	fclose(f);

	/* setup 'this' pointer */
	m_userObjects[0]->m_user = this;
	m_userObjects[0]->m_userType = m_scriptEngineType;

	/* set 'this' variable */
	m_variables[0].SetUser(m_userObjects[0]);

	int threadid;
	printf("%s:\n	{\n", strstr(filename, "/")+1);
	int no_errors = m_machine->ExecuteString(data, &threadid, true, filename, &m_variables[0]);
	bool first = true;
	if(no_errors || m_machine->GetLog().GetEntry(first))
	{
		Log.Error("GMEngine", "Errors occured while compiling %s.", filename);
		DumpErrors();
	}
	else
		Log.Notice("GMEngine", "%u errors.", no_errors);
	printf("	}\n");

	delete [] data;
}

void GMEngine::DumpErrors()
{
	bool first = true;
	const char * message = m_machine->GetLog().GetEntry(first);
	first = false;
	while(message)
	{
		printf("GM_Debug: %s", message);
		message = m_machine->GetLog().GetEntry(first);
	}
	m_machine->GetLog().Reset();  //if not reset, will repeat last error over and over again
}

void GMEngine::DoGMCall(gmFunctionObject *obj, uint32 ArgumentCount, int *return_value)
{
	gmCall call;
	if(call.BeginFunction(m_machine, obj, m_variables[0], false))
	{
		for(uint32 i = 0; i < ArgumentCount; ++i)
			call.AddParam(m_variables[1+i]);

		m_userObjectCounter = ArgumentCount + 1;
		call.End();

		if(return_value != 0)
		{
			int v;
			if(call.GetReturnedInt(v))
				*return_value = v;
			else
				*return_value = 1;
		}

		DumpErrors();
	}
	else
	{
		printf("Could not find function!");
		DumpErrors();
	}
}

bool GMEngine::OnActivateAreaTrigger(AreaTrigger *at, Player *plr)
{
	SingleScriptMap::iterator itr = m_areaTriggerMap.find(at->AreaTriggerID);
	if(itr == m_areaTriggerMap.end())
		return true;

	/*map<uint32, gmFunctionObject*>::iterator it2 = itr->second.find(0);
	if(it2 == itr->second.end())
		return true;*/

	m_lock.Acquire();

	ASSERT(itr->second->GetType() == GM_FUNCTION);

	// Setup 'this' pointer to the areatrigger struct.
	m_userObjects[0]->m_user = at;
	m_userObjects[0]->m_userType = m_areaTriggerType;
	m_variables[0].SetUser(m_userObjects[0]);

	// Setup the first argument (the player entering)
	m_userObjects[1]->m_user = plr;
	m_userObjects[1]->m_userType = m_playerType;
	m_variables[1].SetUser(m_userObjects[1]);
	m_userObjectCounter = 2;

	// Setup the call.
	gmCall call;
	if(call.BeginFunction(m_machine, itr->second, m_variables[0], false))
	{
		call.AddParam(m_variables[1]);
		call.End();

		DumpErrors();
		int res;
		if(!call.GetReturnedInt(res))
		{
			printf("Call failed.");
			m_lock.Release();
			return true;
		}

		m_lock.Release();
		return (res > 0) ? true : false;
	}
	else
	{
		printf("Could not find function!");
		DumpErrors();
		m_lock.Release();
		return true;
	}
}

bool GMEngine::OnQuestEvent(Quest *quest, Creature *pQuestGiver, Player *plr, uint32 Event)
{
	if(!m_questMap.size())
		return false;

	ScriptMap::iterator itr = m_questMap.find(quest->id);
	if(itr == m_questMap.end())
		return false;

	map<uint32, gmFunctionObject*>::iterator it2 = itr->second.find(Event);
	if(it2 == itr->second.end() )
		return false;

	m_lock.Acquire();
	SetVariable(0, quest, m_questType);
	SetVariable(1, pQuestGiver, m_unitType);
	SetVariable(2, plr, m_playerType);

	DoGMCall(it2->second, 2, 0);
	m_lock.Release();
	return true;
}

bool GMEngine::OnQuestRequireEvent(Quest *quest, Creature *pQuestGiver, Player *plr, uint32 Event)
{
	if(!m_questMap.size())
		return true;;

	ScriptMap::iterator itr = m_questMap.find(quest->id);
	if(itr == m_questMap.end())
		return true;

	map<uint32, gmFunctionObject*>::iterator it2 = itr->second.find(Event);
	if(it2 == itr->second.end() )
		return true;

	m_lock.Acquire();
	SetVariable(0, quest, m_questType);
	SetVariable(1, pQuestGiver, m_unitType);
	SetVariable(2, plr, m_playerType);

	int ret;
	DoGMCall(it2->second, 2, &ret);
	m_lock.Release();
	return (ret > 0) ? true : false;
}

bool GMEngine::OnCreatureEvent(Creature *pCreature, Unit *pAttacker, uint32 Event)
{
	if(!m_unitMap.size())
		return false;

	ScriptMap::iterator itr = m_unitMap.find(pCreature->GetEntry());
	if(itr == m_unitMap.end())
		return false;

	map<uint32, gmFunctionObject*>::iterator it2 = itr->second.find(Event);
	if(it2 == itr->second.end() )
		return false;

	m_lock.Acquire();
	SetVariable(0, pCreature, m_unitType);
	SetVariable(1, pAttacker, m_playerType);

	DoGMCall(it2->second, 1, 0);
	m_lock.Release();
	return true;
}

bool GMEngine::OnCreatureEvent(Creature *pCreature, gmFunctionObject *pointer)
{
	if(!m_unitMap.size())
		return false;

	m_lock.Acquire();
	SetVariable(0, pCreature, m_unitType);
	DoGMCall(pointer, 0, 0);
	m_lock.Release();
	return true;
}

bool GMEngine::OnCreatureEventArg(Creature *pCreature, uint32 Argument, uint32 Event)
{
	if(!m_unitMap.size())
		return false;

	ScriptMap::iterator itr = m_unitMap.find(pCreature->GetEntry());
	if(itr == m_unitMap.end())
		return false;

	map<uint32, gmFunctionObject*>::iterator it2 = itr->second.find(Event);
	if(it2 == itr->second.end() )
		return false;

	gmFunctionObject * obj = it2->second;
	m_lock.Acquire();

	ASSERT(obj->GetType() == GM_FUNCTION);

	SetVariable(0, pCreature, m_unitType);
	gmCall call;
	if(call.BeginFunction(m_machine, obj, m_variables[0], false))
	{
		call.AddParamInt(Argument);
		m_userObjectCounter = 2;
		call.End();
		DumpErrors();
	}
	else
	{
		printf("Could not find function!");
		DumpErrors();
	}

	m_lock.Release();
	return true;
}

bool GMEngine::OnGameObjectEvent(GameObject *pGameObject, Player *pUser, uint32 Event)
{
	if(!m_gameObjectMap.size())
		return false;

	ScriptMap::iterator itr = m_gameObjectMap.find(pGameObject->GetEntry());
	if(itr == m_gameObjectMap.end())
		return false;

	map<uint32, gmFunctionObject*>::iterator it2 = itr->second.find(Event);
	if(it2 == itr->second.end() )
		return false;

	m_lock.Acquire();
	SetVariable(0, pGameObject, m_gameObjectType);
	SetVariable(1, pUser, m_playerType);

	DoGMCall(it2->second, 1, 0);
	m_lock.Release();
	return true;
}

bool GMEngine::HasEventType(uint32 Entry, uint32 Event)
{
	ScriptMap::iterator itr = m_unitMap.find(Entry);
	if(itr == m_unitMap.end())
		return false;

	if(itr->second.find(Event) != itr->second.end())
		return false;

	return true;
}

bool GMEngine::CallGMFunction(const char *name, Object *forthis)
{
	m_lock.Acquire();
	gmStringObject * fname = m_machine->AllocStringObject(name, (int)strlen(name));
	ASSERT(fname);
	gmCall call;
	m_userObjectCounter=1;
	m_userObjects[0]->m_user = forthis;
	switch(forthis->GetTypeId())
	{
	case TYPEID_PLAYER:
		m_userObjects[0]->m_userType = m_playerType;
		break;

	case TYPEID_UNIT:
		m_userObjects[0]->m_userType = m_unitType;
		break;

	default:
		m_lock.Release();
		return false;
	}

	m_variables[0].SetUser(m_userObjects[0]);
	if(call.BeginGlobalFunction(m_machine, fname, m_variables[0], false))
	{
		call.End();
		DumpErrors();
		m_lock.Release();
		return true;
	}	

	m_lock.Release();
	return false;
}