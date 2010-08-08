/**************************************
 *		   GameMonkey Engine		  *
 *									  *
 *		Imported and improved from	  *
 *		   original Ascent svn		  *
 **************************************/

#pragma once

#include "gm/gmMachine.h"
#include "gm/gmUserObject.h"
#include "gm/gmCall.h"
#include "StdAfx.h"

class Creature;
class Unit;
class Player;
struct Quest;
struct AreaTrigger;

class GMEngine
{
	gmMachine * m_machine;

public:
	gmType m_playerType;
	gmType m_unitType;
	gmType m_gameObjectType;
	gmType m_questType;
	gmType m_spellType;
	gmType m_auraType;
	gmType m_areaTriggerType;
	gmType m_scriptEngineType;
	
	list<gmType> m_allowedTypes;

protected:
	typedef HM_NAMESPACE::hash_map<uint32, map<uint32, gmFunctionObject*> > ScriptMap;
	typedef HM_NAMESPACE::hash_map<uint32, gmFunctionObject*> SingleScriptMap;
	ScriptMap m_PlayerStorageMap;
	ScriptMap m_unitMap;
	ScriptMap m_gameObjectMap;
	ScriptMap m_questMap;
	ScriptMap m_spellMap;
	ScriptMap m_auraMap;
	SingleScriptMap m_areaTriggerMap;

	Mutex m_lock;

public:

	inline Mutex & GetLock() { return m_lock; }

	gmVariable m_variables[10];
	gmUserObject * m_userObjects[10];

	uint32 m_userObjectCounter;

protected:
	void SetPlayerFunctionTable();
	void SetUnitFunctionTable();
	void SetGameObjectFunctionTable();
	void SetQuestFunctionTable();
	void SetSpellFunctionTable();
	void SetAuraFunctionTable();
	void SetAreaTriggerFunctionTable();
	void SetScriptEngineFunctionTable();

public:
	inline void AddAreaTriggerEvent(uint32 Entry, gmFunctionObject * func) { m_areaTriggerMap[Entry] = func; }
	inline void AddQuestEvent(uint32 Entry, uint32 Type, gmFunctionObject * func) { m_questMap[Entry].insert( make_pair( Type, func ) ); }
	inline void AddCreatureEvent(uint32 Entry, uint32 Type, gmFunctionObject * func) { m_unitMap[Entry].insert( make_pair( Type, func ) ); }
	inline void AddGameObjectEvent(uint32 Entry, uint32 Type, gmFunctionObject * func) { m_gameObjectMap[Entry].insert( make_pair( Type, func ) ); }

	GMEngine();

	~GMEngine();

	void Reload();

	void RegisterAreaTriggerEvent(uint32 Entry, gmFunctionObject * Function, gmType Type);

	void ExecuteScriptFile(const char * filename);

	void DumpErrors();

	void DoGMCall(gmFunctionObject * obj, uint32 ArgumentCount, int * return_value);

	inline void SetVariable(uint32 Index, void * Pointer, gmType Type)
	{
		m_userObjects[Index]->m_userType = Type;
		m_userObjects[Index]->m_user = Pointer;
		m_variables[Index].SetUser(m_userObjects[Index]);
	}

	bool OnActivateAreaTrigger(AreaTrigger * at, Player * plr);
	
	bool OnQuestEvent(Quest * quest, Creature * pQuestGiver, Player * plr, uint32 Event);
	bool OnQuestRequireEvent(Quest * quest, Creature * pQuestGiver, Player * plr, uint32 Event);

	bool OnCreatureEvent(Creature * pCreature, gmFunctionObject * pointer);
	bool OnCreatureEvent(Creature * pCreature, Unit * pAttacker, uint32 Event);
	bool OnCreatureEventArg(Creature * pCreature, uint32 Argument, uint32 Event);

	bool OnGameObjectEvent(GameObject * pGameObject, Player * pUser, uint32 Event);

	inline gmMachine * GetMachine() { return m_machine; }

	bool HasEventType(uint32 Entry, uint32 Event);
	bool CallGMFunction(const char * name, Object* forthis);
};

extern GMEngine * GMSystem;