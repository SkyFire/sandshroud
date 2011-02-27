/*
 * Sun++ Scripts for Aspire MMORPG Server
 * Copyright (C) 2010 Sandshroud Team <http://www.sandshroud.org/>
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

#include "ScriptStdAfx.h"
#include "Raid_TheRubySanctum.h"

void SpellFunc_Enervating_Brand(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType);
BaltharusAI::BaltharusAI( Creature *pCreature ) : MoonScriptBossAI(pCreature)
{
    if(GetInstanceMode() == MODE_25PLAYER_NORMAL)
        bThreeClones = true;
    bThreeClones = false;
    AddSpell(SPELL_BLADE_TEMPEST, Target_Current, 15, 0, 13, 0.0f, 15.0f);
    pRepellingWave = AddSpell(SPELL_REPELLING_WAVE, Target_Current, 0, 1, 30);
    pEnervatingBrand = AddSpell(SPELL_ENERVATING_BRAND, Target_Current, 0, 0, 0);
    pSiphonedMight = AddSpell(SPELL_SIPHONED_MIGHT, Target_Self, 0, 0, 0);
    AddSpell(SPELL_CLEAVE, Target_Current, 15, 0, 30); //dunno if he really use it
    AddSpellFunc(&SpellFunc_Enervating_Brand, Target_Current, 15, 0, 60);
};

void BaltharusAI::AIUpdate()
{
    if(GetHealthPercent()==75 && bThreeClones){
        CastSpell(pRepellingWave);
        mClone = SpawnCreature(NPC_BALTHARUSCLONE, false, GetBestUnitTarget(TargetFilter_SecondMostHated, 0.0f, 40.0f));
    }
    if(GetHealthPercent()==50){
        CastSpell(pRepellingWave);
        mClone = SpawnCreature(NPC_BALTHARUSCLONE, false, GetBestUnitTarget(TargetFilter_SecondMostHated, 0.0f, 40.0f));
    }
    if(GetHealthPercent()==25 && bThreeClones){
        CastSpell(pRepellingWave);
        mClone = SpawnCreature(NPC_BALTHARUSCLONE, false, GetBestUnitTarget(TargetFilter_SecondMostHated, 0.0f, 40.0f));
    }
};

BaltharusCloneAI::BaltharusCloneAI( Creature *pCreature ) : MoonScriptCreatureAI(pCreature)
{
    AddSpell(SPELL_BLADE_TEMPEST, Target_Current, 15, 0, 13, 0.0f, 15.0f);
    AddSpell(SPELL_CLEAVE, Target_Current, 15, 0, 30);
    AddSpellFunc(&SpellFunc_Enervating_Brand, Target_Current, 15, 0, 60);
};


ZarithrianAI::ZarithrianAI( Creature *pCreature ) : MoonScriptBossAI(pCreature)
{
    AddSpell(SPELL_CLEAVE_ARMOR, Target_Current, 15, 0, 30);
    AddSpell(SPELL_INTIMIDATING_ROAR, Target_Current, 15, 1.5f, 30, 0.0f, 40.0f);
};

void SpellFunc_Conflagration(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType);
SavianaAI::SavianaAI( Creature *pCreature ) : MoonScriptBossAI(pCreature)
{
    pEnrage = AddSpell(SPELL_ENRAGE, Target_Self, 0, 0, 60);
    AddSpell(SPELL_FLAME_BREATH, Target_Current, 15, 2.0f, 60);
    pConflagration = AddSpell(SPELL_CONFLAGRATION, Target_RandomPlayerNotCurrent, 0, 0, 0);
    AddSpellFunc(&SpellFunc_Conflagration, Target_RandomPlayerNotCurrent, 15, 6, 60);

};

