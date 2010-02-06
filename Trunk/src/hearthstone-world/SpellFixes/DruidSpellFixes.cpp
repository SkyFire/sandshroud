/*
 * Aspire Hearthstone
 * Copyright (C) 2008 - 2010 AspireDev <http://www.aspiredev.net/>
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

enum FixedDruidSpells
{
	SPELL_DRUID_ENRAGE = 50334,
	SPELL_FERAL_SWIFTNESS_RANK_1 = 17002,
	SPELL_FERAL_SWIFTNESS_RANK_2 = 24866,
	SPELL_FERAL_SWIFTNESS_DODGE_1 = 24867,
	SPELL_FERAL_SWIFTNESS_DODGE_2 = 24864,
	SPELL_MOONKIN_FORM_PASSIVE = 24905,
	SPELL_NATURAL_PERFECTION_1 = 33881,
	SPELL_NATURAL_PERFECTION_2 = 33882,
	SPELL_NATURAL_PERFECTION_3 = 33883,
	SPELL_FRENZIED_REGENERATION_1 = 22842,
	SPELL_FRENZIED_REGENERATION_2 = 22895,
	SPELL_FRENZIED_REGENERATION_3 = 22896,
	SPELL_FRENZIED_REGENERATION_4 = 26999,
    SPELL_CELESTIAL_FOCUS_1 = 16850,
    SPELL_CELESTIAL_FOCUS_2 = 16923,
    SPELL_CELESTIAL_FOCUS_3 = 16924,
    SPELL_LEADER_OF_THE_PACK = 24932,
    SPELL_IMPROVED_LEADER_OF_THE_PACK = 34299,
    SPELL_DRUID_CLEARCASTING = 16870,
    SPELL_DRUID_NATURES_GRACE = 16886,
};

enum DruidAdditionalAuras
{
	SPELL_ENRAGE_BERSERK = 58923,
	SPELL_FRENZIED_REGENERATION_HEAL = 22845,
};

void ApplySingleDruidSpellFixes(SpellEntry *sp)
{
	if(!sp)
		return;
	
	switch(sp->Id)
	{
		case SPELL_DRUID_ENRAGE:
		{
			sp->AdditionalAura = SPELL_ENRAGE_BERSERK;
		}break;

		case SPELL_FERAL_SWIFTNESS_RANK_1:
		{
			sp->AdditionalAura = SPELL_FERAL_SWIFTNESS_DODGE_1;
		}break;
		case SPELL_FERAL_SWIFTNESS_RANK_2:
		{
			sp->AdditionalAura = SPELL_FERAL_SWIFTNESS_DODGE_2;
		}break;

		case SPELL_FERAL_SWIFTNESS_DODGE_1:
		case SPELL_FERAL_SWIFTNESS_DODGE_2:
		{
			sp->apply_on_shapeshift_change = true;
		}break;

		case SPELL_MOONKIN_FORM_PASSIVE: 
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}break;

		case SPELL_NATURAL_PERFECTION_1:
		case SPELL_NATURAL_PERFECTION_2:
		case SPELL_NATURAL_PERFECTION_3:
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		}break;

		case SPELL_FRENZIED_REGENERATION_1:
		case SPELL_FRENZIED_REGENERATION_2:
		case SPELL_FRENZIED_REGENERATION_3:
		case SPELL_FRENZIED_REGENERATION_4:
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = SPELL_FRENZIED_REGENERATION_HEAL;
		}break;

		case SPELL_CELESTIAL_FOCUS_1:
		{
			sp->procChance = 3;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		case SPELL_CELESTIAL_FOCUS_2:	
		{
			sp->procChance = 6;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		case SPELL_CELESTIAL_FOCUS_3:	
		{
			sp->procChance = 9;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		case SPELL_LEADER_OF_THE_PACK:
		{
			sp->Effect[1] = 0;
			sp->Effect[2] = 0; //removing strange effects.
		}break;

		case SPELL_IMPROVED_LEADER_OF_THE_PACK:
		{
			sp->proc_interval = 6000;//6 secs
		}break;

		case SPELL_DRUID_CLEARCASTING:
		case SPELL_DRUID_NATURES_GRACE:
		{
			sp->procCharges	=	2;
		}break;	
	}
}