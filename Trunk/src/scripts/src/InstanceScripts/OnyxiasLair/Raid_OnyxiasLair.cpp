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
#include "Raid_OnyxiasLair.h"

/*
Author: SlavaOrlov
http://www.sandshroud.org

TODO:
- DeepBreath.
- eggs go 176511
*/


void SpellFunc_Deep_Breath(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType);
void SpellFunc_Eruption(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType);
class OnyxiaAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(OnyxiaAI, MoonScriptBossAI);
	OnyxiaAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
    {		
		switch( GetInstanceMode() ){
		case MODE_25PLAYER_NORMAL:
			{
				AddPhaseSpell(1,AddSpell(ONY_FLAME_BREATH25, Target_Current, 15, 2, 20, 0.0f, 45.0f));
				AddPhaseSpell(1,AddSpell(ONY_WING_BUFFET25, Target_Current, 15, 0, 20, 0.0f, 20.0f));
				AddPhaseSpell(1,AddSpell(ONY_TAIL_SWEEP25, Target_Destination, 15, 0, 20, 0.0f, 30.0f));
				AddPhaseSpell(2,AddSpell(ONY_FIREBALL25, Target_RandomPlayerNotCurrent, 15, 3, 20));
				AddPhaseSpell(3,AddSpell(ONY_FLAME_BREATH25, Target_Current, 15, 2, 20, 0.0f, 45.0f));
				AddPhaseSpell(3,AddSpell(ONY_WING_BUFFET25, Target_Current, 15, 0, 20, 0.0f, 20.0f));
				AddPhaseSpell(3,AddSpell(ONY_TAIL_SWEEP25, Target_Destination, 15, 0, 20, 0.0f, 30.0f));
				GetUnit()->SetUInt32Value(UNIT_FIELD_MAXHEALTH,22312000);
			}break;
		case MODE_10PLAYER_NORMAL:
			{
				AddPhaseSpell(1,AddSpell(ONY_FLAME_BREATH, Target_Current, 15, 2, 20, 0.0f, 45.0f));
				AddPhaseSpell(1,AddSpell(ONY_WING_BUFFET, Target_Current, 15, 0, 20, 0.0f, 20.0f));
				AddPhaseSpell(1,AddSpell(ONY_TAIL_SWEEP, Target_Destination, 15, 0, 20, 0.0f, 30.0f));
				AddPhaseSpell(2,AddSpell(ONY_FIREBALL, Target_RandomPlayerNotCurrent, 15, 3, 20));
				AddPhaseSpell(3,AddSpell(ONY_FLAME_BREATH, Target_Current, 15, 2, 20, 0.0f, 45.0f));
				AddPhaseSpell(3,AddSpell(ONY_WING_BUFFET, Target_Current, 15, 0, 20, 0.0f, 20.0f));
				AddPhaseSpell(3,AddSpell(ONY_TAIL_SWEEP, Target_Destination, 15, 0, 20, 0.0f, 30.0f));
				GetUnit()->SetUInt32Value(UNIT_FIELD_MAXHEALTH,4800000);
			}break;
		}
		AddEmote(Event_OnTargetDied, "Learn your place, mortal!", Text_Yell, 0);
		AddPhaseSpell(1,AddSpell(ONY_CLEAVE, Target_Current, 15, 0, 20, 0.0f, 15.0f));
		deepbreath = AddSpell(ONY_BREATH, Target_RandomPlayerNotCurrent, 0, 0, 0);
		AddPhaseSpell(2, AddSpellFunc(&SpellFunc_Deep_Breath, Target_RandomPlayerNotCurrent, 15, 8, 30));
		AddPhaseSpell(3,AddSpell(ONY_CLEAVE, Target_Current, 15, 0, 20, 0.0f, 15.0f));
		bellowingroar = AddPhaseSpell(3,AddSpell(ONY_BELLOWING_ROAR, Target_Current, 15, 0, 10));
		AddPhaseSpell(3,AddSpellFunc(&SpellFunc_Eruption, Target_RandomPlayerNotCurrent, 15, 0, 40));
		GetUnit()->SetStandState(STANDSTATE_SLEEP);
		SetFlyMode(false);
		SetMoveType(Move_DontMoveWP);
		for (int i = 1 ; i < 7 ; i++) AddWaypoint(CreateWaypoint( i, (RandomUInt(20)+40)*1000, coord[i].mAddition, coord[i]));
		mDidHit = false;
    }
    
    void OnCombatStart(Unit *mTarget)
	{
		range = GetUnit()->GetAIInterface()->getOutOfCombatRange();
		GetUnit()->GetAIInterface()->setOutOfCombatRange(200000);
		GetUnit()->SetStandState(STANDSTATE_STAND);
		Emote("How fortuitous, usually I must leave my lair to feed!");
		ParentClass::OnCombatStart(mTarget);
    }

	void OnCombatStop(Unit *mTarget)
	{
		GetUnit()->GetAIInterface()->setOutOfCombatRange(range);
		GetUnit()->SetStandState(STANDSTATE_SLEEP);
		SetMoveType(Move_DontMoveWP);
		SetFlyMode(false);
		ParentClass::OnCombatStop(mTarget);
    }

    void AIUpdate()
	{
		switch(GetPhase())
		{
			case 1:
				{
					if( GetHealthPercent() <= 65 ){
						mWhelpTimer = AddTimer(90000);
						mGuardTimer = AddTimer(30000);
						SetAllowMelee(false);
						Emote("This meaningless exertion bores me. I'll incinerate you all from above!");
						SetFlyMode(true);
						GetUnit()->Emote(EMOTE_ONESHOT_LIFTOFF);
						ForceWaypointMove(1);
						SetMoveType(Move_RandomWP);
						SetPhase(2);
					}
				}break;
			case 2:
				{
					if( GetHealthPercent() <= 40 ){
						RemoveTimer(mWhelpTimer);
						RemoveTimer(mGuardTimer);
						SetAllowMelee(true);
						Emote("It seems you'll need another lesson, mortals!");
						SetFlyMode(false);
						GetUnit()->Emote(EMOTE_ONESHOT_LAND);
						SetPhase(3);
						StopWaypointMovement();
						CastSpell(bellowingroar);
					}
					if(IsTimerFinished(mWhelpTimer)) whelps(40);
					if(IsTimerFinished(mGuardTimer)) guards((GetInstanceMode()==MODE_25PLAYER_NORMAL) ? 2 : 1);
				}break;
			case 3: break; //whelps doesn't spawn at phase 3. Maybe will add something later.
		}
		ParentClass::AIUpdate();
    }

	void whelps(int32 count)
	{
		for(int i = 0; i < count; i++)
        {
			uint32 rnd = RandomUInt(5)+1;
			whelp = SpawnCreature(ONY_WHELP, whelpCoords[rnd].mX, whelpCoords[rnd].mY, whelpCoords[rnd].mZ, whelpCoords[rnd].mO);
			if(whelp)
				whelp->AttackPlayer(GetUnit()->GetAIInterface()->GetNextTarget());
			whelp = SpawnCreature(ONY_WHELP, whelpCoords[rnd-1].mX, whelpCoords[rnd-1].mY, whelpCoords[rnd-1].mZ, whelpCoords[rnd-1].mO);
			if(whelp)
				whelp->AttackPlayer(GetUnit()->GetAIInterface()->GetNextTarget());
        }
		ResetTimer(mWhelpTimer, 90000);
	}

	void guards(int32 count) //only one guard spawns in 10 player mode.
	{ 
		for(int i=0; i<count; i++)
		{
			guard = SpawnCreature(ONY_GUARD, -124.648956f, -214.545670f, -71.680466f, 0.0f);
			if(guard) 
				guard->AttackPlayer(GetUnit()->GetAIInterface()->GetNextTarget());
		}
		ResetTimer(mGuardTimer, 90000);
	}
	bool mDidHit;
	MoonScriptCreatureAI *whelp, *guard;
	int32 mWhelpTimer, mGuardTimer;
	uint32 range;
	SpellDesc *bellowingroar, *deepbreath;
};

void SpellFunc_Deep_Breath(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType)
{
	OnyxiaAI *pOnyxia = (pCreatureAI != NULL) ? (OnyxiaAI*)(pCreatureAI) : NULL;
	if(pOnyxia != NULL)
	{
		Spell *dbspell = new Spell(TO_OBJECT(pOnyxia), dbcSpell.LookupEntry(ONY_BREATH), false, NULLAURA);
		dbspell->SetUnitTarget(pTarget);
		for (unordered_set<Player *>::iterator itr = pOnyxia->GetUnit()->GetInRangePlayerSetBegin(); itr != pOnyxia->GetUnit()->GetInRangePlayerSetEnd(); ++itr) 
		{
			Player *pPlayer = TO_PLAYER(*itr);
			pPlayer->GetSession()->SystemMessage("Lady Onyxia takes in a deep breath...");
		}
		pOnyxia->GetUnit()->CastSpell(dbspell);
		//if(dbspell->_DidHit(dbspell->GetUnitTarget())) //need to somehow return hit state
			//pOnyxia->mDidHit = true;
	}
}
void SpellFunc_Eruption(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType)
{
	if(pCreatureAI != NULL)
	{
		for(unordered_set<Object* >::iterator itr = pCreatureAI->GetUnit()->GetInRangeSetBegin(); itr != pCreatureAI->GetUnit()->GetInRangeSetEnd(); ++itr)
		{
			if((*itr)->GetEntry() == GO_LAVA)
			{
				(*itr)->Activate(pCreatureAI->GetUnit()->GetMapMgr());
				(*itr)->CastSpell(pTarget->GetGUID(), (pCreatureAI->GetInstanceMode() == MODE_10PLAYER_NORMAL) ? ONY_ERUPTION : ONY_ERUPTION25, false);
				(*itr)->Deactivate(pCreatureAI->GetUnit()->GetMapMgr());
			}				
		}
	}
}

void SpellFunc_Ignite_Weapon(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType);
class GuardAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION(GuardAI, MoonScriptBossAI);
	GuardAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
    {	
		AddSpell(GUARD_BLAST_NOVA, Target_Current, 15, 0, 10, 0.0f, 15.0f);
		AddSpell(GUARD_CLEAVE, Target_Current, 15, 0, 10);
	    mIgniteWeapon = AddSpell(GUARD_IGNITE_WEAPON, Target_Self, 0, 0, 0, 0.0f, 0.0f);
		AddSpellFunc(&SpellFunc_Ignite_Weapon, Target_Self, 15, 0, 40, 0.0f, 5.0f);
	}
	SpellDesc *mIgniteWeapon;
};

void SpellFunc_Ignite_Weapon(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType)
{
	GuardAI *pGuard = (pCreatureAI != NULL) ? (GuardAI*)pCreatureAI : NULL;
	if (pGuard != NULL)
	{
		if(!pGuard->GetUnit()->disarmed)
			pGuard->CastSpell(pGuard->mIgniteWeapon);
	}
}



class OnyxiasLair : public MoonInstanceScript
{
public:
	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION( OnyxiasLair, MoonInstanceScript );
	OnyxiasLair( MapMgr *pMapMgr ) : MoonInstanceScript( pMapMgr )
	{
		mWhelpCount = 0;
		pOnyxia = NULLCREATURE;
	};

	void OnCreaturePushToWorld( Creature *pCreature )
	{
		if(pCreature!=NULLCREATURE)
		{
			if(pCreature->GetEntry() == ONY_BOSS)
			{
				mOnyxiaDots = AddTimer(300000);
				pOnyxia=pCreature;
			}
			if(pCreature->GetEntry() == ONY_WHELP)
				mWhelpCount++;
		}
	};

	void OnCreatureDeath( Creature *pVictim, Unit *pKiller )
	{
		if(pKiller != NULLUNIT && pKiller->IsPlayer())
		if(pVictim->GetEntry() == ONY_BOSS)
		{
			(GetInstanceMode() == MODE_10PLAYER_NORMAL) ? HandleLairAchievements(ACHIEV_DEFEAT_N) : HandleLairAchievements(ACHIEV_DEFEAT_H);
			if(!IsTimerFinished(mOnyxiaDots))
			{
				RemoveTimer(mOnyxiaDots);
				(GetInstanceMode() == MODE_10PLAYER_NORMAL) ? HandleLairAchievements(ACHIEV_DOTS_N) : HandleLairAchievements(ACHIEV_DOTS_H);
			}
			if(!((OnyxiaAI*)pOnyxia)->mDidHit)
				(GetInstanceMode() == MODE_10PLAYER_NORMAL) ? HandleLairAchievements(ACHIEV_BREATH_N) : HandleLairAchievements(ACHIEV_BREATH_H);
		}
	};

	void UpdateEvent()
	{
		if(pOnyxia->GetAIInterface()->m_moveFly)
			mOnyxiaFly = AddTimer(10000);
		if(!IsTimerFinished(mOnyxiaFly) && mWhelpCount >=50)
		{
			RemoveTimer(mOnyxiaFly);
			(GetInstanceMode() == MODE_10PLAYER_NORMAL) ? HandleLairAchievements(ACHIEV_WHELPS_N) : HandleLairAchievements(ACHIEV_WHELPS_H);
		}	
	};

	void HandleLairAchievements(uint32 pEntry)
	{ 
		if(pOnyxia == NULL || pOnyxia->GetMapMgr() == NULL)
			return;
		Player *Jerk = NULLPLR;
		for (PlayerStorageMap::iterator itr = pOnyxia->GetMapMgr()->m_PlayerStorage.begin(); itr != pOnyxia->GetMapMgr()->m_PlayerStorage.end(); ++itr) 
		{
			Jerk = itr->second;
			if(Jerk)
				Jerk->GetAchievementInterface()->HandleAchievementCreature(pEntry);
		}
	};

	void Destroy()
	{
		delete this;
	};

private:
	int32 mWhelpCount, mOnyxiaFly, mOnyxiaDots;
	Creature *pOnyxia; //dunno why, but it crashes when used OnyxiaAI*
};

void SetupOnyxiasLair(ScriptMgr * mgr)
{
	mgr->register_instance_script(MAP_LAIR, &OnyxiasLair::Create);
    mgr->register_creature_script(ONY_BOSS, &OnyxiaAI::Create);
	mgr->register_creature_script(ONY_GUARD, &GuardAI::Create);
}