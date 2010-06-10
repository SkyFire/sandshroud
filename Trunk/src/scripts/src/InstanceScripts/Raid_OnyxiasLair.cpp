/*
 * Sun++ Scripts for Aspire MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2010 Sun++ Team <http://www.sunscripting.com/>
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
#include "Setup.h"
#include "../Common/Base.h"

/*
Author: SlavaOrlov
http://www.sandshroud.com
*/


/*
TODO:
- Найти ID голоса Анны, при разговорах.
- Проработать пути следования
- Закончить гвардов

*/

//SlavaOrlov: this is my fuckin 5th rewrite of this fuckin script. tired.

//Phase 1 and 3
#define ONY_FLAME_BREATH 18435
#define ONY_FLAME_BREATH25 68970 //25man
#define ONY_WING_BUFFET 18500
#define ONY_WING_BUFFET25 69293 //25man
#define ONY_TAIL_SWEEP 68867
#define ONY_TAIL_SWEEP25 69286 //25man
#define ONY_ERUPTION 17731
#define ONY_ERUPTION25 69294 //25man
#define ONY_CLEAVE 68868
#define ONY_BELLOWING_ROAR 18431

//Phase 2
#define ONY_FIREBALL 18392
#define ONY_FIREBALL25 68926 //25man
#define ONY_BREATH 17086

//My Love :3
#define ONY_BOSS 10184
//Summons
#define ONY_WHELP 11262
#define ONY_GUARD 36561


static Coords whelpCoords[] =
{
    { -30.812f, -166.395f, -89.000f, 5.160f, Flag_Run },
    { -30.233f, -264.158f, -89.896f, 1.129f, Flag_Run },
    { -35.813f, -169.427f, -90.000f, 5.384f, Flag_Run },
    { -36.104f, -260.961f, -90.600f, 1.111f, Flag_Run },
    { -34.643f, -164.080f, -90.000f, 5.364f, Flag_Run },
    { -35.377f, -267.320f, -91.000f, 1.111f, Flag_Run }
};

static Coords coord[] =
{
    { -75.945f, -219.245f, -83.375f, 0.004947f, Flag_Fly },
    { -72.945f, -219.245f, -80.779f, 0.004947f, Flag_Fly },
    { 42.621f, -217.195f, -66.056f, 3.014011f, Flag_Fly },
    { 12.270f, -254.694f, -67.997f, 2.395585f, Flag_Fly },
    { -79.020f, -252.374f, -68.965f, 0.885179f, Flag_Fly },
    { -80.257f, -174.240f, -69.293f, 5.695741f, Flag_Fly },
    { 27.875f, -178.547f, -66.041f, 3.908957f, Flag_Fly },
    { -4.868f, -217.171f, -86.710f, 3.141590f, Flag_Fly }
};

class OnyxiaAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(OnyxiaAI, MoonScriptBossAI);
	OnyxiaAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
    {		
		GetUnit()->SetStandState(STANDSTATE_SLEEP);
		//FUCKIN AWESOME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(GetUnit()->GetMapMgr()->GetPlayerCount() > 10 ){
			AddPhaseSpell(1,AddSpell(ONY_FLAME_BREATH25, Target_Current, 15, 2, 10, 0.0f, 45.0f));
			AddPhaseSpell(1,AddSpell(ONY_WING_BUFFET25, Target_Current, 15, 0, 10, 0.0f, 20.0f));
			AddPhaseSpell(1,AddSpell(ONY_TAIL_SWEEP25, Target_Destination, 15, 0, 10, 0.0f, 30.0f));
			
			AddPhaseSpell(2,AddSpell(ONY_FIREBALL25, Target_RandomPlayerNotCurrent, 15, 3, 10));

			AddPhaseSpell(3,AddSpell(ONY_FLAME_BREATH25, Target_Current, 15, 2, 10, 0.0f, 45.0f));
			AddPhaseSpell(3,AddSpell(ONY_WING_BUFFET25, Target_Current, 15, 0, 10, 0.0f, 20.0f));
			AddPhaseSpell(3,AddSpell(ONY_TAIL_SWEEP25, Target_Destination, 15, 0, 10, 0.0f, 30.0f));
						
			GetUnit()->SetUInt32Value(UNIT_FIELD_MAXHEALTH,22312000);
		}else{
			AddPhaseSpell(1,AddSpell(ONY_FLAME_BREATH, Target_Current, 15, 2, 10, 0.0f, 45.0f));
			AddPhaseSpell(1,AddSpell(ONY_WING_BUFFET, Target_Current, 15, 0, 10, 0.0f, 20.0f));
			AddPhaseSpell(1,AddSpell(ONY_TAIL_SWEEP, Target_Destination, 15, 0, 10, 0.0f, 30.0f));

			AddPhaseSpell(2,AddSpell(ONY_FIREBALL, Target_RandomPlayerNotCurrent, 15, 3, 10));

			AddPhaseSpell(3,AddSpell(ONY_FLAME_BREATH, Target_Current, 15, 2, 10, 0.0f, 45.0f));
			AddPhaseSpell(3,AddSpell(ONY_WING_BUFFET, Target_Current, 15, 0, 10, 0.0f, 20.0f));
			AddPhaseSpell(3,AddSpell(ONY_TAIL_SWEEP, Target_Destination, 15, 0, 10, 0.0f, 30.0f));
			
			GetUnit()->SetUInt32Value(UNIT_FIELD_MAXHEALTH,4800000);
		}
		AddPhaseSpell(1,AddSpell(ONY_CLEAVE, Target_Current, 15, 0, 10, 0.0f, 15.0f));
		
		AddPhaseSpell(2,AddSpell(ONY_BREATH, Target_RandomPlayerNotCurrent, 15, 8, 10));

		AddPhaseSpell(3,AddSpell(ONY_CLEAVE, Target_Current, 15, 0, 10, 0.0f, 15.0f));
		
		//she casts it when phase 3 starts.
		bellowingroar = AddPhaseSpell(3,AddSpell(ONY_BELLOWING_ROAR, Target_Current, 15, 0, 10));

		//waypoints - like it.
		for (int i = 0 ; i < 6 ; i++) AddWaypoint(CreateWaypoint( i, (RandomUInt(20)+40)*1000, coord[i].mAddition, coord[i]));

    }
    
    void OnCombatStart(Unit *mTarget){
		SetPhase(1);
		StopWaypointMovement();
		mWhelpTimer = AddTimer(90000);
		mGuardTimer = AddTimer(30000);
		Emote("How fortuitous, usually I must leave my lair to feed!");
		GetUnit()->SetStandState(0);
		ParentClass::OnCombatStart(mTarget);
    }

    void AIUpdate(){
		switch(GetPhase()){
		case 1:
			{
				if( GetHealthPercent() <= 65 ){
					SetAllowMelee(false);
					SetFlyMode(true);
					SetWaypointToMove(0);
					SetPhase(2);
				}
			}break;
		case 2:
			{
				if( GetHealthPercent() <= 40 ){
					SetAllowMelee(true);
					SetFlyMode(false);
					SetPhase(3);
					StopWaypointMovement();
					CastSpell(bellowingroar);
				}
				if(IsTimerFinished(mWhelpTimer)) whelps(40);
//				if(IsTimerFinished(mGuardTimer)) guards();
			}break;
		case 3:
			{
				if(IsTimerFinished(mWhelpTimer)) whelps( RandomUInt(20)/2+4 ); //because i love it.
			}break;
		}
		ParentClass::AIUpdate();
    }

	void whelps(uint32 count){
		for(int i = 0; i < count; i++)
        {
			uint32 rnd = RandomUInt(5)+1;
			whelp = SpawnCreature(ONY_WHELP, whelpCoords[rnd].mX, whelpCoords[rnd].mY, whelpCoords[rnd].mZ, whelpCoords[rnd].mO);
			if(whelp)
				//whelp->MoveTo(14.161f, -177.874f, -85.649f, 0.23f);
				whelp->MoveTo(14.161f, -177.874f, -85.649f);
			whelp = SpawnCreature(ONY_WHELP, whelpCoords[rnd-1].mX, whelpCoords[rnd-1].mY, whelpCoords[rnd-1].mZ, whelpCoords[rnd-1].mO);
			if(whelp)
            	//whelp->MoveTo(27.133f, -232.030f, -84.188f, 0.44f);
				whelp->MoveTo(27.133f, -232.030f, -84.188f);
        }
		ResetTimer(mWhelpTimer, 90000);
	}

/*
	void guards(){ 
		//Поставить координаты входа в пещеру.
		for(int i=0; i<2; i++){
			guard = SpawnCreature(ONY_GUARD, x, y, z, o);
			if(guard) guard->MoveTo(14.161f, -177.874f, -85.649f); //, 0.23f
		}
		ResetTimer(mWhelpTimer, 90000);
	}
*/

	void OnCombatStop(Unit *mTarget){
		RemoveTimer(mWhelpTimer);
		RemoveTimer(mGuardTimer);
		GetUnit()->SetStandState(STANDSTATE_SLEEP);
		SetPhase(1);
		ParentClass::OnCombatStop(mTarget);
    }

	void OnDied(Unit *pKiller){
		RemoveTimer(mWhelpTimer);
		RemoveTimer(mGuardTimer);
		SetPhase(1);
		ParentClass::OnDied(pKiller);
	}

protected:
	MoonScriptCreatureAI *whelp, *guard;
	int32 mWhelpTimer, mGuardTimer;
	SpellDesc *bellowingroar;
};

//Guard AI. Fuckin guard ai.
#define GUARD_BLAST_NOVA 68958 //only guard is using this shit.
#define GUARD_CLEAVE 15284
#define GUARD_IGNITE_WEAPON 68960

class GuardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GuardAI, MoonScriptBossAI);
	GuardAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
    {	
		AddSpell(GUARD_BLAST_NOVA, Target_Current, 15, 0, 10, 0.0f, 15.0f);
		AddSpell(GUARD_CLEAVE, Target_Current, 15, 0, 10);
		AddSpell(GUARD_IGNITE_WEAPON, Target_ClosestPlayer, 15, 0, 10, 0.0f, 5.0f); //make it: if guard gets disarmed then do not to cast this shit.
	}
};

void SetupOnyxiasLair(ScriptMgr * mgr)
{
    mgr->register_creature_script(ONY_BOSS, &OnyxiaAI::Create);
	mgr->register_creature_script(ONY_GUARD, &GuardAI::Create);
}