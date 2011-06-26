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

#include "StdAfx.h"

#ifdef M_PI
#undef M_PI
#endif

#define M_PI	   3.14159265358979323846f

Creature::Creature(uint64 guid)
{
#ifdef SHAREDPTR_DEBUGMODE
	printf("Creature::Creature()\n");
#endif
	m_valuesCount = UNIT_END;
	m_objectTypeId = TYPEID_UNIT;
	m_uint32Values = _fields;
	memset(m_uint32Values, 0,(UNIT_END)*sizeof(uint32));
	m_updateMask.SetCount(UNIT_END);
	SetUInt32Value( OBJECT_FIELD_TYPE, TYPE_UNIT|TYPE_OBJECT);
	SetUInt64Value( OBJECT_FIELD_GUID, guid);
	m_wowGuid.Init(GetGUID());
	m_quests = NULL;
	proto = NULL;
	LoadedProto = NULL;
	creature_info = NULL;
	m_H_regenTimer = 0;
	m_P_regenTimer = 0;
	m_useAI = true;
	mTaxiNode = 0;
	Skinned = false;
	m_enslaveCount = 0;
	m_enslaveSpell = 0;
	for(uint32 x = 0; x < 7; x++)
	{
		FlatResistanceMod[x] = 0;
		BaseResistanceModPct[x] = 0;
		ResistanceModPct[x] = 0;
	}

	for(uint32 x=0;x<5;x++)
	{
		TotalStatModPct[x]=0;
		StatModPct[x]=0;
		FlatStatMod[x]=0;
	}
	SummonOwner = 0;
	SummonSlot = -1;
	Totem = false;
	m_PickPocketed = false;
	m_SellItems = NULL;
	_myScriptClass = NULL;
	m_TaxiNode = 0;
	myFamily = 0;
	haslinkupevent = false;
	original_emotestate = 0;
	mTrainer = 0;
	m_spawn = 0;
	auctionHouse = 0;
	has_waypoint_text = has_combat_text = false;
	SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 1);
	SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 1);
	m_custom_waypoint_map = 0;
	m_escorter = NULLPLR;
	m_limbostate = false;
	m_corpseEvent=false;
	m_respawnCell=NULL;
	m_walkSpeed = 2.5f;
	m_runSpeed = MONSTER_NORMAL_RUN_SPEED;
	m_base_runSpeed = m_runSpeed;
	m_base_walkSpeed = m_walkSpeed;
	m_noRespawn=false;
	m_canRegenerateHP = true;
	BaseAttackType = SCHOOL_NORMAL;
	m_taggingPlayer = m_taggingGroup = 0;
	m_lootMethod = -1;
	m_noDeleteAfterDespawn = false;
	spawnid = 0;
}

Creature::~Creature()
{

}

void Creature::Init()
{
	Unit::Init();
}

void Creature::Destruct()
{
	sEventMgr.RemoveEvents(this);

	if(m_escorter)
		m_escorter = NULLPLR;

	if(_myScriptClass != 0)
		_myScriptClass->Destroy();

	if(m_custom_waypoint_map != 0)
	{
		for(WayPointMap::iterator itr = m_custom_waypoint_map->begin(); itr != m_custom_waypoint_map->end(); itr++)
			delete (*itr);
		delete m_custom_waypoint_map;
	}
	if(m_respawnCell!=NULL)
		m_respawnCell->_respawnObjects.erase(this);
	Unit::Destruct();
}

void Creature::Update( uint32 p_time )
{
	Unit::Update( p_time );
	if(IsTotem() && isDead())
	{
		RemoveFromWorld(false, true);
		return;
	}

	if(m_corpseEvent)
	{
		sEventMgr.RemoveEvents(this);
		if(proto == NULL)
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, 1000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if (creature_info->Rank == ELITE_WORLDBOSS || creature_info->Flags1 & CREATURE_FLAGS1_BOSS)
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_BOSSCORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else if ( creature_info->Rank == ELITE_RAREELITE || creature_info->Rank == ELITE_RARE)
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_RARECORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		else
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::OnRemoveCorpse, EVENT_CREATURE_REMOVE_CORPSE, TIME_CREATURE_REMOVE_CORPSE, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		m_corpseEvent = false;
	}
}

void Creature::SafeDelete()
{
	sEventMgr.RemoveEvents(this);
	sEventMgr.AddEvent(TO_CREATURE(this), &Creature::DeleteMe, EVENT_CREATURE_SAFE_DELETE, 1000, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Creature::DeleteMe()
{
	if(IsInWorld())
		RemoveFromWorld(false, true);

	Destruct();
}

void Creature::OnRemoveCorpse()
{
	// time to respawn!
	if (IsInWorld() && (int32)m_mapMgr->GetInstanceID() == m_instanceId)
	{

		DEBUG_LOG("Creature","OnRemoveCorpse Removing corpse of "I64FMT"...", GetGUID());

		if((GetMapMgr()->GetMapInfo() && GetMapMgr()->GetdbcMap()->israid() && proto && proto->boss) || m_noRespawn)
		{
			RemoveFromWorld(false, true);
		}
		else
		{
			if(proto && proto->RespawnTime)
				RemoveFromWorld(true, false);
			else
				RemoveFromWorld(false, true);
		}

		setDeathState(DEAD);
		SetPosition(m_spawnLocation, true);
	}
}

void Creature::OnRespawn( MapMgr* m)
{
	OUT_DEBUG("Respawning "I64FMT"...", GetGUID());
	SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0); // not tagging shiat
	if(proto && m_spawn)
	{
		SetUInt32Value(UNIT_NPC_FLAGS, proto->NPCFLags);
		SetUInt32Value(UNIT_NPC_EMOTESTATE, m_spawn->emote_state);
		SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID,m_spawn->MountedDisplayID);
	}

	RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
	Skinned = false;
	m_taggingGroup = m_taggingPlayer = 0;
	m_lootMethod = -1;

	/* creature death state */
	if(proto && proto->death_state == 1)
	{
		uint32 newhealth = m_uint32Values[UNIT_FIELD_HEALTH] / 100;
		if(!newhealth)
			newhealth = 1;
		SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		m_limbostate = true;
		bInvincible = true;
		SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
	}

	setDeathState(ALIVE);
	GetAIInterface()->StopMovement(0); // after respawn monster can move
	m_PickPocketed = false;
	PushToWorld(m);
}

void Creature::Create (const char* name, uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create( mapid, x, y, z, ang );
}

void Creature::CreateWayPoint (uint32 WayPointID, uint32 mapid, float x, float y, float z, float ang)
{
	Object::_Create( mapid, x, y, z, ang);
}

///////////
/// Looting
uint32 Creature::GetRequiredLootSkill()
{
	if(GetCreatureInfo()->TypeFlags & CREATURE_FLAGS1_HERBLOOT)
		return SKILL_HERBALISM;     // herbalism
	else if(GetCreatureInfo()->TypeFlags & CREATURE_FLAGS1_MININGLOOT)
		return SKILL_MINING;        // mining
	else if(GetCreatureInfo()->Flags1 & CREATURE_FLAGS1_ENGINEERLOOT)
		return SKILL_ENGINERING;
	else
		return SKILL_SKINNING;      // skinning
};

void Creature::GenerateLoot()
{
	if(IsPet() || proto == NULL)
	{
		m_loot.gold = 0;
		return;
	}

	uint8 team = 0;
	uint8 difficulty = (m_mapMgr ? (m_mapMgr->iInstanceMode) : 0);
	if(CombatStatus.GetKiller() && CombatStatus.GetKiller()->IsPlayer())
		team = TO_PLAYER(CombatStatus.GetKiller())->GetTeam();

	lootmgr.FillCreatureLoot(&m_loot, GetEntry(), difficulty, team);

	// -1 , no gold; 0 calculated according level; >0 coppercoins
	if( proto->money == -1)
	{
		m_loot.gold = 0;
		return;
	}

	if(proto->money == 0)
	{
		CreatureInfo *info = GetCreatureInfo();
		if (info && info->Type != BEAST)
		{
			if(m_uint32Values[UNIT_FIELD_MAXHEALTH] <= 1667)
				m_loot.gold = uint32((info->Rank+1)*getLevel()*((rand()%5) + 1)); //generate copper
			else
				m_loot.gold = uint32((info->Rank+1)*getLevel()*((rand()%5) + 1)*(GetUInt32Value(UNIT_FIELD_MAXHEALTH)*0.0006)); //generate copper
		}
		else //Beast don't drop money
			m_loot.gold = 0;
	}
	else
		m_loot.gold = uint32(proto->money);

	if(m_loot.gold)
		m_loot.gold = int32(float(m_loot.gold) * sWorld.getRate(RATE_MONEY));
}

void Creature::SaveToDB(bool saveposition /*= false*/)
{
	if(IsPet() || IsSummon()) //Just in case.
		return;

	if(!spawnid)
		spawnid = objmgr.GenerateCreatureSpawnID();

	float savex = (!saveposition && (m_spawn != NULL)) ? m_spawn->x : m_position.x;
	float savey = (!saveposition && (m_spawn != NULL)) ? m_spawn->y : m_position.y;
	float savez = (!saveposition && (m_spawn != NULL)) ? m_spawn->z : m_position.z;
	float saveo = (!saveposition && (m_spawn != NULL)) ? m_spawn->o : m_position.o;
	std::stringstream ss;
	ss << "REPLACE INTO creature_spawns VALUES("
		<< spawnid << ","
		<< GetEntry() << ","
		<< GetMapId() << ","
		<< savex << ","
		<< savey << ","
		<< savez << ","
		<< saveo << ","
		<< m_aiInterface->getMoveType() << ","
		<< 0 << "," //Uses random display from proto. Setting a displayid manualy will override proto lookup
		<< m_uint32Values[UNIT_FIELD_FACTIONTEMPLATE] << ","
		<< m_uint32Values[UNIT_FIELD_FLAGS] << ","
		<< m_uint32Values[UNIT_FIELD_BYTES_0] << ","
		<< m_uint32Values[UNIT_FIELD_BYTES_1] << ","
		<< m_uint32Values[UNIT_FIELD_BYTES_2] << ","
		<< uint32(original_emotestate) << ",";

	if(m_spawn)
		ss << m_spawn->channel_spell << "," << m_spawn->channel_target_go << "," << m_spawn->channel_target_creature << ",";
	else
		ss << "0,0,0,";

	ss << uint32(GetStandState()) << ", "
		<< uint32(m_floatValues[OBJECT_FIELD_SCALE_X]) << ", "
		<< (m_spawn ? m_spawn->MountedDisplayID : original_MountedDisplayID) << ", "
		<< m_uint32Values[UNIT_VIRTUAL_ITEM_SLOT_ID] << ", "
		<< m_uint32Values[UNIT_VIRTUAL_ITEM_SLOT_ID+1] << ", "
		<< m_uint32Values[UNIT_VIRTUAL_ITEM_SLOT_ID+2] << ", "
		<< m_phaseMask << ", "
		<< uint32(IsVehicle() ? TO_VEHICLE(this)->GetVehicleEntry() : 0) << ", "
		<< uint32(GetCanMove()) << ", "
		<< uint32(VendorMask) << " )";

	WorldDatabase.Execute(ss.str().c_str());
}

void Creature::LoadScript()
{
	_myScriptClass = sScriptMgr.CreateAIScriptClassForEntry(TO_CREATURE(this));
}

void Creature::DeleteFromDB()
{
	if( m_spawn == NULL )
		return;

	WorldDatabase.Execute("DELETE FROM creature_spawns WHERE id=%u",  GetSQL_id());
	WorldDatabase.Execute("DELETE FROM creature_waypoints WHERE spawnid=%u",  GetSQL_id());
}

/////////////
/// Quests

void Creature::AddQuest(QuestRelation *Q)
{
	m_quests->push_back(Q);
}

void Creature::DeleteQuest(QuestRelation *Q)
{
	list<QuestRelation *>::iterator it;
	for ( it = m_quests->begin(); it != m_quests->end(); it++ )
	{
		if (((*it)->type == Q->type) && ((*it)->qst == Q->qst ))
		{
			delete (*it);
			m_quests->erase(it);
			break;
		}
	}
}

Quest* Creature::FindQuest(uint32 quest_id, uint8 quest_relation)
{
	list<QuestRelation *>::iterator it;
	for (it = m_quests->begin(); it != m_quests->end(); it++)
	{
		QuestRelation *ptr = (*it);

		if ((ptr->qst->id == quest_id) && (ptr->type & quest_relation))
		{
			return ptr->qst;
		}
	}
	return NULL;
}

uint16 Creature::GetQuestRelation(uint32 quest_id)
{
	uint16 quest_relation = 0;
	list<QuestRelation *>::iterator it;

	for (it = m_quests->begin(); it != m_quests->end(); it++)
	{
		if ((*it)->qst->id == quest_id)
		{
			quest_relation |= (*it)->type;
		}
	}
	return quest_relation;
}

uint32 Creature::NumOfQuests()
{
	return (uint32)m_quests->size();
}

void Creature::_LoadQuests()
{
	sQuestMgr.LoadNPCQuests(TO_CREATURE(this));
}

void Creature::CreatureSetDeathState(DeathState s)
{
	if(s == JUST_DIED)
	{
		GetAIInterface()->SetUnitToFollow(NULLUNIT);

		//despawn all summons we created
		SummonExpireAll(true);

		if(m_enslaveSpell)
			RemoveEnslave();

		if(m_currentSpell)
			m_currentSpell->cancel();

		m_deathState = CORPSE;
		m_corpseEvent = true;
		if ( lootmgr.IsSkinnable(GetEntry()))
			SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
	}
}

void Creature::AddToWorld()
{
	if(creature_info == 0)
	{
		creature_info = CreatureNameStorage.LookupEntry(GetEntry());
		if(creature_info == 0)
			return;
	}

	// force set faction
	if(m_faction == NULL || m_factionDBC == NULL)
		_setFaction();
	if(m_faction == NULL || m_factionDBC == NULL)
		return;

	Object::AddToWorld();
}

void Creature::AddToWorld(MapMgr* pMapMgr)
{
	if(creature_info == NULL)
	{
		creature_info = CreatureNameStorage.LookupEntry(GetEntry());
		if(creature_info == NULL)
			return;
	}

	// force set faction
	if(m_faction == NULL || m_factionDBC == NULL)
		_setFaction();
	if(m_faction == NULL || m_factionDBC == NULL)
		return;

	Object::AddToWorld(pMapMgr);
}

bool Creature::CanAddToWorld()
{
	if(creature_info == NULL || proto == NULL)
		return false;

	if(m_faction == NULL || m_factionDBC == NULL)
	{
		_setFaction();

		if(m_faction == NULL || m_factionDBC == NULL)
			return false;
	}

	return true;
}

void Creature::RemoveFromWorld(bool addrespawnevent, bool free_guid)
{
	m_taggingPlayer = m_taggingGroup = 0;
	m_lootMethod = 1;

	RemoveAllAuras();

	if(IsPet())
	{
		if(IsInWorld())
			Unit::RemoveFromWorld(true);

		SafeDelete();
	}

	if(this != NULL && IsInWorld())
	{
		uint32 delay = 0;
		if(addrespawnevent && proto && proto->RespawnTime > 0)
			delay = proto->RespawnTime;
		Despawn(0, delay);
	}
}

void Creature::EnslaveExpire()
{
	m_enslaveCount++;
	Player* caster = objmgr.GetPlayer(GetUInt32Value(UNIT_FIELD_CHARMEDBY));
	if(caster)
	{
		caster->SetUInt64Value(UNIT_FIELD_CHARM, 0);
		caster->SetUInt64Value(UNIT_FIELD_SUMMON, 0);
		WorldPacket data(8);
		data.Initialize(SMSG_PET_SPELLS);
		data << uint64(0);
		caster->GetSession()->SendPacket(&data);
	}
	SetUInt64Value(UNIT_FIELD_CHARMEDBY, 0);
	SetUInt64Value(UNIT_FIELD_SUMMONEDBY, 0);
	SetIsPet(false);

	m_walkSpeed = m_base_walkSpeed;
	m_runSpeed = m_base_runSpeed;
	if(proto)
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, proto->Faction);
	else
		SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 90);
	_setFaction();

	GetAIInterface()->Init(this, AITYPE_AGRO, MOVEMENTTYPE_NONE);

	// Update InRangeSet
	UpdateOppFactionSet();
}

bool Creature::RemoveEnslave()
{
	return RemoveAura(m_enslaveSpell);
}

void Creature::AddInRangeObject(Object* pObj)
{
	Unit::AddInRangeObject(pObj);
}

void Creature::OnRemoveInRangeObject(Object* pObj)
{
	if(IsTotem() && SummonOwner && SummonOwner == pObj->GetLowGUID())		// player gone out of range of the totem
		event_ModifyTimeLeft(EVENT_TOTEM_EXPIRE, 1);

	if(m_escorter == pObj)
	{
		// we lost our escorter, return to the spawn.
		m_aiInterface->StopMovement(10000);
		DestroyCustomWaypointMap();
		Despawn(1000, 1000);
	}

	Unit::OnRemoveInRangeObject(pObj);
}

void Creature::ClearInRangeSet()
{
	Unit::ClearInRangeSet();
}

void Creature::CalcResistance(uint32 type)
{
	int32 res = (BaseResistance[type] * (100 + BaseResistanceModPct[type])) / 100;
	res += FlatResistanceMod[type];
	res += (res * ResistanceModPct[type]) / 100;
	if(type==0)res+=GetUInt32Value(UNIT_FIELD_STAT1)*2;//fix armor from agi
	SetUInt32Value(UNIT_FIELD_RESISTANCES + type, res > 0 ? res : 0);
}

void Creature::CalcStat(uint32 type)
{
	int32 res = (BaseStats[type]*(100+StatModPct[type]))/100;

	res += FlatStatMod[type];
	if(res < 0)
		res = 0;

	res += ((res*TotalStatModPct[type])/100);
	SetUInt32Value(UNIT_FIELD_STAT0+type, ((res > 0) ? res : 0));
}


void Creature::RegenerateHealth(bool isinterrupted)
{
	if(m_limbostate || !m_canRegenerateHP || isinterrupted)
		return;

	uint32 cur = GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 mh = GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	if(cur >= mh)
		return;

	//though creatures have their stats we use some wierd formula for amt
	float amt = 0.0f;
	uint32 lvl = getLevel();

	amt = lvl*2.0f;
	if (PctRegenModifier)
		amt += (amt * PctRegenModifier) / 100;

	//Apply shit from conf file
	amt *= sWorld.getRate(RATE_HEALTH);

	if(amt <= 1.0f)//this fixes regen like 0.98
		cur++;
	else
		cur += (uint32)amt;
	SetUInt32Value(UNIT_FIELD_HEALTH, (cur >= mh) ? mh : cur);
}

void Creature::RegenerateMana(bool isinterrupted)
{
	if (m_interruptRegen || isinterrupted)
		return;

	uint32 cur=GetUInt32Value(UNIT_FIELD_POWER1);
	uint32 mm=GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	if(cur>=mm)
		return;

	float amt=(getLevel()+10)*PctPowerRegenModifier[POWER_TYPE_MANA]/10;

	//Apply shit from conf file
	amt*=sWorld.getRate(RATE_POWER1);
	if(amt<=1.0)//this fixes regen like 0.98
		cur++;
	else
		cur+=(uint32)amt;
	SetUInt32Value(UNIT_FIELD_POWER1,(cur>=mm)?mm:cur);
}

void Creature::RegenerateFocus()
{
	if (m_interruptRegen)
		return;

	uint32 cur=GetUInt32Value(UNIT_FIELD_POWER3);
	uint32 mm=GetUInt32Value(UNIT_FIELD_MAXPOWER3);
	if(cur>=mm)return;
	float amt = 1.0f * PctPowerRegenModifier[POWER_TYPE_FOCUS];
	cur+=(uint32)amt;
	SetUInt32Value(UNIT_FIELD_POWER3,(cur>=mm)?mm:cur);
}

void Creature::RegenerateEnergy()
{
	if(m_interruptRegen)
		return;

	uint32 cur = GetUInt32Value(UNIT_FIELD_POWER4);
	uint32 mm = GetUInt32Value(UNIT_FIELD_MAXPOWER4);

	if(cur >= mm)
		return;
	
	float amt = 2.0f * PctPowerRegenModifier[POWER_TYPE_ENERGY];
	amt*= sWorld.getRate(RATE_POWER3);
	cur += (uint32)amt;
	SetUInt32Value(UNIT_FIELD_POWER4,(cur >= mm) ? mm : cur);
}

void Creature::CallScriptUpdate()
{
	ASSERT(_myScriptClass);
	if(!IsInWorld())
		return;

	_myScriptClass->AIUpdate();
}

void Creature::AddVendorItem(uint32 itemid, uint32 amount, uint32 ec)
{
	CreatureItem ci;
	ci.amount = amount;
	ci.itemid = itemid;
	ci.available_amount = 0;
	ci.max_amount = 0;
	ci.vendormask = 1;
	ci.incrtime = 0;
	if(ec)
		ci.extended_cost = dbcItemExtendedCost.LookupEntry(ec);
	else
		ci.extended_cost = NULL;

	if(!m_SellItems)
	{
		m_SellItems = new vector<CreatureItem>;
		objmgr.SetVendorList(GetEntry(), m_SellItems);
	}
	m_SellItems->push_back(ci);
}

void Creature::ModAvItemAmount(uint32 itemid, uint32 value)
{
	for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); itr++)
	{
		if(itr->itemid == itemid)
		{
			if(itr->available_amount)
			{
				if(value > itr->available_amount)	// shouldnt happen
				{
					itr->available_amount=0;
					return;
				}
				else
					itr->available_amount -= value;

				if(!event_HasEvent(EVENT_ITEM_UPDATE))
					sEventMgr.AddEvent(TO_CREATURE(this), &Creature::UpdateItemAmount, itr->itemid, EVENT_ITEM_UPDATE, itr->incrtime, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
			return;
		}
	}
}

void Creature::UpdateItemAmount(uint32 itemid)
{
	for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); itr++)
	{
		if(itr->itemid == itemid)
		{
			if (itr->max_amount==0)		// shouldnt happen
				itr->available_amount=0;
			else
			{
				itr->available_amount = itr->max_amount;
			}
			return;
		}
	}
}

void Creature::FormationLinkUp(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldnt happen
		return;

	Creature* creature = NULLCREATURE;
	creature = m_mapMgr->GetSqlIdCreature(SqlId);
	if( creature != NULL )
	{
		m_aiInterface->m_formationLinkTarget = creature;
		haslinkupevent = false;
		event_RemoveEvents(EVENT_CREATURE_FORMATION_LINKUP);
	}
}

void Creature::ChannelLinkUpGO(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldnt happen
		return;

	GameObject* go = m_mapMgr->GetSqlIdGameObject(SqlId);
	if(go != 0)
	{
		event_RemoveEvents(EVENT_CREATURE_CHANNEL_LINKUP);
		SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, go->GetGUID());
		SetUInt32Value(UNIT_CHANNEL_SPELL, m_spawn->channel_spell);
	}
}

void Creature::ChannelLinkUpCreature(uint32 SqlId)
{
	if(!m_mapMgr)		// shouldnt happen
		return;

	Creature* go = m_mapMgr->GetSqlIdCreature(SqlId);
	if(go != 0)
	{
		event_RemoveEvents(EVENT_CREATURE_CHANNEL_LINKUP);
		SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, go->GetGUID());
		SetUInt32Value(UNIT_CHANNEL_SPELL, m_spawn->channel_spell);
	}
}

WayPoint * Creature::CreateWaypointStruct()
{
	return new WayPoint();
}

bool Creature::Load(CreatureSpawn *spawn, uint32 mode, MapInfo *info)
{
	if(m_loadedFromDB)
		return true;

	m_spawn = spawn;
	proto = CreatureProtoStorage.LookupEntry(spawn->entry);
	if(!proto)
		return false;

	creature_info = CreatureNameStorage.LookupEntry(spawn->entry);
	ExtraInfo = CreatureInfoExtraStorage.LookupEntry(proto->Id);
	if(!creature_info)
		return false;

	uint32 health = 0;
	uint32 power = 0;
	float mindmg = 0.0f;
	float maxdmg = 0.0f;
	uint32 level = 0;

	spawnid = spawn->id;
	m_walkSpeed = m_base_walkSpeed = proto->walk_speed; //set speeds
	m_runSpeed = m_base_runSpeed = proto->run_speed; //set speeds
	m_flySpeed = proto->fly_speed;
	m_phaseMask = spawn->phase;
	VendorMask = spawn->vendormask;

	original_emotestate = spawn->emote_state;
	original_MountedDisplayID = spawn->MountedDisplayID;

	//Set fields
	SetUInt32Value(OBJECT_FIELD_ENTRY,proto->Id);

	// Heroic stats
	if(mode)
	{
		LoadedProto = proto->ModeProto[mode];
		if(LoadedProto != NULL)
		{
			health = LoadedProto->Minhealth + RandomUInt(LoadedProto->Maxhealth - LoadedProto->Minhealth);
			power = LoadedProto->Power;
			mindmg = LoadedProto->Mindmg;
			maxdmg = LoadedProto->Maxdmg;
			level = LoadedProto->Minlevel + RandomUInt(LoadedProto->Maxlevel - LoadedProto->Minlevel);
			for(uint32 i = 0; i < 7; i++)
				SetUInt32Value(UNIT_FIELD_RESISTANCES + i, LoadedProto->Resistances[i]);
		}
		else
		{
			for(uint32 i = 0; i < 7; i++)
				SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);

			float calcu = ((mode*2)/10);

			switch(mode) // TODO: find calculations
			{
			case 1: // 5 man heroic or 25 man.
				{
					health = long2int32(double(proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth)) * 1.5);
					mindmg = proto->MinDamage * 1.2f;
					maxdmg = proto->MaxDamage * 1.2f;
					level = proto->MinLevel + RandomUInt(proto->MaxLevel - proto->MinLevel) + RandomUInt(10);
					if(proto->Power)
						power = proto->Power * 1.2;
				}break;

			default: // 10H or 25H
				{
					uint64 newhealth = long2int32(double(proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth)) * (1.5f + calcu));
					if(newhealth > 4294967295) // This is the maximum uint32, its pretty much only for LK, and maybe some later cata content.
						newhealth = 4294967295;

					health = (uint32)newhealth;
					mindmg = proto->MinDamage * (1.2f+calcu);
					maxdmg = proto->MaxDamage * (1.2f+calcu);
					level = proto->MinLevel + RandomUInt(proto->MaxLevel - proto->MinLevel) + RandomUInt(10);
					if(proto->Power)
						power = proto->Power * (1.1f+calcu);
				}break;
			}
		}
	}
	else
	{
		health = proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth);
		power = proto->Power;
		mindmg = proto->MinDamage;
		maxdmg = proto->MaxDamage;
		level = proto->MinLevel + (RandomUInt(proto->MaxLevel - proto->MinLevel));
		for(uint32 i = 0; i < 7; i++)
			SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);
	}

	SetUInt32Value(UNIT_FIELD_HEALTH, health);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, health);

	SetUInt32Value(UNIT_FIELD_BYTES_0, spawn->bytes);
	SetUInt32Value(UNIT_FIELD_BYTES_1, spawn->bytes1);
	SetUInt32Value(UNIT_FIELD_BYTES_2, spawn->bytes2);

	//Use proto displayid (random + gender generator), unless there is an id  specified in spawn->displayid
	uint32 model = spawn->displayid;
	if(!model)
	{
		uint32 gender = creature_info->GenerateModelId(&model);
		SetByte(UNIT_FIELD_BYTES_0,2,gender);
		spawn->displayid = model;
	}

	SetUInt32Value(UNIT_FIELD_DISPLAYID,spawn->displayid);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID,spawn->displayid);
	float dbcscale = GetScale( dbcCreatureDisplayInfo.LookupEntry( model ));
	float realscale = (m_spawn->scale ? m_spawn->scale : dbcscale);
	SetFloatValue(OBJECT_FIELD_SCALE_X, realscale);
	DEBUG_LOG("Creatures","NPC %u (model %u) got scale %f, found in DBC %f", proto->Id, model, realscale, dbcscale);

	SetUInt32Value(UNIT_NPC_EMOTESTATE, original_emotestate);
	SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID,original_MountedDisplayID);
	SetUInt32Value(UNIT_FIELD_LEVEL, level);
	SetUInt32Value(UNIT_FIELD_BASEATTACKTIME,proto->AttackTime);
	SetFloatValue(UNIT_FIELD_MINDAMAGE, mindmg);
	SetFloatValue(UNIT_FIELD_MAXDAMAGE, maxdmg);
	SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,proto->RangedAttackTime);
	SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,proto->RangedMinDamage);
	SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,proto->RangedMaxDamage);

	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, spawn->ItemSlot1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, spawn->ItemSlot2);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+2, spawn->ItemSlot3);

	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, spawn->factionid);
	SetUInt32Value(UNIT_FIELD_FLAGS, spawn->flags);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, proto->BoundingRadius);
	SetFloatValue(UNIT_FIELD_COMBATREACH, proto->CombatReach);

	// set position
	SetPosition( spawn->x, spawn->y, spawn->z, spawn->o, true);
	m_spawnLocation.ChangeCoords(spawn->x, spawn->y, spawn->z, spawn->o);
	m_aiInterface->setMoveType(spawn->movetype);
	m_aiInterface->m_waypoints = objmgr.GetWayPointMap(spawn->id);

	//use proto faction if spawn faction is unspecified
	m_faction = dbcFactionTemplate.LookupEntry(spawn->factionid ? spawn->factionid : proto->Faction);

	if(m_faction)
	{
		// not a neutral creature
		m_factionDBC = dbcFaction.LookupEntry(m_faction->Faction);
		if(!(m_factionDBC->RepListId == -1 && m_faction->HostileMask == 0 && m_faction->FriendlyMask == 0))
			GetAIInterface()->m_canCallForHelp = true;
	}
	else
		Log.Warning("Creature","Creature is missing a valid faction template for entry %u.", spawn->entry);


//SETUP NPC FLAGS
	SetUInt32Value(UNIT_NPC_FLAGS,proto->NPCFLags);

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR ) )
		m_SellItems = objmgr.GetVendorList(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER ) )
		_LoadQuests();

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TAXIVENDOR) )
		m_TaxiNode = sTaxiMgr.GetNearestTaxiNode( m_position.x, m_position.y, m_position.z, GetMapId() );

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER) || HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER_PROF))
		mTrainer = objmgr.GetTrainer(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER ) )
		auctionHouse = sAuctionMgr.GetAuctionHouse(GetEntry());

//NPC FLAGS
	 m_aiInterface->m_waypoints=objmgr.GetWayPointMap(spawn->id);

	//load resistances
	for(uint32 x=0;x<7;x++)
		BaseResistance[x]=GetUInt32Value(UNIT_FIELD_RESISTANCES+x);
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);

	BaseDamage[0]=GetFloatValue(UNIT_FIELD_MINDAMAGE);
	BaseDamage[1]=GetFloatValue(UNIT_FIELD_MAXDAMAGE);
	BaseOffhandDamage[0]=GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE);
	BaseOffhandDamage[1]=GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE);
	BaseRangedDamage[0]=GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE);
	BaseRangedDamage[1]=GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE);
	BaseAttackType=proto->AttackType;

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);   // better set this one

////////////AI

	// kek
	for(list<AI_Spell*>::iterator itr = proto->spells[mode].begin(); itr != proto->spells[mode].end(); itr++)
		m_aiInterface->addSpellToList(*itr);

	if(ExtraInfo != NULL)
	{
		m_aiInterface->m_canRangedAttack = ExtraInfo->m_canRangedAttack;
		m_aiInterface->m_canCallForHelp = ExtraInfo->m_canCallForHelp;
		m_aiInterface->m_CallForHelpHealth = ExtraInfo->m_callForHelpHP;
		m_aiInterface->m_canFlee = ExtraInfo->m_canFlee;
		m_aiInterface->m_FleeHealth = ExtraInfo->m_fleeHealth;
		m_aiInterface->m_FleeDuration = ExtraInfo->m_fleeDuration;
		m_aiInterface->sendflee_message = ExtraInfo->sendflee_message;
		m_aiInterface->flee_message = ExtraInfo->flee_message;
	}
	else
	{
		m_aiInterface->m_canRangedAttack = false;
		m_aiInterface->m_canCallForHelp = false;
		m_aiInterface->m_CallForHelpHealth = 0.0f;
		m_aiInterface->m_canFlee = false;
		m_aiInterface->m_FleeHealth = 0.0f;
		m_aiInterface->m_FleeDuration = 0;
		m_aiInterface->sendflee_message = false;
		m_aiInterface->flee_message = "";
	}

	//these fields are always 0 in db
	GetAIInterface()->setMoveType(0);
	GetAIInterface()->setMoveRunFlag(0);

	// load formation data
	if( spawn->form != NULL )
	{
		m_aiInterface->m_formationLinkSqlId = spawn->form->fol;
		m_aiInterface->m_formationFollowDistance = spawn->form->dist;
		m_aiInterface->m_formationFollowAngle = spawn->form->ang;
	}
	else
	{
		m_aiInterface->m_formationLinkSqlId = 0;
		m_aiInterface->m_formationFollowDistance = 0;
		m_aiInterface->m_formationFollowAngle = 0;
	}

//////////////AI

	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);


// PLACE FOR DIRTY FIX BASTARDS
	// HACK! set call for help on civ health @ 100%
	if(creature_info->Civilian >= 1)
		m_aiInterface->m_CallForHelpHealth = 100;

	//HACK!
	if(m_uint32Values[UNIT_FIELD_DISPLAYID] == 17743 || m_uint32Values[UNIT_FIELD_DISPLAYID] == 20242 || 
		m_uint32Values[UNIT_FIELD_DISPLAYID] == 15435 || creature_info->Family == UNIT_TYPE_MISC)
		m_useAI = false;

	switch(proto->Powertype)
	{
	case POWER_TYPE_MANA:
		{
			SetPowerType(POWER_TYPE_MANA);
			SetUInt32Value(UNIT_FIELD_POWER1, power);
			SetMaxPower(POWER_TYPE_MANA,power);
			SetUInt32Value(UNIT_FIELD_BASE_MANA, power);
		}break;
	case POWER_TYPE_RAGE:
		{
			SetPowerType(POWER_TYPE_RAGE);
			SetPower(POWER_TYPE_RAGE, power*10);
			SetMaxPower(POWER_TYPE_RAGE,power*10);
		}break;
	case POWER_TYPE_FOCUS:
		{
			SetPowerType(POWER_TYPE_FOCUS);
			SetPower(POWER_TYPE_FOCUS, power);
			SetMaxPower(POWER_TYPE_FOCUS,power);
		}break;
	case POWER_TYPE_ENERGY:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetPower(POWER_TYPE_ENERGY, power);
			SetMaxPower(POWER_TYPE_ENERGY,power);
		}break;
	case POWER_TYPE_RUNE:
		{
			SetPowerType(POWER_TYPE_RUNE);
			SetPower(POWER_TYPE_RUNE, power*10);
			SetMaxPower(POWER_TYPE_RUNE,power*10);
		}break;
	case POWER_TYPE_RUNIC:
		{
			SetPowerType(POWER_TYPE_RUNIC);
			SetPower(POWER_TYPE_RUNIC, power*10);
			SetMaxPower(POWER_TYPE_RUNIC,power*10);
		}break;
		//Special vehicle power type cases.
	case POWER_TYPE_STEAM:
	case POWER_TYPE_HEAT:
	case POWER_TYPE_OOZ:
	case POWER_TYPE_BLOOD:
	case POWER_TYPE_WRATH:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetPower(POWER_TYPE_ENERGY, 100);
			SetMaxPower(POWER_TYPE_ENERGY,100);
		}break;
	case POWER_TYPE_PYRITE:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetMaxPower(POWER_TYPE_ENERGY,50);
			m_interruptRegen = true;
		}break;
	default:
		{
			sLog.outError("Creature %u has an unhandled powertype.", GetEntry());
		}break;
	}
	if(IsVehicle())
		TO_VEHICLE(this)->ChangePowerType();

	has_combat_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_ENTER_COMBAT);
	has_waypoint_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);
	m_aiInterface->m_isGuard = isGuard(GetEntry());

	m_aiInterface->getMoveFlags();
	//CanMove (overrules AI)
	if(!GetCanMove())
		Root();

	/* creature death state */
	if(proto->death_state == 1)
	{
		uint32 newhealth = m_uint32Values[UNIT_FIELD_HEALTH] / 100;
		if(!newhealth)
			newhealth = 1;
		SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		m_limbostate = true;
		bInvincible = true;
		SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
	}
	m_invisFlag = proto->invisibility_type;
	if( spawn->stand_state )
		SetStandState( (uint8)spawn->stand_state );

	uint32 tmpitemid;
	if(tmpitemid = m_spawn->ItemSlot1)
	{
		ItemEntry* DBCItem = dbcItem.LookupEntry(tmpitemid);
		if(DBCItem)
		{
			if(DBCItem->InventoryType == INVTYPE_SHIELD)
			{
				b_has_shield = true;
				IP_shield = ItemPrototypeStorage.LookupEntry(tmpitemid);
			}
			else if(getLevel() > 10)
				setcanperry(true); // Who cares what else it is, set parry = true.
		}
	}

	if(!b_has_shield && (tmpitemid = m_spawn->ItemSlot2))
	{
		ItemEntry* DBCItem = dbcItem.LookupEntry(tmpitemid);
		if(DBCItem)
		{
			if(DBCItem->InventoryType == INVTYPE_SHIELD)
			{
				b_has_shield = true;
				IP_shield = ItemPrototypeStorage.LookupEntry(tmpitemid);
			}
			else if(getLevel() > 10)
			{
				if(DBCItem->InventoryType == INVTYPE_WEAPONOFFHAND
					|| DBCItem->InventoryType == INVTYPE_WEAPON)
					setcanperry(true); // We can hold non weapons here, so we need to be careful and do checks.
			}
		}
	}

	m_loadedFromDB = true;
	return true;
}


void Creature::Load(CreatureProto * proto_, uint32 mode, float x, float y, float z, float o)
{
	proto = proto_;

	creature_info = CreatureNameStorage.LookupEntry(proto->Id);
	ExtraInfo = CreatureInfoExtraStorage.LookupEntry(proto->Id);
	if(!creature_info)
		return;

	m_walkSpeed = m_base_walkSpeed = proto->walk_speed; //set speeds
	m_runSpeed = m_base_runSpeed = proto->run_speed; //set speeds
	VendorMask = 1;

	//Set fields
	SetUInt32Value(OBJECT_FIELD_ENTRY,proto->Id);

	uint32 health = 0;
	uint32 power = 0;
	float mindmg = 0.0f;
	float maxdmg = 0.0f;
	uint32 level = 0;

	// Heroic stats
	if(mode)
	{
		LoadedProto = proto->ModeProto[mode];
		if(LoadedProto != NULL)
		{
			health = LoadedProto->Minhealth + RandomUInt(LoadedProto->Maxhealth - LoadedProto->Minhealth);
			power = LoadedProto->Power;
			mindmg = LoadedProto->Mindmg;
			maxdmg = LoadedProto->Maxdmg;
			level =  LoadedProto->Minlevel + (RandomUInt(LoadedProto->Maxlevel - LoadedProto->Minlevel));
			for(uint32 i = 0; i < 7; i++)
				SetUInt32Value(UNIT_FIELD_RESISTANCES + i, LoadedProto->Resistances[i]);
		}
		else
		{
			for(uint32 i = 0; i < 7; i++)
				SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);

			float calcu = ((mode*2)/10);

			switch(mode) // TODO: find calculations
			{
			case 1: // 5 man heroic or 25 man.
				{
					health = long2int32(double(proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth)) * 1.5);
					mindmg = proto->MinDamage * 1.2f;
					maxdmg = proto->MaxDamage * 1.2f;
					level = proto->MinLevel + RandomUInt(proto->MaxLevel - proto->MinLevel) + RandomUInt(10);
					if(proto->Power)
						power = proto->Power * 1.2;
				}break;

			default: // 10H or 25H
				{
					uint64 newhealth = long2int32(double(proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth)) * (1.5f + calcu));
					if(newhealth > 4294967295) // This is the maximum uint32, its pretty much only for LK, and maybe some later cata content.
						newhealth = 4294967295;

					health = (uint32)newhealth;
					mindmg = proto->MinDamage * (1.2f+calcu);
					maxdmg = proto->MaxDamage * (1.2f+calcu);
					level = proto->MinLevel + RandomUInt(proto->MaxLevel - proto->MinLevel) + RandomUInt(10);
					if(proto->Power)
						power = proto->Power * (1.1f+calcu);
				}break;
			}
		}
	}
	else
	{
		health = proto->MinHealth + RandomUInt(proto->MaxHealth - proto->MinHealth);
		power = proto->Power;
		mindmg = proto->MinDamage;
		maxdmg = proto->MaxDamage;
		level = proto->MinLevel + (RandomUInt(proto->MaxLevel - proto->MinLevel));
		for(uint32 i = 0; i < 7; i++)
			SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);
	}

	SetUInt32Value(UNIT_FIELD_HEALTH, health);
	SetUInt32Value(UNIT_FIELD_MAXHEALTH, health);
	SetUInt32Value(UNIT_FIELD_BASE_HEALTH, health);

	uint32 model = 0;
	uint32 gender = creature_info->GenerateModelId(&model);
	setGender(gender);

	float dbcscale = GetScale( dbcCreatureDisplayInfo.LookupEntry( model ));
	float realscale = (proto->Scale ? proto->Scale : dbcscale);
	SetFloatValue(OBJECT_FIELD_SCALE_X, realscale);
	DEBUG_LOG("Creature","NPC %u (model %u) got scale %f, found in DBC %f", proto->Id, model, realscale, dbcscale);

	SetUInt32Value(UNIT_FIELD_DISPLAYID,model);
	SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID,model);

	SetUInt32Value(UNIT_FIELD_LEVEL, level);

	for(uint32 i = 0; i < 7; i++)
		SetUInt32Value(UNIT_FIELD_RESISTANCES+i,proto->Resistances[i]);

	SetUInt32Value(UNIT_FIELD_BASEATTACKTIME,proto->AttackTime);
	SetFloatValue(UNIT_FIELD_MINDAMAGE, mindmg);
	SetFloatValue(UNIT_FIELD_MAXDAMAGE, maxdmg);

	SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME,proto->RangedAttackTime);
	SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE,proto->RangedMinDamage);
	SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE,proto->RangedMaxDamage);

	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, proto->Item1);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1, proto->Item2);
	SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+2, proto->Item3);

	SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, proto->Faction);
	SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, proto->BoundingRadius);
	SetFloatValue(UNIT_FIELD_COMBATREACH, proto->CombatReach);
	original_emotestate = 0;

	// set position
	SetPosition( x, y, z, o, true);
	m_spawnLocation.ChangeCoords(x, y, z, o);
	m_faction = dbcFactionTemplate.LookupEntry(proto->Faction);

	if(m_faction)
	{
		m_factionDBC = dbcFaction.LookupEntry(m_faction->Faction);
		// not a neutral creature
		if(!(m_factionDBC->RepListId == -1 && m_faction->HostileMask == 0 && m_faction->FriendlyMask == 0))
		{
			GetAIInterface()->m_canCallForHelp = true;
		}
	}

	//SETUP NPC FLAGS
	SetUInt32Value(UNIT_NPC_FLAGS,proto->NPCFLags);

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR ) )
		m_SellItems = objmgr.GetVendorList(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER ) )
		_LoadQuests();

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TAXIVENDOR) )
		m_TaxiNode = sTaxiMgr.GetNearestTaxiNode( m_position.x, m_position.y, m_position.z, GetMapId() );

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER) || HasFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_TRAINER_PROF))
		mTrainer = objmgr.GetTrainer(GetEntry());

	if ( HasFlag( UNIT_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER ) )
		auctionHouse = sAuctionMgr.GetAuctionHouse(GetEntry());

	//load resistances
	for(uint32 x=0;x<7;x++)
		BaseResistance[x]=GetUInt32Value(UNIT_FIELD_RESISTANCES+x);
	for(uint32 x=0;x<5;x++)
		BaseStats[x]=GetUInt32Value(UNIT_FIELD_STAT0+x);

	BaseDamage[0]=GetFloatValue(UNIT_FIELD_MINDAMAGE);
	BaseDamage[1]=GetFloatValue(UNIT_FIELD_MAXDAMAGE);
	BaseOffhandDamage[0]=GetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE);
	BaseOffhandDamage[1]=GetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE);
	BaseRangedDamage[0]=GetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE);
	BaseRangedDamage[1]=GetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE);
	BaseAttackType=proto->AttackType;

	SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);   // better set this one

	////////////AI

	// kek
	for(list<AI_Spell*>::iterator itr = proto->spells[mode].begin(); itr != proto->spells[mode].end(); itr++)
	{
		m_aiInterface->addSpellToList(*itr);
	}

	if(ExtraInfo != NULL)
	{
		m_aiInterface->m_canRangedAttack = ExtraInfo->m_canRangedAttack;
		m_aiInterface->m_canCallForHelp = ExtraInfo->m_canCallForHelp;
		m_aiInterface->m_CallForHelpHealth = ExtraInfo->m_callForHelpHP;
		m_aiInterface->m_canFlee = ExtraInfo->m_canFlee;
		m_aiInterface->m_FleeHealth = ExtraInfo->m_fleeHealth;
		m_aiInterface->m_FleeDuration = ExtraInfo->m_fleeDuration;
	}
	else
	{
		m_aiInterface->m_canRangedAttack = false;
		m_aiInterface->m_canCallForHelp = false;
		m_aiInterface->m_CallForHelpHealth = 0.0f;
		m_aiInterface->m_canFlee = false;
		m_aiInterface->m_FleeHealth = 0.0f;
		m_aiInterface->m_FleeDuration = 0;
	}

	//these fields are always 0 in db
	GetAIInterface()->setMoveType(0);
	GetAIInterface()->setMoveRunFlag(0);

	// load formation data
	m_aiInterface->m_formationLinkSqlId = 0;
	m_aiInterface->m_formationFollowDistance = 0;
	m_aiInterface->m_formationFollowAngle = 0;

	//////////////AI

	myFamily = dbcCreatureFamily.LookupEntry(creature_info->Family);


	// PLACE FOR DIRTY FIX BASTARDS
	// HACK! set call for help on civ health @ 100%
	if(creature_info->Civilian >= 1)
		m_aiInterface->m_CallForHelpHealth = 100;

	//HACK!
	if(m_uint32Values[UNIT_FIELD_DISPLAYID] == 17743 || m_uint32Values[UNIT_FIELD_DISPLAYID] == 20242 ||
		m_uint32Values[UNIT_FIELD_DISPLAYID] == 15435 || creature_info->Family == UNIT_TYPE_MISC)
		m_useAI = false;

	switch(proto->Powertype)
	{
	case POWER_TYPE_MANA:
		{
			SetPowerType(POWER_TYPE_MANA);
			SetPower(POWER_TYPE_MANA,power);
			SetMaxPower(POWER_TYPE_MANA,power);
			SetUInt32Value(UNIT_FIELD_BASE_MANA, power);
		}break;
	case POWER_TYPE_RAGE:
		{
			SetPowerType(POWER_TYPE_RAGE);
			SetPower(POWER_TYPE_RAGE,power*10);
			SetMaxPower(POWER_TYPE_RAGE,power*10);
		}break;
	case POWER_TYPE_FOCUS:
		{
			SetPowerType(POWER_TYPE_FOCUS);
			SetPower(POWER_TYPE_FOCUS,power);
			SetMaxPower(POWER_TYPE_FOCUS,power);
		}break;
	case POWER_TYPE_ENERGY:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetPower(POWER_TYPE_ENERGY,power);
			SetMaxPower(POWER_TYPE_ENERGY,power);
		}break;
	case POWER_TYPE_RUNE:
		{
			SetPowerType(POWER_TYPE_RUNE);
			SetPower(POWER_TYPE_RUNE,power*10);
			SetMaxPower(POWER_TYPE_RUNE,power*10);
		}break;
	case POWER_TYPE_RUNIC:
		{
			SetPowerType(POWER_TYPE_RUNIC);
			SetPower(POWER_TYPE_RUNIC,power*10);
			SetMaxPower(POWER_TYPE_RUNIC,power*10);
		}break;
		//Special vehicle power type cases.
	case POWER_TYPE_STEAM:
	case POWER_TYPE_HEAT:
	case POWER_TYPE_OOZ:
	case POWER_TYPE_BLOOD:
	case POWER_TYPE_WRATH:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetPower(POWER_TYPE_ENERGY,100);
			SetMaxPower(POWER_TYPE_ENERGY,100);
		}break;
	case POWER_TYPE_PYRITE:
		{
			SetPowerType(POWER_TYPE_ENERGY);
			SetMaxPower(POWER_TYPE_ENERGY,50);
			m_interruptRegen = true;
		}break;
	default:
		{
			sLog.outError("Creature %u has an unhandled powertype.", GetEntry());
		}break;
	}
	if(IsVehicle())
		TO_VEHICLE(this)->ChangePowerType();

	has_combat_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_ENTER_COMBAT);
	has_waypoint_text = objmgr.HasMonsterSay(GetEntry(), MONSTER_SAY_EVENT_RANDOM_WAYPOINT);
	m_aiInterface->m_isGuard = isGuard(GetEntry());

	m_aiInterface->getMoveFlags();
	//CanMove (overrules AI)
	if(!GetCanMove())
		Root();

	/* creature death state */
	if(proto->death_state == 1)
	{
		uint32 newhealth = m_uint32Values[UNIT_FIELD_HEALTH] / 100;
		if(!newhealth)
			newhealth = 1;
		SetUInt32Value(UNIT_FIELD_HEALTH, 1);
		m_limbostate = true;
		bInvincible = true;
		SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DEAD);
	}
	m_invisFlag = proto->invisibility_type;

	uint32 tmpitemid;
	if(tmpitemid = proto->Item1)
	{
		ItemEntry* DBCItem = dbcItem.LookupEntry(tmpitemid);
		if(DBCItem)
		{
			if(DBCItem->InventoryType == INVTYPE_SHIELD)
			{
				b_has_shield = true;
				IP_shield = ItemPrototypeStorage.LookupEntry(tmpitemid);
			}
			else if(getLevel() > 10)
				setcanperry(true); // Who cares what else it is, set parry = true.
		}
	}

	if(!b_has_shield && (tmpitemid = proto->Item2))
	{
		ItemEntry* DBCItem = dbcItem.LookupEntry(tmpitemid);
		if(DBCItem)
		{
			if(DBCItem->InventoryType == INVTYPE_SHIELD)
			{
				b_has_shield = true;
				IP_shield = ItemPrototypeStorage.LookupEntry(tmpitemid);
			}
			else if(getLevel() > 10)
			{
				if(DBCItem->InventoryType == INVTYPE_WEAPONOFFHAND
					|| DBCItem->InventoryType == INVTYPE_WEAPON)
					setcanperry(true); // We can hold non weapons here, so we need to be careful and do checks.
			}
		}
	}
}

void Creature::OnPushToWorld()
{
	if(proto)
	{
		uint8 mode = GetMapMgr()->iInstanceMode;
		if(mode && (proto->ModeProto[mode] != NULL))
		{
			set<uint32>::iterator itr = proto->ModeProto[mode]->start_auras.begin();
			SpellEntry * sp = NULL;
			for(; itr != proto->ModeProto[mode]->start_auras.end(); itr++)
			{
				sp = dbcSpell.LookupEntry((*itr));
				if(sp != NULL)
				{
					Unit* target = TO_UNIT(this);
					sEventMgr.AddEvent(target, &Unit::EventCastSpell, target, sp, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
		}
		else
		{
			set<uint32>::iterator itr = proto->start_auras.begin();
			SpellEntry * sp = NULL;
			for(; itr != proto->start_auras.end(); itr++)
			{
				sp = dbcSpell.LookupEntry((*itr));
				if(sp != NULL)
				{
					Unit* target = TO_UNIT(this);
					sEventMgr.AddEvent(target, &Unit::EventCastSpell, target, sp, EVENT_AURA_APPLY, 250, 1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
				}
			}
		}
	}

	LoadScript();
	Unit::OnPushToWorld();

	if(_myScriptClass)
		_myScriptClass->OnLoad();

	if(m_spawn)
	{
		if(m_aiInterface->m_formationLinkSqlId)
		{
			// add event
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::FormationLinkUp, m_aiInterface->m_formationLinkSqlId,
				EVENT_CREATURE_FORMATION_LINKUP, 1000, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			haslinkupevent = true;
		}

		if(m_spawn->channel_target_creature)
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::ChannelLinkUpCreature, m_spawn->channel_target_creature, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0);	// only 5 attempts

		if(m_spawn->channel_target_go)
			sEventMgr.AddEvent(TO_CREATURE(this), &Creature::ChannelLinkUpGO, m_spawn->channel_target_go, EVENT_CREATURE_CHANNEL_LINKUP, 1000, 5, 0);	// only 5 attempts
	}

	m_aiInterface->m_is_in_instance = (m_mapMgr->GetMapInfo()->type!=INSTANCE_NULL) ? true : false;
	if (HasItems())
	{
		for(std::vector<CreatureItem>::iterator itr = m_SellItems->begin(); itr != m_SellItems->end(); itr++)
		{
				if (itr->max_amount == 0)
					itr->available_amount=0;
				else if (itr->available_amount<itr->max_amount)
					sEventMgr.AddEvent(TO_CREATURE(this), &Creature::UpdateItemAmount, itr->itemid, EVENT_ITEM_UPDATE, VENDOR_ITEMS_UPDATE_TIME, 1,0);
		}

	}

	CALL_INSTANCE_SCRIPT_EVENT( m_mapMgr, OnCreaturePushToWorld )( TO_CREATURE(this) );
}

void Creature::Despawn(uint32 delay, uint32 respawntime)
{
	if(delay)
	{
		sEventMgr.AddEvent(TO_CREATURE(this), &Creature::Despawn, (uint32)0, respawntime, EVENT_CREATURE_RESPAWN, delay, 1,0);
		return;
	}

	m_loot.items.clear();

	if(!IsInWorld())
		return;

	//Better make sure all summoned totems/summons/GO's created by this creature spawned removed.
	SummonExpireAll(true);

	if(respawntime)
	{
		/* get the cell with our SPAWN location. if we've moved cell this might break :P */
		MapCell * pCell = m_mapMgr->GetCellByCoords(m_spawnLocation.x, m_spawnLocation.y);
		if(!pCell)
			pCell = m_mapCell;

		ASSERT(pCell);
		pCell->_respawnObjects.insert(TO_OBJECT(this));
		sEventMgr.RemoveEvents(this);
		sEventMgr.AddEvent(m_mapMgr, &MapMgr::EventRespawnCreature, TO_CREATURE(this), pCell, EVENT_CREATURE_RESPAWN, respawntime, 1, 0);
		Unit::RemoveFromWorld(false);
		SetPosition( m_spawnLocation, true);
		m_respawnCell = pCell;
	}
	else
		Unit::RemoveFromWorld(true);
}

void Creature::TriggerScriptEvent(int ref)
{
	if( _myScriptClass )
		_myScriptClass->StringFunctionCall(ref);
}

void Creature::DestroyCustomWaypointMap()
{
	if(m_custom_waypoint_map)
	{
		for(WayPointMap::iterator itr = m_custom_waypoint_map->begin(); itr != m_custom_waypoint_map->end(); itr++)
		{
			delete (*itr);
		}
		delete m_custom_waypoint_map;
		m_custom_waypoint_map = 0;
		m_aiInterface->SetWaypointMap(0);
	}
}

void Creature::RemoveLimboState(Unit* healer)
{
	if(!m_limbostate)
		return;

	m_limbostate = false;
	SetUInt32Value(UNIT_NPC_EMOTESTATE, m_spawn ? m_spawn->emote_state : 0);
	SetUInt32Value(UNIT_FIELD_HEALTH, GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	bInvincible = false;
}

// Generates 3 random waypoints around the NPC
void Creature::SetGuardWaypoints()
{
	if(!GetMapMgr())
		return;

	if(!GetCreatureInfo())
		return;

	GetAIInterface()->setMoveType(1);
	for(int i = 1; i <= 4; i++)
	{
		float ang = rand()/100.0f;
		float ran = (rand()%(100))/10.0f;
		while(ran < 1)
			ran = (rand()%(100))/10.0f;

		WayPoint * wp = NULL;
		wp = new WayPoint;
		wp->id = i;
		wp->flags = 0;
		wp->waittime = 800;  /* these guards are antsy :P */
		wp->x = GetSpawnX()+ran*sin(ang);
		wp->y = GetSpawnY()+ran*cos(ang);

		if(canFly())
			wp->z = GetSpawnZ();
		else
			wp->z = GetCHeightForPosition(canSwim() ? false : true); // Check water heights

		wp->orientation = GetSpawnO();
		wp->backwardemoteid = 0;
		wp->backwardemoteoneshot = 0;
		wp->forwardemoteid = 0;
		wp->forwardemoteoneshot = 0;
		wp->backwardskinid = m_uint32Values[UNIT_FIELD_NATIVEDISPLAYID];
		wp->forwardskinid = m_uint32Values[UNIT_FIELD_NATIVEDISPLAYID];
		wp->forwardStandState = 0;
		wp->backwardStandState = 0;
		wp->forwardSpellToCast = 0;
		wp->backwardSpellToCast = 0;
		wp->forwardSayText = "";
		wp->backwardSayText = "";
		GetAIInterface()->addWayPoint(wp);
	}
}

Unit* Creature::GetSummonOwner()
{
	//we keep a pointer to the totem because owner might have gone feeefeee
	if( SummonOwner && GetMapMgr() )
	{
		Unit * u = NULL;
		u = GetMapMgr()->GetUnit(SummonOwner);
		return u != NULL ? u : NULLUNIT;
	}
	return NULLUNIT;
}

