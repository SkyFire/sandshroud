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

#include "ScriptStdAfx.h"

/************************************/
/*									*/
/*		EMERALD DREAM DRAGONS!		*/
/*									*/
/************************************/


#define CN_EMERISS				14889
#define DREAM_FOG				15224
#define PUTRID_MUSHROOM			31690

#define NOXIOUS_BREATH			24818
#define TAIL_SWEEP				15847
#define MARK_OF_NATURE			25040
#define VOLATILE_INFECTION		24928
#define CORRUPTION_OF_EARTH		24910

class EmerissAI : public MoonScriptBossAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EmerissAI, MoonScriptBossAI);
    EmerissAI(Creature *pCreature) : MoonScriptBossAI(pCreature)
    {
        AddEmote(Event_OnCombatStart, "Hope is a DISEASE of the soul! This land shall wither and die!", Text_Yell);
        AddSpell(NOXIOUS_BREATH, Target_Current, 15, 0, 30);
        AddSpell(TAIL_SWEEP, Target_RandomPlayerNotCurrent, 20, 0, 20);
        mMark = AddSpell(MARK_OF_NATURE, Target_Current, 0, 0, 0);
        AddSpell(VOLATILE_INFECTION, Target_Current, 15, 0, 30);
        mCorrupt = AddSpell(CORRUPTION_OF_EARTH, Target_RandomPlayer, 0, 0, 0);
    }

    void OnCombatStart(Unit *pTarget)
    {
        mCorruptOne = 0;
        ParentClass::OnCombatStart(pTarget);
    }

    void OnCombatStop(Unit *pTarget)
    {
        mCorruptOne = 0;
        ParentClass::OnCombatStop(pTarget);
    }

    void AIUpdate()
    {   
        int32 percent = GetHealthPercent();
        if (percent >= 85 && 65 <= percent && mCorruptOne == 0)
            if(RandomUInt(10) > 5)
            {
                CastSpellInRange(mCorrupt);
                mCorruptOne++;
            }
        if (percent >= 60 && 40 <= percent && mCorruptOne == 1)
            if(RandomUInt(10) > 5)
            {
                CastSpellInRange(mCorrupt);
                mCorruptOne++;
            }
        if (percent >= 35 && 10 <= percent && mCorruptOne == 2)
            if(RandomUInt(10) > 5)
            {
                CastSpellInRange(mCorrupt);
                mCorruptOne++;
            }
        

        
        ParentClass::AIUpdate();
    }

    void OnTargetDied(Unit *pTarget)
    {
        SpawnCreature(PUTRID_MUSHROOM, pTarget->GetPositionX(),pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());
    }
protected:
    uint8 mCorruptOne;
    SpellDesc *mCorrupt, *mMark;
};

// Taerar AI
#define CN_TAERAR			14890
#define CN_SHADESTAERAR		15302

#define SLEEP				24777
#define NOXIOUS_BREATH		24818
#define TAIL_SWEEP			15847
#define TELEPORT			15734
#define MARK_OF_NATURE		25040	//If a player is killed by Taerar, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define ARCANE_BLAST		24857
#define BELLOWING_ROAR		22686	//Mass fear
#define SUMMON_SHADE		24843


// Shades of Taerar AI
#define CN_SHADEOFTAERAR 15302

#define POSION_CLOUD				24840
#define POSION_BREATH				20667





// Ysondre AI
#define CN_YSONDRE			14887
#define CN_YDRUIDS			15260

#define SLEEP				24777
#define NOXIOUS_BREATH		24818
#define TAIL_SWEEP			15847
#define TELEPORT			15734
#define MARK_OF_NATURE		25040	//If a player is killed by Ysondre, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define LIGHTNING_WAVE		24819
#define SUMMON_DRUIDS		24795	// Summon Demented Druid Spirit


// Demented Druid Spirit AI
#define CN_DEMENTEDDRUID	15260

#define MOONFIRE			   27737



// Lethon AI
#define CN_LETHON			14888

#define SLEEP				24777
#define NOXIOUS_BREATH		24818
#define TAIL_SWEEP			15847
#define TELEPORT			15734
#define MARK_OF_NATURE		25040	// If a player is killed by Lethon, they will be afflicted by a 15 minute debuff called Mark of Nature. If resurrected during this time, they will be slept for 2 minutes rather than 4 seconds if they are hit with Sleep.
#define SHADOW_WHIRL		24837
#define SUMMON_SHADES		24810




//Shades of Lethon

#define CN_LSHADE		15261





/****************************************/
/*										*/
/*			Rest of World Bosses		*/
/*										*/
/****************************************/



// Highlord Kruul
#define CN_KRUUL			18338
#define CN_HOUNDS			19207

#define SHADOW_VOLLEY		21341
#define CLEAVE				20677
#define THUNDER_CLAP		23931
#define TWISTED_REFLECTION	21063
#define VOID_BOLT			21066
#define RAGE				21340
#define CAPTURE_SOUL		21053



// Doom Lord Kazzak (Lord Kazzak does not exist anymore, he is promoted and replaced by Highlord Kruul instead)
// TODO:
// Death Messages. Both himself and player needs.
/*
Remaining:
11338,10,"A_GRULLAIR_Kazzak_Slay02" (443956),"GRULLAIR_Kazzak_Slay02.wav"
11340,10,"A_GRULLAIR_Kazzak_Death01" (444060),"GRULLAIR_Kazzak_Death01.wav"
*/

#define CN_KAZZAK			18728

#define SHADOW_VOLLEY		21341
#define CLEAVE				20677
#define THUNDER_CLAP		23931
#define TWISTED_REFLECTION	21063
#define VOID_BOLT			21066
#define RAGE				21340
#define CAPTURE_SOUL		21053
#define MARK_OF_KAZZAK		21056



// Azuregos

#define CN_AZUREGOS		6109

#define MARK_OF_FROST	23183
#define MANA_STORM		21097
#define REFLECT			30969	//Might not be the right, this one doesn't work on dots?
#define ACLEAVE			8255	//This is Strong Cleave, maybe it should be 27794, normal cleave 250+ damage
#define CONE_OF_COLD	30095
#define MASS_TELEPORT	16807



// Doomwalker

#define CN_DOOMWALKER		17711

#define EARTHQUAKE				 32686		  
#define MARK_OF_DEATH		   37128	
#define CHAIN_LIGHTNING			33665
#define OVERRUN					32636
#define ENRAGE				33653
#define AURA_OF_DEATH		37131
#define SUNDER_ARMOR		  33661



// Teremus The Devourer

#define CN_TEREMUS			7846

#define FLAME_BREATH		20712
#define SOUL_CONSUMPTION	12667
#define RESIST_ALL			18114


void SetupWorldBosses(ScriptMgr * mgr)
{
	mgr->register_creature_script(CN_EMERISS, &EmerissAI::Create);
	//mgr->register_creature_script(CN_TAERAR, &TaerarAI::Create);
	//mgr->register_creature_script(CN_SHADEOFTAERAR, &ShadeofTaerarAI::Create);
	//mgr->register_creature_script(CN_YSONDRE, &YsondreAI::Create);
	//mgr->register_creature_script(CN_DEMENTEDDRUID, &DementedDruidSpiritAI::Create);
	//mgr->register_creature_script(CN_LETHON, &LethonAI::Create);
	//mgr->register_creature_script(CN_LSHADE, &ShadeofLethonAI::Create);
	//mgr->register_creature_script(CN_KRUUL, &KruulAI::Create);
	//mgr->register_creature_script(CN_KAZZAK, &KazzakAI::Create);
	//mgr->register_creature_script(CN_AZUREGOS, &AzuregosAI::Create);
	//mgr->register_creature_script(CN_DOOMWALKER, &DoomwalkerAI::Create);
	//mgr->register_creature_script(CN_TEREMUS, &TeremusAI::Create);
}
