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

void ApplySingleSpellFixes(SpellEntry *sp)
{
	/* Note: when applying spell hackfixes, please follow a template */
	switch(sp->Id)
	{
	case 196: // 1H Axes
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_HUNTER|CLASSMASK_ROGUE|CLASSMASK_DEATHKNIGHT|CLASSMASK_SHAMAN;
		break;

	case 197: // 2H Axes
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_HUNTER|CLASSMASK_DEATHKNIGHT|CLASSMASK_SHAMAN;
		break;

	case 198: // 1H Maces
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_ROGUE|CLASSMASK_PRIEST|CLASSMASK_DEATHKNIGHT|CLASSMASK_SHAMAN|CLASSMASK_DRUID;
		break;

	case 199: // 2H Maces
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_DEATHKNIGHT|CLASSMASK_SHAMAN|CLASSMASK_DRUID;
		break;

	case 200: // Polearms
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_HUNTER|CLASSMASK_DEATHKNIGHT|CLASSMASK_DRUID;
		break;

	case 201: // 1H Swords
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_HUNTER|CLASSMASK_ROGUE|CLASSMASK_DEATHKNIGHT|CLASSMASK_MAGE|CLASSMASK_WARLOCK;
		break;

	case 202: // 2H Swords
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_PALADIN|CLASSMASK_HUNTER|CLASSMASK_DEATHKNIGHT;
		break;

	case 227: // Staves
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_HUNTER|CLASSMASK_PRIEST|CLASSMASK_SHAMAN|CLASSMASK_MAGE|CLASSMASK_WARLOCK|CLASSMASK_DRUID;
		break;

	case 1180: // Daggers
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_HUNTER|CLASSMASK_ROGUE|CLASSMASK_PRIEST|CLASSMASK_SHAMAN|CLASSMASK_MAGE|CLASSMASK_WARLOCK|CLASSMASK_DRUID;
		break;

	case 15590: // Fist Weapons
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_HUNTER|CLASSMASK_ROGUE|CLASSMASK_SHAMAN|CLASSMASK_DRUID;
		break;

	case 5009: // Wands
		sp->trnr_req_clsmsk = CLASSMASK_PRIEST|CLASSMASK_MAGE|CLASSMASK_WARLOCK;
		break;

	case 264: // Bows
	case 266: // Guns
	case 2567: // Thrown
	case 5011: // Crossbows
		sp->trnr_req_clsmsk = CLASSMASK_WARRIOR|CLASSMASK_HUNTER|CLASSMASK_ROGUE;
		break;

	case 1455: // Spell 1455 Proc Chance (Life Tap Rank 2:
	case 1456: // Spell 1456 Proc Chance (Life Tap Rank 3:
	case 8182: // Spell 8182 Proc Chance (Frost Resistance Rank 1:
	case 8185: // Spell 8185 Proc Chance (Fire Resistance Rank 1:
	case 10476: // Spell 10476 Proc Chance (Frost Resistance Rank 2:
	case 10477: // Spell 10477 Proc Chance (Frost Resistance Rank 3:
	case 10534: // Spell 10534 Proc Chance (Fire Resistance Rank 2:
	case 10535: // Spell 10535 Proc Chance (Fire Resistance Rank 3:
	case 11687: // Spell 11687 Proc Chance (Life Tap Rank 4:
	case 11688: // Spell 11688 Proc Chance (Life Tap Rank 5:
	case 11689: // Spell 11689 Proc Chance (Life Tap Rank 6:
	case 12292: // Spell 12292 Proc Chance (Death Wish :
	case 18803: // Spell 18803 Proc Chance (Focus :
			sp->procChance = 100;
		break;

	case 3391: // Spell 3391 Proc Chance (Thrash :
	case 15494: // Spell 15494 Proc Chance (Fury of Forgewright :
	case 15601: // Spell 15601 Proc Chance (Hand of Justice :
	case 15642: // Spell 15642 Proc Chance (Ironfoe :
	case 16843: // Spell 16843 Proc Chance (Crimson Fury :
	case 18797: // Spell 18797 Proc Chance (Flurry Axe :
	case 19105: // Spell 19105 Proc Chance (MHTest01 Effect :
	case 19109: // Spell 19109 Proc Chance (MHTest02 Effect :
	case 27035: // Spell 27035 Proc Chance (Sword Specialization (OLD: :
	case 21919: // Spell 21919 Proc Chance (Thrash :
			sp->procChance = 10;
		break;

		// Spell 12284 Proc Chance (Mace Specialization Rank 1:
	case 12284:
			sp->procChance = 1;
		break;

		// Spell 12322 Proc Chance (Unbridled Wrath Rank 1:
	case 12322:
			sp->ProcsPerMinute = 3;
		break;

		// Spell 12701 Proc Chance (Mace Specialization Rank 2:
	case 12701:
			sp->procChance = 2;
		break;

		// Spell 12702 Proc Chance (Mace Specialization Rank 3:
	case 12702:
			sp->procChance = 3;
		break;

		// Spell 12703 Proc Chance (Mace Specialization Rank 4:
	case 12703:
			sp->procChance = 4;
		break;

		// Spell 12704 Proc Chance (Mace Specialization Rank 5:
	case 12704:
			sp->procChance = 6;
		break;

		// Spell 12999 Proc Chance (Unbridled Wrath Rank 2:
	case 12999:
			sp->ProcsPerMinute = 6;
		break;

		// Spell 13000 Proc Chance (Unbridled Wrath Rank 3:
	case 13000:
			sp->ProcsPerMinute = 9;
		break;

		// Spell 13001 Proc Chance (Unbridled Wrath Rank 4:
	case 13001:
			sp->ProcsPerMinute = 12;
		break;

		// Spell 13002 Proc Chance (Unbridled Wrath Rank 5:
	case 13002:
			sp->ProcsPerMinute = 15;
		break;

		// Spell 14076 Proc Chance (Dirty Tricks Rank 1:
	case 14076:
			sp->procChance = 30;
		break;

		// Spell 14094 Proc Chance (Dirty Tricks Rank 2:
	case 14094:
			sp->procChance = 60;
		break;

		// Spell 15600 Proc Chance (Hand of Justice :
	case 15600:
			sp->procChance = 2;
		break;

		// Spell 23158 Proc Chance (Concussive Shot Cooldown Reduction :
	case 23158:
			sp->procChance = 4;
		break;

		// Spell 27521 Proc Chance (Mana Restore :
	case 27521:
			sp->procChance = 2;
		break;

		// Spell 27867 Proc Chance (Freeze :
	case 27867:
			sp->procChance = 2;
		break;

		// Spell 27498 Crusader's Wrath
	case 27498:
			sp->procChance = 7;
		break;

		//Martyrdom
	case 14531:
	case 14774:
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM;
		}break;

		// Impact proc
	case 64343:
		{
			sp->AuraInterruptFlags	|= AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		// Elemental Focus
	case 16164:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 16246:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procCharges++; //	first	charge gets	lost when	it gets	procced
		}break;

		//shaman - Stormstrike
	case 17364:
		{
			sp->procFlags=PROC_ON_SPELL_HIT_VICTIM;
		}break;

		//shaman - Improved Stormstrike
	case 51521:
	case 51522:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		//shaman - Hex
	case 51514:
		{
				// Damage	caused may interrupt the effect.
			sp->AuraInterruptFlags |=	AURA_INTERRUPT_ON_WEAPON_UNSHEATH;
		}break;

	case 52752:
		{
			sp->spell_can_crit = false;
			sp->logsId = sp->Id;
		}break;

		// shaman	-	Maelstrom	Weapon
	case 51528:
		{
			sp->procChance = 0;
			sp->ProcsPerMinute = 1.3f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;
	case 51529:
		{
			sp->procChance = 0;
			sp->ProcsPerMinute = 2.6f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;
	case 51530:
		{
			sp->procChance = 0;
			sp->ProcsPerMinute = 3.9f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;
	case 51531:
		{
			sp->procChance = 0;
			sp->ProcsPerMinute = 5.2f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;
	case 51532:
		{
			sp->procChance = 0;
			sp->ProcsPerMinute = 6.5f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;

		// Maelstorm proc	charge removal
	case 53817:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		// Astral	Shift
	case 52179:
		{
			sp->EffectAmplitude[0] = 0;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectMiscValue[0] = 127;
		}break;

		// Shamanistic Rage
	case 30823:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procflags2 = PROC_TARGET_SELF;
		}break;

	case 51479:
	case 51478:
	case 51474: // Astral Shift
		{
			sp->procChance = 100;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_SPELL_LAND_VICTIM;
			sp->EffectTriggerSpell[0] = 52179;
			sp->Effect[1] = 0;
		}break;

	case 51975:// shaman	-	Poison Cleansing Totem
	case 52025:// shaman	-	Disease	Cleansing	Totem
		{
				
			SpellEntry*  sp2 = dbcSpell.LookupEntryForced( 58780 );
			if( sp2 != NULL)
			{
				sp->EffectRadiusIndex[0] = sp2->EffectRadiusIndex[0];	//30 yards
				sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY_IN_AREA;
			}
		}break;

		// Fire	Nova Totem visual	fix
	case 8349:
	case 8502:
	case 8503:
	case 11306:
	case 11307:
	case 25535:
	case 25537:
	case 61650:
	case 61654:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	19823;
		}break;

		// Flurry
	case 12319:
	case 12971:
	case 12972:
	case 12973:
	case 12974:
	case 16256:
	case 16281:
	case 16282:
	case 16283:
	case 16284:
		{
			sp->procFlags	=	PROC_ON_CRIT_ATTACK;
		}break;

		// Flurry	proc
	case 12966:
	case 12967:
	case 12968:
	case 12969:
	case 12970:
	case 16257:
	case 16277:
	case 16278:
	case 16279:
	case 16280:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->procCharges++; //	first	charge gets	lost when	it gets	procced
		}break;

		//Minor Glyph Research + Northrend Inscription Research
	case 61177:
	case 61288:
	case 60893:
		{
			sp->Effect[1]	=	0;
			sp->EffectBasePoints[1]	=	0;
			sp->EffectImplicitTargetA[1]	=	0;
			sp->EffectDieSides[1]	=	0;
		}break;

		// Shaman	Totem	items	fixes
		// Totem of	Survival,	Totem	of the Tundra
	case 46097:
	case 43860:
	case 43861:
	case 43862:
	case 60564:
	case 60571:
	case 60572:
	case 37575:
		{
			sp->EffectSpellClassMask[0][0] = 0x00100000	|	0x10000000 | 0x80000000; //Earth + Flame + Frost Shocks
			sp->EffectSpellClassMask[0][1] = 0x08000000;	// Wind	Shock
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
		}break;
	case 60567:
		{
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
		}break;

		// Totem of	Third	Wind
	case 46098:
	case 34138:
	case 42370:
	case 43728:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x00000080;	// Lesser	Healing	Wave
		}break;

		// Totem of	the	Elemental	Plane
	case 60770:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x00000001;	// Lightning Bolt
		}break;

		// Fathom-Brooch of	the	Tidewalker
	case 37247:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->proc_interval	=	45000;
		}break;

		//Warlock: Backlash
	case 34935:
	case 34938:
	case 34939:
		{
			sp->proc_interval	=	8000;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_TARGET_SELF;
		}break;

	case 34936:
		{
			sp->AuraInterruptFlags |=	AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		//Priest - Holy	Nova
	case 15237:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	23455;
		}break;
	case 15430:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	23458;
		}break;
	case 15431:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	23459;
		}break;
	case 27799:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	27803;
		}break;
	case 27800:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	27804;
		}break;
	case 27801:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	27805;
		}break;
	case 25331:
			{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	25329;
			}break;
	case 48077:
			{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	48075;
			}break;
	case 48078:
		{
			sp->Effect[1]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	48076;
		}break;

		// Moroes' garrote targets a single	enemy	instead	of us
	case 37066:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		}break;

		// Penance
	case 47540:
	case 53005:
	case 53006:
	case 53007:
		{
			sp->DurationIndex = 566; // Change to instant cast as script will cast the real channeled spell.
			sp->ChannelInterruptFlags = 0; // Remove channeling behaviour.
		}break;

		// Penance triggered healing spells have wrong targets.
	case 47750:
	case 52983:
	case 52984:
	case 52985:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_FRIEND;
		}break;

		// Hymn of Hope
	case 60931:
	case 60933:
		{
			sp->EffectImplicitTargetA[0] = 37;
		}break;

		//paladin	-	Reckoning
	case 20177:
	case 20179:
	case 20180:
	case 20181:
	case 20182:
		{
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		//paladin	-	Reckoning	Effect = proc	on proc
	case 20178:
		{
			sp->procChance = 100;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		//paladin	-	Judgement	of Wisdom
	case 20186:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 20268;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->procChance = 35;
			sp->Effect[1] = 0;
			sp->EffectApplyAuraName[1] = 0;
		}break;

		//paladin	-	Judgement	of Light
	case 20185:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 20267;
			sp->procChance = 35;
			sp->Effect[1] = 0;
			sp->EffectApplyAuraName[1] = 0;
		}break;

	case 20267:
	case 20268:
	case 20341:
	case 20342:
	case 20343:
	case 27163:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
		}break;
		//paladin	-	Eye	for	an Eye
	case 9799:
	case 25988:
		{
			sp->procFlags	=	PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_CRIT_HIT_VICTIM;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	25997;
		}break;

		//paladin Blessed Life
	case 31828:
	case 31829:
	case 31830:
		{
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
			sp->EffectTriggerSpell[0]	=	31828;
		}break;

		//paladin	-	Light's	Grace
	case 31833:
	case 31835:
	case 31836:
		{
			sp->EffectSpellClassMask[0][0]=0x80000000;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		//Paladin - Sacred Cleansing
	case 53551:
	case 53552:
	case 53553:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x00001000;
			sp->EffectSpellClassMask[0][1] = 0x00001000;
			sp->EffectSpellClassMask[0][2] = 0x00001000;
		}break;
		//Shield of the Templar
	case 53709:
	case 53710:
	case 53711:
		{
			sp->EffectSpellClassMask[0][0] = 64;
			sp->EffectSpellClassMask[0][1] = 16384;
			sp->EffectSpellClassMask[0][2] = 1048576;
			sp->EffectSpellClassMask[1][0] = 64;
			sp->EffectSpellClassMask[1][1] = 16384;
			sp->EffectSpellClassMask[1][2] = 1048576;
			sp->EffectSpellClassMask[2][0] = 64;
			sp->EffectSpellClassMask[2][1] = 16384;
			sp->EffectSpellClassMask[2][2] = 1048576;
		}break;
		//Paladin - Infusion of light
	case 53569:
	case 53576:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 53672:
	case 54149:
		{
			sp->AuraInterruptFlags	=	AURA_INTERRUPT_ON_CAST_SPELL;
		}break;
		//Paladin - Vindication
	case 9452:
	case 26016:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance	=	25;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA; // hoax
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->maxstack = 1;
		}break;
		//Paladin - Art of War
	case 53486:
		{
			sp->procFlags	=	PROC_ON_CRIT_ATTACK;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
			sp->EffectApplyAuraName[2]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2]	=	53489;
		}break;
	case 53488:
		{
			sp->procFlags	=	PROC_ON_CRIT_ATTACK;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
			sp->EffectApplyAuraName[2]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2]	=	59578;
		}break;
	case 53489:
	case 59578:
		{
				 sp->AuraInterruptFlags	|= AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		//shaman - Lightning Overload
	case 30675:	// rank	1
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 4;
		}break;

	case 30678:	 //	rank 2
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 8;
		}break;

	case 30679:	// rank	3
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 12;
		}break;

	case 30680:	// rank	4
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 16;
		}break;

	case 30681:	// rank	5
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	39805;//proc something (we will	owerride this)
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}break;

		//shaman - Purge
	case 370:
	case 8012:
	case 27626:
	case 33625:
		{
			sp->DispelType = DISPEL_MAGIC;
		}break;

	case 57663: // Totem of Wrath
		{
			sp->AreaAuraTarget = AA_TARGET_RAID;
		}break;

		//Shaman - Eye of	the	Storm
	case 29062:
	case 29064:
	case 29065:
		{
			sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
		}break;

		//shaman - Elemental Devastation
	case 29179:
	case 29180:
	case 30160:
		//shaman - Ancestral healing
	case 16176:
	case 16235:
	case 16240:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

		//shaman - Ancestral healing proc	spell
	case 16177:
	case 16236:
	case 16237:
		{
			sp->rangeIndex = 4;
		}break;

		//rogue - overkill stealth break
	case 58427:
		{
			sp->AttributesEx |= ATTRIBUTESEX_NOT_BREAK_STEALTH;
		}break;

		//rogue	-	Find Weakness.
	case 31233:
	case 31239:
	case 31240:
	case 31241:
	case 31242:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		//rogue	-	Cheap	Shot
	case 1833:
		{
			sp->Flags3 &=	~FLAGS3_REQ_BEHIND_TARGET;	//Doesn't	require	to be	behind target
		}break;

		//rogue	-	Camouflage.
	case 13975:
	case 14062:
	case 14063:
	case 14064:
	case 14065:
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}break;

		//rogue	-	Mace Specialization.
	case 13709:
	case 13800:
	case 13801:
	case 13802:
	case 13803:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;

		//rogue	- Shadowstep
	case 36563:
		{
			sp->EffectMiscValue[1] = SMT_MISC_EFFECT;
			sp->procFlags	=	0;
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

	case 44373:
		{
			sp->procFlags	=	0;
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		//Rogue  - Ruthlessness
	case 14156:
	case 14160:
	case 14161:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;//proc	spell
			sp->EffectTriggerSpell[0] = 14157;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procflags2 = PROC_TARGET_SELF;
		}break;

		//priest	-	Holy Concentration
	case 34753:
	case 34859:
	case 34860:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 34754://Make it similar to Mage Clearcasting
		{
			sp->procCharges	=	2;
			sp->Flags3	=	4;
			sp->Flags4	=	1073741824;
			sp->procFlags	=	87376;
		}break;

		//guardian spirit
	case 47788:
		{
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
			sp->AuraInterruptFlags = 0;
		}break;

	case 48153:
		{
			sp->EffectBasePoints[0] = 50;
		}break;

		//Priest:	Blessed	Recovery
	case 27811:
		{
			sp->EffectTriggerSpell[0]	=	27813;
			sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
		}break;

	case 27815:
		{
			sp->EffectTriggerSpell[0]	=	27817;
			sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
		}break;

	case 27816:
	{
			sp->EffectTriggerSpell[0]	=	27818;
			sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
		}break;

		//priest-	Blessed	Resilience
	case 33142:
	case 33145:
	case 33146:
		//priest-	Focused	Will
	case 45234:
	case 45243:
	case 45244:
		{
			sp->procFlags	=	PROC_ON_CRIT_HIT_VICTIM;
		}break;
		//Priest:	Improved Vampiric Embrace (Rank1)
	case 27839:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		}break;
		//Paladin: Seal	of Wisdom
	case 20166:
		{
			sp->Effect[0] = sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
		}break;

		//paladin	-	seal of	blood
	case 31892:
	case 38008:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	31893;
		}break;
	case 53720:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 53719;
		}break;

	case 21084: // Seal of Righteousness
		{
			sp->School = SCHOOL_HOLY;
			sp->procFlags = PROC_ON_MELEE_ATTACK; // Overwrite the other procflag
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
			sp->Effect[0] = sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
		}break;

		// Seal of Blood/Martyr holy dmg
	case 31893:
	case 53719:
		{
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;

		//paladin - seal of command
	case 20375:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->ProcsPerMinute = 7;
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;
	case 57770:
		{
			sp->rangeIndex = 1;
		}
		//paladin - hammer of the righteous
	case 53595:
		{
			sp->speed = 0;
			sp->is_melee_spell = true;
		}break;

		//paladin	-	Spiritual	Attunement
	case 31785:
	case 33776:
		{
			sp->procFlags	=	PROC_ON_SPELL_LAND_VICTIM;
			sp->procflags2	=	PROC_TARGET_SELF;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	31786;
		}break;

		//fix	for	the	right	Enchant	ID for Enchant Cloak - Major Resistance
	case 27962:
	case 36285:
			{
			sp->EffectMiscValue[0] = 2998;
			}break;

		//muhaha,	rewriting	Retaliation	spell	as old one :D
	case 20230:
			{
			sp->Effect[0]	=	6; //aura
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	22858; //evil	,	but	this is	good for us	:D
			sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM; //add procflag	here since this	was	not	processed	with the others	!
			}break;

		//"bloodthirst"	new	version	is ok	but	old	version	is wrong from	now	on :(
	case 23881:
			{
			sp->Effect[1]	=	64;	//cast on	us,	it is	good
			sp->EffectTriggerSpell[1]	=	23885; //evil	,	but	this is	good for us	:D
			}break;

	case 23892:
		{
			sp->Effect[1]	=	64;
			sp->EffectTriggerSpell[1]	=	23886; //evil	,	but	this is	good for us	:D
			}break;

	case 23893:
		{
			sp->Effect[1]	=	64;	//
			sp->EffectTriggerSpell[1]	=	23887; //evil	,	but	this is	good for us	:D
			}break;

	case 23894:
		{
			sp->Effect[1]	=	64;	//
			sp->EffectTriggerSpell[1]	=	23888; //evil	,	but	this is	good for us	:D
			}break;

	case 25251:
		{
			sp->Effect[1]	=	64;	//aura
			sp->EffectTriggerSpell[1]	=	25252; //evil	,	but	this is	good for us	:D
			}break;

	case 30335:
		{
			sp->Effect[1]	=	64;	//aura
			sp->EffectTriggerSpell[1]	=	30339; //evil	,	but	this is	good for us	:D
			}break;

		// Hunter	-	Master Tactician
	case 34506:
	case 34507:
	case 34508:
	case 34838:
	case 34839:
			{
			sp->procFlags	=	PROC_ON_RANGED_ATTACK;
			sp->procflags2	=	PROC_TARGET_SELF;
			}break;

		// Hunter	-	T.N.T
	case 56333:
	case 56336:
	case 56337:
		{
			sp->procFlags = PROC_ON_TRAP_TRIGGER | PROC_ON_CAST_SPECIFIC_SPELL | PROC_ON_CRIT_ATTACK | PROC_ON_PHYSICAL_ATTACK | PROC_ON_RANGED_ATTACK;
			sp->EffectSpellClassMask[0][0] = 0x00000004;
			sp->EffectSpellClassMask[0][1] = sp->EffectSpellClassMask[1][1];
			sp->EffectSpellClassMask[1][0] = 0x0;
		}break;

		// Hunter	-	Spirit Bond
	case 19578:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	19579;
			}break;

	case 20895:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	24529;
			}break;

	case 19579:
	case 24529:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA; //we	should do	the	same for player	too	as we	did	for	pet
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectBasePoints[1]	=	sp->EffectBasePoints[0];
			sp->EffectAmplitude[1] = sp->EffectAmplitude[0];
			sp->EffectDieSides[1]	=	sp->EffectDieSides[0];
			}break;

		// Hunter	-	Animal Handler
	case 34453:
	case 34454:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HIT_CHANCE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			}break;

		// Hunter	-	Catlike	Reflexes
	case 34462:
	case 34464:
	case 34465:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[1] = sp->EffectApplyAuraName[0];
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			}break;

		// Hunter	-	Serpent's	Swiftness
	case 34466:
	case 34467:
	case 34468:
	case 34469:
	case 34470:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HASTE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}break;

		// Hunter	-	Ferocious	Inspiration
	case 34455:
	case 34459:
	case 34460:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectTriggerSpell[0]	=	34456;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->Effect[1]	=	0; //remove	this
		}break;

		// Hunter	-	Focused	Fire
	case 35029:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	35060;
		}break;
	case 35030:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	35061;
		}break;
	case 35060:
	case 35061:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}break;

		// Hunter	-	Thick	Hide
	case 19609:
	case 19610:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_RESISTANCE;	//we do	not	support	armor	rating for pets	yet	!
			sp->EffectBasePoints[0]	*= 10; //just	give it	a	little juice :P
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		// Hunter	-	Ferocity
	case 19612:
	case 19599:
	case 19600:
	case 19601:
	case 19602:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_CRIT_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		// Hunter	-	Bestial	Swiftness
	case 19596:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_SPEED;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		// Hunter	-	Endurance	Training
	case 19583:
	case 19584:
	case 19585:
	case 19586:
	case 19587:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;
		// Hunter - Thrill of the Hunt
	case 34497:
	case 34498:
	case 34499:
		{
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 34720;
		}break;

		// Hunter	-	Expose Weakness
	case 34500:
	case 34502:
	case 34503:
		{
			sp->procFlags	=	PROC_ON_RANGED_CRIT_ATTACK;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;
		// hunter - Wild Quiver
	case 53215:
	case 53216:
	case 53217:
		{
			sp->procFlags = PROC_ON_RANGED_ATTACK;
		}break;

		//Hunter - Frenzy
	case 19621:
	case 19622:
	case 19623:
	case 19624:
	case 19625:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	19615;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->procChance = sp->EffectBasePoints[0];
			sp->procFlags	=	PROC_ON_CRIT_ATTACK;
			sp->c_is_flags	=	SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}break;

	case 56641:
	case 34120:
	case 49051:
	case 49052: //Steady Shot cast time fix
		{
			sp->CastingTimeIndex = 5; // Set 2 sec cast time
		}break;

	case 61846:
	case 61847: // Aspect of the Dragonhawk
		{	// need to copy Mod Dodge Percent aura from a separate spell
			CopyEffect(dbcSpell.LookupEntryForced(61848), 0, sp, 2);
		}break;

		//Hunter - Unleashed Fury
	case 19616:
	case 19617:
	case 19618:
	case 19619:
	case 19620:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectMiscValue[0] = 1;	//tweekign melee dmg
			sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET | SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
		}break;

		//Hunter : Pathfinding
	case 19559:
	case 19560:
		{
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}break;

		//Hunter : Rapid Killing - might need	to add honor trigger too here. I'm guessing	you	receive	Xp too so	i'm	avoiding double	proc
	case 34948:
	case 34949:
		{
			sp->procFlags	=	PROC_ON_GAIN_EXPIERIENCE;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		// Winter's	chill
	case 12579:
		{
			sp->c_is_flags |=	SPELL_FLAG_CANNOT_PROC_ON_SELF;
		}break;

		//Mage - Icy Veins
	case 12472:
		{
			sp->EffectMiscValue[1] = SMT_PROC_CHANCE;
		}break;

		//Mage - Wand	Specialization.	Not	the	forst	thing	we messed	up.	Blizz	uses attack	as magic and wandds	as weapons :S
	case 6057:
	case 6085:
		//Priest - Wand	Specialization
	case 14524:
	case 14525:
	case 14526:
	case 14527:
	case 14528:

		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
		}break;

		//Mage - Elemental Precision
	case 29438:
	case 29439:
	case 29440:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_COST;
		}break;

		//Mage - Arcane	Concentration
	case 11213:
	case 12574:
	case 12575:
	case 12576:
	case 12577:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		//Mage - ClearCasting	Effect
		//Druid - Clearcasting Effect
		//Druid - Nature's Grace effect
	case 16870:
	case 12536:
	case 16886:
		{
			sp->procCharges	=	2;
		}break;

		//Mage - Arcane	Blast
	case 36032:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
			sp->procflags2 = PROC_TARGET_SELF;
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;
		}break;

		//rogue - Prey on the weak
	case 51685:
	case 51686:
	case 51687:
	case 51688:
	case 51689:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;

		//rogue	-	Seal Fate
	case 14186:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 20;
		}break;
	case 14190:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 40;
		}break;
	case 14193:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 60;
		}break;
	case 14194:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 80;
		}break;
	case 14195:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}break;

		// druid - Nature's	Grace
	case 16880:
	case 61345:
	case 61346:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

		//druid	-	Blood	Frenzy
	case 16954:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}break;
	case 16952:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 50;
		}break;

		//druid	-	Primal Fury
	case 16961:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 100;
		}break;
	case 16958:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procChance = 50;
		}break;

		//druid	-	Intensity
	case 17106:
	case 17107:
	case 17108:
		{
			sp->EffectApplyAuraName[1]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		
	// Druid - Nurturing Instinct
	case 33872:
		{
			sp->AdditionalAura = 47179;
		}break;
	case 33873:
		{
			sp->AdditionalAura = 47180;
		}break;

		//Improved Sprint
	case 13743:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 50;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}break;
	case 13875:
		{
			sp->procChance = 100;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
		}break;

		//Warlock - healthstones
	case 6201:
	case 6202:
	case 5699:
	case 11729:
	case 11730:
	case 27230:
	case 47871:
	case 47878:
		{
			sp->Reagent[1] = 0;
			sp->ReagentCount[1] = 0;
		}break;
		//warlock	-	 Seed	of Corruption
	case 27243:
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	27285;
			sp->procFlags	=	PROC_ON_SPELL_HIT_VICTIM;
			sp->procflags2	=	PROC_ON_DIE;
			sp->procChance = 100;
		}break;
		//warlock - soul link
	case 19028:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_SPLIT_DAMAGE;
			sp->EffectBasePoints[0] = 20;
			sp->EffectMiscValue[0] = 127;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;

			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = 79;
			sp->EffectBasePoints[1]	= 4;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
		}break;

		//warlock: Nightfall
	case 18094:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 17941;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 2;
			sp->EffectSpellClassMask[0][0] = 0x0;
		}break;
	case 18095:
	{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 17941;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 4;
			sp->EffectSpellClassMask[0][0] = 0x0;
		}break;

		//Warlock - Backdraft
	case 47258:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	54274;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->procCharges	= 4;
			sp->EffectSpellClassMask[0][0]=0x00800000;
			sp->EffectSpellClassMask[0][1]=0x00800000;
		}break;
	case 47269:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	54276;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->procCharges	= 4;
			sp->EffectSpellClassMask[0][0]=0x00800000;
			sp->EffectSpellClassMask[0][1]=0x00800000;
		}break;
	case 47260:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	54277;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->procCharges	= 4;
			sp->EffectSpellClassMask[0][0]=0x00800000;
			sp->EffectSpellClassMask[0][1]=0x00800000;
		}break;
		//Warlock - Eradication
	case 47195:
	case 47196:
	case 47197:
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
			sp->EffectSpellClassMask[0][0] = 2;
			sp->proc_interval = 30000;
		}break;

		//warlock: Improved	Enslave	Demon
	case 18821:
		{
			sp->EffectMiscValue[0]=SMT_SPELL_VALUE_PCT;
			sp->EffectBasePoints[0]	=	-(sp->EffectBasePoints[0]+2);
		}break;
		//Mage - Improved	Blizzard
	case 11185:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	12484;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;
	case 12487:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	12485;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;
	case 12488:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	12486;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		//mage - Combustion
	case 11129:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
			sp->procCharges	=	0;
			sp->c_is_flags |=	SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}break;

		//mage - Master	of Elements
	case 29074:
	case 29075:
	case 29076:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 29077;
			sp->EffectImplicitTargetA[1] = 1;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 100;
		}break;

		//mage:	Blazing	Speed
	case 31641:
	case 31642:
			{
			sp->EffectTriggerSpell[0]	=	31643;
		}break;

		//mage talent	"frostbyte". we	make it	to be	dummy
	case 11071:
	case 12496:
	case 12497:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 12494;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = sp->EffectBasePoints[0];
		}break;

		//rogue-shiv ->	add	1	combo	point
	case 5938:
		{
			sp->Effect[1]=80;
			sp->Flags3 &=	~FLAGS3_REQ_BEHIND_TARGET;	//Doesn't	require	to be	behind target
			sp->is_melee_spell = true;
		}break;

		//warlock	-	Demonic	Sacrifice
	case 18789:
	case 18790:
	case 18791:
	case 18792:
	case 35701:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_ON_PET;
		}break;


		//warlock	-	Demonic	Tactics
	case 30242:
		{
			sp->Effect[0]	=	0; //disble	this.	This is	just blizz crap. Pure	proove that	they suck	:P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectApplyAuraName[2] = SPELL_AURA_MOD_SPELL_CRIT_CHANCE; //lvl 1 has it	fucked up	:O
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}break;
	case 30245:
	case 30246:
	case 30247:
	case 30248:
		{
			sp->Effect[0]	=	0; //disble	this.	This is	just blizz crap. Pure	proove that	they suck	:P
			sp->EffectImplicitTargetB[1] = EFF_TARGET_PET;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_PET;
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}break;

		//warlock	-	Demonic	Resilience
	case 30319:
	case 30320:
	case 30321:
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER ;
		}break;

		//warlock	-	Improved Imp
	case 18694:
	case 18695:
	case 18696:
	case 18705:
	case 18706:
	case 18707:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		//warlock	-	Improved Succubus
	case 18754:
	case 18755:
	case 18756:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
		}break;

		//warlock	-	Fel	Intellect
	case 18731:
	case 18743:
	case 18744:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 3;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		//warlock	-	Fel	Stamina
	case 18748:
	case 18749:
	case 18750:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_PERCENT_STAT;
			sp->EffectMiscValue[0] = 2;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		//warlock	-	Unholy Power
	case 18769:
	case 18770:
	case 18771:
	case 18772:
	case 18773:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	;
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
			//this is	required since blizz uses	spells for melee attacks while we	use	fixed	functions
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PET;
			sp->EffectMiscValue[1] = SCHOOL_NORMAL;
			sp->EffectBasePoints[1]	=	sp->EffectBasePoints[0]	;
		}break;

		//warlock	-	Master Demonologist	-	25 spells	here
	case 23785:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	23784;
		}break;
	case 23822:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	23830;
		}break;
	case 23823:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	23831;
		}break;
	case 23824:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	23832;
		}break;
	case 23825:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET	|	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	35708;
		}break;
		//and	the	rest
	case 23784:
	case 23830:
	case 23831:
	case 23832:
	case 35708:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

	case 23759:
	case 23760:
	case 23761:
	case 23762:
	case 23826:
	case 23827:
	case 23828:
	case 23829:
	case 23833:
	case 23834:
	case 23835:
	case 23836:
	case 23837:
	case 23838:
	case 23839:
	case 23840:
	case 23841:
	case 23842:
	case 23843:
	case 23844:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
		}break;

	case 35702:
	case 35703:
	case 35704:
	case 35705:
	case 35706:
		{
			sp->c_is_flags |= SPELL_FLAG_IS_EXPIREING_WITH_PET;
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
		}break;
		//warlock	-	Improved Healthstone
	case 18692:
	case 18693:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_MISC_EFFECT;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
		}break;
		//warlock	-	Improved Drain Soul
	case 18213:
	case 18372:
		{
			sp->procFlags	=	PROC_ON_TARGET_DIE;
			sp->procflags2	=	PROC_TARGET_SELF;
			sp->procChance = 100;
			sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	18371;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->Effect[2]	=	0	;	//remove this	effect
		}break;

		//warlock	-	Shadow Embrace
	case 32385:
	case 32387:
	case 32392:
	case 32393:
	case 32394:
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_SPELL_LAND;
			sp->Effect[1] = 0;	//remove this	effect ? Maybe remove	the	other	one	:P xD
		}break;
		//warlock - soul leech
	case 30293:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procChance = 10;
			sp->procFlags = PROC_ON_SPELL_LAND;
		}break;
	case 30295:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procChance = 20;
			sp->procFlags = PROC_ON_SPELL_LAND;
		}break;
	case 30296:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 30294;
			sp->procChance = 30;
			sp->procFlags = PROC_ON_SPELL_LAND;
		}break;
		//warlock	-	Pyroclasm
	case 18073:
	case 18096:
	case 63245:
		{
			sp->Effect[0]	=	0; //delete	this owerride	effect :P
			sp->EffectTriggerSpell[1]	=	18093; //trigger spell was wrong :P
			sp->procFlags	=	PROC_ON_ANY_HOSTILE_ACTION;
			sp->procChance = 26; //god,	save us	from fixed values	!
		}break;

		//Mage - Improved Scorch
	case 11095:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 33;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case 12872:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =66;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;
	case 12873:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance =100;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		//Nature's Grasp
	case 16689:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	339;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;
	case 16810:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	1062;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;
	case 16811:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	5195;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;
	case 16812:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	5196;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;
	case 16813:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	9852;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;
	case 17329:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	9853;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;
	case 27009:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	26989;
			sp->Effect[1]	=	0;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_REMOVEONUSE;
			sp->AuraInterruptFlags = 0;	//we remove	it on	proc or	timeout
			sp->procChance = 100;
		}break;

		//wrath	of air totem targets sorounding	creatures	instead	of us
	case 2895:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetA[2] = 0;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
			sp->EffectImplicitTargetB[2] = 0;
		}break;
		//Druid: Natural Shapeshifter
	case 16833:
	case 16834:
	case 16835:
		{
			sp->DurationIndex	=	0;
		}break;


		//Priest - Inspiration proc	spell
	case 14893:
	case 15357:
	case 15359:
		{
			sp->rangeIndex = 4;
		}break;

		//Relentless Strikes
	case 14179:
	case 58422:
	case 58423:
	case 58424:
	case 58425:
		{
			sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;//proc	spell
			sp->EffectTriggerSpell[0]	=	14181;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procflags2	=	PROC_TARGET_SELF;
			sp->procChance = 100;
		}break;

		//priest - surge of	light
	case 33150:
	case 33154:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

	case 33151:
		{
			sp->procCharges	=	2;
		}break;

		//Seal of	Justice	-lowered proc	chance (experimental values	!)
	case 20164:
	case 31895:
		{
			sp->procChance = 20;
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
		}break;
		// judgement of command shit
	case 20425:
		{
			sp->Effect[1] = 2;
			sp->EffectImplicitTargetA[1] = 6;
		}break;
		//Berserking
	case 26635:
		{
			sp->Attributes	=	327680;
			sp->CasterAuraState	=	0;
		}break;
		//Escape Artist
	case 20589:
		{
			sp->Attributes = 65552;
			sp->Flags3 = 0;
			sp->Effect[0] = 108;
			sp->Effect[1] = 108;
			sp->EffectDieSides[0] = 1;
			sp->EffectDieSides[1] = 1;
			sp->EffectBasePoints[0] = 9;
			sp->EffectBasePoints[1] = 9;
			sp->EffectMiscValue[0] = 7;
			sp->EffectMiscValue[1] = 11;
		}break;
		//Zapthrottle Mote Extractor
	case 30427:
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
		}break;
		//Goblin Jumper Cable
	case 22999:
	case 8342:
	case 54732:
		{
			sp->Effect[0] = 113;
			sp->EffectMiscValue[0] = 120;
		}break;
		//rogue	-	intiative
	case 13976:
	case 13979:
	case 13980:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		//this an	on equip item	spell(2824)	:	 ice arrow(29501)
	case 29501:
		{
			sp->procChance = 30;//some say it	is triggered every now and then
			sp->procFlags	=	PROC_ON_RANGED_ATTACK;
		}break;

		//deep wounds
	case 12834:
	case 12849:
	case 12867:
		{
			sp->EffectTriggerSpell[0] = 12868;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_RANGED_CRIT_ATTACK;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;

		//warrior	-	second wind	should trigger on	self
	case 29841:
	case 29842:
		{
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		// Improved Revenge
	case 12797:
	case 12799:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		//warrior	-	berserker	rage is	missing	1	effect = regenerate	rage
	case 18499:
		{
			sp->Effect[2]	=	6;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2]	=	37521; //not sure	if this	is the one.	In my	time this	adds 30	rage
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		//warrior	-	improved berserker rage
	case 20500:
	case 20501:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procflags2	=	PROC_TARGET_SELF;
		}break;

		// warrior - Spell Reflection
	case 23920:
		{
			sp->procFlags	=	PROC_NULL; //	No need	to proc
		}break;


		// warrior - Titan's Grip
	case 46917:	// main	spell
		{
			SpellEntry * sp2 = dbcSpell.LookupEntryForced(	49152	);	// move	this aura	into main	spell
				if(sp != NULL)
				{
				sp->Effect[1] = sp2->Effect[0];
				sp->EffectApplyAuraName[1] = sp2->EffectApplyAuraName[0];
				sp->EffectMiscValue[1] = sp2->EffectMiscValue[0];
				sp->EffectSpellClassMask[1][0] = sp2->EffectSpellClassMask[0][0];
				sp->EffectSpellClassMask[1][1] = sp2->EffectSpellClassMask[0][1];
				sp->EffectSpellClassMask[1][2] = sp2->EffectSpellClassMask[0][2];
				sp->EffectBasePoints[1] = sp2->EffectBasePoints[0];
				sp->EffectDieSides[1]= sp2->EffectDieSides[0];
				}
		}break;

		// Charge	-	Changing from	dummy	for	the	power	regen
	case 100:
	case 6178:
	case 11578:
		{
			sp->Effect[1]	=	SPELL_EFFECT_ENERGIZE;
			sp->EffectMiscValue[1] = 1;
		}break;

		//warrior	-	Rampage
	case 30030:
	case 30033:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->procflags2	=	PROC_TARGET_SELF;
			sp->EffectTriggerSpell[0]	=	sp->EffectTriggerSpell[1];
//			sp->Unique = true; // Crow: It should be, but what if we have 2 warriors?
		}break;

		// warrior - overpower
	case 7384:
	case 7887:
	case 11584:
	case 11585:
		{
			sp->Attributes |=	ATTRIBUTES_CANT_BE_DPB;
		}break;

		// warrior - heroic	fury
	case 60970:
		{
			sp->Effect[1]	=	SPELL_EFFECT_DUMMY;
		}break;

	case 20608:	//Reincarnation
			{
				for(uint32 i=0;i<8;++i)
				{
						if(sp->Reagent[i])
						{
						sp->Reagent[i] = 0;
						sp->ReagentCount[i]	=	0;
						}
				}
			}break;

		// druid - travel	form
	case 5419:
			{
			sp->Attributes |=	ATTRIBUTES_MOUNT_CASTABLE;
			}break;

		// druid - Naturalist
	case 17069:
	case 17070:
	case 17071:
	case 17072:
	case 17073:
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[1] = 1;
		}break;

		// Druid: Omen of Clarity
	case 16864:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_CAST_SPELL;
			sp->procChance = 6;
			sp->proc_interval = 10000;
		}break;

		//Serpent-Coil Braid
	case 37447:
	case 61062://Heroes' Frostfire Garb AND Valorous Frostfire Garb
		{
			sp->Effect[0] = 6;
			sp->Effect[1] = 6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1]	=	37445;
			sp->maxstack = 1;
		}break;

	case 31687:
		{
			sp->EffectMiscValue[0] = 510;
		}break;

		// Mark of Conquest
	case 33510:
		{
			sp->procChance = 7;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;

		//Paladin	-	Improved Lay on	Hands
	case 20234:
	case 20235:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		// Sudden	Death
	case 29724:
	case 29725:
	case 29723:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;

		// Taste for Blood
	case 56638:
	case 56637:
	case 56636:
		{
			sp->procFlags	=	PROC_ON_ANY_HOSTILE_ACTION;
			sp->EffectSpellClassMask[0][0]	= 0x0;
			sp->EffectSpellClassMask[1][0]	= 0x0;
		}break;

		// Flametongue weapon
	case 58792:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	58788;
		}break;
	case 58791:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	58787;
		}break;
	case 58784:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	58786;
		}break;
	case 16313:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	25488;
		}break;
	case 16312:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	16344;
		}break;
	case 16311:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	16343;
		}break;
	case 15569:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	10445;
		}break;
	case 15568:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	8029;
		}break;
	case 15567:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	8028;
		}break;
	case 10400:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	8026;
		}break;

		//windfury weapon
	case 33757:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->EffectTriggerSpell[0]	=	25504;
			sp->procChance = 20;
			sp->proc_interval	=	3000;
			sp->maxstack = 1;
			// Current proc system doesn't support common proc_interval for 2
			// procs making dual wielding WF overpowered. So it is disabled for now
			//sp->always_apply = true; 
			//so that	we can apply 2 wf	auras	while	dual-wielding
		}break;

		// dot heals
	case 38394:
		{
			sp->procFlags	=	1024;
				//sp->SpellGroupType = 6;
		}break;

	case 16972:
	case 16974:
	case 16975:
		{	//fix	for	Predatory	Strikes
			sp->RequiredShapeShift = (1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1))|(1<<(FORM_CAT-1));
		}break;

	case 20134:
		{
			sp->procChance = 50;
		}break;

	/* aspect	of the pack	-	change to	AA */
	case 13159:
		{
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AREA_AURA;
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
		}break;

	/* aspect	of the cheetah - add proc	flags	*/
	case 5118:
			{
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;;
		}break;


	case SPELL_RANGED_GENERAL:
	case SPELL_RANGED_THROW:
	case 26679:
	case 29436:
	case 37074:
	case 41182:
	case 41346:
		{
			if(	sp->RecoveryTime==0	&& sp->StartRecoveryTime ==	0	)
			sp->RecoveryTime = 1600;
		}break;

	case SPELL_RANGED_WAND:
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_RANGED;
				if(	sp->RecoveryTime==0	&& sp->StartRecoveryTime ==	0	)
				sp->RecoveryTime = 1600;
		}break;

	/**********************************************************
	*	Misc stuff (NPC SPELLS)
	**********************************************************/

	case 5106:
		{
			sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_WEAPON_UNSHEATH;
		}break;

	/**********************************************************
	*	Misc stuff (QUEST)
	**********************************************************/
		// Bat Net
	case 52151:
		{
			sp->EffectImplicitTargetA[0] = 6;
		}break;

		// queststuff
	case 30877:
		{
			sp->EffectImplicitTargetB[0]=0;
		}break;

		// quest stuff
	case 23179:
		{
			sp->EffectMiscValue[0] = 1434;
		}break;

		// DK RUNEFORGING
	case 53341:
	case 53343:
		{
			sp->Effect[1] = SPELL_EFFECT_QUEST_COMPLETE;
			sp->EffectMiscValue[1] = 12842;
		}

		// arcane charges
	case 45072:
		{
			sp->Attributes |= ATTRIBUTES_MOUNT_CASTABLE;
		}break;

	case 48252: //Prototype Neural Needle
	case 45634: //Neural Needle
		{
			sp->Effect[1] = 0;
		}break;
		//Tuber whistle
	case 36652:
		{
			sp->Effect[1] = 3;
		}break;
		//Cure Caribou Hide
	case 46387:
		{
			sp->ReagentCount[0] = 1;
			sp->ReagentCount[1] = 0;
			sp->Reagent[1] = 0;
		}break;
		//Create Totem of Issliruk
	case 46816:
		{
			sp->Reagent[0] = 0;
			sp->ReagentCount[0] = 0;
		}break;
	/**********************************************************
	*	Misc stuff (ITEMS)
	**********************************************************/

		// Nitro Boosts
	case 55004:
		{
			CopyEffect(dbcSpell.LookupEntryForced(54861), 0, sp, 2);
			sp->DurationIndex = 39;
		}break;

		// Gnomish Lightning Generator
	case 55039:
		{
			sp->InterruptFlags = 0;
			sp->AuraInterruptFlags = 0;
			sp->ChannelInterruptFlags = 0;
		}break;

		// Libram of Divinity
	case 28853:
		// Libram of Light
	case 28851:
		// Blessed Book of Nagrand
	case 32403:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_FLAT_MODIFIER;
			sp->EffectMiscValue[0] = 0;
			sp->EffectSpellClassMask[0][0] = 1073741824;
		}break;
		// Libram of Mending
	case 43741:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->EffectSpellClassMask[0][0] = 0x80000000;
			sp->EffectSpellClassMask[0][1] = 0x0;
			sp->EffectSpellClassMask[0][2] = 0x0;
			sp->EffectSpellClassMask[1][0] = 0x0;
			sp->EffectSpellClassMask[1][1] = 0x0;
			sp->EffectSpellClassMask[1][2] = 0x0;
			sp->EffectSpellClassMask[2][0] = 0x0;
			sp->EffectSpellClassMask[2][1] = 0x0;
			sp->EffectSpellClassMask[2][2] = 0x0;
		}break;

		// Druid Trinket Living	Root of	the	Wildheart
	case 37336:
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;// oh noez, we	make it	DUMMY, no	other	way	to fix it	atm
		}break;

		// Palla trinket
	case 43745:
	case 34258:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 8388608 | 520;
			sp->procChance = 100;
		}break;

		// Lesser Rune of Warding / Grater Rune of Warding
	case 42135:
	case 42136:
		{
			sp->EffectImplicitTargetA[1] = 6;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
		}break;

		//Idol of terror
	case 43737:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		// Lesser	Heroism	(Tenacious Earthstorm	Diamond)
	case 32844:
		{
			sp->procChance = 5;
			sp->procFlags	 = PROC_ON_MELEE_ATTACK;
		}break;

		// Darkmoon	Card:	Heroism
	case 23689:
		{
			sp->ProcsPerMinute = 2.5f;
			sp->procFlags	 = PROC_ON_MELEE_ATTACK;
		}break;

		// Darkmoon	Card:	Maelstrom
	case 23686:
		{
			sp->ProcsPerMinute = 2.5f;
			sp->procFlags	 = PROC_ON_MELEE_ATTACK;
		}break;

		// dragonspine trophy
	case 34774:
		{
			sp->ProcsPerMinute = 1.5f;
			sp->proc_interval	=	20000;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;

		// Romulo's	Poison Vial
	case 34586:
		{
			sp->ProcsPerMinute = 1.6f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;

		// madness of	the	betrayer
	case 40475:
		{
			sp->procChance = 50;
			sp->ProcsPerMinute = 1.0f;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;

		// Band	of the Eternal Defender
	case 35077:
		{
			sp->proc_interval	=	55000;
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
		}break;
		// Band	of the Eternal Champion
	case 35080:
		{
			sp->proc_interval	=	55000;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;
		// Band	of the Eternal Restorer
	case 35086:
		// Band	of the Eternal Sage
	case 35083:
		{
			sp->proc_interval	=	55000;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;
		// Sonic Booster
	case 54707:
		{
			sp->ProcsPerMinute = 1.0f;
			sp->proc_interval	=	60000;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}break;
	case 60301: // Meteorite Whetstone
	case 60317: // Signet of Edward the Odd
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;
		// Bandit's Insignia
	case 60442:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;
		// Fury of the Five Flights
	case 60313:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;
		// Grim Toll
	case 60436:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;
		// Illustration of the Dragon Soul
	case 60485:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		// Soul of the Dead
	case 60537:
		{
			sp->proc_interval	=	45000;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
		// Anvil of Titans
	case 62115:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;
		// Embrace of the Spider
	case 60490:
		// Flow of Knowledge
	case 62114:
		// Forge Ember
	case 60473:
		// Mystical Skyfire Diamond
	case 32837:
		// Sundial of the Exiled
	case 60063:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		// Majestic Dragon Figurine
	case 60524:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		// Mirror of Truth
	case 33648:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
		}break;

		// Vestige of Haldor
	case 60306:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;

		//Energized
	case 43750:
			{
			sp->procFlags = PROC_ON_CAST_SPELL;
			}break;


		//Spell	Haste	Trinket
	case 33297:
			{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procflags2	=	PROC_TARGET_SELF;
			}break;
	case 57345: // Darkmoon Card: Greatness
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 45000;
		}break;

	case 67702: // Death's Choice/Verdict
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->proc_interval = 45000;
		}break;

	case 67771: // Death's Choice/Verdict (heroic)
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->proc_interval = 45000;
		}break;

	case 72413: //Ashen Bands
		{
			sp->procChance = 10;
		}break;

		// Swordguard Embroidery
	case 55776:
		{
			sp->proc_interval	=	60000;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}break;
		// Chuchu's Tiny Box of Horrors
	case 61618:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}break;

	case 57351: // Darkmoon Card: Berserker!
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK | PROC_ON_ANY_DAMAGE_VICTIM;;	// when you strike, or are struck in combat
		}break;
	case 60196:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
		}break;
		// Darkmoon Card: Death
	case 57352:
		{
			sp->proc_interval	=	45000;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK | PROC_ON_CAST_SPELL;
		}break;
	case 60493: //Dying Curse
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->proc_interval = 45000;
		}break;

		// Ashtongue Talisman	of Shadows
	case 40478:
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
		}break;

		// Ashtongue Talisman	of Swiftness
	case 40485:
		// Ashtongue Talisman	of Valor
	case 40458:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;
		//Ashtongue Talisman of Zeal
	case 40470:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 50;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0] = 40472;
			sp->maxstack = 1;
		}break;

		// Memento of	Tyrande
	case 37655:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 10;
			sp->proc_interval	=	45000;
		}break;

		// Ashtongue Talisman	of Insight
	case 40482:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
			}break;


		//Ashtongue	Talisman of	Equilibrium
	case 40442:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 40;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0]	=	40452;
			sp->maxstack = 1;
			sp->Effect[1]	=	6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 25;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1]	=	40445;
			sp->maxstack = 1;
			sp->Effect[2]	=	6;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 25;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[2]	=	40446;
			sp->maxstack = 1;
		}break;

		//Ashtongue	Talisman of	Acumen
	case 40438:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 10;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0]	=	40441;
			sp->maxstack = 1;
			sp->Effect[1]	=	6;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 10;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[1]	=	40440;
			sp->maxstack = 1;
		}break;

		//Ashtongue	Talisman of	Lethality
	case 40460:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 20;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectTriggerSpell[0]	=	40461;
			sp->maxstack = 1;
		}break;
		//Leatherworking Drums
	case 35475://Drums of War
	case 35476://Drums of Battle
	case 35478://Drums of Restoration
	case 35477://Drums of Speed
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_ALL_PARTY;
			sp->EffectImplicitTargetA[2] = EFF_TARGET_NONE;
			sp->EffectImplicitTargetB[0] = EFF_TARGET_NONE;
			sp->EffectImplicitTargetB[1] = EFF_TARGET_NONE;
			sp->EffectImplicitTargetB[2] = EFF_TARGET_NONE;
		}break;

	case 46699:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_REQUIRE_NO_AMMO;
		}break;
	/**********************************************************
	*	Misc stuff (ITEM SETS)
	**********************************************************/

		//Item Set:	Malorne	Harness
	case 37306:
	case 37311:
		//Item Set:	Deathmantle
	case 37170:
		{
			sp->procChance = 4;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;

		//Item Set:	Netherblade
	case 37168:
		{
			sp->procChance = 15;
		}break;

		//Item Set:	Tirisfal Regalia
	case 37443:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

		//Item Set:	Avatar Regalia
	case 37600:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 6;
		}break;

		//Item Set:	Incarnate	Raiment
	case 37568:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		//Item Set:	Voidheart	Raiment
	case 37377:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->proc_interval	=	20;
			sp->EffectTriggerSpell[0]	=	37379;
		}break;
	case 39437:
		{
			sp->Effect[0]	=	6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 5;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->proc_interval	=	20;
			sp->EffectTriggerSpell[0]	=	37378;
		}break;

		//Item Set:	Cataclysm	Raiment
	case 37227:
		{
			sp->proc_interval	=	60000;
			sp->procChance = 100;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

		//Item Set:	Cataclysm	Regalia
	case 37228:
		{
			sp->procChance = 7;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;
	case 37237:
		{
			sp->procChance = 25;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

		//Item Set:	Cataclysm	Harness
	case 37239:
		{
			sp->procChance = 2;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
		}break;

		//Item Set:	Cyclone	Regalia
	case 37213:
		{
			sp->procChance = 11;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

		//Item Set:	Lightbringer Battlegear
	case 38427:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->procChance = 20;
		}break;

		//Item Set:	Crystalforge Battlegear
	case 37195:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 6;
		}break;

		//Item Set:	Crystalforge Raiment
	case 37189:
		{
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
			sp->proc_interval	=	60000;
		}break;

	case 37188:
		//Item Set:	Crystalforge Armor
	case 37191:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
		}break;

		//Item Set:	Destroyer	Armor
	case 37525:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procChance = 7;
		}break;

		//Item Set:	Destroyer	Battlegear
	case 37528:
		//Item Set:	Warbringer Armor
	case 37516:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}break;

	/**********************************************************
	*	Misc stuff (GLYPH)
	**********************************************************/

	case 55680:// Glyph of Prayer of Healing
	case 56176:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]   =   56161;
			sp->procFlags   =   PROC_ON_CAST_SPELL;
		}break;
	case 58631: // Glyph of Icy Touch
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		// Glyph of reneved life
	case 58059:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		// Glyph of Vigor
	case 56805:
		{
			CopyEffect(dbcSpell.LookupEntryForced(21975), 0, sp, 2);
		}break;

		// Glyph of Lesser Healing Wave
	case 55438:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;

		// Glyph of Frostbolt
	case 56370:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		// Glyph of Revenge
	case 58364:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0]=0x0;
			sp->EffectSpellClassMask[0][1]=0x0;
		}break;
		// Glyph of Revenge Proc
	case 58363:
		{
			sp->AuraInterruptFlags  =   AURA_INTERRUPT_ON_CAST_SPELL;
		}break;
	case 56218://Glyph of Corruption
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	17941;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 4;
			sp->EffectSpellClassMask[0][0]=0x0;
		}break;

		//////////////////////////////////////////////////////
		// CLASS-SPECIFIC	SPELL	FIXES							//
		//////////////////////////////////////////////////////

	/* Note: when	applying spell hackfixes,	please follow	a	template */

		//////////////////////////////////////////
		// WARRIOR									//
		//////////////////////////////////////////
	case 7405: //	Sunder Armor
	case 7386: //	Sunder Armor
	case 8380: //	Sunder Armor
	case 11596:	// Sunder	Armor
	case 11597:	// Sunder	Armor
	case 25225:	// Sunder	Armor
	case 30330:	// Mortal	Strike Ranks
	case 25248:	// Mortal	Strike Ranks
	case 21553:	// Mortal	Strike Ranks
	case 21552:	// Mortal	Strike Ranks
	case 21551:	// Mortal	Strike Ranks
	case 12294:	// Mortal	Strike Ranks
	case 16511:	// Hemo	Rank 1
	case 17347:	// Hemo	Rank 2
	case 17348:	// Hemo	Rank 3
	case 26864:	// Hemo	Rank 4
		{
			sp->Unique = true;
		}break;
		// Wrecking Crew
	case 46867:
	case 56611:
	case 56612:
	case 56613:
	case 56614:
		{
			sp->procFlags	=	 PROC_ON_CRIT_ATTACK;
		}break;
		// Enrage
	case 12317:
	case 13045:
	case 13046:
	case 13047:
	case 13048:
		{
			sp->procFlags	=	 PROC_ON_ANY_DAMAGE_VICTIM;
		}break;
		// Improved Defensive Stance (Missing Parry Flag)
	case 29593:
	case 29594:
		{
			sp->procflags2	=	 PROC_ON_BLOCK_VICTIM | PROC_ON_DODGE_VICTIM;
		}break;
		// Sword and Board
	case 46951:
	case 46952:
	case 46953:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
		// Sword and Board Refresh
	case 50227:
		{
			sp->Effect[1]	=	3;
		}break;
		// Sword specialization
	case 12281:
	case 13960:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 1;
		}break;
	case 12812:
	case 13961:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 2;
		}break;
	case 12813:
	case 13962:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 3;
		}break;
	case 12814:
	case 13963:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 4;
		}break;
	case 12815:
	case 13964:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->procChance = 5;
		}break;
		// vigilance
	case 50720:
		{
			sp->MaxTargets = 1;
			sp->Unique = true;
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_REDIRECT_THREAT;
			sp->EffectImplicitTargetA[2] = 57;
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
		}break;

		// Damage Shield
	case 58872:
	case 58874 :
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK_VICTIM;
			sp->procflags2 = PROC_ON_BLOCK_VICTIM;
			sp->procChance = 100;
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[2] = 1;
			sp->EffectTriggerSpell[2] = 59653;
		}break;

		// Improved Hamstring
	case 12289:
	case 12668:
	case 23695:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 2;
		}break;

		// Whirlwind
	case 1680:
		{
			sp->AllowBackAttack = true;
		}break;

		// Shockwave
	case 46968:
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MELEE;
		}break;

		// Blood Craze
	case 16487:
	case 16489:
	case 16492:
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT_VICTIM | PROC_ON_CRIT_HIT_VICTIM;
		}break;

		// Gag Order
	case 12311:
	case 12958:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		//////////////////////////////////////////
		// PALADIN									//
		//////////////////////////////////////////

		// Insert	paladin	spell	fixes	here
		// Light's Grace PROC
	case 31834:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		// Seal	of Command - Holy	damage,	but	melee	mechanics	(crit	damage,	chance,	etc)
	case 20424:
		{
			sp->rangeIndex = 4;
			sp->is_melee_spell = true;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
			sp->School = SCHOOL_HOLY;
		}break;

		// Illumination
	case 20210:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18350;
			sp->procChance = 20;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 20212:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18350;
			sp->procChance = 40;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 20213:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18350;
			sp->procChance = 60;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 20214:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18350;
			sp->procChance = 80;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 20215:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 18350;
			sp->procChance = 100;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;

		// Heart of the Crusader rank 1
	case 20335:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 21183;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 8388608;
		}break;

		// Heart of the Crusader rank 2
	case 20336:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54498;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 8388608;
		}break;

		// Heart of the Crusader rank 3
	case 20337:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54499;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 8388608;
		}break;

		// Pursuit of Justice rank 1
	case 26022:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[1] = 8;
		}break;

		// Pursuit of Justice rank 2
	case 26023:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_INCREASE_MOUNTED_SPEED;
			sp->EffectImplicitTargetA[1] = 1;
			sp->EffectBasePoints[1] = 15;
		}break;

		// Righteous Vengeance
	case 53380:
	case 53381:
	case 53382:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 61840;
			sp->procFlags = PROC_ON_CRIT_ATTACK;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->EffectSpellClassMask[0][0] = 0xE14BFF42;
		}break;

		// Sheat of Light (Hot Effect)
	case 53501:
	case 53502:
	case 53503:
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1]	=	54203;
			sp->procflags2	=	PROC_ON_SPELL_CRIT_HIT;
		}break;

	case 54203:
		{
			sp->logsId = sp->Id;
		}break;


		// Sacred Shield
	case 53601:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
			sp->procflags2	=	PROC_TARGET_SELF;
			sp->EffectTriggerSpell[0] = 58597;
			sp->proc_interval = 6000;
		}break;

		// SoC/SoV Dot's
	case 31803:
	case 53742:
		{
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
			sp->Unique = true;
		}break;

		// Judgement of righteousness
	case 20187:
		// Judgement of command
	case 20467:
		// Judgement of Justice/Wisdom/Light
	case 53733:
		// Judgement of Blood
	case 31898:
		// Judgement of Martyr
	case 53726:
		// Judgement of Corruption/Vengeance
	case 31804:
		{
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;

		// Shield of Righteousness
	case 53600:
	case 61411:
		{
			sp->School = SCHOOL_HOLY;
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;

		// Divine Storm
	case 53385:
		{
			sp->AllowBackAttack = true;
		}break;

		// Beacon of the Light (Dummy Aura)
	case 53563:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDBUFF;
		}break;

		//////////////////////////////////////////
		// HUNTER								//
		//////////////////////////////////////////

		//Hunter - Go for the Throat
	case 34950:
	case 34954:
		{
			sp->procFlags = PROC_ON_RANGED_CRIT_ATTACK;
		}break;
	case 34952:
	case 34953:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		//Ranged Weapon Specialization
	case 19507:
	case 19508:
	case 19509:
	case 19510:
	case 19511:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[0] = 1;
		}break;
		//Two Handed Weapon Specialization and One Handed Weapon Specializacion
		//Warrior and Paladin and Death Knight - this will change on 3.1.0
	case 20111:
	case 20112:
	case 20113:
	case 12163:
	case 12711:
	case 12712:
	case 16538:
	case 16539:
	case 16540:
	case 16541:
	case 16542:
	case 20196:
	case 20197:
	case 20198:
	case 20199:
	case 20200:
	case 55107:
	case 55108:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;
			sp->EffectMiscValue[0] = 2;
		}break;
		//Frost Trap
	case 13809:
		{
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		//Snakes from Snake Trap cast this, shouldn't stack
	case 25810:
	case 25809:
		{
			sp->maxstack = 1;
		}break;

	case 27065:
	case 20904:
	case 20903:
	case 20902:
	case 20901:
	case 20900:
	case 19434:
		{
			sp->Unique = true;
		}break;

		//Hunter : Entrapment
	case 19184:
	case 19387:
	case 19388:
		{
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
		}break;
		// Hunter - Arcane Shot  - Rank 6 to 11
	case 14285:
	case 14286:
	case 14287:
	case 27019:
	case 49044:
	case 49045:
		{
			sp->Effect[0] = 2;
			sp->Effect[1] = 0;
			sp->EffectBasePoints[0] = sp->EffectBasePoints[1];
			sp->EffectBasePoints[1] = 0;
			sp->EffectImplicitTargetA[0] = 6;
			sp->EffectImplicitTargetA[1] = 0;
		}break;

		// Misdirection
	case 34477:
		{
			sp->MaxTargets = 1;
			sp->Unique = true;
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_REDIRECT_THREAT;
		}break;

		// Misdirection proc
	case 35079:
		{
			sp->MaxTargets = 1;
			sp->Unique = true;
		}break;
		// Aspect of the Viper
	case 34074:
		{
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[2] = 34075;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
		}break;
		// Viper String
	case 3034:
		{
			sp->EffectMultipleValue[0] = 3;
		}break;
		// Improved Steady Shot
	case 53221:
	case 53222:
	case 53224:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x0;
			sp->EffectSpellClassMask[0][1] = 0x0;
		}break;


		// Lock and Load
	case 56342:
	case 56343:
	case 56344:
		{
			sp->procFlags = PROC_ON_TRAP_TRIGGER;
			sp->procChance = sp->EffectBasePoints[0] + 1;
		}break;

		// Lock and load proc
	case 56453:
		{
			sp->DurationIndex = 9;
		}break;

		// Master's Call
	case 53271:
		{
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54216;
			sp->Effect[1] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 54216;
		}break;

		// Haunting party
	case 53290:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 57669;
			sp->EffectImplicitTargetA[0] = 1;
			sp->procChance = 33;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 53291:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 57669;
			sp->EffectImplicitTargetA[0] = 1;
			sp->procChance = 66;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 53292:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 57669;
			sp->EffectImplicitTargetA[0] = 1;
			sp->procChance = 100;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;

		// Flare
	case 1543:
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF | SPELL_FLAG_IS_TARGETINGSTEALTHED;
		}break;


		//////////////////////////////////////////
		// ROGUE									//
		//////////////////////////////////////////

		// Cheat Death
	case 31228:
	case 31229:
	case 31230:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;

		// Cloack of shadows
		// Cloack of shadows PROC
	case 31224:
	case 35729:
		{
			sp->DispelType = DISPEL_MAGIC;
			sp->AttributesEx |= 1024;
		}break;

		// Honor Among Thieves PROC
	case 52916:
		{
			sp->proc_interval = 4000; //workaround
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_RANGED_CRIT_ATTACK;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectImplicitTargetA[1] = 38;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 51699;
		}break;

		// Shadow Dance
	case 51713:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;

		// Let's hack	the	entire cheating	death	effect to	damage perc	resist ;)
	case 45182:	// Cheating	Death	buff
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
			sp->EffectMiscValue[0] = uint32(-91);
		}break;

		// Wound Poison	Stuff
	case 27189:
	case 13224:
	case 13223:
	case 13222:
	case 13218:
		{
			sp->Unique = true;
		}break;

		// Killing Spree
	case 51690:
		{
			sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1]	=	0;
			sp->Effect[2]	=	0;
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;
		// Focused Attacks
	case 51634:
	case 51635:
	case 51636:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
		}break;

		// Setup
	case 13983:
		{
			sp->proc_interval = 1000;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->procflags2 = PROC_ON_FULL_RESIST;
			sp->procChance = 33;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;
	case 14070:
		{
			sp->proc_interval = 1000;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->procflags2 = PROC_ON_FULL_RESIST;
			sp->procChance = 66;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;
	case 14071:
		{
			sp->proc_interval = 1000;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->procflags2 = PROC_ON_FULL_RESIST;
			sp->procChance = 100;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		// Setup PROC
	case 15250:
		{
			sp->proc_interval = 1000;
		}break;

		// Mutilate
	case 1329:
	case 34411:
	case 34412:
	case 34413:
	case 48663:
	case 48666:
		{
			sp->Effect[1] = 0;
			sp->procChance = 0;
			sp->Flags3 &=   ~FLAGS3_REQ_BEHIND_TARGET;
		}break;

	case 35541:
	case 35550:
	case 35551:
	case 35552:
	case 35553:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->procChance	=	20;
		}break;

		//////////////////////////////////////////
		// PRIEST									//
		//////////////////////////////////////////

		// Dispersion (org spell)
	case 47585:
		{
			sp->AdditionalAura = 47218;
		}break;

		// Dispersion (remove im effects, in 3.1 there is a spell for this)
	case 47218:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MECHANIC_IMMUNITY;
			sp->EffectApplyAuraName[1] = SPELL_AURA_MECHANIC_IMMUNITY;
			sp->EffectMiscValue[0] = 7;
			sp->EffectMiscValue[1] = 11;
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectImplicitTargetA[1] = 1;
		}break;

		// Mass dispel
	case 32375:
	case 32592:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 100;
			sp->EffectImplicitTargetA[1] = 1;
		}break;

			// Power Infusion
	case 10060:
		{
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDBUFF;
		}break;

		// Prayer of mending (tricky one :() rank 1
	case 33076:
		{
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 41635;
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PARTY_MEMBER;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
		}break;

		// Prayer of mending (tricky one :() rank 2
	case 48112:
		{
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48110;
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PARTY_MEMBER;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
		}break;

		// Prayer of mending (tricky one :() rank 3
	case 48113:
		{
			sp->Effect[0] = SPELL_EFFECT_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48111;
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PARTY_MEMBER;
			sp->EffectImplicitTargetA[1] = EFF_TARGET_PARTY_MEMBER;
			sp->EffectImplicitTargetB[0] = 0;
			sp->EffectImplicitTargetB[1] = 0;
		}break;

		// triggered spell, rank 1
	case 41635:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 41637;
			sp->EffectBasePoints[0] = 800;
			sp->procCharges = 0;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		}break;

		// triggered spell, rank 2
	case 48110:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 41637;
			sp->EffectBasePoints[0] = 905;
			sp->procCharges = 0;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		}break;

		// triggered spell, rank 3
	case 48111:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 41637;
			sp->EffectBasePoints[0] = 1043;
			sp->procCharges = 0;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		}break;

		// triggered on	hit, this	is the spell that	does the healing/jump
	case 41637:
		{
			sp->Effect[0]	=	SPELL_EFFECT_DUMMY;
			sp->EffectBasePoints[0]	=	5;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
			sp->EffectImplicitTargetB[0] = 0;
		}break;

		// Inner Focus
	case 14751:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		// Divine Aegis
	case 47509:
	case 47511:
	case 47515:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = 21;
			sp->EffectTriggerSpell[0] = 47753;
		}break;

		// Insert	priest spell fixes here

		//////////////////////////////////////////
		// SHAMAN									//
		//////////////////////////////////////////
	case 51466:
	case 51470: //Elemental Oath
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_FLAT_MODIFIER;
			sp->EffectMiscValue[1] = SMT_LAST_EFFECT_BONUS;
			sp->EffectSpellClassMask[1][0] = 0;
			sp->EffectSpellClassMask[1][1] = 0x00004000; // Clearcasting
		}break;
	case 51562:
	case 51563:
	case 51564:
	case 51565:
	case 51566: // Tidal Waves
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x00000100;	// Chain heal
			sp->EffectSpellClassMask[0][1] = 0x00000000;
			sp->EffectSpellClassMask[0][2] = 0x00000010;	// Riptide
		}break;
	case 53390:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case 51940:
	case 51989:
	case 52004:
	case 52005:
	case 52007:
	case 52008: // Earthliving Weapon
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}break;
	case 51945:
	case 51990:
	case 51997:
	case 51998:
	case 51999:
	case 52000:
		{
			sp->logsId = sp->Id;
		}break;
	case 55198:	// Tidal Force
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
			sp->EffectApplyAuraName[0] = 0;
			sp->EffectTriggerSpell[0] = 55166;
		}break;
	case 55166:
		{
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case 51525:
	case 51526:
	case 51527:	// Static Shock
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = sp->Id;
			sp->EffectSpellClassMask[0][0] = 0;
		}break;
	case 16180:
	case 16196:
	case 16198: // Improved Water Shield
		{
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x000000C0; // Healing Wave and Lesser Healing Wave
			sp->EffectSpellClassMask[0][2] = 0x00000010; //Riptide
			sp->EffectTriggerSpell[0] = sp->Id;
		}break;
	case 16187:
	case 16205:
	case 16206:
	case 16207:
	case 16208: // Restorative Totems
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[0] = SMT_DAMAGE_DONE;
			sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
		}break;
	case 31616: // Nature's Guardian
		{
			sp->logsId = sp->Id;
			sp->spell_can_crit = false;
		}break;

		// Improved firenova totem
	case 16086:
	case 16544:
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		//////////////////////////////////////////
		// MAGE									//
		//////////////////////////////////////////

		// Insert	mage spell fixes here

		// Invisibility
	case 66:
		{
			sp->EffectTriggerSpell[1] = 32612;
			sp->EffectAmplitude[1]	=	3000;
		}break;

		// Invisibility part	2
	case 32612:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->Effect[2] = 0;
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL | AURA_INTERRUPT_ON_START_ATTACK | AURA_INTERRUPT_ON_HOSTILE_SPELL_INFLICTED;
		}break;

		//improved blink
	case 31569:
	case 31570:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		// Magic Absorption
	case 29441:
	case 29444:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 29442;
		}break;

		//Missile Barrage
	case 44404:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 4;
		}break;
	case 54486:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 8;
		}break;
	case 54488:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 12;
		}break;
	case 54489:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 16;
		}break;
	case 54490:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = 20;
		}break;
		//Fiery Payback
	case 44440:
	case 44441:
		{
			sp->procChance	=	100;
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_ANY_DAMAGE_VICTIM;
		}break;

		//Fingers of Frost
	case 44543:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance	=	7;
		}break;
	case 44545:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance	=	15;
		}break;

		//Conjure Refreshment table
	case 43985:
	case 58661:
		{
			sp->EffectImplicitTargetA[0]	=	EFF_TARGET_DYNAMIC_OBJECT;
		}break;

		// Ritual of Refreshment
	case 43987:
	case 58659:
		{
			sp->c_is_flags |= SPELL_FLAG_IS_REQUIRECOOLDOWNUPDATE;
		}break;

		// Living bomb
	case 44457:
	case 55359:
	case 55360:
		{
			sp->c_is_flags |= SPELL_FLAG_ON_ONLY_ONE_TARGET;
		}break;

		// Arcane Potency proc
	case 57529:
	case 57531:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

		// Burnout
	case 44449:
	case 44469:
	case 44470:
	case 44471:
	case 44472:
		{
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 44450;
			sp->EffectImplicitTargetA[1] = 1;
			sp->procChance = 100;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;

		//////////////////////////////////////////
		// WARLOCK									//
		//////////////////////////////////////////

		// Insert	warlock	spell	fixes	here
	// Demonic Knowledge
	case 35691:
	case 35692:
	case 35693:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = 100;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectImplicitTargetA[1] = 5;
			sp->EffectTriggerSpell[1] = 35696;
		}break;

		// Demonic Knowledge PROC
	case 35696:
		{
			sp->EffectImplicitTargetA[0] = 1;
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;

		// Demonic Pact AA
	case 48090:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AREA_AURA;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AREA_AURA;
			sp->AreaAuraTarget = AA_TARGET_RAID;
		}break;

		// Demonic Pact
	case 53646:
		{
			sp->procChance = 100;
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}break;
		// Unstable Affliction
	case 30108:
	case 30404:
	case 30405:
	case 47841:
	case 47843:
		{
			sp->procFlags = PROC_ON_DISPEL_AURA_VICTIM;
			sp->procChance = 100;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[1] = 6;
			sp->EffectTriggerSpell[1] = 31117;
		}break;

		// Death's Embrace
	case 47198:
	case 47199:
	case 47200:
		{
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
			sp->EffectImplicitTargetA[2] = 1;
		}break;

		// Everlasting affliction
	case 47201:
	case 47202:
	case 47203:
	case 47204:
	case 47205:
		{
			sp->procFlags = PROC_ON_SPELL_LAND;
		}break;

		//warlock - Molten Core
	case 47245:
	case 47246:
	case 47247:
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION;
		}break;

		// pandemic
	case 58435:
	case 58436:
	case 58437:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 58691;
			sp->EffectImplicitTargetA[0] = 6;
		}break;

		// Mana Feed
	case 30326:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 32554;
			sp->EffectImplicitTargetA[0] = EFF_TARGET_PET;
		}break;

		// Fel Synergy
	case 47230:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54181;
		}break;
	case 47231:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 54181;
		}break;

		//////////////////////////////////////////
		// DRUID									//
		//////////////////////////////////////////

		// Insert	druid	spell	fixes	here
	case 22570:
	case 49802:	// Maim
		{
			sp->AuraInterruptFlags |=	AURA_INTERRUPT_ON_WEAPON_UNSHEATH;
			sp->Attributes |=	ATTRIBUTES_STOP_ATTACK;
		}break;

		// RAVAGE
	case 6785:
		{
			sp->EffectBasePoints[0] = 162;				//r1
		}break;
	case 6787:
		{
			sp->EffectBasePoints[0] = 239;				//r2
		}break;
	case 9866:
		{
			sp->EffectBasePoints[0] = 300;				//r3
		}break;
	case 9867:
		{
			sp->EffectBasePoints[0] = 377;				//r4
		}break;
	case 27005:
		{
			sp->EffectBasePoints[0] = 566;				//r5
		}break;
	case 48578:
		{
			sp->EffectBasePoints[0] = 1405;				//r6
		}break;
	case 48579:
		{
			sp->EffectBasePoints[0] = 1770;				//r7
		}break;

		//SHRED
	case 5221:
		{
			sp->EffectBasePoints[0] = 54;				//r1
		}break;
	case 6800:
		{
			sp->EffectBasePoints[0] = 72;				//r2
		}break;
	case 8992:
		{
			sp->EffectBasePoints[0] = 99;				//r3
		}break;
	case 9829:
		{
			sp->EffectBasePoints[0] = 144;				//r4
		}break;
	case 9830:
		{
			sp->EffectBasePoints[0] = 180;				//r5
		}break;
	case 27001:
		{
			sp->EffectBasePoints[0] = 236;				//r6
		}break;
	case 27002:
		{
			sp->EffectBasePoints[0] = 405;				//r7
		}break;
	case 48571:
		{
			sp->EffectBasePoints[0] = 630;				//r8
		}break;
	case 48572:
		{
			sp->EffectBasePoints[0] = 742;				//r9
		}break;

		// Natural reaction
	case 57878:
	case 57880:
	case 57881:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->procFlags = 0;
			sp->procChance = 0;
			sp->EffectTriggerSpell[1] = 0;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		// Dash
	case 1850:
	case 9821:
	case 33357:
		{
			sp->RequiredShapeShift	=	1;
		}break;

		// Totem of Wrath
	case 30708:
		{
			sp->Effect[0] = 6;
			sp->EffectImplicitTargetA[0] = 22;
			sp->EffectImplicitTargetB[0] = 15;
			sp->EffectRadiusIndex[0] = 10;
			sp->AreaAuraTarget = AA_TARGET_ALLENEMIES;
		}break;

		// Mangle - Cat
	case 33876:
		{
			sp->EffectBasePoints[0] = 198; //rank 1
		}break;
	case 33982:
		{
			sp->EffectBasePoints[0] = 256; //rank 2
		}break;
	case 33983:
		{
			sp->EffectBasePoints[0] = 330; //rank 3
		}break;
	case 48565:
		{
			sp->EffectBasePoints[0] = 536; //rank 4
		}break;
	case 48566:
		{
			sp->EffectBasePoints[0] = 634; //rank 5
		}break;
		// Mangle - Bear
	case 33878:
		{
			sp->EffectBasePoints[0] = 86; //rank 1
		}break;
	case 33986:
		{
			sp->EffectBasePoints[0] = 120; //rank 2
		}break;
	case 33987:
		{
			sp->EffectBasePoints[0] = 155; //rank 3
		}break;
	case 48563:
		{
			sp->EffectBasePoints[0] = 251; //rank 4
		}break;
	case 48564:
		{
			sp->EffectBasePoints[0] = 299; //rank 5
		}break;

		//Druid - Master Shapeshifter
	case 48411:
	case 48412:
		{
			sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0]	=	SPELL_AURA_DUMMY;
			sp->Effect[1]	=	0;
			sp->Effect[2]	=	0;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance	=	100;
		}break;
	case 48418:
		{
			sp->RequiredShapeShift	=	(uint32)(1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1));
		}break;
	case 48420:
		{
			sp->RequiredShapeShift	=	(uint32)(1<<(FORM_CAT-1));
		}break;
	case 48421:
		{
			sp->RequiredShapeShift	=	(uint32)1 << (FORM_MOONKIN-1);
		}break;
	case 48422:
		{
			sp->RequiredShapeShift	=	(uint32)1 << (FORM_TREE-1);
		}break;
		//Owlkin Frenzy
	case 48389:
	case 48392:
	case 48393:
		{
			sp->procFlags	=	PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK | PROC_ON_RANGED_ATTACK;
			sp->EffectSpellClassMask[0][0]	=	0x0;
		}break;
		// Infected Wounds
	case 48483:
	case 48484:
	case 48485:
		{
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK;
		}break;
		// Swipe (cat) max targets, fixed in 3.1
	case 62078:
		{
			sp->MaxTargets = 10;
		}break;
		// faerie fire (feral dmg)
	case 16857:
		{
			sp->Effect[1] = SPELL_EFFECT_SCHOOL_DAMAGE;
			sp->EffectBasePoints[1] = 1;
			sp->EffectImplicitTargetA[1] = 6;
		}break;

		// King of the jungle dmg buff
	case 51185:
		{
			sp->DurationIndex = 1;
			sp->RequiredShapeShift = (uint32)(1<<(FORM_BEAR-1))|(1<<(FORM_DIREBEAR-1));
		}break;
	case 60200:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;

		// Eclipse
	case 48516:
	case 48521:
	case 48525:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48517;
			sp->EffectTriggerSpell[1] = 48518;
		}break;

		// Living Seed
	case 48496:
	case 48499:
	case 48500:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectImplicitTargetA[0] = 21;
			sp->EffectTriggerSpell[0] = 48503;
		}break;

		// Healing way
	case 29203:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = 108; //ADD PCT MOD
			sp->EffectMiscValue[0] = 0;
			sp->EffectSpellClassMask[0][0] = 64;
			sp->EffectSpellClassMask[0][1] = 0;
			sp->EffectSpellClassMask[0][2] = 0;
		}break;

	case 50334:
		{
			sp->AdditionalAura = 58923;
		}break;

	case 17002:
		{
			sp->AdditionalAura = 24867;
		}break;
	case 24866:
		{
			sp->AdditionalAura = 24864;
		}break;

	case 24867:
	case 24864:
		{
			sp->apply_on_shapeshift_change = true;
		}break;

	case 24905:
		{
			sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
		}break;

	case 33881:
	case 33882:
	case 33883:
		{
			sp->procFlags = PROC_ON_CRIT_HIT_VICTIM;
		}break;

	case 22842:
	case 22895:
	case 22896:
	case 26999:
		{
			sp->Effect[0] = 6;
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 22845;
		}break;

	case 16850:
		{
			sp->procChance = 3;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case 16923:
		{
			sp->procChance = 6;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case 16924:
		{
			sp->procChance = 9;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

	case 24932:
		{
			sp->Effect[1] = 0;
			sp->Effect[2] = 0; //removing strange effects.
			sp->AreaAuraTarget = AA_TARGET_PARTY;
		}break;

	case 34299:
		{
			sp->proc_interval = 6000;//6 secs
		}break;
	case 49376:
		{
			sp->Effect[1] = 41;
			sp->EffectImplicitTargetA[1] = 6;
		}break;

		//////////////////////////////////////////
		// DEATH KNIGHT							//
		//////////////////////////////////////////

		// Merciless Combat
	case 49024:
	case 49538:
		{
			sp->EffectMiscValue[0] = 7278;
		}break;

	case 46619:
		{
			sp->Effect[0] = SPELL_EFFECT_NULL;
		}break;

	//Desecration
	case 55666: 
	case 55667:
		{
			sp->proc_interval = 15000;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

	case 50397:
		{
			sp->NameHash = 0;
		}break;

		//////////////////////////////////////////
		// BOSSES								//
		//////////////////////////////////////////

		// Insert	boss spell fixes here

		// War Stomp
	case 20549:
		{
			sp->RequiredShapeShift = 0;
		}break;

		// Dark	Glare
	case 26029:
		{
			sp->cone_width = 15.0f;	// 15	degree cone
		}break;

		// Commendation	of Kael'thas
	case 45057:
		{
			sp->proc_interval	=	30000;
		}break;

		// Recently	Dropped	Flag
	case 42792:
		{
			sp->c_is_flags |=	SPELL_FLAG_IS_FORCEDDEBUFF;
		}break;

	case 43958:
		{
			sp->Effect[0]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->DurationIndex	=	6; //	10 minutes.
			sp->c_is_flags |=	SPELL_FLAG_IS_FORCEDDEBUFF;
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
			sp->EffectMiscValue[1] = SMT_RESIST_DISPEL;
			sp->EffectBasePoints[1]	=	90;
		}break;

		// Recently	Dropped	Flag
	case 43869:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
			sp->c_is_flags |=	SPELL_FLAG_IS_FORCEDDEBUFF;
		}break;

	case 48978:
	case 61216:
		{
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->EffectMiscValue[1] = 1;
		}break;

	case 49390:
	case 61221:
		{
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->EffectMiscValue[1] = 2;
		}break;

	case 49391:
	case 61222:
		{
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->EffectMiscValue[1] = 3;
		}break;

	case 49392:
		{
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->EffectMiscValue[1] = 4;
		}break;

	case 49393:
		{
			sp->Effect[1]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
			sp->EffectMiscValue[1] = 5;
		}break;

		// Furious Attacks
	case 46910:
		{
			sp->EffectTriggerSpell[0]	=	56112;
			sp->procChance = 50;
			sp->procFlags	|= PROC_ON_MELEE_ATTACK;
		}break;

	case 46911:
		{
			sp->EffectTriggerSpell[0]	=	56112;
			sp->procChance = 100;
			sp->procFlags	|= PROC_ON_MELEE_ATTACK;
		}break;

		// Rogue:	Hunger for Blood!
	case 51662:
		{
			sp->Effect[1]	=	SPELL_EFFECT_DUMMY;
		}break;

		// Mage: Focus Magic
	case 54646:
		{
			sp->c_is_flags = SPELL_FLAG_ON_ONLY_ONE_TARGET;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 100;
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[1] = 54648;
		}break;

		// Mage: Hot Streak
	case 44445:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48108;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 33;
			sp->EffectSpellClassMask[0][0] = 0x0;
		}break;
	case 44446:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48108;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 66;
			sp->EffectSpellClassMask[0][0] = 0x0;
		}break;
	case 44448:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0] = 48108;
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
			sp->procChance = 100;
			sp->EffectSpellClassMask[0][0] = 0x0;
		}break;

	case 1122:
		{
			sp->EffectBasePoints[0]	=	0;
		}break;

	case 1860:
	case 20719:
		{
			sp->Effect[0]	=	SPELL_EFFECT_DUMMY;
		}break;
		// Bloodsurge
	case 46913:
	case 46914:
	case 46915:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK | PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_CAST_SPELL;
		}break;
	case 46916:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;
		//Waylay
	case 51692:
	case 51696:
		{
			sp->procFlags	=	PROC_ON_CRIT_ATTACK;
		}break;
		// Cold	Blood
	case 14177:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;
		// priest	-	mind flay
	case 15407:
	case 17311:
	case 17312:
	case 17313:
	case 17314:
	case 18807:
	case 25387:
	case 48155:
	case 48156:
		{
			sp->EffectImplicitTargetA[0] = EFF_TARGET_SINGLE_ENEMY;
			sp->logsId = 58381;
		}break;
		// Death and Decay
	case 43265:
	case 49936:
	case 49937:
	case 49938:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
		}break;
		//Warlock	Chaos	bolt
	case 50796:
	case 59170:
	case 59171:
	case 59172:
		{
			sp->Attributes = ATTRIBUTES_IGNORE_INVULNERABILITY;
		}break;
		//Force	debuff's
		// Hypothermia
	case 41425:
		// Forbearance
	case 25771:
		// Weakened	Soul
	case 6788:
		{
			sp->c_is_flags = SPELL_FLAG_IS_FORCEDDEBUFF;
		}break;
		// Death Knight	spell	fixes
		//Blade	Barrier
	case 49182:
	case 49500:
	case 49501:
	case 55225:
	case 55226:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance = 100;
		}break;
		// Killing Machine
	case 51123:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->ProcsPerMinute = 1;
		}break;
	case 51127:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->ProcsPerMinute = 2;
		}break;
	case 51128:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->ProcsPerMinute = 3;
		}break;
	case 51129:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->ProcsPerMinute = 4;
		}break;
	case 51130:
		{
			sp->procFlags	=	PROC_ON_MELEE_ATTACK;
			sp->ProcsPerMinute = 5;
		}break;
	case 49175:
	case 50031:
	case 51456:	// Improved Icy Touch
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_ADD_PCT_MODIFIER;
		}break;
	case 49143:
	case 51416:
	case 51417:
	case 51418:
	case 51419:
	case 55268: // Frost Strike
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;
	case 55090:
	case 55265:
	case 55270:
	case 55271: // Scourge Strike
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;
	case 1843:	// Disarm
		{
			sp->Effect[0] = 0;	// to prevent abuse at Arathi
		}break;
		// Bone	Shield
	case 49222:
		{
			sp->procFlags	=	PROC_ON_ANY_DAMAGE_VICTIM;
			sp->proc_interval	=	3000;
		}break;
		// Shadow	of Death
	case 49157:
		{
			sp->Effect[0]	=	0;	// don't want	DKs	to be	always invincible
		}break;
		// Death Grip
	case 49576:
		{
			sp->FacingCasterFlags	=	0;
		}break;
		// shadow	of death
	case 54223:
		{
			sp->Effect[2]	=	SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
			sp->Flags4 |=	FLAGS4_DEATH_PERSISTENT;
		}break;
	case 54749://Burning Determination
	case 54747:
		{
			sp->procFlags = PROC_ON_SPELL_LAND_VICTIM;
		}break;
	case 48266://blood presence
		{
			sp->EffectTriggerSpell[1] = 50475;
			sp->procFlags = PROC_ON_ANY_HOSTILE_ACTION | PROC_ON_MELEE_ATTACK;
		}break;
	case 50475:
		{
			sp->Effect[0] = SPELL_EFFECT_NULL;
		}break;
	case 48263://Frost Presence
		{
			sp->AdditionalAura = 61261;
		}break;
	case 48265://Unholy Presence
		{
			sp->AdditionalAura = 49772;
		}break;
	case 56364:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case 44443://Firestarter
	case 44442:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x0;
			sp->EffectSpellClassMask[0][1] = 0x0;
		}break;
		//Mage - Brain Freeze
	case 44546:
	case 44584:
	case 44549:
		{
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x0;
			sp->EffectSpellClassMask[0][1] = 0x0;
		}break;
	case 54741:
		{
			sp->AuraInterruptFlags	=	AURA_INTERRUPT_ON_CAST_SPELL;
		}break;

	case 56368:
		{
			sp->Effect[1] = sp->Effect[0];
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;
	case 48020:
		{
			sp->Effect[1] = SPELL_EFFECT_DUMMY;
		}break;
	case 48018: // Demonic Circle dummy shit.
		{
			sp->AdditionalAura = 62388;
			sp->EffectImplicitTargetA[0] = 1;
		}break;

		//Noise Machine - Sonic Shield
	case 54808:
		{
			sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	55019;
			sp->procFlags	=	PROC_ON_MELEE_ATTACK_VICTIM;
			sp->proc_interval	=	60000;
			sp->procChance	=	50;
		}break;
		//Pendulum of Telluric Currents
	case 60482:
		{
			sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	60483;
			sp->procFlags	=	PROC_ON_CAST_SPELL;
			sp->procChance	=	15;
		}break;
		//Tears of Bitter Anguish
	case 58901:
		{
			sp->EffectApplyAuraName[0]	=	SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->EffectTriggerSpell[0]	=	58904;
			sp->procFlags	=	PROC_ON_CRIT_ATTACK;
			sp->procChance	=	10;
		}break;
	case 20578:
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_MOVEMENT|AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN;
		break;
	case 51729:
		{
			sp->buffIndexType = 0;
			sp->buffType = 0;
			sp->AreaGroupId = 0;
		}break;
	case 58691://Pandemic
		{
			sp->spell_can_crit = false;
		}break;
	case 54197:
		{
			sp->Effect[0] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;
		//Warlock - Nether Protection
	case 30299:
	case 30301:
	case 30302:
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM;
		}break;

	case 71905:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;

		//////////////////////////////////////////
		// ITEMSETS								//
		//////////////////////////////////////////

	case 70765: // Divine Storm Cooldown Clear
		{
//#define STORM_NERF
#ifdef STORM_NERF
			sp->procChance = 20; // Crow: I got a feeling...
#else
			sp->procChance = 40; // Current blizzlike settings.
#endif
		}break;

		//////////////////////////////////////////////////////////////////
		//AREA AURA TARGETS - START
		//////////////////////////////////////////////////////////////////

	case 57658: // Shaman - totem of the wrath
	case 57660:
	case 57662:
	case 34123: // Druid - tree of life
		{
			sp->AreaAuraTarget = AA_TARGET_RAID;
		}break;
	
	//Spells using Aura 109
	case 50040:
	case 50041:
	case 50043:
	case 64745:
	case 60675:
	case 60685:
	case 60686:
	case 60687:
	case 60688:
	case 60690:
	case 64936:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procChance = sp->EffectBasePoints[0]+1;
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;

		// Glyph of hex
	case 63291:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_DUMMY;
		}break;
	}

	switch( sp->NameHash )
	{
		//Pal
	case SPELL_HASH_CONCENTRATION_AURA:
	case SPELL_HASH_RETRIBUTION_AURA:
	case SPELL_HASH_DIVINE_GUARDIAN:
		//Hunter
	case SPELL_HASH_ASPECT_OF_THE_WILD:
	case SPELL_HASH_TRUESHOT_AURA:
		//Death knight
	case SPELL_HASH_PATH_OF_FROST:
		//Druid
	case SPELL_HASH_MOONKIN_AURA:
	case SPELL_HASH_IMPROVED_MOONKIN_FORM:
		//Warlock
	case SPELL_HASH_BLOOD_PACT:
	case SPELL_HASH_FEL_INTELLIGENCE:
		{
			sp->AreaAuraTarget = AA_TARGET_RAID;
		}break;

		//Pal
	case SPELL_HASH_DEVOTION_AURA:
	case SPELL_HASH_CRUSADER_AURA:
	case SPELL_HASH_SHADOW_RESISTANCE_AURA:
	case SPELL_HASH_FROST_RESISTANCE_AURA:
	case SPELL_HASH_FIRE_RESISTANCE_AURA:
		//Hunter
	case SPELL_HASH_ASPECT_OF_THE_PACK:
		//Shaman
	case SPELL_HASH_FIRE_RESISTANCE:
	case SPELL_HASH_FROST_RESISTANCE:
	case SPELL_HASH_NATURE_RESISTANCE:
	case SPELL_HASH_STONESKIN:
	case SPELL_HASH_STRENGTH_OF_EARTH:
	case SPELL_HASH_WINDFURY_TOTEM:
	case SPELL_HASH_WRATH_OF_AIR_TOTEM:
		//Priest
	case SPELL_HASH_PRAYER_OF_FORTITUDE:
	case SPELL_HASH_PRAYER_OF_SHADOW_PROTECTION:
	case SPELL_HASH_PRAYER_OF_SPIRIT:
		//Warrior
	case SPELL_HASH_BATTLE_SHOUT:
	case SPELL_HASH_COMMANDING_SHOUT:
		{
			sp->AreaAuraTarget = AA_TARGET_PARTY;
		}break;

		//Hunter
	case SPELL_HASH_ASPECT_OF_THE_BEAST:
		{
			sp->AreaAuraTarget = AA_TARGET_PET;
		}break;
		//Rogue
	case SPELL_HASH_HONOR_AMONG_THIEVES:
		{
			sp->AreaAuraTarget = AA_TARGET_NOTSELF | AA_TARGET_PARTY;
		}break;

		//////////////////////////////////////////////////////
		// CLASS-SPECIFIC NAMEHASH FIXES					//
		//////////////////////////////////////////////////////
		////////////// WARRIOR ///////////////////

	case SPELL_HASH_MORTAL_STRIKE:
		sp->maxstack = 1; // Healing reduction shouldn't stack
		break;

	case SPELL_HASH_TRAUMA:
		sp->procFlags = PROC_ON_CRIT_ATTACK;
		break;

	case SPELL_HASH_SLAM:
		sp->Effect[0] = SPELL_EFFECT_SCHOOL_DAMAGE;
		break;

	case SPELL_HASH_HOLY_SHIELD:
		sp->procflags2 = PROC_ON_BLOCK_VICTIM;
		break;

		////////////// PALADIN ///////////////////

	case SPELL_HASH_JUDGEMENT_OF_WISDOM:
	case SPELL_HASH_JUDGEMENT_OF_LIGHT:
	case SPELL_HASH_JUDGEMENT_OF_JUSTICE:
	case SPELL_HASH_HEART_OF_THE_CRUSADER:
		sp->maxstack = 1;
		break;

	case SPELL_HASH_SEAL_OF_LIGHT:
		{
			sp->Effect[2] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[2] = SPELL_AURA_DUMMY;
			sp->ProcsPerMinute = 10;	/* this will do */
		}break;

	case SPELL_HASH_SEALS_OF_THE_PURE:
		{
			sp->EffectSpellClassMask[0][0] = 8389632 | 4194312 | 512;
			sp->EffectSpellClassMask[0][1] = 134217728 | 536870912;
			sp->EffectSpellClassMask[0][2] = 0;
			sp->EffectSpellClassMask[1][0] = 2048;
			sp->EffectSpellClassMask[1][1] = 0;
			sp->EffectSpellClassMask[1][2] = 0;
		}break;

	case SPELL_HASH_JUDGEMENTS_OF_THE_PURE:
		{	// Ignore our triggered spells.
			if(sp->Id != 54153 && sp->Id != 53655 && sp->Id != 53656 && sp->Id != 53657 && sp->Id != 54152 )
			{
				sp->EffectSpellClassMask[0][0] = 8388608;
				sp->EffectSpellClassMask[0][1] = 0;
				sp->EffectSpellClassMask[0][2] = 0;
				sp->EffectSpellClassMask[1][0] = 8389632 | 4194312 | 512;
				sp->EffectSpellClassMask[1][1] = 134217728 | 536870912 | 33554432;
				sp->EffectSpellClassMask[1][2] = 0;
				sp->EffectSpellClassMask[2][0] = 2048;
				sp->EffectSpellClassMask[2][1] = 0;
				sp->EffectSpellClassMask[2][2] = 0;
				sp->procFlags = PROC_ON_CAST_SPELL;
			}
		}break;
	case SPELL_HASH_DEVOURING_PLAGUE:
		{
			sp->MaxTargets = 1;
		}break;
	case SPELL_HASH_IMPROVED_DEVOTION_AURA:
		{
				sp->EffectApplyAuraName[1] = SPELL_AURA_MOD_HEALING_PCT;
				sp->EffectBasePoints[1] = 6;
				sp->EffectMiscValue[1] = 127;
		}break;
	case SPELL_HASH_AVENGER_S_SHIELD:
		{
			sp->Spell_Dmg_Type = SPELL_DMG_TYPE_MAGIC;
		}break;
	case SPELL_HASH_SHIELD_OF_RIGHTEOUSNESS:
		{
			sp->EffectChainTarget[0] = 0;
		}break;
	case SPELL_HASH_AIMED_SHOT:
		{
			sp->maxstack = 1; // Healing reduction shouldn't stack
		}break;
	case SPELL_HASH_EXPLOSIVE_SHOT:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PERIODIC_DAMAGE;
		}break;
	case SPELL_HASH_MORTAL_SHOTS:
		{
			sp->EffectSpellClassMask[0][0] += 1;
		}break;
	case SPELL_HASH_MEND_PET:
		{
			sp->ChannelInterruptFlags = 0;
		}break;
	case SPELL_HASH_EAGLE_EYE:
		{
			sp->Effect[1] = 0;
		}break;
	case SPELL_HASH_ENTRAPMENT:
		{
			if(sp->EffectApplyAuraName[0] == SPELL_AURA_MOD_ROOT)
			{
				sp->EffectImplicitTargetA[0] = 15;
				sp->EffectRadiusIndex[0] = 13;
			}
		}break;
	case SPELL_HASH_WILD_QUIVER:
		{
			sp->EffectApplyAuraName[1] = 0;
		}break;
	case SPELL_HASH_REMORSELESS_ATTACKS:
		{
			sp->procFlags = PROC_ON_GAIN_EXPIERIENCE;
		}break;
	case SPELL_HASH_UNFAIR_ADVANTAGE:
		{
			sp->procflags2 = PROC_ON_DODGE_VICTIM;
		}break;
	case SPELL_HASH_COMBAT_POTENCY:
		{
			sp->procFlags = PROC_ON_MELEE_ATTACK;
		}break;
	case SPELL_HASH_PAIN_AND_SUFFERING:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case SPELL_HASH_FOCUSED_ATTACKS:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
		}break;
	case SPELL_HASH_SEAL_FATE:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;
	case SPELL_HASH_VILE_POISONS:
		{
			sp->EffectSpellClassMask[0][0] = 8388608; // envenom
			sp->EffectSpellClassMask[0][1] = 8;
			sp->EffectSpellClassMask[1][0] = 8192 | 268435456 | 65536; //poisons
			sp->EffectSpellClassMask[1][1] = 524288;
		}break;
	case SPELL_HASH_STEALTH:
		{
			if( !(sp->AuraInterruptFlags & AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN ) )
				sp->AuraInterruptFlags |= AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN;

			// fuck this
			sp->EffectBasePoints[1] = 0;
		}break;
	case SPELL_HASH_NERVES_OF_STEEL:
		{
			sp->CasterAuraState = 6;
			sp->EffectBasePoints[0] = -31;
			sp->EffectApplyAuraName[0] = SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN;
		}break;
	case SPELL_HASH_DISARM_TRAP:
		{
			sp->Effect[0] = SPELL_EFFECT_DUMMY;
		}break;
	case SPELL_HASH_BORROWED_TIME:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectApplyAuraName[1] = SPELL_AURA_ADD_PCT_MODIFIER;
		}break;
	case SPELL_HASH_IMPROVED_SPIRIT_TAP:
		{
			sp->procflags2 = PROC_ON_SPELL_CRIT_HIT;
		}break;
	case SPELL_HASH_MISERY:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case SPELL_HASH_POWER_INFUSION:
	case SPELL_HASH_HEROISM:
	case SPELL_HASH_BLOODLUST:
		{
			sp->buffType = SPELL_TYPE_HASTE;
		}break;
	case SPELL_HASH_HEX:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_WEAPON_UNSHEATH;
		}break;
	case SPELL_HASH_LIGHTNING_SHIELD:
		{
			sp->spell_can_crit = false;
		}break;
	case SPELL_HASH_FROSTBRAND_WEAPON:
		{
			sp->ProcsPerMinute = 9.0f;
			sp->Flags3 |= FLAGS3_ENCHANT_OWN_ONLY;
		}break;
	case SPELL_HASH_NATURE_S_GUARDIAN:
		{
			sp->procFlags = PROC_ON_SPELL_HIT_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM | PROC_ON_ANY_DAMAGE_VICTIM;
			sp->proc_interval = 8000;
			sp->EffectTriggerSpell[0] = 31616;
		}break;
	case SPELL_HASH_WINDFURY_WEAPON:
	case SPELL_HASH_FLAMETONGUE_WEAPON:
	case SPELL_HASH_ROCKBITER_WEAPON:
	case SPELL_HASH_EARTHLIVING_WEAPON:
		{
			sp->Flags3 |= FLAGS3_ENCHANT_OWN_ONLY;
		}break;
	case SPELL_HASH_STONECLAW_TOTEM_PASSIVE:
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		}break;
	case SPELL_HASH_FLAMETONGUE_TOTEM:
		{
			sp->AreaAuraTarget = AA_TARGET_PARTY;
			if(sp->Attributes & ATTRIBUTES_PASSIVE)
			{
				sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
				sp->EffectImplicitTargetB[0] = 0;
				sp->EffectImplicitTargetA[1] = EFF_TARGET_SELF;
				sp->EffectImplicitTargetB[1] = 0;
			}
		}break;
	case SPELL_HASH_UNLEASHED_RAGE:
		{
			sp->procFlags = PROC_ON_CRIT_ATTACK;
		}break;
	case SPELL_HASH_HEALING_STREAM_TOTEM:
		{
			if(sp->Effect[0] == SPELL_EFFECT_DUMMY)
			{
				sp->EffectRadiusIndex[0] = 10; // 30 yards
				sp->Effect[0] = SPELL_EFFECT_HEAL;
				sp->logsId = 5394;
			}
		}break;
	case SPELL_HASH_MANA_SPRING_TOTEM:
		{
			if(sp->Effect[0] == SPELL_EFFECT_DUMMY)
			{
				sp->Effect[0] = SPELL_EFFECT_ENERGIZE;
				sp->EffectMiscValue[0] = POWER_TYPE_MANA;
				sp->logsId = 5675;
			}
		}break;
	case SPELL_HASH_FAR_SIGHT:
		{
			sp->Effect[1] = 0;
		}break;
	case SPELL_HASH_HYPOTHERMIA:
		{
			sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
		}break;
	case SPELL_HASH_IMPROVED_COUNTERSPELL:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->EffectSpellClassMask[0][0] = 0x00004000;	// Proc on counterspell only
		}break;
	case SPELL_HASH_SHADOW_WEAVING:
		{
			sp->EffectApplyAuraName[0] = SPELL_AURA_PROC_TRIGGER_SPELL;
			sp->procFlags = PROC_ON_CAST_SPELL;
			sp->procChance = sp->EffectBasePoints[0] + 1;
		}break;
	case SPELL_HASH_SHADOW_TRANCE:
		{
			sp->AuraInterruptFlags = AURA_INTERRUPT_ON_CAST_SPELL;
		}break;
	case SPELL_HASH_ERADICATION:
		{
			sp->EffectTriggerSpell[0] = 54370;
			sp->procFlags = PROC_ON_SPELL_LAND;
		}break;
	case SPELL_HASH_INFECTED_WOUNDS:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case SPELL_HASH_POUNCE:
		{
			sp->always_apply = true;
		}break;
	case SPELL_HASH_OWLKIN_FRENZY:
		{
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;
		}break;
	case SPELL_HASH_EARTH_AND_MOON:
		{
			sp->procFlags = PROC_ON_CAST_SPELL;
		}break;
	case SPELL_HASH_STARFALL:
		{
			if(sp->Effect[1] == SPELL_EFFECT_TRIGGER_SPELL )
			{//we can only attack one target with main star
				sp->MaxTargets = 1;
			}
		}break;
	case SPELL_HASH_SHRED:
		{
			sp->MechanicsType = MECHANIC_BLEEDING;
		}break;
	case SPELL_HASH_NURTURING_INSTINCT:
		{
			sp->Effect[1] = SPELL_EFFECT_APPLY_AURA;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;
	case SPELL_HASH_PRIMAL_TENACITY:
		{
			sp->DurationIndex = 21;
			sp->EffectBasePoints[1] = 0;
			sp->EffectApplyAuraName[1] = SPELL_AURA_DUMMY;
		}break;
	case SPELL_HASH_PROWL:
		{
			sp->EffectBasePoints[0] = 0;
		}break;
	}

	if( sp->EquippedItemClass == 2 && sp->EquippedItemSubClass & 262156 ) // 4 + 8 + 262144 ( becomes item classes 2, 3 and 18 which correspond to bow, gun and crossbow respectively)
		sp->is_ranged_spell = true;

	//////////////////////////////////////////////////////////////////
	//AREA AURA TARGETS - END
	//////////////////////////////////////////////////////////////////

	if( IsDamagingSpell( sp ) )
		sp->c_is_flags |= SPELL_FLAG_IS_DAMAGING;
	if( IsHealingSpell( sp ) )
		sp->c_is_flags |= SPELL_FLAG_IS_HEALING;
	if( IsTargetingStealthed( sp ) )
		sp->c_is_flags |= SPELL_FLAG_IS_TARGETINGSTEALTHED;
	if( IsFlyingSpell(sp) )
		sp->c_is_flags |= SPELL_FLAG_IS_FLYING;
	if( IsCastedOnFriends(sp) )
		sp->c_is_flags |= SPELL_FLAG_CASTED_ON_FRIENDS;
	if( IsCastedOnEnemies(sp) )
		sp->c_is_flags |= SPELL_FLAG_CASTED_ON_ENEMIES;
	if(sp->Flags3 & FLAGS3_CANT_CRIT) //I can haz hacky? :O
		sp->spell_can_crit = false;
}

void ApplyCoeffSpellFixes(SpellEntry *sp)
{
	switch(sp->Id)
	{
	case 17:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 116:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 120:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 122:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 133:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 324:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 325:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 331:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 332:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 339:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 348:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 403:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 421:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 529:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 547:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 548:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 585:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 589:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 591:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 592:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 594:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 596:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 598:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 600:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 603:
		{
			sp->AP_coef_override = float(2.000000f);
		}break;
	case 635:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 639:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 647:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 686:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 689:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 695:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 699:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 703:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 705:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 707:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 709:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 740:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 755:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 774:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 837:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 879:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 905:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 913:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 915:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 930:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 943:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 945:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 970:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 974:
		{
			sp->AP_coef_override = float(0.476100f);
		}break;
	case 980:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 984:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 992:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 996:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 1004:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 1014:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 1026:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 1042:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 1058:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 1062:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 1064:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 1079:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 1088:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 1094:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 1106:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 1120:
		{
			sp->AP_coef_override = float(0.429000f);
		}break;
	case 1430:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 1449:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 1463:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 1495:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 1776:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 1777:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 1949:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 1978:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 2060:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 2061:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 2090:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 2091:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 2120:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 2121:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 2136:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 2137:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 2138:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 2643:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 2767:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 2812:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 2818:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 2819:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 2860:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 2912:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 2941:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 2944:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 2948:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 3009:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 3010:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 3044:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 3140:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 3472:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 3606:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 3627:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 3698:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 3699:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 3700:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 3747:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 5143:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 5144:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 5145:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 5176:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 5177:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 5178:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 5179:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 5180:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 5185:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 5186:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 5187:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 5188:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 5189:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 5195:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 5196:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 5308:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 5570:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 5614:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 5615:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 5676:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 5740:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 6041:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 6060:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 6065:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 6066:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 6074:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 6075:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 6076:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 6077:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 6078:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 6131:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 6217:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 6219:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 6222:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 6223:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 6229:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 6343:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 6350:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 6351:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 6352:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 6353:
		{
			sp->AP_coef_override = float(1.150000f);
		}break;
	case 6572:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 6574:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 6778:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 6780:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 6789:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 7322:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 7379:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 7641:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 7648:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 7651:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 8004:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 8005:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 8008:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 8010:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 8026:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8028:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8029:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8034:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8037:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8042:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 8044:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 8045:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 8046:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 8050:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8052:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8053:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8056:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 8058:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 8092:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 8102:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 8103:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 8104:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 8105:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 8106:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 8134:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 8187:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8198:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 8204:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 8205:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 8288:
		{
			sp->AP_coef_override = float(0.429000f);
		}break;
	case 8289:
		{
			sp->AP_coef_override = float(0.429000f);
		}break;
	case 8400:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 8401:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 8402:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 8406:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 8407:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 8408:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 8412:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 8413:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 8416:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 8417:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 8422:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 8423:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 8437:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 8438:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 8439:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 8444:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 8445:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 8446:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 8492:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 8494:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 8495:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 8629:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 8631:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 8632:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 8633:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 8680:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8685:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8689:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 8903:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 8905:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 8910:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 8918:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 8921:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8924:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8925:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8926:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8927:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8928:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8929:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 8936:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 8938:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 8939:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 8940:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 8941:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 8949:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 8950:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 8951:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 9472:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 9473:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 9474:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 9492:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 9493:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 9750:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 9752:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 9758:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 9839:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 9840:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 9841:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 9852:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 9853:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 9856:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 9857:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 9858:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 9862:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 9863:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 9875:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 9876:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 9888:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 9889:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 9894:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 9896:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 9912:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 10148:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 10149:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 10150:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 10151:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 10159:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 10160:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 10161:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 10179:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 10180:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 10181:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 10191:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 10192:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 10193:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 10197:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 10199:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 10201:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 10202:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 10205:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 10206:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 10207:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 10211:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 10212:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 10215:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 10216:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 10230:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 10312:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 10313:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 10314:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 10318:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 10328:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 10329:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 10391:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 10392:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 10395:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 10396:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 10412:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 10413:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 10414:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 10431:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 10432:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 10435:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 10436:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 10445:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10447:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10448:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10458:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10466:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 10467:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 10468:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 10472:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 10473:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 10579:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10580:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10581:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 10605:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 10622:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 10623:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 10892:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 10893:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 10894:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 10898:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10899:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10900:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10901:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10915:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10916:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10917:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10927:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 10928:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 10929:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 10933:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 10934:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 10945:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 10946:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 10947:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 10960:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10961:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 10963:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 10964:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 10965:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 11113:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 11285:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11286:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11289:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 11290:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 11335:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 11336:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 11337:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 11353:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 11354:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 11366:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 11426:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 11580:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 11581:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 11600:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 11601:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 11659:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 11660:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 11661:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 11665:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11667:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11668:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11671:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11672:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 11675:
		{
			sp->AP_coef_override = float(0.429000f);
		}break;
	case 11677:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 11678:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 11683:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 11684:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 11693:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 11694:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 11695:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 11699:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 11700:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 11711:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 11712:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 11713:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 11739:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 11740:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 12505:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 12522:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 12523:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 12524:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 12525:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 12526:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 12809:
		{
			sp->AP_coef_override = float(0.750000f);
		}break;
	case 13018:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 13019:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 13020:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 13021:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 13031:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 13032:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 13033:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 13218:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 13222:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 13223:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 13224:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 13549:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13550:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13551:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13552:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13553:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13554:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13555:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 13797:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 14269:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 14270:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 14271:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 14281:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14282:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14283:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14284:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14285:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14286:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14287:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 14288:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 14289:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 14290:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 14298:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 14299:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 14300:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 14301:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 14914:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15207:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 15208:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 15237:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 15261:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15262:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15263:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15264:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15265:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15266:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15267:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 15407:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 15430:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 15431:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 16343:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 16344:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 16352:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 16353:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 16827:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 16828:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 16829:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 16830:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 16831:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 16832:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 16857:
		{
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 17253:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17255:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17256:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17257:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17258:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17259:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17260:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17261:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 17311:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 17312:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 17313:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 17314:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 17877:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 17919:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 17920:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 17921:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 17922:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 17923:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 17924:
		{
			sp->AP_coef_override = float(1.150000f);
		}break;
	case 17925:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 17926:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 17962:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 18220:
		{
			sp->AP_coef_override = float(0.960000f);
		}break;
	case 18807:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 18809:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 18867:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 18868:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 18869:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 18870:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 18871:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 18937:
		{
			sp->AP_coef_override = float(0.960000f);
		}break;
	case 18938:
		{
			sp->AP_coef_override = float(0.960000f);
		}break;
	case 19236:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 19238:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 19240:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 19241:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 19242:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 19243:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 19276:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 19277:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 19278:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 19279:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 19280:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 19750:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 19939:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 19940:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 19941:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 19942:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 19943:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 20116:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 20167:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 20168:
		{
			sp->AP_coef_override = float(0.250000f);
		}break;
	case 20187:
		{
			sp->AP_coef_override = float(0.400000f);
			sp->AP_coef_override = float(0.250000f);
		}break;
	case 20252:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 20267:
		{
			sp->AP_coef_override = float(0.100000f);
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 20424:
		{
			sp->AP_coef_override = float(0.230000f);
		}break;
	case 20467:
		{
			sp->AP_coef_override = float(0.250000f);
			sp->AP_coef_override = float(0.160000f);
		}break;
	case 20658:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 20660:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 20661:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 20662:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 20922:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 20923:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 20924:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 20925:
		{
			sp->AP_coef_override = float(0.090000f);
			sp->AP_coef_override = float(0.056000f);
		}break;
	case 20927:
		{
			sp->AP_coef_override = float(0.090000f);
			sp->AP_coef_override = float(0.056000f);
		}break;
	case 20928:
		{
			sp->AP_coef_override = float(0.090000f);
			sp->AP_coef_override = float(0.056000f);
		}break;
	case 21084:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.039000f);
		}break;
	case 23455:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 23458:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 23459:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 24239:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 24274:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 24275:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 24583:
		{
			sp->RAP_coef_override = float(0.015000f);
		}break;
	case 24586:
		{
			sp->RAP_coef_override = float(0.015000f);
		}break;
	case 24587:
		{
			sp->RAP_coef_override = float(0.015000f);
		}break;
	case 24640:
		{
			sp->RAP_coef_override = float(0.015000f);
		}break;
	case 24844:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 24974:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 24975:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 24976:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 24977:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 25008:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 25009:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 25010:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 25011:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 25012:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 25210:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 25213:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 25217:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25218:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25221:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 25222:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 25233:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25234:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 25235:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25236:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 25264:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 25269:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 25288:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 25292:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 25294:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 25295:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 25297:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 25298:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 25299:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 25304:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 25306:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 25307:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 25308:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25309:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 25311:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 25314:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 25315:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 25316:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25329:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 25331:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 25345:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 25349:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 25357:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 25363:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 25364:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 25367:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 25368:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 25372:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 25375:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 25384:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 25387:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 25391:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 25396:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 25420:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 25422:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 25423:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 25437:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 25439:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 25442:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 25448:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 25449:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 25454:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 25457:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 25464:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 25467:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 25469:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 25472:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 25488:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 25501:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 25530:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 25550:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 25902:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 25903:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 25911:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 25912:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 25913:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 25914:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 26573:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 26839:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 26884:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 26890:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 26968:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 26978:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 26979:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 26980:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 26981:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 26982:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 26983:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 26984:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 26985:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 26986:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 26987:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 26988:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 26989:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 27008:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 27013:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 27016:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 27019:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 27021:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 27024:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 27049:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 27050:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 27060:
		{
			sp->RAP_coef_override = float(0.015000f);
		}break;
	case 27070:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 27071:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 27072:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 27073:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 27074:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 27075:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 27078:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 27079:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 27080:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 27082:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 27086:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 27087:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 27088:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 27131:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 27132:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 27133:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 27134:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 27135:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 27136:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 27137:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 27138:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 27139:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 27173:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 27175:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 27176:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 27179:
		{
			sp->AP_coef_override = float(0.090000f);
			sp->AP_coef_override = float(0.056000f);
		}break;
	case 27180:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 27187:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 27189:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 27209:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 27210:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 27211:
		{
			sp->AP_coef_override = float(1.150000f);
		}break;
	case 27212:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 27213:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 27215:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 27216:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 27217:
		{
			sp->AP_coef_override = float(0.429000f);
		}break;
	case 27218:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 27219:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 27220:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 27223:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 27243:
		{
			sp->AP_coef_override = float(0.250000f);
		}break;
	case 27259:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 27263:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 27265:
		{
			sp->AP_coef_override = float(0.960000f);
		}break;
	case 27799:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 27800:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 27801:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 27803:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 27804:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 27805:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 28610:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 29228:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 29722:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 30108:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 30283:
		{
			sp->AP_coef_override = float(0.193200f);
		}break;
	case 30357:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 30404:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 30405:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 30413:
		{
			sp->AP_coef_override = float(0.193200f);
		}break;
	case 30414:
		{
			sp->AP_coef_override = float(0.193200f);
		}break;
	case 30451:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 30455:
		{
			sp->AP_coef_override = float(0.142900f);
		}break;
	case 30459:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 30546:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 30824:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 30910:
		{
			sp->AP_coef_override = float(2.000000f);
		}break;
	case 31117:
		{
			sp->AP_coef_override = float(1.800000f);
		}break;
	case 31661:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 31803:
		{
			sp->AP_coef_override = float(0.025000f);
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 31804:
		{
			sp->AP_coef_override = float(0.220000f);
			sp->AP_coef_override = float(0.140000f);
		}break;
	case 31893:
		{
			sp->AP_coef_override = float(0.020000f);
			sp->AP_coef_override = float(0.030000f);
		}break;
	case 31898:
		{
			sp->AP_coef_override = float(0.250000f);
			sp->AP_coef_override = float(0.160000f);
		}break;
	case 31935:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 32231:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 32379:
		{
			sp->AP_coef_override = float(0.429600f);
		}break;
	case 32546:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 32593:
		{
			sp->AP_coef_override = float(0.476100f);
		}break;
	case 32594:
		{
			sp->AP_coef_override = float(0.476100f);
		}break;
	case 32699:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 32700:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 32996:
		{
			sp->AP_coef_override = float(0.429600f);
		}break;
	case 33041:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 33042:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 33043:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 33073:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 33074:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 33405:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 33745:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 33763:
		{
			sp->AP_coef_override = float(0.095180f);
		}break;
	case 33933:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 33938:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 34120:
		{
			sp->RAP_coef_override = float(0.100000f);
		}break;
	case 34428:
		{
			sp->AP_coef_override = float(0.450000f);
		}break;
	case 34861:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 34863:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 34864:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 34865:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 34866:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 34889:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 34914:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 34916:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 34917:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 35290:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35291:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35292:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35293:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35294:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35295:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35323:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35387:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35389:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 35392:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 36916:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 38692:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 38697:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 38699:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 38704:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 38764:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 41637:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 42198:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42208:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42209:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42210:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42211:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42212:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42213:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42230:
		{
			sp->AP_coef_override = float(0.128980f);
		}break;
	case 42231:
		{
			sp->AP_coef_override = float(0.128980f);
		}break;
	case 42232:
		{
			sp->AP_coef_override = float(0.128980f);
		}break;
	case 42233:
		{
			sp->AP_coef_override = float(0.128980f);
		}break;
	case 42832:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 42833:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 42841:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 42842:
		{
			sp->AP_coef_override = float(0.814300f);
		}break;
	case 42843:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 42846:
		{
			sp->AP_coef_override = float(0.285700f);
		}break;
	case 42858:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 42859:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 42872:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 42873:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 42890:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 42891:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 42894:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 42896:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 42897:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 42913:
		{
			sp->AP_coef_override = float(0.142900f);
		}break;
	case 42914:
		{
			sp->AP_coef_override = float(0.142900f);
		}break;
	case 42917:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 42920:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 42921:
		{
			sp->AP_coef_override = float(0.212800f);
		}break;
	case 42925:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 42926:
		{
			sp->AP_coef_override = float(0.122000f);
		}break;
	case 42930:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 42931:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 42937:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42938:
		{
			sp->AP_coef_override = float(0.143700f);
		}break;
	case 42944:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 42945:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 42949:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 42950:
		{
			sp->AP_coef_override = float(0.193600f);
		}break;
	case 43019:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 43020:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 43038:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 43039:
		{
			sp->AP_coef_override = float(0.805300f);
		}break;
	case 44425:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 44457:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 44614:
		{
			sp->AP_coef_override = float(0.857100f);
		}break;
	case 44780:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 44781:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 46968:
		{
			sp->AP_coef_override = float(0.750000f);
		}break;
	case 47470:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47471:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47501:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 47502:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 47610:
		{
			sp->AP_coef_override = float(0.857100f);
		}break;
	case 47666:
		{
			sp->AP_coef_override = float(0.229000f);
		}break;
	case 47750:
		{
			sp->AP_coef_override = float(0.535000f);
		}break;
	case 47808:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 47809:
		{
			sp->AP_coef_override = float(0.856900f);
		}break;
	case 47810:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47811:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47812:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47813:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47814:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 47815:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 47819:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 47820:
		{
			sp->AP_coef_override = float(0.693200f);
		}break;
	case 47823:
		{
			sp->AP_coef_override = float(0.120000f);
		}break;
	case 47824:
		{
			sp->AP_coef_override = float(1.150000f);
		}break;
	case 47825:
		{
			sp->AP_coef_override = float(1.150000f);
		}break;
	case 47826:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 47827:
		{
			sp->AP_coef_override = float(0.429300f);
		}break;
	case 47835:
		{
			sp->AP_coef_override = float(0.250000f);
		}break;
	case 47836:
		{
			sp->AP_coef_override = float(0.250000f);
		}break;
	case 47837:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 47838:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 47841:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47843:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 47846:
		{
			sp->AP_coef_override = float(0.193200f);
		}break;
	case 47847:
		{
			sp->AP_coef_override = float(0.193200f);
		}break;
	case 47855:
		{
			sp->AP_coef_override = float(0.429000f);
		}break;
	case 47856:
		{
			sp->AP_coef_override = float(0.448500f);
		}break;
	case 47857:
		{
			sp->AP_coef_override = float(0.143000f);
		}break;
	case 47859:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 47860:
		{
			sp->AP_coef_override = float(0.214000f);
		}break;
	case 47863:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 47864:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 47867:
		{
			sp->AP_coef_override = float(2.000000f);
		}break;
	case 47890:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 47891:
		{
			sp->AP_coef_override = float(0.300000f);
		}break;
	case 48062:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 48063:
		{
			sp->AP_coef_override = float(1.613500f);
		}break;
	case 48065:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48066:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48067:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 48068:
		{
			sp->AP_coef_override = float(0.360000f);
		}break;
	case 48070:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48071:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48072:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48075:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 48076:
		{
			sp->AP_coef_override = float(0.303500f);
		}break;
	case 48077:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 48078:
		{
			sp->AP_coef_override = float(0.160600f);
		}break;
	case 48088:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 48089:
		{
			sp->AP_coef_override = float(0.402000f);
		}break;
	case 48119:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48120:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48122:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 48123:
		{
			sp->AP_coef_override = float(0.714000f);
		}break;
	case 48124:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 48125:
		{
			sp->AP_coef_override = float(0.182900f);
		}break;
	case 48126:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 48127:
		{
			sp->AP_coef_override = float(0.428000f);
		}break;
	case 48134:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 48135:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 48155:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 48156:
		{
			sp->AP_coef_override = float(0.257000f);
		}break;
	case 48157:
		{
			sp->AP_coef_override = float(0.429600f);
		}break;
	case 48158:
		{
			sp->AP_coef_override = float(0.429600f);
		}break;
	case 48159:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 48160:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 48172:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48173:
		{
			sp->AP_coef_override = float(0.806800f);
		}break;
	case 48181:
		{
			sp->AP_coef_override = float(0.479300f);
		}break;
	case 48299:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 48300:
		{
			sp->AP_coef_override = float(0.184900f);
		}break;
	case 48377:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 48378:
		{
			sp->AP_coef_override = float(1.610400f);
		}break;
	case 48438:
		{
			sp->AP_coef_override = float(0.115050f);
		}break;
	case 48440:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 48441:
		{
			sp->AP_coef_override = float(0.376040f);
		}break;
	case 48442:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 48443:
		{
			sp->AP_coef_override = float(0.188000f);
		}break;
	case 48446:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 48447:
		{
			sp->AP_coef_override = float(0.538000f);
		}break;
	case 48450:
		{
			sp->AP_coef_override = float(0.095180f);
		}break;
	case 48451:
		{
			sp->AP_coef_override = float(0.095180f);
		}break;
	case 48459:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 48461:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 48462:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 48463:
		{
			sp->AP_coef_override = float(0.130000f);
		}break;
	case 48464:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 48465:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 48466:
		{
			sp->AP_coef_override = float(0.128980f);
		}break;
	case 48468:
		{
			sp->AP_coef_override = float(0.127000f);
		}break;
	case 48567:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 48675:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 48676:
		{
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 48781:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 48782:
		{
			sp->AP_coef_override = float(1.660000f);
		}break;
	case 48784:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 48785:
		{
			sp->AP_coef_override = float(1.000000f);
		}break;
	case 48800:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 48801:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 48805:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 48806:
		{
			sp->AP_coef_override = float(0.150000f);
			sp->AP_coef_override = float(0.150000f);
		}break;
	case 48816:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 48817:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 48818:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 48819:
		{
			sp->AP_coef_override = float(0.040000f);
		}break;
	case 48820:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 48821:
		{
			sp->AP_coef_override = float(0.810000f);
		}break;
	case 48822:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 48823:
		{
			sp->AP_coef_override = float(0.428600f);
		}break;
	case 48826:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 48827:
		{
			sp->AP_coef_override = float(0.070000f);
			sp->AP_coef_override = float(0.070000f);
		}break;
	case 48951:
		{
			sp->AP_coef_override = float(0.090000f);
			sp->AP_coef_override = float(0.056000f);
		}break;
	case 48952:
		{
			sp->AP_coef_override = float(0.090000f);
			sp->AP_coef_override = float(0.056000f);
		}break;
	case 49000:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 49001:
		{
			sp->RAP_coef_override = float(0.040000f);
		}break;
	case 49044:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 49045:
		{
			sp->RAP_coef_override = float(0.150000f);
		}break;
	case 49047:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 49048:
		{
			sp->RAP_coef_override = float(0.200000f);
		}break;
	case 49051:
		{
			sp->RAP_coef_override = float(0.100000f);
		}break;
	case 49052:
		{
			sp->RAP_coef_override = float(0.100000f);
		}break;
	case 49053:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 49054:
		{
			sp->RAP_coef_override = float(0.020000f);
		}break;
	case 49230:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 49231:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 49232:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 49233:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 49235:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 49236:
		{
			sp->AP_coef_override = float(0.385800f);
		}break;
	case 49237:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 49238:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 49270:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 49271:
		{
			sp->AP_coef_override = float(0.400000f);
		}break;
	case 49272:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 49273:
		{
			sp->AP_coef_override = float(1.610600f);
		}break;
	case 49275:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 49276:
		{
			sp->AP_coef_override = float(0.808200f);
		}break;
	case 49280:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 49281:
		{
			sp->AP_coef_override = float(0.330000f);
		}break;
	case 49283:
		{
			sp->AP_coef_override = float(0.476100f);
		}break;
	case 49284:
		{
			sp->AP_coef_override = float(0.476100f);
		}break;
	case 49799:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 49800:
		{
			sp->AP_coef_override = float(0.010000f);
		}break;
	case 49821:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 50288:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 50294:
		{
			sp->AP_coef_override = float(0.012000f);
		}break;
	case 50464:
		{
			sp->AP_coef_override = float(0.661100f);
		}break;
	case 50516:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 50590:
		{
			sp->AP_coef_override = float(0.162200f);
		}break;
	case 50796:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 51505:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 52041:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 52046:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 52047:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 52048:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 52049:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 52050:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 52471:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 52472:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 52473:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 52474:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 52983:
		{
			sp->AP_coef_override = float(0.535000f);
		}break;
	case 52984:
		{
			sp->AP_coef_override = float(0.535000f);
		}break;
	case 52985:
		{
			sp->AP_coef_override = float(0.535000f);
		}break;
	case 52998:
		{
			sp->AP_coef_override = float(0.229000f);
		}break;
	case 52999:
		{
			sp->AP_coef_override = float(0.229000f);
		}break;
	case 53000:
		{
			sp->AP_coef_override = float(0.229000f);
		}break;
	case 53022:
		{
			sp->AP_coef_override = float(0.714300f);
		}break;
	case 53188:
		{
			sp->AP_coef_override = float(0.012000f);
		}break;
	case 53189:
		{
			sp->AP_coef_override = float(0.012000f);
		}break;
	case 53190:
		{
			sp->AP_coef_override = float(0.012000f);
		}break;
	case 53191:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 53194:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 53195:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 53223:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 53225:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 53226:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 53248:
		{
			sp->AP_coef_override = float(0.115050f);
		}break;
	case 53249:
		{
			sp->AP_coef_override = float(0.115050f);
		}break;
	case 53251:
		{
			sp->AP_coef_override = float(0.115050f);
		}break;
	case 53301:
		{
			sp->RAP_coef_override = float(0.140000f);
		}break;
	case 53308:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 53339:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 53351:
		{
			sp->RAP_coef_override = float(0.400000f);
		}break;
	case 53719:
		{
			sp->AP_coef_override = float(0.250000f);
		}break;
	case 53726:
		{
			sp->AP_coef_override = float(0.250000f);
			sp->AP_coef_override = float(0.160000f);
		}break;
	case 53733:
		{
			sp->AP_coef_override = float(0.250000f);
			sp->AP_coef_override = float(0.160000f);
		}break;
	case 53742:
		{
			sp->AP_coef_override = float(0.025000f);
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 55359:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 55360:
		{
			sp->AP_coef_override = float(0.200000f);
		}break;
	case 55458:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 55459:
		{
			sp->AP_coef_override = float(0.800000f);
		}break;
	case 55482:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55483:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55484:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55485:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55555:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55556:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55557:
		{
			sp->RAP_coef_override = float(0.125000f);
		}break;
	case 55728:
		{
			sp->RAP_coef_override = float(0.015000f);
		}break;
	case 56641:
		{
			sp->RAP_coef_override = float(0.100000f);
		}break;
	case 23881: // Bloodthirst
	case 57755: // Heroic Throw
	case 64382: // Shattering Throw
		{
			sp->AP_coef_override = float(0.500000f);
		}break;
	case 57823:
		{
			sp->AP_coef_override = float(0.207000f);
		}break;
	case 57964:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 57965:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 57969:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 57970:
		{
			sp->AP_coef_override = float(0.024000f);
		}break;
	case 57974:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 57975:
		{
			sp->AP_coef_override = float(0.008000f);
		}break;
	case 58597:
		{
			sp->AP_coef_override = float(0.750000f);
		}break;
	case 58700:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 58701:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 58702:
		{
			sp->AP_coef_override = float(0.166700f);
		}break;
	case 58732:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58735:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58759:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 58760:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 58761:
		{
			sp->AP_coef_override = float(0.045000f);
		}break;
	case 58786:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58787:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58788:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58797:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58798:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 58799:
		{
			sp->AP_coef_override = float(0.100000f);
		}break;
	case 59092:
		{
			sp->AP_coef_override = float(0.960000f);
		}break;
	case 59161:
		{
			sp->AP_coef_override = float(0.479300f);
		}break;
	case 59163:
		{
			sp->AP_coef_override = float(0.479300f);
		}break;
	case 59164:
		{
			sp->AP_coef_override = float(0.479300f);
		}break;
	case 59170:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 59171:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 59172:
		{
			sp->AP_coef_override = float(0.713900f);
		}break;
	case 60043:
		{
			sp->AP_coef_override = float(0.571400f);
		}break;
	case 60051:
		{
			sp->RAP_coef_override = float(0.140000f);
		}break;
	case 60052:
		{
			sp->RAP_coef_override = float(0.140000f);
		}break;
	case 60053:
		{
			sp->RAP_coef_override = float(0.140000f);
		}break;
	case 60089:
		{
			sp->AP_coef_override = float(0.050000f);
		}break;
	case 61005:
		{
			sp->RAP_coef_override = float(0.400000f);
		}break;
	case 61006:
		{
			sp->RAP_coef_override = float(0.400000f);
		}break;
	case 61295:
		{
			sp->AP_coef_override = float(0.180000f);
		}break;
	case 61299:
		{
			sp->AP_coef_override = float(0.180000f);
		}break;
	case 61300:
		{
			sp->AP_coef_override = float(0.180000f);
		}break;
	case 61301:
		{
			sp->AP_coef_override = float(0.180000f);
		}break;
	case 61384:
		{
			sp->AP_coef_override = float(0.193000f);
		}break;
	case 61650:
		{
			sp->AP_coef_override = float(0.214200f);
		}break;
	case 61654:
		{
			sp->AP_coef_override = float(0.214200f);
		}break;
	case 62124:
		{
			sp->AP_coef_override = float(0.085000f);
		}break;
	case 62606:
		{
			sp->AP_coef_override = float(0.250000f);
		}break;

	default:
		{
			sp->SP_coef_override = 0.0f;
			sp->AP_coef_override = 0.0f;
			sp->RAP_coef_override = 0.0f;
		}break;
	}
}
