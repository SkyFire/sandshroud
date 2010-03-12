/*
 * Sun++ Scripts
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunplusplus.info/> 
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
#include "Base.h"

uint32 opengatebt = 4; // contador para abrir la puerta despues de haber matado a shade of akama, gurtogg bloodboil, reliquary of lost y teron gorefiend para pasar a mother sharaz y weass..
uint32 spawnakama = 4; // contador para que spawnee AKAMA, despues de haber matado a los 4 boss ( the illidari concilium ) para seguir con el evento de Illidan lalala y weas testeado y funcionando...

//abrir puerta hacia mother sharaz
void Matar4firstboss(uint32 count, CreaturePointer unitx)
{
	   opengatebt -= count;
	   if (opengatebt<=0)
	   {
			GameObjectPointer pDoorBT = unitx->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(855.715f, 392.976f, 112.784f, 185479);
			if(pDoorBT == 0)
					return;
			pDoorBT->SetByte(GAMEOBJECT_BYTES_1, 0, 0); // Open the door
	   }
}
// spawn akama para evento illidan
void Matar4TheIllidariConcilio(uint32 count, CreaturePointer unitx)
{
	   spawnakama -= count;
	   if (spawnakama<=0)
	   {
			CreaturePointer cre = NULLCREATURE;
            cre = unitx->GetMapMgr()->GetInterface()->SpawnCreature(22990, 661.563f, 305.711f, 271.689f, 0.00628f, true, false, 0, 0); 
			if(cre == 0)
					return;
	   }
}
/************************************************************************/
/* Raid_BlackTemple.cpp Script								           */
/************************************************************************/

//////////////////////////////////////////////////////////////////////////
/////// Trash AIs

class TemplateAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TemplateAI, MoonScriptCreatureAI);
	TemplateAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
/////// Beast AIs

#define CN_DRAGON_TURTLE					22885
#define DRAGON_TRUTLE_SHELL_SHIELD			40087
#define DRAGON_TURTLE_WATER_SPIT			40086

class DragonTurtleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonTurtleAI, MoonScriptCreatureAI);
	DragonTurtleAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGON_TRUTLE_SHELL_SHIELD, Target_Self, 10, 0, 25);
		AddSpell(DRAGON_TURTLE_WATER_SPIT, Target_Current, 8, 2.5, 20, 0, 45);		// Random?
	}
};

#define CN_LEVIATHAN						22884
#define LEVIATHAN_DEBILITATING_SPRAY		40079
#define LEVIATHAN_POISON_SPIT				40078
#define LEVIATHAN_TAIL_SWEEP				40077

class LeviathanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LeviathanAI, MoonScriptCreatureAI);
	LeviathanAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(LEVIATHAN_DEBILITATING_SPRAY, Target_RandomPlayer, 8, 2.5, 20);
		AddSpell(LEVIATHAN_POISON_SPIT, Target_RandomPlayer, 7, 2.5, 25);
		AddSpell(LEVIATHAN_TAIL_SWEEP, Target_Self, 7, 0, 30);
	}
};

#define CN_MUTANT_WAR_HOUND					23232
#define MUTANT_WAR_HOUND_CLOUD_OF_DISEASE	41193

class MutantWarHoundAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MutantWarHoundAI, MoonScriptCreatureAI);
	MutantWarHoundAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnDied(UnitPointer pKiller)
	{
		ParentClass::OnDied(pKiller);

		AuraPointer pAura = AuraPointer(new Aura(dbcSpell.LookupEntry(MUTANT_WAR_HOUND_CLOUD_OF_DISEASE), 20000, _unit, _unit));
		if (pAura != NULL)
			_unit->AddAura(pAura,NULLAURA);
	}
};
// I don't have this creature in DB, so hasn't checked it yet
#define CN_SHADOWMOON_RIDING_HOUND					23083
#define SHADOWMOON_RIDING_HOUND_CARNIVOROUS_BITE	41092
#define SHADOWMOON_RIDING_HOUND_CHARGE				25821
#define SHADOWMOON_RIDING_HOUND_ENRAGE				8599

void SpellFunc_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType);

class ShadowmoonRidingHoundAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonRidingHoundAI, MoonScriptCreatureAI);
	ShadowmoonRidingHoundAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_RIDING_HOUND_CARNIVOROUS_BITE, Target_Current, 10, 0, 20, 0, 10);
		mCharge = AddSpell(SHADOWMOON_RIDING_HOUND_CHARGE, Target_Current, 0, 0, 0);
		AddSpellFunc(&SpellFunc_Charge, Target_RandomPlayer, 8, 0, 35);
		AddSpell(SHADOWMOON_RIDING_HOUND_ENRAGE, Target_Self, 6, 0, 40);
	}

	SpellDesc*	mCharge;
};

void SpellFunc_Charge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	ShadowmoonRidingHoundAI *pHoundAI = (pCreatureAI != NULL) ? (ShadowmoonRidingHoundAI*)pCreatureAI : NULL;
	if (pHoundAI != NULL)
	{
		UnitPointer pCurrentTarget = pHoundAI->GetUnit()->GetAIInterface()->GetNextTarget();
		if (pCurrentTarget != NULL && pCurrentTarget != pTarget)
		{
			pHoundAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
			pHoundAI->GetUnit()->GetAIInterface()->SetNextTarget(pTarget);
			pHoundAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		}

		pHoundAI->CastSpell(pHoundAI->mCharge);
	}
}

//////////////////////////////////////////////////////////////////////////
/////// Demon AIs

#define	CN_SISTER_OF_PLEASURE				22964
#define	SISTER_OF_PLEASURE_GREATER_HEAL		41378
#define SISTER_OF_PLEASURE_HOLY_NOVA		41380
#define SISTER_OF_PLEASURE_SHARED_BONDS		41362
#define SISTER_OF_PLEASURE_SHELL_OF_LIFE	41381

class SisterOfPleasureAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SisterOfPleasureAI, MoonScriptCreatureAI);
	SisterOfPleasureAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SISTER_OF_PLEASURE_GREATER_HEAL, Target_WoundedFriendly, 10, 2, 45);
		AddSpell(SISTER_OF_PLEASURE_HOLY_NOVA, Target_Self, 10, 0, 20);
		AddSpell(SISTER_OF_PLEASURE_SHARED_BONDS, Target_Current, 5, 0, 25);
		AddSpell(SISTER_OF_PLEASURE_SHELL_OF_LIFE, Target_Self, 7, 1, 45);
	}
};

#define	CN_SISTER_OF_PAIN					22956
#define	SISTER_OF_PAIN_LASH_OF_PAIN			41353
#define	SISTER_OF_PAIN_PAINFUL_RAGE			41369
#define	SISTER_OF_PAIN_SHADOW_WORD_PAIN		41355
#define	SISTER_OF_PAIN_SHARED_BONDS			41362
#define	SISTER_OF_PAIN_SHELL_OF_PAIN		41371

class SisterOfPainAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SisterOfPainAI, MoonScriptCreatureAI);
	SisterOfPainAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SISTER_OF_PAIN_LASH_OF_PAIN, Target_Current, 10, 0, 25, 0, 10);
		AddSpell(SISTER_OF_PAIN_PAINFUL_RAGE, Target_Self, 5, 0, 60);
		AddSpell(SISTER_OF_PAIN_SHADOW_WORD_PAIN, Target_RandomPlayer, 10, 0, 25);
		AddSpell(SISTER_OF_PAIN_SHARED_BONDS, Target_Current, 5, 0, 25);
		AddSpell(SISTER_OF_PAIN_SHELL_OF_PAIN, Target_Self, 7, 1, 45);
	}
};

#define	CN_PRIESTESS_OF_DEMENTIA				22957
#define PRIESTESS_OF_DEMENTIA_CONFUSION			41397
#define PRIESTESS_OF_DEMENTIA_DEMENTIA			41404

class PriestessOfDementiaAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PriestessOfDementiaAI, MoonScriptCreatureAI);
	PriestessOfDementiaAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(PRIESTESS_OF_DEMENTIA_CONFUSION, Target_Self, 8, 0, 30);
		AddSpell(PRIESTESS_OF_DEMENTIA_DEMENTIA, Target_Self, 5, 0, 60);
	}
};

#define	CN_PRIESTESS_OF_DELIGHT					22962
#define PRIESTESS_OF_DELIGHT_CURSE_OF_VITALITY	41351

class PriestessOfDelightAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PriestessOfDelightAI, MoonScriptCreatureAI);
	PriestessOfDelightAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(PRIESTESS_OF_DELIGHT_CURSE_OF_VITALITY, Target_RandomPlayer, 10, 0, 30);
	}
};

#define	CN_ILLIDARI_NIGHTLORD					22855
#define ILLIDARI_NIGHTLORD_SUMMON_SHADOWFIENDS	39649
#define ILLIDARI_NIGHTLORD_SHADOW_INFERNO		39645
#define ILLIDARI_NIGHTLORD_FEAR					41150
#define ILLIDARI_NIGHTLORD_CURSE_OF_MENDING		39647

class IllidariNightlordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariNightlordAI, MoonScriptCreatureAI);
	IllidariNightlordAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_NIGHTLORD_SUMMON_SHADOWFIENDS, Target_Self, 7, 0, 45);			// does it work?
		AddSpell(ILLIDARI_NIGHTLORD_SHADOW_INFERNO, Target_Self, 10, 0, 25);
		AddSpell(ILLIDARI_NIGHTLORD_FEAR, Target_Self, 7, 1, 30);
		AddSpell(ILLIDARI_NIGHTLORD_CURSE_OF_MENDING, Target_RandomPlayer, 8, 2, 35);
	}
};

#define CN_ILLIDARI_HEARTSEEKER							23339
#define ILLIDARI_HEARTSEEKER_CURSE_OF_THE_BLEAKHEART	41170
#define ILLIDARI_HEARTSEEKER_RAPID_SHOT					41173
#define ILLIDARI_HEARTSEEKER_SHOOT						41169
#define ILLIDARI_HEARTSEEKER_SKELETON_SHOT				41171

void SpellFunc_RapidShot(SpellDesc * pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType);

class IllidariHeartseekerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariHeartseekerAI, MoonScriptCreatureAI);
	IllidariHeartseekerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_HEARTSEEKER_CURSE_OF_THE_BLEAKHEART, Target_RandomPlayer, 8, 0, 40, 0, 30);
		mRapidShot = AddSpell(ILLIDARI_HEARTSEEKER_RAPID_SHOT, Target_Self, 0, 8, 0);						// I need to check mechanics (maybe it should speed up usage of Shoot spell (2x lesser freq))
		AddSpell(ILLIDARI_HEARTSEEKER_SHOOT, Target_Current, 80, 0, 1, 0, 30);
		AddSpell(ILLIDARI_HEARTSEEKER_SKELETON_SHOT, Target_RandomPlayer, 7, 0, 25, 0, 30);
		AddSpellFunc(&SpellFunc_RapidShot, Target_Current, 7, 8, 40, 0, 30);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 30.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		UnitPointer pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 30.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}

	SpellDesc*	mRapidShot;
};

void SpellFunc_RapidShot(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	IllidariHeartseekerAI *pHeartseeker = (pCreatureAI != NULL) ? (IllidariHeartseekerAI*)pCreatureAI : NULL;
	if (pHeartseeker != NULL)
	{
		pHeartseeker->CastSpell(pHeartseeker->mRapidShot);
	}
}

#define CN_ILLIDARI_FEARBRINGER					22954
#define ILLIDARI_FEARBRINGER_ILLIDARI_FLAMES	40938
#define ILLIDARI_FEARBRINGER_RAIN_OF_CHAOS		40946
#define ILLIDARI_FEARBRINGER_WAR_STOMP			40936

class IllidariFearbringerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariFearbringerAI, MoonScriptCreatureAI);
	IllidariFearbringerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_FEARBRINGER_ILLIDARI_FLAMES, Target_Current, 8, 2, 25, 0, 10);
		AddSpell(ILLIDARI_FEARBRINGER_RAIN_OF_CHAOS, Target_RandomDestination, 7, 0, 35);
		AddSpell(ILLIDARI_FEARBRINGER_WAR_STOMP, Target_Self, 8, 0, 35);
	}
};

#define CN_ILLIDARI_DEFILER					22853
#define ILLIDARI_DEFILER_BANISH				39674
#define ILLIDARI_DEFILER_CURSE_OF_AGONY		39672
#define ILLIDARI_DEFILER_FEL_IMMOLATE		39670
#define ILLIDARI_DEFILER_RAIN_OF_CHAOS		39671

class IllidariDefilerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariDefilerAI, MoonScriptCreatureAI);
	IllidariDefilerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_DEFILER_BANISH, Target_RandomPlayer, 8, 1, 30);
		AddSpell(ILLIDARI_DEFILER_CURSE_OF_AGONY, Target_RandomPlayer, 7, 0, 45);
		AddSpell(ILLIDARI_DEFILER_FEL_IMMOLATE, Target_RandomPlayer, 8, 2, 25);
		AddSpell(ILLIDARI_DEFILER_RAIN_OF_CHAOS, Target_RandomDestination, 7, 6, 35);
	}
};

#define CN_ILLIDARI_CENTURION				23337
#define ILLIDARI_CENTURION_CLEAVE			15284
#define ILLIDARI_CENTURION_SONIC_STRIKE		41168

class IllidariCenturionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariCenturionAI, MoonScriptCreatureAI);
	IllidariCenturionAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_CENTURION_CLEAVE, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(ILLIDARI_CENTURION_SONIC_STRIKE, Target_Self, 8, 0, 35);
	}
};

#define CN_ILLIDARI_BONESLICER					22869
#define ILLIDARI_BONESLICER_CLOAK_OF_SHADOWS	39666
#define ILLIDARI_BONESLICER_GOUGE				24698
#define ILLIDARI_BONESLICER_SHADOWSTEP			41176

void SpellFunc_Gouge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType);

class IllidariBoneslicerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariBoneslicerAI, MoonScriptCreatureAI);
	IllidariBoneslicerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_BONESLICER_CLOAK_OF_SHADOWS, Target_Self, 8, 0, 25);
		mGouge = AddSpell(ILLIDARI_BONESLICER_GOUGE, Target_Current, 0, 0, 0);
		AddSpellFunc(&SpellFunc_Gouge, Target_Current, 7, 0, 25, 0, 10);
		AddSpell(ILLIDARI_BONESLICER_SHADOWSTEP, Target_Current, 7, 0, 30);
	}

	SpellDesc*	mGouge;
};

void SpellFunc_Gouge(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	IllidariBoneslicerAI *pBoneslicer = (pCreatureAI != NULL) ? (IllidariBoneslicerAI*)pCreatureAI : NULL;
	if (pBoneslicer != NULL)
	{
		pBoneslicer->CastSpell(pBoneslicer->mGouge);
		UnitPointer pSecondHated = pBoneslicer->GetUnit()->GetAIInterface()->GetSecondHated();
		if (pSecondHated != NULL)
		{
			pBoneslicer->GetUnit()->GetAIInterface()->AttackReaction(pSecondHated, 2000);
			pBoneslicer->GetUnit()->GetAIInterface()->SetNextTarget(pSecondHated);
			pBoneslicer->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pTarget);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/////// Humanoid AIs

#define CN_ASHTONGUE_BATTLELORD					22844
#define ASHTONGUE_BATTLELORD_CLEAVE				15284
#define ASHTONGUE_BATTLELORD_CONCUSSION_BLOW	32588
#define ASHTONGUE_BATTLELORD_CONCUSSIVE_THROW	41182
#define ASHTONGUE_BATTLELORD_ENRAGE				34970

class AshtongueBattlelordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueBattlelordAI, MoonScriptCreatureAI);
	AshtongueBattlelordAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_BATTLELORD_CLEAVE, Target_Current, 10, 0, 15, 0, 10);
		AddSpell(ASHTONGUE_BATTLELORD_CONCUSSION_BLOW, Target_Current, 8, 0, 30, 0, 10);
		AddSpell(ASHTONGUE_BATTLELORD_CONCUSSIVE_THROW, Target_RandomPlayerNotCurrent, 8, 0, 25, 10, 40);
		AddSpell(ASHTONGUE_BATTLELORD_ENRAGE, Target_Self, 4, 0, 50);
	}
};

#define CN_ASHTONGUE_DEFENDER					23216
#define ASHTONGUE_DEFENDER_DEBILITATING_STRIKE	41178
#define ASHTONGUE_DEFENDER_SHIELD_BASH			41180

class AshtongueDefenderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueDefenderAI, MoonScriptCreatureAI);
	AshtongueDefenderAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_DEFENDER_DEBILITATING_STRIKE, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(ASHTONGUE_DEFENDER_SHIELD_BASH, Target_Current, 7, 0, 25, 0, 10);
	}
};

#define CN_ASHTONGUE_ELEMENTALIST				23523
#define ASHTONGUE_ELEMENTALIST_LIGHTNING_BOLT	42024
#define ASHTONGUE_ELEMENTALIST_RAID_OF_FIRE		42023

class AshtongueElementalistAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueElementalistAI, MoonScriptCreatureAI);
	AshtongueElementalistAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_ELEMENTALIST_LIGHTNING_BOLT, Target_Current, 8, 1, 20, 0, 50);	// Random?
		AddSpell(ASHTONGUE_ELEMENTALIST_RAID_OF_FIRE, Target_RandomDestination, 6, 0, 25);
	}
};
// TO DO: Add Totem AIs
#define CN_ASHTONGUE_MYSTIC						22845
#define ASHTONGUE_MYSTIC_BLOODLUST				41185
#define ASHTONGUE_MYSTIC_CHAIN_HEAL				41114
#define ASHTONGUE_MYSTIC_CYCLONE_TOTEM			39589
#define ASHTONGUE_MYSTIC_FLAME_SHOCK			41115
#define ASHTONGUE_MYSTIC_FROST_SHOCK			41116
#define ASHTONGUE_MYSTIC_SEARING_TOTEM			39588
#define ASHTONGUE_MYSTIC_SUMMON_WINDFURY_TOTEM	39586

class AshtongueMysticAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueMysticAI, MoonScriptCreatureAI);
	AshtongueMysticAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_MYSTIC_BLOODLUST, Target_Self, 5, 0, 45);
		AddSpell(ASHTONGUE_MYSTIC_CHAIN_HEAL, Target_WoundedFriendly, 6, 2.5, 35);
		AddSpell(ASHTONGUE_MYSTIC_CYCLONE_TOTEM, Target_Self, 7, 0, 35);
		AddSpell(ASHTONGUE_MYSTIC_FLAME_SHOCK, Target_Current, 8, 0, 25, 0, 20);
		AddSpell(ASHTONGUE_MYSTIC_FROST_SHOCK, Target_Current, 8, 0, 25, 0, 20);
		AddSpell(ASHTONGUE_MYSTIC_SEARING_TOTEM, Target_Self, 7, 0, 35);
		AddSpell(ASHTONGUE_MYSTIC_SUMMON_WINDFURY_TOTEM, Target_Self, 7, 0, 35);
	}
};

#define CN_ASHTONGUE_PRIMALIST					22847
#define ASHTONGUE_PRIMALIST_MULTISHOT			41187
#define ASHTONGUE_PRIMALIST_SHOOT				41188
#define ASHTONGUE_PRIMALIST_SWEEPING_WING_CLIP	39584
#define ASHTONGUE_PRIMALIST_WYVERN_STRING		41186

class AshtonguePrimalistAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtonguePrimalistAI, MoonScriptCreatureAI);
	AshtonguePrimalistAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_PRIMALIST_MULTISHOT, Target_Current, 8, 0, 40, 0, 30);			// Target_Self - self dmg too (kill == crash)
		AddSpell(ASHTONGUE_PRIMALIST_SHOOT, Target_Current, 80, 0, 1, 0, 30);
		AddSpell(ASHTONGUE_PRIMALIST_SWEEPING_WING_CLIP, Target_Current, 8, 0, 15, 0, 10, true);
		AddSpell(ASHTONGUE_PRIMALIST_WYVERN_STRING, Target_RandomPlayer, 7, 0, 25, 0, 30);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 30.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		UnitPointer pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 30.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};

#define CN_ASHTONGUE_ROGUE					23318
#define ASHTONGUE_ROGUE_DEBILITATING_POISON	41978
#define ASHTONGUE_ROGUE_EVISCERATE			41177

class AshtongueRogueAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueRogueAI, MoonScriptCreatureAI);
	AshtongueRogueAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_ROGUE_DEBILITATING_POISON, Target_Current, 7, 0, 30, 0, 10);
		AddSpell(ASHTONGUE_ROGUE_EVISCERATE, Target_Current, 10, 0, 15, 0, 10);
	}
};

#define CN_ASHTONGUE_SPIRITBINDER			23524
#define ASHTONGUE_SPIRITBINDER_CHAIN_HEAL	42027
#define ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL	42317
#define ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL2	42318
#define ASHTONGUE_SPIRITBINDER_SPIRIT_MEND	42025

class AshtongueSpiritbinderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueSpiritbinderAI, MoonScriptCreatureAI);
	AshtongueSpiritbinderAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_SPIRITBINDER_CHAIN_HEAL, Target_WoundedFriendly, 7, 1, 25);
		if (_unit->GetMapMgr()->iInstanceMode != MODE_HEROIC)	// Guessed
			AddSpell(ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL, Target_Self, 7, 0, 25);				// Weaker version
		else
			AddSpell(ASHTONGUE_SPIRITBINDER_SPIRIT_HEAL2, Target_Self, 7, 0, 25);				// Stronger version
		AddSpell(ASHTONGUE_SPIRITBINDER_SPIRIT_MEND, Target_WoundedFriendly, 8, 0, 25);
	}
};
// Completely guessed mechanics
#define CN_ASHTONGUE_STALKER					23374
#define ASHTONGUE_STALKER_BLIND					34654
#define ASHTONGUE_STALKER_INSTANT_POISON		41189
#define ASHTONGUE_STALKER_MINDNUMBING_POISON	41190
#define ASHTONGUE_STALKER_STEATH				34189

class AshtongueStalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueStalkerAI, MoonScriptCreatureAI);
	AshtongueStalkerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_STALKER_BLIND, Target_RandomPlayer, 7, 1, 25);				// Should be Blind - > Attack Next Target?
		AddSpell(ASHTONGUE_STALKER_INSTANT_POISON, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(ASHTONGUE_STALKER_MINDNUMBING_POISON, Target_RandomPlayer, 8, 0, 20);
		ApplyAura(ASHTONGUE_STALKER_STEATH);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pSpell = FindSpellById(ASHTONGUE_STALKER_BLIND);
		if (pSpell != NULL)
		{
			CastSpellNowNoScheduling(pSpell);
		}
	}

	void OnCombatStop(UnitPointer pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (IsAlive())
		{
			ApplyAura(ASHTONGUE_STALKER_STEATH);
		}
	}
};

#define CN_ASHTONGUE_STORMCALLER				22846
#define ASHTONGUE_STORMCALLER_CHAIN_LIGHTNING	41183
#define ASHTONGUE_STORMCALLER_LIGHTNING_BOLT	41184
#define ASHTONGUE_STORMCALLER_LIGHTNING_SHIELD	41151

class AshtongueStormcallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueStormcallerAI, MoonScriptCreatureAI);
	AshtongueStormcallerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ASHTONGUE_STORMCALLER_CHAIN_LIGHTNING, Target_Current, 7, 3, 35, 0, 30);		// Random?
		AddSpell(ASHTONGUE_STORMCALLER_LIGHTNING_BOLT, Target_Current, 10, 2, 20);
		AddSpell(ASHTONGUE_STORMCALLER_LIGHTNING_SHIELD, Target_Self, 10, 0, 50);
	}
};

#define CN_BONECHEWER_BEHEMOTH					23196
#define BONECHEWER_BEHEMOTH_BEHEMOTH_CHARGE		41272
#define BONECHEWER_BEHEMOTH_ENRAGE				8269
#define BONECHEWER_BEHEMOTH_FEL_STOMP			41274
#define BONECHEWER_BEHEMOTH_FIERY_COMET			41277
#define BONECHEWER_BEHEMOTH_METEOR				41276

class BonechewerBehemothAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBehemothAI, MoonScriptCreatureAI);
	BonechewerBehemothAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BEHEMOTH_BEHEMOTH_CHARGE, Target_Current, 0, 0, 20);	// Mechanics are like charge on random target?
		AddSpell(BONECHEWER_BEHEMOTH_ENRAGE, Target_Self, 5, 0, 45);
		AddSpell(BONECHEWER_BEHEMOTH_FEL_STOMP, Target_Self, 7, 0, 30);
		AddSpell(BONECHEWER_BEHEMOTH_FIERY_COMET, Target_RandomDestination, 6, 1, 30);
		AddSpell(BONECHEWER_BEHEMOTH_METEOR, Target_RandomDestination, 5, 2, 40);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pCharge = FindSpellById(BONECHEWER_BEHEMOTH_BEHEMOTH_CHARGE);
		if (pCharge != NULL)
		{
			CastSpellNowNoScheduling(pCharge);
		}
	}
};

#define CN_BONECHEWER_BLADE_FURY					23235
#define BONECHEWER_BLADE_FURY_WHIRLWIND				41194
#define BONECHEWER_BLADE_FURY_WHIRLWIND2			41195

class BonechewerBladeFuryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBladeFuryAI, MoonScriptCreatureAI);
	BonechewerBladeFuryAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BLADE_FURY_WHIRLWIND, Target_Self, 7, 8, 40);	// Self / Current ?
	}
};

#define CN_BONECHEWER_BLOOD_PROPHET					23237
#define BONECHEWER_BLOOD_PROPHET_BLOOD_DRAIN		41238
#define BONECHEWER_BLOOD_PROPHET_BLOODBOLT			41229
#define BONECHEWER_BLOOD_PROPHET_ENRAGE				8269
#define BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD	41230
#define BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD2	41231

class BonechewerBloodProphetAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBloodProphetAI, MoonScriptCreatureAI);
	BonechewerBloodProphetAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BLOOD_PROPHET_BLOOD_DRAIN, Target_Current, 6, 5, 30, 0, 20);	// Random?
		AddSpell(BONECHEWER_BLOOD_PROPHET_BLOODBOLT, Target_RandomPlayer, 7, 1.5, 25);
		AddSpell(BONECHEWER_BLOOD_PROPHET_ENRAGE, Target_Self, 5, 0, 50);
		if (_unit->GetMapMgr()->iInstanceMode == MODE_HEROIC)						// Guessed
			AddSpell(BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD, Target_RandomPlayer, 10, 0, 30);
		else
			AddSpell(BONECHEWER_BLOOD_PROPHET_PROPHECY_OF_BLOOD2, Target_RandomPlayer, 10, 0, 30);
	}
};

#define CN_BONECHEWER_BRAWLER				23222
#define BONECHEWER_BRAWLER_FRENZY			41254

class BonechewerBrawlerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerBrawlerAI, MoonScriptCreatureAI);
	BonechewerBrawlerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_BRAWLER_FRENZY, Target_Self, 7, 0, 30);
	}
};

#define CN_BONECHEWER_COMBATANT				23239
#define BONECHEWER_COMBATANT_FRENZY			8269

class BonechewerCombatantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerCombatantAI, MoonScriptCreatureAI);
	BonechewerCombatantAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_COMBATANT_FRENZY, Target_Self, 7, 0, 45);
	}
};

#define CN_BONECHEWER_SHIELD_DISCIPLE				23236
#define BONECHEWER_SHIELD_DISCIPLE_INTERVENE		41198
#define BONECHEWER_SHIELD_DISCIPLE_SHIELD_BASH		41197
#define BONECHEWER_SHIELD_DISCIPLE_SHIELD_WALL		41196
#define BONECHEWER_SHIELD_DISCIPLE_THROW_SHIELD		41213

class BonechewerShieldDiscipleAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerShieldDiscipleAI, MoonScriptCreatureAI);
	BonechewerShieldDiscipleAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_INTERVENE, Target_Current, 0, 0, 20);
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_SHIELD_BASH, Target_Current, 8, 0, 25, 0, 10);
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_SHIELD_WALL, Target_Self, 8, 0, 35);
		AddSpell(BONECHEWER_SHIELD_DISCIPLE_THROW_SHIELD, Target_RandomPlayer, 7, 0, 30);	// Current?
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pIntervene = FindSpellById(BONECHEWER_SHIELD_DISCIPLE_INTERVENE);
		if (pIntervene != NULL)
		{
			CastSpellNowNoScheduling(pIntervene);
		}
	}
};

#define CN_BONECHEWER_SPECTATOR					23223
#define BONECHEWER_SPECTATOR_CHARGE				36140
#define BONECHEWER_SPECTATOR_CLEAVE				40505
#define BONECHEWER_SPECTATOR_MORTAL_WOUND		25646
#define BONECHEWER_SPECTATOR_STRIKE				13446
#define BONECHEWER_SPECTATOR_SUNDER_ARMOR		13444

class BonechewerSpectatorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerSpectatorAI, MoonScriptCreatureAI);
	BonechewerSpectatorAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_SPECTATOR_CHARGE, Target_Current, 0, 0, 20);
		AddSpell(BONECHEWER_SPECTATOR_CLEAVE, Target_Current, 10, 0, 25, 0, 10);
		AddSpell(BONECHEWER_SPECTATOR_MORTAL_WOUND, Target_Current, 7, 0, 15, 0, 10);
		AddSpell(BONECHEWER_SPECTATOR_STRIKE, Target_Current, 10, 0, 10, 0, 10);
		AddSpell(BONECHEWER_SPECTATOR_SUNDER_ARMOR, Target_Current, 7, 0, 20, 0, 10);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pCharge = FindSpellById(BONECHEWER_SPECTATOR_CHARGE);
		if (pCharge != NULL)
		{
			CastSpellNowNoScheduling(pCharge);
		}
	}
};

#define CN_BONECHEWER_TASKMASTER				23028
#define BONECHEWER_TASKMASTER_DISGRUNTLED		40851
#define BONECHEWER_TASKMASTER_FURY				40845

class BonechewerTaskmasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerTaskmasterAI, MoonScriptCreatureAI);
	BonechewerTaskmasterAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_TASKMASTER_DISGRUNTLED, Target_Self, 0, 0, 45);	//5, 0, 45);	// Disabled for now (buffs players instead of Taskmasters)		
		AddSpell(BONECHEWER_TASKMASTER_FURY, Target_Self, 8, 0, 15);
	}
};

#define CN_BONECHEWER_WORKER					22963
#define BONECHEWER_WORKER_THROW_PICK			40844

class BonechewerWorkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(BonechewerWorkerAI, MoonScriptCreatureAI);
	BonechewerWorkerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BONECHEWER_WORKER_THROW_PICK, Target_RandomPlayer, 8, 0.5, 15);	// Current? Also shouldn't be cast once only?
	}
};

#define CN_CHARMING_COURTESAN					22955
#define CHARMING_COURTESAN_INFATUATION			41345
#define CHARMING_COURTESAN_POISONOUS_THROW		41346

class CharmingCourtesanAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CharmingCourtesanAI, MoonScriptCreatureAI);
	CharmingCourtesanAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CHARMING_COURTESAN_INFATUATION, Target_RandomPlayer, 7, 20, 40);
		AddSpell(CHARMING_COURTESAN_POISONOUS_THROW, Target_RandomPlayer, 8, 0, 20);
	}
};

#define CN_COILSKAR_GENERAL						22873
#define COILSKAR_GENERAL_BOOMING_VOICE			40080
#define COILSKAR_GENERAL_FREE_FRIEND			40081

class CoilskarGeneralAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarGeneralAI, MoonScriptCreatureAI);
	CoilskarGeneralAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_GENERAL_BOOMING_VOICE, Target_Self, 7, 0, 35);
		AddSpell(COILSKAR_GENERAL_FREE_FRIEND, Target_RandomFriendly, 7, 0, 20);	// SpellFunc to check incapacitating effects?
	}
};
// Is it Ranged unit throwing spears all the time?
#define CN_COILSKAR_HARPOONER					22874
#define COILSKAR_HARPOONER_HARPOONERS_MARK		40084
#define COILSKAR_HARPOONER_HOOKED_NET			40082
#define COILSKAR_HARPOONER_SPEAR_THROW			40083

class CoilskarHarpoonerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarHarpoonerAI, MoonScriptCreatureAI);
	CoilskarHarpoonerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_HARPOONER_HARPOONERS_MARK, Target_RandomPlayer, 7, 0, 35);		// I must link Dragon Turtle with Harpooner before scripting it
		AddSpell(COILSKAR_HARPOONER_HOOKED_NET, Target_RandomPlayer, 7, 0.5, 25);
		AddSpell(COILSKAR_HARPOONER_SPEAR_THROW, Target_Current, 8, 0, 15, 0, 40);			// Random? Also isn't it typical ranged unit? (using Spear Throw instead of Shoot/Shot spell?)
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		SpellDesc *pMark = FindSpellById(COILSKAR_HARPOONER_HARPOONERS_MARK);
		if (pMark != NULL)
		{
			CastSpellNowNoScheduling(pMark);
		}
	}
};

#define CN_COILSKAR_SEACALLER					22875
#define COILSKAR_SEACALLER_FORKED_LIGHTNING		40088
#define COILSKAR_SEACALLER_HURRICANE			40090
#define COILSKAR_SEACALLER_SUMMON_GEYSER		40091

class CoilskarSeacallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarSeacallerAI, MoonScriptCreatureAI);
	CoilskarSeacallerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_SEACALLER_FORKED_LIGHTNING, Target_Self, 8, 2, 20);
		AddSpell(COILSKAR_SEACALLER_HURRICANE, Target_RandomDestination, 8, 20, 35);
		AddSpell(COILSKAR_SEACALLER_SUMMON_GEYSER, Target_RandomPlayer, 7, 2, 25);			// Npc to add to DB + AI
	}
};

#define CN_COILSKAR_SOOTHSAYER					22876
#define COILSKAR_SOOTHSAYER_HOLY_NOVA			40096
#define COILSKAR_SOOTHSAYER_RESTORATION			40097

class CoilskarSoothsayerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarSoothsayerAI, MoonScriptCreatureAI);
	CoilskarSoothsayerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_SOOTHSAYER_HOLY_NOVA, Target_Self, 10, 0, 20);
		AddSpell(COILSKAR_SOOTHSAYER_RESTORATION, Target_WoundedFriendly, 8, 1.5, 35);
	}
};

#define CN_COILSKAR_WRANGLER					22877
#define COILSKAR_WRANGLER_CLEAVE				15284
#define COILSKAR_WRANGLER_ELECTRIC_SPUR			40076
#define COILSKAR_WRANGLER_LIGHTNING_PROD		40066

class CoilskarWranglerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CoilskarWranglerAI, MoonScriptCreatureAI);
	CoilskarWranglerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(COILSKAR_WRANGLER_CLEAVE, Target_Current, 10, 0, 20, 0, 10);
		AddSpell(COILSKAR_WRANGLER_ELECTRIC_SPUR, Target_Self, 8, 3, 45);					// Should fraize Leviathan, but it does not (core? or script? :<)
		AddSpell(COILSKAR_WRANGLER_LIGHTNING_PROD, Target_RandomPlayer, 8, 3, 25);
	}
};

#define CN_DRAGONMAW_SKY_STALKER				23030
#define DRAGONMAW_SKY_STALKER_IMMOLATION_ARROW	40872
#define DRAGONMAW_SKY_STALKER_SHOOT				40873

class DragonmawSkyStalkerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonmawSkyStalkerAI, MoonScriptCreatureAI);
	DragonmawSkyStalkerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGONMAW_SKY_STALKER_IMMOLATION_ARROW, Target_Current, 8, 2, 15, 0, 40);
		AddSpell(DRAGONMAW_SKY_STALKER_SHOOT, Target_Current, 75, 0, 1, 0, 40);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		UnitPointer pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};
// Should it run away from tank when he's close?
#define CN_DRAGONMAW_WIND_REAVER					23330
#define DRAGONMAW_WIND_REAVER_DOOM_BOLT				40876
#define DRAGONMAW_WIND_REAVER_FIREBALL				40877
#define DRAGONMAW_WIND_REAVER_FREEZE				40875

class DragonmawWindReaverAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonmawWindReaverAI, MoonScriptCreatureAI);
	DragonmawWindReaverAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGONMAW_WIND_REAVER_DOOM_BOLT, Target_Current, 10, 1.5, 15, 0, 40);
		AddSpell(DRAGONMAW_WIND_REAVER_FIREBALL, Target_Current, 75, 2, 0, 0, 40);
		AddSpell(DRAGONMAW_WIND_REAVER_FREEZE, Target_RandomPlayer, 10, 2, 15);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		UnitPointer pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}
};

#define CN_DRAGONMAW_WYRMCALLER					22960
#define DRAGONMAW_WYRMCALLER_CLEAVE				15284
#define DRAGONMAW_WYRMCALLER_FIXATE				40892
#define DRAGONMAW_WYRMCALLER_JAB				40895

class DragonmawWyrmcallerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DragonmawWyrmcallerAI, MoonScriptCreatureAI);
	DragonmawWyrmcallerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAGONMAW_WYRMCALLER_CLEAVE, Target_Current, 10, 0, 15, 0, 10);
		AddSpell(DRAGONMAW_WYRMCALLER_FIXATE, Target_RandomUnit, 7, 0, 20);
		AddSpell(DRAGONMAW_WYRMCALLER_JAB, Target_Current, 8, 0, 25, 0, 10);
	}
};

#define CN_ENSLAVED_SERVANT						22965
#define ENSLAVED_SERVANT_KIDNEY_SHOT			41389
#define ENSLAVED_SERVANT_UPPERCUT				41388

class EnslavedServantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EnslavedServantAI, MoonScriptCreatureAI);
	EnslavedServantAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ENSLAVED_SERVANT_KIDNEY_SHOT, Target_Current, 7, 0, 25, 0, 10);
		AddSpell(ENSLAVED_SERVANT_UPPERCUT, Target_RandomUnit, 8, 0, 20);

		mHealthResetTimer = -1;
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		mHealthResetTimer = AddTimer(45000);	// to check
	}

	void AIUpdate()
	{
		if (IsTimerFinished(mHealthResetTimer))
		{
			_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));	// Found such note about this mob
			ResetTimer(mHealthResetTimer, 45000);
		}

		ParentClass::AIUpdate();
	}

	int32	mHealthResetTimer;
};

#define CN_HAND_OF_GOREFIEND				23172
#define HAND_OF_GOREFIEND_FRENZY			38166

class HandOfGorefiendAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HandOfGorefiendAI, MoonScriptCreatureAI);
	HandOfGorefiendAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HAND_OF_GOREFIEND_FRENZY, Target_Self, 6, 0, 45);
	}
};
// Mechanics are guessed. I'm also not sure if it's not typical caster unit
#define CN_ILLIDARI_ARCHON					23400
#define ILLIDARI_ARCHON_HEAL				41372
#define ILLIDARI_ARCHON_HOLY_SMITE			41370
#define ILLIDARI_ARCHON_MIND_BLAST			41374
#define ILLIDARI_ARCHON_POWER_WORD_SHIELD	41373
#define ILLIDARI_ARCHON_SHADOW_WORD_DEATH	41375
#define ILLIDARI_ARCHON_SHADOWFORM			29406

class IllidariArchonAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariArchonAI, MoonScriptCreatureAI);
	IllidariArchonAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mIsShadowPriest = false;
		switch (RandomUInt(1))
		{
		case 0:
			AddSpell(ILLIDARI_ARCHON_HEAL, Target_WoundedFriendly, 5, 2, 30);
			AddSpell(ILLIDARI_ARCHON_HOLY_SMITE, Target_Current, 8, 2.5, 25, 0, 40);
			AddSpell(ILLIDARI_ARCHON_POWER_WORD_SHIELD, Target_Self, 7, 0, 35);
			break;
		case 1:
			AddSpell(ILLIDARI_ARCHON_MIND_BLAST, Target_Current, 8, 1.5, 25, 0, 30);
			AddSpell(ILLIDARI_ARCHON_SHADOW_WORD_DEATH, Target_RandomPlayer, 7, 0, 35);
			ApplyAura(ILLIDARI_ARCHON_SHADOWFORM);
			mIsShadowPriest = true;
			break;
		}
	}

	void OnCombatStop(UnitPointer pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (mIsShadowPriest && IsAlive())
		{
			ApplyAura(ILLIDARI_ARCHON_SHADOWFORM);
		}
	}

	void AIUpdate()
	{
		SpellDesc *pDeath = FindSpellById(ILLIDARI_ARCHON_SHADOW_WORD_DEATH);
		if (mIsShadowPriest && _unit->GetUInt32Value(UNIT_FIELD_HEALTH) <= 2500 && pDeath->mEnabled)
		{
			SpellPointer pCurrentSpell = _unit->GetCurrentSpell();
			if (pCurrentSpell != NULL && pCurrentSpell->pSpellId == pDeath->mInfo->Id)
			{
				pCurrentSpell->cancel();
			}
			for (SpellDescList::iterator itr = mQueuedSpells.begin(); itr != mQueuedSpells.end(); ++itr)
			{
				if ((*itr)->mInfo->Id == pDeath->mInfo->Id)
					mQueuedSpells.erase(itr);
			}
			for (SpellDescList::iterator itr = mScheduledSpells.begin(); itr != mScheduledSpells.end(); ++itr)
			{
				if ((*itr)->mInfo->Id == pDeath->mInfo->Id)
					mScheduledSpells.erase(itr);
			}

			pDeath->mEnabled = false;
		}

		ParentClass::AIUpdate();
	}

	bool	mIsShadowPriest;
};
// Couldn't find mechanics nowhere around the net, so kept it simple
#define CN_ILLIDARI_ASSASSIN				23403
#define ILLIDARI_ASSASSIN_PARALYZING_POISON	3609
#define ILLIDARI_ASSASSIN_VANISH			39667

class IllidariAssassinAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariAssassinAI, MoonScriptCreatureAI);
	IllidariAssassinAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_ASSASSIN_PARALYZING_POISON, Target_Current, 8, 0, 25, 0, 10);
		AddSpell(ILLIDARI_ASSASSIN_VANISH, Target_Self, 7, 1, 30);
	}

	void AIUpdate()
	{
		if (_unit->HasAura(ILLIDARI_ASSASSIN_VANISH))
		{
			DelayNextAttack(1500);
		}

		ParentClass::AIUpdate();
	}
};
// I've parted it on frost and fire mage - correct me if it's wrong (also slap me if it's typical caster)
#define CN_ILLIDARI_BATTLEMAGE					23402
#define ILLIDARI_BATTLEMAGE_BLIZZARD			41382
#define ILLIDARI_BATTLEMAGE_FIREBALL			41383
#define ILLIDARI_BATTLEMAGE_FLAMESTRIKE			41379
#define ILLIDARI_BATTLEMAGE_FROSTBOLT			41384

class IllidariBattlemageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariBattlemageAI, MoonScriptCreatureAI);
	IllidariBattlemageAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		switch (RandomUInt(1))
		{
		case 0:
			AddSpell(ILLIDARI_BATTLEMAGE_BLIZZARD, Target_RandomDestination, 8, 8, 35);
			AddSpell(ILLIDARI_BATTLEMAGE_FROSTBOLT, Target_Current, 15, 0, 10, 0, 40);
			break;
		case 1:
			AddSpell(ILLIDARI_BATTLEMAGE_FIREBALL, Target_Current, 15, 0, 10, 0, 40);
			AddSpell(ILLIDARI_BATTLEMAGE_FLAMESTRIKE, Target_RandomDestination, 8, 0, 40);
			break;
		}
	}
};

#define CN_ILLIDARI_BLOOD_LORD						23397
#define ILLIDARI_BLOOD_LORD_DIVINE_SHIELD			41367
#define ILLIDARI_BLOOD_LORD_HAMMER_OF_JUSTICE		13005
#define ILLIDARI_BLOOD_LORD_JUDGEMENT_OF_COMMAND	41368

class IllidariBloodLordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IllidariBloodLordAI, MoonScriptCreatureAI);
	IllidariBloodLordAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ILLIDARI_BLOOD_LORD_DIVINE_SHIELD, Target_Self, 8, 0, 30);
		AddSpell(ILLIDARI_BLOOD_LORD_HAMMER_OF_JUSTICE, Target_Current, 9, 0, 20, 0, 10);
		AddSpell(ILLIDARI_BLOOD_LORD_JUDGEMENT_OF_COMMAND, Target_Current, 8, 0, 25, 0, 10);
	}
};
// Should be summoned by Priestess of Dementia
#define CN_IMAGE_OF_DEMENTIA						23436
#define IMAGE_OF_DEMENTIA_WHRILWIND					41399
#define IMAGE_OF_DEMENTIA_WHIRLWIND2				41400

void SpellFunc_Whirlwind(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType);

class ImageOfDementiaAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ImageOfDementiaAI, MoonScriptCreatureAI);
	ImageOfDementiaAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mWhirlwind = AddSpell(IMAGE_OF_DEMENTIA_WHRILWIND, Target_Self, 0, 15, 0);
		AddSpellFunc(&SpellFunc_Whirlwind, Target_Self, 15, 15, 35, 0, 10);
	}

	SpellDesc*	mWhirlwind;
};

void SpellFunc_Whirlwind(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	ImageOfDementiaAI *Dementia = (pCreatureAI != NULL) ? (ImageOfDementiaAI*)pCreatureAI : NULL;
	if (Dementia != NULL)
	{
		Dementia->CastSpell(Dementia->mWhirlwind);
		Dementia->Despawn(25000);
	}
}

#define CN_SHADOWMOON_BLOOD_MAGE					22945
#define SHADOWMOON_BLOOD_MAGE_BLOOD_SIPHON			41068
#define SHADOWMOON_BLOOD_MAGE_BLOOD_BOLT			41072

class ShadowmoonBloodMageAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonBloodMageAI, MoonScriptCreatureAI);
	ShadowmoonBloodMageAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_BLOOD_MAGE_BLOOD_SIPHON, Target_Self, 8, 0, 35);
		AddSpell(SHADOWMOON_BLOOD_MAGE_BLOOD_BOLT, Target_RandomPlayer, 9, 3, 25);
	}
};

#define CN_SHADOWMOON_CHAMPION						22880
#define SHADOWMOON_CHAMPION_CHAOTIC_LIGHT			41063
#define SHADOWMOON_CHAMPION_WHIRLING_BLADE			41053

class ShadowmoonChampionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonChampionAI, MoonScriptCreatureAI);
	ShadowmoonChampionAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_CHAMPION_CHAOTIC_LIGHT, Target_Current, 8, 0, 25, 0, 10);
		AddSpell(SHADOWMOON_CHAMPION_WHIRLING_BLADE, Target_RandomPlayer, 8, 1, 30);	// I must check its mechanics
	}
};

#define CN_SHADOWMOON_DEATHSHAPER					22882
#define SHADOWMOON_DEATHSHAPER_DEATH_COIL			41070
#define SHADOWMOON_DEATHSHAPER_DEMON_ARMOR			13787
#define SHADOWMOON_DEATHSHAPER_RAISE_DEAD			41071
#define SHADOWMOON_DEATHSHAPER_SHADOW_BOLT			41069

void SpellFunc_RaiseDead(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType);

class ShadowmoonDeathshaperAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonDeathshaperAI, MoonScriptCreatureAI);
	ShadowmoonDeathshaperAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_DEATHSHAPER_DEATH_COIL, Target_RandomPlayer, 7, 0, 30);
		AddSpell(SHADOWMOON_DEATHSHAPER_DEMON_ARMOR, Target_Self, 8, 0, 60);
		mRaiseDead = AddSpell(SHADOWMOON_DEATHSHAPER_RAISE_DEAD, Target_Self, 0, 1.5, 0);
//		AddSpellFunc(&SpellFunc_RaiseDead, Target_RandomCorpse, 12, 1.5, 30);
		AddSpell(SHADOWMOON_DEATHSHAPER_SHADOW_BOLT, Target_Current, 75, 3, 0, 0, 40);	// Typical caster? Slap me if not...

		_unit->SetUInt32Value(UNIT_FIELD_POWER1, 100000);	// temporary way to set up mana of this unit
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 40.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(false);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();

		UnitPointer pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 40.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(false);
			}
		}
	}

	SpellDesc*	mRaiseDead;
};

void SpellFunc_RaiseDead(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	ShadowmoonDeathshaperAI *pDeathshaper = (pCreatureAI != NULL) ? (ShadowmoonDeathshaperAI*)pCreatureAI : NULL;
	if (pDeathshaper != NULL)
	{
		pDeathshaper->CastSpell(pDeathshaper->mRaiseDead);
		MoonScriptCreatureAI *pAI = pDeathshaper->SpawnCreature(23371, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), pTarget->GetOrientation());
		if (pAI != NULL)
		{
			pAI->GetUnit()->GetAIInterface()->StopMovement(2500);
			pAI->DelayNextAttack(2500);
		}

		TO_CREATURE(pTarget)->Despawn(3000, 0);
	}
}
// No entry in my DB, but AI may stay :)
#define CN_SHADOWMOON_FALLEN					23371

class ShadowmoonFallenAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonFallenAI, MoonScriptCreatureAI);
	ShadowmoonFallenAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AggroNearestUnit(200);
	}
};

#define CN_SHADOWMOON_HOUNDMASTER						23018
#define SHADOWMOON_HOUNDMASTER_FLARE					41094
#define SHADOWMOON_HOUNDMASTER_FREEZING_TRAP			41085
#define SHADOWMOON_HOUNDMASTER_SHOOT					41093
#define SHADOWMOON_HOUNDMASTER_SILENCING_SHOT			41084
#define SHADOWMOON_HOUNDMASTER_SUMMON_RIDING_WARHOUND	39906
#define SHADOWMOON_HOUNDMASTER_VOLLEY					41091
#define SHADOWMOON_HOUNDMASTER_WING_CLIP				32908

class ShadowmoonHoundmasterAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonHoundmasterAI, MoonScriptCreatureAI);
	ShadowmoonHoundmasterAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_HOUNDMASTER_FLARE, Target_RandomDestination, 5, 0, 30);
		AddSpell(SHADOWMOON_HOUNDMASTER_FREEZING_TRAP, Target_Self, 5, 0, 30);
		AddSpell(SHADOWMOON_HOUNDMASTER_SHOOT, Target_Current, 70, 0, 1, 0, 30);
		AddSpell(SHADOWMOON_HOUNDMASTER_SILENCING_SHOT, Target_RandomPlayer, 6, 0, 35);
		AddSpell(SHADOWMOON_HOUNDMASTER_SUMMON_RIDING_WARHOUND, Target_Self, 5, 0, 45);
		AddSpell(SHADOWMOON_HOUNDMASTER_VOLLEY, Target_RandomDestination, 5, 1, 25);
		AddSpell(SHADOWMOON_HOUNDMASTER_WING_CLIP, Target_Current, 5, 0, 20, 0, 10, true);
	}

	void OnCombatStart(UnitPointer pTarget)
	{
		ParentClass::OnCombatStart(pTarget);
		if (GetRangeToUnit(pTarget) <= 30.0f)
		{
			SetBehavior(Behavior_Spell);
			SetCanMove(true);
		}
	}

	void AIUpdate()
	{
		ParentClass::AIUpdate();
		UnitPointer pTarget = _unit->GetAIInterface()->GetNextTarget();
		if (pTarget != NULL)
		{
			if (GetRangeToUnit(pTarget) <= 30.0f)
			{
				SetBehavior(Behavior_Spell);
				SetCanMove(true);
			}
		}
	}
};

#define CN_SHADOWMOON_REAVER					22879
#define SHADOWMOON_REAVER_SPELL_ABSORPTION		41034

class ShadowmoonReaverAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonReaverAI, MoonScriptCreatureAI);
	ShadowmoonReaverAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_REAVER_SPELL_ABSORPTION, Target_Self, 8, 0, 35);
	}
};

#define CN_SHADOWMOON_SOLDIER					23047
#define SHADOWMOON_SOLDIER_STRIKE				11976

class ShadowmoonSoldierAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonSoldierAI, MoonScriptCreatureAI);
	ShadowmoonSoldierAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWMOON_SOLDIER_STRIKE, Target_Current, 10, 0, 15, 0, 10);
	}
};
// Haven't found informations about Shield Wall ability
#define CN_SHADOWMOON_WEAPON_MASTER					23049
#define SHADOWMOON_WEAPON_MASTER_BATTLE_AURA		41106
#define SHADOWMOON_WEAPON_MASTER_BATTLE_STANCE		41099
#define SHADOWMOON_WEAPON_MASTER_BERSEKER_AURA		41107
#define SHADOWMOON_WEAPON_MASTER_BERSERKER_STANCE	41100
#define SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA		41105
#define SHADOWMOON_WEAPON_MASTER_DEFENSIVE_STANCE	41101
#define SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY			18813
#define SHADOWMOON_WEAPON_MASTER_MUTILATE			41103
#define SHADOWMOON_WEAPON_MASTER_WHIRLWIND			41097
#define SHADOWMOON_WEAPON_MASTER_WHIRLWIND2			41098
#define SHADOWMOON_WEAPON_MASTER_SHIELD_WALL		41104	// guessed, but close Id :)

void SpellFunc_DefensiveStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType);
void SpellFunc_BerserkerStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType);

class ShadowmoonWeaponMasterAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowmoonWeaponMasterAI, MoonScriptBossAI);
	ShadowmoonWeaponMasterAI(CreaturePointer pCreature) : MoonScriptBossAI(pCreature)
	{
		AddPhaseSpell(1, AddSpell(SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY, Target_Current, 9, 0, 25, 0, 10));
		AddPhaseSpell(1, AddSpell(SHADOWMOON_WEAPON_MASTER_MUTILATE, Target_Current, 8, 0, 30, 0, 10));
		AddPhaseSpell(2, AddSpell(SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY, Target_Current, 9, 0, 25, 0, 10));
		AddPhaseSpell(2, AddSpell(SHADOWMOON_WEAPON_MASTER_SHIELD_WALL, Target_Self, 10, 0, 35));
		AddPhaseSpell(3, AddSpell(SHADOWMOON_WEAPON_MASTER_KNOCK_AWAY, Target_Current, 9, 0, 25, 0, 10));
		AddPhaseSpell(3, AddSpell(SHADOWMOON_WEAPON_MASTER_WHIRLWIND, Target_Self, 10, 15, 35));
		AddPhaseSpell(3, AddSpell(SHADOWMOON_WEAPON_MASTER_MUTILATE, Target_Current, 8, 0, 30, 0, 10));
		mDefensiveStance = AddSpellFunc(&SpellFunc_DefensiveStance, Target_Self, 0, 0, 0);
		mBerserkerStance = AddSpellFunc(&SpellFunc_BerserkerStance, Target_Self, 0, 0, 0);
		//SetDisplayWeaponIds(0, 0, 0, 0, 0, 0)	// Sword
		ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_STANCE);
		ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_AURA);
	}

	void OnCombatStop(UnitPointer pTarget)
	{
		ParentClass::OnCombatStop(pTarget);

		if (IsAlive())
		{
			RemoveAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA);
			RemoveAura(SHADOWMOON_WEAPON_MASTER_BERSEKER_AURA);
			//SetDisplayWeaponIds(0, 0, 0, 0, 0, 0)	// Sword
			ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_STANCE);
			ApplyAura(SHADOWMOON_WEAPON_MASTER_BATTLE_AURA);
		}
	}

	void AIUpdate()
	{
		if (GetPhase() == 1 && GetHealthPercent() <= 85)
		{
			SetPhase(2, mDefensiveStance);
			return;
		}
		if (GetPhase() == 2 && GetHealthPercent() <= 35)
		{
			SetPhase(3, mBerserkerStance);
			return;
		}

		ParentClass::AIUpdate();
	}

	SpellDesc*	mDefensiveStance;
	SpellDesc*	mBerserkerStance;
};

void SpellFunc_DefensiveStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	ShadowmoonWeaponMasterAI *pWeaponMaster = (pCreatureAI != NULL) ? (ShadowmoonWeaponMasterAI*)pCreatureAI : NULL;
	if (pWeaponMaster != NULL)
	{
		pWeaponMaster->RemoveAura(SHADOWMOON_WEAPON_MASTER_BATTLE_AURA);
		//SetDisplayWeaponIds(0, 0, 0, 0, 0, 0)	// Axe + Shield
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_STANCE);
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA);
	}
}

void SpellFunc_BerserkerStance(SpellDesc *pThis, MoonScriptCreatureAI *pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	ShadowmoonWeaponMasterAI *pWeaponMaster = (pCreatureAI != NULL) ? (ShadowmoonWeaponMasterAI*)pCreatureAI : NULL;
	if (pWeaponMaster != NULL)
	{
		pWeaponMaster->RemoveAura(SHADOWMOON_WEAPON_MASTER_DEFENSIVE_AURA);
		//SetDisplayWeaponIds(0, 0, 0, 0, 0, 0)	// Sword
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_BERSERKER_STANCE);
		pWeaponMaster->ApplyAura(SHADOWMOON_WEAPON_MASTER_BERSEKER_AURA);
		pWeaponMaster->Emote("Berserker stance! Attack them recklessly!", Text_Say, 0);
	}
}

#define CN_SPELLBOUND_ATTENDANT						22959
#define SPELLBOUND_ATTENDANT_KICK					41395
#define SPELLBOUND_ATTENDANT_SLEEP					41396

class SpellboundAttendantAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SpellboundAttendantAI, MoonScriptCreatureAI);
	SpellboundAttendantAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SPELLBOUND_ATTENDANT_KICK, Target_Current, 9, 0, 25, 0, 10);
		AddSpell(SPELLBOUND_ATTENDANT_SLEEP, Target_RandomPlayerNotCurrent, 8, 1, 25);
	}
};

#define CN_TEMPLE_CONCUBINE							22939
#define TEMPLE_CONCUBINE_LOVE_TAP					41338
#define TEMPLE_CONCUBINE_POLYMORPH					41334

class TempleConcubineAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(TempleConcubineAI, MoonScriptCreatureAI);
	TempleConcubineAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(TEMPLE_CONCUBINE_LOVE_TAP, Target_Current, 9, 0, 25, 0, 10);
		AddSpell(TEMPLE_CONCUBINE_POLYMORPH, Target_RandomPlayerNotCurrent, 7, 1, 25);
	}
};

//////////////////////////////////////////////////////////////////////////
/////// Elemental AIs

#define CN_STORM_FURY				22848
#define STORM_FURY_STORM_BLINK		39581

void SpellFunc_StormBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType);

class StormFuryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(StormFuryAI, MoonScriptCreatureAI);
	StormFuryAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		mStormBlink = AddSpell(STORM_FURY_STORM_BLINK, Target_Self, 0, 1, 0);	// Mechanics was guessed
		AddSpellFunc(&SpellFunc_StormBlink, Target_RandomPlayerNotCurrent, 8, 0, 35);
	}

	void AIUpdate()
	{
		if (_unit->HasAura(STORM_FURY_STORM_BLINK))
		{
			DelayNextAttack(2000);
		}

		ParentClass::AIUpdate();
	}

	SpellDesc*	mStormBlink;
};

void SpellFunc_StormBlink(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	StormFuryAI *pStormFuryAI = (pCreatureAI != NULL) ? (StormFuryAI*)pCreatureAI : NULL;
	if (pStormFuryAI != NULL)
	{
		UnitPointer pCurrentTarget = pStormFuryAI->GetUnit()->GetAIInterface()->GetNextTarget();
		if (pCurrentTarget == NULL)
			return;

		if (pTarget == pCurrentTarget)
			return;

		pStormFuryAI->ClearHateList();
		pStormFuryAI->GetUnit()->GetAIInterface()->AttackReaction(pTarget, 500);
		pStormFuryAI->GetUnit()->GetAIInterface()->SetNextTarget(pTarget);
		pStormFuryAI->GetUnit()->GetAIInterface()->RemoveThreatByPtr(pCurrentTarget);
		pStormFuryAI->CastSpell(pStormFuryAI->mStormBlink);
	}
}

#define CN_AQUEOUS_SURGER					22881
#define AQUEOUS_SURGER_POISON_BOLT_VOLLEY	40095

class AqueousSurgerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AqueousSurgerAI, MoonScriptCreatureAI);
	AqueousSurgerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AQUEOUS_SURGER_POISON_BOLT_VOLLEY, Target_Self, 7, 0, 20);
	}
};

#define CN_AQUEOUS_SPAWN				22883
#define AQUEOUS_SPAWN_MERGE				40106
#define AQUEOUS_SPAWN_SLUDGE_NOVA		40102

class AqueousSpawnAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AqueousSpawnAI, MoonScriptCreatureAI);
	AqueousSpawnAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AQUEOUS_SPAWN_MERGE, Target_Self, 7, 11, 45);
		AddSpell(AQUEOUS_SPAWN_SLUDGE_NOVA, Target_RandomPlayer, 8, 2.5, 20);
	}
};

#define CN_AQUEOUS_LORD					22878
#define AQUEOUS_LORD_CRASHING_WAVE		40100
#define AQUEOUS_LORD_VILE_SLIME			40099

void SpellFunc_SpawnAqueousSpawn(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType);

class AqueousLordAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AqueousLordAI, MoonScriptCreatureAI);
	AqueousLordAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(AQUEOUS_LORD_CRASHING_WAVE, Target_Self, 8, 0, 35);
		AddSpell(AQUEOUS_LORD_VILE_SLIME, Target_RandomPlayer, 10, 0, 25);
		AddSpellFunc(&SpellFunc_SpawnAqueousSpawn, Target_Self, 10, 0, 30);
	}
};

void SpellFunc_SpawnAqueousSpawn(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, UnitPointer pTarget, TargetType pType)
{
	AqueousLordAI *pAqueousLordAI = (pCreatureAI != NULL) ? (AqueousLordAI*)pCreatureAI : NULL;
	if (pAqueousLordAI != NULL)
	{
		MoonScriptCreatureAI *pSpawnAI = pAqueousLordAI->SpawnCreature(CN_AQUEOUS_SPAWN);
		if (pSpawnAI != NULL)
		{
			pSpawnAI->AggroRandomUnit(500);
			pSpawnAI->SetDespawnWhenInactive(true);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/////// Mechanical AIs

#define CN_PROMENADE_SENTINEL				23394
#define PROMENADE_SENTINEL_L5_ARCANE_CHARGE	41360

class PromenadeSentinelAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(PromenadeSentinelAI, MoonScriptCreatureAI);
	PromenadeSentinelAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(PROMENADE_SENTINEL_L5_ARCANE_CHARGE, Target_RandomPlayer, 8, 2.7f, 35);
	}
};

//////////////////////////////////////////////////////////////////////////
/////// Uncategorized AIs

#define CN_ANGERED_SOUL_FRAGMENT			23398
#define ANGERED_SOUL_FRAGMENT_ANGER			41986

class AngeredSoulFragmentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AngeredSoulFragmentAI, MoonScriptCreatureAI);
	AngeredSoulFragmentAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ANGERED_SOUL_FRAGMENT_ANGER, Target_Self, 10, 3, 25);
	}
};

#define CN_ASHTONGUE_FERAL_SPIRIT				22849
#define ASHTONGUE_FERAL_SPIRIT_SPIRIT_BOND		39578	// won't be used for now
#define ASHTONGUE_FERAL_SPIRIT_CHARGE_FRENZY	39575	// won't be added for now (can cause crashes - self-kills)

class AshtongueFeralSpiritAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AshtongueFeralSpiritAI, MoonScriptCreatureAI);
	AshtongueFeralSpiritAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
	}
};

//////////////////////////////////////////////////////////////////////////USADO EN SCRIPT DE RELIQUARY OF SOUL
/////// Undead AIs
// No NPC in DB - could not check it
/* #define CN_ENSLAVED_SOUL				23469
#define ENSLAVED_SOUL_SOUL_RELEASE		41542

class EnslavedSoulAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EnslavedSoulAI, MoonScriptCreatureAI);
	EnslavedSoulAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature) {}

	void OnDied(UnitPointer mKiller)
	{
		ApplyAura(ENSLAVED_SOUL_SOUL_RELEASE);			// beg core to support OnDied casts
		ParentClass::OnDied(mKiller);
	}
}; */

#define CN_HUNGERING_SOUL_FRAGMENT					23401
#define HUNGERING_SOUL_FRAGMENT_CONSUMING_STRIKES	41248

class HungeringSoulFragmentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HungeringSoulFragmentAI, MoonScriptCreatureAI);
	HungeringSoulFragmentAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		ApplyAura(HUNGERING_SOUL_FRAGMENT_CONSUMING_STRIKES);
	}

	void OnCombatStop(UnitPointer pTarget)
	{
		ParentClass::OnCombatStop(pTarget);
		if (IsAlive())
		{
			ApplyAura(HUNGERING_SOUL_FRAGMENT_CONSUMING_STRIKES);
		}
	}
};

#define CN_SHADOWY_CONSTRUCT					23111
#define SHADOWY_CONSTRUCT_ATROPHY				40327

class ShadowyConstructAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ShadowyConstructAI, MoonScriptCreatureAI);
	ShadowyConstructAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHADOWY_CONSTRUCT_ATROPHY, Target_Current, 10, 0, 45, 0, 10);
	}
};

#define CN_SUFFERING_SOUL_FRAGMENT				23999
#define SUFFERING_SOUL_FRAGMENT_SOUL_BLAST		41245

class SufferingSoulFragmentAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SufferingSoulFragmentAI, MoonScriptCreatureAI);
	SufferingSoulFragmentAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SUFFERING_SOUL_FRAGMENT_SOUL_BLAST, Target_Self, 8, 2, 30);
	}
};
// Teron Gorefiend transforms random player into it with those abilities, but AI might be handy too (too overpowered?) - cannot check (no npc in DB)
#define CN_VANGEFUL_SPIRIT					23109
#define VANGEFUL_SPIRIT_SPIRIT_CHAINS		40175
#define VANGEFUL_SPIRIT_SPIRIT_LANCE		40157
#define VANGEFUL_SPIRIT_SPIRIT_SHIELD		40322
#define VANGEFUL_SPIRIT_SPIRIT_STRIKE		40325
#define VANGEFUL_SPIRIT_SPIRIT_VOLLEY		40314

class VangefulSpiritAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(VangefulSpiritAI, MoonScriptCreatureAI);
	VangefulSpiritAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_CHAINS, Target_Self, 8, 0, 25);
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_LANCE, Target_RandomPlayer, 6, 0, 35);	// Current?
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_SHIELD, Target_Self, 6, 0, 50);
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_STRIKE, Target_Current, 9, 0, 15, 0, 10);
		AddSpell(VANGEFUL_SPIRIT_SPIRIT_VOLLEY, Target_Self, 4, 0, 40);
	}
};

#define CN_WRATHBONE_FLAYER				22953
#define WRATHBONE_FLAYER_CLEAVE			15496
#define WRATHBONE_FLAYER_IGNORED		39544

class WrathboneFlayerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(WrathboneFlayerAI, MoonScriptCreatureAI);
	WrathboneFlayerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WRATHBONE_FLAYER_CLEAVE, Target_Current, 10, 0, 15, 0, 10);
		AddSpell(WRATHBONE_FLAYER_IGNORED, Target_Current, 7, 0, 25, 0, 10);		// Current or random?
	}
};

////////////////////////////////////////////////
////////////////////////////////////////////////
//----------------------------------------------------------------------//
//----------------------High Warlord Naj'entus--------------------------//
//----------------------------------------------------------------------//

#define CN_NAJENTUS 22887

#define CRASHING_WAVE					40100	// Crashing Wave (2313-2687 Nature damage)
#define NEEDLE_SPINE					39835	// Needle Spine (3188-4312 Damage, AoE of 2375-2625 Aura): Fires a needle spine at an enemy target.
#define NEEDLE_SPINE_EXPLOSION			39968
#define GREATHER_PW_OF_SHIELD			33147//Greather power of shield, hasta encontrar la forma de dispelear el tidal	// Tidal Shield : Impervious to normal attack and spells. Regenerating health. Will deal 8500 frost damage to the raid when broken.
#define IMPALING_SPINE					39837 //
#define NAJENTUS_SPINE_GO				185584	//agregar a db... ^^

class NajentusAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(NajentusAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    NajentusAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 4;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(CRASHING_WAVE);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 100.0f;

		spells[1].info = dbcSpell.LookupEntry(NEEDLE_SPINE);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 10;
		spells[1].minhp2cast = 0;
		spells[1].maxhp2cast = 100;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 60.0f;

		spells[2].info = dbcSpell.LookupEntry(GREATHER_PW_OF_SHIELD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 60;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 100.0f;

		spells[3].info = dbcSpell.LookupEntry(IMPALING_SPINE);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 22;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 60.0f;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You will die, in the name of Lady Vashj!");
		_unit->PlaySoundToSet(11450);

		enrage_timer = 0;

		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		
		enrage_timer = 0;
		if( _unit->HasAura(19953) )
			_unit->RemoveAura(19953);

       RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan will... crush you.");
		_unit->PlaySoundToSet(11459);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		switch (RandomUInt(2))
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time for you to go!");
			_unit->PlaySoundToSet(11456);
			break;
		default:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Your success was short lived!");	// not sure
				_unit->PlaySoundToSet(11455);
			}
		}
	}

	void AIUpdate()
	{
		//Tidal Shield
		if ( spells[2].cooldown == 0 )
		{
			_unit->setAttackTimer(spells[2].attackstoptimer, false);

			_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

			spells[2].cooldown = 60;
		}

		//Impaling Spine
		if ( spells[3].cooldown == 0 )
		{
			switch (RandomUInt(2))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stick around!");
				_unit->PlaySoundToSet(11451);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll deal with you later!"); 
					_unit->PlaySoundToSet(11452);
				}
			}
			_unit->setAttackTimer(spells[3].attackstoptimer, false);

			CastSpellOnRandomTarget(3, spells[3].mindist2cast, spells[3].maxdist2cast, 0, 100);

			spells[3].cooldown = 22;
		}

		if( enrage_timer >= 900 )
			if( !_unit->HasAura(26662) )
				_unit->CastSpell(_unit, 26662, true);

		spells[2].cooldown--;
		spells[3].cooldown--;
		enrage_timer++;

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
							//_unit->GetMapMgr()->GetInterface()->SpawnGameObject(NAJENTUS_SPINE_GO , target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0.207343, true, 0, 0); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if( (*itr)->IsUnit() &&
					(spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr)) || 
					spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)))
					)
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);
					if( RandomTarget && RandomTarget->isAlive() && 
						_unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast &&
                        _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && 
						RandomTarget->GetHealthPct() >= minhp2cast && 
						RandomTarget->GetHealthPct() <= maxhp2cast
						)
					{
						TargetTable.push_back(RandomTarget);
					}
				} 
			}
			if(_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if(!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			//Impaling Spine 
			if( i == 3 )
			{
				GameObjectPointer pGO = _unit->GetMapMgr()->GetInterface()->SpawnGameObject(NAJENTUS_SPINE_GO, RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), RTarget->GetOrientation(), true, 0, 0);
				if( pGO )
					pGO->SetFloatValue(OBJECT_FIELD_SCALE_X,5.0f);
			}
			//Visual para Needle Spine =))
			if( i == 1 )
				RTarget->CastSpell(RTarget, dbcSpell.LookupEntry(NEEDLE_SPINE_EXPLOSION), true);

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
	int enrage_timer;
};


//------------------------------------
//          -= Supremus =-
//------------------------------------
struct Posxyzo 
{
	float x;
	float y;
};

struct Posxyzo VolcanoSpawn[]=
{
	{726.579102f, 703.755066f},
	{747.086853f, 675.635254f},
    {720.506714f, 688.728271f},
    {683.769043f, 681.010193f},
	{685.349976f, 706.817017f},
    {720.297058f, 730.904907f},
    {692.916016f, 729.032776f},
	{679.330688f, 681.300049f},
	{732.274170f, 626.551758f},//plataformas para evitar bugs
	{721.762207f, 614.583740f},
	{704.446960f, 621.973206f},
	{690.183289f, 618.849731f},
	{672.611633f, 615.592712f},
	{702.448792f, 578.278564f}
};


struct TargetHealthOrder 
{
  bool operator() (UnitPointer target1, UnitPointer target2) { (target1->GetUInt32Value(UNIT_FIELD_HEALTH) < target2->GetUInt32Value(UNIT_FIELD_HEALTH));}
} 
HealthOrder;
//////////////////////////////////////
//////////////////////////////////////
//------------------------------------
//-= Supremus by soulshifter 
//       no 100% blizz =-
//------------------------------------
//////////////////////////////////////

#define CN_SUPREMUS 22898

//Spells

#define HATEFUL_STRIKE			41926
#define DEMON_FIRE				40029
#define MOLTEN_FLAME			40980 
#define MOLTEN_FLAME2			32492//40265
#define VOLCANIC_ERUPTION		40276
#define VOLCANIC_FIREBALL		40117
#define VOLCANIC_GEYSER			42055
#define MOLTEN_PUNCH			28405 //40126
#define BERSERK					45078

// Triggers
#define CN_VOLCANO				23085
#define CN_PUNCH				23095 // nor used. 40126
#define CN_MOLTEN				90005
#define GO_GATE					185882 // Cambiar id

class SupremusAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SupremusAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    SupremusAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		// -- Number of spells to add --
		nrspells = 3;
		BerserkOn = false;
		_unit->GetAIInterface()->setOutOfCombatRange(30000);
		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(703.891f, 544.47f, 112.654f, GO_GATE);
		if(Gate)
			Gate->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

		// --- Initialization ---
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HATEFUL_STRIKE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000; // 1sec
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 15.0f;

		spells[1].info = dbcSpell.LookupEntry(DEMON_FIRE);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000; // 2sec
		
		spells[2].info = dbcSpell.LookupEntry(MOLTEN_PUNCH);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 8000; // 1sec
		
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		MoltenFlameTimer = 20;
		PhaseTimer = 120;
		BerserkTimer = 900;
		RegisterAIUpdateEvent(1000);
    }

	void OnCombatStop(UnitPointer mTarget)
    {
		Reset();
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		if(Gate)
		{
			Gate->SetByte(GAMEOBJECT_BYTES_1, 0, 0); // abierta
		}
		Reset();
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
	}

	void AIUpdate()
	{
		if(!Gate)
			Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(703.891f, 544.47f, 112.654f, GO_GATE);
		if(Gate)
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Hay Gate");
		PhaseTimer--;
		if(PhaseTimer >= 60)
		{
			Phase1();
		}
		if(PhaseTimer < 60)
		{
			Phase2();
		}
		if(!BerserkTimer && !BerserkOn)
		{
			BerserkOn = true;
			_unit->CastSpell(_unit, BERSERK, true);
		}
		if(!PhaseTimer)
			PhaseTimer = 120;
		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}
	void Phase1()
	{
		if(!MoltenFlameTimer)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MOLTEN, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
			MoltenFlameTimer = 20;
		}
		GazeTimer = 10;
		TimerVolcanos = 15 + RandomUInt(20);
		MoltenFlameTimer--;
	}
	void Phase2()
	{
		if(!TimerVolcanos)
			SummonVolcanos();
			
		if(!GazeTimer)
			Gaze();
		
		GazeTimer--;
		TimerVolcanos--;
		MoltenFlameTimer = 20;
	}	
	void SummonVolcanos()
	{
		int Volcanos;
		Volcanos = 4 + rand()%4;
		for(int i= 0; i < Volcanos; i++)
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_VOLCANO, (VolcanoSpawn[i].x + RandomFloat(20)), (VolcanoSpawn[i].y + RandomFloat(20)), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
		}
		for(int f= 8; f < 14; f++)//Plataforma para evitar bugs
		{
			_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_VOLCANO, VolcanoSpawn[f].x, VolcanoSpawn[f].y, 120.706291f, _unit->GetOrientation(), true, false, 0, 0);
		}
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_VOLCANO, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
		switch(rand()%2)
		{
		case 0:
			_unit->SendChatMessage(CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, "The ground begins to crack open!.");
			break;
		case 1:
			_unit->SendChatMessage(CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, "Supremus punches the ground in anger!.");
			break;
		}
		TimerVolcanos = 20 + RandomUInt(20);
	}
	void Gaze()
	{
		_unit->GetAIInterface()->ClearHateList();
		_unit->GetAIInterface()->WipeHateList();
		UnitPointer RandomTarget = NULLUNIT;
		std::vector<UnitPointer> TargetTable;
		for(unordered_set<PlayerPointer>::iterator itr = _unit->GetInRangePlayerSetBegin(); itr != _unit->GetInRangePlayerSetEnd(); ++itr) 
		{ 
			if ((*itr)->GetTypeId() == TYPEID_PLAYER && (*itr)->GetInstanceID() == _unit->GetInstanceID() && (*itr)->GetDistance2dSq(_unit) > 225) //15 yards
			{
				RandomTarget = TO_UNIT(*itr);

				if (RandomTarget && RandomTarget->isAlive())
					TargetTable.push_back(RandomTarget);
			}
		}

		//if there isn't any target, take the tank
		if(!TargetTable.size())
			TargetTable.push_back(_unit->GetAIInterface()->GetNextTarget());

		RandomTarget = *(TargetTable.begin()+rand()%TargetTable.size());
		if(RandomTarget)
		{
			_unit->GetAIInterface()->AttackReaction(RandomTarget, 100000000);
			_unit->SendChatMessage(CHAT_MSG_RAID_BOSS_EMOTE, LANG_UNIVERSAL, "Supremus acquires a new target!.");
			if(RandomTarget && _unit->GetDistance2dSq(RandomTarget) >= 1600)
			{
				_unit->GetAIInterface()->m_moveSprint = true;
				if(RandomTarget && _unit->GetDistance2dSq(RandomTarget) <= 400)
					_unit->GetAIInterface()->m_moveSprint = false;
			}
			else
			{
				_unit->CastSpell(RandomTarget, MOLTEN_PUNCH, true);
				_unit->CastSpell(RandomTarget, MOLTEN_PUNCH, true);
			}
			
		}
		GazeTimer = 10;
	}
    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid);
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::list<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if(!TargetTable.size())
				TargetTable.push_back(_unit->GetAIInterface()->GetNextTarget()); // Attack The Tank

			TargetTable.sort(HealthOrder);
			TargetTable.resize(2);

			UnitPointer RTarget = *(TargetTable.end());

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant);
				break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void Reset()
	{
		//Gate = NULL;
		BerserkTimer = 900;
		MoltenFlameTimer = 20;
		GazeTimer = 10;
		TimerVolcanos = 15 + RandomUInt(20);
		PhaseTimer = 120;
	}
protected:
	GameObjectPointer Gate;
	uint32 BerserkTimer;
	uint32 MoltenFlameTimer;
	uint32 GazeTimer;
	uint32 TimerVolcanos;
	uint32 PhaseTimer;
	int nrspells;
	bool BerserkOn;
};

// Volcano Trigger

class VolcanoAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VolcanoAI);

    VolcanoAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->CastSpell(_unit, VOLCANIC_FIREBALL, true);
		_unit->CastSpell(_unit, VOLCANIC_GEYSER, true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;
		RegisterAIUpdateEvent(1000);
		DespawnTimer = 20;
	}
	
	void AIUpdate()
	{
		DespawnTimer--;
		if(!DespawnTimer)
			_unit->Despawn(1,0);
	}

protected:
	uint32 DespawnTimer;
};

// Molten Flame Trigger AI

class MoltenFlameAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MoltenFlameAI);

    MoltenFlameAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->CastSpell(_unit, MOLTEN_FLAME, true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_moveRun = true;
		_unit->m_noRespawn = true;
		RegisterAIUpdateEvent(1000);
		DespawnTimer = 20;
	}

	void AIUpdate()
	{
		_unit->CastSpell(_unit, MOLTEN_FLAME2, true);
		if(!_unit->GetAIInterface()->getUnitToFollow())
		{
			UnitPointer NewTarget = NULLUNIT;
			NewTarget = FindTarget();
			if (NewTarget)
			{
				_unit->GetAIInterface()->SetUnitToFollow(NewTarget);
			}
		}
		if (!DespawnTimer)
		{
			_unit->Despawn(1,0);
		}
		DespawnTimer--;
	}
	// A bit rewritten FindTarget function
	UnitPointer FindTarget()
	{
		UnitPointer target = NULLUNIT;
		float distance = 60.0f;
		float z_diff;

		UnitPointer pUnit;
		float dist;

		for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			z_diff = fabs(_unit->GetPositionZ() - pUnit->GetPositionZ());
			if(z_diff > 2.5f)
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			if (dist < 3.0f)
				continue;

			distance = dist;
			target = pUnit;
		}

		return target;
	}

protected:
	uint32 DespawnTimer;
};


//-------------------------Gurtogg Bloodboil----------------------------//

#define CN_GURTOGG_BLOODBOIL 22948

// Phase 1
#define BLOODBOIL		42005
#define ACIDIC_WOUND	40481
#define DISORIENT		32752
#define KNOCKBACK		38576

// Phase 2
#define FEL_RAGE1		40604
#define FEL_RAGE2		40594	// 40616 - doesn't work at all
#define ACID_GEYSER		40629

// Spells common for both phases
#define ARCING_SMASH	40599
#define FEL_ACID_BREATH	40595

class GurtoggAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GurtoggAI);
	SP_AI_Spell spells[9];
	bool m_spellcheck[9];

    GurtoggAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 5;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ARCING_SMASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;

		spells[1].info = dbcSpell.LookupEntry(FEL_ACID_BREATH);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;

		// Phase 1
		spells[2].info = dbcSpell.LookupEntry(BLOODBOIL);
		spells[2].targettype = TARGET_VARIOUS;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 10;

		// Not sure about this one system of casting
		spells[3].info = dbcSpell.LookupEntry(ACIDIC_WOUND);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].perctrigger = 10.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 15;

		spells[4].info = dbcSpell.LookupEntry(DISORIENT);
		spells[4].targettype = TARGET_ATTACKING;
		spells[4].instant = true;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 2000;
		spells[4].cooldown = 10;

		spells[5].info = dbcSpell.LookupEntry(KNOCKBACK);
		spells[5].targettype = TARGET_ATTACKING;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;
		spells[5].cooldown = 15;

		// Phase 2
		spells[6].info = dbcSpell.LookupEntry(ACID_GEYSER);
		spells[6].targettype = TARGET_DESTINATION;
		spells[6].instant = true;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 2000;
		spells[6].cooldown = 10;

		spells[7].info = dbcSpell.LookupEntry(FEL_RAGE1);
		spells[7].targettype = TARGET_RANDOM_SINGLE;
		spells[7].instant = true;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 2000;
		spells[7].cooldown = 90;
		spells[7].mindist2cast = 0.0f;
		spells[7].maxdist2cast = 60.0f;

		spells[8].info = dbcSpell.LookupEntry(FEL_RAGE2);
		spells[8].targettype = TARGET_RANDOM_SINGLE;
		spells[8].instant = true;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 2000;
		spells[8].cooldown = 90;
		spells[8].mindist2cast = 0.0f;
		spells[8].maxdist2cast = 60.0f;

		LastThreat = 0;
		PhaseTimer = 0;
		Phase = 1;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < 9; i++)
		{
			spells[i].casttime = 0;
		}

		spells[2].casttime = (uint32)time(NULL) + 10;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Horde will crush you!");
		_unit->PlaySoundToSet(11432);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

		PhaseTimer = (uint32)time(NULL) + 60;
		LastThreat = 0;
		Phase = 1;
    }

	void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		if (_unit->isAlive())
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll rip the meat from your bones!");
			_unit->PlaySoundToSet(11437);
		}

		RemoveAIUpdateEvent();

		LastThreat = 0;
		PhaseTimer = 0;
		Phase = 1;
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->PlaySoundToSet(11439);
		Matar4firstboss(1,_unit);

		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		switch (RandomUInt(2))
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "More! I want more!");
			_unit->PlaySoundToSet(11434);
			break;
		default:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Time to feast!");
				_unit->PlaySoundToSet(11433);
			}
		}
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (Phase == 1 && t > PhaseTimer)
		{
			spells[7].casttime = 0;

			PhaseTimer = 0;
			Phase = 2;
		}

		// Phase 1
		if (Phase == 1 && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			// Bloodboil
			if (t > spells[2].casttime)
			{
				_unit->setAttackTimer(spells[2].attackstoptimer, false);

				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				spells[2].casttime = t + spells[2].cooldown;
				return;
			}

			// Arcing Smash
			if (t > spells[0].casttime)
			{
				_unit->setAttackTimer(spells[0].attackstoptimer, false);

				UnitPointer target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(target, spells[0].info, spells[0].instant);

				spells[0].casttime = t + spells[0].cooldown;
				return;
			}

			// Knockback
			if (t > spells[5].casttime)
			{
				_unit->setAttackTimer(spells[5].attackstoptimer, false);

				UnitPointer target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(target, spells[5].info, spells[5].instant);

				if (_unit->GetAIInterface()->GetSecondHated())
					_unit->GetAIInterface()->SetNextTarget(_unit->GetAIInterface()->GetSecondHated());

				spells[5].casttime = t + spells[5].cooldown;
				return;
			}
		}

		if (Phase == 2 && _unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			uint32 t = (uint32)time(NULL);

			// Fel Rage check
			if (t > spells[7].casttime - 62 && LastThreat != 0)
			{
				UnitPointer RageTarget = NULLUNIT;
				RageTarget = _unit->GetAIInterface()->GetNextTarget();
				if (RageTarget->FindAura(FEL_RAGE1) && RageTarget->FindAura(FEL_RAGE2))
				{
					_unit->GetAIInterface()->RemoveThreatByPtr(RageTarget);
					_unit->GetAIInterface()->AttackReaction(RageTarget, LastThreat, 0);

					LastThreat = 0;
				}
			}

			// Acid Geyser
			if (LastThreat != 0 && RandomUInt(5) == 1 && t > spells[6].casttime)
			{
				UnitPointer RageTarget = NULLUNIT;
				RageTarget = _unit->GetAIInterface()->GetNextTarget();
				if (RageTarget->FindAura(FEL_RAGE1) || RageTarget->FindAura(FEL_RAGE2))
				{
					_unit->setAttackTimer(spells[6].attackstoptimer, false);

					_unit->CastSpellAoF(RageTarget->GetPositionX(),RageTarget->GetPositionY(),RageTarget->GetPositionZ(), spells[6].info, spells[6].instant);

					spells[6].casttime = t + spells[6].cooldown;
					return;
				}
			}

			// Arcing Smash
			if (t > spells[0].casttime)
			{
				_unit->setAttackTimer(spells[0].attackstoptimer, false);

				UnitPointer target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(target, spells[0].info, spells[0].instant);

				spells[0].casttime = t + spells[0].cooldown;
				return;
			}

			// Fel Rage - not sure about system
			if (t > spells[7].casttime)
			{
				_unit->setAttackTimer(spells[7].attackstoptimer, false);

				std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
													/* If anyone wants to use this function, then leave this note!										 */
				for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if (isHostile(_unit, (*itr)) && (*itr) != _unit && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
					{
						UnitPointer RandomTarget = NULLUNIT;
						RandomTarget = TO_UNIT(*itr);

						if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= 0.0f && _unit->GetDistance2dSq(RandomTarget) <= 6400.0f && _unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))
						{
							TargetTable.push_back(RandomTarget);
						} 
					} 
				}

				if (!TargetTable.size())
					return;

				size_t RandTarget = rand()%TargetTable.size();

				UnitPointer RTarget = TargetTable[RandTarget];

				if (!RTarget)
					return;

				_unit->CastSpell(RTarget, spells[7].info, spells[7].instant);
				_unit->CastSpell(_unit, spells[8].info, spells[8].instant);

				LastThreat = _unit->GetAIInterface()->getThreatByPtr(RTarget);

				UnitPointer Target = _unit->GetAIInterface()->GetMostHated();
				uint32 MostHatedThreat = 0;
				if (Target)	// unneeded, but still safe check :P
				{
					MostHatedThreat = _unit->GetAIInterface()->getThreatByPtr(Target);
				}

				_unit->GetAIInterface()->AttackReaction(RTarget, MostHatedThreat + 100000, 0);

				TargetTable.clear();

				switch (RandomUInt(2))
				{
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll rip the meat from your bones!");
					_unit->PlaySoundToSet(11438);
					break;
				default:
					{
						_unit->PlaySoundToSet(11437);
					}
				}

				spells[7].casttime = t + spells[7].cooldown;
				spells[6].casttime = t;
				return;
			}
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if ((i == 3 || i == 4) && Phase != 1)
					continue;

				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant);
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();

			if (i == 7 || i == 8)
			{
				//AuraPointer aura = new Aura(spells[7].info, 30000, _unit, RTarget);
				//if (aura)
				//	RTarget->AddAura(aura);
				RTarget->CastSpell(RTarget, spells[7].info, spells[7].instant);

				LastThreat = _unit->GetAIInterface()->getThreatByPtr(RTarget);
				UnitPointer Target = _unit->GetAIInterface()->GetNextTarget();
				RTarget->GetAIInterface()->AttackReaction(RTarget, _unit->GetAIInterface()->getThreatByPtr(Target) + 100000, 0);
			}
		}
	}

protected:

	uint32 LastThreat;
	uint32 PhaseTimer;
	uint32 Phase;
	int nrspells;
};

//----------------------------------------------------------------------//
//-------------------------Reliquary of Souls---------------------------//
//----------------------------------------------------------------------//

#define CN_RELIQUARYOFTHELOST 22856
#define CN_ENSLAVEDSOUL 23469
#define CN_ESSENCEOFANGER 23420
#define CN_ESSENCEOFDESIRE 23419
#define CN_ESSENCEOFSUFFERING 23418

#define AURA_OF_SUFFERING1 41292
#define AURA_OF_SUFFERING2 42017
#define AURA_OF_ANGER 41337
#define AURA_OF_DESIRE 41350

//----------------------------------------------------------------------//
//-----------------------Reliquary of the Lost--------------------------//
//----------------------------------------------------------------------//

class ReliquaryoftheLostAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ReliquaryoftheLostAI);

    ReliquaryoftheLostAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_canMove = false;
		suffering = NULLCREATURE;
		desire = NULLCREATURE;
		anger = NULLCREATURE;
		encounter = false;
		phase = 1;
		phase0_timer = 0;
		switching = false;
		reset_encounter = false;
		anger_timer = 3;
		anger_counter = 0;
		RegisterAIUpdateEvent(1000);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't leave me alone!.");
		_unit->PlaySoundToSet(11416);
		
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{	
		if( reset_encounter )
		{
			_unit->Emote(EMOTE_STATE_NONE);
			encounter = false;
			phase = 1;
			reset_encounter = false;
/*			if(!suffering)
			{
				suffering = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(),
							_unit->GetPositionY(), _unit->GetPositionZ(), CN_ESSENCEOFSUFFERING);
				if( suffering )
				{
					suffering->Despawn(500,0);
					suffering = NULL;
				}
			}
			else
			{
				suffering = _unit->GetMapMgr()->GetCreature( (uint32)suffering->GetGUID() );
				if( suffering )
				{
					suffering->Despawn(500,0);
					suffering = NULL;
				}
			}

			if( !desire )
			{
				desire = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(),
							_unit->GetPositionY(), _unit->GetPositionZ(), CN_ESSENCEOFDESIRE);
				if( desire )
				{
					desire->Despawn(500,0);
					desire = NULLCREATURE;
				}
			}
			else
			{
				desire = _unit->GetMapMgr()->GetCreature( (uint32)desire->GetGUID() );
				if( desire )
				{
					desire->Despawn(500,0);
					desire = NULLCREATURE;
				}
			}

			if( !anger )
			{
				anger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(),
							_unit->GetPositionY(), _unit->GetPositionZ(), CN_ESSENCEOFANGER);
				if( anger )
				{
					anger->Despawn(500,0);
					anger = NULL;
				}
			}
			else
			{
				anger = _unit->GetMapMgr()->GetCreature( (uint32)anger->GetGUID() );
				if( anger )
				{
					anger->Despawn(500,0);
					anger = NULL;
				}
			}*/

			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if ( (*itr) && (*itr)->IsPlayer() )
				{
					UnitPointer tg = TO_UNIT(*itr);
					if( tg && tg->isAlive() )
					{
						tg->HasAura(AURA_OF_SUFFERING1);
						tg->HasAura(AURA_OF_SUFFERING2);
						tg->HasAura(AURA_OF_DESIRE);
						tg->HasAura(AURA_OF_ANGER);
						//tg->RL_phase2 = false; commented by devnull: what the hell is this bool?
					}
				}
			}
		}

		if( !encounter )
		{
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if ( (*itr) && (*itr)->IsPlayer() )
				{
					UnitPointer tg = TO_UNIT(*itr);
					float dist = _unit->GetDistance2dSq( static_cast< ObjectPointer >( tg ) );
					if( tg->isAlive() && dist <= 100.0f )
					{
						encounter = true;
						_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
						suffering = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ESSENCEOFSUFFERING, _unit->GetPositionX(), _unit->GetPositionY()-20.0f, _unit->GetPositionZ(),_unit->GetOrientation(), false, false, 0, 0);
						if( suffering )
							suffering->m_noRespawn = true;
						switching = true;
						break;
					}
				}
			}
		}
		else
		{
			if( phase0_timer > 0 )
			{
				std::vector<UnitPointer > TargetTable;		
				for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if ( (*itr) && (*itr)->IsPlayer() )
					{
						UnitPointer tg = TO_UNIT(*itr);
						if( tg && tg->isAlive() )
							TargetTable.push_back(tg);
					}
				}
				if (!TargetTable.size())
					return;
				size_t RandTarget = rand()%TargetTable.size();
				UnitPointer RTarget = TargetTable[RandTarget];
				if (!RTarget)
					return;
				CreaturePointer cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ENSLAVEDSOUL, 455.0f+RandomFloat(83.0f), 135.0f+RandomFloat(80.0f), _unit->GetPositionZ(),_unit->GetOrientation(), false, false, 0, 0);
				if( cre )
				{
					CreaturePointer crevisual = _unit->GetMapMgr()->GetInterface()->SpawnCreature(1234501, cre->GetPositionX(), cre->GetPositionY(), _unit->GetPositionZ(),_unit->GetOrientation(), false, false, 0, 0);
					if( crevisual )
					{
						crevisual->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						crevisual->GetAIInterface()->SetAllowedToEnterCombat(false);
						crevisual->m_noRespawn = true;
						crevisual->CastSpell(crevisual,42656,true);
						crevisual->Despawn(7000,0);
					}

					cre->m_noRespawn = true;
					cre->Despawn(60000,0);
					cre->GetAIInterface()->AttackReaction(RTarget, 100000, 0);
				}
				TargetTable.clear();
				phase0_timer--;
			}

			if( phase == 1 )
			{
				if( switching )
				{
					switching = false;
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);	
					for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if ( (*itr) && (*itr)->IsPlayer() )
						{
							UnitPointer tg = TO_UNIT(*itr);
							if( tg && tg->isAlive() )
							{
								tg->RemoveAura(AURA_OF_DESIRE);
								tg->RemoveAura(AURA_OF_ANGER);
								AuraPointer aura = AuraPointer(new Aura(dbcSpell.LookupEntry(AURA_OF_SUFFERING1), -1, _unit, tg));
			                    if (aura)
			                    	tg->AddAura(aura,NULLAURA);
                                /* 
								AuraPointer aura = new Aura(dbcSpell.LookupEntry(AURA_OF_SUFFERING1), -1, _unit, tg);
								aura->m_areaAura = false;
								aura->AddMod(118, -100, 127, 0);
								aura->AddMod(88, -100, 0, 1);
								tg->AddAura(aura);*/
								AuraPointer aura2 = AuraPointer(new Aura(dbcSpell.LookupEntry(AURA_OF_SUFFERING2), -1, _unit, tg));
			                    if (aura2)
			                    	tg->AddAura(aura2,NULLAURA);
								/* 
								AuraPointer aura2 = new Aura(dbcSpell.LookupEntry(AURA_OF_SUFFERING2), -1, _unit, tg);
								aura2->m_areaAura = false;
								aura2->AddMod(101, -100, 1, 0);
								aura2->AddMod(30, -500, 95, 1);
								tg->AddAura(aura2);*/
							}
						}
					}
				}
				if( suffering && suffering->GetHealthPct() <= 1 )
				{
					for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if ( (*itr) && (*itr)->IsPlayer() )
						{
							UnitPointer tg = TO_UNIT(*itr);
							if( tg && tg->isAlive() )
							{
								tg->RemoveAura(AURA_OF_SUFFERING1);
								tg->RemoveAura(AURA_OF_SUFFERING2);
							}
						}
					}
					suffering->Despawn(500,0);
					suffering = NULLCREATURE;
					phase = 2;
					switching = true;
					phase0_timer = 20;
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
				}
			}	

			if( phase == 2 && phase0_timer == 0 )
			{
				if( switching )
				{
					for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if ( (*itr) && (*itr)->IsPlayer() )
						{
							UnitPointer tg = TO_UNIT(*itr);
							if( tg && tg->isAlive() )
							{
								AuraPointer aura = AuraPointer(new Aura(dbcSpell.LookupEntry(AURA_OF_DESIRE), 160000, _unit, tg));
			                    if (aura)
			                    	tg->AddAura(aura,NULLAURA);
								//	tg->RL_phase2 = true;

                                /*
								AuraPointer aura = new Aura(dbcSpell.LookupEntry(AURA_OF_DESIRE), 160000, _unit, tg);
								aura->m_areaAura = false;
								aura->AddMod(118, 100, 127, 0);
								aura->AddMod(132, 0, 0, 1);
								aura->AddMod(23, 0, 0, 2);
								tg->AddAura(aura);
								tg->RL_phase2 = true;*/ 
							}
						}
					}
					_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
					desire = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ESSENCEOFDESIRE, _unit->GetPositionX(), _unit->GetPositionY()-20.0f, _unit->GetPositionZ(),_unit->GetOrientation(), false, false, 0, 0);
					if( desire )
						desire->m_noRespawn = true;
					
					switching = false;
				}

				if( desire && desire->GetHealthPct() <= 1 )
				{
					for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if ( (*itr) && (*itr)->IsPlayer() )
						{
							UnitPointer tg = TO_UNIT(*itr);
							if( tg && tg->isAlive() )
								tg->RemoveAura(AURA_OF_DESIRE);
						}
					}

					desire->Despawn(500,0);
					desire = NULLCREATURE;
					phase = 3;
					switching = true;
					phase0_timer = 20;
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
				}

				for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if ( (*itr) && (*itr)->IsPlayer() )
					{
						UnitPointer tg = TO_UNIT(*itr);
						if( tg && tg->isAlive() && tg->GetPowerType() == POWER_TYPE_MANA )
						{
							tg->SetUInt32Value(UNIT_FIELD_MAXPOWER1,float2int32(float(tg->GetUInt32Value(UNIT_FIELD_MAXPOWER1))*0.96f));
							if( tg->GetUInt32Value(UNIT_FIELD_POWER1) > tg->GetUInt32Value(UNIT_FIELD_MAXPOWER1) )
								tg->SetUInt32Value(UNIT_FIELD_POWER1, tg->GetUInt32Value(UNIT_FIELD_MAXPOWER1)); 
						}
					}
				}
			}

			if( phase == 3 && phase0_timer == 0 )
			{
				if( switching )
				{
					anger_timer = 3;
					anger_counter = 0;
					for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if ( (*itr) && (*itr)->IsPlayer() )
						{
							UnitPointer tg = TO_UNIT(*itr);
							if( tg && tg->isAlive() )
							{ 
								AuraPointer aura = AuraPointer(new Aura(dbcSpell.LookupEntry(AURA_OF_ANGER), 600000, _unit, tg));
			                    if (aura)
			                    	tg->AddAura(aura,NULLAURA);
									//tg->RL_phase2 = true;
                                /*
								AuraPointer aura = new Aura(dbcSpell.LookupEntry(AURA_OF_ANGER), 600000, _unit, tg);
								aura->m_areaAura = false;
								aura->AddMod(3, 100, 0, 0);
								aura->AddMod(79, 5, 127, 1);
								tg->AddAura(aura);
								tg->RL_phase2 = false;*/ 
							}
						}
					}
					_unit->Emote(EMOTE_ONESHOT_SUBMERGE);
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);
					anger = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ESSENCEOFANGER, _unit->GetPositionX(), _unit->GetPositionY()-20.0f, _unit->GetPositionZ(),_unit->GetOrientation(), false, false, 0, 0);
					if( anger )
						anger->m_noRespawn = true;
					
					switching = false;
				}

				if( anger_timer == 0 )
				{
					anger_counter++;
					for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
					{ 
						if ( (*itr) && (*itr)->IsPlayer() )
						{
							UnitPointer tg = TO_UNIT(*itr);
							if( tg && tg->isAlive() )
								_unit->SpellNonMeleeDamageLog(tg, AURA_OF_ANGER, 103*anger_counter, 0);						
						}
					}
					anger_timer = 3;
				}
				anger_timer--;
			}
		}
	}

   	bool encounter;
	bool reset_encounter;

protected:

	CreaturePointer suffering;
	CreaturePointer desire;
	CreaturePointer anger;
	int phase;
	int phase0_timer;
	bool switching;
	int anger_timer;
	int anger_counter;
	
};

//------------------------------------
//    -= Essence of Suffering =-
//------------------------------------
#define FIXATE 40414
#define SOUL_DRAIN 41303
#define ENRAGE_EOS 41305

class EssenceOfSufferingAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EssenceOfSufferingAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EssenceOfSufferingAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 4;
		SoulDrainTimer = 15;
		EnrageTimer = 23;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(FIXATE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 5;

		spells[1].info = dbcSpell.LookupEntry(ENRAGE_EOS);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(SOUL_DRAIN);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 20;

		spells[3].info = dbcSpell.LookupEntry(AURA_OF_SUFFERING1);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;

    }

    void OnCombatStart(UnitPointer mTarget)
    {
		SoulDrainTimer = 15;
		EnrageTimer = 23;
		RegisterAIUpdateEvent(1000);
		spells[0].cooldown = 5;
		spells[1].cooldown = 60;
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Pain and suffering are all that await you.");
		_unit->PlaySoundToSet(11415);
    }

	void OnCombatStop(UnitPointer mTarget)
    {
		CreaturePointer boss = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(496.4747f, 181.1151f, 94.05f, CN_RELIQUARYOFTHELOST);
		if( boss )
			static_cast<ReliquaryoftheLostAI*>(boss->GetScript())->reset_encounter = true;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
	   _unit->Despawn(10000,0);
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I don't want to go back!");
		_unit->PlaySoundToSet(11420);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Look at what you made me do.");
		_unit->PlaySoundToSet(11417);
	}

	void AIUpdate()
	{
		//Fixate
		if( spells[0].cooldown == 0 )
		{
			std::vector<UnitPointer > TargetTable;		
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if ( (*itr) && (*itr)->IsPlayer() )
				{
					UnitPointer tg = TO_UNIT(*itr);
					if( tg && tg->isAlive() && _unit->GetDistance2dSq(tg) <= 50.0f )
						TargetTable.push_back(tg);
				}
			}
			if (!TargetTable.size())
				return;
			size_t RandTarget = rand()%TargetTable.size();
			UnitPointer RTarget = TargetTable[RandTarget];
			if (!RTarget)
				return;
/*			AuraPointer aura = new Aura();
			if (aura)
		    {
*/				RTarget->CastSpell(RTarget, spells[0].info, spells[0].instant);		
//			}   
/*
			AuraPointer aura = new Aura(dbcSpell.LookupEntry(FIXATE), 5000, _unit, RTarget);
			aura->m_areaAura = false;
			RTarget->AddAura(aura);
			aura->AddMod(4, 1, 0, 0);
			aura->AddMod(4, 0, 0, 1);*/ 
			_unit->GetAIInterface()->WipeHateList();
			_unit->GetAIInterface()->AttackReaction(RTarget, 1000000, 0);
			TargetTable.clear();
			spells[0].cooldown = 5;
		}
	
		//Enrage
		if(!EnrageTimer)
		{
			_unit->GetAIInterface()->CastSpell(_unit, dbcSpell.LookupEntry(ENRAGE_EOS), TARGET_SELF);
			EnrageTimer = 23;
		}
		
		//Soul Drain
		if(!SoulDrainTimer)
		{
			_unit->GetAIInterface()->CastSpell(_unit, dbcSpell.LookupEntry(SOUL_DRAIN), TARGET_RANDOM_SINGLE);
			SoulDrainTimer = 15;
		}

		spells[0].cooldown--;
		
	}

protected:
	int nrspells;
	uint32 EnrageTimer;
	uint32 SoulDrainTimer;
};

//------------------------------------
//    -= EssenceOfDesire =-
//------------------------------------
#define RUNE_SHIELD 41431
#define DEADEN 41410
#define SPIRIT_SHOCK 41426

class EssenceOfDesireAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EssenceOfDesireAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EssenceOfDesireAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(RUNE_SHIELD);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 25;

		spells[1].info = dbcSpell.LookupEntry(DEADEN);
		spells[1].targettype = TARGET_ATTACKING;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 30;

		spells[2].info = dbcSpell.LookupEntry(SPIRIT_SHOCK);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 5;

		spells[3].info = dbcSpell.LookupEntry(AURA_OF_DESIRE);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		spells[0].cooldown = 25;
		spells[1].cooldown = 30;
		spells[2].cooldown = 5;
		RegisterAIUpdateEvent(1000);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You can have anything you desire... for a price.");
		_unit->PlaySoundToSet(11408);
    }

	void OnCombatStop(UnitPointer mTarget)
    {
		CreaturePointer boss = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(496.4747f, 181.1151f, 94.05f, CN_RELIQUARYOFTHELOST);
		if( boss )
			static_cast<ReliquaryoftheLostAI*>(boss->GetScript())->reset_encounter = true;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
	   _unit->Despawn(10000,0);
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll be waiting.");
		_unit->PlaySoundToSet(11413);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Fulfilment is at hand.");
		_unit->PlaySoundToSet(11409);
	}

	void AIUpdate()
	{
		if( spells[0].cooldown == 0 )
		{
			_unit->CastSpell(_unit, RUNE_SHIELD, false);
			spells[0].cooldown = 25+RandomUInt(15);
		}
		
		if( spells[1].cooldown == 0 )
		{
			if( _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() )
				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(),DEADEN,false);
			
			spells[1].cooldown = 30;
		}
		
		if( spells[2].cooldown == 0 )
		{
//			if( _unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() )
//				_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(),SPIRIT_SHOCK,false);
				
			spells[2].cooldown = 5;
		}

		spells[0].cooldown--;
		spells[1].cooldown--;
		spells[2].cooldown--;
	}

    
protected:
	int nrspells;
};
//------------------------------------
//    -= EssenceOfAnger =-
//------------------------------------
#define SEETHE1 41520
#define SEETHE2 41364
#define SOUL_SCREAM 41545
#define SPITE_DMG 41377
#define SPITE_IMM 41376

class EssenceOfAngerAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EssenceOfAngerAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    EssenceOfAngerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 4;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SEETHE1);
		spells[0].targettype = TARGET_VARIOUS;
		spells[0].instant = false;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 20;

		spells[1].info = dbcSpell.LookupEntry(SOUL_SCREAM);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 25;

		spells[2].info = dbcSpell.LookupEntry(SPITE_DMG);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 30;

		spells[3].info = dbcSpell.LookupEntry(AURA_OF_ANGER);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 1000;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		spit_timer = 6;
		spited_target = NULLUNIT;
		spells[1].cooldown = 25;
		spells[2].cooldown = 30;
		last_tank = _unit->GetAIInterface()->GetNextTarget();
		RegisterAIUpdateEvent(1000);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Beware - I live.");
		_unit->PlaySoundToSet(11399);
    }

	void OnCombatStop(UnitPointer mTarget)
    {
		CreaturePointer boss = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(496.4747f, 181.1151f, 94.05f, CN_RELIQUARYOFTHELOST);
		if( boss )
			static_cast<ReliquaryoftheLostAI*>(boss->GetScript())->reset_encounter = true;
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
       RemoveAIUpdateEvent();
	   _unit->Despawn(10000,0);
    }

    void OnDied(UnitPointer mKiller)
    {
		CreaturePointer boss = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(496.4747f, 181.1151f, 94.05f, CN_RELIQUARYOFTHELOST);
		if( boss )
			boss->DealDamage(boss, boss->GetUInt32Value(UNIT_FIELD_HEALTH), 0, 0, 0);
		for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if ( (*itr) && (*itr)->IsPlayer() )
			{
				UnitPointer tg = TO_UNIT(*itr);
				if( tg && tg->isAlive() )
				{
					tg->HasAura(AURA_OF_ANGER);
//					tg->RL_phase2 = false;
				}
			}
		}
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I won't be ignored.");
		_unit->PlaySoundToSet(11404);
		Matar4firstboss(1,_unit);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Enough! No more!");
		_unit->PlaySoundToSet(11402);
		_unit->PlaySoundToSet(11401);
	}

	void AIUpdate()
	{
		if( spited_target )
		{
			if( spit_timer == 0)
			{
				_unit->CastSpell(spited_target, SPITE_DMG, true);
				spited_target = NULLUNIT;
			}
			else
				spit_timer--;
		}

		if( last_tank == NULL )
			last_tank = _unit->GetAIInterface()->GetNextTarget();

		//Seethe
		if( last_tank && last_tank != _unit->GetAIInterface()->GetNextTarget() )
		{
			std::vector<UnitPointer > TargetTable;		
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if ( (*itr) && (*itr)->IsPlayer() )
				{
					UnitPointer tg = TO_UNIT(*itr);
					if( tg && tg->isAlive() )
						TargetTable.push_back(tg);
				}
			}
			if (!TargetTable.size())
				return;
			size_t RandTarget = rand()%TargetTable.size();
			UnitPointer RTarget = TargetTable[RandTarget];
			if (!RTarget)
				return;
			RTarget->CastSpell(RTarget,SEETHE1,true);
			RTarget->CastSpell(RTarget,SEETHE2,true);
			last_tank = _unit->GetAIInterface()->GetNextTarget();
			TargetTable.clear();
		}

		//Soul Scream
		if( spells[1].cooldown == 0 )
		{
			_unit->CastSpell(_unit, SOUL_SCREAM, true);
			spells[1].cooldown = 22+RandomUInt(20);
		}

		//Spite
		if( spells[2].cooldown == 0 )
		{
			std::vector<UnitPointer > TargetTable;		
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if ( (*itr) && (*itr)->IsPlayer() )
				{
					UnitPointer tg = TO_UNIT(*itr);
					if( tg && tg->isAlive() )
						TargetTable.push_back(tg);
				}
			}
			if (!TargetTable.size())
				return;
			size_t RandTarget = rand()%TargetTable.size();
			UnitPointer RTarget = TargetTable[RandTarget];
			if (!RTarget)
				return;
			
			spited_target = RTarget;
			spit_timer = 6;


				RTarget->CastSpell(RTarget, dbcSpell.LookupEntry(SPITE_IMM), true);		
/*
			AuraPointer aura = new Aura(dbcSpell.LookupEntry(SPITE_IMM), true);
			aura->m_areaAura = false;
			aura->AddMod(40, 0, 127, 0);
			RTarget->AddAura(aura);*/ 

			TargetTable.clear();

			spells[2].cooldown = 24+RandomUInt(20);
		}

		spells[1].cooldown--;
		spells[2].cooldown--;
		
	}

protected:
	int nrspells;
	UnitPointer spited_target;
	int spit_timer;
	UnitPointer last_tank;
};

//---------Enslaved Soul---------//

class EnslavedSoulAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EnslavedSoulAI);

    EnslavedSoulAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {		
    }
    
    void OnDied(UnitPointer mKiller)
    {
		uint32 hptoheal = mKiller->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*30/100;
		SpellPointer spell = SpellPointer(new Spell(_unit,dbcSpell.LookupEntry(41542),true,NULLAURA));
		//SpellPointer spell=new Spell(_unit,dbcSpell.LookupEntry(41542),true,NULL);
		SpellCastTargets targets;
		spell->forced_basepoints[0] = hptoheal;
		targets.m_unitTarget = mKiller->GetGUID();
		spell->prepare(&targets);
    }
};

//--------------------------Mother Shahraz------------------------------//

#define CN_MOTHER_SHAHRAZ 22947

#define SINFUL_BEAM				40827
#define SINISTER_BEAM			40859
#define VILE_BEAM				40860
#define WICKED_BEAM				40861

#define SABER_LASH				40810	// Should be 40816 but 40816 is not supported by core
#define FATAL_ATTRACTION		41001	// 40869 - needs more scripting
#define MS_ENRAGE				40743	// 40683
#define GO_GATE_MOTHER          185482
// Dunno where "spell" sounds/texts should go
class ShahrazAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShahrazAI);
	SP_AI_Spell spells[6];
	bool m_spellcheck[6];

    ShahrazAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {   
		Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(903.278f, 149.008f, 192.818f, GO_GATE_MOTHER);
		if(Gate)
			Gate->SetByte(GAMEOBJECT_BYTES_1, 0, 1);// cerrada

		nrspells = 2;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SABER_LASH);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 15;

		spells[1].info = dbcSpell.LookupEntry(FATAL_ATTRACTION);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;
		spells[1].mindist2cast = 10.0f;
		spells[1].maxdist2cast = 60.0f;

		spells[2].info = dbcSpell.LookupEntry(SINFUL_BEAM);
		spells[2].targettype = TARGET_RANDOM_SINGLE;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 9;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 80.0f;
		
		spells[3].info = dbcSpell.LookupEntry(WICKED_BEAM);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 9;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 80.0f;

		spells[4].info = dbcSpell.LookupEntry(VILE_BEAM);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 2000;
		spells[4].cooldown = 9;
		spells[4].mindist2cast = 0.0f;
		spells[4].maxdist2cast = 80.0f;

		spells[5].info = dbcSpell.LookupEntry(SINISTER_BEAM);
		spells[5].targettype = TARGET_RANDOM_SINGLE;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;
		spells[5].cooldown = 9;
		spells[5].mindist2cast = 0.0f;
		spells[5].maxdist2cast = 80.0f;

		Enraged = false;
		AuraChange = 0;
		SoundTimer = 0;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So, business... or pleasure?");
		_unit->PlaySoundToSet(11504);

		for (int i = 0; i < 6; i++)
			spells[i].casttime = 0;

		AuraChange = (uint32)time(NULL) + 15;
		Enraged = false;
		SoundTimer = 5;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }
	
    void OnDied(UnitPointer mKiller)
    {   
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I wasn't finished.");
		_unit->PlaySoundToSet(11511);
		if(Gate)
		{
			Gate->SetByte(GAMEOBJECT_BYTES_1, 0, 0); //abierta
		}
		Reset();
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		switch (RandomUInt(2))
		{
		case 1:
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "So much for a happy ending.");
			_unit->PlaySoundToSet(11509);
			break;
		default:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Easy come, easy go.");
				_unit->PlaySoundToSet(11508);
			}
		}
	}

	void AIUpdate()
	{
		SoundTimer++;
		if (_unit->GetAIInterface()->GetIsTaunted() && SoundTimer > 10)
		{
			switch (RandomUInt(3))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'm not impressed.");
				_unit->PlaySoundToSet(11502);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Enjoying yourselves?");
				_unit->PlaySoundToSet(11503);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You play, you pay.");
					_unit->PlaySoundToSet(11501);
				}
			}

			SoundTimer = 0;
		}

		if (!Enraged && _unit->GetHealthPct() <= 20)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stop toying with my emotions!");
			_unit->PlaySoundToSet(11510);

			_unit->CastSpell(_unit, MS_ENRAGE, true);

			Enraged = true;
		}

		uint32 t = (uint32)time(NULL);

		// In normal way it is applied to players all around enemy caster =/
		if (t > AuraChange)
		{
			uint32 SpellId = 0;
			switch (RandomUInt(6))
			{
			case 1:
				SpellId = 40891;	// Arcane
				break;
			case 2:
				SpellId = 40882;	// Fire
				break;
			case 3:
				SpellId = 40896;	// Frost
				break;
			case 4:
				SpellId = 40897;	// Holy
				break;
			case 5:
				SpellId = 40883;	// Nature
				break;
			case 6:
				SpellId = 40880;	// Shadow
				break;
			default:
				{
					SpellId = 40880;	// Shadow
				}
			}

			//_unit->CastSpell(_unit, SpellId, true);
			AuraPointer aura = AuraPointer(new Aura(dbcSpell.LookupEntry(SpellId), 15000, _unit, _unit));
			if (aura)
				_unit->AddAura(aura,NULLAURA);

			AuraChange = t + 15;
		}

		if (t > spells[2].casttime)
		{
			uint32 SpellId = RandomUInt(4) + 1;
			if (SpellId < 2 || SpellId > 5)
				SpellId = 5;

			_unit->setAttackTimer(spells[2].attackstoptimer, false);

			CastSpellOnRandomTarget(SpellId, spells[2].mindist2cast, spells[2].maxdist2cast, 0, 100);

			spells[2].casttime = t + spells[2].cooldown;
			return;
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
	void Reset()
	{
		//Gate = NULL;
	}

protected:
    GameObjectPointer Gate;
	uint32 SoundTimer;
	uint32 AuraChange;
	bool Enraged;
	int nrspells;
};

//-----------------------The Illidari Council---------------------------//

//------------------------------------
//    -=Veras Darkshadow  =-
//------------------------------------
#define CN_VERAS_DARKSHADOW 22952

#define DEADLY_POISON	41485
#define ENVENOM			41487
#define VANISH			41476

class VerasAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VerasAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    VerasAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEADLY_POISON);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 60.0f;

		spells[1].info = dbcSpell.LookupEntry(ENVENOM);
		spells[1].targettype = TARGET_RANDOM_SINGLE;
		spells[1].instant = true;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 10;

		spells[2].info = dbcSpell.LookupEntry(VANISH);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 32000;
		spells[2].cooldown = 85;

		Poisoned = false;
		Vanish = 0;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You wish to test me?");
		_unit->PlaySoundToSet(11524);

		for (int i = 0; i < 2; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[2].casttime = t + 55;

		Poisoned = false;
		Vanish = 0;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->disable_melee = false;

        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You got lucky!");
		_unit->PlaySoundToSet(11527);
        Matar4TheIllidariConcilio(1,_unit);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Valiant Effort.");
		_unit->PlaySoundToSet(11525);
	}

	void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > Vanish)
		{
			_unit->GetAIInterface()->ClearHateList();
		}

		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL && !_unit->FindAura(VANISH))
		{
			uint32 t = (uint32)time(NULL);
			if (t > spells[2].casttime)
			{
				std::vector <UnitPointer > TargetTable;
				for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if (isHostile(_unit, (*itr)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
					{
						UnitPointer Target = NULLUNIT;
						Target = TO_UNIT(*itr);

						if (Target->isAlive() && _unit->GetDistance2dSq(Target) <= 30.0f && _unit->GetAIInterface()->getThreatByPtr(Target) > 0)
							TargetTable.push_back(Target);
					} 
				}

				if (TargetTable.size())
				{
					_unit->GetAIInterface()->ClearHateList();

					UnitPointer UnitToFollow = NULLUNIT;
					size_t Rand = rand()%TargetTable.size();
					UnitToFollow = TargetTable[Rand];

					if (UnitToFollow)
					{
						_unit->GetAIInterface()->AttackReaction(UnitToFollow, 10000, 0);
					}

					Vanish = t + 31;	// 31, coz there is 1-2 sec till he fights again after vanish aura is removed
				}

				_unit->setAttackTimer(spells[2].attackstoptimer, false);
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				spells[2].casttime = t + spells[2].cooldown;
				return;
			}

			if (_unit->GetAIInterface()->GetNextTarget() && t > spells[1].casttime && Poisoned && RandomUInt(3) != 1)
			{
				UnitPointer TargetToAttack = NULLUNIT;
				for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
				{ 
					if (isHostile(_unit, (*itr)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID())
					{
						UnitPointer PoisonedTarget = NULLUNIT;
						PoisonedTarget = TO_UNIT(*itr);

						if (PoisonedTarget->isAlive() && _unit->GetAIInterface()->getThreatByPtr(PoisonedTarget) > 0 && PoisonedTarget->FindAura(DEADLY_POISON))
						{
							TargetToAttack = PoisonedTarget;
							break;
						} 
					} 
				}

				if (TargetToAttack)
				{
					_unit->CastSpell(TargetToAttack, spells[1].info, spells[1].instant);
				}

				spells[1].casttime = t + spells[1].cooldown;
				Poisoned = false;
				return;
			}
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget() && !_unit->FindAura(VANISH))
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

    void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();

			if (i == 0)
				Poisoned = true;
		}
	}

protected:

	uint32 Vanish;
	bool Poisoned;
	int nrspells;
};

//------------------------------------
//    -= High Nethermancer Zerevor  =-
//------------------------------------
#define CN_HIGH_NETHERMANCER_ZEREVOR 22950

#define ARCANE_BOLT			41483
#define BLIZZARD			41482
#define FLAMESTRIKE			41481
#define ARCANE_EXPLOSION	29973
#define DAMPEN_MAGIC		41478

class ZerevorAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ZerevorAI);
	SP_AI_Spell spells[5];
	bool m_spellcheck[5];

    ZerevorAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 3;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(ARCANE_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 65.0f;
		spells[0].attackstoptimer = 2000;
		spells[0].cooldown = 0;

		spells[1].info = dbcSpell.LookupEntry(BLIZZARD);
		spells[1].targettype = TARGET_RANDOM_DESTINATION;
		spells[1].instant = true;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 15;
		spells[1].mindist2cast = 0.0f;
		spells[1].maxdist2cast = 60.0f;

		spells[2].info = dbcSpell.LookupEntry(FLAMESTRIKE);
		spells[2].targettype = TARGET_RANDOM_DESTINATION;
		spells[2].instant = false;
		spells[2].perctrigger = 10.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 15;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 60.0f;

		spells[3].info = dbcSpell.LookupEntry(DAMPEN_MAGIC);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 60;	// not sure at all

		spells[4].info = dbcSpell.LookupEntry(ARCANE_EXPLOSION);
		spells[4].targettype = TARGET_VARIOUS;
		spells[4].instant = false;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 11000;
		spells[4].cooldown = 0;

		ArcaneCheck = 0;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
		spells[3].casttime = (uint32)time(NULL) + spells[3].cooldown;

		if (_unit->GetDistance2dSq(mTarget) <= 1600.0f)
		{
			_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
			_unit->GetAIInterface()->StopMovement(1);
		}

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Common... such a crude language. Bandal!");
		_unit->PlaySoundToSet(11440);

		_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

		ArcaneCheck = 5;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		//_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "BossDied");
		_unit->PlaySoundToSet(11443);
        Matar4TheIllidariConcilio(1,_unit);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Shorel'aran.");
		_unit->PlaySoundToSet(11441);
	}

	void AIUpdate()
	{
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);

		ArcaneCheck--;
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			if (ArcaneCheck <= 0)
			{
				ArcaneCheck = 5;
				UnitPointer Target = NULLUNIT;
				Target = FindTargetForSpell();

				if (Target)
				{
					_unit->CastSpell(_unit, spells[4].info, spells[4].instant);
					return;
				}
			}

			uint32 t = (uint32)time(NULL);
			if (t > spells[3].casttime)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				spells[3].casttime = t + spells[3].cooldown;
				return;
			}

			if (_unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) >= 0.0f && _unit->GetDistance2dSq(_unit->GetAIInterface()->GetNextTarget()) <= 900.0f)
			{
				_unit->GetAIInterface()->setCurrentAgent(AGENT_SPELL);
				_unit->GetAIInterface()->StopMovement(1);
				
				float val = RandomFloat(100.0f);
				SpellCast(val);
			}
		}
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	// A bit rewritten FindTarget function
	UnitPointer FindTargetForSpell()
	{
		UnitPointer target = NULLUNIT;
		float distance = 5.0f;

		UnitPointer pUnit;
		float dist;

		for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
		{
			if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
				continue;

			pUnit = TO_UNIT((*itr));

			if(pUnit->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_FEIGN_DEATH))
				continue;

			if(pUnit->m_invisible)
				continue;
			
			if(!pUnit->isAlive() || _unit == pUnit)
				continue;

			dist = _unit->GetDistance2dSq(pUnit);

			if(dist > distance*distance)
				continue;

			target = pUnit;
			break;
		}

		return target;
	}

protected:

	int ArcaneCheck;
	int nrspells;
};

//------------------------------------
//    -= Gathios the Shatterer =-
//------------------------------------
#define CN_GATHIOS_THE_SHATTERER 22949

#define HAMMER_OF_JUSTICE			41468
#define SEAL_OF_COMMAND				41469
#define SEAL_OF_BLOOD				41459
#define CONSECRATION				41541
#define BLESSING_OF_SPELL_WARDING	41451
#define BLESSING_OF_PROTECTION		41450
#define CHROMATIC_RESISTANCE_AURA	41453
#define DEVOTION_AURA				41452

class GathiosAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(GathiosAI);
	SP_AI_Spell spells[8];
	bool m_spellcheck[8];

    GathiosAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 3;

		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(HAMMER_OF_JUSTICE);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = true;
		spells[0].perctrigger = 10.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 10;
		spells[0].mindist2cast = 10.0f;
		spells[0].maxdist2cast = 40.0f;

		// Next 2 spells share cooldowns (but couldn't have found infos about them)
		spells[1].info = dbcSpell.LookupEntry(SEAL_OF_COMMAND);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 8.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 30;

		spells[2].info = dbcSpell.LookupEntry(SEAL_OF_BLOOD);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = true;
		spells[2].perctrigger = 8.0f;
		spells[2].attackstoptimer = 2000;
		spells[2].cooldown = 30;

		spells[3].info = dbcSpell.LookupEntry(CONSECRATION); 
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = true;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 30;

		// Next 2 spells share cooldowns
		spells[4].info = dbcSpell.LookupEntry(BLESSING_OF_SPELL_WARDING);  
		spells[4].targettype = TARGET_RANDOM_FRIEND;
		spells[4].instant = true;
		spells[4].perctrigger = 0.0f;
		spells[4].attackstoptimer = 2000;
		spells[4].cooldown = 15;
		
		spells[5].info = dbcSpell.LookupEntry(BLESSING_OF_PROTECTION); 
		spells[5].targettype = TARGET_RANDOM_FRIEND;
		spells[5].instant = true;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 2000;
		spells[5].cooldown = 15;

		// Next 2 spells share cooldowns
		spells[6].info = dbcSpell.LookupEntry(CHROMATIC_RESISTANCE_AURA);
		spells[6].targettype = TARGET_RANDOM_FRIEND;
		spells[6].instant = true;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 2000;
		spells[6].cooldown = 60;
		
		spells[7].info = dbcSpell.LookupEntry(DEVOTION_AURA); 
		spells[7].targettype = TARGET_RANDOM_FRIEND;
		spells[7].instant = true;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 2000;
		spells[7].cooldown = 60;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		for (int i = 3; i < 8; i++)
			spells[i].casttime = t + spells[i].cooldown;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have better things to do!");
		_unit->PlaySoundToSet(11422);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Lord Illidan... I...");
		_unit->PlaySoundToSet(11425);
        Matar4TheIllidariConcilio(1,_unit);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Selama amor'anore!");
		_unit->PlaySoundToSet(11423);
	}

	void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			uint32 t = (uint32)time(NULL);
			if (t > spells[3].casttime)
			{
				_unit->setAttackTimer(spells[3].attackstoptimer, false);

				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				spells[3].casttime = t + spells[3].cooldown;
				return;
			}

			if (t > spells[4].casttime)
			{
				UnitPointer TargetToBuff = NULLUNIT;
				UnitPointer Malande = NULLUNIT;
				UnitPointer Zerevor = NULLUNIT;
				UnitPointer Veras = NULLUNIT;

				Malande = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 22951);
				if (Malande && Malande->isAlive())
				{
					TargetToBuff = Malande;
					if (RandomUInt(10) == 1)
					{
						std::vector<UnitPointer > BuffTable;
						Zerevor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 22950);
						Veras = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 22952);

						if (Zerevor && Zerevor->isAlive())
							BuffTable.push_back(Zerevor);
						if (Veras && Veras->isAlive())
							BuffTable.push_back(Veras);

						if (BuffTable.size())
						{
							size_t Target = rand()%BuffTable.size();
							TargetToBuff = BuffTable[Target];
						}
					}
				}

				else
				{
					std::vector<UnitPointer > BuffTable;
					Zerevor = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 22950);
					Veras = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 22952);

					if (Zerevor && Zerevor->isAlive())
						BuffTable.push_back(Zerevor);
					if (Veras && Veras->isAlive())
						BuffTable.push_back(Veras);

					if (BuffTable.size())
					{
						size_t Target = rand()%BuffTable.size();
						TargetToBuff = BuffTable[Target];
					}
				}

				if (TargetToBuff)
				{
					uint32 SpellId = BLESSING_OF_SPELL_WARDING;
					if (RandomUInt(2) == 1)
						SpellId = BLESSING_OF_PROTECTION;

					_unit->setAttackTimer(spells[4].attackstoptimer, false);

					_unit->CastSpell(TargetToBuff, SpellId, spells[4].instant);

					spells[4].casttime = t + spells[4].cooldown;
					return;
				}

				spells[4].casttime = t + spells[4].cooldown;
			}

			if (t > spells[6].casttime)
			{
				uint32 SpellId = CHROMATIC_RESISTANCE_AURA;
				if (RandomUInt(2) == 1)
					SpellId = DEVOTION_AURA;

				_unit->setAttackTimer(spells[6].attackstoptimer, false);

				_unit->CastSpell(_unit, SpellId, spells[6].instant);

				spells[6].casttime = t + spells[6].cooldown;
				return;
			}
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;

					if (i == 1 || i == 2)
					{
						spells[1].casttime = t + spells[1].cooldown;
						spells[2].casttime = t + spells[2].cooldown;
					}
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

//------------------------------------
//    -= Lady Malande =-
//------------------------------------
#define CN_LADY_MALANDE 22951

#define DIVINE_WRATH		41472
#define REFLECTIVE_SHIELD	41475
#define EMPOWERED_SMITE		41471
#define CIRCLE_OF_HEALING	41455

class MalandeAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MalandeAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    MalandeAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DIVINE_WRATH);
		spells[0].targettype = TARGET_RANDOM_SINGLE;
		spells[0].instant = false;
		spells[0].perctrigger = 9.0f;
		spells[0].attackstoptimer = 2000;
		spells[0].cooldown = 15;
		spells[0].mindist2cast = 0.0f;
		spells[0].maxdist2cast = 60.0f;

		spells[1].info = dbcSpell.LookupEntry(REFLECTIVE_SHIELD);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].perctrigger = 6.0f;
		spells[1].attackstoptimer = 2000;
		spells[1].cooldown = 30;

		spells[2].info = dbcSpell.LookupEntry(EMPOWERED_SMITE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = false;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 30;

		spells[3].info = dbcSpell.LookupEntry(CIRCLE_OF_HEALING);
		spells[3].targettype = TARGET_VARIOUS;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 4000;
		spells[3].cooldown = 20;
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		for (int i = 2; i < 4; i++)
			spells[i].casttime = t + spells[i].cooldown;

		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Flee or die!");
		_unit->PlaySoundToSet(11482);

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

       RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Destiny... awaits!");
		_unit->PlaySoundToSet(11485);
        Matar4TheIllidariConcilio(1,_unit);
		RemoveAIUpdateEvent();
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My work is done.");
		_unit->PlaySoundToSet(11483);
	}

	void AIUpdate()
	{
		if (_unit->GetAIInterface()->GetNextTarget() && _unit->GetCurrentSpell() == NULL)
		{
			uint32 t = (uint32)time(NULL);
			if (t > spells[2].casttime)
			{
				UnitPointer target = _unit->GetAIInterface()->GetNextTarget();
				_unit->CastSpell(target, spells[2].info, spells[2].instant);

				spells[2].casttime = t + spells[2].cooldown;
				return;
			}

			if (t > spells[3].casttime)
			{
				_unit->CastSpell(_unit, spells[3].info, spells[3].instant);

				spells[3].casttime = t + spells[3].cooldown;
				return;
			}
		}

		float val = (float)RandomFloat(100.0f);
        SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

//--------------------------Teron Gorefiend-----------------------------//

#define CN_TERON_GOREFIEND 22871

//#define CN_SHADOWY_CONSTRUCTS 23111 //falta agregar este summon al evento
#define DOOM_BLOSSOM		40188	// needs additional creature in DB
#define CRUSHING_SHADOWS	40243
#define INCINERATE			40239
#define SHADOW_OF_DEATH		40251	// need further scripting?
// Dunno where other sounds should go
class TeronGorefiendAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TeronGorefiendAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    TeronGorefiendAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(DOOM_BLOSSOM); //summon visual effect 
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 8.0f;
		spells[0].attackstoptimer = 1000;
		spells[0].cooldown = 25;

		spells[1].info = dbcSpell.LookupEntry(CRUSHING_SHADOWS);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].perctrigger = 10.0f;
		spells[1].attackstoptimer = 1000;
		spells[1].cooldown = 20;

		spells[2].info = dbcSpell.LookupEntry(INCINERATE);
		spells[2].targettype = TARGET_RANDOM_SINGLE; 
		spells[2].instant = true;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;
		spells[2].cooldown = 20;
		spells[2].mindist2cast = 0.0f;
		spells[2].maxdist2cast = 45.0f;

		spells[3].info = dbcSpell.LookupEntry(SHADOW_OF_DEATH);
		spells[3].targettype = TARGET_RANDOM_SINGLE;
		spells[3].instant = false;
		spells[3].perctrigger = 0.0f;
		spells[3].attackstoptimer = 2000;
		spells[3].cooldown = 30;
		spells[3].mindist2cast = 0.0f;
		spells[3].maxdist2cast = 60.0f;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Vengeance is mine!");
		_unit->PlaySoundToSet(11513);

		for (int i = 0; i < 4; i++)
			spells[i].casttime = 0;

		uint32 t = (uint32)time(NULL);
		spells[2].casttime = t + spells[2].cooldown;
		spells[3].casttime = t + 10;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			switch (RandomUInt(2))
			{
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It gets worse...");
				_unit->PlaySoundToSet(11515);
				break;
			default:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I have use for you!");
					_unit->PlaySoundToSet(11514);
				}
			}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The wheel...spins...again....");
		_unit->PlaySoundToSet(11521);
		Matar4firstboss(1,_unit);

		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		uint32 t = (uint32)time(NULL);
		if (t > spells[2].casttime)
		{
			_unit->setAttackTimer(spells[2].attackstoptimer, false);

			CastSpellOnRandomTarget(2, spells[2].mindist2cast, spells[2].maxdist2cast, 0, 100);

			spells[2].casttime = t + spells[2].cooldown;
		}

		if (t > spells[3].casttime)
		{
			_unit->setAttackTimer(spells[3].attackstoptimer, false);

			CastSpellOnRandomTarget(3, spells[3].mindist2cast, spells[3].maxdist2cast, 0, 100);

			spells[3].casttime = t + spells[3].cooldown;
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

protected:

	int nrspells;
};

//////////////////////////minion's teron gorefrend///// y m la maskan
// Shadowy ConstructsAI
#define ATROPHY 40327

class  ShadowyconstructsAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadowyconstructsAI);

    ShadowyconstructsAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->CastSpell(_unit,39205,true);
		RegisterAIUpdateEvent(5000);

		std::vector<UnitPointer > TargetTable;		
		for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if ( (*itr) && (*itr)->IsPlayer() )
			{
				PlayerPointer tg = TO_PLAYER( (*itr) );
				if( tg && tg->isAlive() )
					TargetTable.push_back(tg);
			}
		}
			
		if (!TargetTable.size())
			return;
		size_t RandTarget = rand()%TargetTable.size();
		UnitPointer RTarget = TargetTable[RandTarget];
		if (!RTarget)
			return;

		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		_unit->GetAIInterface()->AttackReaction(RTarget, 100000, 0);

		TargetTable.clear();
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		std::vector<UnitPointer > TargetTable;		
		for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if ( (*itr) && (*itr)->IsPlayer() )
			{
				UnitPointer tg = TO_UNIT(*itr);
				if( tg && tg->isAlive() && !tg->HasAura(SHADOW_OF_DEATH) )
				{
					float dist = tg->GetDistance2dSq( static_cast< ObjectPointer >( _unit ) );
					if( dist <= 150.0f )
						_unit->CastSpell(tg,ATROPHY,true);
				}
			}
		}	
		
    }

	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch (iWaypointId)
		{
		case 1:
			{
				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->m_canMove = true;
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->setWaypointToMove(0);
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

			}break;
		}
    }
};

// doom bloss
#define DOOM_BLOSSOM_BOLT 40876
class DOOMBLOSSOMAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DOOMBLOSSOMAI);

    DOOMBLOSSOMAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		RegisterAIUpdateEvent(5000);			
    }
    
    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		std::vector<UnitPointer > TargetTable;		
		for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if ( (*itr) && (*itr)->IsPlayer() )
			{
				UnitPointer tg = TO_UNIT(*itr);
				if( tg && tg->isAlive() )
					TargetTable.push_back(tg);
			}
				
			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			_unit->CastSpell(RTarget,DOOM_BLOSSOM_BOLT,true);

			TargetTable.clear();
		}
	}
};

//////////////////////////////////////////
//////////////////////////////////////////
////////shade of akama by soulshifter/////
//////////////////////////////////////////
//////////////////////////////////////////
//Coords y Variables de inicio
struct Gates
{
	float x;
	float y;
	float z;
	float o;
};

struct Gates Side[]=
{
	{499.408977f, 465.714996f, 112.783997f, 4.822640f},
	{498.165985f, 335.088989f, 112.783997f, 1.441500f}
};

struct Coord
{
	float x;
	float y;
	float z;
};

// Channelers Coords
struct Coord Channelers[]=
{
	{443.495f, 413.973f, 118.721f},
	{456.694f, 414.117f, 118.721f},
	{457.763f, 390.117f, 118.721f},
	{444.153f, 389.214f, 118.721f},
	{438.279f, 401.258f, 118.721f},
	{463.186f, 401.64f, 118.721f}
};

struct Coord Brokens[]=
{
	{499.714294f, 396.856964f, 112.783897f},
	{499.714294f, 404.856964f, 112.783897f},
	{499.714294f, 392.856964f, 112.783897f},
	{499.714294f, 408.856964f, 112.783897f},
	{499.714294f, 400.856964f, 112.783897f}
};

// Event Start
bool EventStarted = false;
bool EventDone = false;
bool ShadeOfAkamaDeath = false;
bool ShadeOfAkamaActive = false;
bool AkamaFakeDeath = false;
bool BrokenAshtongueFriendly = false;
bool DefenderOut = false;

// Shade of AkamaAI
// Definiciones

#define CN_SHADE_OF_AKAMA			22841
#define CN_AKAMAFAKE				90006

// Adds
#define CN_ASHTONGUE_CHANNELER		23421
#define CN_ASHTONGUE_DEFENDER		23216
#define CN_ASHTONGUE_ELEMENTALIST	23523
#define CN_ASHTONGUE_ROGUE			23318
#define CN_ASHTONGUE_SPIRITBINDER	23524
#define CN_ASHTONGUE_SORCERER		23215
#define CN_BROKEN_ASHTONGUE			23319
// Texts
#define SOUND_DEATH					11386
#define SAY_DEATH					"No! Not yet..."
#define SOUND_LOW_HEALTH			11385
#define SAY_LOW_HEALTH				"I will not last much longer..."

// Ending cinematic text
#define SAY_FREE0					"Broken of the Ashtongue tribe, your leader speaks!"
#define SAY_FREE1                   "Come out from the shadows! I've returned to lead you against our true enemy! Shed your chains and raise your weapons against your Illidari masters!"
#define SAY_BROKEN_FREE_01          "Hail our leader! Hail Akama!"
#define SAY_BROKEN_FREE_02          "Hail Akama!"

// Gossips
#define GOSSIP_ITEM                 "We are ready to fight alongside you, Akama"

// Spells
#define SPELL_SHADOWFORM		    39833
#define SPELL_DESTRUCTIVE_POISON    40874
#define SPELL_LIGHTNING_BOLT        42024
#define SPELL_AKAMA_SOUL_CHANNEL    40447
#define SPELL_AKAMA_SOUL_RETRIEVE   40902
#define AKAMA_SOUL_EXPEL            40927

class ShadeofakamaAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(ShadeofakamaAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    ShadeofakamaAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 
		spells[0].info = dbcSpell.LookupEntry(SPELL_DESTRUCTIVE_POISON);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 2000;
		
		_unit->CastSpell(_unit, SPELL_SHADOWFORM, false); //ShadowForm Visual
		_unit->m_canMove = false;
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		AkamaTarget = NULLCREATURE;
		AkamaTarget = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(498.972229f, 400.349823f, 112.783951f, CN_AKAMAFAKE);
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }
    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(UnitPointer mKiller)
    {
	/*	NO BLIZZLIKE, agregado spawn luego de matar The illidari concilium
	
	    CreaturePointer cre = NULLCREATURE;  
        cre = _unit->GetMapMgr()->GetInterface()->SpawnCreature(22990, 643.741f, 305.852f, 271.689f, 0.00628f, true, false, 0, 0); 
	*/ 
		ShadeOfAkamaDeath = true;
		Matar4firstboss(1,_unit);
		RemoveAIUpdateEvent();
    }
    void AIUpdate()
	{
		if(AkamaTarget && _unit->GetAIInterface()->GetNextTarget() != AkamaTarget)
		{
			_unit->GetAIInterface()->SetNextTarget(AkamaTarget);
			_unit->GetAIInterface()->AttackReaction(AkamaTarget, 100000000, 0);
		}
		if(AkamaFakeDeath)
		{
			_unit->Despawn(1, 0);
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
	}

    void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

                  	m_spellcheck[i] = false;
					return;
				}

				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger))
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}
protected:
	int nrspells;
	CreaturePointer AkamaTarget;
};


//mobs

// Ashtongue Defender AI
#define DEBILITATING_STRIKE 41178 //


class AshtonguedefenderAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguedefenderAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtonguedefenderAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEBILITATING_STRIKE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = true;
		spells[0].perctrigger = 3.0f;
		spells[0].attackstoptimer = 1000;

		RegisterAIUpdateEvent(1000);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }


	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		/*_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);*/
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(ShadeOfAkamaDeath || AkamaFakeDeath || DefenderOut)
		{
			_unit->Despawn(1, 0);
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
};

// AshtongueElementalist AI
#define RAIN_OF_FIRE 42023


class AshtongueelementalistAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtongueelementalistAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtongueelementalistAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(RAIN_OF_FIRE);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(ShadeOfAkamaDeath || ShadeOfAkamaActive || AkamaFakeDeath)
		{
			_unit->Despawn(1, 0);
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
};

// AshtongueRogue AI
#define DEBILITATING_POISON 41978


class AshtonguerogueAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguerogueAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtonguerogueAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(DEBILITATING_POISON);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(ShadeOfAkamaDeath || ShadeOfAkamaActive || AkamaFakeDeath)
		{
			_unit->Despawn(1, 0);
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
};

// AshtongueRogue AI
#define SPIRIT_MEND 42025
#define AS_CHAIN_HEAL 42025


class AshtonguespiritbinderAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtonguespiritbinderAI);
	SP_AI_Spell spells[2];
	bool m_spellcheck[2];

    AshtonguespiritbinderAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(SPIRIT_MEND);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].perctrigger = 5.0f;
		spells[0].attackstoptimer = 1000;
		
		spells[1].info = dbcSpell.LookupEntry(AS_CHAIN_HEAL);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = false;
		spells[1].perctrigger = 5.0f;
		spells[1].attackstoptimer = 1000;

		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }

	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(ShadeOfAkamaDeath || ShadeOfAkamaActive || AkamaFakeDeath)
		{
			_unit->Despawn(1, 0);
		}
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
	{
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				spells[i].casttime--;
				
				if (m_spellcheck[i])
				{					
					spells[i].casttime = spells[i].cooldown;
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					if (spells[i].speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
						_unit->PlaySoundToSet(spells[i].soundid); 
					}

					m_spellcheck[i] = false;
					return;
				}

				if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
		}
	}

protected:

	int nrspells;
};

// Ashtongue Channeler AI
#define AKAMA_CHANNEL 40401


class AshtongueChannelerAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtongueChannelerAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtongueChannelerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		RegisterAIUpdateEvent(1000);
		UnitPointer SoAkama = NULLUNIT;
		SoAkama = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(442.804f, 401.362f ,118.538f , CN_SHADE_OF_AKAMA);
		if(SoAkama)
		{
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, SoAkama->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_CHANNEL);
		}
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }


	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(EventStarted)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		}
		if(AkamaFakeDeath)
		{
			_unit->Despawn(1, 0);
		}
    }

protected:
};

class AshtongueSorcererAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AshtongueSorcererAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AshtongueSorcererAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->m_canMove = false;
		_unit->GetAIInterface()->disable_melee = true;
		//_unit->setAttackTimer(2000, false);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		RegisterAIUpdateEvent(1000);
		UnitPointer SoAkama = NULLUNIT;
		SoAkama = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(442.804f, 401.362f ,118.538f , CN_SHADE_OF_AKAMA);
		if(SoAkama)
		{
			_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, SoAkama->GetGUID());
			_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_CHANNEL);
		}
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
    }


	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
    }

    void OnDied(UnitPointer mKiller)
    {
		RemoveAIUpdateEvent();
    }

    void AIUpdate()
	{
		if(EventStarted)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		}
		if(ShadeOfAkamaActive)
		{
			_unit->Despawn(1, 0);
		}
    }

protected:
};

// Trigger Shade of Akama
#define CN_TRIGGERSOFAKAMA		90007
#define CN_SOULEXPELTRIGGER		90008	
class TriggerShadeofAkamaAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(TriggerShadeofAkamaAI);
    TriggerShadeofAkamaAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_canMove = false;
		RegisterAIUpdateEvent(1000);//TIEMPO
		EventTimer = 35;
		DefenderTimer = 20 + RandomUInt(15);
		SorceressTimer = 60 + RandomUInt(20);
		EndingEvent = 0;
		EventDone = false;
		ShadeOfAkamaDeath = false;
		ShadeOfAkamaActive = false;
		AkamaFakeDeath = false;
		BrokenAshtongueFriendly = false;

		Channeler1 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[0].x, Channelers[0].y, Channelers[0].z, CN_ASHTONGUE_CHANNELER);
		Channeler2 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[1].x, Channelers[1].y, Channelers[1].z, CN_ASHTONGUE_CHANNELER);
		Channeler3 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[2].x, Channelers[2].y, Channelers[2].z, CN_ASHTONGUE_CHANNELER);
		Channeler4 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[3].x, Channelers[3].y, Channelers[3].z, CN_ASHTONGUE_CHANNELER);
		Channeler5 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[4].x, Channelers[4].y, Channelers[4].z, CN_ASHTONGUE_CHANNELER);
		Channeler6 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[5].x, Channelers[5].y, Channelers[5].z, CN_ASHTONGUE_CHANNELER);
		SofAkama = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(451.451996f, 401.210999f, 118.538002f, CN_SHADE_OF_AKAMA);
		AkamaFake = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(498.972229f, 400.349823f, 112.783951f, CN_AKAMAFAKE);

	}
	void AIUpdate()
		{
			if(!AkamaFake)
				AkamaFake = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(498.972229f, 400.349823f, 112.783951f, CN_AKAMAFAKE);
			
			if (!ShadeOfAkamaActive)
			{
				CheckChannelers();
			}
			if (!EventTimer && !ShadeOfAkamaDeath && !ShadeOfAkamaActive)
			{
				NormalPacks();
			}
			if (!DefenderTimer && !ShadeOfAkamaDeath && !ShadeOfAkamaActive)
			{
				UnitPointer Defender = NULLUNIT;
				Defender = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_DEFENDER, Side[0].x, Side[0].y, Side[0].z, Side[0].o, true, false, 0, 0);
				Defender->GetAIInterface()->setMoveRunFlag(true);
					switch(RandomUInt(2))
					{				
						case 0:
							Defender->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Defender->GetOrientation());
							break;
						case 1:
							Defender->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Defender->GetOrientation());
							break;
					}
				DefenderTimer = 15;
			}
			if (!SorceressTimer && !ShadeOfAkamaDeath && !ShadeOfAkamaActive)
			{
				Sorceress = NULLUNIT;
				Sorceress = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_SORCERER, Side[0].x, Side[0].y, Side[0].z, Side[0].o, true, false, 0, 0);
				Sorceress->GetAIInterface()->setMoveRunFlag(true);
					switch(RandomUInt(2))
					{
						case 0:
							Sorceress->GetAIInterface()->MoveTo((Channelers[1].x + 1.0f), (Channelers[1].y + 1.0f), Channelers[1].z, Sorceress->GetOrientation());
							break;
						case 1:
							Sorceress->GetAIInterface()->MoveTo((Channelers[1].x + 1.0f), (Channelers[1].y + 1.0f), Channelers[1].z, Sorceress->GetOrientation());
							break;
					}
				SorceressTimer = 60 + RandomUInt(10);
			}
			if(AkamaFakeDeath)
			{
				AkamaDeathReset();
			}
			if (ShadeOfAkamaDeath)
			{
				switch(EndingEvent)
				{
				case 0:
					AkamaFake->GetAIInterface()->setMoveRunFlag(false);
					SoulExpelSoA = NULLUNIT;
					SoulExpelSoA = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOULEXPELTRIGGER, SofAkama->GetPositionX(), SofAkama->GetPositionY(), SofAkama->GetPositionZ(), SofAkama->GetOrientation(), true, false, 0, 0);
					SoulExpelSoA->GetAIInterface()->MoveTo(SoulExpelSoA->GetPositionX(), SoulExpelSoA->GetPositionY(), (SoulExpelSoA->GetPositionZ() + 20.0f), 6.270652f);
					break;
				case 2:
					AkamaFake->GetAIInterface()->MoveTo(463.186005f, 401.640015f, 118.721001f, AkamaFake->GetOrientation());
					break;
				case 16:
					AkamaFake->SetFacing(6.270652f);
					SoulExpelAkama = NULLUNIT;
					SoulExpelAkama = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SOULEXPELTRIGGER, AkamaFake->GetPositionX(), AkamaFake->GetPositionY(), AkamaFake->GetPositionZ(), SofAkama->GetOrientation(), true, false, 0, 0);
					SoulExpelAkama->GetAIInterface()->MoveTo(SoulExpelAkama->GetPositionX(), SoulExpelAkama->GetPositionY(), (SoulExpelAkama->GetPositionZ() + 20.0f), 3.112556f);
					break;
				case 18:
					AkamaFake->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, SofAkama->GetGUID());
					AkamaFake->SetUInt32Value(UNIT_CHANNEL_SPELL, SPELL_AKAMA_SOUL_RETRIEVE);
					break;
				case 25:
					SofAkama->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, AkamaFake->GetGUID());
					SofAkama->SetUInt32Value(UNIT_CHANNEL_SPELL, SPELL_AKAMA_SOUL_RETRIEVE);
					_unit->CastSpell(SoulExpelSoA, AKAMA_SOUL_EXPEL, false);
					SoulExpelSoA->CastSpell(_unit, AKAMA_SOUL_EXPEL, false);
					SoulExpelAkama->CastSpell(AkamaFake, AKAMA_SOUL_EXPEL, false);
					AkamaFake->CastSpell(SoulExpelAkama, AKAMA_SOUL_EXPEL, false);
					break;
				case 45:
					SofAkama->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					SofAkama->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					AkamaFake->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					AkamaFake->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					break;
				case 50:
					AkamaFake->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_FREE0);
					AkamaFake->Emote(EMOTE_ONESHOT_POINT);
					break;
				case 55:
					BrokenAshtongue1 = NULLUNIT;
					BrokenAshtongue2 = NULLUNIT;
					BrokenAshtongue3 = NULLUNIT;
					BrokenAshtongue4 = NULLUNIT;
					BrokenAshtongueL = NULLUNIT;
					BrokenAshtongue1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BROKEN_ASHTONGUE, Brokens[0].x, Brokens[0].y, Brokens[0].z, 3.112556f, true, false, 0, 0);
					BrokenAshtongue1->GetAIInterface()->MoveTo((Brokens[0].x - 4.0f), Brokens[0].y, Brokens[0].z, BrokenAshtongue1->GetOrientation());
					BrokenAshtongue2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BROKEN_ASHTONGUE, Brokens[1].x, Brokens[1].y, Brokens[1].z, 3.112556f, true, false, 0, 0);
					BrokenAshtongue2->GetAIInterface()->MoveTo((Brokens[1].x - 4.0f), Brokens[1].y, Brokens[1].z, BrokenAshtongue2->GetOrientation());
					BrokenAshtongue3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BROKEN_ASHTONGUE, Brokens[2].x, Brokens[2].y, Brokens[2].z, 3.112556f, true, false, 0, 0);
					BrokenAshtongue3->GetAIInterface()->MoveTo((Brokens[2].x - 4.0f), Brokens[2].y, Brokens[2].z, BrokenAshtongue3->GetOrientation());
					BrokenAshtongue4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BROKEN_ASHTONGUE, Brokens[3].x, Brokens[3].y, Brokens[3].z, 3.112556f, true, false, 0, 0);
					BrokenAshtongue4->GetAIInterface()->MoveTo((Brokens[3].x - 4.0f), Brokens[3].y, Brokens[3].z, BrokenAshtongue4->GetOrientation());
					BrokenAshtongueL = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BROKEN_ASHTONGUE, 499.714294f, 400.856964f, 112.783897f, 3.112556f, true, false, 0, 0);
					BrokenAshtongueL->GetAIInterface()->MoveTo((499.714294f - 8.0f), 400.856964f, 112.783897f, BrokenAshtongueL->GetOrientation());
					break;
				case 70:
					AkamaFake->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_FREE1);
					AkamaFake->Emote(EMOTE_ONESHOT_CHEER);
					break;
				case 75:
					BrokenAshtongue1->Emote(EMOTE_ONESHOT_KNEEL);
					BrokenAshtongue2->Emote(EMOTE_ONESHOT_KNEEL);
					BrokenAshtongue3->Emote(EMOTE_ONESHOT_KNEEL);
					BrokenAshtongue4->Emote(EMOTE_ONESHOT_KNEEL);
					BrokenAshtongueL->Emote(EMOTE_ONESHOT_KNEEL);
					break;
				case 78:
					BrokenAshtongueFriendly = true;
					BrokenAshtongueL->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_BROKEN_FREE_01);
					BrokenAshtongueL->Emote(EMOTE_ONESHOT_SALUTE);
					break;
				case 83:
					BrokenAshtongue1->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_BROKEN_FREE_02);
					BrokenAshtongue1->Emote(EMOTE_ONESHOT_SALUTE);
					BrokenAshtongue2->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_BROKEN_FREE_02);
					BrokenAshtongue2->Emote(EMOTE_ONESHOT_SALUTE);
					BrokenAshtongue3->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_BROKEN_FREE_02);
					BrokenAshtongue3->Emote(EMOTE_ONESHOT_SALUTE);
					BrokenAshtongue4->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_BROKEN_FREE_02);
					BrokenAshtongue4->Emote(EMOTE_ONESHOT_SALUTE);
					break;
				case 93:
					EventDone = true;
					break;
				case 100:
					Reset();
					_unit->Despawn(1000,0);
					break;
				}
				EndingEvent++;
			}
			EventTimer--;
			DefenderTimer--;
			SorceressTimer--;
		}
	void CheckChannelers()
	{
		/*Channeler1 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[0].x, Channelers[0].y, Channelers[0].z, CN_ASHTONGUE_CHANNELER);
		Channeler2 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[1].x, Channelers[1].y, Channelers[1].z, CN_ASHTONGUE_CHANNELER);
		Channeler3 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[2].x, Channelers[2].y, Channelers[2].z, CN_ASHTONGUE_CHANNELER);
		Channeler4 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[3].x, Channelers[3].y, Channelers[3].z, CN_ASHTONGUE_CHANNELER);
		Channeler5 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[4].x, Channelers[4].y, Channelers[4].z, CN_ASHTONGUE_CHANNELER);
		Channeler6 =_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(Channelers[5].x, Channelers[5].y, Channelers[5].z, CN_ASHTONGUE_CHANNELER);
		SofAkama = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(451.451996f, 401.210999f, 118.538002f, CN_SHADE_OF_AKAMA);
		AkamaFake = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(498.972229f, 400.349823f, 112.783951f, CN_AKAMAFAKE);*/
		if((!Channeler1 || !Channeler1->isAlive())&& (!Channeler2 || !Channeler2->isAlive()) && (!Channeler3 || !Channeler3->isAlive()) && (!Channeler4 || !Channeler4->isAlive()) && (!Channeler5 || !Channeler5->isAlive()) && (!Channeler6 || !Channeler6->isAlive())/*&& (!Sorceress || Sorceress->isAlive())*/)
		{		
			Channeler1 = NULLUNIT;
			Channeler2 = NULLUNIT;
			Channeler3 = NULLUNIT;
			Channeler4 = NULLUNIT;
			Channeler5 = NULLUNIT;
			Channeler6 = NULLUNIT;
			AkamaFake->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
			AkamaFake->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
			SofAkama->SetUInt32Value(UNIT_FIELD_FLAGS, 0);
			SofAkama->m_canMove = true;
			SofAkama->GetAIInterface()->SetNextTarget(AkamaFake);
			SofAkama->GetAIInterface()->AttackReaction(AkamaFake, 1000000, 0);
			SofAkama->GetAIInterface()->SetAllowedToEnterCombat(true);
			ShadeOfAkamaActive = true;
		}
		else
		{
			if((AkamaFake->GetPositionX() == 498.972229f) && (AkamaFake->GetPositionY() == 400.349823f) && (AkamaFake->GetPositionZ() == 112.783951f)) 
			{
				if(SofAkama && AkamaFake && !ShadeOfAkamaDeath)
				{
					AkamaFake->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, SofAkama->GetGUID());
					AkamaFake->SetUInt32Value(UNIT_CHANNEL_SPELL, SPELL_AKAMA_SOUL_CHANNEL);
				}
			}
		}
	}
	void NormalPacks()
	{
		UnitPointer Ashtongue1 = NULLUNIT;
		UnitPointer Ashtongue2 = NULLUNIT;
		UnitPointer Ashtongue3 = NULLUNIT;
		UnitPointer Ashtongue4 = NULLUNIT;
		UnitPointer Ashtongue5 = NULLUNIT;
		UnitPointer Ashtongue6 = NULLUNIT;
		// Left Door
		Ashtongue1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_SPIRITBINDER, Side[0].x, Side[0].y, Side[0].z, Side[0].o, true, false, 0, 0);
		Ashtongue2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_ROGUE, (Side[0].x + 2.0f), Side[0].y, Side[0].z, Side[0].o, true, false, 0, 0);
		Ashtongue3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_ELEMENTALIST, (Side[0].x - 2.0f), Side[0].y, Side[0].z, Side[0].o, true, false, 0, 0);
		Ashtongue1->GetAIInterface()->setMoveRunFlag(true);
		Ashtongue2->GetAIInterface()->setMoveRunFlag(true);
		Ashtongue3->GetAIInterface()->setMoveRunFlag(true);
		Ashtongue1->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Ashtongue1->GetOrientation());
		Ashtongue2->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Ashtongue2->GetOrientation());
		Ashtongue3->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Ashtongue3->GetOrientation());
		// Right Door
		Ashtongue4 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_SPIRITBINDER, Side[1].x, Side[1].y, Side[1].z, Side[1].o, true, false, 0, 0);
		Ashtongue5 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_ROGUE, (Side[1].x + 2.0f), Side[1].y, Side[1].z, Side[1].o, true, false, 0, 0);
		Ashtongue6 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_ELEMENTALIST, (Side[1].x - 2.0f), Side[1].y, Side[1].z, Side[1].o, true, false, 0, 0);
		Ashtongue4->GetAIInterface()->setMoveRunFlag(true);
		Ashtongue5->GetAIInterface()->setMoveRunFlag(true);
		Ashtongue6->GetAIInterface()->setMoveRunFlag(true);
		Ashtongue4->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Ashtongue4->GetOrientation());
		Ashtongue5->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Ashtongue5->GetOrientation());
		Ashtongue6->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, Ashtongue6->GetOrientation());
		EventTimer = 45;
	}
	void Reset()
	{
		AkamaFake = NULLUNIT;
		Channeler1 = NULLUNIT;
		Channeler2 = NULLUNIT;
		Channeler3 = NULLUNIT;
		Channeler4 = NULLUNIT;
		Channeler5 = NULLUNIT;
		Channeler6 = NULLUNIT;
		Sorceress = NULLUNIT;
		BrokenAshtongue1 = NULLUNIT;
		BrokenAshtongue2 = NULLUNIT;
		BrokenAshtongue3 = NULLUNIT;
		BrokenAshtongue4 = NULLUNIT;
		BrokenAshtongueL = NULLUNIT;
		SofAkama = NULLCREATURE;
		SoulExpelSoA = NULLUNIT;
		SoulExpelAkama = NULLUNIT;
	}
	void AkamaDeathReset()
	{
		SofAkama->Despawn(1,0);
		//Spawnear a Shade
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_SHADE_OF_AKAMA, 449.595f, 401.194f, 118.721f, 0.0f, true, false, 0, 0);
		//Spawnear los channelers
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, 463.186f, 401.64f, 118.721f, 3.19395f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, 438.279f, 401.258f, 118.721f, 0.0174533f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, 444.153f, 389.214f, 118.721f, 1.0821f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, 457.763f, 390.117f, 118.721f, 2.25147f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, 456.694f, 414.117f, 118.721f, 4.2586f, true, false, 0, 0);
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_ASHTONGUE_CHANNELER, 443.495f, 413.973f, 118.721f, 5.13127f, true, false, 0, 0);
		//Spawnear akama
		_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_AKAMAFAKE, 528.158f, 400.446f, 112.784f, 3.07629f, true, false, 0, 0);
		_unit->Despawn(999, 0);
		// Event Restart
		//AkamaFake = NULL;
		/*Channeler1 = NULL;
		Channeler2 = NULL;
		Channeler3 = NULL;
		Channeler4 = NULL;
		Channeler5 = NULL;
		Channeler6 = NULL;
		Sorceress = NULLUNIT;
		BrokenAshtongue1 = NULL;
		BrokenAshtongue2 = NULL;
		BrokenAshtongue3 = NULL;
		BrokenAshtongue4 = NULL;
		BrokenAshtongueL = NULL;
		SofAkama = NULL;
		SoulExpelSoA = NULLUNIT;
		SoulExpelAkama = NULL;*/
		EventStarted = false;
		EventDone = false;
		ShadeOfAkamaDeath = false;
		ShadeOfAkamaActive = false;
		AkamaFakeDeath = false;
		BrokenAshtongueFriendly = false;
		DefenderOut = true;
	}
protected:
uint32 EventTimer;
uint32 DefenderTimer;
uint32 SorceressTimer;
uint32 EndingEvent;
UnitPointer AkamaFake;
UnitPointer Channeler1;
UnitPointer Channeler2;
UnitPointer Channeler3;
UnitPointer Channeler4;
UnitPointer Channeler5;
UnitPointer Channeler6;
UnitPointer Sorceress;
UnitPointer BrokenAshtongue1;
UnitPointer BrokenAshtongue2;
UnitPointer BrokenAshtongue3;
UnitPointer BrokenAshtongue4;
UnitPointer BrokenAshtongueL;
CreaturePointer SofAkama;
UnitPointer SoulExpelSoA;
UnitPointer SoulExpelAkama;
};
class AkamaFakeAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(AkamaFakeAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    AkamaFakeAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;

		} 

		spells[0].info = dbcSpell.LookupEntry(SPELL_LIGHTNING_BOLT);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].perctrigger = 20.0f;
		spells[0].attackstoptimer = 2000;

		TimerLowHealth = 1;
		TimerBolt = 1;
    }   

    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->AttackReaction((_unit->GetAIInterface()->GetNextTarget()), 1000000, 0);
		RegisterAIUpdateEvent(1000);
    }
	void OnTargetDied(UnitPointer mTarget)
    {
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->Despawn(3000, 0);
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_DEATH);
		_unit->PlaySoundToSet(SOUND_DEATH);
		_unit->Despawn(1,0);
		AkamaFakeDeath = true;
    }

    void AIUpdate()
	{
		if(_unit->GetHealthPct() < 15 && !TimerLowHealth)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, SAY_LOW_HEALTH);
			_unit->PlaySoundToSet(SOUND_LOW_HEALTH);
			TimerLowHealth = 4;
		}
		if(EventDone)
		{
			_unit->Despawn(1000, 0);
		}
		if(!TimerBolt)
		{
			_unit->CastSpell(_unit->GetAIInterface()->GetNextTarget(), SPELL_LIGHTNING_BOLT, false);
		}
		TimerLowHealth--;
		TimerBolt--;
		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }
	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
	
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }
protected:
int nrspells;
uint32 TimerLowHealth;
uint32 TimerBolt;
};

class BrokenAshtongueAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(BrokenAshtongueAI);
    BrokenAshtongueAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
		_unit->GetAIInterface()->setMoveRunFlag(false);
		RegisterAIUpdateEvent(1000);
    }
	void AIUpdate()
	{
		if(EventDone)
			{
				_unit->Despawn(1000, 0);
			}	
		if (BrokenAshtongueFriendly)
			_unit->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
	}
protected:
};

/* Akama AI - Creature Gossip Class */
 
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, plr); \
    Menu->SendTo(plr);
 
class AkamaFakeGossip : public GossipScript
{
public:
    void GossipHello(ObjectPointer pObject, PlayerPointer plr, bool AutoSend)
    {
        GossipMenu *Menu;
		CreaturePointer Akama = TO_CREATURE(pObject);
		if (Akama == NULL)
			return;
		if (Akama)
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			Menu->AddItem( 0, GOSSIP_ITEM, 1);
		}
	 
		if(AutoSend)
			Menu->SendTo(plr);
    }
 
	void GossipSelectOption(ObjectPointer pObject, PlayerPointer plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		CreaturePointer Akama = TO_CREATURE(pObject);
		if (Akama == NULL)
			return;

		switch (IntId)
		{
		case 0:
			GossipHello(pObject, plr, true);
			break;
		case 1:
			Akama->SetUInt32Value(UNIT_NPC_FLAGS, 0);
			Akama->GetAIInterface()->setMoveRunFlag(false);
			Akama->GetAIInterface()->MoveTo(498.972229f, 400.349823f, 112.783951f, 3.112556f);
			Akama->SetFacing(3.112556f);
			Akama->GetMapMgr()->GetInterface()->SpawnCreature(CN_TRIGGERSOFAKAMA, 468.826477f, 401.076904f, 118.537880f, 3.112556f, true, false, 0 , 0);
			EventStarted = true;
			DefenderOut = false;
			break;
		}
	}
 
    void Destroy()
    {
        delete this;
    }
};

//////////////////////////////////////////////////
/////////// illidan encounter ////////////////////
// ---    Illidan Encounter Event      --- //

/* Global Variables */

// Additional stuff
uint32 m_phase[1000000];	// dunno if it's enough (uint32 InstanceID -> 4,294,967,295)
uint32 FoADeaths[1000000];
uint32 MaievDialog[1000000];

// Events
uint32 BladesEvent[1000000];
uint32 DeathEvent[1000000];

#define WALK 0
#define RUN 256
#define FLY 768

// Global definitions - comment to disable
#define USE_SHADOW_PRISON	// aggroes a lot of mobs/NPCs if they are not friendly to Illidan

/* Effect and Trigger AIs */

/* Door Event Trigger AI - Creature AI Class */

#define CN_DOOR_EVENT_TRIGGER 30001

class DoorEventTriggerAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DoorEventTriggerAI);

    DoorEventTriggerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->m_moveFly = true;
    }
};

/* Demon Fire AI - Creature AI Class */

#define CN_DEMON_FIRE 23069

#define DEMON_FIRE 40029

class DemonFireAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(DemonFireAI);

    DemonFireAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(DEMON_FIRE), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(60000);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(60000);
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		_unit->Despawn(0,0);
    }

protected:

	int nrspells;
};

/* Eye Beam Trigger AI - Creature AI Class */

#define CN_EYE_BEAM_TRIGGER 30000

#define EYE_BLAST 40017	//spawning doesn't work so we will use little hack

class EyeBeamTriggerAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(EyeBeamTriggerAI);

    EyeBeamTriggerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);		// temp. fix
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_moveRun = true;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(1000);

		WallCreating = false;
		DespawnTimer = 12;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(1000);
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		if (WallCreating == false)
		{
			if (_unit->GetPositionX() == 642.240601f && _unit->GetPositionY() == 297.297180f && _unit->GetPositionZ() == 353.423401f)
				_unit->GetAIInterface()->MoveTo(707.019043f, 270.441772f, 352.996063f, 5.953270f);
			if (_unit->GetPositionX() == 641.197449f && _unit->GetPositionY() == 314.330963f && _unit->GetPositionZ() == 353.300262f)
				_unit->GetAIInterface()->MoveTo(706.592407f, 343.425568f, 352.996124f, 3.659908f);
			if (_unit->GetPositionX() == 657.239807f && _unit->GetPositionY() == 256.925568f && _unit->GetPositionZ() == 352.996094f)
				_unit->GetAIInterface()->MoveTo(706.593262f, 310.011475f, 353.693848f, 2.506154f);
			if (_unit->GetPositionX() == 657.913330f && _unit->GetPositionY() == 353.562775f && _unit->GetPositionZ() == 352.996185f)
				_unit->GetAIInterface()->MoveTo(706.751343f, 298.312683f, 353.653809f, 3.436852f);
			if (_unit->GetPositionX() == 707.019043f && _unit->GetPositionY() == 270.441772f && _unit->GetPositionZ() == 352.996063f)
				_unit->GetAIInterface()->MoveTo(642.240601f, 297.297180f, 353.423401f, 5.953270f);
			if (_unit->GetPositionX() == 706.592407f && _unit->GetPositionY() == 343.425568f && _unit->GetPositionZ() == 352.996124f)
				_unit->GetAIInterface()->MoveTo(641.197449f, 314.330963f, 353.300262f, 3.659908f);
			if (_unit->GetPositionX() == 706.593262f && _unit->GetPositionY() == 310.011475f && _unit->GetPositionZ() == 353.693848f)
				_unit->GetAIInterface()->MoveTo(657.239807f, 256.925568f, 352.996094f, 2.506154f);
			if (_unit->GetPositionX() == 706.751343f && _unit->GetPositionY() == 298.312683f && _unit->GetPositionZ() == 353.653809f)
				_unit->GetAIInterface()->MoveTo(657.913330f, 353.562775f, 352.996185f, 3.436852f);

			WallCreating = true;
		}

		_unit->CastSpell(_unit, dbcSpell.LookupEntry(EYE_BLAST), true);

		DespawnTimer--;
		if (!DespawnTimer)
			_unit->Despawn(0,0);
    }

protected:

	bool WallCreating;
	uint32 DespawnTimer;
	int nrspells;
};

/* Flame Crash Effect AI - Creature AI Class */

#define CN_FLAME_CRASH 23336

#define FLAME_CRASH_EFFECT 40836

class FlameCrashAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FlameCrashAI);

    FlameCrashAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(FLAME_CRASH_EFFECT), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(60000);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(60000);
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		_unit->Despawn(0,0);
    }

protected:

	int nrspells;
};

/* Blaze Effect AI - Creature AI Class */

#define CN_BLAZE 23259

#define BLAZE_EFFECT 40610

class BlazeAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BlazeAI);

    BlazeAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(BLAZE_EFFECT), true);
		_unit->GetAIInterface()->disable_melee = true;
		_unit->GetAIInterface()->m_canMove = false;
		_unit->m_noRespawn = true;

		RegisterAIUpdateEvent(60000);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(60000);
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

		RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		_unit->Despawn(0,0);
    }

protected:

	int nrspells;
};

/* Misc Creature AIs */

/* Flame of Azzinoth AI - Creature AI Class */

#define CN_FLAME_OF_AZZINOTH 22997	

#define BLAZE 40637		// this one just summons mob that will trigger that fire cloud - doesn't spawn mob
#define FLAME_BLAST	40631
#define FOA_ENRAGE 40743	// 40683 or 40743
#define CHAOS_CHARGE 40497 // CHAOS_CHARGE 40497 CHARGE 40602

class FlameOfAzzinothAI : public MoonScriptCreatureAI
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(FlameOfAzzinothAI);
	SP_AI_Spell spells[4];
	bool m_spellcheck[4];

    FlameOfAzzinothAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 2;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(BLAZE);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 30;
		spells[0].perctrigger = 7.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(FLAME_BLAST);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = true;
		spells[1].cooldown = 15;
		spells[1].perctrigger = 13.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(CHAOS_CHARGE);
		spells[2].targettype = TARGET_ATTACKING;
		spells[2].instant = true;

		spells[3].info = dbcSpell.LookupEntry(FOA_ENRAGE);
		spells[3].targettype = TARGET_SELF;
		spells[3].instant = true;

		_unit->m_noRespawn = true;

		Enrage = false;
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
        RemoveAIUpdateEvent();

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
	   FoADeaths[_unit->GetInstanceID()]--;
    }

    void AIUpdate()
	{
		if (Enrage == true)
		{
			_unit->CastSpell(_unit, spells[3].info, spells[3].instant);
			Enrage = false;
			return;
		}

		if (_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
		{
			UnitPointer target = NULLUNIT;
			target = _unit->GetAIInterface()->GetNextTarget();

			if (_unit->GetDistance2dSq(target) >= 225.0f && _unit->GetDistance2dSq(target) <= 1225 && rand()%5 == 0)	// I must test this value (15-35yards for now)
			{
				_unit->CastSpell(target, spells[2].info, spells[2].instant);
				Enrage = true;
				return;
			}
		}

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}

					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

protected:

	bool Enrage;
	int nrspells;
};

/* Akama AI - Creature Gossip Class */
 
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, plr); \
    Menu->SendTo(plr);
 
class SCRIPT_DECL AkamaGossip : public GossipScript
{
public:
    void GossipHello(ObjectPointer pObject, PlayerPointer plr, bool AutoSend)
    {
        GossipMenu *Menu;
		CreaturePointer Akama = TO_CREATURE(pObject);
		if (Akama == NULL)
			return;

		if (Akama->GetAIInterface()->getCurrentWaypoint() >= 10)
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 229902, plr);
			Menu->AddItem( 0, "We're ready to face Illidan.", 2);
		}

		else
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 229901, plr);
			Menu->AddItem( 0, "I'm ready, Akama.", 1);
		}
	 
		if(AutoSend)
			Menu->SendTo(plr);
    }
 
	void GossipSelectOption(ObjectPointer pObject, PlayerPointer plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		CreaturePointer Akama = TO_CREATURE(pObject);
		if (Akama == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;
 
			case 1:
			{
				Akama->SetUInt32Value(UNIT_NPC_FLAGS, 0);

				Akama->GetAIInterface()->StopMovement(0);
				Akama->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				Akama->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				Akama->GetAIInterface()->setWaypointToMove(1);
			}break;

			case 2:
			{
				Akama->SetUInt32Value(UNIT_NPC_FLAGS, 0);

				Akama->GetAIInterface()->StopMovement(0);
				Akama->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				Akama->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				Akama->GetAIInterface()->setWaypointToMove(17);
			}break;
		}
	}
 
    void Destroy()
    {
        delete this;
    }
};

/* Akama AI - Creature AI Class */

#define CN_AKAMA 22990

#define HEALING_POTION 40535
#define BLESSING_OF_KINGS 20217
#define AKAMA_DESPAWN 41242

// Door Event Spells
#define AKAMA_DOOR_FAIL 41271
#define AKAMA_DOOR_OPEN 41268
#define DEATHSWORN_DOOR_OPEN 41269
#define GATE_FAILURE 10390
/*
struct Coords
{
    float x;
    float y;
    float z;
    float o;
};*/

static Coords ToIllidan[] = 
{
	{  },
	{ 660.248596f, 330.695679f, 271.688110f, 1.243284f },
	{ 671.172241f, 353.273193f, 271.689453f, 1.022600f },
	{ 694.227783f, 379.461365f, 271.792145f, 6.232135f },
	{ 724.159973f, 373.263275f, 282.614349f, 5.324218f },
	{ 747.034973f, 335.668274f, 307.047150f, 4.972365f },
	{ 755.477234f, 304.194580f, 312.167328f, 6.276120f },
	{ 771.809753f, 303.744873f, 313.563507f, 6.265894f },
	{ 778.550232f, 304.515198f, 318.818542f, 0.002354f },
	{ 789.564697f, 304.493652f, 319.759583f, 6.248631f },
	{ 799.598389f, 295.776642f, 319.760040f, 4.687257f },
	{ 799.054016f, 288.826660f, 320.030334f, 4.563174f },
	{ 794.595459f, 262.302856f, 341.463715f, 4.500343f },
	{ 794.396973f, 256.420471f, 341.463715f, 4.557680f },
	{ 783.355957f, 250.460892f, 341.463776f, 3.746361f },
	{ 764.988098f, 238.561462f, 353.646484f, 3.324606f },
	{ 749.337463f, 236.288681f, 352.997223f, 1.633631f },
	{ 751.941528f, 304.626221f, 352.996124f, 3.128243f },
	{ 747.236511f, 304.922913f, 352.996155f, 6.278462f },
	{ 747.834106f, 362.513977f, 352.996155f, 1.604554f }
};

class AkamaAI : public MoonScriptCreatureAI
{
public:
	friend class AkamaGossip;

    ADD_CREATURE_FACTORY_FUNCTION(AkamaAI);
	SP_AI_Spell spells[3];
	bool m_spellcheck[3];

    AkamaAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		for (int i = 1; i < 20; i++)
		{
			_unit->GetAIInterface()->addWayPoint(CreateWaypoint(i, 0, RUN));
		}

		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

		spells[0].info = dbcSpell.LookupEntry(BLESSING_OF_KINGS);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 600;	// 10 min
		spells[0].perctrigger = 15.0f;
		spells[0].attackstoptimer = 1000;

		spells[1].info = dbcSpell.LookupEntry(HEALING_POTION);
		spells[1].targettype = TARGET_SELF;
		spells[1].instant = true;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 0.0f;
		spells[1].attackstoptimer = 1000;

		spells[2].info = dbcSpell.LookupEntry(AKAMA_DESPAWN);
		spells[2].targettype = TARGET_SELF;
		spells[2].instant = false;
		spells[2].cooldown = 0;
		spells[2].perctrigger = 0.0f;
		spells[2].attackstoptimer = 1000;

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->m_moveRun = true;
		RegisterAIUpdateEvent(1000);

		if (!DeathEvent[_unit->GetInstanceID()])
			_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
		else _unit->SetUInt32Value(UNIT_NPC_FLAGS, 0);

		MaievDialog[_unit->GetInstanceID()] = 0;
		m_phase[_unit->GetInstanceID()] = 0;
		AkamaDialog = 0;
		DoorEvent = 0;
		CourtSay = 0;
		AkamaRun = 0;
	}
    
    void OnCombatStart(UnitPointer mTarget)
    {
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);

		SoundLimiter = 0;

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		if (m_phase[_unit->GetInstanceID()] > 0)
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
		_unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);

		//RemoveAIUpdateEvent();

		AkamaDialog = 0;
		//AkamaRun = 0;
		CourtSay = 0;
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Illidan will pay!");
				_unit->PlaySoundToSet(11381);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "One step closer!");
				_unit->PlaySoundToSet(11382);
				break;
			}
		}
    }

    void OnDied(UnitPointer mKiller)
    {
		_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Fight on friends! Kill him and end the curse on my people!");	// not sure
		_unit->PlaySoundToSet(11391);

		RemoveAIUpdateEvent();

		AkamaDialog = 0;
		AkamaRun = 0;
		CourtSay = 0;
    }

    void AIUpdate()
    {
		SoundLimiter++;

		if (_unit->GetHealthPct() <= 15 && _unit->GetAIInterface()->GetNextTarget())
		{
			int RandomSpeach = rand()%2;

			if (SoundLimiter >= 15)
			{
				switch (RandomSpeach)
				{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not last much longer!");
					_unit->PlaySoundToSet(11385);
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No! Not yet!");
					_unit->PlaySoundToSet(11386);
					break;
				}

				SoundLimiter = 0;
			}

			if (_unit->GetAIInterface()->GetNextTarget()->GetTypeId() == TYPEID_UNIT)
			{
				CreaturePointer Target = NULLCREATURE;
				Target = TO_CREATURE(_unit->GetAIInterface()->GetNextTarget());
				if (Target && Target->GetEntry() == 22917)
					_unit->CastSpell(_unit, spells[1].info, spells[1].instant);
			}
		}

		if (_unit->GetAIInterface()->getCurrentWaypoint() == 6 && DoorEvent)
		{
			GameObjectPointer Gate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(774.7f, 304.6f, 314.85f, 185905);

			UnitPointer DoorTrigger = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(773.992f, 304.743f, 320.536f, CN_DOOR_EVENT_TRIGGER);

			if (!Gate)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "It's strange that Illidan doesn't protect himself against intruders." );
				_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
				_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(7);
				_unit->SetFacing(6.248631f);

				DoorEvent = 0;

				if (Udalo)
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}

				if (Olum)
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}

				return;
			}

			switch (DoorEvent)
			{
				case 2:
				{
					_unit->SetFacing(6.248631f);
				}break;

				case 6:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The door is all that stands between us and the Betrayer. Stand aside, friends." );
					_unit->Emote(EMOTE_ONESHOT_TALK);
				}break;

				case 10:		// here will be first try to break door (Akama himself only)
				{
					if (DoorTrigger)
					{
						_unit->CastSpell(DoorTrigger, dbcSpell.LookupEntry(AKAMA_DOOR_FAIL), false);
						_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
						_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_DOOR_FAIL);
					}
				}break;

				case 19:		// here we will stop casting spell on door
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}break;

				case 20:
				{
					_unit->Emote(EMOTE_ONESHOT_NO);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I cannot do this alone..." );
				}break;

				case 26:		// summoning two spirits to help Akama with breaking doors
				{
					Udalo = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23410, 751.884705f, 311.270050f, 312.121185f, 0.047113f, true, false, 0, 0);
					Olum  = _unit->GetMapMgr()->GetInterface()->SpawnCreature(23411, 751.687744f, 297.408600f, 312.124817f, 0.054958f, true, false, 0, 0);

					if (Udalo)
					{
						Udalo->GetAIInterface()->SetAllowedToEnterCombat(false);
						Udalo->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
					}
					
					if (Olum)
					{
						Olum->GetAIInterface()->SetAllowedToEnterCombat(false);
						Olum->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
					}
				}break;

				case 27:
				{
					if (Udalo)
						Udalo->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You are not alone, Akama." );
				}break;

				case 33:
				{
					if (Olum)
						Olum->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Your people will always be with you." );
				}break;

				case 38:			// Akama starts casting his spell again with both spirits
				{
					if (DoorTrigger)
					{
						_unit->CastSpell(DoorTrigger, dbcSpell.LookupEntry(AKAMA_DOOR_OPEN), false);
						_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
						_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, AKAMA_DOOR_OPEN);
					}
				}break;

				case 39:
				{
					if (DoorTrigger)
					{
						if (Udalo)
						{
							Udalo->CastSpell(DoorTrigger, dbcSpell.LookupEntry(DEATHSWORN_DOOR_OPEN), false);
							Udalo->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
							Udalo->SetUInt32Value(UNIT_CHANNEL_SPELL, DEATHSWORN_DOOR_OPEN);
						}
	
						if (Olum)
						{
							Olum->CastSpell(DoorTrigger, dbcSpell.LookupEntry(DEATHSWORN_DOOR_OPEN), false);
							Olum->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, DoorTrigger->GetGUID());
							Olum->SetUInt32Value(UNIT_CHANNEL_SPELL, DEATHSWORN_DOOR_OPEN);
						}
					}
				}break;

				case 48:			// Door has been broken and channel stoped
				{
					Gate->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

					if (DoorTrigger)
					{
						DoorTrigger->CastSpell(_unit, dbcSpell.LookupEntry(GATE_FAILURE), true);
					}
				}break;

				case 49:
				{
					_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

					if (Udalo)
					{
						Udalo->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						Udalo->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					}

					if (Olum)
					{
						Olum->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						Olum->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
					}
				}break;

				case 52:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I thank you for your aid, my brothers. Our people will be redeemed!!" );
					_unit->Emote(EMOTE_ONESHOT_SALUTE);
				}break;

				case 55:
				{
					if (Udalo)
						Udalo->Emote(EMOTE_ONESHOT_SALUTE);
					if (Olum)
						Olum->Emote(EMOTE_ONESHOT_SALUTE);
				}break;

				case 58:
				{
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(7);
				}break;
			}

			DoorEvent++;

			if (DoorEvent > 58)
				DoorEvent = 0;
		}

		if (CourtSay)
		{
			CourtSay++;

			switch (CourtSay)
			{
			case 2:
				{
					_unit->PlaySoundToSet(11388);
					_unit->SetFacing(2.113512f);
				}break;
			case 17:
				{
					_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);

					CourtSay = 0;
				}break;
			}
		}

		if (AkamaDialog)
		{
			Illidan = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(704.539001f, 305.282013f, 353.919006f, 22917);

			if (!Illidan || !Illidan->isAlive())
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Not this time my friends." );
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
				_unit->Despawn(0,0);

				AkamaDialog = 0;
				return;
			}

			switch(AkamaDialog)
			{
			case 1:
				{
					_unit->GetAIInterface()->SetNextTarget(Illidan);
					_unit->SetFacing(3.126680f);
				}break;
			case 2:
				{
					Illidan->RemoveAura(39656);
				}break;
			case 3:
			    {
			        Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Akama... your duplicity is hardly surprising. I should have slaughtered you and your malformed brethren long ago.");
					Illidan->PlaySoundToSet(11463);
			    }break;
			case 7:
				{
					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 12:
				{
					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 15:
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "We've come to end your reign, Illidan. My people and all of Outland shall be free!");
					_unit->PlaySoundToSet(11389);

					_unit->Emote(EMOTE_ONESHOT_POINT);
			    }break;
			case 18:
				{
					_unit->Emote(EMOTE_ONESHOT_TALK);
				}break;
			case 22:
				{
					_unit->Emote(EMOTE_ONESHOT_SALUTE);
				}break;
			case 25:
			    {
			        Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Boldly said. But I remain unconvinced.");
					Illidan->PlaySoundToSet(11464);

					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
			    }break;
			case 28:
				{
					Illidan->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 32:
			    {
			        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The time has come! The moment is at hand!");
					_unit->PlaySoundToSet(11380);

					_unit->Emote(EMOTE_ONESHOT_SHOUT);
			    }break;
			case 34:
				{
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
					_unit->Emote(EMOTE_ONESHOT_ROAR);
				}break;
			case 36:
				{
					Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You are not prepared!");
					Illidan->PlaySoundToSet(11466);

					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);

					Illidan->Emote(EMOTE_ONESHOT_CUSTOMSPELL05);
				}break;
			case 38:
				{
					Illidan->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
				}break;
			case 40:
				{
					GameObjectPointer RightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
					GameObjectPointer LeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
					
					if (RightGate)
						RightGate->SetByte(GAMEOBJECT_BYTES_1, 0, 1);

					if (LeftGate)
						LeftGate->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
					
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					_unit->GetAIInterface()->SetAIState(STATE_IDLE);
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					Illidan->GetAIInterface()->SetAllowedToEnterCombat(true);
					Illidan->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					Illidan->GetAIInterface()->m_canMove = true;

					_unit->GetAIInterface()->AttackReaction(Illidan, 1, 0);

					CastTriggering = 2;
					AkamaRun = 0;
				}break;
			}

			AkamaDialog++;

			if (AkamaDialog > 40)
				AkamaDialog = 0;
		}

		if (m_phase[_unit->GetInstanceID()] == 0 && DeathEvent[_unit->GetInstanceID()] >= 41)
		{	
			if (DeathEvent[_unit->GetInstanceID()] == 41)
			{
				Maiev = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(700.009216f, 305.660065f, 354.405609f, 23197);
				if (Maiev && Maiev->isAlive())
				{
					MposX = Maiev->GetPositionX();
					MposY = Maiev->GetPositionY();
					MposZ = Maiev->GetPositionZ();
					MposO = Maiev->GetOrientation();
				}
			}

			if (_unit->GetPositionX() == MposX && _unit->GetPositionY() == MposY && _unit->GetPositionZ() == MposZ && DeathEvent[_unit->GetInstanceID()] < 44)
				DeathEvent[_unit->GetInstanceID()] = 44;

			if (DeathEvent[_unit->GetInstanceID()] == 42)
			{
				_unit->GetAIInterface()->MoveTo(MposX, MposY, MposZ, MposO);
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
				_unit->GetAIInterface()->m_moveRun = true;

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(1000);

				DeathEvent[_unit->GetInstanceID()] = 43;

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);	// dunno why this emote sometimes bugs facing
				//_unit->GetAIInterface()->SetNextTarget(Illidan);
			}

			if (DeathEvent[_unit->GetInstanceID()] >= 45)
			{
				switch (DeathEvent[_unit->GetInstanceID()])
				{
				case 45:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "The Light will fill these dismal halls once again. I swear it.");
						_unit->PlaySoundToSet(11387);

						if (Illidan)
							_unit->GetAIInterface()->SetNextTarget(Illidan);

						_unit->Emote(EMOTE_ONESHOT_TALK);
					}break;
				case 49:
					{
						_unit->Emote(EMOTE_ONESHOT_SALUTE);
					}break;
				case 53:
					{
						DeathEvent[_unit->GetInstanceID()] = 0;

						_unit->CastSpell(_unit, spells[2].info, spells[2].instant);
						_unit->Despawn(0,0);
					}break;
				}
			}

			if (DeathEvent[_unit->GetInstanceID()] >= 44)
				DeathEvent[_unit->GetInstanceID()]++;
		}

		if ((_unit->GetAIInterface()->GetNextTarget() || AkamaRun >= 7) && _unit->GetAIInterface()->getCurrentWaypoint() < 18)
		{
			CastTriggering--;
			if (!CastTriggering && AkamaRun < 7)	// && Illidan && Illidan->GetHealthPct() > 85)
			{
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
				float val = (float)RandomFloat(100.0f);
				SpellCast(val);

				CastTriggering = 2;
			}

			if (((!Illidan || !Illidan->isAlive()) && AkamaRun <= 7) || (!DeathEvent[_unit->GetInstanceID()] && !Illidan->isAlive()))	// added check for Illidan life state, coz some stupid GMs can kill Illidan by command before real part of event starts
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Not this time my friends." );
				_unit->CastSpell(_unit, spells[2].info, spells[2].instant);

				AkamaRun = 0;

				_unit->Despawn(0,0);
				return;
			}

			if (Illidan && Illidan->isAlive() && Illidan->GetHealthPct() <= 85 && AkamaRun <= 7 || AkamaRun > 7)	// we don't need to check Illidan if AkamaRun > 7 coz we don't use him in that time (only _unit)
			{
				switch (AkamaRun)
				{
					case 0:
					{
						Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Come, my minions. Deal with this traitor as he deserves!");
						Illidan->PlaySoundToSet(11465);
					}break;

					case 7:
					{	
						_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
						_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
						_unit->GetAIInterface()->SetAIState(STATE_IDLE);
						//_unit->GetAIInterface()->SetNextTarget(NULL);
						/*_unit->GetAIInterface()->WipeTargetList();	// it's strange that if Akama goes out of combat he affects Illidan so he won't cast or update ai correctly
						_unit->GetAIInterface()->WipeHateList();*/

						_unit->GetAIInterface()->disable_melee = true;
						_unit->GetAIInterface()->m_canMove = false;

						Illidan->GetAIInterface()->RemoveThreatByPtr(_unit);	// Akama will stuck in place anyway if alone in combat with Illidan

						//_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					}break;

					case 8:
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will deal with these mongrels! Strike now, friends! Strike at the Betrayer!");
						_unit->PlaySoundToSet(11390);

						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					}break;

					case 9:
					{
						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					}break;

					case 10:
					{
						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					}break;

					case 11:
					{
						_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
						_unit->Emote(EMOTE_ONESHOT_EXCLAMATION);
					}break;
		
					case 13:
					{
						_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
						_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
					}break;

					case 15:
					{
						if (_unit->GetAIInterface()->getMoveType() != MOVEMENTTYPE_WANTEDWP) 
						{
							// maybe something to stop Illidan from fight Akama, or switch target to PC something!
							_unit->GetAIInterface()->m_canMove = true;

							_unit->GetAIInterface()->StopMovement(0);
							_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
							_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
							_unit->GetAIInterface()->setWaypointToMove(18);

							//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
						}
					}break;
				}
				
				AkamaRun++;
				if (AkamaRun > 15)
					AkamaRun = 0;
			}
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

    void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch (iWaypointId)
		{
		case 6:
			{
				DoorEvent = 1;
			}break;
		case 16:
			{
				CourtSay = 1;
			}break;
		case 17:
			{
				AkamaDialog = 1;
			}break;
		case 19:
			{
				//_unit->Despawn(0, 0);	// do not let Illidan's AI being updated
			}break;
		case 20:
			{
				DeathEvent[_unit->GetInstanceID()] = 49;
			}break;
		default:
			{
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(iWaypointId+1);
			}
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = ToIllidan[id].mX;
        wp->y = ToIllidan[id].mY;
        wp->z = ToIllidan[id].mZ;
        wp->o = ToIllidan[id].mO;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	float MposX, MposY, MposZ, MposO;
	uint32 CastTriggering;
	uint32 SoundLimiter;
	uint32 m_currentWP;
	uint32 AkamaDialog;
	uint32 DoorEvent;
	uint32 AkamaRun;
	uint32 CourtSay;
	UnitPointer Illidan;
	int nrspells;
	UnitPointer Maiev;
	UnitPointer Udalo;
	UnitPointer Olum;
};

/* Blade of Azzinoth AI - Creature AI Class */

#define CN_BLADE_OF_AZZINOTH 22996

class BladeOfAzzinothAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(BladeOfAzzinothAI);

    BladeOfAzzinothAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->m_noRespawn = true;
		
		RegisterAIUpdateEvent(500);
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

        //RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       //RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
		switch (BladesEvent[_unit->GetInstanceID()])
		{
			case 29:	// Despawn of Blades -> Return to Illidan's hands
			{
				_unit->Despawn(0,0);
			}break;
		}
    }
};

/* Maiev AI - Creature AI Class */

#define CN_MAIEV 23197

//#define TELEPORT_MAIEV 41221
#define TELEPORT 34673	// 34673, 41992

class MaievAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(MaievAI);
	SP_AI_Spell spells[1];
	bool m_spellcheck[1];

    MaievAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		nrspells = 1;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
		}

		spells[0].info = dbcSpell.LookupEntry(TELEPORT);
		spells[0].targettype = TARGET_SELF;
		spells[0].instant = true;
		spells[0].cooldown = 0;
		spells[0].perctrigger = 0.0f;
		spells[0].attackstoptimer = 0;

		Illidan = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(704.539001f, 305.282013f, 353.919006f, 22917);

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
	
		RegisterAIUpdateEvent(250);

		CastTriggering = 0;
		SoundTimer = 50;

		//_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
		//RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		//_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        //_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        //_unit->GetAIInterface()->SetAIState(STATE_IDLE);

		//_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
        //RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
       RemoveAIUpdateEvent();
	   SoundTimer = 50;
    }

	void OnDamageTaken(UnitPointer mAttacker, float fAmount)
	{
		_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
	}

    void AIUpdate()
    {
		_unit->SetUInt32Value(UNIT_FIELD_HEALTH, _unit->GetUInt32Value(UNIT_FIELD_MAXHEALTH));

		float val = (float)RandomFloat(100.0f);
		SpellCast(val);

		if (MaievDialog[_unit->GetInstanceID()] >= 29 && !DeathEvent[_unit->GetInstanceID()])
			SoundTimer++;

		float SayNaisha = (float)RandomFloat(100.0f);
		if (!DeathEvent[_unit->GetInstanceID()] && ((SayNaisha < 2.5f && SoundTimer >= 100 && SoundTimer < 1330) || SoundTimer == 1361 || SoundTimer > 1400))
		{
			if (SoundTimer != 1361)
				RandText = rand()%3;
				//RandText = sRand.rand(3);
			if (SoundTimer == 1361)
				RandText = rand()%2;
				//RandText = sRand.rand(2);
			switch (RandText)	// Not sure if those both should be here, but couldn't find any relation when those are said on blizz
			{
				case 0:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Bleed as I have bled!");	// not sure
					_unit->PlaySoundToSet(11494);

					SoundTimer = 0;
					break;
				case 1:
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "That is for Naisha!");
					_unit->PlaySoundToSet(11493);

					SoundTimer = 0;
					break;
				case 2:
					if (Illidan)
					{
						Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Feel the hatred of ten thousand years!");
						Illidan->PlaySoundToSet(11470);

						SoundTimer = 1337;
				}break;
			}
		}

		if (DeathEvent[_unit->GetInstanceID()] && Illidan)
		{
			switch (DeathEvent[_unit->GetInstanceID()])
			{
			case 1:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, it is finished. You are beaten.");	// not sure
					_unit->PlaySoundToSet(11496);

					//_unit->GetAIInterface()->SetAIState(STATE_IDLE);
					//_unit->GetAIInterface()->WipeTargetList();
					//_unit->GetAIInterface()->WipeHateList();

					//Illidan->GetAIInterface()->SetNextTarget(NULL);
					Illidan->Emote(EMOTE_ONESHOT_CUSTOMSPELL06);
					if(Illidan->GetCurrentSpell() != NULL)
						Illidan->GetCurrentSpell()->cancel();

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(1000);
				}break;
			case 2:
				{
					Illidan->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_CUSTOMSPELL07);
					_unit->GetAIInterface()->SetNextTarget(Illidan);
				}break;
			case 5:
				{
					Illidan->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You have won... Maiev. But the huntress... is nothing without the hunt. You... are nothing... without me.");
					Illidan->PlaySoundToSet(11478);

					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
					//MaievWeaponOff();
				}break;
			case 25:
				{
					GameObjectPointer RightGate = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(745.07f, 241.802f, 354.292f, 200000);
					GameObjectPointer LeftGate  = _unit->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(744.829f, 369.276f, 354.324f, 200001);
					
					if (RightGate)
						RightGate->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

					if (LeftGate)
						LeftGate->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

					Illidan->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
					Illidan->SetUInt32Value(UNIT_FIELD_HEALTH,0);
					Illidan->setDeathState(JUST_DIED);
				}break;
			case 30:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "He's right. I feel nothing. I am nothing.");
					_unit->PlaySoundToSet(11497);
				}break;
			case 39:
				{
					//_unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_AKAMA, 739.545410f, 323.023743f, 352.996094f, 4.578086f, true, false, 0, 0);	// we have one Akama already spawned so we don't need another one =/ (still it's freakin' workaround -.-')
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Farewell, champions.");
					_unit->PlaySoundToSet(11498);
				}break;
			case 40:
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(TELEPORT), true);
				}break;
			case 41:
				{
					RemoveAIUpdateEvent();

					_unit->Despawn(0,0);
				}break;
			}

			if (DeathEvent[_unit->GetInstanceID()] <= 41)
				DeathEvent[_unit->GetInstanceID()]++;
		}
    }

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					_unit->setAttackTimer(spells[i].attackstoptimer, false);
					spells[i].casttime = t + spells[i].cooldown;
					m_spellcheck[i] = true;
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }


protected:

	uint32 CastTriggering;
	uint32 SoundTimer;
	uint32 RandText;
	UnitPointer Illidan;
	int nrspells;
	UnitPointer Maiev;
};


/* Illidan Stormrage AI - Creature AI Class */

#define CN_ILLIDAN_STORMRAGE 22917

// Normal Form Spells
#define SHEAR								41032  // +
#define DRAW_SOUL							40904  // +
#define FLAME_CRASH							40832  // +
#define MORTAL_STRIKE						37335  // +

// Demon Forms
#define DEMON_FORM1							40511  // +
#define DEMON_FORM2							40398  // +
#define DEMON_FORM3							40510  // +

// Demon Form Spells
#define SHADOW_BLAST						41078  // +
#define SUMMON_DEMON						41117  // - Core Support
#define FLAME_BURST							41131  // ? Haven't Tried	// 41126 is the right one, but it's spell that needs to be scripted

// Illidan's Glaive spells
#define SUMMON_GLAIVE						41466
#define GLAIVE_RETURNS						39873
#define THROW_GLAIVE1						39635
#define THROW_GLAIVE2						39849

// Phase 2 Spells
#define AURA_OF_DREAD						41142
#define FIREBALL							40598  // +
#define DARK_BARRAGE						40585  // ? Haven't Tried
#define SUMMON_TEAR_OF_AZZINOTH				39855  // +

#define EYE_BLAST1 39908
#define EYE_BLAST2 40017
#define EYE_BLAST3 40017

#define TEAR_OF_AZZINOTH_CHANNEL 39857

// Phase 3 Spells
#define AGONIZING_FLAMES					40932  // ? Haven't Tried (Before Demon Form!)

// Phase 4 Spells

// Phase 5 Spells
#define ENRAGE								34971  // +

// Other spells
#define SHADOW_PRISON						40647  // +
#define SKULL_INTRO							39656  // + Works with RemoveAura
#define SUMMON_PARASITIC_SHADOWFIENDS		41915  // ? Haven't Tried
#define PARASITIC_SHADOWFIEND1				41914  // ? Haven't Tried
#define PARASITIC_SHADOWFIEND2				41913  // ? Haven't Tried

static Coords ForIllidan[] = 
{
	{  },
	{ 709.783203f, 305.073883f, 362.988770f, 1.546307f },
	{ 680.798157f, 305.648743f, 353.192200f, 3.196716f }
};

class IllidanStormrageAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(IllidanStormrageAI);
	SP_AI_Spell spells[15];
	bool m_spellcheck[15];

    IllidanStormrageAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(1, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(2, 0, FLY));
		_unit->GetAIInterface()->addWayPoint(CreateWaypoint(3, 0, RUN));

		nrspells = 5;
		for(int i=0;i<nrspells;i++)
		{
			m_spellcheck[i] = false;
			spells[i].casttime = 0;
		}

        spells[0].info = dbcSpell.LookupEntry(SHEAR);
		spells[0].targettype = TARGET_ATTACKING;
		spells[0].instant = false;
		spells[0].cooldown = 15;
		spells[0].perctrigger = 12.0f;
		spells[0].attackstoptimer = 1000;

        spells[1].info = dbcSpell.LookupEntry(DRAW_SOUL);
		spells[1].targettype = TARGET_VARIOUS;
		spells[1].instant = false;
		spells[1].cooldown = 0;
		spells[1].perctrigger = 0.0f;	// 6.0f	attacks himself too so it can cause crashes if he dies because of his own hit
		spells[1].attackstoptimer = 1000;

        spells[2].info = dbcSpell.LookupEntry(FLAME_CRASH);
		spells[2].targettype = TARGET_DESTINATION;
		spells[2].instant = false;
		spells[2].cooldown = 25;
		spells[2].perctrigger = 12.0f;
		spells[2].attackstoptimer = 1000;

        spells[3].info = dbcSpell.LookupEntry(MORTAL_STRIKE);
		spells[3].targettype = TARGET_ATTACKING;
		spells[3].instant = true;
		spells[3].cooldown = 20;
		spells[3].perctrigger = 8.0f;
		spells[3].attackstoptimer = 1000;

		spells[4].info = dbcSpell.LookupEntry(AGONIZING_FLAMES);
		spells[4].targettype = TARGET_RANDOM_SINGLE;
		spells[4].instant = true;
		spells[4].cooldown = 25;
		spells[4].perctrigger = 7.0f;
		spells[4].attackstoptimer = 1000;
		spells[4].mindist2cast = 0.0f;
		spells[4].maxdist2cast = 30.0f;
		spells[4].minhp2cast = 0;
		spells[4].maxhp2cast = 100;

        spells[5].info = dbcSpell.LookupEntry(ENRAGE);
		spells[5].targettype = TARGET_SELF;
		spells[5].instant = true;
		spells[5].cooldown = 0;
		spells[5].perctrigger = 0.0f;
		spells[5].attackstoptimer = 1000;
		spells[5].speech = "You've wasted too much time mortals, now you shall fall!";
		spells[5].soundid = 11474;

        spells[6].info = dbcSpell.LookupEntry(SHADOW_BLAST);
		spells[6].targettype = TARGET_DESTINATION;
		spells[6].instant = false;
		spells[6].cooldown = 0;
		spells[6].perctrigger = 0.0f;
		spells[6].attackstoptimer = 1000;

        spells[7].info = dbcSpell.LookupEntry(SUMMON_DEMON);
		spells[7].targettype = TARGET_SELF;	// ?
		spells[7].instant = false;
		spells[7].cooldown = 0;
		spells[7].perctrigger = 0.0f;
		spells[7].attackstoptimer = 1000;

        spells[8].info = dbcSpell.LookupEntry(SHADOW_PRISON);
		spells[8].targettype = TARGET_VARIOUS;
		spells[8].instant = true;
		spells[8].cooldown = 0;
		spells[8].perctrigger = 0.0f;
		spells[8].attackstoptimer = 1000;

		/* Spells after this point are not counted when using the SpellCast() 
		   funct in AI and need to be manually triggered! :-D */
		spells[9].info = dbcSpell.LookupEntry(DEMON_FORM1);
		spells[9].instant = true;
		spells[9].cooldown = 0;
		spells[9].perctrigger = 0.0f;
		spells[9].attackstoptimer = 1000;

		spells[10].info = dbcSpell.LookupEntry(DEMON_FORM2);
		spells[10].instant = true;
		spells[10].cooldown = 0;
		spells[10].perctrigger = 0.0f;

		spells[11].info = dbcSpell.LookupEntry(DEMON_FORM3);
		spells[11].instant = true;
		spells[11].cooldown = 0;
		spells[11].perctrigger = 0.0f;

		spells[12].info = dbcSpell.LookupEntry(FIREBALL);
		spells[12].targettype = TARGET_RANDOM_DESTINATION;
		spells[12].instant = false;
		spells[12].cooldown = 0;
		spells[12].perctrigger = 0.0f;

		spells[13].info = dbcSpell.LookupEntry(DARK_BARRAGE);
		spells[13].targettype = TARGET_RANDOM_SINGLE;
		spells[13].instant = false;
		spells[13].cooldown = 0;
		spells[13].perctrigger = 0.0f;

		spells[14].info = dbcSpell.LookupEntry(EYE_BLAST1);
		spells[14].instant = false;
		spells[14].cooldown = 0;
		spells[14].perctrigger = 0.0f;

		/* Setup Illidan */

		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
		_unit->CastSpell(_unit, dbcSpell.LookupEntry(SKULL_INTRO), true);
		_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
		_unit->GetAIInterface()->m_canMove = false;

		/* Initialize variables and stuff, I Guess */
		FoA1 = FoA2 = Maiev = Blade1 = Blade2 = NULLUNIT;
		BladesEvent[_unit->GetInstanceID()] = 0;
		DeathEvent[_unit->GetInstanceID()] = 0;
		m_phase[_unit->GetInstanceID()] = 0;	// must be 0 to use it with some other stuff.
		DemonSoundRand = false;
		DemonPhaseTimer = 5;
		DemonPhase = 0;		
    }

    void OnCombatStart(UnitPointer mTarget)
    {
		RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
		_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
		_unit->GetAIInterface()->m_canMove = true;

		m_phase[_unit->GetInstanceID()] = 1;
		DemonPhaseTimer = 5;
		DemonPhase = 0;

		for (int i = 0; i < nrspells; i++)
			spells[i].casttime = 0;
    }

	void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%2;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Who shall be next to taste my blades?!");
				_unit->PlaySoundToSet(11473);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "This is too easy!");
				_unit->PlaySoundToSet(11472);
				break;
			}
		}
    }

    void OnCombatStop(UnitPointer mTarget)
    {
		std::vector<CreaturePointer > DespawnTable;	/* From M4ksiu - Big THX to Capt who learned me how to use std stuff <3	*/
												/* If anyone wants to use this function, then leave this note!			*/
		for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
		{ 
			if ((*itr)->GetTypeId()== TYPEID_UNIT && (*itr)->GetInstanceID() == _unit->GetInstanceID() && (*itr) != _unit)
			{
				CreaturePointer CreatureToAdd = NULLCREATURE;
				CreatureToAdd = TO_CREATURE(*itr);

				if (CreatureToAdd && CreatureToAdd->GetSQL_id() == 0 && (CreatureToAdd->GetEntry() == CN_FLAME_OF_AZZINOTH || 
					CreatureToAdd->GetEntry() == CN_BLADE_OF_AZZINOTH || (_unit->isAlive() && CreatureToAdd->GetEntry() == CN_MAIEV)))
				{
					DespawnTable.push_back(CreatureToAdd);
				} 
			} 
		}

		for (uint32 i = 0; i < DespawnTable.size(); i++)
		{
			CreaturePointer CreatureToDespawn = DespawnTable[i];

			if (CreatureToDespawn)
				CreatureToDespawn->Despawn(0,0);
		}

		DespawnTable.clear();

		_unit->GetAIInterface()->disable_melee = false;
		_unit->GetAIInterface()->m_moveFly = false;
		_unit->GetAIInterface()->m_canMove = true;

		_unit->GetAIInterface()->WipeTargetList();
		_unit->GetAIInterface()->WipeHateList();

		RemoveAIUpdateEvent();

		_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
		_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);

		_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);
		_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
		
		_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
		_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
		
		BladesEvent[_unit->GetInstanceID()] = 0;
		DeathEvent[_unit->GetInstanceID()] = 0;
		m_phase[_unit->GetInstanceID()] = 0;
		DemonSoundRand = false;
		DemonPhaseTimer = 5;
		m_currentWP = 0;
		DemonPhase = 0;

		if (m_phase[_unit->GetInstanceID()] == 2)
		{
			_unit->GetAIInterface()->WipeTargetList();
			_unit->GetAIInterface()->WipeHateList();

			_unit->Emote(EMOTE_ONESHOT_LAND);

			WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
			data << _unit->GetNewGUID();
			data << uint32(0);
			_unit->SendMessageToSet(&data, false);

			float x = _unit->m_spawn->x;
			float y = _unit->m_spawn->y;
			float z = _unit->m_spawn->z;
			float o = _unit->m_spawn->o;
			
			_unit->GetAIInterface()->MoveTo(x, y, z, o);
		}

		if (m_phase[_unit->GetInstanceID()] == 4)
		{
			RegisterAIUpdateEvent(250);

			DemonSoundRand = false;
			DemonPhase = 76;
		}

		FoA1 = NULLUNIT;
		FoA2 = NULLUNIT;
		Blade1 = NULLUNIT;
		Blade2 = NULLUNIT;

		/*if (FoA1) delete FoA1;
		if (FoA2) delete FoA2;
		if (Blade1) delete Blade1;
		if (Blade2) delete Blade2;*/

		if (_unit->isAlive()) Maiev->Delete();
    }

    void OnDied(UnitPointer mKiller)
    {
		if (m_phase[_unit->GetInstanceID()] == 2)
		{
			_unit->GetAIInterface()->m_moveFly = false;
			_unit->Emote(EMOTE_ONESHOT_LAND);
		}

		m_phase[_unit->GetInstanceID()] = 0;
		DemonSoundRand = false;
		DemonPhaseTimer = 30;
		DemonPhase = 0;
		
		RemoveAIUpdateEvent();

		if (!DeathEvent[_unit->GetInstanceID()] && MaievDialog[_unit->GetInstanceID()] >= 28 && Maiev && Maiev->isAlive())	// it's workaround (added pointer check, coz if he dies when Maiev is not on battlefield then it will crash server)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			//_unit->GetAIInterface()->SetNextTarget(NULL);
			_unit->GetAIInterface()->m_canMove = false;
			_unit->SetUInt32Value(UNIT_FIELD_HEALTH, 1);
			_unit->GetAIInterface()->StopMovement(0);	// needed?
			if(_unit->GetCurrentSpell() != NULL)
			    _unit->GetCurrentSpell()->cancel();

			Maiev->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			Maiev->GetAIInterface()->SetAIState(STATE_IDLE);
			Maiev->GetAIInterface()->WipeTargetList();
			Maiev->GetAIInterface()->WipeHateList();

			DeathEvent[_unit->GetInstanceID()] = 1;
			m_phase[_unit->GetInstanceID()] = 0;
		}
    }

	void AIUpdate()
	{
		if (_unit->CombatStatus.IsInCombat())
		{
			if (DemonPhase > 0 && DemonPhase < 75)	// 0;9
			{
				DemonTrans();
			}
	
			if (DemonPhase > 75 && DemonPhase < 134)	// 9;17
			{
				TransRevert();
			}
	
			else
			{
				if (m_phase[_unit->GetInstanceID()] == 1)	// First Phase (without demon form)
				{
					PhaseOne();
				}
			
				if (m_phase[_unit->GetInstanceID()] == 2)	// Second Phase (Fly phase)
				{
					PhaseTwo();
				}
		
				if (m_phase[_unit->GetInstanceID()] == 3)	// Third Phase normal form with ability to change into Demon
				{
					PhaseThree();
				}
		
				if (m_phase[_unit->GetInstanceID()] == 4)	// Fourth Phase Demon Form
				{
					PhaseFour();
				}
	
				if (m_phase[_unit->GetInstanceID()] == 5)	// Fifth Phase with ability to change into Demon and Enrage
				{
					PhaseFive();
				}
			}
		}
	}

	void PhaseOne()
	{
		if (_unit->GetHealthPct() <= 65 && _unit->GetCurrentSpell() == NULL)	// <-
		{
			if (_unit->GetCurrentSpell() != NULL)	// safe check after adding		^-- this
				_unit->GetCurrentSpell()->cancel();

			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I will not be touched by a rebels such as you!");	// not sure
			_unit->PlaySoundToSet(11479);

			_unit->GetAIInterface()->MoveTo(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ() + 10.0f, _unit->GetOrientation());
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->GetAIInterface()->m_moveFly = true;
			_unit->Emote(EMOTE_ONESHOT_LIFTOFF);

			WorldPacket data(SMSG_MOVE_SET_HOVER, 13);
            data << _unit->GetNewGUID();
            data << uint32(0);
            _unit->SendMessageToSet(&data, false);

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(500);

			BladesEvent[_unit->GetInstanceID()] = 1;
			FoADeaths[_unit->GetInstanceID()] = 2;
			m_phase[_unit->GetInstanceID()] = 2;
			TriggerSpawned = false;
			FireWallTimer = 0;
			LastFireWall = 0;
			m_currentWP = 0;
		}

		else
		{
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);	// in OnCombatStart or in Dialog part it doesn't work (in places I wanted to add it)
			float val = (float)RandomFloat(100.0f);
			SpellCast(val);
		}
	}

	void FoASummon()	// little hacky way to spawn them by pushing into world, but proper spell won't work now
	{					// we can eventually add them as pointers and trigger some events from here (like Blade's channeling)
		FoA1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_OF_AZZINOTH, 672.039246f, 326.748322f, 354.206390f, 0.207343f, true, false, 0, 0);
		FoA2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_FLAME_OF_AZZINOTH, 673.008667f, 283.813660f, 354.267548f, 6.203853f, true, false, 0, 0);
	
		if (Blade1 && FoA1)
		{
			Blade1->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, FoA1->GetGUID());
			Blade1->SetUInt32Value(UNIT_CHANNEL_SPELL, TEAR_OF_AZZINOTH_CHANNEL);
		}

		if (Blade2 && FoA2)
		{
			Blade2->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, FoA2->GetGUID());
			Blade2->SetUInt32Value(UNIT_CHANNEL_SPELL, TEAR_OF_AZZINOTH_CHANNEL);
		}
	}


	void PhaseTwo()
	{
		if ((BladesEvent[_unit->GetInstanceID()] && BladesEvent[_unit->GetInstanceID()] < 20) || (BladesEvent[_unit->GetInstanceID()] > 24 && BladesEvent[_unit->GetInstanceID()] < 45))
		{
			switch (BladesEvent[_unit->GetInstanceID()])
			{
			case 2:
				{
					_unit->GetAIInterface()->StopMovement(0);
					_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
					_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
					_unit->GetAIInterface()->setWaypointToMove(1);
				}break;
			case 10:	// Blade1 fly
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(THROW_GLAIVE2), false);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Behold the flames of Azzinoth!");
					_unit->PlaySoundToSet(11480);
				}break;
			case 12:
				{
					_unit->CastSpellAoF(676.717346f, 322.445251f, 354.153320f, dbcSpell.LookupEntry(THROW_GLAIVE1), false);
					_unit->CastSpellAoF(677.368286f, 285.374725f, 354.242157f, dbcSpell.LookupEntry(THROW_GLAIVE1), false);
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
				}break;
			case 14:		// Blades spawn
				{
					Blade1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLADE_OF_AZZINOTH, 676.717346f, 322.445251f, 354.153320f, 5.732623f, false, false, 0, 0);
					Blade2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_BLADE_OF_AZZINOTH, 677.368286f, 285.374725f, 354.242157f, 5.645614f, false, false, 0, 0);
					
					if (Blade1)
					{
						Blade1->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						Blade1->GetAIInterface()->SetAllowedToEnterCombat(false);
					}
					
					if (Blade2)
					{
						Blade2->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
						Blade2->GetAIInterface()->SetAllowedToEnterCombat(false);
					}
				}break;
			case 18:		// Flames of Azzinoth Spawn + channeling
				{
					FoASummon();
					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(1000);

					TriggerSpawned = false;
					FireWallTimer = 0;

					//_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);	//<-- flag blocks dmg of Fireball spell
					//_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
				}break;
			case 30:		// Blades return
				{
					_unit->CastSpell(_unit, dbcSpell.LookupEntry(GLAIVE_RETURNS), false);
					_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);	// he should also make something like jump emote when weapons are in his hands, but I can't find right one o_O
				}break;
			case 34:
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);	
					_unit->GetAIInterface()->m_moveFly = false;
					_unit->Emote(EMOTE_ONESHOT_LAND);

					WorldPacket data(SMSG_MOVE_UNSET_HOVER, 13);
					data << _unit->GetNewGUID();
					data << uint32(0);
					_unit->SendMessageToSet(&data, false);
				}break;
			case 40:	//36
				{
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->disable_melee = false;
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

					MaievDialog[_unit->GetInstanceID()] = 0;
					m_phase[_unit->GetInstanceID()] = 3;
					TriggerSpawned = false;
					DemonPhaseTimer = 30;
					FireWallTimer = 0;
					m_currentWP = 0;
				}break;
			}
			if (BladesEvent[_unit->GetInstanceID()] != 3)
				BladesEvent[_unit->GetInstanceID()]++;
		}

		else
		{
			if (BladesEvent[_unit->GetInstanceID()] > 18 && BladesEvent[_unit->GetInstanceID()] < 25)
			{
				if (Blade1 && FoA1 && !FoA1->isAlive())
				{
					Blade1->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					Blade1->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}

				if (Blade2 && FoA2 && !FoA2->isAlive())
				{
					Blade2->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
					Blade2->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
				}
			}

			if (!FoADeaths[_unit->GetInstanceID()] && FireWallTimer < 30 && _unit->GetCurrentSpell() != NULL)
			{
				_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
				_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
				_unit->GetAIInterface()->m_canMove = true;
				if(_unit->GetCurrentSpell() != NULL)
					_unit->GetCurrentSpell()->cancel();

				_unit->GetAIInterface()->StopMovement(0);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTMOVE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
				_unit->GetAIInterface()->setWaypointToMove(2);
				
				FoADeaths[_unit->GetInstanceID()] = 2;
			}

			if (m_currentWP != 2 && _unit->GetAIInterface()->getMoveType() != MOVEMENTTYPE_WANTEDWP && BladesEvent[_unit->GetInstanceID()] == 20)
			{
				FireWallTimer++;

				if (FireWallTimer >= 30)
				{
					if (TriggerSpawned == false)
					{
						float posX;
						float posY;
						float posZ;

						uint32 RandomWall = rand()%8+1;

						while ((RandomWall == LastFireWall) || (RandomWall == LastFireWall - 4) || (RandomWall == LastFireWall + 4))
						{
							RandomWall = rand()%8+1;
						}

						switch (RandomWall)
						{
						case 1:
							posX = 642.240601f;
							posY = 297.297180f;
							posZ = 353.423401f;
							break;
						case 2:
							posX = 641.197449f;
							posY = 314.330963f;
							posZ = 353.300262f;
							break;
						case 3:
							posX = 657.239807f;
							posY = 256.925568f;
							posZ = 352.996094f;
							break;
						case 4:
							posX = 657.913330f;
							posY = 353.562775f;
							posZ = 352.996185f;
							break;
						case 5:
							posX = 707.019043f;
							posY = 270.441772f;
							posZ = 352.996063f;
							break;
						case 6:
							posX = 706.592407f;
							posY = 343.425568f;
							posZ = 352.996124f;
							break;
						case 7:
							posX = 706.593262f;
							posY = 310.011475f;
							posZ = 353.693848f;
							break;
						case 8:
							posX = 706.751343f;
							posY = 298.312683f;
							posZ = 353.653809f;
							break;
						}

						Trigger = NULLUNIT;
						Trigger = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_EYE_BEAM_TRIGGER, posX, posY, posZ, 0.0f, false, false, 0, 0);
						
						if (Trigger && Trigger->isAlive() && Trigger->IsInWorld())
						{
							if(_unit->GetCurrentSpell() != NULL)
								_unit->GetCurrentSpell()->cancel();

							_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, Trigger->GetGUID());
							_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, EYE_BLAST1);
							_unit->CastSpell(Trigger, spells[14].info, true);
						}
						
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Stare into the eyes of the Betrayer!");	// must check this
						_unit->PlaySoundToSet(11481);

						LastFireWall = RandomWall;
						TriggerSpawned = true;
						FireWallTimer = 30;
					}

					if (TriggerSpawned == true && FireWallTimer == 43)
					{
						_unit->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT, 0);
						_unit->SetUInt32Value(UNIT_CHANNEL_SPELL, 0);
						_unit->RemoveAura(EYE_BLAST1);

						TriggerSpawned = false;
						FireWallTimer = 0;
					}
				}
				
				else if (_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
				{
					UnitPointer target = NULLUNIT;
					target = _unit->GetAIInterface()->GetNextTarget();

					int Spell = rand()%100;
		
					if (Spell >= 0 && Spell <= 25) // dunno why it doesn't make dmg, but looks correctly (it does damage when target can get into combat mode)
					{
						CastSpellOnRandomTarget(12, 0.0f, 60.0f, 0, 100);
					} 
					if (Spell > 25 && Spell <= 35) 
					{
						CastSpellOnRandomTarget(13, 0.0f, 60.0f, 0, 100);
					}
				} 
			}
		}
	 }

	void PhaseThree()
	{
		if (_unit->GetHealthPct() <= 30 && !MaievDialog[_unit->GetInstanceID()] && (DemonPhase <= 0 || DemonPhase >= 135 && !_unit->GetCurrentSpell()))	// add more stuff with maiev in future!
		{
			#ifdef USE_SHADOW_PRISON
				_unit->CastSpell(_unit, spells[8].info, spells[8].instant);
			#endif

			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
			_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->disable_melee = true;
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->m_canMove = false;
			if(_unit->GetCurrentSpell() != NULL)		// keeping it for now, will delete it later anyway
				_unit->GetCurrentSpell()->cancel();

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(1000);

			MaievDialog[_unit->GetInstanceID()] = 1;
		}

		if (MaievDialog[_unit->GetInstanceID()] && MaievDialog[_unit->GetInstanceID()] < 32 && (DemonPhase <= 0 || DemonPhase >= 135))
		{	// add her weapon switching (from belt to hand, emote cases etc.)
			switch (MaievDialog[_unit->GetInstanceID()])
			{
			case 3:		// 3
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Is this it, mortals? Is this all the fury you can muster?");
					_unit->PlaySoundToSet(11476);
				}break;
			case 6:		// 6
				{
					_unit->Emote(EMOTE_ONESHOT_QUESTION);
				}break;
			case 11:	// 12
				{
					SpawnMaiev();
					Maiev->GetAIInterface()->SetNextTarget(_unit);

					Maiev->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Their fury pales before mine, Illidan. We have some unsettled business between us.");
					Maiev->PlaySoundToSet(11491);

					_unit->GetAIInterface()->SetNextTarget(Maiev);
					//_unit->SetFacing(5.287312f);					// I must think about good formula for both units
					//_unit->GetAIInterface()->setInFront(Maiev);	// dunno why it doesn't work -.-'
				}break;
			case 12:	// 13
				{
					Maiev->CastSpell(_unit, dbcSpell.LookupEntry(TELEPORT), true);

					MaievWeaponOff();
				}break;
			case 13:	// 14
				{
					Maiev->Emote(EMOTE_ONESHOT_EXCLAMATION);
				}break;
			case 18:	// 20
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Maiev... How is it even possible?");
					_unit->PlaySoundToSet(11477);
				}break;
			case 21:	// 23
				{
					//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
				}break;
			case 24:	// 27
				{
					Maiev->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Ah, my long hunt is finally over. Today, Justice will be done!");
					Maiev->PlaySoundToSet(11492);

					Maiev->Emote(EMOTE_ONESHOT_EXCLAMATION);
				}break;
			case 27:	// 30
				{
					Maiev->Emote(EMOTE_ONESHOT_YES);
				}break;
			case 28:	// 31
				{
					Maiev->Emote(EMOTE_ONESHOT_ROAR);
				}break;
			case 31:	//34
				{
					_unit->GetAIInterface()->SetAllowedToEnterCombat(true);
					_unit->GetAIInterface()->disable_melee = false;
					_unit->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
					_unit->GetAIInterface()->m_canMove = true;

					Maiev->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					Maiev->GetAIInterface()->SetAllowedToEnterCombat(true);
					Maiev->GetAIInterface()->setCurrentAgent(AGENT_NULL);
					Maiev->GetAIInterface()->SetAIState(STATE_IDLE);
					Maiev->GetAIInterface()->m_canMove = true;

					Maiev->GetAIInterface()->AttackReaction(_unit, 1, 0);

					MaievWeaponOn();
					
					RemoveAIUpdateEvent();
					RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));
					
					m_phase[_unit->GetInstanceID()] = 5;
					DemonPhaseTimer = 30;
					EnrageTimer = 10;
				}break;
			}

			MaievDialog[_unit->GetInstanceID()]++;
		}
		
		else
		{
			DemonPhaseTimer--;

			if (!DemonPhaseTimer)
			{
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
				_unit->GetAIInterface()->disable_melee = true;
				if(_unit->GetCurrentSpell() != NULL)
					_unit->GetCurrentSpell()->cancel();
				
				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(250);

				DemonPhase = 1;
			}
	
			else
			{
				if (DemonPhase < 1 || DemonPhase > 134)
				{
					_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
					float val = (float)RandomFloat(100.0f);
					SpellCast(val);
				}
			}
		}
	}

	void DemonTrans()	// check and change how long spell 9 is in use on official
	{
		if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();

		switch (DemonPhase)
		{	// let's give it 4 free triggers to not use melee attacks and spells
		case 5:
			{
				_unit->CastSpell(_unit, spells[9].info, spells[9].instant);
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);

				//_unit->GetAIInterface()->disable_melee = true;
			}break;

		case 13:
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Behold the power... of the demon within!");
				_unit->PlaySoundToSet(11475);

				_unit->CastSpell(_unit, spells[10].info, spells[10].instant);
			}break;

		case 33:
			{
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21322);
				_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 5.0f);
			}break;

		case 43:
			{
				_unit->CastSpell(_unit, spells[11].info, spells[11].instant);

				//_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04);
				//_unit->RemoveAura(40398);	
			}break;

		case 59:
			{
				_unit->CastSpell(_unit, spells[11].info, spells[11].instant);

				//_unit->RemoveAura(40510);
			}break;

		case 61:	//63	//65	// he should also drop aggro and take new target (warlock should take it by dots casted before final change)
			{
				_unit->RemoveAura(40510);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

				DemonSoundRand = false;
				DemonPhaseTimer = 0;
				DemonPhase = 74;
				m_phase[_unit->GetInstanceID()] = 4;
			}break;
		}

		DemonPhase++;
	}

	void TransRevert()	// same here to spell 9 (but not that much I think); also I am thinking about changing spells on emotes
	{
		if(_unit->GetCurrentSpell() != NULL)
                _unit->GetCurrentSpell()->cancel();

		switch (DemonPhase)
		{
		case 81:	// 1 diff is too long?
			{
				_unit->CastSpell(_unit, spells[9].info, spells[9].instant);
				_unit->GetAIInterface()->m_canMove = false;

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 403);
				//_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL02);
			}break;

		case 84:	//85
			{
				_unit->CastSpell(_unit, spells[10].info, spells[10].instant);

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 404);
			}break;

		case 108:	//108	//109
			{
				_unit->SetUInt32Value(UNIT_FIELD_DISPLAYID , 21137);
				_unit->SetFloatValue(OBJECT_FIELD_SCALE_X , 1.5f);
			}break;

		case 114:	//116	//117
			{
				_unit->CastSpell(_unit, spells[11].info, spells[11].instant);

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 405);
				//_unit->Emote(EMOTE_ONESHOT_CUSTOMSPELL04);
			}break;

		case 125:	//122	//125	//123 - hand show	//125 - on rage showing lol	//126
			{
				_unit->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
			}break;

		case 129:	//133	//129	//130
			{
				_unit->GetAIInterface()->disable_melee = false;
				_unit->GetAIInterface()->m_canMove = true;
				_unit->RemoveAura(40510);

				if (!MaievDialog[_unit->GetInstanceID()])
					m_phase[_unit->GetInstanceID()] = 3;
				else
				{
					EnrageTimer = rand()%10+11;
					m_phase[_unit->GetInstanceID()] = 5;
				}
				
				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(_unit->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME));

				DemonPhaseTimer = 30;
				DemonPhase = 135;

				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);
				//_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			}break;
		}

		DemonPhase++;
	}

	void PhaseFour()
	{
		DemonPhaseTimer++;

		if (DemonPhaseTimer == 15)
		{
			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(250);

			DemonSoundRand = false;
			DemonPhase = 76;
		}

		if (DemonPhaseTimer < 2 && MaievDialog[_unit->GetInstanceID()] && !DemonSoundRand)
		{
			int Sound = rand()%3;	// 66% chance to say something, should work fine

			switch (Sound)
			{
			case 0:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You know nothing of power!");
					_unit->PlaySoundToSet(11469);

					DemonSoundRand = true;
				}break;
			case 1:
				{
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Such arrogance!");
					_unit->PlaySoundToSet(11471);

					DemonSoundRand = true;
				}break;
			}
		}

		if (_unit->GetHealthPct() <= 30 && !MaievDialog[_unit->GetInstanceID()])
		{
			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(250);

			DemonSoundRand = false;
			DemonPhase = 76;
		}

		else
		{
			if (_unit->GetAIInterface()->GetNextTarget() && DemonPhase == 75)
			{
				UnitPointer target = NULLUNIT;
				target = _unit->GetAIInterface()->GetNextTarget();

				if (_unit->GetDistance2dSq(target) >= 0.0f && _unit->GetDistance2dSq(target) <= 10000.0f)
				{
					_unit->GetAIInterface()->m_canMove = false;

					if (_unit->GetCurrentSpell()) return;

					int Spell = rand()%100;
					if (Spell >= 0 && Spell <= 80)
						_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[6].info, spells[6].instant);
					if (Spell > 80 && Spell <= 90) return;
						//_unit->CastSpell(_unit, spells[7].info, spells[7].instant);	// disabled for now as it can cause some unneeded animations			// both should have _unit as target
					if (Spell > 90 && Spell <= 100)
						_unit->CastSpell(_unit, spells[1].info, spells[1].instant);	// this one needs to be scripted
				}

				else
					_unit->GetAIInterface()->m_canMove = true;
			}
		}
	}

	void PhaseFive()
	{
		DemonPhaseTimer--;
		EnrageTimer--;

		if (!DemonPhaseTimer)
		{
			_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
			_unit->GetAIInterface()->disable_melee = true;
			if(_unit->GetCurrentSpell() != NULL)
				_unit->GetCurrentSpell()->cancel();

			RemoveAIUpdateEvent();
			RegisterAIUpdateEvent(250);
			
			DemonPhase = 1;
		}

		if (!EnrageTimer)
		{
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You've wasted too much time mortals, now you shall fall!");
			_unit->PlaySoundToSet(11474);

			_unit->CastSpell(_unit, spells[5].info, spells[5].instant);
		}

		else
		{
			if (DemonPhase < 1 || DemonPhase > 134)
			{
				_unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_READY1H);

				float val = (float)RandomFloat(100.0f);
				SpellCast(val);
			}
		}
	}

	void SpellCast(float val)
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			float comulativeperc = 0;
		    UnitPointer target = NULLUNIT;
			for(int i=0;i<nrspells;i++)
			{
				if(!spells[i].perctrigger) continue;
				
				if(m_spellcheck[i])
				{
					target = _unit->GetAIInterface()->GetNextTarget();
					switch(spells[i].targettype)
					{
						case TARGET_SELF:
						case TARGET_VARIOUS:
							_unit->CastSpell(_unit, spells[i].info, spells[i].instant); break;
						case TARGET_ATTACKING:
							_unit->CastSpell(target, spells[i].info, spells[i].instant); break;
						case TARGET_DESTINATION:
							_unit->CastSpellAoF(target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), spells[i].info, spells[i].instant); break;
						case TARGET_RANDOM_FRIEND:
						case TARGET_RANDOM_SINGLE:
						case TARGET_RANDOM_DESTINATION:
							CastSpellOnRandomTarget(i, spells[i].mindist2cast, spells[i].maxdist2cast, spells[i].minhp2cast, spells[i].maxhp2cast); break;
					}
					m_spellcheck[i] = false;
					return;
				}

				uint32 t = (uint32)time(NULL);
				if(val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger) && t > spells[i].casttime)
				{
					if ((i == 4 && m_phase[_unit->GetInstanceID()] == 3) || i != 4)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						spells[i].casttime = t + spells[i].cooldown;
						m_spellcheck[i] = true;
					}
				}
				comulativeperc += spells[i].perctrigger;
			}
        }
    }

	void CastSpellOnRandomTarget(uint32 i, float mindist2cast, float maxdist2cast, int minhp2cast, int maxhp2cast)
	{
		if (!maxdist2cast) maxdist2cast = 100.0f;
		if (!maxhp2cast) maxhp2cast = 100;

		if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
        {
			std::vector<UnitPointer > TargetTable;		/* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
												/* If anyone wants to use this function, then leave this note!										 */
			for(unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeSetBegin(); itr != _unit->GetInRangeSetEnd(); ++itr) 
			{ 
				if (((spells[i].targettype == TARGET_RANDOM_FRIEND && isFriendly(_unit, (*itr))) || (spells[i].targettype != TARGET_RANDOM_FRIEND && isHostile(_unit, (*itr)) && (*itr) != _unit)) && ((*itr)->GetTypeId()== TYPEID_UNIT || (*itr)->GetTypeId() == TYPEID_PLAYER) && (*itr)->GetInstanceID() == _unit->GetInstanceID()) // isAttackable(_unit, (*itr)) && 
				{
					UnitPointer RandomTarget = NULLUNIT;
					RandomTarget = TO_UNIT(*itr);

					if (RandomTarget->isAlive() && _unit->GetDistance2dSq(RandomTarget) >= mindist2cast*mindist2cast && _unit->GetDistance2dSq(RandomTarget) <= maxdist2cast*maxdist2cast && ((RandomTarget->GetHealthPct() >= minhp2cast && RandomTarget->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND) || (_unit->GetAIInterface()->getThreatByPtr(RandomTarget) > 0 && isHostile(_unit, RandomTarget))))
					{
						TargetTable.push_back(RandomTarget);
					} 
				} 
			}

			if (_unit->GetHealthPct() >= minhp2cast && _unit->GetHealthPct() <= maxhp2cast && spells[i].targettype == TARGET_RANDOM_FRIEND)
				TargetTable.push_back(_unit);

			if (!TargetTable.size())
				return;

			size_t RandTarget = rand()%TargetTable.size();

			UnitPointer RTarget = TargetTable[RandTarget];

			if (!RTarget)
				return;

			switch (spells[i].targettype)
			{
			case TARGET_RANDOM_FRIEND:
			case TARGET_RANDOM_SINGLE:
				_unit->CastSpell(RTarget, spells[i].info, spells[i].instant); break;
			case TARGET_RANDOM_DESTINATION:
				_unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant); break;
			}

			TargetTable.clear();
		}
	}

	void SpawnMaiev()	// this doesn't have collision checks! so keep in mind that Maiev can be spawned behind walls!
	{
		float xchange  = (float)RandomFloat(15.0f);
		float distance = 15.0f;

		float ychange = sqrt(distance*distance - xchange*xchange);

		if (rand()%2 == 1)
			xchange *= -1;
		if (rand()%2 == 1)
			ychange *= -1;

		float newposx = _unit->GetPositionX() + xchange;
		float newposy = _unit->GetPositionY() + ychange;

		Maiev = _unit->GetMapMgr()->GetInterface()->SpawnCreature(CN_MAIEV, newposx, newposy, _unit->GetPositionZ() + 0.5f, 2.177125f, true, false, 0, 0);
	}

	void MaievWeaponOff()
	{
		/*Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 44850);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 218169346);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 4);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);*/
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID_1, 32425);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 0);
	}

	void MaievWeaponOn()
	{
		/*Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 44850);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 218169346);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 4);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_02, 0);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_03, 0);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);*/	
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 32425);
		Maiev->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID_1, 0);
		Maiev->SetUInt32Value(UNIT_FIELD_BYTES_2, 1);
	}

	void OnReachWP(uint32 iWaypointId, bool bForwards)
    {
		switch(iWaypointId)
		{
		case 1:
			{
				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);

				BladesEvent[_unit->GetInstanceID()] = 4;

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(500);
			}break;
		case 2:
			{
				_unit->GetAIInterface()->m_canMove = false;

				_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
				_unit->GetAIInterface()->SetAIState(STATE_SCRIPTIDLE);
				_unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_DONTMOVEWP);
				_unit->GetAIInterface()->setWaypointToMove(0);

				RemoveAIUpdateEvent();
				RegisterAIUpdateEvent(500);

				BladesEvent[_unit->GetInstanceID()] = 25;
				m_currentWP = 2;
			}break;
		}
    }

    inline WayPoint* CreateWaypoint(int id, uint32 waittime, uint32 flags)
    {
        WayPoint * wp = _unit->CreateWaypointStruct();
        wp->id = id;
        wp->x = ForIllidan[id].mX;
        wp->y = ForIllidan[id].mY;
        wp->z = ForIllidan[id].mZ;
        wp->o = ForIllidan[id].mO;
        wp->waittime = waittime;
        wp->flags = flags;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        return wp;
    }

protected:

	uint32 FireWallTimer;
	bool DemonSoundRand;
	uint32 LastFireWall;
	bool TriggerSpawned;
	int DemonPhaseTimer;
	uint32 m_currentWP;
	int EnrageTimer;
	int DemonPhase;
	UnitPointer Trigger;
	int nrspells;
	UnitPointer Blade1;
	UnitPointer Blade2;
	UnitPointer Maiev;
	UnitPointer FoA1;
	UnitPointer FoA2;
};


void SetupBlackTemple(ScriptMgr * mgr)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	///////// Mobs
	mgr->register_creature_script(CN_DRAGON_TURTLE, &DragonTurtleAI::Create);
	mgr->register_creature_script(CN_LEVIATHAN, &LeviathanAI::Create);
	mgr->register_creature_script(CN_MUTANT_WAR_HOUND, &MutantWarHoundAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_RIDING_HOUND, &ShadowmoonRidingHoundAI::Create);
	mgr->register_creature_script(CN_SISTER_OF_PLEASURE, &SisterOfPleasureAI::Create);
	mgr->register_creature_script(CN_SISTER_OF_PAIN, &SisterOfPainAI::Create);
	mgr->register_creature_script(CN_PRIESTESS_OF_DEMENTIA, &PriestessOfDementiaAI::Create);
	mgr->register_creature_script(CN_PRIESTESS_OF_DELIGHT, &PriestessOfDelightAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_NIGHTLORD, &IllidariNightlordAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_HEARTSEEKER, &IllidariHeartseekerAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_FEARBRINGER, &IllidariFearbringerAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_DEFILER, &IllidariDefilerAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_CENTURION, &IllidariCenturionAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_BONESLICER, &IllidariBoneslicerAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_BATTLELORD, &AshtongueBattlelordAI::Create);
	//mgr->register_creature_script(CN_ASHTONGUE_DEFENDER, &AshtongueDefenderAI::Create);
	//mgr->register_creature_script(CN_ASHTONGUE_ELEMENTALIST, &AshtongueElementalistAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_MYSTIC, &AshtongueMysticAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_PRIMALIST, &AshtonguePrimalistAI::Create);
	//mgr->register_creature_script(CN_ASHTONGUE_ROGUE, &AshtongueRogueAI::Create);
	//mgr->register_creature_script(CN_ASHTONGUE_SPIRITBINDER, &AshtongueSpiritbinderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_STALKER, &AshtongueStalkerAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_STORMCALLER, &AshtongueStormcallerAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BEHEMOTH, &BonechewerBehemothAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BLADE_FURY, &BonechewerBladeFuryAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BLOOD_PROPHET, &BonechewerBloodProphetAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_BRAWLER, &BonechewerBrawlerAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_COMBATANT, &BonechewerCombatantAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_SHIELD_DISCIPLE, &BonechewerShieldDiscipleAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_SPECTATOR, &BonechewerSpectatorAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_TASKMASTER, &BonechewerTaskmasterAI::Create);
	mgr->register_creature_script(CN_BONECHEWER_WORKER, &BonechewerWorkerAI::Create);
	mgr->register_creature_script(CN_CHARMING_COURTESAN, &CharmingCourtesanAI::Create);
	mgr->register_creature_script(CN_COILSKAR_GENERAL, &CoilskarGeneralAI::Create);
	mgr->register_creature_script(CN_COILSKAR_HARPOONER, &CoilskarHarpoonerAI::Create);
	mgr->register_creature_script(CN_COILSKAR_SEACALLER, &CoilskarSeacallerAI::Create);
	mgr->register_creature_script(CN_COILSKAR_SOOTHSAYER, &CoilskarSoothsayerAI::Create);
	mgr->register_creature_script(CN_COILSKAR_WRANGLER, &CoilskarWranglerAI::Create);
	mgr->register_creature_script(CN_DRAGONMAW_SKY_STALKER, &DragonmawSkyStalkerAI::Create);
	mgr->register_creature_script(CN_DRAGONMAW_WIND_REAVER, &DragonmawWindReaverAI::Create);
	mgr->register_creature_script(CN_DRAGONMAW_WYRMCALLER, &DragonmawWyrmcallerAI::Create);
	mgr->register_creature_script(CN_ENSLAVED_SERVANT, &EnslavedServantAI::Create);
	mgr->register_creature_script(CN_HAND_OF_GOREFIEND, &HandOfGorefiendAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_ARCHON, &IllidariArchonAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_ASSASSIN, &IllidariAssassinAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_BATTLEMAGE, &IllidariBattlemageAI::Create);
	mgr->register_creature_script(CN_ILLIDARI_BLOOD_LORD, &IllidariBloodLordAI::Create);
	mgr->register_creature_script(CN_IMAGE_OF_DEMENTIA, &ImageOfDementiaAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_BLOOD_MAGE, &ShadowmoonBloodMageAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_CHAMPION, &ShadowmoonChampionAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_DEATHSHAPER, &ShadowmoonDeathshaperAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_FALLEN, &ShadowmoonFallenAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_HOUNDMASTER, &ShadowmoonHoundmasterAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_REAVER, &ShadowmoonReaverAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_SOLDIER, &ShadowmoonSoldierAI::Create);
	mgr->register_creature_script(CN_SHADOWMOON_WEAPON_MASTER, &ShadowmoonWeaponMasterAI::Create);
	mgr->register_creature_script(CN_SPELLBOUND_ATTENDANT, &SpellboundAttendantAI::Create);
	mgr->register_creature_script(CN_TEMPLE_CONCUBINE, &TempleConcubineAI::Create);
	mgr->register_creature_script(CN_STORM_FURY, &StormFuryAI::Create);
	mgr->register_creature_script(CN_AQUEOUS_SURGER, &AqueousSurgerAI::Create);
	mgr->register_creature_script(CN_AQUEOUS_SPAWN, &AqueousSpawnAI::Create);
	mgr->register_creature_script(CN_AQUEOUS_LORD, &AqueousLordAI::Create);
	mgr->register_creature_script(CN_PROMENADE_SENTINEL, &PromenadeSentinelAI::Create);
	mgr->register_creature_script(CN_ANGERED_SOUL_FRAGMENT, &AngeredSoulFragmentAI::Create);
	//mgr->register_creature_script(CN_ASHTONGUE_FERAL_SPIRIT, &AshtongueFeralSpiritAI::Create);//NO USADO POR CRASHEAR EL SERVR
	//mgr->register_creature_script(CN_ENSLAVED_SOUL, &EnslavedSoulAI::Create);
	mgr->register_creature_script(CN_HUNGERING_SOUL_FRAGMENT, &HungeringSoulFragmentAI::Create);
	mgr->register_creature_script(CN_SHADOWY_CONSTRUCT, &ShadowyConstructAI::Create);
	mgr->register_creature_script(CN_SUFFERING_SOUL_FRAGMENT, &SufferingSoulFragmentAI::Create);
	mgr->register_creature_script(CN_VANGEFUL_SPIRIT, &VangefulSpiritAI::Create);
	mgr->register_creature_script(CN_WRATHBONE_FLAYER, &WrathboneFlayerAI::Create);

	//Bosses
	
	//supremus
	mgr->register_creature_script(CN_SUPREMUS, &SupremusAI::Create);
	mgr->register_creature_script(CN_VOLCANO, &VolcanoAI::Create);
	mgr->register_creature_script(CN_MOLTEN, &MoltenFlameAI::Create);
	
	//High Warlord Naj'entus
	mgr->register_creature_script(CN_NAJENTUS, &NajentusAI::Create);
    
	// Gurtogg bloodboil
	mgr->register_creature_script(CN_GURTOGG_BLOODBOIL, &GurtoggAI::Create);
	
	//Reliquary of the Lost
	mgr->register_creature_script(CN_RELIQUARYOFTHELOST, &ReliquaryoftheLostAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFSUFFERING, &EssenceOfSufferingAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFDESIRE, &EssenceOfDesireAI::Create);
	mgr->register_creature_script(CN_ESSENCEOFANGER, &EssenceOfAngerAI::Create);
	mgr->register_creature_script(CN_ENSLAVEDSOUL, &EnslavedSoulAI::Create);
	
	//mother sharaz
	mgr->register_creature_script(CN_MOTHER_SHAHRAZ, &ShahrazAI::Create);
	
	// the illidari concilium
	mgr->register_creature_script(CN_LADY_MALANDE, &MalandeAI::Create);
	mgr->register_creature_script(CN_GATHIOS_THE_SHATTERER, &GathiosAI::Create);
	mgr->register_creature_script(CN_HIGH_NETHERMANCER_ZEREVOR, &ZerevorAI::Create);
	mgr->register_creature_script(CN_VERAS_DARKSHADOW, &VerasAI::Create);
	
	//teron gorefiend
	mgr->register_creature_script(CN_TERON_GOREFIEND, &TeronGorefiendAI::Create);
	mgr->register_creature_script(DOOM_BLOSSOM, &DOOMBLOSSOMAI::Create);
	//mgr->register_creature_script(CN_SHADOWY_CONSTRUCTS, &ShadowyconstructsAI::Create); testeando en codigo privado.
	
	//shade of akama event y relacionados
	mgr->register_creature_script(CN_SHADE_OF_AKAMA, &ShadeofakamaAI::Create);
 
	mgr->register_creature_script(CN_ASHTONGUE_DEFENDER, &AshtonguedefenderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ELEMENTALIST, &AshtongueelementalistAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_ROGUE, &AshtonguerogueAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_SPIRITBINDER, &AshtonguespiritbinderAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_CHANNELER, &AshtongueChannelerAI::Create);
	mgr->register_creature_script(CN_TRIGGERSOFAKAMA, &TriggerShadeofAkamaAI::Create);
	mgr->register_creature_script(CN_AKAMAFAKE, &AkamaFakeAI::Create);
	mgr->register_creature_script(CN_ASHTONGUE_SORCERER, &AshtongueSorcererAI::Create);
	mgr->register_creature_script(CN_BROKEN_ASHTONGUE, &BrokenAshtongueAI::Create);
	GossipScript * AFG = (GossipScript*) new AkamaFakeGossip();
	mgr->register_gossip_script(CN_AKAMAFAKE, AFG);
	
	// illidan relacionados
		GossipScript * AG = (GossipScript*) new AkamaGossip();
	mgr->register_gossip_script(CN_AKAMA, AG);

	mgr->register_creature_script(CN_DOOR_EVENT_TRIGGER, &DoorEventTriggerAI::Create);
	mgr->register_creature_script(CN_DEMON_FIRE, &DemonFireAI::Create);
	mgr->register_creature_script(CN_EYE_BEAM_TRIGGER, &EyeBeamTriggerAI::Create);
	mgr->register_creature_script(CN_FLAME_CRASH, &FlameCrashAI::Create);
	mgr->register_creature_script(CN_BLAZE, &BlazeAI::Create);
	mgr->register_creature_script(CN_FLAME_OF_AZZINOTH, &FlameOfAzzinothAI::Create);
	mgr->register_creature_script(CN_AKAMA, &AkamaAI::Create);
	mgr->register_creature_script(CN_MAIEV, &MaievAI::Create);
	mgr->register_creature_script(CN_BLADE_OF_AZZINOTH, &BladeOfAzzinothAI::Create);
	mgr->register_creature_script(CN_ILLIDAN_STORMRAGE, &IllidanStormrageAI::Create);

	
}




