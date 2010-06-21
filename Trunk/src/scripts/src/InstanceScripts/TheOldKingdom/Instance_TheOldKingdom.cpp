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

#include "Instance_TheOldKingdom.h"

NadoxAI::NadoxAI( Creature *pCreature ) : MoonScriptCreatureAI( pCreature )
{
	if(IsHeroic())
		AddSpell(H_SPELL_BROOD_PLAGUE, Target_Current, 15, 0, 30, 0.0f, 5.0f);
	else
		AddSpell(SPELL_BROOD_PLAGUE, Target_Current, 15, 0, 60, 0.0f, 5.0f);
    pBerserk = AddSpell(SPELL_BERSERK, Target_Self, 0, 0, 60);
    pGuardA = AddSpell(SPELL_GUARDIAN_AURA, Target_Self, 0, 0, 0);
    mLoc.ChangeCoords(GetUnit()->GetPositionX(),GetUnit()->GetPositionY(),GetUnit()->GetPositionZ(),GetUnit()->GetOrientation());
    AddEmote(Event_OnCombatStart, "The secrets of the deep shall remain hidden.", Text_Yell);
    AddEmote(Event_OnTargetDied, "Sleep now, in the cold dark.", Text_Yell);
    AddEmote(Event_OnTargetDied, "For the Lich King!", Text_Yell);
    AddEmote(Event_OnTargetDied, "Perhaps we will be allies soon.", Text_Yell);
    AddEmote(Event_OnDied, "Master, is my service complete?", Text_Yell);
};

void NadoxAI::OnCombatStart(Unit *pTarget)
{
    mSwarmerTimer = AddTimer(RandomUInt(15)*1000);
    ParentClass::OnCombatStart(pTarget);
};

void NadoxAI::AIUpdate()
{   
    if (IsTimerFinished(mSwarmerTimer))
    {
        ResetTimer(mSwarmerTimer, RandomUInt(15)*1000);
        uint32 m_swarm = RandomUInt(5)+1;
        for (uint32 i=0; i < m_swarm; ++i)
        {
            mSwarm = SpawnCreature(NPC_SWARMER, 682.851f, -905.214f, 25.614f, 0.0f, true, GetUnit()->GetAIInterface()->GetSecondHated());
            if (IsHeroic()) Swarmers.push_back(TO_UNIT(mSwarm));
        }
    }
    if (GetUnit()->CalcDistance(mLoc) >= 100.0f)
        CastSpell(pBerserk);
    if (IsHeroic())
    {
        switch(RandomUInt(2))
        {
            case 0:
                Emote("The young must not grow hungry..."); break;
            case 1:
                Emote("Shhhad ak kereeesshh chak-k-k!"); break;
        }
        for (UnitArray::iterator swarm = Swarmers.begin(); swarm != Swarmers.end(); ++swarm)
            if ((*swarm)!=NULLUNIT && (*swarm)->isAlive())
                GetUnit()->CastSpell((*swarm), SPELL_BROOD_RAGE, false);
        Swarmers.clear();
    }
    if (GetHealthPercent() == 50)
    {
        mGuard = SpawnCreature(NPC_GUARDIAN, true, GetUnit()->GetAIInterface()->GetSecondHated());
        mGuard->CastSpell(pGuardA);
    }
	ParentClass::AIUpdate();
};


TaldaramAI::TaldaramAI( Creature *pCreature ) : MoonSciptBossAI(pCreature)
{
    AddSpell(SPELL_BLOODTHIRST, Target_Current, 15, 0, 20);
    pVanish = AddSpell(SPELL_VANISH, Target_Self, 0, 0, 0);
    pEmbrace = (IsHeroic()) ? AddSpell(H_SPELL_EMBRACE, Target_Current, 0, 0, 0) : AddSpell(SPELL_EMBRACE, Target_Current, 0, 0, 0);

};


