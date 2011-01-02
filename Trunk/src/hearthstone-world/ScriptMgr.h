/*
 * Sandshroud Hearthstone
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
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

#ifndef SCRIPTMGR_H
#define SCRIPTMGR_H

#define SCRIPT_MODULE void*
#define ADD_CREATURE_FACTORY_FUNCTION(cl) static CreatureAIScript * Create(Creature* c) { return new cl(c); }
#define ADD_INSTANCE_FACTORY_FUNCTION( ClassName ) static InstanceScript* Create( MapMgr* pMapMgr ) { return new ClassName( pMapMgr ); };
#define ADD_GAMEOBJECT_FACTORY_FUNCTION(cl) static GameObjectAIScript * Create(GameObject* go) { return new cl(go); }

class Channel;
class Guild;
struct Quest;

enum ServerHookEvents
{
	SERVER_HOOK_EVENT_ON_NEW_CHARACTER		= 1,
	SERVER_HOOK_EVENT_ON_KILL_PLAYER		= 2,
	SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD	= 3,
	SERVER_HOOK_EVENT_ON_ENTER_WORLD		= 4,
	SERVER_HOOK_EVENT_ON_GUILD_JOIN			= 5,
	SERVER_HOOK_EVENT_ON_DEATH				= 6,
	SERVER_HOOK_EVENT_ON_REPOP				= 7,
	SERVER_HOOK_EVENT_ON_EMOTE				= 8,
	SERVER_HOOK_EVENT_ON_ENTER_COMBAT		= 9,
	SERVER_HOOK_EVENT_ON_CAST_SPELL			= 10,
	SERVER_HOOK_EVENT_ON_TICK				= 11,
	SERVER_HOOK_EVENT_ON_LOGOUT_REQUEST		= 12,
	SERVER_HOOK_EVENT_ON_LOGOUT				= 13,
	SERVER_HOOK_EVENT_ON_QUEST_ACCEPT		= 14,
	SERVER_HOOK_EVENT_ON_ZONE				= 15,
	SERVER_HOOK_EVENT_ON_CHAT				= 16,
	SERVER_HOOK_EVENT_ON_LOOT				= 17,
	SERVER_HOOK_EVENT_ON_GUILD_CREATE		= 18,
	SERVER_HOOK_EVENT_ON_FULL_LOGIN			= 19,
	SERVER_HOOK_EVENT_ON_CHARACTER_CREATE	= 20,
	SERVER_HOOK_EVENT_ON_QUEST_CANCELLED	= 21,
	SERVER_HOOK_EVENT_ON_QUEST_FINISHED		= 22,
	SERVER_HOOK_EVENT_ON_HONORABLE_KILL		= 23,
	SERVER_HOOK_EVENT_ON_ARENA_FINISH		= 24,
	SERVER_HOOK_EVENT_ON_OBJECTLOOT			= 25,
	SERVER_HOOK_EVENT_ON_AREATRIGGER		= 26,
	SERVER_HOOK_EVENT_ON_POST_LEVELUP		= 27,
	SERVER_HOOK_EVENT_ON_PRE_DIE			= 28,
	SERVER_HOOK_EVENT_ON_ADVANCE_SKILLLINE	= 29,
	SERVER_HOOK_EVENT_ON_DUEL_FINISHED		= 30,
	SERVER_HOOK_EVENT_ON_AURA_REMOVE		= 31,
	SERVER_HOOK_EVENT_ON_RESURRECT			= 32,
	SERVER_HOOK_EVENT_ON_UPDATE				= 33,
	SERVER_HOOK_EVENT_ON_CONTINENT_CREATE	= 34,
	SERVER_HOOK_EVENT_ON_POST_SPELL_CAST	= 35,
	SERVER_HOOK_EVENT_ON_PLAYER_SAVE_TO_DB	= 36,

	// Destructable Buildings
	SERVER_HOOK_EVENT_ON_DESTROY_BUILDING	= 37,
	SERVER_HOOK_EVENT_ON_DAMAGE_BUILDING	= 38,
	SERVER_HOOK_EVENT_ON_MOUNT_FLYING		= 39,
	SERVER_HOOK_EVENT_ON_PRE_AURA_REMOVE	= 40,
	SERVER_HOOK_EVENT_ON_SLOW_LOCK_OPEN		= 41,
	NUM_SERVER_HOOKS,
};

enum ScriptTypes
{
	SCRIPT_TYPE_MISC						= 0x01,
	SCRIPT_TYPE_INFODUMPER					= 0x02,
	SCRIPT_TYPE_SCRIPT_ENGINE				= 0x04,
	SCRIPT_TYPE_SCRIPT_ENGINE_LUA			= 0x08,
	SCRIPT_TYPE_SCRIPT_ENGINE_GM			= 0x10,
};


/* Hook typedefs */
typedef bool(*tOnNewCharacter)(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
typedef void(*tOnKillPlayer)(Player* pPlayer, Player* pVictim);
typedef void(*tOCharacterCreate)(Player* pPlayer);
typedef void(*tOnFirstEnterWorld)(Player* pPlayer);
typedef void(*tOnEnterWorld)(Player* pPlayer);
typedef void(*tOnFullLogin)(Player* pPlayer);
typedef void(*tOnGuildCreate)(Player* pLeader, Guild * pGuild);
typedef void(*tOnGuildJoin)(Player* pPlayer, Guild * pGuild);
typedef void(*tOnDeath)(Player* pPlayer);
typedef bool(*tOnRepop)(Player* pPlayer);
typedef void(*tOnEmote)(Player* pPlayer, uint32 Emote, Unit* pUnit);
typedef void(*tOnEnterCombat)(Player* pPlayer, Unit* pTarget);
typedef bool(*tOnCastSpell)(Player* pPlayer, SpellEntry * pSpell);
typedef void(*tOnTick)();
typedef bool(*tOnLogoutRequest)(Player* pPlayer);
typedef void(*tOnLogout)(Player* pPlayer);
typedef void(*tOnQuestAccept)(Player* pPlayer, Quest * pQuest, Object* pObject);
typedef void(*tOnZone)(Player* pPlayer, uint32 Zone, uint32 OldZone);
typedef bool(*tOnChat)(Player* pPlayer, uint32 Type, uint32 Lang, string Message, string Misc);
typedef void(*tOnLoot)(Player * pPlayer, Object* pTarget, uint32 Money, uint32 ItemId);
typedef bool(*ItemScript)(Item* pItem, Player* pPlayer);
typedef void(*tOnQuestCancel)(Player* pPlayer, Quest * pQuest);
typedef void(*tOnQuestFinished)(Player* pPlayer, Quest * pQuest, Object* pObject);
typedef void(*tOnHonorableKill)(Player* pPlayer, Player* pKilled);
typedef void(*tOnArenaFinish)(Player* pPlayer, uint32 type, ArenaTeam * pTeam, bool victory, bool rated);
typedef void(*tOnObjectLoot)(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId);
typedef void(*tOnAreaTrigger)(Player * pPlayer, uint32 areaTrigger);
typedef void(*tOnPostLevelUp)(Player * pPlayer);
typedef bool(*tOnPreUnitDie)(Unit *killer, Unit *target);
typedef void(*tOnAdvanceSkillLine)(Player * pPlayer, uint32 SkillLine, uint32 Current);
typedef void(*tOnDuelFinished)(Player * Winner, Player * Looser);
typedef void(*tOnAuraRemove)(Player* pPlayer, uint32 spellID);
typedef bool(*tOnResurrect)(Player * pPlayer);
typedef void(*tOnUpdate)(Player * plr);
typedef void(*tOnContinentCreate)(MapMgr* mgr);
typedef void(*tOnPostSpellCast)(Player* pPlayer, SpellEntry * pSpell, Unit* pTarget);
typedef void(*tOnPlayerSaveToDB)(Player* pPlayer, QueryBuffer* buf);

//Destructable building
typedef void(*tOnDestroyBuilding)(GameObject* go);
typedef void(*tOnDamageBuilding)(GameObject* go);
typedef bool(*tOnMountFlying) (Player* plr);
typedef bool(*tOnPreAuraRemove)(Player* plr,uint32 spellID);
typedef void(*tOnSlowLockOpen)(GameObject* go,Player* plr);

class CreatureAIScript;
class GossipScript;
class GameObjectAIScript;
class InstanceScript;
class ScriptMgr;
struct ItemPrototype;

/* Factory Imports (from script lib) */
typedef CreatureAIScript*(*exp_create_creature_ai)(Creature* pCreature);
typedef GameObjectAIScript*(*exp_create_gameobject_ai)(GameObject* pGameObject);
typedef InstanceScript* ( *exp_create_instance_ai )( MapMgr* pMapMgr );
typedef bool(*exp_handle_dummy_spell)(uint32 i, Spell* pSpell);
typedef bool(*exp_handle_dummy_aura)(uint32 i, Aura* pAura, bool apply);
typedef void(*exp_script_register)(ScriptMgr * mgr);
typedef uint32(*exp_get_script_type)();
typedef uint32(*exp_get_version)();

/* Hashmap typedefs */
typedef HM_NAMESPACE::hash_map<uint32, exp_create_creature_ai> CreatureCreateMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_create_gameobject_ai> GameObjectCreateMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_handle_dummy_aura> HandleDummyAuraMap;
typedef HM_NAMESPACE::hash_map<uint32, exp_handle_dummy_spell> HandleDummySpellMap;
typedef HM_NAMESPACE::hash_map< uint32, exp_create_instance_ai > InstanceCreateMap;
typedef map<uint8, map<uint32, GossipScript*> > MultiTypeGossipMap;
typedef map<uint32, QuestScript* > QuestScriptMap;
typedef set<GossipScript*> CustomGossipScripts;
typedef set<QuestScript*> QuestScripts;
typedef list<void*> ServerHookList;
typedef list<SCRIPT_MODULE> LibraryHandleMap;

enum GOSSIPTYPEID
{
	GTYPEID_CTR			= 0,
	GTYPEID_ITEM		= 1,
	GTYPEID_GAMEOBJECT	= 2,
};

class SERVER_DECL ScriptMgr : public Singleton<ScriptMgr>
{
public:
	ScriptMgr();
	~ScriptMgr();

	friend class HookInterface;

	void LoadScripts();
	void UnloadScripts(bool safe = false);
	void ReloadScripts();

	CreatureAIScript * CreateAIScriptClassForEntry(Creature* pCreature);
	GameObjectAIScript * CreateAIScriptClassForGameObject(uint32 uEntryId, GameObject* pGameObject);
	InstanceScript* CreateScriptClassForInstance( uint32 pMapId, MapMgr* pMapMgr );

	bool CallScriptedDummySpell(uint32 uSpellId, uint32 i, Spell* pSpell);
	bool CallScriptedDummyAura( uint32 uSpellId, uint32 i, Aura*  pAura, bool apply);
	bool CallScriptedItem(Item* pItem, Player* pPlayer);

	void register_creature_script(uint32 entry, exp_create_creature_ai callback);
	void register_gameobject_script(uint32 entry, exp_create_gameobject_ai callback);
	void register_gossip_script(uint32 entry, GossipScript * gs);
	void register_go_gossip_script(uint32 entry, GossipScript * gs);
	void register_dummy_aura(uint32 entry, exp_handle_dummy_aura callback);
	void register_dummy_spell(uint32 entry, exp_handle_dummy_spell callback);
	void register_hook(ServerHookEvents event, void * function_pointer);
	void register_item_gossip_script(uint32 entry, GossipScript * gs);
	void register_quest_script(uint32 entry, QuestScript * qs);
	void register_instance_script( uint32 pMapId, exp_create_instance_ai pCallback );

	HEARTHSTONE_INLINE QuestScript* GetQuestScript(uint32 entry) { return EntryQuestScriptMap[entry]; };
	HEARTHSTONE_INLINE GossipScript* GetRegisteredGossipScript(uint8 type, uint32 entry, bool rdefault = true)
	{
		if(GossipMaps[type][entry] != NULL)
			return GossipMaps[type][entry];
		if(rdefault)
			return DefaultGossipScript;
		return NULL;
	};

protected:

	InstanceCreateMap mInstances;
	CreatureCreateMap _creatures;
	GameObjectCreateMap _gameobjects;
	HandleDummyAuraMap _auras;
	HandleDummySpellMap _spells;
	LibraryHandleMap _handles;
	GossipScript * DefaultGossipScript;
	ServerHookList _hooks[NUM_SERVER_HOOKS];
	CustomGossipScripts _customgossipscripts;
	QuestScripts _questscripts;
	MultiTypeGossipMap GossipMaps;
	QuestScriptMap EntryQuestScriptMap;
};

class SERVER_DECL CreatureAIScript
{
public:
	CreatureAIScript(Creature* creature);
	virtual ~CreatureAIScript() {};

	virtual void OnCombatStart(Unit* mTarget) {}
	virtual void OnCombatStop(Unit* mTarget) {}
	virtual void OnDamageTaken(Unit* mAttacker, float fAmount) {}
	virtual void OnCastSpell(uint32 iSpellId) {}
	virtual void OnTargetParried(Unit* mTarget) {}
	virtual void OnTargetDodged(Unit* mTarget) {}
	virtual void OnTargetBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnTargetCritHit(Unit* mTarget, float fAmount) {}
	virtual void OnTargetDied(Unit* mTarget) {}
	virtual void OnParried(Unit* mTarget) {}
	virtual void OnDodged(Unit* mTarget) {}
	virtual void OnBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnCritHit(Unit* mTarget, float fAmount) {}
	virtual void OnHit(Unit* mTarget, float fAmount) {}
	virtual void OnDied(Unit* mKiller) {}
	virtual void OnAssistTargetDied(Unit* mAssistTarget) {}
	virtual void OnFear(Unit* mFeared, uint32 iSpellId) {}
	virtual void OnFlee(Unit* mFlee) {}
	virtual void OnCallForHelp() {}
	virtual void OnLoad() {}
	virtual void OnReachWP(uint32 iWaypointId, bool bForwards) {}
	virtual void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype) {}
	virtual void AIUpdate() {}
	virtual void OnEmote(Player* pPlayer, EmoteType Emote) {}
	virtual void StringFunctionCall(int) {}

	void RegisterAIUpdateEvent(uint32 frequency);
	void ModifyAIUpdateEvent(uint32 newfrequency);
	void RemoveAIUpdateEvent();

	virtual void Destroy() { delete this; }
	Creature* GetUnit() { return _unit; }

protected:
	Creature* _unit;
};

class SERVER_DECL GameObjectAIScript
{
public:
	GameObjectAIScript(GameObject* goinstance);
	virtual ~GameObjectAIScript() {}

	virtual void OnCreate() {}
	virtual void OnSpawn() {}
	virtual void OnDespawn() {}
	virtual void OnLootTaken(Player * pLooter, ItemPrototype *pItemInfo) {}
	virtual void OnActivate(Player* pPlayer) {}
	virtual void AIUpdate() {}
	virtual void Destroy() { delete this; }

	void RegisterAIUpdateEvent(uint32 frequency);
	void RemoveAIUpdateEvent();

protected:

	GameObject* _gameobject;
};

class SERVER_DECL GossipScript
{
public:
	GossipScript();
	virtual ~GossipScript() {}

	virtual void GossipHello(Object* pObject, Player* Plr, bool AutoSend);
	virtual void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * EnteredCode);
	virtual void GossipEnd(Object* pObject, Player* Plr);
	virtual void Destroy();
};

class SERVER_DECL QuestScript
{
public:
	QuestScript() {};
	virtual ~QuestScript() {};

	virtual void OnQuestStart(Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnQuestCancel(Player* mTarget) {}
	virtual void OnGameObjectActivate(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnCreatureKill(uint32 entry, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnExploreArea(uint32 areaId, Player* mTarget, QuestLogEntry *qLogEntry) {}
	virtual void OnPlayerItemPickup(uint32 itemId, uint32 totalCount, Player* mTarget, QuestLogEntry *qLogEntry) {}
};

/* * Class InstanceScript
   * Instanced class created for each instance of the map, holds all
   * scriptable exports
*/

class SERVER_DECL InstanceScript
{
public:
	InstanceScript( MapMgr* pMapMgr );
	virtual ~InstanceScript() {};

	// Procedures that had been here before
	virtual GameObject *		GetObjectForOpenLock( Player* pCaster, Spell* pSpell, SpellEntry* pSpellEntry ) { return NULLGOB; };
	virtual void				SetLockOptions( uint32 pEntryId, GameObject* pGameObject ) {};
	virtual uint32				GetRespawnTimeForCreature( uint32 pEntryId, Creature* pCreature) { return 240000; };

	// Player
	virtual void				OnPlayerDeath( Player* pVictim, Unit* pKiller ) {};

	// Area and AreaTrigger
	virtual void				OnPlayerEnter( Player* pPlayer ) {};
	virtual void				OnAreaTrigger( Player* pPlayer, uint32 pAreaId ) {};
	virtual void				OnZoneChange( Player* pPlayer, uint32 pNewZone, uint32 pOldZone ) {};

	// Data get / set - idea taken from ScriptDev2
	virtual void				SetInstanceData( uint32 pType, uint32 pIndex, uint32 pData ) {};
	virtual uint32				GetInstanceData( uint32 pType, uint32 pIndex ) { return 0; };

	// Creature / GameObject - part of it is simple reimplementation for easier use Creature / GO < --- > Script
	virtual void				OnCreatureDeath( Creature* pVictim, Unit* pKiller ) {};
	virtual void				OnCreaturePushToWorld( Creature* pCreature ) {};
	virtual void				OnGameObjectActivate( GameObject* pGameObject, Player* pPlayer ) {};
	virtual void				OnGameObjectPushToWorld( GameObject* pGameObject ) {};

	// Standard virtual methods
	virtual void				OnLoad() {};
	virtual void				UpdateEvent() {};
	virtual void				Destroy() {};

	// UpdateEvent
	void						RegisterUpdateEvent( uint32 pFrequency );
	void						ModifyUpdateEvent( uint32 pNewFrequency );
	void						RemoveUpdateEvent();

	// Something to return Instance's MapMgr
	MapMgr*						GetInstance() { return mInstance; };

protected:

	MapMgr* mInstance;
};

class SERVER_DECL HookInterface : public Singleton<HookInterface>
{
public:
	friend class ScriptMgr;

	bool OnNewCharacter(uint32 Race, uint32 Class, WorldSession * Session, const char * Name);
	void OnKillPlayer(Player* pPlayer, Player* pVictim);
	void OnFirstEnterWorld(Player* pPlayer);
	void OnEnterWorld(Player* pPlayer);
	void OnGuildCreate(Player* pLeader, Guild * pGuild);
	void OnGuildJoin(Player* pPlayer, Guild * pGuild);
	void OnDeath(Player* pPlayer);
	bool OnRepop(Player* pPlayer);
	void OnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit);
	void OnEnterCombat(Player* pPlayer, Unit* pTarget);
	bool OnCastSpell(Player* pPlayer, SpellEntry * pSpell);
	bool OnLogoutRequest(Player* pPlayer);
	void OnLogout(Player* pPlayer);
	void OnQuestAccept(Player* pPlayer, Quest * pQuest, Object* pObject);
	void OnZone(Player* pPlayer, uint32 Zone, uint32 OldZone);
	bool OnChat(Player* pPlayer, uint32 Type, uint32 Lang, string Message, string Misc);
	void OnLoot(Player * pPlayer, Object* pTarget, uint32 Money, uint32 ItemId);
	void OnFullLogin(Player* pPlayer);
	void OnCharacterCreate(Player* pPlayer);
	void OnQuestCancelled(Player* pPlayer, Quest * pQuest);
	void OnQuestFinished(Player* pPlayer, Quest * pQuest, Object* pObject);
	void OnHonorableKill(Player* pPlayer, Player* pKilled);
	void OnArenaFinish(Player* pPlayer, uint32 type, ArenaTeam* pTeam, bool victory, bool rated);
	void OnObjectLoot(Player * pPlayer, Object * pTarget, uint32 Money, uint32 ItemId);
	void OnAreaTrigger(Player * pPlayer, uint32 areaTrigger);
	void OnPostLevelUp(Player * pPlayer);
	bool OnPreUnitDie(Unit *Killer, Unit *Victim);
	void OnAdvanceSkillLine(Player * pPlayer, uint32 SkillLine, uint32 Current);
	void OnDuelFinished(Player * Winner, Player * Looser);
	void OnUpdate(Player *plr);
	void OnContinentCreate(MapMgr* pMgr);
	void OnPostSpellCast(Player* pPlayer, SpellEntry * pSpell, Unit* pTarget);
	void OnPlayerSaveToDB(Player* pPlayer, QueryBuffer* buf);
	void OnAuraRemove(Player* pPlayer, uint32 spellID);
	bool OnResurrect(Player * pPlayer);

	//Destructable buildings
	void OnDestroyBuilding(GameObject* go);
	void OnDamageBuilding(GameObject* go);
	bool OnMountFlying(Player* plr);
	bool OnPreAuraRemove(Player* remover,uint32 spellID);
	void OnSlowLockOpen(GameObject* go,Player* plr);
};

#define sScriptMgr ScriptMgr::getSingleton()
#define sHookInterface HookInterface::getSingleton()

#endif
