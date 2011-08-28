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

// thank god for macros

#include "StdAfx.h"

#define CREATESPELL(caster, info, triggered, aur) \
	new Spell( caster, info, triggered, (aur == NULL ? NULLAURA : aur));

pSpellEffect SpellEffectsHandler[TOTAL_SPELL_EFFECTS] = {
	&Spell::SpellEffectNULL,						// 0 - SPELL_EFFECT_NULL
	&Spell::SpellEffectInstantKill,					// 1 - SPELL_EFFECT_INSTAKILL
	&Spell::SpellEffectSchoolDMG,					// 2 - SPELL_EFFECT_SCHOOL_DAMAGE
	&Spell::SpellEffectDummy,						// 3 - SPELL_EFFECT_DUMMY
	&Spell::SpellEffectNULL,						// 4 - SPELL_EFFECT_PORTAL_TELEPORT
	&Spell::SpellEffectTeleportUnits,				// 5 - SPELL_EFFECT_TELEPORT_UNITS
	&Spell::SpellEffectApplyAura,					// 6 - SPELL_EFFECT_APPLY_AURA
	&Spell::SpellEffectEnvironmentalDamage,			// 7 - SPELL_EFFECT_ENVIRONMENTAL_DAMAGE
	&Spell::SpellEffectPowerDrain,					// 8 - SPELL_EFFECT_POWER_DRAIN
	&Spell::SpellEffectHealthLeech,					// 9 - SPELL_EFFECT_HEALTH_LEECH
	&Spell::SpellEffectHeal,						// 10 - SPELL_EFFECT_HEAL
	&Spell::SpellEffectBind,						// 11 - SPELL_EFFECT_BIND
	&Spell::SpellEffectTeleportUnits,				// 12 - SPELL_EFFECT_PORTAL
	&Spell::SpellEffectNULL,						// 13 - Unused
	&Spell::SpellEffectNULL,						// 14 - Unused
	&Spell::SpellEffectNULL,						// 15 - Unused
	&Spell::SpellEffectQuestComplete,				// 16 - SPELL_EFFECT_QUEST_COMPLETE
	&Spell::SpellEffectWeapondamageNoschool,		// 17 - SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL
	&Spell::SpellEffectResurrect,					// 18 - SPELL_EFFECT_RESURRECT
	&Spell::SpellEffectAddExtraAttacks,				// 19 - SPELL_EFFECT_ADD_EXTRA_ATTACKS
	&Spell::SpellEffectDodge,						// 20 - SPELL_EFFECT_DODGE
	&Spell::SpellEffectNULL,						// 21 - Unused
	&Spell::SpellEffectParry,						// 22 - SPELL_EFFECT_PARRY
	&Spell::SpellEffectBlock,						// 23 - SPELL_EFFECT_BLOCK
	&Spell::SpellEffectCreateItem,					// 24 - SPELL_EFFECT_CREATE_ITEM
	&Spell::SpellEffectWeapon,						// 25 - SPELL_EFFECT_WEAPON
	&Spell::SpellEffectDefense,						// 26 - SPELL_EFFECT_DEFENSE
	&Spell::SpellEffectPersistentAA,				// 27 - SPELL_EFFECT_PERSISTENT_AREA_AURA
	&Spell::SpellEffectSummon,						// 28 - SPELL_EFFECT_SUMMON
	&Spell::SpellEffectLeap,						// 29 - SPELL_EFFECT_LEAP
	&Spell::SpellEffectEnergize,					// 30 - SPELL_EFFECT_ENERGIZE
	&Spell::SpellEffectWeaponDmgPerc,				// 31 - SPELL_EFFECT_WEAPON_PERCENT_DAMAGE
	&Spell::SpellEffectTriggerMissile,				// 32 - SPELL_EFFECT_TRIGGER_MISSILE
	&Spell::SpellEffectOpenLock,					// 33 - SPELL_EFFECT_OPEN_LOCK
	&Spell::SpellEffectTranformItem,				// 34 - SPELL_EFFECT_TRANSFORM_ITEM
	&Spell::SpellEffectApplyAA,						// 35 - SPELL_EFFECT_APPLY_AREA_AURA
	&Spell::SpellEffectLearnSpell,					// 36 - SPELL_EFFECT_LEARN_SPELL
	&Spell::SpellEffectSpellDefense,				// 37 - SPELL_EFFECT_SPELL_DEFENSE
	&Spell::SpellEffectDispel,						// 38 - SPELL_EFFECT_DISPEL
	&Spell::SpellEffectLanguage,					// 39 - SPELL_EFFECT_LANGUAGE
	&Spell::SpellEffectDualWield,					// 40 - SPELL_EFFECT_DUAL_WIELD
	&Spell::SpellEffectJump,						// 41 - SPELL_EFFECT_JUMP
	&Spell::SpellEffectJump,						// 42 - SPELL_EFFECT_JUMP_2
	&Spell::SpellEffectTeleportToCaster,			// 43 - SPELL_EFFECT_TELEPORT_UNITS_FACE_CASTER
	&Spell::SpellEffectSkillStep,					// 44 - SPELL_EFFECT_SKILL_STEP
	&Spell::SpellEffectAddHonor,					// 45 - SPELL_ADD_HONOR
	&Spell::SpellEffectSpawn,						// 46 - SPELL_EFFECT_SPAWN
	&Spell::SpellEffectNULL,						// 47 - SPELL_EFFECT_TRADE_SKILL
	&Spell::SpellEffectNULL,						// 48 - SPELL_EFFECT_STEALTH
	&Spell::SpellEffectDetect,						// 49 - SPELL_EFFECT_DETECT
	&Spell::SpellEffectSummonObject,				// 50 - SPELL_EFFECT_SUMMON_OBJECT
	&Spell::SpellEffectNULL,						// 51 - Unused
	&Spell::SpellEffectNULL,						// 52 - Unused
	&Spell::SpellEffectEnchantItem,					// 53 - SPELL_EFFECT_ENCHANT_ITEM
	&Spell::SpellEffectEnchantItemTemporary,		// 54 - SPELL_EFFECT_ENCHANT_ITEM_TEMPORARY
	&Spell::SpellEffectTameCreature,				// 55 - SPELL_EFFECT_TAMECREATURE
	&Spell::SpellEffectSummonPet,					// 56 - SPELL_EFFECT_SUMMON_PET
	&Spell::SpellEffectLearnPetSpell,				// 57 - SPELL_EFFECT_LEARN_PET_SPELL
	&Spell::SpellEffectWeapondamage,				// 58 - SPELL_EFFECT_WEAPON_DAMAGE
	&Spell::SpellEffectOpenLockItem,				// 59 - SPELL_EFFECT_OPEN_LOCK_ITEM
	&Spell::SpellEffectProficiency,					// 60 - SPELL_EFFECT_PROFICIENCY
	&Spell::SpellEffectSendEvent,					// 61 - SPELL_EFFECT_SEND_EVENT
	&Spell::SpellEffectPowerBurn,					// 62 - SPELL_EFFECT_POWER_BURN
	&Spell::SpellEffectThreat,						// 63 - SPELL_EFFECT_THREAT
	&Spell::SpellEffectTriggerSpell,				// 64 - SPELL_EFFECT_TRIGGER_SPELL
	&Spell::SpellEffectHealthFunnel,				// 65 - SPELL_EFFECT_HEALTH_FUNNEL
	&Spell::SpellEffectPowerFunnel,					// 66 - SPELL_EFFECT_POWER_FUNNEL
	&Spell::SpellEffectHealMaxHealth,				// 67 - SPELL_EFFECT_HEAL_MAX_HEALTH
	&Spell::SpellEffectInterruptCast,				// 68 - SPELL_EFFECT_INTERRUPT_CAST
	&Spell::SpellEffectDistract,					// 69 - SPELL_EFFECT_DISTRACT
	&Spell::SpellEffectPull,						// 70 - SPELL_EFFECT_PULL
	&Spell::SpellEffectPickpocket,					// 71 - SPELL_EFFECT_PICKPOCKET
	&Spell::SpellEffectAddFarsight,					// 72 - SPELL_EFFECT_ADD_FARSIGHT
	&Spell::SpellEffectResetTalents,				// 73 - SPELL_EFFECT_RESET_TALENTS
	&Spell::SpellEffectUseGlyph,					// 74 - SPELL_EFFECT_USE_GLYPH
	&Spell::SpellEffectHealMechanical,				// 75 - SPELL_EFFECT_HEAL_MECHANICAL
	&Spell::SpellEffectSummonObject,				// 76 - SPELL_EFFECT_SUMMON_OBJECT_WILD
	&Spell::SpellEffectScriptEffect,				// 77 - SPELL_EFFECT_SCRIPT_EFFECT
	&Spell::SpellEffectNULL,						// 78 - SPELL_EFFECT_ATTACK
	&Spell::SpellEffectSanctuary,					// 79 - SPELL_EFFECT_SANCTUARY
	&Spell::SpellEffectAddComboPoints,				// 80 - SPELL_EFFECT_ADD_COMBO_POINTS
	&Spell::SpellEffectCreateHouse,					// 81 - SPELL_EFFECT_CREATE_HOUSE
	&Spell::SpellEffectNULL,						// 82 - SPELL_EFFECT_BIND_SIGHT
	&Spell::SpellEffectDuel,						// 83 - SPELL_EFFECT_DUEL
	&Spell::SpellEffectStuck,						// 84 - SPELL_EFFECT_STUCK
	&Spell::SpellEffectSummonPlayer,				// 85 - SPELL_EFFECT_SUMMON_PLAYER
	&Spell::SpellEffectActivateObject,				// 86 - SPELL_EFFECT_ACTIVATE_OBJECT
	&Spell::SpellEffectWMODamage,					// 87 - SPELL_EFFECT_WMO_DAMAGE
	&Spell::SpellEffectWMORepair,					// 88 - SPELL_EFFECT_WMO_REPAIR
	&Spell::SpellEffectChangeWMOState,				// 89 - SPELL_EFFECT_WMO_CHANGE
	&Spell::SpellEffectKillCredit,					// 90 - SPELL_EFFECT_KILL_CREDIT
	&Spell::SpellEffectNULL,						// 91 - SPELL_EFFECT_THREAT_ALL
	&Spell::SpellEffectEnchantHeldItem,				// 92 - SPELL_EFFECT_ENCHANT_HELD_ITEM
	&Spell::SpellEffectNULL,						// 93 - SPELL_EFFECT_SUMMON_PHANTASM
	&Spell::SpellEffectSelfResurrect,				// 94 - SPELL_EFFECT_SELF_RESURRECT
	&Spell::SpellEffectSkinning,					// 95 - SPELL_EFFECT_SKINNING
	&Spell::SpellEffectCharge,						// 96 - SPELL_EFFECT_CHARGE
	&Spell::SpellEffectPlaceTotemsOnBar,			// 97 - SPELL_EFFECT_PLACE_ALL_TOTEMS_ON_BAR
	&Spell::SpellEffectKnockBack,					// 98 - SPELL_EFFECT_KNOCK_BACK
	&Spell::SpellEffectDisenchant,					// 99 - SPELL_EFFECT_DISENCHANT
	&Spell::SpellEffectInebriate,					// 100 - SPELL_EFFECT_INEBRIATE
	&Spell::SpellEffectFeedPet,						// 101 - SPELL_EFFECT_FEED_PET
	&Spell::SpellEffectDismissPet,					// 102 - SPELL_EFFECT_DISMISS_PET
	&Spell::SpellEffectReputation,					// 103 - SPELL_EFFECT_REPUTATION
	&Spell::SpellEffectSummonObjectSlot,			// 104 - SPELL_EFFECT_SUMMON_OBJECT_SLOT1
	&Spell::SpellEffectSummonObjectSlot,			// 105 - SPELL_EFFECT_SUMMON_OBJECT_SLOT2
	&Spell::SpellEffectSummonObjectSlot,			// 106 - SPELL_EFFECT_SUMMON_OBJECT_SLOT3
	&Spell::SpellEffectSummonObjectSlot,			// 107 - SPELL_EFFECT_SUMMON_OBJECT_SLOT4
	&Spell::SpellEffectDispelMechanic,				// 108 - SPELL_EFFECT_DISPEL_MECHANIC
	&Spell::SpellEffectSummonDeadPet,				// 109 - SPELL_EFFECT_SUMMON_DEAD_PET
	&Spell::SpellEffectDestroyAllTotems,			// 110 - SPELL_EFFECT_DESTROY_ALL_TOTEMS
	&Spell::SpellEffectNULL,						// 111 - SPELL_EFFECT_DURABILITY_DAMAGE
	&Spell::SpellEffectNULL,						// 112 - Unused
	&Spell::SpellEffectResurrectNew,				// 113 - SPELL_EFFECT_RESURRECT_NEW
	&Spell::SpellEffectAttackMe,					// 114 - SPELL_EFFECT_ATTACK_ME
	&Spell::SpellEffectNULL,						// 115 - SPELL_EFFECT_DURABILITY_DAMAGE_PCT
	&Spell::SpellEffectSkinPlayerCorpse,			// 116 - SPELL_EFFECT_SKIN_PLAYER_CORPSE
	&Spell::SpellEffectNULL,						// 117 - SPELL_EFFECT_SPIRIT_HEAL
	&Spell::SpellEffectSkill,						// 118 - SPELL_EFFECT_SKILL
	&Spell::SpellEffectApplyPetAura,				// 119 - SPELL_EFFECT_APPLY_PET_AURA
	&Spell::SpellEffectNULL,						// 120 - SPELL_EFFECT_TELEPORT_GRAVEYARD
	&Spell::SpellEffectDummyMelee,					// 121 - SPELL_EFFECT_DUMMYMELEE
	&Spell::SpellEffectNULL,						// 122 - Unused
	&Spell::SpellEffectSendTaxi,					// 123 - SPELL_EFFECT_SEND_TAXI  taxi/flight related (misc value is taxi path id)
	&Spell::SpellEffectPull,						// 124 - SPELL_EFFECT_PULL
	&Spell::SpellEffectNULL,						// 125 - Reduce Threat by % http://www.thottbot.com/?sp=32835
	&Spell::SpellEffectSpellSteal,					// 126 - SPELL_EFFECT_SPELL_STEAL Steal Beneficial Buff (Magic) //http://www.thottbot.com/?sp=30449
	&Spell::SpellEffectProspecting,					// 127 Search 5 ore of a base metal for precious gems.  This will destroy the ore in the process.
	&Spell::SpellEffectApplyAA,						// 128 - Area Aura
	&Spell::SpellEffectApplyAA,						// 129 - Aura Aura
	&Spell::SpellEffectRedirectThreat,				// 130 - Redirect your threat.
	&Spell::SpellEffectNULL,						// 131 - send notifaction like "You have entered a no-fly zone"
	&Spell::SpellEffectPlayMusic,					// 132 - Play music derp
	&Spell::SpellEffectNULL,						// 133 - SPELL_EFFECT_FORGET_SPECIALIZATION
	&Spell::SpellEffectKillCredit,					// 134 - SPELL_EFFECT_KILL_CREDIT misc value is creature entry
	&Spell::SpellEffectSummonPet,					// 135 - I can haz pet?
	&Spell::SpellEffectRestoreHealthPct,			// 136 - Restore Health %
	&Spell::SpellEffectRestoreManaPct,				// 137 - Restore Mana %
	&Spell::SpellEffectDisengage,					// 138 - knockback backwards :3
	&Spell::SpellEffectClearFinishedQuest,			// 139 - remove a finished quest.
	&Spell::SpellEffectTeleportUnits,				// 140 - SPELL_EFFECT_TELEPORT_UNITS
	&Spell::SpellEffectTriggerSpell,				// 141 - Triggers a spell
	&Spell::SpellEffectTriggerSpellWithValue,		// 142 - triggers some kind of "Put spell on target" thing... (dono for sure) http://www.thottbot.com/s40872 and http://www.thottbot.com/s33076
	&Spell::SpellEffectApplyDemonAura,				// 143 - Apply pet aura for summons :3
	&Spell::SpellEffectKnockBack,					// 144 - Spectral Blast
	&Spell::SpellEffectPull,						// SPELL_EFFECT_TRACTOR_BEAM_FROM_DEST - 145
	&Spell::SpellEffectActivateRune,				// 146 - Finish a rune's cooldown
	&Spell::SpellEffectFailQuest,					// 147 - Fail quest.
	&Spell::SpellEffectTriggerSpell,				// 148 - Appears to trigger a spell?
	&Spell::SpellEffectCharge,						// SPELL_EFFECT_CHARGE2
	&Spell::SpellEffectStartQuest,					// SPELL_EFFECT_START_QUEST
	&Spell::SpellEffectTriggerSpell,				// 151 SPELL_EFFECT_TRIGGER_SPELL_2
	&Spell::SpellEffectSummonPlayer,				// 152 - This is RAF summon but we don't support RAF.
	&Spell::SpellEffectCreatePet,					// 153 SPELL_EFFECT_CREATE_PET
	&Spell::SpellEffectNULL,						// 154 unused
	&Spell::SpellEffectTitanGrip,					// 155 - Titan's Grip
	&Spell::SpellEffectAddPrismaticSocket,			// 156 - Add Socket
	&Spell::SpellEffectCreateRandomItem,			// 157 - create/learn random item/spell for profession
	&Spell::SpellEffectMilling,						// 158 - milling
	&Spell::SpellEffectAllowPetRename,				// 159 - allow rename pet once again
	&Spell::SpellEffectTriggerSpell,				// 160 - Only 1 spell has this and it has a trigger spell so fuck it!
	&Spell::SpellEffectSetTalentSpecsCount,			// 161 - Sets number of talent specs available to the player
	&Spell::SpellEffectActivateTalentSpec,			// 162 - Activates one of talent specs
	&Spell::SpellEffectNULL,						// 163 Unused
	&Spell::SpellEffectRemoveAura,					// 164
};

void Spell::SpellEffectNULL(uint32 i)
{
	if(sLog.IsOutDevelopement())
		printf("Unhandled spell effect %u in spell %u.\n",GetSpellProto()->Effect[i],GetSpellProto()->Id);
	else
		Log.DebugSpell("Spell","Unhandled spell effect %u in spell %u.",GetSpellProto()->Effect[i],GetSpellProto()->Id);
}

void Spell::SpellEffectInstantKill(uint32 i)
{
	if( unitTarget == NULL || !unitTarget->isAlive() || u_caster == NULL)
		return;

	//Sacrifice: if spell caster has "void walker" pet, pet dies and spell caster gets a
	/*Sacrifices the Voidwalker, giving its owner a shield that will absorb
	305 damage for 30 sec. While the shield holds, spellcasting will not be \
	interrupted by damage.*/

	/*
	Demonic Sacrifice

	When activated, sacrifices your summoned demon to grant you an effect that lasts
	30 minutes. The effect is canceled if any Demon is summoned.
	Imp: Increases your Fire damage by 15%.
	Voidwalker: Restores 3% of total Health every 4 sec.
	Succubus: Increases your Shadow damage by 15%.
	Felhunter: Restores 2% of total Mana every 4 sec.

	When activated, sacrifices your summoned demon to grant you an effect that lasts $18789d.  The effect is canceled if any Demon is summoned.

	Imp: Increases your Fire damage by $18789s1%.

	Voidwalker: Restores $18790s1% of total Health every $18790t1 sec.

	Succubus: Increases your Shadow damage by $18791s1%.

	Felhunter: Restores $18792s1% of total Mana every $18792t1 sec.

	*/
	uint32 spellId = GetSpellProto()->Id;

	switch(spellId)
	{
	case 7814:
	case 7815:
	case 7816:
	case 7876:
	case 7877:
	case 7878:
	case 11778:
	case 11779:
	case 11780:
	case 15968:
	case 15969:
	case 18128:
	case 18129:
	case 20398:
	case 20399:
	case 20400:
	case 20401:
	case 20402:
		{
		}break;

	case 18788: //Demonic Sacrifice (508745)
		{
			if( p_caster )
			{
				uint32 spellid1 = 0;
				switch(unitTarget->GetEntry())
				{
					case 416: //Imp
					{
						spellid1 = 18789;
					}break;
					case 417: //Felhunter
					{
						spellid1 = 18792;
					}break;
					case 1860: //VoidWalker
					{
						spellid1 = 18790;
					}break;
					case 1863: //Succubus
					{
						spellid1 = 18791;
					}break;
					case 17252: //felguard
					{
						spellid1 = 35701;
					}break;
				}
				//now caster gains this buff
				if (spellid1 && spellid1 != 0)
				{
					u_caster->CastSpell(u_caster, dbcSpell.LookupEntry(spellid1), true);
				}
			}
		}break;

	case 7812: //Sacrifice Voidwalker
	case 19438:
	case 19440:
	case 19441:
	case 19442:
	case 19443:
	case 27273:
		{
			if( unitTarget->GetEntry() != 1860 )
				return;
		}break;
	case 29364:	//Encapsulate Voidwalker
		{
		if( unitTarget->GetEntry() != 16975 )
				return;
		}break;
	case 33974: //Power Burn for each Point consumed mana (Effect1) target get damage(Effect3) no better idea :P
		{
			unitTarget->GetPowerType() == POWER_TYPE_RAGE ? m_caster->DealDamage(unitTarget, GetSpellProto()->EffectBasePoints[0], 0, 0, spellId) : m_caster->DealDamage(unitTarget, GetSpellProto()->EffectBasePoints[1], 0, 0, spellId);
			return;
		}break;
	case 36484: //Mana Burn same like Power Burn
		{
			m_caster->DealDamage(unitTarget, GetSpellProto()->EffectBasePoints[0], 0, 0, spellId);
			return;
		}break;
	case 37056: //Kill Legion Hold Infernals
		{
			if( unitTarget->GetEntry() != 21316 )
				return;
		}break;
	case 40105: //Infusion (don't know why this should kill anything makes no sence)
		{
			return;
		}break;
	case 43135: //Bubbling Ooze
		{
			return;
		}break;
	case 41626:	//Destroy Spirit
	case 44659:
		{
			if( unitTarget->GetEntry() != 23109 )
				return;
		}break;
	case 45259: //Karazhan - Kill Chest Bunny
		{
			if( unitTarget->GetEntry() != 25213 )
				return;
		}break;
	case 48743:
		{
			if(p_caster == NULL || p_caster->GetSummon() == NULL)
				return;
			m_caster->DealDamage(p_caster->GetSummon(), p_caster->GetSummon()->GetUInt32Value(UNIT_FIELD_HEALTH), 0, 0, 0);
			WorldPacket data(SMSG_SPELLINSTAKILLLOG, 200);
			data << m_caster->GetGUID() << p_caster->GetSummon()->GetGUID() << spellId;
			m_caster->SendMessageToSet(&data, true);
			return; //We do not want the generated targets!!!!!!!!!!
		}break;

	}

	switch( GetSpellProto()->NameHash )
	{
	case SPELL_HASH_SACRIFICE:
		{
			if( !u_caster->IsPet() )
				return;

			TO_PET(u_caster)->Dismiss( true );
			return;

		}break;
	case SPELL_HASH_DIVINE_INTERVENTION:
	case SPELL_HASH_DEMONIC_SACRIFICE:
		{
		}break;
	default:
		{
			// moar cheaters
			if( p_caster == NULL || (u_caster != NULL && u_caster->IsPet() ))
				return;

			if( p_caster->GetSession()->GetPermissionCount() == 0 )
				return;
		}break;
	}
	m_caster->DealDamage(unitTarget, unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH), 0, 0, 0);
	WorldPacket data(SMSG_SPELLINSTAKILLLOG, 200);
	data << m_caster->GetGUID() << unitTarget->GetGUID() << spellId;
	m_caster->SendMessageToSet(&data, true);
}

void Spell::SpellEffectSchoolDMG(uint32 i) // dmg school
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	if(unitTarget->SchoolImmunityList[GetSpellProto()->School])
	{
		SendCastResult(SPELL_FAILED_IMMUNE);
		return;
	}

	if(p_caster != NULL && playerTarget != NULL && p_caster != playerTarget)
	{
		if( playerTarget->IsPvPFlagged() )
			u_caster->SetPvPFlag();
		if( playerTarget->IsFFAPvPFlagged() )
			u_caster->SetFFAPvPFlag();
	}

	uint32 dmg = damage;
	bool static_damage = false;
	uint32 AdditionalCritChance = 0;

	if(GetSpellProto()->EffectChainTarget[i])//chain
	{
		if( GetSpellProto()->Id == 53595 ) // Hammer of the righteous
		{
			if( p_caster == NULL )
				return;

			float WMIN = 1.0f;
			float WMAX = 2.0f;
			float MWS = 2.0f;
			Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
			if( it && it->GetProto() )
			{
				WMIN = it->GetProto()->Damage[0].Min;
				WMAX = it->GetProto()->Damage[0].Max;
				MWS = it->GetProto()->Delay / 1000.0f;
			}	// Crow: Do not randomize 0, it will crash.
			dmg = float2int32(((WMIN + RandomUInt(WMAX-WMIN)) / (2 * MWS)) * 4 + (u_caster->GetStrength() / 5.5f) * 4 + (u_caster->GetAP() / 14) * 4);
		}
		else
		{
			int32 reduce = (int32)(GetSpellProto()->dmg_multiplier[i] * 100.0f);

			if(reduce && chaindamage)
			{
				if(GetSpellProto()->SpellGroupType && u_caster)
				{
					SM_FIValue(u_caster->SM[SMT_JUMP_REDUCE][1],&reduce,GetSpellProto()->SpellGroupType);
				}
				chaindamage = chaindamage * reduce / 100;
			}
		}
	}
	else
	{
		switch(GetSpellProto()->Id)
		{
			/************************
			*		WARRIOR			*
			************************/
		case 23922: case 23923: // Shield Slam - damage is increased by block value
		case 23924: case 23925: case 25258:
		case 30356: case 47487: case 47488:
			{
				if( p_caster != NULL )
				{
					Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
					if( it && it->GetProto() && it->GetProto()->InventoryType == INVTYPE_SHIELD )
					{
						dmg += p_caster->GetUInt32Value(PLAYER_SHIELD_BLOCK);
					}
				}
			}break;

		case 34428: // Victory Rush
			{
				if(u_caster->HasFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_VICTORIOUS))
					u_caster->RemoveFlag(UNIT_FIELD_AURASTATE, AURASTATE_FLAG_VICTORIOUS);
			}break;

			// Heroic Strike, commented ones don't have bonus.
		/*case 78: case 284: case 285: case 1608: case 11564:
		case 11565: case 11566: case 11567: case 25286:*/
		case 29707: case 30324: case 47449: case 47450:
			{
				if(unitTarget->IsDazed())
				{
					switch(GetSpellProto()->Id)
					{ // This info isn't in the dbc files.....
					case 29707:
						{
							dmg += 81.9;
						}break;

					case 30324:
						{
							dmg += 110.95;
						}break;

					case 47449:
						{
							dmg += 151.2;
						}break;

					case 47450:
						{
							dmg += 173.25;
						}break;
					}
				}
			}break;

			// BEHOLD, CLEAVE!!!!!!!!!!!!
		case 845: case 7369: case 11608: case 11609:
		case 20569: case 25231: case 47519: case 47520:
			{
				if(p_caster != NULL)
				{
					Item* itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(p_caster->HasAura(12329))
						dmg += (((itm->GetProto()->Damage[0].Min + itm->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i]) * 1.4;
					else if(p_caster->HasAura(12950))
						dmg += (((itm->GetProto()->Damage[0].Min + itm->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i]) * 1.8;
					else if(p_caster->HasAura(20496))
						dmg += (((itm->GetProto()->Damage[0].Min + itm->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i]) * 2.2;
					else
						dmg += ((itm->GetProto()->Damage[0].Min + itm->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i];
				}
			}break;

			// Slam
		case 1464: case 8820: case 11604: case 11605:
		case 25241: case 25242: case 47474: case 47475:
			{
				if(p_caster != NULL)
				{
					Item* itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					if(itm != NULL)
						dmg = ((itm->GetProto()->Damage[0].Min + itm->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i];
				}
			}break;

		case 59653: // Damage Shield
			{
				if(p_caster != NULL)
				{
					Item* shield = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND);
					if(shield != NULL && shield->GetProto() && shield->GetProto()->InventoryType == INVTYPE_SHIELD)
						if(p_caster->Damageshield_amount)
							dmg = p_caster->Damageshield_amount;
						else
							return; // No damage shield amount is impossible.
					else
						return; // No Shield = No Damage.
				}
			}break;

			/************************
			*		PALADIN			*
			************************/
		case 20187: // Righteousness
		case 20425: // Command
		case 31804: // Vengeance
		case 53733: // Corruption
		case 54158: // Light/Wisdom/Justice
			{
				if(p_caster != NULL)
				{
					if( p_caster->HasAura(34258) )
						p_caster->CastSpell(p_caster, 34260, true);

					if(p_caster->HasAura(53696) || p_caster->HasAura(53695))
						p_caster->CastSpell(unitTarget, 68055, true);

					// Damage Calculations:
					switch(GetSpellProto()->Id)
					{
					case 31804:
						{
							p_caster->CastSpell(unitTarget, 31803, true);
							dmg = (1+uint32(uint32(0.22f*p_caster->GetDamageDoneMod(SCHOOL_HOLY))+uint32(0.14f*p_caster->GetAP())));
							if(unitTarget->HasActiveAura(31803))
							{
								uint8 stacksize = 1;
								Aura* curse = unitTarget->FindActiveAura(31803);
								if(curse != NULL)
									stacksize = curse->stackSize;
								dmg += (dmg*(0.1f * stacksize));
							}
						}break;
					case 53733:
						{
							p_caster->CastSpell(unitTarget, 53742, true);
							dmg = (1+uint32(uint32(0.22f*p_caster->GetDamageDoneMod(SCHOOL_HOLY))+uint32(0.14f*p_caster->GetAP())));
							if(unitTarget->HasAura(31803))
							{
								uint8 stacksize = 1;
								Aura* curse = unitTarget->FindActiveAura(31803);
								if(curse != NULL)
									stacksize = curse->stackSize;
								dmg += (dmg*(0.1f * stacksize));
							}
						}break;
					case 20187: // Righteousness
						dmg = (1+uint32(0.22f*p_caster->GetAP())+uint32(0.32f*p_caster->GetDamageDoneMod(SCHOOL_HOLY)));
						break;
					case 20425: // Command
						dmg = (1+uint32(0.27f*p_caster->GetAP())+uint32(0.21f*p_caster->GetDamageDoneMod(SCHOOL_HOLY)));
						break;
					case 54158: // Light/Wisdom/Justice
						dmg = (1+uint32(0.16f*p_caster->GetAP())+uint32(0.25f*p_caster->GetDamageDoneMod(SCHOOL_HOLY)));
						break;
					}

					if( p_caster->HasAura(37186) )
						dmg += 33;
				}
			}break;

		case 53600: case 61411: // Shield of Righteousness
			{
				if( p_caster != NULL )
				{
					Item* it = TO_ITEM(p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_OFFHAND));
					if( it && it->GetProto() && it->GetProto()->InventoryType == INVTYPE_SHIELD )
					{
						dmg += float2int32(1.3f * p_caster->GetUInt32Value(PLAYER_SHIELD_BLOCK));
					}
				}
			}break;

		case 25742: // Seal of Righteousness damage effect
			{
				if( p_caster != NULL )
				{
					dmg *= (p_caster->GetUInt32Value(UNIT_FIELD_BASEATTACKTIME)/1000);
					if(p_caster->HasAura(56414))
						dmg += uint32((float)dmg*0.1f);
				}
			}break;

		case 9799: case 25988: // Eye for an Eye damage check. Calculates itself, just doesn't limit.
			{
				if( p_caster != NULL)
				{
					if(dmg > (p_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2))
						dmg = (p_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH)/2);
				}
			}break;

			/************************
			*		HUNTER			*
			************************/
		case 3044: // Arcane shot
		case 14281: case 14282: case 14283: case 14284: case 14285:
		case 14286: case 14287: case 27019: case 49044: case 49045:
			{
				if( p_caster != NULL )
				{
					if(p_caster->HasAurasOfNameHashWithCaster(SPELL_HASH_RAPID_KILLING, p_caster))
						dmg += float2int32(dmg * (GetSpellProto()->RankNumber * 0.1f));
				}
			}break;

		case 19434: // Aimed shot, increase damage with Rapid Killing.
		case 20900: case 20901: case 20902: case 20903:
		case 20904: case 27065: case 49049: case 49050:
			{
				if( p_caster != NULL )
				{
					Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					dmg = ((it->GetProto()->Damage[0].Min + it->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i];

					if(p_caster->HasAurasOfNameHashWithCaster(SPELL_HASH_RAPID_KILLING, u_caster)) //Aimed/Auto shot deals Y% extra damage with rapid killing buff
						dmg += float2int32(dmg * (GetSpellProto()->RankNumber * 0.1f));
				}
			}break;

		case 53209: // Chimera shot
			{
				if( p_caster != NULL )
				{
					Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					dmg = ((it->GetProto()->Damage[0].Min + it->GetProto()->Damage[0].Max) * 0.2f) * 1.25;

					if(p_caster->HasAurasOfNameHashWithCaster(SPELL_HASH_RAPID_KILLING, u_caster))
						dmg += float2int32(dmg * (GetSpellProto()->RankNumber * 0.1f));
				}
			}break;

		case 56641: // Steady Shot
		case 34120: case 49051: case 49052:
			{
				if( p_caster != NULL )
				{
					Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
					ItemPrototype* ip = ItemPrototypeStorage.LookupEntry(p_caster->GetUInt32Value(PLAYER_AMMO_ID));
					float ammodmg;
					uint32 stundmg;
					float bowdmg;
					// Stun Damage
					if(unitTarget->IsDazed())
					{
						stundmg = p_caster->GetRAP()/10 + GetSpellProto()->EffectBasePoints[i] + GetSpellProto()->EffectBasePoints[i + 1];
					}
					else
						stundmg = p_caster->GetRAP()/10 + GetSpellProto()->EffectBasePoints[i];

					// Bow Damage
					if(it)
						bowdmg = (it->GetProto()->Damage[0].Min + it->GetProto()->Damage[0].Max) * 0.2f; //+unmodified weapon damage
					else
						bowdmg = 0;

					// Ammo Damage
					if(ip)
						ammodmg = (ip->Damage[0].Min + ip->Damage[0].Max) * 0.2f; //+unmodified ammo damage
					else
						ammodmg = 0;
					// Actual damage :D
					dmg = float2int32(ammodmg + bowdmg) + stundmg;
				}
			}break;

			/************************
			*		ROGUE			*
			************************/

		case 1776:	// Gouge: turns off your combat
			{
				if( p_caster != NULL )
				{
					p_caster->EventAttackStop();
					p_caster->smsg_AttackStop( unitTarget );
				}
			}break;

		case 2094:	// Blind: turns off your attack
			{
				if( p_caster != NULL )
				{
					p_caster->EventAttackStop();
					p_caster->smsg_AttackStop( unitTarget );
				}
			}break;

		case 2098: // Eviscerate, uses combo points, what a bitch.
		case 6760: case 6761: case 6762: case 8623: case 8624: case 11299:
		case 11300: case 31016: case 26865: case 48667: case 48668:
			{
				if( p_caster != NULL )
				{
					uint32 perc = p_caster->m_comboPoints * 3;
					perc += (RandomUInt(5) * p_caster->m_comboPoints);
					dmg += float2int32(p_caster->GetAP() * ( perc * 0.01f ));//UINT =+ INT + FLOAT = 0 LOL
					if( p_caster->HasDummyAura(SPELL_HASH_CUT_TO_THE_CHASE) )
					{
						Aura* aur = p_caster->FindPositiveAuraByNameHash(SPELL_HASH_SLICE_AND_DICE);
						if( aur )
						{
							aur->SetDuration(21000);
							aur->SetTimeLeft(21000);
						}
					}
				}
			}break;

		case 32645: // Envenom
		case 32684: case 57992: case 57993:
			{
				if( p_caster != NULL )
				{
					// Lets find all deadly poisons...
					uint32 dosestoate = 0;
					uint32 doses = unitTarget->GetPoisonDosesCount( POISON_TYPE_DEADLY );
					if( doses )
					{
						if (doses <= uint32(p_caster->m_comboPoints))
							dosestoate = doses;
						else
							dosestoate = p_caster->m_comboPoints;

						uint32 bpdamage = GetSpellProto()->EffectBasePoints[i] + 1;
						dmg = ( bpdamage * dosestoate) + float2int32(p_caster->GetAP() * (0.07f * dosestoate));
						// Remove deadly poisons
						for(uint32 x = MAX_POSITIVE_AURAS; x < MAX_AURAS; ++x)
						{
							if(unitTarget->m_auras[x] && unitTarget->m_auras[x]->m_spellProto->poison_type == POISON_TYPE_DEADLY )
							{
								if (dosestoate >= doses)
									unitTarget->m_auras[x]->Remove();
								else
									unitTarget->m_auras[x]->ModStackSize(-int32(dosestoate));
								break;
							}
						}
					}
				}
			}break;

			/***********************/
			/*******POISONS*********/
			/***********************/
		case 8680: // Instant Poison 1
			{
				if(u_caster != NULL)
					dmg = (13+RandomUInt(4));
			}break;
		case 8685: // Instant Poison 2
			{
				if(u_caster != NULL)
					dmg = (21+RandomUInt(4));
			}break;
		case 8689: // Instant Poison 3
			{
				if(u_caster != NULL)
					dmg = (30+RandomUInt(8));
			}break;
		case 11335: // Instant Poison 4
			{
				if(u_caster != NULL)
					dmg = (45+RandomUInt(12));
			}break;
		case 11336: // Instant Poison 5
			{
				if(u_caster != NULL)
					dmg = (62+RandomUInt(18));
			}break;
		case 11337: // Instant Poison 6
			{
				if(u_caster != NULL)
					dmg = (76+RandomUInt(24));
			}break;
		case 26890: // Instant Poison 7
			{
				if(u_caster != NULL)
					dmg = (161+RandomUInt(54));
			}break;
		case 57964: // Instant Poison 8
			{
				if(u_caster != NULL)
					dmg = (245+RandomUInt(82));
			}break;
		case 57965: // Instant Poison 9
			{
				if(u_caster != NULL)
					dmg = (300+RandomUInt(100));
			}break;

			/************************
			*		PRIEST			*
			************************/

		case 32379: // Shadow Word Death
		case 32996: case 48157: case 48158:
			{
				if( p_caster != NULL )
				{
					p_caster->CastSpell(u_caster, 32409, true);
				}
			}break;

			/************************
			*		DEATHKNIGHT		*
			************************/
			/************************
			*		SHAMAN			*
			************************/
			/************************
			*		MAGE			*
			************************/
		case 30455: // Ice Lance
		case 42913: case 42914:
			{
				if( unitTarget->HasFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_FROZEN) || unitTarget->m_frozenTargetCharges > 0 )
					dmg *= 3;
			}break;

		case 34913: // Fire Armor no longer has Static Damage
		case 43043: case 43044:
			{
				dmg = GetSpellProto()->EffectBasePoints[i] + 1;
			}break;

			/************************
			*		WARLOCK			*
			************************/
		case 29722:	// Incinerate deals x-x extra damage if the target is affected by immolate
		case 32231: case 47837: case 47838:
			{
				if( unitTarget->GetAuraSpellIDWithNameHash( SPELL_HASH_IMMOLATE ) )
				{
					// Random extra damage
					uint32 extra_dmg = 89 + (GetSpellProto()->RankNumber * 11) + RandomUInt(GetSpellProto()->RankNumber * 11);
					dmg += extra_dmg;
				}
			}break;

		case 47897: case 61290: // Shadowflame
			{
				uint32 spellid = ( GetSpellProto()->RankNumber == 1 ) ? 47960 : 61291;
				SpellEntry* SpellInfo = dbcSpell.LookupEntry( spellid );
				Spell* sp (new Spell( u_caster, SpellInfo, true, NULLAURA ));
				SpellCastTargets tgt;
				tgt.m_unitTarget = unitTarget->GetGUID();
				sp->prepare( &tgt );
			}break;

		case 17962:
			{
				Aura* pAura;
				for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
				{
					pAura = unitTarget->m_auras[i];
					if( pAura && ( pAura->m_spellProto->NameHash == SPELL_HASH_IMMOLATE || pAura->m_spellProto->NameHash == SPELL_HASH_SHADOWFLAME ))
					{
						if( u_caster->GetDummyAura(SPELL_HASH_FIRE_AND_BRIMSTONE) && pAura->GetTimeLeft() <= 5 )
							AdditionalCritChance += u_caster->GetDummyAura(SPELL_HASH_FIRE_AND_BRIMSTONE)->RankNumber * 5;

						unitTarget->RemoveAuraBySlot(i);
						break;
					}
				}
			}break;

			/************************
			*		DRUID			*
			************************/
		case 22570:	case 49802: // Maim: turns off your attack
			{
				if( p_caster != NULL )
				{
					p_caster->EventAttackStop();
					p_caster->smsg_AttackStop( unitTarget );
				}
			}break;

		case 22568: case 22827:  //Ferocious Bite dmg correction
		case 22828: case 22829: case 31018:
		case 24248: case 48576: case 48577:
			{
				if( p_caster != NULL )
				{
					int32 usedpower = std::max(p_caster->GetUInt32Value(UNIT_FIELD_POWER4), (uint32)30);
					dmg += float2int32( ( p_caster->m_comboPoints * p_caster->GetAP() * 0.07f ) + ( (usedpower * GetSpellProto()->dmg_multiplier[1] + p_caster->GetAP()) / 410.0f ) );

					p_caster->ModUnsigned32Value(UNIT_FIELD_POWER4, -usedpower);
				}
			}break;

		case 1822: case 1823: case 1824: // Bear Rake
		case 9904: case 27003: case 48573: case 48574:
			{
				if( p_caster != NULL )
				{
					dmg += uint32(float(p_caster->GetAP())/100);
				}
			}break;

		case 779: case 780: case 769: case 9754: case 9908: // Bear Swipe
		case 26997: case 48561: case 48562: // Bear Swipe
			{
				if( p_caster != NULL )
				{
					dmg = (p_caster->GetAP() * 0.07f);
				}
			}break;

		case 62078: // Cat Swipe
			{
				if( p_caster != NULL )
				{
					// Get weapon damage.
					Item* it = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					float wpdmg;
					if(it)
						wpdmg = (it->GetProto()->Damage[0].Min + it->GetProto()->Damage[0].Max) * 0.2f;
					else
						wpdmg = (p_caster->GetUInt32Value(UNIT_FIELD_MINDAMAGE) + p_caster->GetUInt32Value(UNIT_FIELD_MAXDAMAGE) * 0.2f);

					// Set damage. "Swipe nearby enemies, inflicting 250% weapon damage." This should work :|
					dmg = wpdmg * 2.5;
				}
			}break;

		case 16857: // Faerie Fire(Feral)
			{
				if( p_caster != NULL )
				{
					dmg = (p_caster->GetAP() * 0.15) + 1;
				}
			}break;
			/************************
			*		NON-CLASS		*
			************************/
		case 50811: case 61547: case 61546: case 33654: case 50810: case 33671: // Damage based on distance
			{
				if( u_caster != NULL)
				{
					float dist = u_caster->CalcDistance( TO_OBJECT( unitTarget ) );
					if( dist <= 20.0f && dist >= 0.0f )
						dmg = float2int32( -450 * dist + damage );
				}
			}break;

			// Cataclysmic Bolt
		case 38441:
			{
				dmg = unitTarget->GetMaxHealth() / 2;
				break;
			}

			// Thundercrash
		case 25599:
			{
				dmg = unitTarget->GetHealth() / 2;
				if(dmg < 200)
					dmg = 200;
				break;
			}

			// Fatal Attraction
		case 41001:
			{
				if(unitTarget->HasAura(43690))// Saber Lash
					dmg = 0;
			}break;

			// Tympanic Tantrum
		case 62775:
			{
				dmg = unitTarget->GetMaxHealth() / 10;
			}break;

		case 53353:
			{
				dmg += forced_basepoints[0];
			}break;
		}
	}

	if(GetSpellProto()->isAOE && dmg < 0)
		dmg /= m_hitTargetCount;

	// check for no more damage left (chains)
	if( dmg < 0 )
		return;

	// Todo: Add a config option, and check for magic buffs like Tenacity.
	//if((p_caster != NULL) && dmg > 25000)
		//dmg = 10000;

	// stealthed stuff
	if( m_projectileWait && unitTarget->InStealth() )
		return;

	if(GetSpellProto()->speed > 0)
	{
		m_caster->SpellNonMeleeDamageLog(unitTarget,GetSpellProto()->Id, dmg, pSpellId==0, false, false, AdditionalCritChance);
	}
	else
	{
		if( GetType() == SPELL_DMG_TYPE_MAGIC )
		{
			m_caster->SpellNonMeleeDamageLog( unitTarget, GetSpellProto()->Id, dmg, pSpellId == 0, static_damage, false, AdditionalCritChance );
		}
		else
		{
			if( u_caster != NULL )
			{
				uint32 _type;
				if( GetType() == SPELL_DMG_TYPE_RANGED )
					_type = RANGED;
				else
				{
					if (GetSpellProto()->Flags4 & FLAGS4_OFFHAND)
						_type =  OFFHAND;
					else
						_type = MELEE;
				}

				u_caster->Strike( unitTarget, _type, GetSpellProto(), 0, 0, dmg, pSpellId != 0, true );
			}
		}
	}
}

void Spell::SpellEffectDummy(uint32 i) // Dummy(Scripted events)
{
	bool hashed = true;
	uint32 spellId = GetSpellProto()->Id;

	switch( GetSpellProto()->NameHash )
	{
	case SPELL_HASH_STEADY_SHOT:
		{
			if( p_caster != NULL && unitTarget != NULL )
				if ( unitTarget->IsDazed() )
					p_caster->Strike( unitTarget, RANGED, NULL, 0, 0, damage, false, false );
		}break;

	case SPELL_HASH_SLAM:
		{
			if(p_caster != NULL && unitTarget != NULL )
				p_caster->Strike( unitTarget, MELEE, NULL, damage, 0, 0, false, false);
		}break;

	case SPELL_HASH_SAVAGE_ROAR:
		{
			if(p_caster != NULL && unitTarget != NULL )
				p_caster->Strike( unitTarget, MELEE, GetSpellProto(), 0, 0, 0, false, false);
		}break;

	case SPELL_HASH_HOLY_SHOCK:
		{
			if( p_caster != NULL && unitTarget != NULL )
			{
				uint32 hostileSpell = 0;
				uint32 friendlySpell = 0;
				switch( GetSpellProto()->Id )
				{
				case 20473: //Rank 1
					{
						hostileSpell = 25912;
						friendlySpell = 25914;
					}break;
				case 20929: //Rank 2
					{
						hostileSpell = 25911;
						friendlySpell = 25913;
					}break;
				case 20930: //Rank 3
					{
						hostileSpell = 25902;
						friendlySpell = 25903;
					}break;
				case 27174: //Rank 4
					{
						hostileSpell = 27176;
						friendlySpell = 27175;
					}break;
				case 33072: //Rank 5
					{
						hostileSpell = 33073;
						friendlySpell = 33074;
					}break;
				case 48824: //Rank 6
					{
						hostileSpell = 48822;
						friendlySpell = 48820;
					}break;
				case 48825: //Rank 7
					{
						hostileSpell = 48823;
						friendlySpell = 48821;
					}break;
				default:
					{
						hostileSpell = 48823;
						friendlySpell = 48821;
					}break;
				}

				// Do holy damage
				if(p_caster->DuelingWith != NULL && p_caster->DuelingWith == unitTarget)
					p_caster->CastSpell(unitTarget, hostileSpell, true);
				else if(p_caster->DuelingWith != NULL && unitTarget->IsPet() && (p_caster->DuelingWith == (TO_PET(unitTarget)->GetOwner())))
					p_caster->CastSpell(unitTarget, hostileSpell, true);
				else if(isHostile(p_caster, unitTarget))
					p_caster->CastSpell(unitTarget, hostileSpell, true);
				else if(isCombatSupport(p_caster, unitTarget))
					p_caster->CastSpell(unitTarget, friendlySpell, true);
				else
					p_caster->CastSpell(p_caster, friendlySpell, true);
			}
		}break;

	case SPELL_HASH_PENANCE:
		{
			if( p_caster != NULL && unitTarget != NULL )
			{
				uint32 hostileSpell[] = {0, 0};
				uint32 friendlySpell[] = {0, 0};
				switch( GetSpellProto()->Id )
				{
					case 47540: //Rank 1
						hostileSpell[0] = 47666;
						hostileSpell[1] = 47758;

						friendlySpell[0] = 47750;
						friendlySpell[1] = 47757;
						break;
					case 53005:
						hostileSpell[0] = 52998;
						hostileSpell[1] = 53001;

						friendlySpell[0] = 52983;
						friendlySpell[1] = 52986;
						break;
					case 53006:
						hostileSpell[0] = 52999;
						hostileSpell[1] = 53002;

						friendlySpell[0] = 52984;
						friendlySpell[1] = 52987;
						break;
					case 53007:
						hostileSpell[0] = 53000;
						hostileSpell[1] = 53003;

						friendlySpell[0] = 52985;
						friendlySpell[1] = 52988;
						break;
				}
				if( isAttackable(p_caster, unitTarget) ) // Do holy damage
				{
					// First tick is instant.
					p_caster->CastSpell(unitTarget, hostileSpell[0], true);
					p_caster->CastSpell(unitTarget, hostileSpell[1], false);
				}
				else // Heal
				{
					p_caster->CastSpell(unitTarget, friendlySpell[0], true);
					p_caster->CastSpell(unitTarget, friendlySpell[1], false);
				}
			}
		}break;

	case SPELL_HASH_STARFALL:
		{
			if( p_caster != NULL)
			{
				SpellEntry* spellInfo= dbcSpell.LookupEntryForced( damage );
				Spell* sp (new Spell(p_caster, spellInfo, true, NULLAURA));
				SpellCastTargets targets;
				sp->FillAllTargetsInArea(i,m_caster->GetPositionX(),m_caster->GetPositionY(),m_caster->GetPositionZ(),GetRadius(i));
				sp->prepare(&targets);
			}
		}break;
	case SPELL_HASH_EXPOSE_ARMOR:
	case SPELL_HASH_KIDNEY_SHOT:
		{
			if(p_caster == NULL)
				return;

			p_caster->NullComboPoints();
		}break;
	default:
		hashed = false;
		break;
	}

	if(hashed == true)
		return;

	switch(spellId)
	{
		/*****************************************
		*	Class Spells
		*****************************************/
		/*************************
		* WARRIOR SPELLS
		*************************/
	case 60970: // Heroic Fury
		{
			if( p_caster == NULL)
				return;

			if( p_caster->HasSpell( 20252 ) )
					p_caster->ClearCooldownForSpell( 20252 );
		}break;

	case 50227: // Sword and Board
		{
			if( p_caster == NULL)
				return;

			uint32 ClearSpellId[8] =
			{
				23922,  /* Shield Slam - Rank 1 */
				23923,  /* Shield Slam - Rank 2 */
				23924,  /* Shield Slam - Rank 3 */
				23925,  /* Shield Slam - Rank 4 */
				25258,  /* Shield Slam - Rank 5 */
				30356,  /* Shield Slam - Rank 6 */
				47487,  /* Shield Slam - Rank 7 */
				47488,  /* Shield Slam - Rank 8 */
			};

			for(i = 0; i < 8; i++)
			{
				if( p_caster->HasSpell( ClearSpellId[i] ) )
					p_caster->ClearCooldownForSpell( ClearSpellId[i] );
			}
		}break;

	// warrior - slam
	case 1464:
	case 8820:
	case 11604:
	case 11605:
	case 25241:
	case 25242:
	case 47474:
	case 47475:
			{
				if(p_caster != NULL)
				{
					Item* itm = p_caster->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
					damage = ((itm->GetProto()->Damage[0].Min + itm->GetProto()->Damage[0].Max) * 0.2f) + GetSpellProto()->EffectBasePoints[i];
				}
		}break;

    //warrior - thunder clap
	case 6343:
	case 8198:
	case 8204:
	case 8205:
	case 11580:
	case 11581:
	case 25264:
	case 47501:
	case 47502:
 			{
 				if(u_caster)
					damage = float2int32((GetSpellProto()->EffectBasePoints[0]+1)+ u_caster->GetAP() * 0.20f);
 			}break;

	case 5308:
	case 20658:
	case 20660:
	case 20661:
	case 20662:
	case 25234:
	case 25236:
	case 47470:
	case 47471:// Execute
		{
			if( u_caster == NULL|| !u_caster->IsInWorld() || !unitTarget || !unitTarget->IsInWorld() || !GetSpellProto())
				return;

			int32 value = GetSpellProto()->EffectBasePoints[i]+1;
			int32 rageUsed = p_caster->GetUInt32Value(UNIT_FIELD_POWER2);

			int32 rageLeft = 0; // We use all available rage by default
			Aura* suddenDeath = NULL;
			suddenDeath = u_caster->FindActiveAura(52437);

			if(suddenDeath != NULL && unitTarget->GetHealthPct() > 20)
			{
				SpellEntry * sd = NULL;
				sd = dbcSpell.LookupEntry(suddenDeath->pSpellId);
				if(sd!=NULL)
					rageLeft = sd->RankNumber > 1 ? sd->RankNumber * 30 + 10 : sd->RankNumber * 30;
				unitTarget->RemoveAura(suddenDeath); // Sudden Death is removed after 1 execute
				// With Sudden Death can only use up to 30 total rage. so 30-15 = 15 goes to execute damage
				rageLeft = std::max(rageLeft, rageUsed - 150);
				rageUsed = std::min(rageUsed, 150);
			}

			if( u_caster->HasDummyAura(SPELL_HASH_GLYPH_OF_EXECUTION) )
				rageUsed += 100; //Your Execute ability deals damage as if you had 10 additional rage.

			value += (int32) (rageUsed * GetSpellProto()->dmg_multiplier[0]);
			u_caster->SetPower(POWER_TYPE_RAGE, rageLeft);
			SpellEntry *spellInfo = dbcSpell.LookupEntry(20647);
			u_caster->Strike(unitTarget,MELEE,spellInfo,0,0,value,false,false);
		}break;

		// Damage Shield
	case 58872: // Rank 1
	case 58874: // Rank 2
		{
			if(p_caster != NULL)
			{
				float amount = ((p_caster->GetUInt32Value(PLAYER_SHIELD_BLOCK)*GetSpellProto()->EffectBasePoints[i]+1)/100);
				p_caster->Damageshield_amount = amount > 0 ? amount : 0;
			}
		}break;

	/*************************
	 * MAGE SPELLS
	 *************************
	 * IDs:
	 *	11189 Frost Warding   -	RANK 1,		STATUS: DONE
	 *  28332 Frost Warding   -	RANK 2,		STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Frost Warding
		Increases the armor and resistances given by your Frost Armor and Ice Armor spells by X%.  In addition, gives your Frost Ward a X% chance to reflect Frost spells and effects while active.

		Effect #1	Apply Aura: Add % Modifier (8)

		Effect #2	Apply Aura: Dummy
		11189, 28332
	*/
	case 11189:
	case 28332:
		{
			if(unitTarget == NULL)
				break;
			for(std::list<struct ReflectSpellSchool*>::iterator i = unitTarget->m_reflectSpellSchool.begin(), i2;i != unitTarget->m_reflectSpellSchool.end();)
				if(GetSpellProto()->Id == (*i)->spellId)
				{
					i2 = i;
					++i;

					unitTarget->m_reflectSpellSchool.erase(i2);
				}
				else
					++i;

			ReflectSpellSchool *rss = new ReflectSpellSchool;
			rss->chance = GetSpellProto()->procChance;
			rss->spellId = GetSpellProto()->Id;
			rss->require_aura_hash = 2161224959UL;
			rss->school = GetSpellProto()->School;

			unitTarget->m_reflectSpellSchool.push_back(rss);
		}break;

	case 11958:		// Cold Snap
		{
			if(p_caster != NULL) // Clears our cooldowns on frost spells.
			{
				for(uint8 index = COOLDOWN_TYPE_SPELL; index < NUM_COOLDOWN_TYPES; index++)
				{
					PlayerCooldownMap cm = TO_PLAYER(m_caster)->GetCooldownMap(index);
					for(PlayerCooldownMap::const_iterator itr = cm.begin(); itr != cm.end(); itr++)
					{
						uint32 spellid = itr->second.SpellId;
						SpellEntry *spellInfo = dbcSpell.LookupEntry(spellid);
						if(spellInfo)
						{
							uint32 cooldown = spellInfo->RecoveryTime ? spellInfo->RecoveryTime : spellInfo->CategoryRecoveryTime;
							if(spellInfo->SpellFamilyName == SPELLFAMILY_MAGE && (spellInfo->School == SCHOOL_FROST)
								&& spellInfo->Id != 11958 && cooldown > 0)
							{
								TO_PLAYER(m_caster)->ClearCooldownForSpell(spellid);
							}
						}
					}
				}
			}
			else
			{
				// Todo: For our AI, we should clear cooldowns on frost AI spells.
			}
		}break;

	case 31687: // Summon Water Elemental.
		{
			m_summonProperties = dbcSummonProps.LookupEntryForced( GetSpellProto()->EffectMiscValueB[i] );
			SummonGuardian(i);
		}break;

	/*************************
	 * ROGUE SPELLS
	 *************************
	 * IDs:
	 * 14185 Preparation		STATUS: DONE
	 *  --------------------------------------------
	 * 35729 Cloak of Shadows	STATUS: DONE
	 *  --------------------------------------------
	 *************************/
		/*
		Hunger For Blood
		Enrages you, increasing all damage caused by 3%.  If used while a Bleed effect is afflicting you, it will attempt to remove it and refund 15 energy.  This effect can be stacked up to 3 times.  Lasts 30 sec.
		*/
	case 51662:
		{
			if( p_caster == NULL)
				return;

			if( p_caster->RemoveAllAurasByMechanic( MECHANIC_BLEEDING , 1 , true ) )
			{
				// Give 15 energy.
				p_caster->Energize(p_caster, 51662, 15, POWER_TYPE_ENERGY);
			}
		}break;

		//Disarm Trap
	case 1842:
		{
			if( p_caster == NULL || gameObjTarget == NULL )
				return;

			if( gameObjTarget->GetType() == GAMEOBJECT_TYPE_TRAP )
				gameObjTarget->_Expire();
		}break;
	/*
		Preparation
		When activated, this ability immediately finishes the cooldown on your Evasion, Sprint, Vanish, Cold Blood and Shadowstep abilities.

		Effect	Dummy
	*/
	case 14185:
		{
			if( p_caster == NULL )
				return;

			uint32 ClearSpellId[10] =
			{
				5277,  /* Evasion - Rank 1 */
				26669, /* Evasion - Rank 2 */
				2983,  /* Sprint  - Rank 1 */
				8696,  /* Sprint  - Rank 2 */
				11305, /* Sprint  - Rank 3 */
				1856,  /* Vanish  - Rank 1 */
				1857,  /* Vanish  - Rank 2 */
				26889, /* Vanish  - Rank 3 */
				14177, /* Cold Blood	   */
				36554  /* Shadowstep	   */
			};

			for(i = 0; i < 10; i++)
			{
				if( p_caster->HasSpell( ClearSpellId[i] ) )
					p_caster->ClearCooldownForSpell( ClearSpellId[i] );
			}
		}break;
	/*
		Cloak of Shadows
		Instantly removes all existing harmful spell effects and increases your chance to resist all spells by 90% for 5 sec.  Does not remove effects that prevent you from using Cloak of Shadows.

		Effect #1	Apply Aura: Mod Attacker Spell Hit Chance (126)
			Value: -90
		Effect #2	Trigger Spell
			Spell #35729 <--- THIS SPELL
	*/
	case 35729:
		{
			if( unitTarget == NULL || !unitTarget->isAlive())
				return;

			Aura* pAura = NULL;
			for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
			{
				pAura = unitTarget->m_auras[i];
				if( pAura != NULL && !pAura->IsPassive() && !pAura->IsPositive() && !(pAura->GetSpellProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY) &&
					pAura->GetSpellProto()->School != 0 )
				{
					unitTarget->RemoveAuraBySlot(i);
				}
			}
		}break;
	 case 5938:
		{
			if( p_caster == NULL || unitTarget == NULL || !unitTarget->isAlive())
				return;

			Item* Offhand = p_caster->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_OFFHAND);

			if( Offhand == NULL || Offhand->GetEnchantment( 1 ) == NULL )
				return;

			SpellEntry* spellInfo = dbcSpell.LookupEntry(Offhand->GetEnchantment( 1 )->Enchantment->spell[0]);

			if(spellInfo && spellInfo->c_is_flags & SPELL_FLAG_IS_POISON )
			{
				Spell* spell(new Spell( p_caster, spellInfo, true, NULLAURA));
				SpellCastTargets targets;
				targets.m_unitTarget = unitTarget->GetGUID();
				spell->prepare( &targets );
			}
		}break;
		//Vanish dummy spell lets give it a use kk?
	case 18461:
		{
			if(p_caster == NULL)
				return;
			SpellEntry *p = NULL;
			for( uint32 x = MAX_POSITIVE_AURAS ; x < MAX_AURAS ; x++ )
			{
				if( p_caster->m_auras[x] != NULL && !p_caster->m_auras[x]->IsPositive())
				{
					p = p_caster->m_auras[x]->GetSpellProto();
					if( Spell::HasMechanic(p, 7) || Spell::HasMechanic(p, 11) )
						p_caster->m_auras[x]->AttemptDispel( p_caster );
				}
			}
			p_caster->RemoveAuraNegByNameHash(SPELL_HASH_HUNTER_S_MARK);
			p_caster->CastSpell(p_caster->GetGUID(), dbcSpell.LookupEntryForced(1784), true);
		}break;
	/*************************
	 * DRUID SPELLS
	 *************************
	 * IDs:
	 * 34297 Improved Leader of the Pack RANK 1		STATUS: DONE
	 * 34300 Improved Leader of the Pack RANK 2		STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Improved Leader of the Pack
		Your Leader of the Pack ability also causes affected targets to have a X% chance to heal themselves for X% of their total health when they critically hit with a melee or ranged attack.  The healing effect cannot occur more than once every 6 sec.

		Effect #1	Apply Aura: Add Flat Modifier (12)
			Value: X
		Effect #2	Dummy
			Value: 100
	*/
	case 34297:
	case 34300:
		{
			if (!u_caster->IsPlayer())
				return;
			ProcTriggerSpell ILotP;
			ILotP.origId = 34299;
			ILotP.spellId = 34299;
			ILotP.procChance = 100;
			ILotP.procFlags = PROC_ON_CRIT_ATTACK;
			ILotP.procflags2 = PROC_TARGET_SELF;
			ILotP.deleted = false;
			ILotP.caster = u_caster->GetGUID();
			ILotP.LastTrigger = 0;
			ILotP.weapon_damage_type = 0;
			u_caster->m_procSpells.push_back(ILotP);
		}break;

	case 52610: // Savage Roar
		{
			if(p_caster != NULL)
			{	// We add an event so we can have the duration from combo points.
				sEventMgr.AddEvent(p_caster, &Player::NullComboPoints, EVENT_COMBO_POINT_CLEAR_FOR_TARGET, 20, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			}
		}break;
	/*************************
	 * HUNTER SPELLS
	 *************************
	 * IDs:
	 * 35029 Focused Fire RANK 1		STATUS: ToDo
	 * 35030 Focused Fire RANK 2		STATUS: ToDo
	 *  --------------------------------------------
	 *************************/
	/*************************
	 * PALADIN SPELLS
	 *************************
	 * IDs:
	 * 31789 Righteous Defense		STATUS: DONE
	 *  --------------------------------------------
	 * 18350 illumination			STATUS: DONE
	 *  --------------------------------------------
	 *************************/

	/*
		Righteous Defense
		Come to the defense of a friendly target, commanding up to 3 enemies attacking the target to attack the Paladin instead.

		Effect #1	Dummy
			Radius: 5 yards

		Effect #2	Trigger Spell
			Spell #31980
	*/
	case 31789:
		{
			//we will try to lure 3 enemies from our target
			if( unitTarget == NULL || u_caster == NULL)
				break;
			Unit* targets[3];
			int targets_got=0;
			for(unordered_set<Object* >::iterator itr = unitTarget->GetInRangeSetBegin(), i2; itr != unitTarget->GetInRangeSetEnd(); )
			{
				i2 = itr++;
				// don't add objects that are not units and that are dead
				if((*i2)->GetTypeId()!= TYPEID_UNIT || !TO_UNIT(*i2)->isAlive())
					continue;

				Creature* cr=TO_CREATURE((*i2));
				if(cr->GetAIInterface()->GetNextTarget()==unitTarget)
					targets[targets_got++]=cr;
				if(targets_got==3)
					break;
			}
			for(int i=0;i<targets_got;++i)
			{
				//set threat to this target so we are the msot hated
				uint32 threat_to_him = targets[i]->GetAIInterface()->getThreatByPtr( unitTarget );
				uint32 threat_to_us = targets[i]->GetAIInterface()->getThreatByPtr(u_caster);
				int threat_dif = threat_to_him - threat_to_us;
				if(threat_dif>0)//should nto happen
					targets[i]->GetAIInterface()->modThreatByPtr(u_caster,threat_dif);
				targets[i]->GetAIInterface()->AttackReaction(u_caster,1,0);
				targets[i]->GetAIInterface()->SetNextTarget(u_caster);
			}
		}break;
	/*
		Divine Storm
	*/

	case 53385:
		{
			if( p_caster != NULL && m_targetList.size())
			{
				uint32 amt = float2int32( 0.25f * m_targetList.size() * CalculateDamage(p_caster, unitTarget, MELEE, GetSpellProto()) );
				uint32 count = 0;

				if( p_caster->GetGroup() )
				{
					p_caster->GetGroup()->Lock();
					for(uint32 x = 0; x < p_caster->GetGroup()->GetSubGroupCount(); ++x)
					{
						if( count == 3 )
							break;

						for(GroupMembersSet::iterator itr = p_caster->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != p_caster->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); itr++)
						{
							if( (*itr) && (*itr)->m_loggedInPlayer && (*itr)->m_loggedInPlayer->GetPowerType() == POWER_TYPE_MANA && count != 3)
							{
								SpellEntry *spellInfo = dbcSpell.LookupEntry( 54172 );
								Spell* sp(new Spell( p_caster, spellInfo, true, NULLAURA ));
								sp->forced_basepoints[0] = amt;
								SpellCastTargets tgt;
								tgt.m_unitTarget = (*itr)->m_loggedInPlayer->GetGUID();
								sp->prepare(&tgt);
								count++;
							}
						}
					}
					p_caster->GetGroup()->Unlock();
				}
				else
				{
					SpellEntry *spellInfo = dbcSpell.LookupEntry( 54172 );
					Spell* sp(new Spell( p_caster, spellInfo, true, NULLAURA ));
					sp->forced_basepoints[0] = amt;
					SpellCastTargets tgt;
					tgt.m_unitTarget = p_caster->GetGUID();
					sp->prepare(&tgt);
				}
			}
		}break;
	case 70769:// Crow: OMG NERFFFFFFF :D
		{
			if(p_caster != NULL)
				p_caster->ClearCooldownForSpell(53385);
		}break;
	case 18350: // Massive Dummy spell behind Illumination.
		{
			if(p_caster == NULL || ProcedOnSpell == NULL) // Someone is force casting it.
				return;

			SpellEntry* sp = dbcSpell.LookupEntry(ProcedOnSpell->Id);
			if(sp != NULL)
			{
				uint32 cost = 0;
				if(sp->ManaCostPercentage)
				{
					uint32 maxmana = p_caster->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
					cost = maxmana*(float(sp->ManaCostPercentage)/100);
				}
				else if(sp->manaCostPerlevel)
				{
					cost = sp->manaCostPerlevel*p_caster->getLevel();
				}
				else if(sp->manaCost)
				{
					cost = sp->manaCost;
				}

				if(cost > 0)
				{
					uint32 amount = ((cost*p_caster->m_Illumination_amount)/100);
					SpellEntry* newspell = dbcSpell.LookupEntry(20272);
					Spell* nsp = new Spell(p_caster, newspell, true, NULLAURA);
					SpellCastTargets tgts(p_caster->GetGUID());
					nsp->forced_basepoints[0] = amount;
					nsp->prepare(&tgts);
				}
			}
		}break;

	/*************************
	 * SHAMAN SPELLS
	 *************************
	 * IDs:
	 * NO SPELLS
	 *  --------------------------------------------
	 *************************/
	case 55198:	// Tidal Force
		{
			SpellEffectTriggerSpell(i);
			SpellEffectTriggerSpell(i);
			SpellEffectTriggerSpell(i);	// want stack of 3 so cast it 3 times. hacky :/
		}break;

	/*************************
	 * WARLOCK SPELLS
	 *************************
	 * IDs:
	 * 19028 soul link effect		STATUS: DONE
	 *  --------------------------------------------
	 *************************/
	/*
		Soul Link - EFFECT
		20% of damage taken by master is split with demon. All damage increased by 5%.
	*/
	case 25228:
		{
			if( u_caster == NULL || !u_caster->isAlive() || unitTarget == NULL || !unitTarget->isAlive())
				return;
			uint32 pet_dmg = forced_basepoints[0]*20/100;
			unitTarget->ModUnsigned32Value(UNIT_FIELD_HEALTH,pet_dmg);
			unitTarget->DealDamage(u_caster,pet_dmg,0,0,25228);
		}break;
		//Demonic Circle
	case 48020:
		{
			if( u_caster == NULL )
				return;

			GameObject* DemonicCircle = u_caster->GetMapMgr()->GetGameObject( u_caster->m_ObjectSlots[0] );
			if( DemonicCircle )
			{
				u_caster->Teleport( DemonicCircle->GetPositionX(), DemonicCircle->GetPositionY(), DemonicCircle->GetPositionZ(), DemonicCircle->GetOrientation());
			}
		}break;
	/*************************
	 * DEATH KNIGHT SPELLS
	 *************************/
	case 52375: // Old Death Coil
	case 47541: // New Death Coil
	case 49892:
	case 49893:
	case 49894:
	case 49895: //Death Coil
		{
			if( u_caster == NULL || unitTarget == NULL)
				return;
			if(spellId == 52375)
				damage = damage * 2 / 5;	// 40% for rank 1
			if(isAttackable(u_caster, unitTarget, false))
				u_caster->SpellNonMeleeDamageLog(unitTarget, spellId, damage, true);
			if(unitTarget->GetCreatureType() == UNDEAD)
			{
				u_caster->Heal(unitTarget, spellId, float2int32(damage * 1.5f));
				u_caster->Energize(u_caster, 47541, 20, POWER_TYPE_RUNIC);
			}
		}break;

	case 45463: // Death Strike
	case 49923:
	case 49924:
	case 49998:
	case 49999:
		{
			if(u_caster != NULL && unitTarget != NULL)
			{
				uint32 diseasecount = 0;
				uint32 diseases[2] = { 55078, 55095 };
				for(int8 i = 0; i < 2; i++)
					if(unitTarget->HasAura(diseases[i]))
						diseasecount++;

				if(diseasecount)
				{
					uint32 healamount = (u_caster->GetMaxHealth()/20)*diseasecount;
					u_caster->Heal(u_caster, spellId, healamount);
				}
			}
		}break;

	case 49020:
		{
			if(u_caster != NULL && unitTarget != NULL)
			{
				uint32 diseasecount = 0;
				uint32 diseases[2] = { 55078, 55095 };
				for(int8 i = 0; i < 2; i++)
					if(unitTarget->HasAura(diseases[i]))
						diseasecount++;
				if(diseasecount)
					unitTarget->RemoveAura(diseases[i]);
			}

		}break;

	case 49576: // Death Grip
		{
			if( u_caster == NULL || unitTarget == NULL )
				return;

			// Move Effect
			unitTarget->CastSpellAoF( u_caster->GetPositionX(), u_caster->GetPositionY(), u_caster->GetPositionZ(), dbcSpell.LookupEntryForced(49575), true);
			u_caster->CastSpell( unitTarget, 51399, true ); // Taunt Effect
		}break;

	case 49203: //Hungering Cold
		{
			unitTarget->CastSpell(u_caster, 51209, true);
 		}break;
	case 61999: //Raise Ally
		{
			if(!unitTarget->isDead() || !playerTarget)
				return;
			unitTarget->setDeathState(DEAD);
 			CreatureInfo *ci = CreatureNameStorage.LookupEntry(30230);
			Pet *summon = objmgr.CreatePet();
			summon->CreateAsSummon(30230, ci, TO_CREATURE(summon), u_caster, GetSpellProto(), 1, (uint32)320000); //Give it an extra seconds for being unpossesed.
			playerTarget->SetSummon(summon);

			summon->SetUInt32Value(UNIT_FIELD_LEVEL, unitTarget->getLevel());
			summon->SetUInt32Value(UNIT_FIELD_MAXHEALTH, unitTarget->GetMaxHealth());
			summon->SetUInt32Value(UNIT_FIELD_HEALTH, summon->GetMaxHealth());			
			summon->SetUInt32Value(UNIT_FIELD_RESISTANCES, p_caster->GetUInt32Value(UNIT_FIELD_RESISTANCES));
			/*summon->AddSpell(dbcSpell.LookupEntry(62225), true); // Claw
			summon->AddSpell(dbcSpell.LookupEntry(47480), true); // Thrash
			summon->AddSpell(dbcSpell.LookupEntry(47481), true); // Gnaw
			summon->AddSpell(dbcSpell.LookupEntry(47482), true); // Leap
			summon->AddSpell(dbcSpell.LookupEntry(47484), true); // Huddle
			summon->AddSpell(dbcSpell.LookupEntry(51874), true); // Explode*/
			summon->SetPowerType(POWER_TYPE_ENERGY);
			summon->SetPower(POWER_TYPE_ENERGY,100);
			summon->SetMaxPower(POWER_TYPE_ENERGY,100);
			unitTarget->CastSpell(unitTarget, 46619, true);
		}break;
	case 48743:
		{
			//uhhh what to do here?
		}break;
	/*************************
		Non-Class spells
		- Done
	 *************************/
	/*
		Living Root of the Wildheart
		Your spells and attacks in each form have a chance to grant you a blessing for 15 sec.
	*/
	case 37336:
		{
			if( p_caster == NULL || playerTarget == NULL )
				return;

			uint32 NewSpell = 0;
			uint8 shape = playerTarget->GetShapeShift();
			switch( shape )
			{
			case FORM_NORMAL:
				NewSpell = 37344; break;
			case FORM_BEAR:
				NewSpell = 37340; break;
			case FORM_DIREBEAR:
				NewSpell = 37340; break;
			case FORM_CAT:
				NewSpell = 37341; break;
			case FORM_TREE:
				NewSpell = 37342; break;
			case FORM_MOONKIN:
				NewSpell = 37343; break;
			}
			if( NewSpell != 0 )
				p_caster->CastSpell(playerTarget,NewSpell,true);
		}break;
	/*
		Poultryizer
		Turns the target into a chicken for 15 sec.   Well, that is assuming the transmogrification polarity has not been reversed...
	*/
	case 30507:
		{
			if( unitTarget == NULL || !unitTarget->isAlive())
				return;

			u_caster->CastSpell(unitTarget->GetGUID(),30501,true);
		}break;
	/*
		Six Demon Bag
		Blasts enemies in front of you with the power of wind, fire, all that kind of thing!
	*/
	case 14537:
		{
			if( unitTarget == NULL || !unitTarget->isAlive())
				return;

			uint32 ClearSpellId[6] = { 8401,8408,930,118,1680,10159 };
			int min = 1;
			uint32 effect = min + int( ((6-min)+1) * rand() / (RAND_MAX + 1.0) );

			u_caster->CastSpell(unitTarget, ClearSpellId[effect] ,true);
		}break;

	case 30427: // Extract Gas
		{
			bool check = false;
			uint32 cloudtype;
			Creature* creature = NULLCREATURE;

			if(p_caster == NULL)
				return;

			for(Object::InRangeSet::iterator i = p_caster->GetInRangeSetBegin(); i != p_caster->GetInRangeSetEnd(); i++)
			{
				if((*i)->GetTypeId() == TYPEID_UNIT)
				{
					creature= TO_CREATURE(*i);
					cloudtype=creature->GetEntry();
					if(cloudtype == 24222 || cloudtype == 17408 || cloudtype == 17407 || cloudtype == 17378)
					{
						if(p_caster->GetDistance2dSq((*i)) < 400)
						{
							p_caster->SetSelection(creature->GetGUID());
							check = true;
							break;
						}
					}
				}
			}

			if(check)
			{
				uint32 item,count = 3+(rand()%3);

				if (cloudtype==24222) item=22572;//-air
				if (cloudtype==17408) item=22576;//-mana
				if (cloudtype==17407) item=22577;//-shadow
				if (cloudtype==17378) item=22578;//-water

				Item* add = p_caster->GetItemInterface()->FindItemLessMax(item, count, false);
				if (!add)
				{
					ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(item);
					SlotResult slotresult;

					slotresult = p_caster->GetItemInterface()->FindFreeInventorySlot(proto);
					if(!slotresult.Result)
					{
						p_caster->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM, INV_ERR_INVENTORY_FULL);
						return;
					}
					Item* it=objmgr.CreateItem(item,p_caster);
					it->SetUInt32Value( ITEM_FIELD_STACK_COUNT, count);
					p_caster->GetItemInterface()->SafeAddItem(it,slotresult.ContainerSlot, slotresult.Slot);
					creature->Despawn(3500,creature->proto->RespawnTime);
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + count);
					add->m_isDirty = true;
					creature->Despawn(3500,creature->proto->RespawnTime);
				}
			}
		}break;

	//curse of agony(18230) = periodic damage increased in
	//flag 2031678
	case 1454://life tap
	case 1455:
	case 1456:
	case 11687:
	case 11688:
	case 11689:
	case 27222:
	case 57946:
		{
			if(p_caster == NULL || playerTarget== NULL )
				return;

			uint32 energy = (GetSpellProto()->EffectBasePoints[i] + 1)+(p_caster->GetDamageDoneMod(SCHOOL_SHADOW)/2);
			if(playerTarget->m_lifetapbonus)
				energy *= (100 + playerTarget->m_lifetapbonus) / 100;	// Apply improved life tap
			p_caster->Energize(playerTarget, pSpellId ? pSpellId : GetSpellProto()->Id, energy, POWER_TYPE_MANA);
		}break;
	case 39610://Mana Tide
		{
			if(unitTarget == NULL || unitTarget->isDead() || unitTarget->getClass() == WARRIOR || unitTarget->getClass() == ROGUE)
 				return;
 			uint32 gain = (uint32) (unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*0.06);
			unitTarget->Energize(unitTarget,16191,gain,POWER_TYPE_MANA);
		}break;
	case 20577:// Cannibalize
		{
			if(p_caster == NULL)
				return;
			p_caster->CastSpell( p_caster, 20578, true);
			p_caster->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_CANNIBALIZE);
		}break;
	case 23074:// Arcanite Dragonling
	case 23075:// Mithril Mechanical Dragonling
	case 23076:// Mechanical Dragonling
	case 23133:// Gnomish Battle Chicken
		{
			uint32 spell_id;
			switch(GetSpellProto()->Id)
			{
				case 23074: spell_id = 19804; break;
				case 23075: spell_id = 12749; break;
				case 23076: spell_id =  4073; break;
				case 23133: spell_id = 13166; break;
				default:
					return;
			}
			u_caster->CastSpell(u_caster,spell_id,true);
		}break;
	case 23725:// Gift of Life
		{
			if(playerTarget == NULL)
				break;

			SpellCastTargets tgt;
			tgt.m_unitTarget = playerTarget->GetGUID();
			SpellEntry * inf =dbcSpell.LookupEntry(23782);
			Spell* spe = CREATESPELL(u_caster,inf,true,NULLAURA);
			spe->prepare(&tgt);

		}break;
	case 12975:// Last Stand
		{
			if(playerTarget == NULL)
				break;
			SpellCastTargets tgt;
			tgt.m_unitTarget = playerTarget->GetGUID();
			SpellEntry * inf =dbcSpell.LookupEntry(12976);
			Spell* spe = CREATESPELL(u_caster,inf,true,NULLAURA);
			spe->prepare(&tgt);
		}break;
	/*************************
		Non-Class spells
		- ToDo
	 *************************/
	case 6668:// Red Firework
		{
			// Shoots a firework into the air that bursts into a thousand red stars
		}break;
	case 8344:// Universal Remote
		{
			//FIXME:Allows control of a mechanical target for a short time.  It may not always work and may just root the machine or make it very very angry.  Gnomish engineering at its finest.
		}break;
	case 9976:// Polly Eats the E.C.A.C.
		{
			//FIXME: Don't know what this does
		}break;
	case 10137:// Fizzule's Whistle
		{
			//FIXME:Blow on the whistle to let Fizzule know you're an ally
			//This item comes after a finish of quest at venture co.
			//You must whistle this every time you reach there to make Fizzule
			//ally to you.
		}break;
	case 11540:// Blue Firework
		{
			//Shoots a firework into the air that bursts into a thousand blue stars
		}break;
	case 11541:// Green Firework
		{
			//Shoots a firework into the air that bursts into a thousand green stars
		}break;
	case 11542:// Red Streaks Firework
		{
			//Shoots a firework into the air that bursts into a thousand red streaks
		}break;
	case 11543:// Red, White and Blue Firework
		{
			//Shoots a firework into the air that bursts into red, white and blue stars
		}break;
	case 11544:// Yellow Rose Firework
		{
			//Shoots a firework into the air that bursts in a yellow pattern
		}break;
	case 12151:// Summon Atal'ai Skeleton
		{
			//8324	Atal'ai Skeleton

			//FIXME:Add here remove in time event
		}break;
	case 13535:// Tame Beast
		{

		}break;
	case 13006:// Shrink Ray
		{
			//FIXME:Schematic is learned from the gnomish engineering trainer. The gnomish/gobblin engineering decision is made when you are lvl40+ and your engineering is 200+. Interestingly, however, when this item fails to shrink the target, it can do a variety of things, such as...
			//-Make you bigger (attack power +250)
			//-Make you smaller (attack power -250)
			//-Make them bigger (same effect as above)
			//-Make your entire party bigger
			//-Make your entire party smaller
			//-Make every attacking enemy bigger
			//-Make ever attacking enemy smaller
			//Works to your advantage for the most part (about 70% of the time), but don't use in high-pressure situations, unless you're going to die if you don't. Could tip the scales the wrong way.
			//Search for spells of this


			//13004 - grow <- this one
			//13010 - shrink <-this one
			//
		}break;
	case 13180:// Gnomish Mind Control Cap
		{
			// FIXME:Take control of humanoid target by chance(it can be player)
		}break;
	case 13278:// Gnomish Death Ray
		{
			// FIXME:The devices charges over time using your life force and then directs a burst of energy at your opponent
			//Drops life
		}break;
	case 13280:// Gnomish Death Ray
		{
			//FIXME: Drop life
		}break;
	case 17816:// Sharp Dresser
		{
			//Impress others with your fashion sense
		}break;
	case 21343:// Snowball
		{
		}break;
	case 23645:// Hourglass Sand
		{
			//Indeed used at the Chromo fight in BWL. Chromo has a stunning debuff, uncleansable, unless you have hourglass sand. This debuff will stun you every 4 seconds, for 4 seconds. It is resisted a lot though. Mage's and other casters usually have to do this fight with the debuff on, healers, tanks and hunters will get some to cure themselves from the debuff
		}break;
	case 24325:// Pagle's Point Cast - Create Mudskunk Lure
		{
			//FIXME:Load with 5 Zulian Mudskunks, and then cast from Pagle's Point in Zul'Gurub
		}
	case 24392:// Frosty Zap
		{
			//FIXME:Your Frostbolt spells have a 6% chance to restore 50 mana when cast.
			//damage == 50
		}break;
	case 25822:// Firecrackers
		{
			//FIXME:Find firecrackers
		}break;
	case 26373:// Lunar Invititation
		{
			//FIXME: Teleports the caster from within Greater Moonlight
		}break;
	case 26374:// Elune's Candle
		{
			if(u_caster == NULL)
				return;
			u_caster->CastSpell(unitTarget,26622,true);
		}break;
	case 26889:// Give Friendship Bracelet
		{
			//Give to a Heartbroken player to cheer them up
			//laugh emote
		}break;
	case 27662:// Throw Cupid's Dart
		{
			//FIXME:Shoot a player, and Kwee Q. Peddlefeet will find them! (Only works on players with no current critter pets.)
		}break;
	case 28414:// Call Ashbringer
		{
			//http://www.thottbot.com/?i=53974
			if(p_caster == NULL)
			 	return;

			uint32 ashcall = RandomUInt(12);

			WorldPacket data;
			data.SetOpcode(SMSG_PLAY_OBJECT_SOUND);

			WorldPacket *crap;
			std::stringstream Reply;

			switch(ashcall)
			{
			case 1:
				{
						data << uint32(8906) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "I... was... pure... once.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 2:
				{
						data << uint32(8907) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Fought... for... righteousness.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 3:
				{
						data << uint32(8908) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "I... was... once... called... Ashbringer.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 4:
				{
						data << uint32(8920) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Betrayed... by... my... order.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 5:
				{
						data << uint32(8921) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Destroyed... by... Kel'Thuzad.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 6:
				{
						data << uint32(8922) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Made... to serve.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 7:
				{
						data << uint32(8923) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "My... son... watched... me... die.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 8:
				{
						data << uint32(8924) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Crusades... fed his rage.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 9:
				{
						data << uint32(8925) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Truth... is... unknown... to him.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 10:
				{
						data << uint32(8926) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Scarlet... Crusade... is pure... no longer.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 11:
				{
						data << uint32(8927) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Balnazzar's... crusade... corrupted... my son.";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			case 12:
				{
						data << uint32(8928) << unitTarget->GetGUID();
						p_caster->SendMessageToSet(&data, true);
						Reply << "Kill... them... all!";
						crap = sChatHandler.FillMessageData( CHAT_MSG_WHISPER, LANG_UNIVERSAL, Reply.str().c_str(), p_caster->GetGUID(), 0);
				}break;
			}
			p_caster->GetSession()->SendPacket(crap);
		}break;
	case 28806:// Toss Fuel on Bonfire
		{
			//FIXME:Dont know what this dummy does
		}break;
	case 7669:// Bethor's Potion
		{
			// related to Hex of Ravenclaw,
			// its a dispell spell.
			//FIXME:Dont know whats the usage of this dummy
		}break;
	case 8283:// Snufflenose Command
		{
			//FIXME:Quest Blueleaf Tubers
			//For use on a Snufflenose Gopher
		}break;
	case 8913:// Sacred Cleansing
		{
			//FIXME:Removes the protective enchantments around Morbent Fel
			//Quest Morbent Fel
		}break;
	case 9962://Capture Treant
		{
			//Quest Treant Muisek
		}break;
	case 10113:// Flare Gun's flare
		{
			//FIXME:Quest Deep Cover
			//1543 may need to cast this
			//2 flares and the /salute
		}break;
	case 10617:// Release Rageclaw
		{
			//Quest Druid of the Claw
			//Use on the fallen body of Rageclaw
		}break;
	case 11402:// Shay's Bell
		{
			//FIXME:Quest Wandering Shay
			//Ring to call Shay back to you
		}break;

	case 45109: // Orb of Murloc Control
		{
			if(SpellEffectUpdateQuest( 11541 ))
			{
				Creature* qtarget = TO_CREATURE(GetUnitTarget());
				if( qtarget )
				{
					p_caster->GetMapMgr()->GetInterface()->SpawnCreature(25085, qtarget->GetPositionX(), qtarget->GetPositionY(), qtarget->GetPositionZ(), qtarget->GetOrientation(), true, false, 0, 0)->Despawn(600000, 0);
					qtarget->Despawn(0, 600000);
				}
			}
		} break;
	case 44997: // Converting Sentry
		{
			if(!SpellEffectUpdateQuest( 11524 ))
				SpellEffectUpdateQuest( 11525 );
		}break;
	case 32042: // Arcane Disturbances for Karazan atunament
		{
			SpellEffectUpdateQuest( 9824 );
		} break;
	case 36904: // Scratches
		{
			SpellEffectUpdateQuest( 10556 );
		} break;
	case 33655:// Mission: The Murketh and Shaadraz Gateways and Mission: Gateways Murketh and Shaadraz
		{
			if(!SpellEffectUpdateQuest( 10129 ))
				SpellEffectUpdateQuest( 10146 );
		}break;
	case 38439:// Kindness
		{
			SpellEffectUpdateQuest( 10804 );
		}break;
	case 31927:// Blessing of Incineratus
		{
			SpellEffectUpdateQuest( 9805 );
		}break;
	case 11548:// Summon Spider God
		{
			float SSX = p_caster->GetPositionX();
			float SSY = p_caster->GetPositionY();
			float SSZ = p_caster->GetPositionZ();
			float SSO = p_caster->GetOrientation();

			p_caster->GetMapMgr()->GetInterface()->SpawnCreature(2707, SSX, SSY, SSZ, SSO, true, false, 0, 0)->Despawn(60000, 0);
		}break;
	case 11610:// Gammerita Turtle Camera
		{
			//Quest The Super Snapper FX
		}break;
	case 11886:// Capture Wildkin
		{
			//Quest Testing the Vessel
			//Shrink and Capture a Fallen Wildkin
		}break;
	case 11887:// Capture Hippogryph
		{
			//FIXME:Same with 11888
			//Quest Hippogryph Muisek
		}break;
	case 11888:// Capture Faerie Dragon
		{
			//FIXME:Check Faerie Dragon Muisek is killed or not if its killed update quest
			//And allow create of fearie Dragon which is effect 1
			//Quest: Faerie Dragon Muisek
		}break;
	case 11889:// Capture Mountain Giant
		{
			//FIXME:Same with 11888
			//Quest: Mountain Giant Muisek
		}break;
	case 12189:// Summon Echeyakee
		{
			//3475	Echeyakee

			//FIXME:Quest Echeyakee
		}break;
	case 12283:// Xiggs Signal Flare
		{
			//Quest Signal for Pickup
			//To be used at the makeshift helipad in Azshara. It will summon Pilot Xiggs Fuselighter to pick up the tablet rubbings
		}break;
	case 12938:// Fel Curse
		{
			//FIXME:Makes near target killable(servants of Razelikh the Defiler)
		}break;
	case 14247:// Blazerunner Dispel
		{
			//FIXME:Quest Aquementas and some more
		}break;
	case 14250:// Capture Grark
		{
			//Quest Precarious Predicament
		}break;
	case 14813:// Rocknot's Ale
		{
			//you throw the mug
			//and the guy gets pissed well everyone gets pissed and he crushes the door so you can get past
			//maybe after like 30 seconds so you can get past.  but lke I said I have never done it myself
			//so i am not 100% sure what happens.
		}break;
	case 15991://Revive Ringo
		{
			//Quest A Little Help From My Friends
			//Revive Ringo with water
		}break;
	case 15998:// Capture Worg Pup
		{
			//FIXME:Ends Kibler's Exotic Pets  (Dungeon) quest
		}break;
	case 16031:// Releasing Corrupt Ooze
		{
			//FIXME:Released ooze moves to master ooze and "Merged Ooze Sample"
			//occurs after some time.This item helps to finish quest
		}break;
	case 16378:// Temperature Reading
		{
			//FIXME:Quest Finding the Source
			//Take a reading of the temperature at a hot spot.
		}break;
	case 17166:// Release Umi's Yeti
		{
			//Quest Are We There, Yeti?
			//Select Umi's friend and click to release the Mechanical Yeti
		}break;
	case 17271:// Test Fetid Skull
		{
			//FIXME:Marauders of Darrowshire
			//Wave over a Fetid skull to test its resonance
		}break;
	case 18153:// Kodo Kombobulator
		{
			//FIXME:Kodo Roundup Quest
			//Kodo Kombobulator on any Ancient, Aged, or Dying Kodo to lure the Kodo to follow (one at a time)
		}break;
	case 19250:// Placing Smokey's Explosives
		{
			//This is something related to quest i think
		}break;
	case 19512:// Apply Salve
		{
			//FIXME:Cure a sickly animal afflicted by the taint of poisoning
			if(unitTarget->GetEntry() == 12298 && unitTarget->HasActiveAura(19502))
			{
				//unitTarget->AddAuraVisual(19502, -1, true);
				unitTarget->SetUInt32Value(UNIT_FIELD_DISPLAYID, 347);
				sQuestMgr._OnPlayerKill(p_caster, TO_CREATURE(unitTarget)->GetLowGUID());
				TO_CREATURE(unitTarget)->Despawn(5000, 360000);
			}
			else if(unitTarget->GetEntry() == 12296 && unitTarget->HasActiveAura(19502))
			{
				//unitTarget->AddAuraVisual(19502, -1, true);
				unitTarget->SetUInt32Value(UNIT_FIELD_DISPLAYID, 1547);
				sQuestMgr._OnPlayerKill(p_caster, TO_CREATURE(unitTarget)->GetLowGUID());
				TO_CREATURE(unitTarget)->Despawn(5000, 360000);
			}
			else
				SendCastResult(SPELL_FAILED_BAD_TARGETS);
		}break;
	case 20804:// Triage
		{
			//Quest Triage
			//Use on Injured, Badly Injured, and Critically Injured Soldiers
		}break;
	case 21050:// Melodious Rapture
		{
			//Quest Deeprun Rat Roundup
		}break;
	case 21332:// Aspect of Neptulon
		{
			//FIXME:Used on plagued water elementals in Eastern Plaguelands
			//Quest:Poisoned Water
		}break;
	case 21960:// Manifest Spirit
		{
			//FIXME:Forces the spirits of the first centaur Kahns to manifest in the physical world
			//thats a quest
			//its for maraudon i think
			//u use that on the spirit mobs
			//to release them
		}break;
	case 23359:// Transmogrify!
		{
			//Quest Zapped Giants
			//Zap a Feralas giant into a more manageable form
		}break;
	case 27184:// Summon Mor Grayhoof
		{
			//16044	Mor Grayhoof Trigger
			//16080	Mor Grayhoof

			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case 27190:// Summon Isalien
		{
			//16045	Isalien Trigger
			//16097	Isalien

			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case 27191:// Summon the remains of Jarien and Sothos
		{
			/*
			16046	Jarien and Sothos Trigger
			16101	Jarien
			16103	Spirit of Jarien

			16102	Sothos
			16104	Spirit of Sothos
			*/

			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case 27201:// Summon the spirit of Kormok
		{
			/*16047	Kormok Trigger
			16118	Kormok
			*/
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case 27202:// Summon Lord Valthalak
		{
			/*
			16042	Lord Valthalak
			16048	Lord Valthalak Trigger
			16073	Spirit of Lord Valthalak

			*/
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case 27203:// Summon the spirits of the dead at haunted locations
		{
			//Related to quests The Left Piece of Lord Valthalak's Amulet  (Dungeon)
			//and The Right Piece of Lord Valthalak's Amulet  (Dungeon)
		}break;
	case 27517:// Use this banner at the Arena in Blackrock Depths to challenge Theldren
		{
			//This is used to make Theldren spawn at the place where it used
			//I couldnt find theldrin, and his men in creature names database
			//Someone has to write this and this is related to The Challange quest
			/*By moving to the center grate, you trigger the arena event.
			A random group of mobs (spiders, worms, bats, raptors) spawns,
			and you have to kill them. After the last one dies, and a small
			break, a boss mob spawns. Successfully completing this event
			turns the arena spectators from red to yellow*/
		}break;
	case 34026: //Kill Command
		{
			if( p_caster == NULL  || p_caster->GetSummon() == NULL )
				return;
			Unit* petunit = TO_UNIT(p_caster->GetSummon());
			if( petunit != NULL )
			{
				p_caster->CastSpell(petunit, 34027, true);
				p_caster->CastSpell(petunit, 34027, true);
				p_caster->CastSpell(petunit, 34027, true);
			}
		}break;
	//Berserking Troll Racial
	case  20554:
	case  26296:
	case  26297:
	case  50621:
		{
			if(playerTarget == NULL)
				break;
			SpellCastTargets tgt;
			tgt.m_unitTarget = playerTarget->GetGUID();
			SpellEntry * inf =dbcSpell.LookupEntry(26635);
			Spell* spe = CREATESPELL(u_caster,inf,true,NULLAURA);
			spe->prepare(&tgt);
		}break;

		/*
		Crow: Dummy handlers for skills that are always at max.
		I have no idea what else we would put here o.o */
	case 33388: // Apprentice Riding.
	case 33391: // Journeyman Riding.
	case 34090: // Expert Riding.
	case 34091: // Artisan Riding.
		{
			if(p_caster != NULL)
			{
				skilllineentry* skill = dbcSkillLine.LookupEntry(GetSpellProto()->EffectMiscValue[1]);
				if(skill != NULL)
				{
					uint32 current = p_caster->_GetSkillLineCurrent(skill->id, false);
					uint32 maximum = p_caster->_GetSkillLineMax(skill->id);
					if(current < maximum)
						p_caster->_AdvanceSkillLine(skill->id, uint32(maximum - current));
				}
			}
		}break;

	case 20585:
		{	// Stinking elves, make it available to all if they have the aura.
			if(p_caster != NULL && p_caster->getRace() == RACE_NIGHTELF)
				p_caster->Wispform = true;
		}break;

		// Safe fall checks?
	case 1860:
	case 20719:
		{
			if(p_caster != NULL)
				p_caster->safefall = true;
		}break;

	case 22904: // Onyxia Sword crap for Quel'Serrar
		{
			unordered_set< Object* >::iterator itr,itr2;
			for( itr = m_caster->GetInRangeSetBegin(); itr != m_caster->GetInRangeSetEnd();)
			{
				itr2 = itr++;
				Object* obj = (*itr2);
				if(obj->IsGameObject() && obj->GetEntry() == 179561)
				{
					GameObject* gobj = TO_GAMEOBJECT(obj);
					if(m_caster->isTargetInFront(gobj))
					{
						MapMgr* mgr = gobj->GetMapMgr();
						GameObject* newobj = NULL;
						newobj = mgr->CreateGameObject(179562);
						if(newobj == NULL || !newobj->CreateFromProto(179562, mgr->GetMapId(), gobj->GetPosition()))
							return;

						newobj->SetInstanceID(mgr->GetInstanceID());
						newobj->PushToWorld(mgr);
						gobj->Despawn(0, 0);
					}
				}
			}
		}break;
	case 67799:
		{
			if(!p_caster || !unitTarget)
				return;
			uint32 result = RandomUInt(4);
			switch(result)
			{
			case 1:
				{
					p_caster->CastSpell(unitTarget,605,true);
				}break;
			case 2:
				{
					p_caster->CastSpell(p_caster,67809,true);
				}break;
			case 3:
				{
					p_caster->CastSpell(unitTarget,67810,true);
				}break;
			case 4:
				{
					if(playerTarget)
						unitTarget->CastSpell(p_caster,605,true);
					else
						return;
				}break;
			}
		}break;

	case 63624:
		{
			if(!playerTarget)
				return;
			u_caster->CastSpell(playerTarget, 63680, false);
		}break;
	case 31225: // Filled Shimmering Vessel, Redeeming the Dead.
		{
			if(p_caster != NULL)
			{
				QuestLogEntry* qle = p_caster->GetQuestLogForEntry(9685);
				if(qle == NULL || qle->CanBeFinished())
					return;

				qle->SetMobCount(0, 1);
				qle->SendUpdateAddKill(1);
				qle->UpdatePlayerFields();
				qle->SendQuestComplete();
				Creature* ctr = GetUnitTarget() ? (GetUnitTarget()->IsCreature() ? TO_CREATURE(GetUnitTarget()) : NULL) : NULL;
				if(ctr != NULL)
				{
					ctr->setDeathState(ALIVE);
					ctr->SetStandState(STANDSTATE_STAND);
					ctr->Despawn(10000, ctr->GetProto()->RespawnTime);
				}
			}
		}break;

	case 60103: { }break;
	default:
		{
			if(sLog.IsOutDevelopement())
				printf("Dummy spell not handled: %u%s\n", spellId, ((ProcedOnSpell != NULL) ? (format(" proc'd on: %u", ProcedOnSpell->Id).c_str()) : ""));
		}break;
	}
}

void Spell::SpellEffectTeleportUnits( uint32 i )  // Teleport Units
{
	uint32 spellId = GetSpellProto()->Id;

	if( unitTarget == NULL )
		return;

	// Shadowstep
	if( (GetSpellProto()->NameHash == SPELL_HASH_SHADOWSTEP) && p_caster && p_caster->IsInWorld() )
	{
		/* this is rather tricky actually. we have to calculate the orientation of the creature/player, and then calculate a little bit of distance behind that. */
		float ang;
		Unit* pTarget = unitTarget;
		if( pTarget == m_caster )
		{
			/* try to get a selection */
 			pTarget = m_caster->GetMapMgr()->GetUnit(p_caster->GetSelection());
			if( (pTarget == NULL ) || !isAttackable(p_caster, pTarget, !(GetSpellProto()->c_is_flags & SPELL_FLAG_IS_TARGETINGSTEALTHED) ) || (pTarget->CalcDistance(p_caster) > 30.0f))
				return;
		}

		if( pTarget->GetTypeId() == TYPEID_UNIT )
		{
			if( pTarget->GetUInt64Value( UNIT_FIELD_TARGET ) != 0 )
			{
				/* We're chasing a target. We have to calculate the angle to this target, this is our orientation. */
				ang = m_caster->calcAngle(m_caster->GetPositionX(), m_caster->GetPositionY(), pTarget->GetPositionX(), pTarget->GetPositionY());

				/* convert degree angle to radians */
				ang = ang * float(M_PI) / 180.0f;
			}
			else
			{
				/* Our orientation has already been set. */
				ang = pTarget->GetOrientation();
			}
		}
		else
		{
			/* Players orientation is sent in movement packets */
			ang = pTarget->GetOrientation();
		}

		// avoid teleporting into the model on scaled models
		const static float shadowstep_distance = 1.6f * GetScale( dbcCreatureDisplayInfo.LookupEntry( unitTarget->GetUInt32Value(UNIT_FIELD_DISPLAYID)));
		float new_x = pTarget->GetPositionX() - (shadowstep_distance * cosf(ang));
		float new_y = pTarget->GetPositionY() - (shadowstep_distance * sinf(ang));
		float new_z = pTarget->GetCHeightForPosition(true);
		/* Send a movement packet to "charge" at this target. Similar to warrior charge. */
		p_caster->z_axisposition = 0.0f;
		p_caster->SafeTeleport(p_caster->GetMapId(), p_caster->GetInstanceID(), LocationVector(new_x, new_y, new_z, pTarget->GetOrientation()));
		return;
	}

	if(unitTarget->IsPlayer())
		HandleTeleport(spellId, unitTarget);
}

void Spell::SpellEffectApplyAura(uint32 i)  // Apply Aura
{
	if(unitTarget == NULL)
		return;

	//Aura Immune Flag Check
	if ( playerTarget == NULL)
	{
		Creature* c = TO_CREATURE( unitTarget );
		if(c != NULL)
		{
			if(c->LoadedProto != NULL)
			{
				if(c->LoadedProto->auraimmune_flag && (c->LoadedProto->auraimmune_flag & GetSpellProto()->auraimmune_flag))
					return;
			}
			else if(c->proto != NULL)
				if( c->proto->auraimmune_flag && (c->proto->auraimmune_flag & GetSpellProto()->auraimmune_flag ))
					return;
		}
	}

	/* Aura Mastery + Aura Of Concentration = No Interrupting effects */
	if(GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_SILENCE && unitTarget->HasAura(31821) && unitTarget->HasAura(19746))
		return;

	if( unitTarget->isDead() && !(GetSpellProto()->Flags4 & FLAGS4_DEATH_PERSISTENT) )
		return;

	// avoid map corruption.
	if(unitTarget->GetInstanceID()!=m_caster->GetInstanceID())
		return;

	//check if we already have stronger aura
	Aura* pAura = NULL;

	std::map<uint32,Aura* >::iterator itr=unitTarget->tmpAura.find(GetSpellProto()->Id);
	//if we do not make a check to see if the aura owner is the same as the caster then we will stack the 2 auras and they will not be visible client sided
	if(itr==unitTarget->tmpAura.end())
	{
		int32 Duration = GetDuration();

		// Handle diminishing returns, if it should be resisted, it'll make duration 0 here.
		if(!(GetSpellProto()->Attributes & ATTRIBUTES_PASSIVE)) // Passive
			::ApplyDiminishingReturnTimer(&Duration, unitTarget, GetSpellProto());

		if(!Duration)
		{
			//maybe add some resist messege to client here ?
			return;
		}
		if(g_caster && g_caster->GetUInt32Value(OBJECT_FIELD_CREATED_BY) && g_caster->m_summoner)
			pAura = new Aura(GetSpellProto(), Duration, g_caster->m_summoner, unitTarget);
		else
			pAura = new Aura(GetSpellProto(), Duration, m_caster, unitTarget);
		if(pAura == NULL)
			return;

		pAura->pSpellId = pSpellId; //this is required for triggered spells

		unitTarget->tmpAura[GetSpellProto()->Id] = pAura;
	}
	else
	{
		 pAura=itr->second;
	}
	int32 miscValue = GetSpellProto()->EffectMiscValue[i];

	if(i_caster && m_caster->IsPlayer() && GetSpellProto()->EffectApplyAuraName[i]==SPELL_AURA_PROC_TRIGGER_SPELL)
		miscValue = p_caster->GetItemInterface()->GetInventorySlotByGuid( i_caster->GetGUID() ); // Need to know on which hands attacks spell should proc

	//Interactive spells
	uint32 spellID = GetSpellProto()->Id;
	switch(spellID)
	{
		case 27907:
		{
			if(unitTarget->GetEntry() == 15941)
			{
				sQuestMgr.OnPlayerKill(p_caster, ((Creature*)unitTarget));
				unitTarget->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "What? Oh, not this again!");
			}
			else if(unitTarget->GetEntry() == 15945)
			{
				sQuestMgr.OnPlayerKill(p_caster, ((Creature*)unitTarget));
				unitTarget->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You can't do this to me! We had a deal!");
			}
			else
			{
				SendCastResult(SPELL_FAILED_BAD_TARGETS);
				return;
			}
		}break;
		case 28880:
		{
			if(!p_caster)
			break;

			if(unitTarget->GetEntry() == 16483)
			{
				unitTarget->RemoveAura(29152);
				unitTarget->SetStandState(0);
				sQuestMgr.OnPlayerKill(p_caster, ((Creature*)unitTarget));
				static const char* testo[12] =
				{"None","Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "None", "Druid"};
				char msg[150];
				snprintf(msg, 150, "Many thanks to you %s. I'd best get to the crash site and see how I can help out. Until we meet again...", testo[p_caster->getClass()]);
				unitTarget->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg);
				((Creature*)unitTarget)->Despawn(900000, 300000); //removed after 15minutes, respawned after another 5 minutes
			}
		}break;

		case 38177: //Blackwhelp Net
		{
			if(!p_caster)
			break;

			if(unitTarget->GetEntry() == 21387)
			{
				((Creature*)unitTarget)->Despawn(5000, 360000);
				p_caster->CastSpell(p_caster, 38178, true);
			}else
			{
				SendCastResult(SPELL_FAILED_BAD_TARGETS);
				return;
			}
		}break;
	}

	pAura->AddMod(GetSpellProto()->EffectApplyAuraName[i],damage,miscValue,i);
}

void Spell::SpellEffectPowerDrain(uint32 i)  // Power Drain
{
	if( unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 powerField = UNIT_FIELD_POWER1+GetSpellProto()->EffectMiscValue[i];
	uint32 curPower = unitTarget->GetUInt32Value(powerField);
	uint32 amt = u_caster->GetSpellBonusDamage(unitTarget, GetSpellProto(), damage, false, false);

	if( GetPlayerTarget() )
		amt *= float2int32( 1 - ( ( TO_PLAYER(GetPlayerTarget())->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE ) * 2 ) / 100.0f ) );

	if(amt > curPower)
		amt = curPower;

	unitTarget->SetUInt32Value(powerField, curPower - amt);
	u_caster->Energize(u_caster, pSpellId ? pSpellId : GetSpellProto()->Id, amt, GetSpellProto()->EffectMiscValue[i]);
	unitTarget->SendPowerUpdate();
}

void Spell::SpellEffectHealthLeech(uint32 i) // Health Leech
{
	if( unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 curHealth = unitTarget->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 amt = damage;
	if(amt > curHealth)
	{
		amt = curHealth;
	}
	m_caster->DealDamage(unitTarget, damage, 0, 0, GetSpellProto()->Id);

	float coef = GetSpellProto()->EffectMultipleValue[i]; // how much health is restored per damage dealt
	if( u_caster && GetSpellProto() )
	{
		SM_FFValue(u_caster->SM[SMT_MULTIPLE_VALUE][0], &coef, GetSpellProto()->SpellGroupType);
		SM_PFValue(u_caster->SM[SMT_MULTIPLE_VALUE][1], &coef, GetSpellProto()->SpellGroupType);
	}

	amt = float2int32((float)amt * coef);

	uint32 playerCurHealth = m_caster->GetUInt32Value(UNIT_FIELD_HEALTH);
	uint32 playerMaxHealth = m_caster->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	uint32 overheal = 0;
	if(playerCurHealth + amt > playerMaxHealth)
	{
		m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, playerMaxHealth);
		overheal = playerCurHealth + amt - playerMaxHealth;
	}
	else
	{
		m_caster->SetUInt32Value(UNIT_FIELD_HEALTH, playerCurHealth + amt);
	}
	SendHealSpellOnPlayer(m_caster, m_caster, amt, false, overheal, GetSpellProto()->Id);
}

void Spell::SpellEffectHeal(uint32 i) // Heal
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	if(GetSpellProto()->EffectChainTarget[i])//chain
	{
		if(!chaindamage)
		{
			Aura* riptide = NULLAURA;
			riptide = unitTarget->FindPositiveAuraByNameHash(SPELL_HASH_RIPTIDE);
			if(unitTarget != NULL && riptide != NULL && GetSpellProto()->NameHash == SPELL_HASH_CHAIN_HEAL)
			{
				damage += damage / 4; // +25%
				unitTarget->RemoveAura(riptide);
			}
			chaindamage = damage;
			Heal((int32)chaindamage);
		}
		else
		{
			int32 reduce = (int32)(GetSpellProto()->dmg_multiplier[i] * 100.0f);
			if(GetSpellProto()->SpellGroupType && u_caster)
			{
				SM_FIValue(u_caster->SM[SMT_JUMP_REDUCE][1],&reduce,GetSpellProto()->SpellGroupType);
			}
			chaindamage -= (reduce * chaindamage) / 100;
			Heal((int32)chaindamage);
		}
	}
	else
	{
		//yep, the usual special case. This one is shaman talent : Nature's guardian
		//health is below 30%, we have a mother spell to get value from
		switch (GetSpellProto()->Id)
		{
		case 34299: //Druid: Improved Leader of the PAck
			{
				if ( !playerTarget )
					break;

				if( !playerTarget->IsInFeralForm() || (
					playerTarget->GetShapeShift() != FORM_CAT &&
					playerTarget->GetShapeShift() != FORM_BEAR &&
					playerTarget->GetShapeShift() != FORM_DIREBEAR ) )
					break;

				uint32 max = playerTarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH );
				uint32 val = float2int32( ( ( playerTarget->FindAura( 34300 ) ) ? 0.04f : 0.02f ) * max );
				if (val)
					playerTarget->Heal(playerTarget,34299,(uint32)(val));
			}break;
		case 22845: // Druid: Frenzied Regeneration
			{
				if ( !playerTarget )
					break;

				if (!playerTarget->IsInFeralForm() ||
					(playerTarget->GetShapeShift() != FORM_BEAR &&
					playerTarget->GetShapeShift() != FORM_DIREBEAR))
					break;

				uint32 val = playerTarget->GetUInt32Value(UNIT_FIELD_POWER2);
				if (val > 100)
					val = 100;

				playerTarget->SetUInt32Value(UNIT_FIELD_POWER2, playerTarget->GetUInt32Value(UNIT_FIELD_POWER2) - val);
				if (val)
					playerTarget->Heal(playerTarget, 22845, ( playerTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * 0.003f ) * (val / 10) );

			}break;
		case 18562: //druid - swiftmend
			{
				uint32 new_dmg = 0;
				//consume rejuvenetaion and regrowth
				Aura* taura = NULL;
				taura = unitTarget->FindPositiveAuraByNameHash( SPELL_HASH_REGROWTH ); //Regrowth
				if( taura != NULL && taura->GetSpellProto() != NULL)
				{
					uint32 amplitude = taura->GetSpellProto()->EffectAmplitude[1] / 1000;
					if( !amplitude )
						amplitude = 3;

					//our hapiness is that we did not store the aura mod amount so we have to recalc it
					Spell* spell = new Spell( m_caster, taura->GetSpellProto(), false, NULLAURA );
					uint32 healamount = spell->CalculateEffect( 1, unitTarget );
					spell->Destruct();
					spell = NULLSPELL;
					new_dmg = healamount * 18 / amplitude;

					unitTarget->RemoveAura( taura );

					//do not remove flag if we still can cast it again
					if( !unitTarget->GetAuraSpellIDWithNameHash( SPELL_HASH_REJUVENATION ) )
					{
						unitTarget->RemoveFlag( UNIT_FIELD_AURASTATE, AURASTATE_FLAG_REJUVENATE );
						sEventMgr.RemoveEvents( unitTarget, EVENT_REJUVENATION_FLAG_EXPIRE );
					}
				}
				else
				{
					taura = NULL;
					taura = unitTarget->FindPositiveAuraByNameHash( SPELL_HASH_REJUVENATION );//Rejuvenation
					if( taura != NULL && taura->GetSpellProto() != NULL )
					{
						uint32 amplitude = taura->GetSpellProto()->EffectAmplitude[0] / 1000;
						if( !amplitude )
							amplitude = 3;

						//our hapiness is that we did not store the aura mod amount so we have to recalc it
						Spell* spell = new Spell( m_caster, taura->GetSpellProto(), false, NULLAURA );
						uint32 healamount = spell->CalculateEffect( 0, unitTarget );
						spell->Destruct();
						spell = NULLSPELL;
						new_dmg = healamount * 12 / amplitude;

						unitTarget->RemoveAura( taura );
						unitTarget->RemoveFlag( UNIT_FIELD_AURASTATE,AURASTATE_FLAG_REJUVENATE );
						sEventMgr.RemoveEvents( unitTarget,EVENT_REJUVENATION_FLAG_EXPIRE );
					}
				}

				if( new_dmg > 0 )
					Heal( (int32)new_dmg );
			}break;
		case 48743://death pact
			{
				if( p_caster == NULL || p_caster->GetSummon() == NULL)
					return;

				Heal( float2int32(float(p_caster->GetMaxHealth()) * 0.3f) );
			}break;
		case 50464:
			{
				if( m_caster == NULL )
					return;

				bool bonus = false;
				Aura* pAura = NULL;
				for(uint32 i = 0; i < MAX_POSITIVE_AURAS; i++)
				{
					pAura = unitTarget->m_auras[i];
					if( pAura != NULL && pAura->GetCaster() == m_caster )
					{
						if( pAura->m_spellProto->NameHash == SPELL_HASH_REJUVENATION ||
							pAura->m_spellProto->NameHash == SPELL_HASH_REGROWTH ||
							pAura->m_spellProto->NameHash == SPELL_HASH_LIFEBLOOM ||
							pAura->m_spellProto->NameHash == SPELL_HASH_WILD_GROWTH )
							bonus = true;
					}
				}
				if( bonus )
				{
					int32 new_dmg = damage + float2int32(damage*0.2f);
					Heal(new_dmg);
				}
				else
					Heal((int32)damage);
			}break;
		case 48153: // Guardian spirit
			{
				if( p_caster == NULL )
					return;

				Heal( float2int32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * (damage/100.0f) ));
			}break;
		case 20267: // judgement of light heal effect
			{
				if( u_caster != NULL )
				{
					Aura* aur = NULL;
					aur = u_caster->FindAura(20185);
					if( aur != NULL)
					{
						Unit* orgcstr = u_caster->FindAura(20185)->GetUnitCaster();
						if( orgcstr )
							Heal( float2int32(orgcstr->GetAP() * 0.10f + orgcstr->GetDamageDoneMod(SCHOOL_HOLY) * 0.10f) );
					}
				}
			}break;

		case 379: // Earth Shield
		case 54172: // Divine strom heal
		case 54968: // Glyph of Holy Light
			{
				Heal((int32)forced_basepoints[0]);
			}break;

		case 23880: // Bloodthirst
			{
				Heal( float2int32( unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) / 100.0f ) );
			}break;
		default:
			Heal((int32)damage);
			break;
		}
	}
}

void Spell::SpellEffectBind(uint32 i) // Innkeeper Bind
{
	if(!playerTarget)
		return;
	playerTarget->SetBindPoint(playerTarget->GetPositionX(),playerTarget->GetPositionY(),playerTarget->GetPositionZ(),playerTarget->GetMapId(),playerTarget->GetZoneId());
}

void Spell::SpellEffectQuestComplete(uint32 i) // Quest Complete
{
	if( playerTarget != NULL )
	{
		QuestLogEntry *qle;
		qle = playerTarget->GetQuestLogForEntry(GetSpellProto()->EffectMiscValue[i]);
		if( qle == NULL )
			return;

		qle->SendQuestComplete();
	}
}

//wand->
void Spell::SpellEffectWeapondamageNoschool(uint32 i) // Weapon damage + (no School)
{
	if( unitTarget == NULL  || u_caster == NULL )
		return;

	if( GetType() == SPELL_DMG_TYPE_RANGED && GetSpellProto()->speed > 0.0f )
		u_caster->Strike( unitTarget, RANGED, GetSpellProto(), 0, 0, 0, false, true );
	else
		u_caster->Strike( unitTarget, ( GetType() == SPELL_DMG_TYPE_RANGED ? RANGED : MELEE ), GetSpellProto(), damage, 0, 0, false, true );
}

void Spell::SpellEffectAddExtraAttacks(uint32 i) // Add Extra Attacks
{
	if( u_caster == NULL )
		return;

	if( ProcedOnSpell )
		u_caster->m_extraattacks[0] = ProcedOnSpell->procCharges;

	if( GetSpellProto()->procCharges > 0 )
		u_caster->m_extraattacks[0] = GetSpellProto()->procCharges;

	u_caster->m_extraattacks[1] += damage;
}

void Spell::SpellEffectDodge(uint32 i)
{
	//i think this actually enbles the skill to be able to dodge melee+ranged attacks
	//value is static and sets value directly which will be modified by other factors
	//this is only basic value and will be overwiten elsewhere !!!
//	if(playerTarget)
//		unitTarget->SetFloatValue(PLAYER_DODGE_PERCENTAGE,damage);
}

void Spell::SpellEffectParry(uint32 i)
{
	if(unitTarget != NULL )
		unitTarget->setcanperry(true);
}

void Spell::SpellEffectBlock(uint32 i)
{
	//i think this actually enbles the skill to be able to block melee+ranged attacks
	//value is static and sets value directly which will be modified by other factors
//	if(playerTarget)
//		unitTarget->SetFloatValue(PLAYER_BLOCK_PERCENTAGE,damage);
}

void Spell::SpellEffectCreateItem(uint32 i) // Create item
{
	if(!playerTarget)
		return;

	if(GetSpellProto()->EffectItemType[i] == 0)
		return;

	SlotResult slotresult;
	ItemPrototype *m_itemProto = ItemPrototypeStorage.LookupEntry( GetSpellProto()->EffectItemType[i] );
	if (!m_itemProto)
		return;

	if(GetSpellProto()->Id == 3286)
	{
		// Add a hearthstone if they don't have one
		if(!playerTarget->GetItemInterface()->GetItemCount(6948, true))
		{
			// We don't have a hearthstone. Add one.
			if(playerTarget->GetItemInterface()->CalculateFreeSlots(NULL) > 0)
			{
				Item* item = objmgr.CreateItem( 6948, playerTarget);
				if( playerTarget->GetItemInterface()->AddItemToFreeSlot(item) )
				{
					SlotResult * lr = playerTarget->GetItemInterface()->LastSearchResult();
					playerTarget->GetSession()->SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,1);
				}
				else
				{
					item->DeleteMe();
					item = NULLITEM;
				}
			}
		}return;
	}

	uint32 item_count = 0;
	if (m_itemProto->Class != ITEM_CLASS_CONSUMABLE || GetSpellProto()->SpellFamilyName != SPELLFAMILY_MAGE)
		item_count = damage;
	else if(playerTarget->getLevel() >= GetSpellProto()->spellLevel)
	{
		item_count = ((playerTarget->getLevel() - (GetSpellProto()->spellLevel-1))*damage);
		// These spells can only create one stack!
		if((m_itemProto->MaxCount > 0) && (item_count > m_itemProto->MaxCount))
			item_count = m_itemProto->MaxCount;
	}

	if(!item_count)
		item_count = damage;

	//conjure water ranks 7,8 & 9 and conjure food ranks 7 & 8 have different starting amounts
	// tailoring specializations get +1 cloth bonus
	switch(GetSpellProto()->Id)
	{
		case 27389: //Conjure Food 7
		case 10140: //Conjure Water 7
		case 37420: //Conjure Water 8
			{
				if(item_count <= 12)
					item_count += 8;
			}break;
		case 36686: //Shadowcloth
			if(playerTarget->HasSpell(26801))
				item_count += 1;
			break;
		case 26751: // Primal Mooncloth
			if(playerTarget->HasSpell(26798))
				item_count += 1;
			break;
		case 31373: //Spellcloth
			if(playerTarget->HasSpell(26797))
				item_count += 1;
			break;
	}

	skilllinespell* skill = objmgr.GetSpellSkill(GetSpellProto()->Id);
	if(skill)
	{
		// Alchemy Specializations
		// http://www.wowwiki.com/Alchemy#Alchemy_Specializations
		if ( skill->skilline == SKILL_ALCHEMY && Rand(15) )
		{
			//Potion Master and Elixer Master (Elixers and Flasks)
			if(( playerTarget->HasSpell(28675) && m_itemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_POTION ) ||
				( playerTarget->HasSpell(28677) && ( m_itemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_ELIXIR || m_itemProto->SubClass == ITEM_SUBCLASS_CONSUMABLE_FLASK ) ))
			{
				for(int x=0; x<5; x++)
				{
					SpellEntry *spellInfo;
					uint32 spellid = m_itemProto->Spells[x].Id;
					if( spellid )
					{
						spellInfo = dbcSpell.LookupEntry(spellid);
						if ( spellInfo )
						{
							item_count = item_count + rand() % 4 + 1;
							break;
						}
					}
				}
			}
			//Transmutation Master
			else if( playerTarget->HasSpell(28672) && GetSpellProto()->Category == 310 )
			{
				item_count = item_count + rand() % 4 + 1;
			}
		}

		// Profession Discoveries
		uint32 discovered_recipe = 0;
		std::set<ProfessionDiscovery*>::iterator itr = objmgr.ProfessionDiscoveryTable.begin();
		for ( ; itr != objmgr.ProfessionDiscoveryTable.end(); itr++ )
		{
			ProfessionDiscovery * pf = ( *itr );
			if ( pf != NULL && GetSpellProto()->Id == pf->SpellId && playerTarget->_GetSkillLineCurrent( skill->skilline ) >= pf->SkillValue && !playerTarget->HasSpell( pf->SpellToDiscover ) && Rand( pf->Chance ) )
			{
				discovered_recipe = pf->SpellToDiscover;
				break;
			}
		}
		// if something discovered learn playerTarget that recipe and broadcast message
		if ( discovered_recipe != 0 )
		{
			SpellEntry * se = dbcSpell.LookupEntry( discovered_recipe );
			if ( se != NULL )
			{
				playerTarget->addSpell( discovered_recipe );
				WorldPacket * data;
				char msg[256];
				sprintf( msg, "%sDISCOVERY! %s has discovered how to create %s.|r", MSG_COLOR_GOLD, playerTarget->GetName(), se->Name );
				data = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL,  msg, playerTarget->GetGUID(), 0 );
				playerTarget->GetMapMgr()->SendChatMessageToCellPlayers( playerTarget, data, 2, 1, LANG_UNIVERSAL, playerTarget->GetSession() );
				delete data;
			}
		}
	}

	// item count cannot be more than item unique value
	if(m_itemProto->Unique > 0 && item_count > m_itemProto->Unique)
		item_count = m_itemProto->Unique;

	if(playerTarget->GetItemInterface()->CanReceiveItem(m_itemProto, item_count, NULL)) //reversed since it sends >1 as invalid and 0 as valid
	{
		SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
		return;
	}

	if(!playerTarget->GetItemInterface()->AddItemById(GetSpellProto()->EffectItemType[i], (item_count > 1 ? item_count : 1 ), m_itemProto->RandomPropId ? m_itemProto->RandomPropId : 0, true, playerTarget))
		return;

	if(skill)
		DetermineSkillUp(skill->skilline);

	playerTarget->Cooldown_Add(GetSpellProto(), NULLITEM);
}

void Spell::SpellEffectWeapon(uint32 i)
{
	if( playerTarget == NULL )
		return;

	uint32 skill = 0;
	uint32 spell = 0;

	switch( GetSpellProto()->Id )
	{
	case 201:	// one-handed swords
		{
			skill = SKILL_SWORDS;
		}break;
	case 202:   // two-handed swords
		{
			skill = SKILL_2H_SWORDS;
		}break;
	case 203:   // Unarmed
		{
			skill = SKILL_UNARMED;
		}break;
	case 199:   // two-handed maces
		{
			skill = SKILL_2H_MACES;
		}break;
	case 198:   // one-handed maces
		{
			skill = SKILL_MACES;
		}break;
	case 197:   // two-handed axes
		{
			skill = SKILL_2H_AXES;
		}break;
	case 196:   // one-handed axes
		{
			skill = SKILL_AXES;
		}break;
	case 5011: // crossbows
		{
			skill = SKILL_CROSSBOWS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 227:   // staves
		{
			skill = SKILL_STAVES;
		}break;
	case 1180:  // daggers
		{
			skill = SKILL_DAGGERS;
		}break;
	case 200:   // polearms
		{
			skill = SKILL_POLEARMS;
		}break;
	case 15590: // fist weapons
		{
			skill = SKILL_UNARMED;
		}break;
	case 264:   // bows
		{
			skill = SKILL_BOWS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 266: // guns
		{
			skill = SKILL_GUNS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 2567:  // thrown
		{
			skill = SKILL_THROWN;
			spell = SPELL_RANGED_THROW;
		}break;
	case 5009:  // wands
		{
			skill = SKILL_WANDS;
			spell = SPELL_RANGED_GENERAL;
		}break;
	case 2382:  //Generic Weapon Spell
		{
			skill = SKILL_DODGE;
			spell = SPELL_ATTACK;
		}break;
	case 9125:  //Generic Block Spell
		{
			skill = SKILL_BLOCK;
		}break;
	default:
		{
			skill = 0;
			Log.Warning("Spell","Could not determine skill for spell id %d (SPELL_EFFECT_WEAPON)", GetSpellProto()->Id);
		}break;
	}

	if(skill)
	{
		if(spell)
			playerTarget->addSpell(spell);

		// if we do not have the skill line
		if(!playerTarget->_HasSkillLine(skill))
		{
			if(sWorld.StartLevel > 1)
				playerTarget->_AddSkillLine(skill, 5*sWorld.StartLevel, playerTarget->getLevel()*5);
			else
				playerTarget->_AddSkillLine(skill, 1, playerTarget->getLevel()*5);
		}
	}
}

void Spell::SpellEffectDefense(uint32 i)
{
	//i think this actually enbles the skill to be able to use defense
	//value is static and sets value directly which will be modified by other factors
	//this is only basic value and will be overwiten elsewhere !!!
//	if(unitTarget != NULL && playerTarget)
//		unitTarget->SetFloatValue(UNIT_FIELD_RESISTANCES,damage);
}

void Spell::SpellEffectPersistentAA(uint32 i) // Persistent Area Aura
{
	if(m_AreaAura == true || m_caster == NULL || !m_caster->IsInWorld())
		return;

	//create only 1 dyn object
	int32 dur = GetDuration();
	float r = GetRadius(i);

	//Note: this code seems to be useless
	//this must be only source point or dest point
	//this AREA aura it's apllied on area
	//it can'be on unit or self or item or object
	//uncomment it if i'm wrong
	//We are thinking in general so it might be useful later DK
	DynamicObject* dynObj = NULL;
	dynObj = m_caster->GetMapMgr()->CreateDynamicObject();
	if(dynObj == NULL)
		return;

	if(g_caster != NULL && g_caster->IsInWorld() && g_caster->m_summoner)
	{
		if (g_caster->GetType() == GAMEOBJECT_TYPE_TRAP && g_caster->m_summoner->dynObj != NULL)
			g_caster->m_summoner->dynObj->SetAliveDuration(1);

		if(!unitTarget)
		{
			dynObj->Create(g_caster->m_summoner, this, g_caster->GetPositionX(), g_caster->GetPositionY(),
				g_caster->GetPositionZ(), dur, r);
		}
		else
		{
			dynObj->Create(g_caster->m_summoner, this, unitTarget->GetPositionX(), unitTarget->GetPositionY(),
				unitTarget->GetPositionZ(), dur, r);
		}
		m_AreaAura = true;
		return;
	}

	switch(m_targets.m_targetMask)
	{
	case TARGET_FLAG_SELF:
		{
			dynObj->Create(u_caster, this,	m_caster->GetPositionX(),
				m_caster->GetPositionY(), m_caster->GetPositionZ(), dur,r);
		}break;
	case TARGET_FLAG_UNIT:
		{
			if( !unitTarget || !unitTarget->isAlive() )
				break;

			dynObj->Create(m_caster, this, unitTarget->GetPositionX(),
				unitTarget->GetPositionY(), unitTarget->GetPositionZ(),	dur, r);
		}break;
	case TARGET_FLAG_OBJECT:
		{
			if( !unitTarget || !unitTarget->isAlive() )
				break;

			dynObj->Create(m_caster, this, unitTarget->GetPositionX(),
				unitTarget->GetPositionY(), unitTarget->GetPositionZ(),	dur, r);
		}break;
	case TARGET_FLAG_SOURCE_LOCATION:
		{
			dynObj->SetInstanceID(m_caster->GetInstanceID());
			dynObj->Create(m_caster, this, m_targets.m_srcX,
				m_targets.m_srcY, m_targets.m_srcZ, dur,r);
		}break;
	case TARGET_FLAG_DEST_LOCATION:
		{
			dynObj->SetInstanceID(m_caster->GetInstanceID());
			dynObj->Create(m_caster, this,
				m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ,dur,r);
		}break;
	default:
		return;
	}

	if(u_caster != NULL)
	{
		if(GetSpellProto()->ChannelInterruptFlags > 0)
		{
			u_caster->SetUInt64Value(UNIT_FIELD_CHANNEL_OBJECT,dynObj->GetGUID());
			u_caster->SetUInt32Value(UNIT_CHANNEL_SPELL,GetSpellProto()->Id);
		}
	}
	m_AreaAura = true;
}

void Spell::SpellEffectSummon(uint32 i)
{
	if(u_caster == NULL )
		 return;

	SummonPropertiesEntry * spe =  NULL;
	spe = dbcSummonProps.LookupEntryForced( GetSpellProto()->EffectMiscValueB[i] );
	if( spe == NULL )
		return;

	m_summonProperties = spe;

	// Delete any objects in my slots. Slot 0 can have unlimited objects.
	if( spe->slot < 7 && u_caster->m_SummonSlots[ spe->slot ] != NULL )
	{
		u_caster->SummonExpireSlot(spe->slot);

		if( i_caster ) // Assume this is the 'toggle off' mode since we had one already.
			return;
	}

	switch( spe->Id )
	{
	case SUMMON_TYPE_POSSESSED:
		{
			SummonPossessed(i);
		}break;
	case SUMMON_TYPE_WILD:
	case SUMMON_TYPE_DEMON:
	case SUMMON_TYPE_WOLFS:
	case SUMMON_TYPE_GUARDIAN:
		{
			SummonGuardian(i);
		}break;
	case SUMMON_TYPE_TOTEM_1:
	case SUMMON_TYPE_TOTEM_2:
	case SUMMON_TYPE_TOTEM_3:
	case SUMMON_TYPE_TOTEM_4:
		{
			SummonTotem(i);
		}break;
	case SUMMON_TYPE_GHOUL:
	case SUMMON_TYPE_SUMMON:
		{
			SummonCreature(i);
		}break;
	case SUMMON_TYPE_CRITTER:
		{
			SummonNonCombatPet(i);
		}break;
	case SUMMON_TYPE_LIGHTWELL:
		{
			SummonLightwell(i);
		}break;
	case SUMMON_TYPE_TOTEM_COPY:
		{
			SummonTotemCopy(i);
		}break;
	default:
		{
			sLog.outError("Spell Id %u, has an invalid summon type %u report this to devs.", GetSpellProto()->Id, spe->Id);
			//SummonGuardian(i);
			break;
		}
	}
}

void Spell::SummonCreature(uint32 i) // Summon
{
	if( p_caster == NULL || !p_caster->IsInWorld())
		return;

	/* This is for summon water elemenal, etc */
	CreatureInfo * ci = CreatureNameStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	CreatureProto * cp = CreatureProtoStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if( !ci || !cp )
		return;

	float x, y, z;
	if( m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION && m_targets.m_destX && m_targets.m_destY && m_targets.m_destZ )
	{
		x = m_targets.m_destX;
		y = m_targets.m_destY;
		z = m_targets.m_destZ;
	}
	else
	{
		x = u_caster->GetPositionX();
		y = u_caster->GetPositionY();
		z = u_caster->GetPositionZ();
	}
	uint32 health = 0;
	uint32 count = GetSpellProto()->EffectBasePoints[i] + 1;
	if( m_summonProperties->unk2 & 2 ) // one, please.
	{
		count = 1;
		health = GetSpellProto()->EffectBasePoints[i] + 1;
	}

	for (uint32 j=0; j<count; j++)
	{
		float m_fallowAngle=-((float(M_PI)/2)*j);
		x += (GetRadius(i)*(cosf(m_fallowAngle+u_caster->GetOrientation())));
		y += (GetRadius(i)*(sinf(m_fallowAngle+u_caster->GetOrientation())));

		Creature* pCreature = NULLCREATURE;
		pCreature = p_caster->GetMapMgr()->CreateCreature(cp->Id);
		if(pCreature == NULLCREATURE)
			continue;
		pCreature->Load(cp, p_caster->GetMapMgr()->iInstanceMode, x, y, z, p_caster->GetOrientation());
		if(health)
		{
			pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH, health);
			pCreature->SetUInt32Value(UNIT_FIELD_HEALTH, health);
		}
		pCreature->GetAIInterface()->Init(pCreature,AITYPE_PET,MOVEMENTTYPE_NONE,u_caster);
		pCreature->GetAIInterface()->SetUnitToFollow(u_caster);
		pCreature->GetAIInterface()->SetUnitToFollowAngle(float(-(M_PI/2)));
		pCreature->GetAIInterface()->SetFollowDistance(3.0f);
		pCreature->SetUInt32Value(UNIT_FIELD_LEVEL, p_caster->getLevel());
		pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, p_caster->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
		pCreature->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, p_caster->GetGUID());
		pCreature->_setFaction();
		p_caster->SetUInt64Value(UNIT_FIELD_SUMMON, pCreature->GetGUID());

		if( m_summonProperties->slot < 7 )
		{
			//record our owner guid and slotid
			pCreature->SetSummonOwnerSlot(p_caster->GetGUID(),m_summonProperties->slot);
			p_caster->m_SummonSlots[ m_summonProperties->slot ] = pCreature;
		}

		if ( GetSpellProto()->EffectMiscValue[i] == 19668 ) //shadowfiend
		{
			float parent_bonus = (float)(p_caster->GetDamageDoneMod(SCHOOL_SHADOW)*0.065f);
			pCreature->SetFloatValue(UNIT_FIELD_MINDAMAGE, pCreature->GetFloatValue(UNIT_FIELD_MINDAMAGE) + parent_bonus);
			pCreature->SetFloatValue(UNIT_FIELD_MAXDAMAGE, pCreature->GetFloatValue(UNIT_FIELD_MAXDAMAGE) + parent_bonus);
			pCreature->BaseDamage[0] += parent_bonus;
			pCreature->BaseDamage[1] += parent_bonus;
		}

		pCreature->PushToWorld(p_caster->GetMapMgr());
		sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
	}
}

void Spell::SpellEffectLeap(uint32 i) // Leap
{
	if(m_caster == NULL )
		return;
	float radius = GetRadius(i);

	//FIXME: check for obstacles

	if(!p_caster)
		return;

	// remove movement impeding auras
	p_caster->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_ANY_DAMAGE_TAKEN);

	if(p_caster->m_bg && !p_caster->m_bg->HasStarted())
		return;

	// just in case
	for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
	{
		if( p_caster->m_auras[i] != NULL )
		{
			for(uint32 j = 0; j < 3; ++j)
			{
				if( p_caster->m_auras[i]->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_STUN ||
					p_caster->m_auras[i]->GetSpellProto()->EffectApplyAuraName[j] == SPELL_AURA_MOD_ROOT )
				{
					p_caster->RemoveAuraBySlot(i);
					break;
				}
			}
		}
	}

	if(p_caster->GetMapMgr() && !IS_INSTANCE(p_caster->GetMapId()) && p_caster->GetMapMgr()->CanUseCollision(p_caster))
	{
		float ori = m_caster->GetOrientation();
		float posX = m_caster->GetPositionX()+(radius*(cosf(ori)));
		float posY = m_caster->GetPositionY()+(radius*(sinf(ori)));
		float posZ;

		if( CollideInterface.GetFirstPoint(m_caster->GetMapId(), p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ() + p_caster->m_noseLevel, posX, posY, p_caster->GetPositionZ(), posX, posY, posZ, -1.5f) )
		{
			posZ = p_caster->GetCHeightForPosition(true, posX, posY, posZ);
			float diff = fabs(fabs(posZ) - fabs(m_caster->GetPositionZ()));
			if( diff <= 10.0f)
			{
				p_caster->blinked = true;
				p_caster->blinktimer = getMSTime()+5000;
				p_caster->SafeTeleport( p_caster->GetMapId(), p_caster->GetInstanceID(), posX, posY, posZ, m_caster->GetOrientation() );

				// reset heartbeat for a little while, 5 seconds maybe?
				p_caster->DelaySpeedHack( 5000 );
				++p_caster->m_heartbeatDisable;
				p_caster->z_axisposition = 0.0f;
			}
		}
		else
		{
			// either no objects in the way, or no wmo height
			posZ = p_caster->GetCHeightForPosition(true, posX, posY);
			float diff = fabs(fabs(posZ) - fabs(m_caster->GetPositionZ()));
			if( diff <= 10.0f)
			{
				p_caster->blinked = true;
				p_caster->blinktimer = getMSTime()+5000;
				p_caster->SafeTeleport( p_caster->GetMapId(), p_caster->GetInstanceID(), posX, posY, posZ, m_caster->GetOrientation() );

				// reset heartbeat for a little while, 5 seconds maybe?
				p_caster->DelaySpeedHack( 5000 );
				++p_caster->m_heartbeatDisable;
				p_caster->z_axisposition = 0.0f;
			}
		}

		if(p_caster->blinked)
			return;
	}

	p_caster->blinked = true;

	WorldPacket data(SMSG_MOVE_KNOCK_BACK, 50);
	data << u_caster->GetNewGUID();
	data << getMSTime();
	data << cosf(u_caster->GetOrientation()) << sinf(u_caster->GetOrientation());
	data << radius;
	data << float(-10.0f);
	p_caster->GetSession()->SendPacket(&data);

	// reset heartbeat for a little while, 2 seconds maybe?
	p_caster->DelaySpeedHack( 10000 );
	++p_caster->m_heartbeatDisable;
	p_caster->z_axisposition = 0.0f;
}

void Spell::SpellEffectEnergize(uint32 i) // Energize
{
	if( unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 modEnergy = 0;
	switch( GetSpellProto()->Id )
	{
	case 58883: //Rapid Recuperation
		{
			modEnergy = float2int32(0.01f * unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
		}break;
	case 57669: // replenishment
		{
			modEnergy = float2int32(0.0025f * unitTarget->GetUInt32Value(UNIT_FIELD_BASE_MANA));
		}break;
	case 31930:
		{
			modEnergy = float2int32(0.25f * u_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA));
		}break;
	case 31786:
		{
			if( ProcedOnSpell )
			{
				SpellEntry *motherspell=dbcSpell.LookupEntry(pSpellId);
				if(motherspell)
				{
					//heal amount from procspell (we only proced on a heal spell)
					uint32 healamt=0;
					if(ProcedOnSpell->Effect[0]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[0]==SPELL_EFFECT_SCRIPT_EFFECT)
						healamt=ProcedOnSpell->EffectBasePoints[0]+1;
					else if(ProcedOnSpell->Effect[1]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[1]==SPELL_EFFECT_SCRIPT_EFFECT)
						healamt=ProcedOnSpell->EffectBasePoints[1]+1;
					else if(ProcedOnSpell->Effect[2]==SPELL_EFFECT_HEAL || ProcedOnSpell->Effect[2]==SPELL_EFFECT_SCRIPT_EFFECT)
						healamt=ProcedOnSpell->EffectBasePoints[2]+1;
					modEnergy = (motherspell->EffectBasePoints[0]+1)*(healamt)/100;
				}
			}
		}break;
	case 2687:
		{
			modEnergy = damage;
			if( p_caster != NULL )
			{
				if(p_caster->HasSpell(12818))
					modEnergy += 60;
				else if(p_caster->HasSpell(12301))
					modEnergy += 30;
			}
		}break;
	case 20268:
	case 29442:
		{
			if( unitTarget != NULL )
			{
				modEnergy = float2int32(unitTarget->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) * 0.02f);
			}
		}break;
	case 20272:
	case 47755:
		{
			modEnergy = forced_basepoints[0];
		}break;
	default:
		{
			modEnergy = damage;
		}break;
	}

	u_caster->Energize(unitTarget, GetSpellProto()->logsId ? GetSpellProto()->logsId : (pSpellId ? pSpellId : GetSpellProto()->Id), modEnergy, GetSpellProto()->EffectMiscValue[i]);
}

void Spell::SpellEffectWeaponDmgPerc(uint32 i) // Weapon Percent damage
{
	if( unitTarget == NULL || u_caster == NULL )
		return;

	uint32 _type;
	if( GetType() == SPELL_DMG_TYPE_RANGED )
		_type = RANGED;
	else
	{
		if (GetSpellProto()->Flags4 & FLAGS4_OFFHAND)
			_type = OFFHAND;
		else
			_type = MELEE;
	}

	if( GetType() == SPELL_DMG_TYPE_MAGIC )
	{
		float fdmg = (float)CalculateDamage( u_caster, unitTarget, _type, GetSpellProto() );
		uint32 dmg = float2int32(fdmg*(float(damage/100.0f)));
		u_caster->SpellNonMeleeDamageLog(unitTarget, GetSpellProto()->Id, dmg, false, false, false);
	}
	else
	{
		u_caster->Strike( unitTarget, _type, GetSpellProto(), add_damage, damage, 0, false, false );
	}
}

void Spell::SpellEffectTriggerMissile(uint32 i) // Trigger Missile
{
	//Used by mortar team
	//Triggers area affect spell at destinatiom
	if(u_caster == NULL )
		return;

	uint32 spellid = GetSpellProto()->EffectTriggerSpell[i];
	if(spellid == 0)
		return;

	SpellEntry *spInfo = NULL;
	spInfo = dbcSpell.LookupEntry(spellid);
	if(spInfo == NULL )
		return;

	// Just send this spell where he wants :S
	u_caster->CastSpellAoF(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, spInfo, true);
}

void Spell::SpellEffectOpenLock(uint32 i) // Open Lock
{
	if( p_caster  == NULL )
		return;

	if( p_caster->InStealth() )
	{
		p_caster->RemoveAura(p_caster->m_stealth);
		p_caster->m_stealth = 0;
	}

	uint8 loottype = 0;

	uint32 locktype=GetSpellProto()->EffectMiscValue[i];
	switch(locktype)
	{
	case LOCKTYPE_PICKLOCK:
		{
			uint32 v = 0;
			uint32 lockskill = p_caster->_GetSkillLineCurrent(SKILL_LOCKPICKING);

			if(itemTarget)
			{
				if(!itemTarget->locked)
				return;

				Lock *lock = dbcLock.LookupEntry( itemTarget->GetProto()->LockId );
				if(!lock) return;
				for(int i=0; i < 8; ++i)
					if(lock->locktype[i] == 2 && lock->minlockskill[i] && lockskill >= lock->minlockskill[i])
					{
						v = lock->minlockskill[i];
						itemTarget->locked = false;
						itemTarget->SetFlag(ITEM_FIELD_FLAGS,4); // unlock
						DetermineSkillUp(SKILL_LOCKPICKING,v/5);
						break;
					}
			}
			else if(gameObjTarget)
			{
				if(gameObjTarget->GetState() == 0)
					return;

				Lock *lock = dbcLock.LookupEntryForced(gameObjTarget->GetInfo()->GetLockID());
				if( lock == NULL )
					return;

				for(int i=0; i < 8;++i)
				{
					if(lock->locktype[i] == 2 && lock->minlockskill[i] && lockskill >= lock->minlockskill[i])
					{
						v = lock->minlockskill[i];
						gameObjTarget->SetFlags(0);
						gameObjTarget->SetState(1);
						lootmgr.FillGOLoot(&gameObjTarget->m_loot,gameObjTarget->GetEntry(), (gameObjTarget->GetMapMgr() ? gameObjTarget->GetMapMgr()->iInstanceMode : 0), p_caster->GetTeam());
						loottype = LOOT_CORPSE;
						DetermineSkillUp(SKILL_LOCKPICKING,v/5);
						break;
					}
				}
			}
		}break;
	case LOCKTYPE_HERBALISM:
		{
			if(!gameObjTarget) return;

			uint32 v = gameObjTarget->GetGOReqSkill();
			bool bAlreadyUsed = false;

			if(Rand(100.0f)) // 3% chance to fail//why?
			{
				if( TO_PLAYER( m_caster )->_GetSkillLineCurrent( SKILL_HERBALISM ) < v )
				{
					//SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
					return;
				}
				else
				{
					if( gameObjTarget->m_loot.items.size() == 0 )
					{
						lootmgr.FillGOLoot(&gameObjTarget->m_loot,gameObjTarget->GetEntry(), (gameObjTarget->GetMapMgr() ? gameObjTarget->GetMapMgr()->iInstanceMode : 0), p_caster->GetTeam());
					}
					else
						bAlreadyUsed = true;
				}
				loottype = LOOT_GATHERING;
			}
			else
				SendCastResult(SPELL_FAILED_TRY_AGAIN);

			//Skill up
			if(!bAlreadyUsed) //Avoid cheats with opening/closing without taking the loot
				DetermineSkillUp(SKILL_HERBALISM,v/5);
		}break;
	case LOCKTYPE_MINING:
		{
			if(!gameObjTarget) return;

			uint32 v = gameObjTarget->GetGOReqSkill();
			bool bAlreadyUsed = false;

			if( Rand( 100.0f ) ) // 3% chance to fail//why?
			{
				if( TO_PLAYER( m_caster )->_GetSkillLineCurrent( SKILL_MINING ) < v )
				{
					//SendCastResult(SPELL_FAILED_LOW_CASTLEVEL);
					return;
				}
				else if( gameObjTarget->m_loot.items.size() == 0 )
				{
					lootmgr.FillGOLoot(&gameObjTarget->m_loot,gameObjTarget->GetEntry(), (gameObjTarget->GetMapMgr() ? gameObjTarget->GetMapMgr()->iInstanceMode : 0), p_caster->GetTeam());
				}
				else
					bAlreadyUsed = true;

				loottype = LOOT_GATHERING;
			}
			else
				SendCastResult(SPELL_FAILED_TRY_AGAIN);

			//Skill up
			if(!bAlreadyUsed) //Avoid cheats with opening/closing without taking the loot
				DetermineSkillUp(SKILL_MINING,v/5);
		}break;
	case LOCKTYPE_SLOW_OPEN: // used for BG go's
		{
			if(!gameObjTarget )
				return;

			if(p_caster->m_bgFlagIneligible)
				return;

			if(p_caster && p_caster->m_bg)
				if(p_caster->m_bg->HookSlowLockOpen(gameObjTarget,p_caster,this))
					return;

			sHookInterface.OnSlowLockOpen(gameObjTarget,p_caster);

			uint32 spellid = 23932;
			if(gameObjTarget->GetInfo()->RawData.ListedData[10])
				spellid = gameObjTarget->GetInfo()->RawData.ListedData[10];

			SpellEntry*en = dbcSpell.LookupEntry(spellid);
			Spell* sp = CREATESPELL(p_caster,en,true,NULLAURA);
			SpellCastTargets tgt;
			tgt.m_unitTarget = gameObjTarget->GetGUID();
			sp->prepare(&tgt);
		}break;
	case LOCKTYPE_QUICK_CLOSE:
		{
			if(!gameObjTarget )
				return;

			gameObjTarget->EventCloseDoor();
		}break;
	default://not profession
		{
			if(!gameObjTarget)
				return;

			if(sQuestMgr.OnActivateQuestGiver(gameObjTarget, p_caster))
				return;

			if( gameObjTarget->GetType() == GAMEOBJECT_TYPE_GOOBER)
			{
 				CALL_GO_SCRIPT_EVENT(gameObjTarget, OnActivate)(TO_PLAYER(p_caster));
				CALL_INSTANCE_SCRIPT_EVENT( gameObjTarget->GetMapMgr(), OnGameObjectActivate )( gameObjTarget, p_caster );
			}

			if(sQuestMgr.OnGameObjectActivate(p_caster, gameObjTarget))
			{
				p_caster->UpdateNearbyGameObjects();
				return;
			}

			if(gameObjTarget->m_loot.items.size() == 0)
			{
				lootmgr.FillGOLoot(&gameObjTarget->m_loot,gameObjTarget->GetEntry(), (gameObjTarget->GetMapMgr() ? gameObjTarget->GetMapMgr()->iInstanceMode : 0), p_caster->GetTeam());
			}
			loottype = LOOT_CORPSE;
		}break;
	};
	if( gameObjTarget != NULL && gameObjTarget->GetType() == GAMEOBJECT_TYPE_CHEST)
		TO_PLAYER( m_caster )->SendLoot( gameObjTarget->GetGUID(), gameObjTarget->GetMapId(), loottype );
}

void Spell::SpellEffectOpenLockItem(uint32 i)
{
	Unit* caster = u_caster;
	if(caster == NULL && i_caster != NULL )
		caster = i_caster->GetOwner();

	if( caster == NULL || gameObjTarget  == NULL || !gameObjTarget->IsInWorld())
		return;

	if( caster->IsPlayer() && sQuestMgr.OnGameObjectActivate( (TO_PLAYER(caster)), gameObjTarget ) )
		TO_PLAYER(caster)->UpdateNearbyGameObjects();

	CALL_GO_SCRIPT_EVENT(gameObjTarget, OnActivate)(TO_PLAYER(caster));
	CALL_INSTANCE_SCRIPT_EVENT( gameObjTarget->GetMapMgr(), OnGameObjectActivate )( gameObjTarget, TO_PLAYER( caster ) );
	gameObjTarget->SetState(0);

	if( gameObjTarget->GetType() == GAMEOBJECT_TYPE_CHEST)
	{
		lootmgr.FillGOLoot(&gameObjTarget->m_loot,gameObjTarget->GetEntry(), (gameObjTarget->GetMapMgr() ? gameObjTarget->GetMapMgr()->iInstanceMode : 0), p_caster->GetTeam());
		if(gameObjTarget->m_loot.items.size() > 0)
		{
			TO_PLAYER(caster)->SendLoot(gameObjTarget->GetGUID(), gameObjTarget->GetMapId(), LOOT_CORPSE);
		}
		gameObjTarget->SetFlags(1);
	}

	if( gameObjTarget->GetType() == GAMEOBJECT_TYPE_DOOR)
	{
		gameObjTarget->SetFlags(33);

		if(gameObjTarget->GetMapMgr()->GetMapInfo()->type==INSTANCE_NULL)//dont close doors for instances
			sEventMgr.AddEvent(gameObjTarget,&GameObject::EventCloseDoor, EVENT_GAMEOBJECT_DOOR_CLOSE,10000,1,0);
	}

	sEventMgr.AddEvent(gameObjTarget, &GameObject::Despawn, (uint32)0, (uint32)1, EVENT_GAMEOBJECT_ITEM_SPAWN, 6*60*1000, 1, 0);
}

void Spell::SpellEffectProficiency(uint32 i)
{
	uint32 skill = 0;
	skilllinespell* skillability = objmgr.GetSpellSkill(GetSpellProto()->Id);
	if (skillability)
		skill = skillability->skilline;
	skilllineentry* sk = dbcSkillLine.LookupEntry(skill);
	if(skill)
	{
		if(playerTarget != NULL)
		{
			if(!playerTarget->_HasSkillLine(skill))
			{
				// Don't add skills to players logging in.
				/*if((GetSpellProto()->Attributes & 64) && playerTarget->m_TeleportState == 1)
					return;*/

				if(sk && sk->type == SKILL_TYPE_WEAPON)
				{
					if(sWorld.StartLevel > 1)
						playerTarget->_AddSkillLine(skill, 5*sWorld.StartLevel, 5*playerTarget->getLevel());
					else
						playerTarget->_AddSkillLine(skill, 1, 5*playerTarget->getLevel());
				}
				else
					playerTarget->_AddSkillLine(skill, 1, 1);
			}
		}
	}
}

void Spell::SpellEffectSendEvent(uint32 i) //Send Event
{
	//This is mostly used to trigger events on quests or some places

	uint32 spellId = GetSpellProto()->Id;

	switch(spellId)
	{

	// Place Loot
	case 25720: // Places the Bag of Gold at the designated Drop-Off Point.
		{

		}break;

	// Item - Cleansing Vial DND
	case 29297: // Empty the vial near the Bones of Aggonar to cleanse the waters of their demonic taint.
		{
			QuestLogEntry *en=NULL;
			en = p_caster->GetQuestLogForEntry(9427);
			if(en== NULL)
				return;
			en->SendQuestComplete();
		}break;

	/*//Warlock: Summon Succubus Quest
	case 8674:
	case 9223:
	case 9224:
		{
			CreatureInfo * ci = CreatureNameStorage.LookupEntry(5677);
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(5677);
			if( !ci || !cp )
				return;

			pCreature* pCreature = p_caster->GetMapMgr()->CreateCreature(cp->Id);
			pCreature->Load(cp, p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), p_caster->GetOrientation());
			pCreature->GetAIInterface()->Init(pCreature,AITYPE_AGRO,MOVEMENTTYPE_NONE);
			pCreature->GetAIInterface()->taunt(p_caster,true);
			pCreature->_setFaction();
			pCreature->PushToWorld(p_caster->GetMapMgr());
			sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE,60000, 1, 0);
		}break;

	//Warlock: Summon Voidwalker Quest
	case 30208:
	case 9221:
	case 9222:
	case 7728:
		{
			CreatureInfo * ci = CreatureNameStorage.LookupEntry(5676);
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(5676);
			if( !ci || !cp )
				return;

			Creature* pCreature = p_caster->GetMapMgr()->CreateCreature(cp->Id);
			pCreature->Load(cp, p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), p_caster->GetOrientation());
			pCreature->GetAIInterface()->Init(pCreature,AITYPE_AGRO,MOVEMENTTYPE_NONE);
			pCreature->GetAIInterface()->taunt(p_caster,true);
			pCreature->_setFaction();
			pCreature->PushToWorld(p_caster->GetMapMgr());
			sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE,60000, 1, 0);
		}break;

	//Warlock: Summon Felhunter Quest
	case 8712:
		{
			CreatureInfo * ci = CreatureNameStorage.LookupEntry(6268);
			CreatureProto * cp = CreatureProtoStorage.LookupEntry(6268);
			if( !ci || !cp )
				return;

			Creature* pCreature = p_caster->GetMapMgr()->CreateCreature(cp->Id);
			pCreature->Load(cp, p_caster->GetPositionX(), p_caster->GetPositionY(), p_caster->GetPositionZ(), p_caster->GetOrientation());
			pCreature->GetAIInterface()->Init(pCreature,AITYPE_AGRO,MOVEMENTTYPE_NONE);
			pCreature->GetAIInterface()->taunt(p_caster,true);
			pCreature->_setFaction();
			pCreature->PushToWorld(p_caster->GetMapMgr());
			sEventMgr.AddEvent(pCreature, &Creature::SafeDelete, EVENT_CREATURE_REMOVE_CORPSE,60000, 1, 0);
		}break;*/
	}
}

void Spell::SpellEffectApplyAA(uint32 i) // Apply Area Aura
{
	if(!unitTarget || !unitTarget->isAlive() || u_caster != unitTarget)
		return;

	Aura* pAura;
	std::map<uint32,Aura* >::iterator itr = unitTarget->tmpAura.find(GetSpellProto()->Id);
	if(itr == unitTarget->tmpAura.end())
	{
		pAura = (new Aura(GetSpellProto(),GetDuration(),m_caster,unitTarget));

		unitTarget->tmpAura [GetSpellProto()->Id]= pAura;

		float r=GetRadius(i);
		r *= r;

		if( u_caster->IsPlayer() || ( u_caster->GetTypeId() == TYPEID_UNIT && (TO_CREATURE(u_caster)->IsTotem() || TO_CREATURE(u_caster)->IsPet()) ) )
		{
			sEventMgr.AddEvent(pAura, &Aura::EventUpdatePlayerAA, r, EVENT_AREAAURA_UPDATE, GetSpellProto()->area_aura_update_interval, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		}
		else if( u_caster->GetTypeId() == TYPEID_UNIT )
		{
			sEventMgr.AddEvent(pAura, &Aura::EventUpdateCreatureAA, r, EVENT_AREAAURA_UPDATE, GetSpellProto()->area_aura_update_interval, 0,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
			pAura->m_creatureAA = true;
		}
	}
	else
		pAura = itr->second;

	pAura->AddMod(GetSpellProto()->EffectApplyAuraName[i],damage,GetSpellProto()->EffectMiscValue[i],i);
}

void Spell::SpellEffectLearnSpell(uint32 i) // Learn Spell
{
	if(playerTarget == 0 && unitTarget != NULL && unitTarget->IsPet())
	{
		SpellEffectLearnPetSpell(i);
		return;
	}

	if( GetSpellProto()->Id == 483 || GetSpellProto()->Id == 55884 )		// "Learning"
	{
		if( i_caster == NULL || p_caster == NULL)
			return;

		uint32 spellid = 0;
		for(int i = 0; i < 5; i++)
		{
			if( i_caster->GetProto()->Spells[i].Trigger == LEARNING && i_caster->GetProto()->Spells[i].Id != 0 )
			{
				spellid = i_caster->GetProto()->Spells[i].Id;
				break;
			}
		}

		if( !spellid || !dbcSpell.LookupEntryForced(spellid) )
			return;

		// learn me!
		p_caster->addSpell( spellid );

		// no normal handler
		return;
	}

	if(playerTarget!=NULL)
	{
		/*if(u_caster && isHostile(playerTarget, u_caster))
			return;*/

		uint32 spellToLearn = GetSpellProto()->EffectTriggerSpell[i];
		playerTarget->addSpell(spellToLearn);
		//smth is wrong here, first we add this spell to player then we may cast it on player...
		SpellEntry *spellinfo = dbcSpell.LookupEntry(spellToLearn);
		//remove specializations
		switch(spellinfo->Id)
		{
		case 26801: //Shadoweave Tailoring
			playerTarget->removeSpell(26798); //Mooncloth Tailoring
			playerTarget->removeSpell(26797); //Spellfire Tailoring
			break;
		case 26798: // Mooncloth Tailoring
			playerTarget->removeSpell(26801); //Shadoweave Tailoring
			playerTarget->removeSpell(26797); //Spellfire Tailoring
			break;
		case 26797: //Spellfire Tailoring
			playerTarget->removeSpell(26801); //Shadoweave Tailoring
			playerTarget->removeSpell(26798); //Mooncloth Tailoring
			break;
		case 10656: //Dragonscale Leatherworking
			playerTarget->removeSpell(10658); //Elemental Leatherworking
			playerTarget->removeSpell(10660); //Tribal Leatherworking
			break;
		case 10658: //Elemental Leatherworking
			playerTarget->removeSpell(10656); //Dragonscale Leatherworking
			playerTarget->removeSpell(10660); //Tribal Leatherworking
			break;
		case 10660: //Tribal Leatherworking
			playerTarget->removeSpell(10656); //Dragonscale Leatherworking
			playerTarget->removeSpell(10658); //Elemental Leatherworking
			break;
		case 28677: //Elixir Master
			playerTarget->removeSpell(28675); //Potion Master
			playerTarget->removeSpell(28672); //Transmutation Maste
			break;
		case 28675: //Potion Master
			playerTarget->removeSpell(28677); //Elixir Master
			playerTarget->removeSpell(28672); //Transmutation Maste
			break;
		case 28672: //Transmutation Master
			playerTarget->removeSpell(28675); //Potion Master
			playerTarget->removeSpell(28677); //Elixir Master
			break;
		case 20219: //Gnomish Engineer
			playerTarget->removeSpell(20222); //Goblin Engineer
			break;
		case 20222: //Goblin Engineer
			playerTarget->removeSpell(20219); //Gnomish Engineer
			break;
		case 9788: //Armorsmith
			playerTarget->removeSpell(9787); //Weaponsmith
			playerTarget->removeSpell(17039); //Master Swordsmith
			playerTarget->removeSpell(17040); //Master Hammersmith
			playerTarget->removeSpell(17041); //Master Axesmith
			break;
		case 9787: //Weaponsmith
			playerTarget->removeSpell(9788); //Armorsmith
			break;
		case 17041: //Master Axesmith
			playerTarget->removeSpell(9788); //Armorsmith
			playerTarget->removeSpell(17040); //Master Hammersmith
			playerTarget->removeSpell(17039); //Master Swordsmith
			break;
		case 17040: //Master Hammersmith
			playerTarget->removeSpell(9788); //Armorsmith
			playerTarget->removeSpell(17039); //Master Swordsmith
			playerTarget->removeSpell(17041); //Master Axesmith
			break;
		case 17039: //Master Swordsmith
			playerTarget->removeSpell(9788); //Armorsmith
			playerTarget->removeSpell(17040); //Master Hammersmith
			playerTarget->removeSpell(17041); //Master Axesmith
			break;
		}
		for(uint32 i=0;i<3;++i)
			if(spellinfo->Effect[i] == SPELL_EFFECT_WEAPON ||
			   spellinfo->Effect[i] == SPELL_EFFECT_PROFICIENCY ||
			   spellinfo->Effect[i] == SPELL_EFFECT_DUAL_WIELD )
			{
				Spell* sp = CREATESPELL(unitTarget, spellinfo, true, NULLAURA);
				SpellCastTargets targets;
				targets.m_unitTarget = unitTarget->GetGUID();
				targets.m_targetMask = TARGET_FLAG_UNIT;
				sp->prepare(&targets);
				break;
			}
		return;
	}

	// if we got here... try via pet spells..
	SpellEffectLearnPetSpell(i);
}

void Spell::SpellEffectSpellDefense(uint32 i)
{
	//used to enable this ability. We use it all the time ...
}

void Spell::SpellEffectLearnPetSpell(uint32 i)
{
	if(unitTarget && unitTarget->IsPet() && p_caster)
	{
		if (GetSpellProto()->EffectTriggerSpell[i])
		{
			Pet* pPet = TO_PET( unitTarget );
			if(pPet->IsSummonedPet())
				p_caster->AddSummonSpell(unitTarget->GetEntry(), GetSpellProto()->EffectTriggerSpell[i]);

			pPet->AddSpell( dbcSpell.LookupEntry( GetSpellProto()->EffectTriggerSpell[i] ), true );
		}
	}
}

void Spell::SpellEffectDispel(uint32 i) // Dispel
{
	if(u_caster == NULL || unitTarget == NULL )
		return;

	Aura* aur = NULL;;
	uint32 start,end;
	if(isAttackable(u_caster,unitTarget))
	{
		start=0;
		end=MAX_POSITIVE_AURAS;
	}
	else
	{
		start=MAX_POSITIVE_AURAS;
		end=MAX_AURAS;
	}

	WorldPacket data(SMSG_SPELLDISPELLOG, 16);

	for(uint32 x=start;x<end;x++)
	if(unitTarget->m_auras[x])
	{
		aur = unitTarget->m_auras[x];
		//Nothing can dispel resurrection sickness;
		if(aur != NULL && !aur->IsPassive() && !(aur->GetSpellProto()->Attributes & ATTRIBUTES_IGNORE_INVULNERABILITY))
		{
			int32 resistchance = 0;
			Unit* caster = aur->GetUnitCaster();
			if( caster )
				SM_FIValue(caster->SM[SMT_RESIST_DISPEL][0], &resistchance, aur->GetSpellProto()->SpellGroupType);

			if( !Rand(resistchance) )
			{
				if(GetSpellProto()->DispelType == DISPEL_ALL)
				{
					unitTarget->HandleProc( PROC_ON_DISPEL_AURA_VICTIM, NULL, u_caster , GetSpellProto(), aur->GetSpellId() );
					data.clear();
					data << m_caster->GetNewGUID();
					data << unitTarget->GetNewGUID();
					data << (uint32)1;//probably dispel type
					data << aur->GetSpellId();
					m_caster->SendMessageToSet(&data,true);
					aur->AttemptDispel( u_caster );
					if(!--damage)
						return;
				}
				else if(aur->GetSpellProto()->DispelType == GetSpellProto()->EffectMiscValue[i])
				{
					if( (aur->GetSpellProto()->NameHash != SPELL_HASH_ICE_BARRIER &&
						 aur->GetSpellProto()->NameHash != SPELL_HASH_DIVINE_SHIELD) ||
						 GetSpellProto()->NameHash == SPELL_HASH_MASS_DISPEL )
					{
						unitTarget->HandleProc( PROC_ON_DISPEL_AURA_VICTIM, NULL, u_caster, GetSpellProto(), aur->GetSpellId() );
						data.clear();
						data << m_caster->GetNewGUID();
						data << unitTarget->GetNewGUID();
						data << (uint32)1;
						data << aur->GetSpellId();
						m_caster->SendMessageToSet(&data,true);
						aur->AttemptDispel( u_caster );
						if(!--damage)
							return;
					}
				}
			}
			else if( !--damage )
				return;
		}
	}
}

void Spell::SpellEffectLanguage(uint32 i)
{
	if(m_caster->GetTypeId() != TYPEID_PLAYER)
		return;

	Player* pPlayer = TO_PLAYER( m_caster );

	if(!pPlayer->GetSession()->HasGMPermissions())
	{
		if(pPlayer->GetTeam() == ALLIANCE)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_COMMON ) )
				pPlayer->_AddSkillLine( SKILL_LANG_COMMON, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_COMMON );

		if(pPlayer->GetTeam() == HORDE)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_ORCISH ) )
				pPlayer->_AddSkillLine( SKILL_LANG_ORCISH, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_ORCISH );

		if(pPlayer->getRace() == RACE_DWARF)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_DWARVEN ) )
				pPlayer->_AddSkillLine( SKILL_LANG_DWARVEN, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_DWARVEN );

		if(pPlayer->getRace() == RACE_NIGHTELF)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_DARNASSIAN ) )
				pPlayer->_AddSkillLine( SKILL_LANG_DARNASSIAN, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_DARNASSIAN );

		if(pPlayer->getRace() == RACE_UNDEAD)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_GUTTERSPEAK ) )
				pPlayer->_AddSkillLine( SKILL_LANG_GUTTERSPEAK, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_GUTTERSPEAK );

		if(pPlayer->getRace() == RACE_TAUREN)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_TAURAHE ) )
				pPlayer->_AddSkillLine( SKILL_LANG_TAURAHE, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_TAURAHE );

		if(pPlayer->getRace() == RACE_GNOME)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_GNOMISH ) )
				pPlayer->_AddSkillLine( SKILL_LANG_GNOMISH, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_GNOMISH );

		if(pPlayer->getRace() == RACE_TROLL)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_TROLL ) )
				pPlayer->_AddSkillLine( SKILL_LANG_TROLL, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_TROLL );

		if(pPlayer->getRace() == RACE_BLOODELF)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_THALASSIAN ) )
				pPlayer->_AddSkillLine( SKILL_LANG_THALASSIAN, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_THALASSIAN );

		if(pPlayer->getRace() == RACE_DRAENEI)
		{
			if( !pPlayer->_HasSkillLine( SKILL_LANG_DRAENEI ) )
				pPlayer->_AddSkillLine( SKILL_LANG_DRAENEI, 300, 300 );
		}
		else
			pPlayer->_RemoveSkillLine( SKILL_LANG_DRAENEI );
	}
}

void Spell::SpellEffectDualWield(uint32 i)
{
	if(p_caster == NULL)
		return;

	if( !p_caster->_HasSkillLine( SKILL_DUAL_WIELD ) )
		 p_caster->_AddSkillLine( SKILL_DUAL_WIELD, 1, 1 );

		// Increase it by one
		//dual wield is 1/1 , it never increases it's not even displayed in skills tab

	//note: probably here must be not caster but unitVictim
}

void Spell::SummonGuardian(uint32 i) // Summon Guardian
{
	if ( u_caster == NULL || !u_caster->GetMapMgr())
		return;

	uint32 cr_entry = GetSpellProto()->EffectMiscValue[i];
	uint32 level = u_caster->getLevel();

	if( i != 0 && GetSpellProto()->Effect[i-1] == SPELL_EFFECT_APPLY_AURA && GetSpellProto()->EffectApplyAuraName[i-1] == SPELL_AURA_MOUNTED )
	{
		CreatureProto * cp = NULL;
		cp = CreatureProtoStorage.LookupEntry(cr_entry);
		if(cp == NULL )
			return;

		if(cp->vehicle_entry < 0)
		{
			Log.Error("SpellEffect", "Creature %u, has invalid vehicle_entry, please check your creature_proto table", cr_entry);
			return;
		}

		Vehicle* veh = NULLVEHICLE;
		veh = u_caster->GetMapMgr()->CreateVehicle( cr_entry );
		if(veh == NULLVEHICLE)
			return;

		veh->m_CreatedFromSpell = true;
		veh->m_mountSpell = GetSpellProto()->EffectBasePoints[i];
		veh->Load( cp, u_caster->GetPositionX(), u_caster->GetPositionY(), u_caster->GetPositionZ(), u_caster->GetOrientation());
		veh->SetInstanceID( u_caster->GetInstanceID() );
		veh->PushToWorld( u_caster->GetMapMgr() ); // we can do this safely since we're in the mapmgr's context
	}
	else
	{
		//we only have 7 summon slots
		damage = damage > 6 ? 6 : damage;

		// it's health., or a fucked up infernal.
		if( m_summonProperties == NULL || (m_summonProperties->unk2 & 2 || m_summonProperties->Id == 711))
			damage = 1;

		if(cr_entry == 31216)	// mirror image
			damage = 3;

		if(cr_entry == 24207)	// army of the dead
			damage = 6;

		if(cr_entry == 29264)	// Spirit Wolf
		{
			CreatureInfo *ci = CreatureNameStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
			Pet *summon = objmgr.CreatePet();
			summon->SetInstanceID(p_caster->GetInstanceID());
			summon->SetPosition(p_caster->GetPosition(), true);
			summon->CreateAsSummon(cr_entry, ci, TO_CREATURE(summon), p_caster, GetSpellProto(), 4, GetDuration());
			summon->AddSpell(dbcSpell.LookupEntry(58877), true); // Spirit Hunt
			summon->AddSpell(dbcSpell.LookupEntry(58875), true); // Spirit walk
			summon->AddSpell(dbcSpell.LookupEntry(58857), true); // Twin Howl
			summon->AddSpell(dbcSpell.LookupEntry(58861), true); // Spirit Bash

			Pet *summon2 = objmgr.CreatePet();
			summon2->SetInstanceID(p_caster->GetInstanceID());
			summon2->SetPosition(p_caster->GetPosition(), true);
			summon2->CreateAsSummon(cr_entry, ci, TO_CREATURE(summon2), p_caster, GetSpellProto(), 4, GetDuration());
			summon2->GetAIInterface()->SetUnitToFollowAngle(float(-(M_PI/2)));
			summon2->AddSpell(dbcSpell.LookupEntry(58877), true); // Spirit Hunt
			summon2->AddSpell(dbcSpell.LookupEntry(58875), true); // Spirit walk
			summon2->AddSpell(dbcSpell.LookupEntry(58857), true); // Twin Howl
			summon2->AddSpell(dbcSpell.LookupEntry(58861), true); // Spirit Bash
			return;
		};

		if(cr_entry == 510)
		{
			CreatureInfo *ci = CreatureNameStorage.LookupEntry(cr_entry);
			Pet *summon = objmgr.CreatePet();
			summon->SetInstanceID(p_caster->GetInstanceID());
			summon->SetPosition(p_caster->GetPosition(), true);
			summon->CreateAsSummon(cr_entry, ci, TO_CREATURE(summon), p_caster, GetSpellProto(), 4, GetDuration());
			summon->GetAIInterface()->SetUnitToFollowAngle(float(-(M_PI/2)));
			summon->AddSpell(dbcSpell.LookupEntry(58877), true); // Spirit Hunt
			summon->AddSpell(dbcSpell.LookupEntry(58875), true); // Spirit walk
			summon->AddSpell(dbcSpell.LookupEntry(58857), true); // Twin Howl
			summon->AddSpell(dbcSpell.LookupEntry(58861), true); // Spirit Bash
			return;
		};

		//Spread spawns equally around summoner
		float angle_for_each_spawn = damage ? - float(M_PI * 2.0f)/damage : - float(M_PI * 2.0f);

		for( uint8 d = 0; d < 7; d++ )
		{
			//skip already filled slots
			if(u_caster->m_SummonSlots[ d ] != NULL)
				continue;

			float m_fallowAngle = angle_for_each_spawn * d;
			int32 duration = GetDuration();
			duration = duration <= 3600000 ? duration : 3600000; //limit to 1hr max.
			if ( g_caster )
			{
				if(g_caster->m_summoner == NULL)
					return;
				u_caster = g_caster->m_summoner;
				u_caster->m_SummonSlots[d] = TO_CREATURE(g_caster->CreateTemporaryGuardian(cr_entry, duration, m_fallowAngle, u_caster, d));
			}
			else
				u_caster->m_SummonSlots[d] = TO_CREATURE(u_caster->CreateTemporaryGuardian(cr_entry, duration, m_fallowAngle, level, d));

			if(u_caster->m_SummonSlots[d] != NULL)
				u_caster->m_SummonSlots[d]->SetSummonOwnerSlot(u_caster->GetGUID(),d);
			break;
		}
	}
}

void Spell::SpellEffectSkillStep(uint32 i) // Skill Step
{
	Player* target = NULL;
	if(u_caster == NULL)
		return;

	// Check targets
	if( m_targets.m_unitTarget )
		target = u_caster->GetMapMgr()->GetPlayer(uint32(m_targets.m_unitTarget));
	else
		target = TO_PLAYER( m_caster );

	if( target == NULL )
		return;

	uint32 skill = GetSpellProto()->EffectMiscValue[i];
	if( skill == 242 )
		skill = SKILL_LOCKPICKING; // somehow for lockpicking misc is different than the skill :s

	skilllineentry* sk = NULL;
	sk = dbcSkillLine.LookupEntry( skill );
	if( sk == NULL )
		return;

	uint32 max = 1;
	switch( sk->type )
	{
		case SKILL_TYPE_PROFESSION:
		case SKILL_TYPE_SECONDARY:
			max = damage * 75;
			break;
		case SKILL_TYPE_WEAPON:
			max = 5 * target->getLevel();
			break;
		case SKILL_TYPE_CLASS:
		case SKILL_TYPE_ARMOR:
			if( skill == SKILL_LOCKPICKING )
				max = damage * 75;
			else
				max = 1;
			break;
		default: //u cant learn other types in game
			return;
	};

	if( target->_HasSkillLine( skill ) )
		target->_ModifySkillMaximum( skill, max );
	else
	{
		if( sk->type == SKILL_TYPE_PROFESSION )
			target->ModUnsigned32Value( PLAYER_CHARACTER_POINTS+1, -1 );

		if( skill == SKILL_RIDING )
			target->_AddSkillLine( skill, max, max );
		else
			target->_AddSkillLine( skill, 1, max );
	}

	//professions fix, for unknow reason when u learn profession it
	//does not teach find herbs for herbalism etc. moreover there is no spell
	//in spell.dbc that would teach u this. It means blizz does it in some tricky way too
	switch( skill )
	{
	case SKILL_ALCHEMY:
		target->addSpell( 2330 );//Minor Healing Potion
		target->addSpell( 2329 );//Elixir of Lion's Strength
		target->addSpell( 7183 );//Elixir of Minor Defense
		break;
	case SKILL_ENCHANTING:
		target->addSpell( 7418 );//Enchant Bracer - Minor Health
		target->addSpell( 7428 );//Enchant Bracer - Minor Deflection
		target->addSpell( 7421 );//Runed Copper Rod
		target->addSpell( 13262 );//Disenchant
		break;
	case SKILL_HERBALISM:
		target->addSpell( 2383 );//find herbs
		break;
	case SKILL_MINING:
		target->addSpell( 2657 );//smelt copper
		target->addSpell( 2656 );//smelting
		target->addSpell( 2580 );//find minerals
		break;
	case SKILL_FIRST_AID:
		target->addSpell( 3275 );//Linen Bandage
		break;
	case SKILL_TAILORING:
		target->addSpell( 2963 );//Bolt of Linen Cloth
		target->addSpell( 2387 );//Linen Cloak
		target->addSpell( 2393 );//White Linen Shirt
		target->addSpell( 3915 );//Brown Linen Shirt
		target->addSpell( 12044 );//Simple Linen Pants
		break;
	case SKILL_LEATHERWORKING:
		target->addSpell( 2149 );//Handstitched Leather Boots
		target->addSpell( 2152 );//Light Armor Kit
		target->addSpell( 2881 );//Light Leather
		target->addSpell( 7126 );//Handstitched Leather Vest
		target->addSpell( 9058 );//Handstitched Leather Cloak
		target->addSpell( 9059 );//Handstitched Leather Bracers
		break;
	case SKILL_ENGINERING:
		target->addSpell( 3918 );//Rough Blasting Powder
		target->addSpell( 3919 );//Rough Dynamite
		target->addSpell( 3920 );//Crafted Light Shot
		break;
	case SKILL_COOKING:
		target->addSpell( 2538 );//Charred Wolf Meat
		target->addSpell( 2540 );//Roasted Boar Meat
		target->addSpell( 818 );//Basic Campfire
		target->addSpell( 8604 );//Herb Baked Egg
		break;
	case SKILL_BLACKSMITHING:
		target->addSpell( 2660 );//Rough Sharpening Stone
		target->addSpell( 2663 );//Copper Bracers
		target->addSpell( 12260 );//Rough Copper Vest
		target->addSpell( 2662 );//Copper Chain Pants
		target->addSpell( 3115 );//Rough Weightstone
		break;
	case SKILL_JEWELCRAFTING:
		target->addSpell( 25255 );// Delicate Copper Wire
		target->addSpell( 25493 );// Braided Copper Ring
		target->addSpell( 26925 );// Woven Copper Ring
		target->addSpell( 32259 );// Rough Stone Statue
		break;
	case SKILL_INSCRIPTION:
		target->addSpell( 51005 );// Milling
		target->addSpell( 48116 );// Scroll of Spirit
		target->addSpell( 48114 );// Scroll of Intellect
		target->addSpell( 45382 );// Scroll of Stamina
		target->addSpell( 52738 );// Ivory Ink
		break;
	}
}

void Spell::SpellEffectDetect(uint32 i)
{
	if( u_caster == NULL )
		return;
	/* Crow:
	Makes me afraid to see what this us used for.
	Notes below...
	*/

	// Crow: We'll just do a visibility update....
	u_caster->UpdateVisibility();
}

void Spell::SpellEffectSummonObject(uint32 i)
{
	if( u_caster == NULL )
		return;

	uint32 entry = GetSpellProto()->EffectMiscValue[i];

	uint32 mapid = u_caster->GetMapId();
	float px = u_caster->GetPositionX();
	float py = u_caster->GetPositionY();
	float pz = u_caster->GetPositionZ();
	float orient = m_caster->GetOrientation();
	float posx = 0,posy = 0,posz = 0;

	if( entry == GO_FISHING_BOBBER && p_caster)
	{
		float co = cos( orient );
		float si = sin( orient );
		MapMgr* map = m_caster->GetMapMgr();
		Spell* spell = u_caster->GetCurrentSpell();

		float r;
		for( r = 20; r > 10; r-- )
		{
			posx = px+r*co;
			posy = py+r*si;
			if( !(map->GetWaterType( posx, posy ) & 1) )//water
				continue;
			posz = map->GetWaterHeight( posx, posy );
			if( posz > map->GetLandHeight( posx, posy ) )//water
				break;
		}

		posx = px + r * co;
		posy = py + r * si;

		// Todo / Fix me: This should be loaded / cached
		uint32 zone = p_caster->GetPlayerAreaID();
		if( zone == 0 ) // If the player's area ID is 0, use the zone ID instead
			zone = p_caster->GetZoneId();

		spell->SendChannelStart( 20000 ); // 30 seconds

		GameObject* go = u_caster->GetMapMgr()->CreateGameObject(GO_FISHING_BOBBER);
		if( go == NULL || !go->CreateFromProto( GO_FISHING_BOBBER, mapid, posx, posy, posz, orient, 0.0f, 0.0f, 0.0f, 0.0f ))
			return;

		go->SetUInt32Value( GAMEOBJECT_FLAGS, 0 );
		go->SetByte(GAMEOBJECT_BYTES_1, 0, 0 );
		go->SetUInt64Value( OBJECT_FIELD_CREATED_BY, m_caster->GetGUID() );
		u_caster->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, go->GetGUID() );

		go->SetInstanceID( m_caster->GetInstanceID() );
		go->PushToWorld( m_caster->GetMapMgr() );

		if( lootmgr.IsFishable( zone ) ) // Only set a 'splash' if there is any loot in this area / zone
		{
			uint32 seconds = RandomUInt(17) + 2;
			sEventMgr.AddEvent( go, &GameObject::FishHooked, TO_PLAYER( m_caster ), EVENT_GAMEOBJECT_FISH_HOOKED, seconds * 1000, 1, 0 );
		}
		sEventMgr.AddEvent( go, &GameObject::EndFishing, TO_PLAYER( m_caster ),true, EVENT_GAMEOBJECT_END_FISHING, uint32(GetDuration() + 1000), 1, 0 );
		p_caster->SetSummonedObject( go );
	}
	else
	{//portal
		posx=px;
		posy=py;
		GameObjectInfo * goI = GameObjectNameStorage.LookupEntry(entry);
		if(!goI)
		{
			if( p_caster != NULL )
			{
				sChatHandler.BlueSystemMessage(p_caster->GetSession(),
				"non-existant gameobject %u tried to be created by SpellEffectSummonObject. Report to devs!", entry);
			}
			return;
		}

		GameObject* go= NULL;
		go = u_caster->GetMapMgr()->CreateGameObject(entry);
		if( go == NULL )
			return;

		if( m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION )
		{
			px = m_targets.m_destX;
			py = m_targets.m_destY;
			pz = m_targets.m_destZ;
		}
		if( !go->CreateFromProto(entry, mapid, posx, posy, pz, orient, 0.0f, 0.0f, 0.0f, 0.0f) )
			return;

		go->SetInstanceID(m_caster->GetInstanceID());
		go->SetState(1);
		go->SetUInt64Value(OBJECT_FIELD_CREATED_BY, m_caster->GetGUID());
		go->PushToWorld(m_caster->GetMapMgr());
		go->ExpireAndDelete(GetDuration());
		if(p_caster)
		{
			if(entry ==17032)//this is a portal
			{
				//enable it for party only
				go->SetByte(GAMEOBJECT_BYTES_1, 0, 0);

				//disable by default
				WorldPacket *pkt = go->BuildFieldUpdatePacket(GAMEOBJECT_BYTES_1, 1);
				SubGroup * pGroup = p_caster->GetGroup() ? p_caster->GetGroup()->GetSubGroup(p_caster->GetSubGroup()) : NULL;

				if(pGroup)
				{
					p_caster->GetGroup()->Lock();
					for(GroupMembersSet::iterator itr = pGroup->GetGroupMembersBegin();
						itr != pGroup->GetGroupMembersEnd(); itr++)
					{
						if((*itr)->m_loggedInPlayer && m_caster != (*itr)->m_loggedInPlayer)
							(*itr)->m_loggedInPlayer->GetSession()->SendPacket(pkt);
					}
					p_caster->GetGroup()->Unlock();
				}
				delete pkt;
			}
			else if(entry == 36727 || entry == 177193) // Portal of doom
			{
				Player* pTarget = NULL;
				pTarget = p_caster->GetMapMgr()->GetPlayer((uint32)p_caster->GetSelection());
				if(pTarget == NULL)
					return;

				go->m_ritualmembers[0] = p_caster->GetLowGUID();
				go->SetGOui32Value(GO_UINT32_M_RIT_CASTER, p_caster->GetLowGUID());
				go->SetGOui32Value(GO_UINT32_M_RIT_TARGET, pTarget->GetLowGUID());
				go->SetGOui32Value(GO_UINT32_RIT_SPELL, GetSpellProto()->Id);
			}
			else if(entry == 194108) // Ritual of Summoning
			{
				go->m_ritualmembers[0] = p_caster->GetLowGUID();
				go->SetGOui32Value(GO_UINT32_M_RIT_CASTER, p_caster->GetLowGUID());
				go->SetGOui32Value(GO_UINT32_RIT_SPELL, GetSpellProto()->Id);
			}
			else if( entry == 186811 || entry == 193062 ) // Ritual of refreshment
			{
				go->m_ritualmembers[0] = p_caster->GetLowGUID();
				go->SetGOui32Value(GO_UINT32_M_RIT_CASTER, p_caster->GetLowGUID());
				go->SetGOui32Value(GO_UINT32_RIT_SPELL, GetSpellProto()->Id);
			}
			else if( entry == 181622 || entry == 193168 ) // Ritual of Souls
			{
				go->m_ritualmembers[0] = p_caster->GetLowGUID();
				go->SetGOui32Value(GO_UINT32_M_RIT_CASTER, p_caster->GetLowGUID());
				go->SetGOui32Value(GO_UINT32_RIT_SPELL, GetSpellProto()->Id);
			}
			else
				go->charges = 10;
			p_caster->SetSummonedObject(go);
		}
	}
}

void Spell::SpellEffectEnchantItem(uint32 i) // Enchant Item Permanent
{
	if( itemTarget == NULL || p_caster == NULL )
		return;
	EnchantEntry * Enchantment = NULL;
	Enchantment = dbcEnchant.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(Enchantment == NULL )
		return;
	//Start Scroll Creation
	if( itemTarget->GetEntry() == 38682 || itemTarget->GetEntry() == 39349 || itemTarget->GetEntry() == 37602 || itemTarget->GetEntry() == 39350 || itemTarget->GetEntry() == 43145 || itemTarget->GetEntry() == 43146 )
	{
		Item* newItem = NULL;
		Item* add = NULL;
		uint8 slot;
		uint32 itemid;
		SlotResult slotresult;

		ItemPrototype *m_itemProto = NULL;
		// Scroll Creation
		ScrollCreation * sc = ScrollCreationStorage.LookupEntry( GetSpellProto()->Id );
		if(sc)
		{
			m_itemProto = ItemPrototypeStorage.LookupEntry( sc->ItemId );
			itemid	=	sc->ItemId;

			if (!m_itemProto)
				 return;
			if(itemid == 0)
				return;
			uint32 item_count = 0;
			if(p_caster->GetItemInterface()->CanReceiveItem(m_itemProto, item_count, NULL))
			{
				SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
				return;
			}

			slot = 0;
			add = p_caster->GetItemInterface()->FindItemLessMax(itemid,1, false);
			if (add == NULL )
			{
				slotresult = p_caster->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
				if(!slotresult.Result)
				{
					SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
					return;
				}

				newItem =objmgr.CreateItem(itemid,p_caster);
				if(newItem == NULL)
					return;
				newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
				newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count);

				if(p_caster->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
				{
					p_caster->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,item_count);
				}
				else
				{
					newItem->DeleteMe();
					newItem = NULLITEM;
				}
				DetermineSkillUp(SKILL_ENCHANTING);
			}
			else
			{
				//scale item_count down if total stack will be more than 20
				if(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count > 20)
				{
					uint32 item_count_filled;
					item_count_filled = 20 - add->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
					add->SetCount(20);
					add->m_isDirty = true;

					slotresult = p_caster->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
					if(!slotresult.Result)
						item_count = item_count_filled;
					else
					{
						newItem =objmgr.CreateItem(itemid,p_caster);
						if(newItem == NULL)
							return;
						newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
						newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count - item_count_filled);
						if(!p_caster->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
						{
							newItem->DeleteMe();
							newItem = NULLITEM;
							item_count = item_count_filled;
						}
						else
							p_caster->GetSession()->SendItemPushResult(newItem, true, false, true, true, slotresult.ContainerSlot, slotresult.Slot, item_count-item_count_filled);
					}
				}
				else
				{
					add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count);
					add->m_isDirty = true;
					p_caster->GetSession()->SendItemPushResult(add, true,false,true,false,p_caster->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,item_count);
				}
				DetermineSkillUp(SKILL_ENCHANTING);
			}
		}
	}
	//End of Scroll Creation
	//remove Vellum
	if( itemTarget->GetEntry() == 38682 || itemTarget->GetEntry() == 39349 || itemTarget->GetEntry() == 37602 || itemTarget->GetEntry() == 39350 || itemTarget->GetEntry() == 43145 || itemTarget->GetEntry() == 43146 )
	{
		p_caster->GetItemInterface()->RemoveItemAmt(itemTarget->GetEntry(),1);
		return;
	}

	WorldSession* session = p_caster->GetSession();
	if(session->GetPermissionCount() > 0)
		sWorld.LogGM(session, "enchanted item for %s", itemTarget->GetOwner()->GetName());

	//remove other perm enchantment that was enchanted by profession
	itemTarget->RemoveProfessionEnchant();
	int32 Slot = itemTarget->AddEnchantment(Enchantment, 0, true, true, false, 0);
	if(Slot < 0)
		return; // Apply failed

	DetermineSkillUp(SKILL_ENCHANTING);
	itemTarget->m_isDirty = true;
}

void Spell::SpellEffectEnchantItemTemporary(uint32 i)  // Enchant Item Temporary
{
	if(itemTarget == NULL || p_caster == NULL )
		return;

	uint32 Duration = damage > 1 ? damage : 1800;

	EnchantEntry * Enchantment = dbcEnchant.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(!Enchantment)
		return;

	itemTarget->RemoveEnchantment(1);
	int32 Slot = itemTarget->AddEnchantment(Enchantment, Duration, false, true, false, 1);
	if(Slot < 0)
		return; // Apply failed

	skilllinespell* skill = objmgr.GetSpellSkill(GetSpellProto()->Id);
	if(skill)
		DetermineSkillUp(skill->skilline,itemTarget->GetProto()->ItemLevel);

	itemTarget->m_isDirty = true;
}

void Spell::SpellEffectAddPrismaticSocket(uint32 i)
{
	if( p_caster == NULL)
		return;

	if(!itemTarget)
		return;

	EnchantEntry* pEnchant = dbcEnchant.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(!pEnchant)
		return;

	bool add_socket = false;
	for(uint8 i = 0; i < 3; i++)
	{
		if(pEnchant->type[i] == 8)
		{
			add_socket = true;
			break;
		}
	}

	if(!add_socket) // Wrong spell.
		return;

	// Item can be in trade slot and have owner diff. from caster
	Player* item_owner = itemTarget->GetOwner();
	if(!item_owner)
		return;

	if(itemTarget->GetMaxSocketsCount() >= 3)
	{
		SendCastResult(SPELL_FAILED_MAX_SOCKETS);
		return;
	}

	itemTarget->RemoveProfessionEnchant();
	itemTarget->AddEnchantment(pEnchant, 0, true, true, false, 6); // 6 is profession slot.
}

bool isExotic(uint32 family)
{
	switch(family)
	{
	case FAMILY_WORM:
	case FAMILY_RHINO:
	case FAMILY_CHIMAERA:
	case FAMILY_SILITHID:
	case FAMILY_DEVILSAUR:
	case FAMILY_CORE_HOUND:
	case FAMILY_SPIRIT_BEAST:
		return true;
	}
	return false;
}

void Spell::SpellEffectTameCreature(uint32 i)
{
	Creature* tame = NULL;
	tame = (unitTarget->IsCreature() ? TO_CREATURE(unitTarget) : NULLCREATURE);
	if(tame== NULL)
		return;

	CreatureFamilyEntry *cf = dbcCreatureFamily.LookupEntry(tame->GetCreatureInfo()->Family);
	uint8 result = SPELL_CANCAST_OK;

	if(!tame || !p_caster || !p_caster->isAlive() || !tame->isAlive() || p_caster->getClass() != HUNTER )
		result = SPELL_FAILED_BAD_TARGETS;
	else if(!tame->GetCreatureInfo())
		result = SPELL_FAILED_BAD_TARGETS;
	else if(tame->GetCreatureInfo()->Type != BEAST)
		result = SPELL_FAILED_BAD_TARGETS;
	else if(tame->getLevel() > p_caster->getLevel())
		result = SPELL_FAILED_HIGHLEVEL;
	else if(p_caster->GeneratePetNumber() == 0)
		result = SPELL_FAILED_BAD_TARGETS;
	else if(!cf || cf && !cf->tameable)
		result = SPELL_FAILED_BAD_TARGETS;
	else if(isExotic(cf->ID) && !p_caster->m_BeastMaster)
		result = SPELL_FAILED_BAD_TARGETS;
	else if(p_caster->GetSummon() || p_caster->GetUnstabledPetNumber())
		result = SPELL_FAILED_ALREADY_HAVE_SUMMON;

	if(result != SPELL_CANCAST_OK)
	{
		SendCastResult(result);
		return;
	}

	// Remove target
	tame->GetAIInterface()->HandleEvent(EVENT_LEAVECOMBAT, p_caster, 0);

	Pet* pPet = objmgr.CreatePet();
	pPet->SetInstanceID(p_caster->GetInstanceID());
	pPet->SetPosition(p_caster->GetPosition(), true);
	pPet->CreateAsSummon(tame->GetEntry(), tame->GetCreatureInfo(), tame, p_caster, NULL, 2, 0);

	// Add removal event.
	sEventMgr.AddEvent(tame, &Creature::Despawn, uint32(1), tame->GetRespawnTime(), EVENT_CORPSE_DESPAWN, 5, 0, 0);
}

void Spell::SpellEffectSummonPet(uint32 i) //summon - pet
{
	if(GetSpellProto()->Id == 883)
	{
		// "Call Pet" spell
		if(p_caster== NULL )
			return;

		if(p_caster->GetSummon() != 0)
		{
			p_caster->GetSession()->SendNotification("You already have a pet summoned.");
			return;
		}

		uint8 petno = p_caster->GetUnstabledPetNumber();

		if(petno)
		{
			p_caster->SpawnPet(petno);
		}
		else
		{
			WorldPacket data(SMSG_AREA_TRIGGER_MESSAGE, 50);
			data << uint32(0) << "You do not have any pets to call." << uint8(0);
			p_caster->GetSession()->SendPacket(&data);
		}
		return;
	}

	//uint32 entryId = GetSpellProto()->EffectMiscValue[i];

	//VoidWalker:torment, sacrifice, suffering, consume shadows
	//Succubus:lash of pain, soothing kiss, seduce , lesser invisibility
	//felhunter:	 Devour Magic,Paranoia,Spell Lock,	Tainted Blood

	if( p_caster == NULL || p_caster->getClass() != WARLOCK)
		return;

	// remove old pet
	Pet* old = NULL;
	old = TO_PLAYER(m_caster)->GetSummon();
	if(old != NULL)
		old->Dismiss(false);

	CreatureInfo *ci = CreatureNameStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(ci)
	{
		//if demonic sacrifice auras are still active, remove them
		//uint32 spids[] = { 18789, 18790, 18791, 18792, 35701, 0 };
		//p_caster->RemoveAuras(spids);
		p_caster->RemoveAura(18789);
		p_caster->RemoveAura(18790);
		p_caster->RemoveAura(18791);
		p_caster->RemoveAura(18792);
		p_caster->RemoveAura(35701);

		Pet* summon = objmgr.CreatePet();
		if(summon == NULL)
			return;
		summon->SetInstanceID(m_caster->GetInstanceID());
		summon->SetPosition(m_caster->GetPosition(), true);
		summon->CreateAsSummon(GetSpellProto()->EffectMiscValue[i], ci, TO_CREATURE(summon), p_caster, GetSpellProto(), 1, 0);
		if( u_caster->IsPvPFlagged() )
			summon->SetPvPFlag();
	}
}

void Spell::SpellEffectWeapondamage( uint32 i ) // Weapon damage +
{
	if( unitTarget == NULL || u_caster == NULL )
		return;

	//Hackfix for Mangle
	if( GetSpellProto()->NameHash == SPELL_HASH_MANGLE__CAT_ && u_caster->IsPlayer() )
		TO_PLAYER( u_caster )->AddComboPoints( unitTarget->GetGUID(), 1 );

	// Hacky fix for druid spells where it would "double attack".
	if( GetSpellProto()->Effect[2] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE || GetSpellProto()->Effect[1] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE )
	{
		add_damage += damage;
		return;
	}

	switch(GetSpellProto()->Id)
	{
	case 60103: // Lava Lash
		{
			if(p_caster != NULL)
			{
				Item* offhandweapon = NULL;
				ItemInterface * ii = p_caster->GetItemInterface();
				if(ii && (offhandweapon = ii->GetInventoryItem(INVENTORY_SLOT_NOT_SET, EQUIPMENT_SLOT_OFFHAND)))
				{
					bool hasenchantment = false;
					uint32 flame[10] = {3, 4, 5, 523, 1665, 1666, 2634, 3779, 3780, 3781};
					for(uint8 i = 0; i < 10; i++)
					{
						if(offhandweapon->HasEnchantment(flame[i]))
						{
							hasenchantment = true;
							break;
						}
					}

					if(hasenchantment)
					{
						uint32 bonus = 125;
						if(u_caster && u_caster->HasAura(55444)) // Glyph of Lava Lash
							bonus += 10;
						damage = uint32(float(damage * bonus) / 100);
					}
				}
			}
		}
	}

	uint32 _type;
	if( GetType() == SPELL_DMG_TYPE_RANGED )
		_type = RANGED;
	else
	{
		if (GetSpellProto()->Flags4 & FLAGS4_OFFHAND)
			_type =  OFFHAND;
		else
			_type = MELEE;
	}
	u_caster->Strike( unitTarget, _type, GetSpellProto(), damage, 0, 0, false, true );
}

void Spell::SpellEffectPowerBurn(uint32 i) // power burn
{
	if( unitTarget == NULL ||!unitTarget->isAlive() || unitTarget->GetPowerType() != POWER_TYPE_MANA)
		return;

	//Resilience reduction
	if(playerTarget)
	{
		float dmg_reduction_pct = 2.2f * playerTarget->CalcRating( PLAYER_RATING_MODIFIER_SPELL_CRIT_RESILIENCE ) / 100.0f;
		if( dmg_reduction_pct > 0.33f )
			dmg_reduction_pct = 0.33f; // 3.0.3
		damage = float2int32( damage - damage * dmg_reduction_pct );
	}

	//this is pct...
	int32 mana = float2int32( unitTarget->GetUInt32Value( UNIT_FIELD_POWER1 ) * (damage/100.0f));
	if( GetSpellProto()->Id == 8129 )
		if( mana > u_caster->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) * 0.26 )
			mana = int32(u_caster->GetUInt32Value( UNIT_FIELD_MAXPOWER1 ) * 0.26);

	unitTarget->ModUnsigned32Value(UNIT_FIELD_POWER1,-mana);

	float coef = GetSpellProto()->EffectMultipleValue[i]; // damage per mana burned
	if(u_caster)
	{
		SM_FFValue(u_caster->SM[SMT_MULTIPLE_VALUE][0], &coef, GetSpellProto()->SpellGroupType);
		SM_PFValue(u_caster->SM[SMT_MULTIPLE_VALUE][1], &coef, GetSpellProto()->SpellGroupType);
	}
	mana = float2int32((float)mana * coef);

	m_caster->SpellNonMeleeDamageLog(unitTarget,GetSpellProto()->Id, mana, pSpellId==0,true);
}

void Spell::SpellEffectThreat(uint32 i) // Threat
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	bool chck = unitTarget->GetAIInterface()->modThreatByPtr(u_caster,damage);
	if(chck == false)
		unitTarget->GetAIInterface()->AttackReaction(u_caster,1,0);
}

void Spell::SpellEffectTriggerSpell(uint32 i) // Trigger Spell
{
	if(unitTarget == NULL || m_caster == NULL )
		return;

	SpellEntry *spe = dbcSpell.LookupEntryForced(GetSpellProto()->EffectTriggerSpell[i]);
	if(spe == NULL )
		return;

	if(GetSpellProto() == spe) // Infinite loop fix.
		return;

	Spell* sp = new Spell( m_caster,spe,true,NULLAURA);
	memcpy(sp->forced_basepoints, GetSpellProto()->EffectBasePoints, sizeof(uint32)*3);
	SpellCastTargets tgt((spe->procflags2 & PROC_TARGET_SELF) ? m_caster->GetGUID() : unitTarget->GetGUID());
	sp->prepare(&tgt);
}

void Spell::SpellEffectHealthFunnel(uint32 i) // Health Funnel
{
//	if(unitTarget == NULL || !unitTarget->isAlive() || !unitTarget->IsPet())
//		return;

	//does not exist
}

void Spell::SpellEffectPowerFunnel(uint32 i) // Power Funnel
{
//	if(unitTarget == NULL || !unitTarget->isAlive() || !unitTarget->IsPet())
//		return;

	//does not exist
}

void Spell::SpellEffectHealMaxHealth(uint32 i)   // Heal Max Health
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	uint32 dif = unitTarget->GetUInt32Value( UNIT_FIELD_MAXHEALTH ) - unitTarget->GetUInt32Value( UNIT_FIELD_HEALTH );
	if( !dif )
	{
		SendCastResult( SPELL_FAILED_ALREADY_AT_FULL_HEALTH );
		return;
	}

	if( unitTarget->IsPlayer())
	{
		 SendHealSpellOnPlayer( TO_PLAYER( m_caster ), playerTarget, dif, false, 0, pSpellId ? pSpellId : GetSpellProto()->Id );
	}
	unitTarget->ModUnsigned32Value( UNIT_FIELD_HEALTH, dif );
}

void Spell::SpellEffectInterruptCast(uint32 i) // Interrupt Cast
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	// can't apply stuns/fear/polymorph/root etc on boss
	if(unitTarget->GetTypeId() == TYPEID_UNIT)
	{
		Creature* c = TO_CREATURE( unitTarget );
		if (c && c->GetCreatureInfo() && (c->GetCreatureInfo()->Rank == ELITE_WORLDBOSS || c->GetCreatureInfo()->Flags1 & CREATURE_FLAGS1_BOSS))
			return;
	}
	// FIXME:This thing prevent target from spell casting too but cant find.
	uint32 school=0;
	if( unitTarget->GetCurrentSpell() != NULL && (unitTarget->GetCurrentSpell()->getState() == SPELL_STATE_PREPARING || unitTarget->GetCurrentSpell()->getState() == SPELL_STATE_CASTING) && unitTarget->GetCurrentSpell() != this )
	{
		school=unitTarget->GetCurrentSpell()->GetSpellProto()->School;
		unitTarget->InterruptCurrentSpell();
		if(school)//prevent from casts in this school
		{
			unitTarget->SchoolCastPrevent[school] = GetDuration() + getMSTime();
		}
	}

}

void Spell::SpellEffectDistract(uint32 i) // Distract
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	//spellId 1725 Distract:Throws a distraction attracting the all monsters for ten sec's
	if(m_targets.m_destX != 0.0f || m_targets.m_destY != 0.0f || m_targets.m_destZ != 0.0f)
	{
//		unitTarget->GetAIInterface()->MoveTo(m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, 0);
		int32 Stare_duration=GetDuration();
		if(Stare_duration>30*60*1000)
			Stare_duration=10000;//if we try to stare for more then a half an hour then better not stare at all :P (bug)
		float newo=unitTarget->calcRadAngle(unitTarget->GetPositionX(),unitTarget->GetPositionY(),m_targets.m_destX,m_targets.m_destY);
		unitTarget->GetAIInterface()->StopMovement(Stare_duration);
		unitTarget->SetFacing(newo);
	}

	//Smoke Emitter 164870
	//Smoke Emitter Big 179066
	//Unit Field Target of
}

void Spell::SpellEffectPickpocket(uint32 i) // pickpocket
{
	if( unitTarget == NULL || p_caster == NULL || unitTarget->GetTypeId() != TYPEID_UNIT)
		return;

	//Show random loot based on roll,
	Creature* target = TO_CREATURE( unitTarget );
	if(target->IsPickPocketed() || (target->GetCreatureInfo() && target->GetCreatureInfo()->Type != HUMANOID))
	{
		SendCastResult(SPELL_FAILED_TARGET_NO_POCKETS);
		return;
	}

	lootmgr.FillPickpocketingLoot(&TO_CREATURE(unitTarget)->m_loot,unitTarget->GetEntry());

	uint32 _rank = TO_CREATURE(unitTarget)->GetCreatureInfo() ? TO_CREATURE(unitTarget)->GetCreatureInfo()->Rank : 0;
	unitTarget->m_loot.gold = float2int32((_rank+1) * unitTarget->getLevel() * (RandomUInt(5) + 1) * sWorld.getRate(RATE_MONEY));

	p_caster->SendLoot(unitTarget->GetGUID(), unitTarget->GetMapId(), LOOT_GATHERING);
	target->SetPickPocketed(true);
}

void Spell::SpellEffectAddFarsight(uint32 i) // Add Farsight
{
	if( p_caster == NULL )
		return;

	DynamicObject* dyn = NULL;
	dyn = p_caster->GetMapMgr()->CreateDynamicObject();
	if(dyn == NULL)
		return;

	dyn->Create(p_caster, this, m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, GetDuration(), GetRadius(i));
	dyn->SetUInt32Value(OBJECT_FIELD_TYPE, 65);
	dyn->SetUInt32Value(DYNAMICOBJECT_BYTES, 0x80000002);
	dyn->AddToWorld(p_caster->GetMapMgr());
	p_caster->SetUInt64Value(PLAYER_FARSIGHT, dyn->GetGUID());
	p_caster->SetUInt32Value(PLAYER_FARSIGHT, dyn->GetLowGUID());
	p_caster->GetMapMgr()->ChangeFarsightLocation(p_caster, m_targets.m_destX, m_targets.m_destY, true);
}

void Spell::SpellEffectResetTalents(uint32 i) // Used by Trainers.
{
	if( !playerTarget )
		return;

	playerTarget->Reset_Talents();
}

void Spell::SummonPossessed(uint32 i)
{
	if(!p_caster || GetSpellProto()->Id != 126)
		return;
	uint32 entry = GetSpellProto()->EffectMiscValue[i];
	CreatureInfo *ci = CreatureNameStorage.LookupEntry(entry);
	if(ci == NULL)
		return;

	Pet *summon = objmgr.CreatePet();
	summon->SetPosition(p_caster->GetPosition(), true);
	summon->CreateAsSummon(GetSpellProto()->EffectMiscValue[i], ci, TO_CREATURE(summon), p_caster, GetSpellProto(), 1, GetDuration()+2000); //Give it an extra seconds for being unpossesed.
	p_caster->SetSummon(summon);
	summon->DisableAI();
	summon->SetUInt32Value(UNIT_FIELD_LEVEL,m_caster->GetUInt32Value(UNIT_FIELD_LEVEL));
	summon->SetUInt32Value(UNIT_FIELD_DISPLAYID,  ci->Male_DisplayID);
	summon->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, ci->Male_DisplayID);
	summon->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, p_caster->GetGUID());
	summon->SetUInt64Value(UNIT_FIELD_CREATEDBY, p_caster->GetGUID());
	summon->SetUInt32Value(UNIT_FIELD_HEALTH , 100);
	summon->SetUInt32Value(UNIT_FIELD_MAXHEALTH , 100);
	summon->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
	//Dummy aura is used to possess the creature.
}

void Spell::SpellEffectUseGlyph(uint32 i)
{
	if( p_caster == NULL )
		return;
	uint8 result = 0;
	result = p_caster->SetGlyph(m_glyphIndex, GetSpellProto()->EffectMiscValue[i]);
	if(result) // there was an error
		SendCastResult(result);
	else // success, need to update client display
		p_caster->smsg_TalentsInfo(false);
}

void Spell::SpellEffectHealMechanical(uint32 i)
{
	if(unitTarget == NULL || unitTarget->GetCreatureType() != MECHANICAL)
		return;

	Heal((int32)damage);
}

void Spell::SpellEffectSummonObjectWild(uint32 i)
{
	if(u_caster == NULL )
		return;

	// spawn a new one
	GameObject* GoSummon = u_caster->GetMapMgr()->CreateGameObject(GetSpellProto()->EffectMiscValue[i]);
	if( GoSummon == NULL || !GoSummon->CreateFromProto(GetSpellProto()->EffectMiscValue[i],	m_caster->GetMapId(), m_caster->GetPosition()))
		return;

	GoSummon->SetUInt32Value(GAMEOBJECT_LEVEL, u_caster->getLevel());
	GoSummon->SetUInt64Value(OBJECT_FIELD_CREATED_BY, m_caster->GetGUID());
	GoSummon->SetState(0);
	GoSummon->PushToWorld(u_caster->GetMapMgr());
	GoSummon->SetSummoned(u_caster);
	GoSummon->ExpireAndDelete(GetDuration());
}

void Spell::SpellEffectScriptEffect(uint32 i) // Script Effect
{
	uint32 spellId = GetSpellProto()->Id;

	switch(spellId)
	{
	// Demonic Empowerement (seccubus effect)
	case 54436:
		{
			if( unitTarget != NULL)
				for( uint32 x = MAX_POSITIVE_AURAS ; x < MAX_AURAS ; x++ )
				{
					if( unitTarget->m_auras[x] && unitTarget->m_auras[x]->IsPositive() )
					{
						SpellEntry *p = unitTarget->m_auras[x]->GetSpellProto();
						if( Spell::HasMechanic(p, MECHANIC_STUNNED) ||
							Spell::HasMechanic(p, MECHANIC_ENSNARED) ||
							Spell::HasMechanic(p, MECHANIC_ROOTED) )
						{
							unitTarget->m_auras[x]->AttemptDispel( u_caster );
						}
					}
				}
		}break;
	// Everlasting Affliction
	case 47422:
		{
			if( unitTarget != NULL && p_caster != NULL )
			{
				Aura* pAura = NULL;
				for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
				{
					pAura = unitTarget->m_auras[i];
					if( pAura != NULL && pAura->GetUnitCaster() == p_caster )
					{
						if( pAura->GetSpellProto()->NameHash == SPELL_HASH_CORRUPTION )
						{
							pAura->SetDuration(pAura->GetDuration());
							pAura->SetTimeLeft(pAura->GetDuration());
						}
					}
				}
			}
		}break;
	case 30918:
		{
			if( p_caster == NULL )
				return;
			//ugly thingo
			Aura* pAura = NULL;
			for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
			{
				pAura = p_caster->m_auras[i];
				if( pAura != NULL )
					for( int i=0 ; i<3 ; i++ )
						if( pAura->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_DECREASE_SPEED || pAura->GetSpellProto()->EffectApplyAuraName[i] == SPELL_AURA_MOD_ROOT )
						{
							p_caster->RemoveAuraBySlot(i);
							break;
						}
			}
		}break;
	case 53209://Chimera Shot
		{
			if( p_caster  == NULL )
				return;

			Unit * Gettarget = NULL;
			Gettarget = GetUnitTarget();
			if( Gettarget == NULL )
				return;

			SpellEntry* sting_proto = NULL;
			Aura* pAura = NULL;
			uint32 actdur = 0;
			for(uint32 i = MAX_POSITIVE_AURAS; i < MAX_AURAS; i++)
			{
				pAura = Gettarget->m_auras[i];
				if( pAura != NULL )
				{
					if( pAura->m_spellProto->NameHash == SPELL_HASH_SERPENT_STING ||
						pAura->m_spellProto->NameHash == SPELL_HASH_VIPER_STING ||
						pAura->m_spellProto->NameHash == SPELL_HASH_SCORPID_STING )
					{
						actdur = pAura->GetDuration() - pAura->GetTimeLeft();
						sting_proto = pAura->m_spellProto;
						break;
					}
				}
			}
			if( sting_proto != NULL )
			{
				p_caster->CastSpell(GetUnitTarget(), sting_proto->Id, true);
				//trigger effect
				switch( sting_proto->NameHash )
				{
				case SPELL_HASH_SERPENT_STING:
					{
						uint32 ticks = float2int32(actdur * 0.003f);

						SpellEntry* spellInfo = dbcSpell.LookupEntryForced( 53353 );
						Spell* sp=new Spell( p_caster, spellInfo, true, NULLAURA );
						sp->forced_basepoints[0] = float2int32(((sting_proto->EffectBasePoints[0] / 5) * ticks) * 0.4f);
						SpellCastTargets tgt;
						tgt.m_unitTarget = GetUnitTarget()->GetGUID();
						sp->prepare(&tgt);
					}break;
				case SPELL_HASH_VIPER_STING:
					{
						uint32 ticks = float2int32(actdur * 0.002f);

						SpellEntry* spellInfo = dbcSpell.LookupEntryForced( 53358 );
						Spell* sp=new Spell( p_caster, spellInfo, true, NULLAURA );
						sp->forced_basepoints[0] = float2int32(((GetUnitTarget()->GetUInt32Value(UNIT_FIELD_MAXPOWER1) * 0.04f) * ((16 / 4) * ticks)) * 0.6f);
						SpellCastTargets tgt;
						tgt.m_unitTarget = p_caster->GetGUID();
						sp->prepare(&tgt);
					}break;
				case SPELL_HASH_SCORPID_STING:
					{
						if( p_caster->m_CustomTimers[CUSTOM_TIMER_CHIMERA_SCORPID] <= getMSTime() )
						{
							p_caster->m_CustomTimers[CUSTOM_TIMER_CHIMERA_SCORPID] = getMSTime() + MSTIME_MINUTE;
							p_caster->CastSpell( GetUnitTarget(), 53359, true);
						}
					}break;
				}
			}
		}break;
	case 50725://vigilance
		{
			if( p_caster == NULL )
				return;

			Aura* aur = NULL;
			aur = p_caster->FindAura( 50720 );
			if( aur  != NULL )
			{
				Unit* caster = aur->GetUnitCaster();
				if( caster != NULL && caster->IsPlayer() )
					TO_PLAYER(caster)->ClearCooldownForSpell(355);
			}
		}break;
	case 47948://Pain and Suffering
		{
			Unit * Gettarget = NULL;
			Gettarget = GetUnitTarget();
			if( Gettarget == NULL )
				return;

			Aura* aur = NULL;
			Gettarget->FindNegativeAuraByNameHash(SPELL_HASH_SHADOW_WORD__PAIN);
			if( aur != NULL) // Crow: Hmmm....
				aur->SetTimeLeft(aur->GetDuration());
		}break;
	case 47951:
		{
			if( p_caster == NULL)
				return;

			SpellEntry* sp = NULL;
			SpellCastTargets targets;

			sp = dbcSpell.LookupEntryForced( 59600 );
			Spell* pSpell_a = new Spell(p_caster, sp, true, NULLAURA);
			pSpell_a->GenerateTargets( &targets );
			pSpell_a->prepare( &targets );

			sp = dbcSpell.LookupEntryForced( 47953 );
			Spell* pSpell_b =new Spell(p_caster, sp, true, NULLAURA);
			pSpell_b->GenerateTargets( &targets );
			pSpell_b->prepare( &targets );
		}break;
	case 46584:
		{
			if(p_caster == NULL)
				return;

			p_caster->CastSpell(p_caster, 46585, true );
		}break;
	// Warlock Healthstones, just how much health does a lock need?
	case 6201:		// Minor Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18692))
		{
			CreateItem(19004);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18693))
		{
			CreateItem(19005);
			break;
		}
		CreateItem(5512);
		break;
	case 6202:		// Lesser Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19007);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19006);
			break;
		}
		CreateItem(5511);
		break;
	case 5699:		// Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19009);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19008);
			break;
		}
		CreateItem(5509);
		break;
	case 11729:		// Greater Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19011);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19010);
			break;
		}
		CreateItem(5510);
		break;
	case 11730:		// Major Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(19013);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(19012);
			break;
		}
		CreateItem(9421);
		break;
	case 27230:		// Master Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(22105);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(22104);
			break;
		}
		CreateItem(22103);
		break;
	case 47871:		// Demonic Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(36891);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(36890);
			break;
		}
		CreateItem(36889);
		break;
	case 47878:		// Fel Healthstone
		if (p_caster != NULL && p_caster->HasSpell(18693))	// Improved Healthstone (2)
		{
			CreateItem(36894);
			break;
		}
		if (p_caster != NULL && p_caster->HasSpell(18692))	// Improved Healthstone (1)
		{
			CreateItem(36893);
			break;
		}
		CreateItem(36892);
		break;

	// Holy Light
	case 635:
	case 639:
	case 647:
	case 1026:
	case 1042:
	case 3472:
	case 10328:
	case 10329:
	case 10348:
	case 25292:
	case 27135:
	case 27136:
	//Flash of light
	case 19750:
	case 19939:
	case 19940:
	case 19941:
	case 19942:
	case 19943:
	case 27137:
		{
			uint32 NewSpell = 0;
			switch(GetSpellProto()->Id)
			{
				// hehe xD
				case 19750: NewSpell = 19993; break;
				case 19939: NewSpell = 35211; break;
				case 19940: NewSpell = 35212; break;
				case 19941: NewSpell = 35213; break;
				case 19942: NewSpell = 35214; break;
				case 19943: NewSpell = 35215; break;
				case 27137: NewSpell = 35216; break;
				case 635: NewSpell = 19982; break;
				case 639: NewSpell = 19981; break;
				case 647: NewSpell = 19980; break;
				case 1026: NewSpell = 19968; break;
				case 1042: NewSpell = 35217; break;
				case 3472: NewSpell = 35218; break;
				case 10328: NewSpell = 35219; break;
				case 10329: NewSpell = 35220; break;
				case 25292: NewSpell = 35221; break;
				case 27135: NewSpell = 35222; break;
				case 27136: NewSpell = 35223; break;
			}
			if(u_caster && NewSpell)
			{
				Spell* sp = new Spell(u_caster,dbcSpell.LookupEntry(NewSpell),true,NULLAURA);
				if(sp == NULL)
					return;
				SpellCastTargets tgt(unitTarget->GetGUID());
				//sp->damage = damage;
				sp->forced_basepoints[0] = damage;
				sp->prepare(&tgt);
			}
		}
	break;

	// Judgement
	case 20271:
	case 53407:
	case 53408:
		{
			if(unitTarget == NULL || p_caster == NULL)
				return;

			uint32 spellid;
			if( GetSpellProto()->Id == 20271 )
				spellid = 20185;
			else if( GetSpellProto()->Id == 53407 )
				spellid = 20184;
			else
				spellid = 20186;

			if(p_caster->JudgementSpell)
				p_caster->CastSpell(unitTarget, p_caster->JudgementSpell, true);

			SpellEntry *en = dbcSpell.LookupEntry(spellid);
			if(en == NULL)
				return;

			Spell* sp(new Spell(p_caster, en, true, NULLAURA));
			SpellCastTargets tgt;
			tgt.m_unitTarget = unitTarget->GetGUID();
			tgt.m_targetMask = TARGET_FLAG_UNIT;
			sp->prepare(&tgt);

			if( p_caster->HasDummyAura(SPELL_HASH_JUDGEMENTS_OF_THE_WISE) )
			{
				if( Rand( p_caster->GetDummyAura(SPELL_HASH_JUDGEMENTS_OF_THE_WISE)->RankNumber * 33 + 1 ) )
				{
					p_caster->CastSpell( p_caster, 31930, true );
					//cast Replenishment
					if( p_caster->GetGroup() )
					{
						uint32 TargetCount = 0;
						p_caster->GetGroup()->Lock();
						for(uint32 x = 0; x < p_caster->GetGroup()->GetSubGroupCount(); ++x)
						{
							if( TargetCount == 10 )
								break;

							for(GroupMembersSet::iterator itr = p_caster->GetGroup()->GetSubGroup( x )->GetGroupMembersBegin(); itr != p_caster->GetGroup()->GetSubGroup( x )->GetGroupMembersEnd(); itr++)
							{
								if((*itr)->m_loggedInPlayer && TargetCount <= 10)
								{
									Player* p_target = (*itr)->m_loggedInPlayer;
									if( p_target->GetPowerType() != POWER_TYPE_MANA )
										continue;

									SpellEntry* Replinishment = dbcSpell.LookupEntryForced( 57669 );
									Spell* pSpell = new Spell(p_caster, Replinishment, true, NULLAURA);
									SpellCastTargets tgt;
									tgt.m_unitTarget = p_target->GetGUID();
									pSpell->prepare(&tgt);
									TargetCount++;
								}
							}
						}
						p_caster->GetGroup()->Unlock();
					}
					else
					{
						SpellEntry* Replinishment = dbcSpell.LookupEntryForced( 57669 );
						Spell* pSpell = new Spell(p_caster, Replinishment, true, NULLAURA);
						SpellCastTargets tgt;
						tgt.m_unitTarget = p_caster->GetGUID();
						pSpell->prepare(&tgt);
					}
				}
			}
		}break;
	//warlock - Master Demonologist
	case 23784:
		{
			if( p_caster == NULL || unitTarget == NULL)
				return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23759 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23760 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23761 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23762 ;
				inc_resist_by_level = 20 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35702 ;
				inc_resist_by_level_spell = 23762 ;
				inc_resist_by_level = 10 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = p_caster->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
		}break;
	case 23830:
		{
			if( p_caster == NULL || unitTarget == NULL)
				return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23826 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23841 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23833 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 1 ;
				inc_resist_by_level_spell = 23837 ;
				inc_resist_by_level = 40 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35703 ;
				inc_resist_by_level_spell = 23837 ;
				inc_resist_by_level = 20 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = p_caster->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
		}break;
	case 23831:
		{
			if( p_caster == NULL || unitTarget == NULL)
				return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23827 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23842 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23834 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23838 ;
				inc_resist_by_level = 60 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35704 ;
				inc_resist_by_level_spell = 23838 ;
				inc_resist_by_level = 30 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = p_caster->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = NULL;
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
		}break;
	case 23832:
		{
			if( p_caster == NULL || unitTarget == NULL)
				return; //can't imagine how this talent got to anybody else then a player casted on pet
			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23828 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23843 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23835 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23839 ;
				inc_resist_by_level = 80 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35705 ;
				inc_resist_by_level_spell = 23839 ;
				inc_resist_by_level = 40 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell* sp = new Spell( p_caster, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = NULL;
				sp = new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell* sp=new Spell( p_caster, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = p_caster->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				sp = NULL;
				sp = new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				sp->prepare( &tgt1 );
			}
		}break;
	case 35708:
		{
			if( p_caster == NULL || unitTarget == NULL)
				return; //can't imagine how this talent got to anybody else then a player casted on pet

			uint32 casted_spell_id = 0 ;
			uint32 inc_resist_by_level = 0 ;
			uint32 inc_resist_by_level_spell = 0 ;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //in case it is imp
				casted_spell_id = 23829 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				casted_spell_id = 23844 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				casted_spell_id = 23836 ;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
			{
				casted_spell_id = 0 ;
				inc_resist_by_level_spell = 23840 ;
				inc_resist_by_level = 100 ;
			}
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
			{
				casted_spell_id = 35706 ;
			}
			if( casted_spell_id )
			{
				//for self
				Spell* sp=new Spell( p_caster, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				Spell* Petsp=new Spell(  unitTarget, dbcSpell.LookupEntry( casted_spell_id ), true, NULLAURA );
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				Petsp->prepare( &tgt1 );
			}
			if( inc_resist_by_level_spell )
			{
				//for self
				Spell* sp=new Spell( p_caster, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				sp->forced_basepoints[0] = p_caster->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt( p_caster->GetGUID() );
				sp->prepare( &tgt );
				//for pet
				Spell* Petsp=new Spell( unitTarget, dbcSpell.LookupEntry( inc_resist_by_level_spell ), true, NULLAURA );
				Petsp->forced_basepoints[0] = unitTarget->GetUInt32Value( UNIT_FIELD_LEVEL ) * inc_resist_by_level / 100;
				SpellCastTargets tgt1( unitTarget->GetGUID() );
				Petsp->prepare( &tgt1 );
			}
		}break;
	case 44876: //Force Cast - Portal Effect: Sunwell Isle
		{
			if( u_caster == NULL || unitTarget == NULL )
				return;

			if(p_caster != NULL && p_caster->getLevel() < 70)
			{
				sChatHandler.RedSystemMessage(p_caster->GetSession(),"You must be level 70 to use this!");
				return;
			}

			u_caster->CastSpell(unitTarget,damage,true);
		}break;
	case 47193: // [Warlock] Demonic Empowerment
		{
			if(unitTarget== NULL)
				return;

			uint32 nspellid = 0;
			if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 416 ) //Imp
				nspellid = 54444;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1860 ) //VoidWalker
				nspellid = 54443;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 1863 ) //Succubus
				nspellid = 54435;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 417 ) //Felhunter
				nspellid = 54509;
			else if ( unitTarget->GetUInt32Value( OBJECT_FIELD_ENTRY ) == 17252 ) //Felguard
				nspellid = 54508;

			unitTarget->CastSpell(unitTarget, nspellid, true);
		}break;
	case 50842: // Pestilence.
		{
			if(p_caster != NULL)
			{
				if(p_caster->HasAura(63334)) // Glyph of Disease
				{
					uint num = 2;
					uint32 diseases[2] = { 55078, 55095 };
					for(uint i = 0; i < num; i++)
						if(unitTarget->HasAura(diseases[i]))
							p_caster->CastSpell(unitTarget, diseases[i], true);
				}
			}
		}break;
	case 60123: //Priest Lightwell spellclick script effect
		{
			switch(u_caster->GetEntry())
			{
			case 31883:
				{
					u_caster->CastSpell(unitTarget,48085,true);
				}break;
			case 31893:
				{
					u_caster->CastSpell(unitTarget,48084,true);
				}break;
			case 31894:
				{
					u_caster->CastSpell(unitTarget,28276,true);
				}break;
			case 31895:
				{
					u_caster->CastSpell(unitTarget,27874,true);
				}break;
			case 31896:
				{
					u_caster->CastSpell(unitTarget,27873,true);
				}break;
			}
		}break;
	default:
		{
			if(sLog.IsOutDevelopement())
				printf("Unhandled Scripted Effect In Spell %u\n", spellId);
			else
				sLog.outSpellDebug("Unhandled Scripted Effect In Spell %u", spellId);
		}break;
	}
}

void Spell::SpellEffectSanctuary(uint32 i) // Stop all attacks made to you
{
	if( unitTarget == NULL )
		return;

	unordered_set< Object* >::iterator itr, it2;
	Unit* pUnit;

	for( itr = unitTarget->GetInRangeOppFactsSetBegin(); itr != unitTarget->GetInRangeOppFactsSetEnd(); )
	{
		it2 = itr++;
		pUnit = TO_UNIT(*it2);
		if( pUnit->GetTypeId() == TYPEID_UNIT )
			pUnit->GetAIInterface()->RemoveThreatByPtr( unitTarget );
	}
	

	// also cancel any spells we are casting
	if( unitTarget->GetCurrentSpell() != NULL && unitTarget->GetCurrentSpell() != this && unitTarget->GetCurrentSpell()->getState() == SPELL_STATE_PREPARING )
		unitTarget->InterruptCurrentSpell();
	unitTarget->smsg_AttackStop( unitTarget );

	if( playerTarget != NULL )
		playerTarget->EventAttackStop();
}

void Spell::SpellEffectAddComboPoints(uint32 i) // Add Combo Points
{
	if(p_caster == NULL)
		return;

	//if this is a procspell Ruthlessness (maybe others later)
	if(pSpellId && GetSpellProto()->Id==14157)
	{
		//it seems this combo adding procspell is going to change combopoint count before they will get reseted. We add it after the reset
		/* burlex - this is wrong, and exploitable.. :/ if someone uses this they will have unlimited combo points */
		//re-enabled this by Zack. Explained why it is used + recheked to make sure initialization is good ...
		// while casting a spell talent will trigger uppon the spell prepare faze
		// the effect of the talent is to add 1 combo point but when triggering spell finishes it will clear the extra combo point
		p_caster->m_spellcomboPoints += damage;
		return;
	}
	p_caster->AddComboPoints(p_caster->GetSelection(), damage);
}

void Spell::SpellEffectCreateHouse(uint32 i) // Create House
{


}

void Spell::SpellEffectDuel(uint32 i) // Duel
{
	if( p_caster == NULL  || !p_caster->isAlive() || playerTarget == p_caster )
		return;

	uint32 areaId = p_caster->GetAreaID();
	AreaTable * at = dbcArea.LookupEntryForced(areaId);
	if( sWorld.FunServerMall != -1 && areaId == (uint32)sWorld.FunServerMall )
	{
		if(at != NULL)
			p_caster->SendAreaTriggerMessage("Sandshroud System: Dueling is not allowed in %s.", at->name);
		else
			p_caster->SendAreaTriggerMessage("Sandshroud System: Dueling is not allowed in the mall.");
		return;
	}

	if(p_caster->GetAreaID() && sWorld.IsSanctuaryArea(p_caster->GetAreaID()))
	{
		SendCastResult(SPELL_FAILED_NO_DUELING);
		return;
	}

	if(p_caster->m_bg)
	{
		SendCastResult(SPELL_FAILED_NOT_IN_BATTLEGROUND);
		return;
	}

	if (p_caster->InStealth())
	{
		SendCastResult(SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED);
		return; // Player is stealth // Crow: No shit?
	}

	if(playerTarget == NULL)
	{
		SendCastResult(SPELL_FAILED_BAD_TARGETS);
		return; // invalid Target
	}
	if (!playerTarget->isAlive())
	{
		SendCastResult(SPELL_FAILED_TARGETS_DEAD);
		return; // Target not alive
	}
	if (playerTarget->hasStateFlag(UF_ATTACKING))
	{
		SendCastResult(SPELL_FAILED_TARGET_IN_COMBAT);
		return; // Target in combat with another unit
	}
	if (playerTarget->DuelingWith != NULL)
	{
		SendCastResult(SPELL_FAILED_TARGET_DUELING);
		return; // Already Dueling
	}

	if(playerTarget->bGMTagOn && !p_caster->GetSession()->HasPermissions())
	{
		SendCastResult(SPELL_FAILED_BAD_TARGETS);
		return;
	}

	p_caster->RequestDuel(playerTarget);
}

void Spell::SpellEffectStuck(uint32 i)
{
	if( playerTarget == NULL || playerTarget != p_caster)
		return;

	uint32 mapid = playerTarget->GetBindMapId();
	float x = playerTarget->GetBindPositionX();
	float y = playerTarget->GetBindPositionY();
	float z = playerTarget->GetBindPositionZ();
	float orientation = 0;

	sEventMgr.AddEvent(playerTarget,&Player::EventTeleport,mapid,x,y,z,orientation,1,
		EVENT_PLAYER_TELEPORT,50,1,EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Spell::SpellEffectSummonPlayer(uint32 i)
{
	if( playerTarget == NULL)
		return;

	if(m_caster->GetMapMgr()->GetMapInfo() && m_caster->GetMapMgr()->GetMapInfo()->type != INSTANCE_NULL)
		return;

	playerTarget->SummonRequest(m_caster, m_caster->GetZoneId(), m_caster->GetMapId(),
		m_caster->GetInstanceID(), m_caster->GetPosition());
}

void Spell::SpellEffectActivateObject(uint32 i) // Activate Object
{
/*	if( p_caster == NULL)
		return;

	if( gameObjTarget == NULL)
		return;

	gameObjTarget->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);

	sEventMgr.AddEvent(gameObjTarget, &GameObject::Deactivate, EVENT_GAMEOBJECT_DEACTIVATE, GetDuration(), 1);*/
}

void Spell::SpellEffectWMODamage(uint32 i)
{
	if(p_caster == NULL && v_caster && v_caster->GetControllingPlayer() )
		p_caster = TO_PLAYER(v_caster->GetControllingPlayer());
	if(gameObjTarget == NULL)
	{
		DamageGosAround(i);
		return;
	}

	gameObjTarget->TakeDamage(damage,m_caster,p_caster, GetSpellProto()->Id);
}

void Spell::SpellEffectWMORepair(uint32 i)
{
	if(gameObjTarget == NULL)
		return;

	gameObjTarget->Rebuild();
}

void Spell::SpellEffectChangeWMOState(uint32 i)
{
	if(gameObjTarget == NULL)
		return;

	switch(GetSpellProto()->EffectMiscValue[i])
	{
	case 1:
		{
			gameObjTarget->Damage();
		}break;
	case 2:
		{
			gameObjTarget->Destroy();
		}break;
	default:
		{
			gameObjTarget->Rebuild();
		}break;
	}
}

void Spell::SummonTotem(uint32 i) // Summon Totem
{
	if(p_caster== NULL)
		return;

	uint32 entry = GetSpellProto()->EffectMiscValue[i];

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
	CreatureProto* cp = CreatureProtoStorage.LookupEntry(entry);
	if(!ci || !cp)
	{
		if(sLog.IsOutDevelopement())
			printf("Totem entry %u doesn't exist, exiting\n",entry);
		else
			sLog.outSpellDebug("Totem entry %u doesn't exist, exiting",entry);
		return;
	}

	// Obtain the spell we will be casting.
	bool hastotemspell = true;
	SpellEntry * TotemSpell = objmgr.GetTotemSpell(GetSpellProto()->Id);
	if(TotemSpell == NULL)
		hastotemspell = false;

	Creature* pTotem = NULLCREATURE;
	pTotem = p_caster->GetMapMgr()->CreateCreature(entry);
	if(pTotem == NULL)
		return;

	float x = p_caster->GetPositionX();
	float y = p_caster->GetPositionY();
	uint32 slot = m_summonProperties->slot;

	switch(GetSpellProto()->EffectMiscValueB[i])
	{
		case SUMMON_TYPE_TOTEM_1:
		{
			x -= 1.5f;
			y -= 1.5f;
		}break;
		case SUMMON_TYPE_TOTEM_2:
		{
			x -= 1.5f;
			y += 1.5f;
		}break;
		case SUMMON_TYPE_TOTEM_3:
		{
			x += 1.5f;
			y -= 1.5f;
		}break;
		case SUMMON_TYPE_TOTEM_4:
		{
			x += 1.5f;
			y += 1.5f;
		}break;
	}
	//1.5 distance from caster only, don't bother getting height (it's unreliable since 3xx anyway).
	float landh = p_caster->GetPositionZ() + 1.5f;

	//We already emptied this slot in SpellEffectSummon
	if( slot < 7 )
		p_caster->m_SummonSlots[slot] = pTotem;

	//record our owner guid and slotid
	pTotem->SetSummonOwnerSlot(u_caster->GetGUID(),int8(slot));

	//Make it a Totem
	pTotem->SetTotem(true);

	pTotem->Load(cp, 0, x, y, landh);
	pTotem->Create(ci->Name, p_caster->GetMapId(), x, y, landh, p_caster->GetOrientation());

	uint32 displayID = 0;

	if( p_caster->GetTeam() == 0 )
	{
		if ( ci->Female_DisplayID != 0 )
		{
			displayID = ci->Female_DisplayID; //this is the nice solution provided by emsy
		}
		else //this is the case when you are using a blizzlike db
		{
			if( ci->Male_DisplayID == 4587 )
				displayID = 19075;
			else if( ci->Male_DisplayID == 4588 )
				displayID = 19073;
			else if( ci->Male_DisplayID == 4589 )
				displayID = 19074;
			else if( ci->Male_DisplayID == 4590 )
				displayID = 19071;
			else if( ci->Male_DisplayID == 4683 )
				displayID = 19074;
			else
				displayID = ci->Male_DisplayID;
		}
	}
	else
	{
		displayID = ci->Male_DisplayID;
	}

	// Set up the creature.
	pTotem->SetUInt32Value(OBJECT_FIELD_ENTRY, entry);
	pTotem->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	pTotem->SetUInt64Value(UNIT_FIELD_CREATEDBY, p_caster->GetGUID());
	pTotem->SetUInt32Value(UNIT_FIELD_HEALTH, damage > 1 ? damage: 5 );
	pTotem->SetUInt32Value(UNIT_FIELD_MAXHEALTH, damage > 1 ? damage: 5);
	pTotem->SetUInt32Value(UNIT_FIELD_POWER3, p_caster->getLevel() * 30);
	pTotem->SetUInt32Value(UNIT_FIELD_MAXPOWER3, p_caster->getLevel() * 30);
	pTotem->SetUInt32Value(UNIT_FIELD_LEVEL, p_caster->getLevel());
	pTotem->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, p_caster->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTotem->SetUInt32Value(UNIT_FIELD_BYTES_0, (1 << 8) | (2 << 16) | (1 << 24));
	pTotem->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_SELF_RES);
	pTotem->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
	pTotem->SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000);
	pTotem->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 1.0f);
	pTotem->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.0f);
	pTotem->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayID);
	pTotem->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, displayID); //blizzlike :P
	pTotem->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	pTotem->SetUInt32Value(UNIT_CREATED_BY_SPELL, GetSpellProto()->Id);
	pTotem->SetPhaseMask(p_caster->GetPhaseMask());
	if( p_caster->IsPvPFlagged() )
		pTotem->SetPvPFlag();

	if( p_caster->IsFFAPvPFlagged() )
		pTotem->SetFFAPvPFlag();

	// Initialize faction stuff.
	pTotem->m_faction = p_caster->m_faction;
	pTotem->m_factionDBC = p_caster->m_factionDBC;

	//added by Zack : Some shaman talents are casted on player but it should be inherited or something by totems
	pTotem->InheritSMMods(p_caster);
	// Totems get spell damage and healing bonus from the Shaman
	for(int school=0;school<7;school++)
	{
		pTotem->DamageDonePosMod[school] = p_caster->DamageDonePosMod[school];
		pTotem->DamageDoneNegMod[school] = p_caster->DamageDoneNegMod[school];
		pTotem->DamageDonePctMod[school] = p_caster->DamageDonePctMod[school];
		pTotem->HealDoneBase = p_caster->HealDoneBase;
	}

	uint32 j = 0;
	// Set up AI, depending on our spells.
	if(hastotemspell)
	{
		for(; j < 3; ++j )
		{
			if( TotemSpell->Effect[j] == SPELL_EFFECT_APPLY_AREA_AURA || TotemSpell->Effect[j] == SPELL_EFFECT_PERSISTENT_AREA_AURA || TotemSpell->EffectApplyAuraName[j] == SPELL_AURA_PERIODIC_TRIGGER_SPELL )
			{
				break;
			}
		}
	}

	// Setup complete. Add us to the world.
	pTotem->PushToWorld(m_caster->GetMapMgr());
	sEventMgr.AddEvent( pTotem, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );

	if(hastotemspell)
	{
		if(j != 3)
		{
			// We're an area aura. Simple. Disable AI and cast the spell.
			pTotem->DisableAI();
			pTotem->GetAIInterface()->totemspell = GetSpellProto();

			Spell* pSpell(new Spell(pTotem, TotemSpell, true, NULLAURA));

			SpellCastTargets targets;
			targets.m_destX = pTotem->GetPositionX();
			targets.m_destY = pTotem->GetPositionY();
			targets.m_destZ = pTotem->GetPositionZ();
			targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;

			pSpell->prepare(&targets);

			if( TotemSpell->NameHash == SPELL_HASH_TOTEM_OF_WRATH )
				pTotem->CastSpell(pTotem, dbcSpell.LookupEntry(30708), true);
		}
		else
		{
			// We're a casting totem. Switch AI on, and tell it to cast this spell.
			pTotem->EnableAI();
			pTotem->GetAIInterface()->Init(pTotem, AITYPE_TOTEM, MOVEMENTTYPE_NONE, p_caster);
			pTotem->GetAIInterface()->totemspell = TotemSpell;
			int32 totemspelltimer = 3000, totemspelltime = 3000;	// need a proper resource for this.

			switch(TotemSpell->Id)
			{
			case 51975: //Poison Cleansing Totem
			case 52025: //Disease Cleansing Totem
				{
					totemspelltime =  5000;
					totemspelltimer = 0; //First tick done immediately
					break;
				}
			case 8146: //Tremor Totem
				{
					totemspelltime = 1000;	// not sure about correctness, but spell amplitude = 1000
					totemspelltimer = 0; //First tick done immediately
					break;
				}
			default:break;
			}
			switch(GetSpellProto()->NameHash)
			{
			case SPELL_HASH_STONECLAW_TOTEM:
				{
					totemspelltime = 30000; // Should be casted only once
					totemspelltimer = 0; // Casted immediately
					break;
				}
			case SPELL_HASH_MAGMA_TOTEM:
			case SPELL_HASH_MANA_SPRING_TOTEM:
			case SPELL_HASH_HEALING_STREAM_TOTEM:
				{
					totemspelltimer = totemspelltime = 2000;
					break;
				}
			case SPELL_HASH_SEARING_TOTEM:
				{
					totemspelltimer = totemspelltime = 2200;
					break;
				}
			default:
				break;
			}

			pTotem->GetAIInterface()->m_totemspelltimer = totemspelltimer;
			pTotem->GetAIInterface()->m_totemspelltime = totemspelltime;
		}
	}

	//in case these are our elemental totems then we should set them up
	switch (GetSpellProto()->Id)
	{
		case 2062: //Earth Elemental Totem
		{
			pTotem->GetAIInterface()->Event_Summon_Elemental(GetDuration(), 15352, NATURE_DAMAGE, 0);
			pTotem->DisableAI();
		}break;
		case 2894://Fire Elemental Totem
		{
			pTotem->GetAIInterface()->Event_Summon_Elemental(GetDuration(), 15438, FIRE_DAMAGE, 0);
			pTotem->DisableAI();
		}break;
	}

	if( p_caster != NULL)
	{
		WorldPacket data(SMSG_TOTEM_CREATED, 20);
		data << uint8(m_summonProperties->slot - 1);
		data << pTotem->GetGUID();
		data << GetDuration();
		data << GetSpellProto()->Id;
		p_caster->CopyAndSendDelayedPacket(&data);
	}

}

void Spell::SpellEffectSelfResurrect(uint32 i)
{
	if( p_caster == NULL || playerTarget->isAlive() || playerTarget->PreventRes)
		return;
	uint32 mana;
	uint32 health;
	uint32 class_=unitTarget->getClass();

	switch(GetSpellProto()->Id)
	{
	case 3026:
	case 20758:
	case 20759:
	case 20760:
	case 20761:
	case 27240:
		{
			health = GetSpellProto()->EffectMiscValue[i];
			mana = -damage;
		}break;
	case 21169: //Reincarnation. Ressurect with 20% health and mana
		{
			health = uint32((unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*damage)/100);
			mana = uint32((unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*damage)/100);
		}
		break;
	default:
		{
			if(damage < 0) return;
			health = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)*damage/100);
			mana = uint32(unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)*damage/100);
		}break;
		}

	if(class_==WARRIOR||class_==ROGUE)
		mana=0;

	playerTarget->m_resurrectHealth = health;
	playerTarget->m_resurrectMana = mana;

	playerTarget->ResurrectPlayer();
	playerTarget->m_resurrectHealth = 0;
	playerTarget->m_resurrectMana = 0;
	playerTarget->SetMovement(MOVE_UNROOT, 1);

	playerTarget->SetUInt32Value(PLAYER_SELF_RES_SPELL, 0);

	if(GetSpellProto()->Id==21169)
		AddCooldown();
}

void Spell::SpellEffectSkinning(uint32 i)
{
	if( unitTarget == NULL || !unitTarget->IsCreature() || p_caster == NULL )
		return;

	Creature* cr = TO_CREATURE( unitTarget );
	uint32 skill = cr->GetRequiredLootSkill();

	uint32 sk = TO_PLAYER( m_caster )->_GetSkillLineCurrent( skill );
	uint32 lvl = cr->getLevel();

	if( ( sk >= lvl * 5 ) || ( ( sk + 100 ) >= lvl * 10 ) )
	{
		//Fill loot for Skinning
		lootmgr.FillGatheringLoot(&cr->m_loot, cr->GetEntry());
		TO_PLAYER( m_caster )->SendLoot( cr->GetGUID(), cr->GetMapId(), 2 );

		//Not skinable again
		cr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
		cr->Skinned = true;
		//double chance from elite
		if(cr->GetCreatureInfo()->Rank > 0)
			DetermineSkillUp(skill ,sk < lvl * 5 ? sk/5 : lvl, 2);
		else
			DetermineSkillUp(skill ,sk < lvl * 5 ? sk/5 : lvl, 1);
	}
	else
		SendCastResult(SPELL_FAILED_TARGET_UNSKINNABLE);
}

void Spell::SpellEffectCharge(uint32 i)
{
	if( unitTarget == NULL || !u_caster || !unitTarget->isAlive())
		return;

	if (u_caster->IsStunned() || u_caster->m_rooted || u_caster->IsPacified() || u_caster->IsFeared())
		return;

	float dx,dy;

	float x, y, z;
	if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
	{
		if(m_targets.m_destX == 0.0f || m_targets.m_destY == 0.0f)
			return;
		x = m_targets.m_destX;
		y = m_targets.m_destY;
		z = m_targets.m_destZ;
	}
	else
	{
		if(!unitTarget)
			return;
		x = unitTarget->GetPositionX();
		y = unitTarget->GetPositionY();
		z = unitTarget->GetPositionZ();
	}

	dx=x-m_caster->GetPositionX();
	dy=y-m_caster->GetPositionY();
	if(dx == 0.0f || dy == 0.0f)
		return;

	uint32 time = uint32( (m_caster->CalcDistance(x,y,z) / ((u_caster->m_runSpeed * 3.5) * 0.001f)) + 0.5);

	u_caster->GetAIInterface()->SendMoveToPacket(x, y, z, 0.0f, time, MONSTER_MOVE_FLAG_WALK);
	u_caster->SetPosition(x,y,z,0.0f,true);

	if(unitTarget && unitTarget->GetTypeId() == TYPEID_UNIT)
		unitTarget->GetAIInterface()->StopMovement(time);

	u_caster->addStateFlag(UF_ATTACKING);
	if(unitTarget)
		u_caster->smsg_AttackStart( unitTarget );
	u_caster->setAttackTimer(time, false);
	u_caster->setAttackTimer(time, true);
	if(p_caster)
	{
		p_caster->EventAttackStart();
		p_caster->ResetHeartbeatCoords();
		p_caster->DelaySpeedHack( time + 1000 );
		p_caster->z_axisposition = 0.0f;
	}
}

void Spell::SpellEffectPlaceTotemsOnBar(uint32 i)
{
	if(!p_caster)
		return;
	uint32 button = m_spellInfo->EffectMiscValue[i] + 132;
	uint32 button_count = m_spellInfo->EffectMiscValueB[i];
	for (; button_count;button_count--, button++)
	{
		if(p_caster->mActions[button].Type != 0)
			 continue;

		uint32 spell = p_caster->mActions[button].Action;
		if(!spell)
			continue;
		p_caster->CastSpell(p_caster,spell,true);
	}
}

void Spell::SpellEffectSendTaxi( uint32 i )
{
	if ( playerTarget == NULL || !playerTarget->isAlive() )
		return;

	if( playerTarget->HasFlag( UNIT_FIELD_FLAGS, UNIT_FLAG_LOCK_PLAYER ) )
		return;

	TaxiPath* taxipath = sTaxiMgr.GetTaxiPath( GetSpellProto()->EffectMiscValue[i] );

	if( !taxipath )
		return;

	TaxiNode* taxinode = sTaxiMgr.GetTaxiNode( taxipath->GetSourceNode() );

	if( !taxinode )
		return;

	uint32 mount_entry = 0;
	uint32 modelid = 0;

	if( playerTarget->GetTeam() )		// HORDE
	{
		mount_entry = taxinode->horde_mount;
		if( !mount_entry )
			mount_entry = taxinode->alliance_mount;

		CreatureInfo* ci = CreatureNameStorage.LookupEntry( mount_entry );
		if( !ci )
			return;

		modelid = ci->Female_DisplayID;
		if( !modelid )
			return;
	}
	else								// ALLIANCE
	{
		mount_entry = taxinode->alliance_mount;
		if( !mount_entry )
			mount_entry = taxinode->horde_mount;

		CreatureInfo* ci = CreatureNameStorage.LookupEntry( mount_entry );
		if( !ci )
			return;

		modelid = ci->Male_DisplayID;
		if( !modelid )
			return;
	}

	playerTarget->TaxiStart( taxipath, modelid, 0 );
}

void Spell::SpellEffectPull( uint32 i )
{
	if( unitTarget == NULL && u_caster != NULL)
	{
		unitTarget = u_caster;
	}

	if(unitTarget == NULL)
		return;


	if(unitTarget->IsCreature() && isTargetDummy(unitTarget->GetEntry()))
		return;

	float pullX = 0.0f;
	float pullY = 0.0f;
	float pullZ = 0.0f;
	if(m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
	{
		pullX = m_targets.m_destX;
		pullY = m_targets.m_destY;
		pullZ = m_targets.m_destZ;
	}
	else
	{
		pullX = m_caster->GetPositionX();
		pullY = m_caster->GetPositionY();
		pullZ = m_caster->GetPositionZ();
	}
	uint32 time = uint32((CalculateEffect(i, unitTarget) / 10.0f) * 100);
	unitTarget->GetAIInterface()->StopMovement(time);
	unitTarget->GetAIInterface()->JumpTo(pullX, pullY,pullZ,time, GetSpellProto()->EffectMiscValue[i]/10);
	unitTarget->SetPosition(pullX,pullY,pullZ, 0.0f);
	if(playerTarget)
	{
		if( playerTarget->IsPvPFlagged() )
			u_caster->SetPvPFlag();
		if( playerTarget->IsFFAPvPFlagged() )
			u_caster->SetFFAPvPFlag();
	}
}

void Spell::SummonNonCombatPet(uint32 i)
{
	if( u_caster == NULL|| !u_caster->IsInWorld())
		return;

	uint32 SummonCritterID = GetSpellProto()->EffectMiscValue[i];

	if(!SummonCritterID)
		return;

	CreatureInfo * ci = CreatureNameStorage.LookupEntry(SummonCritterID);
	CreatureProto * cp = CreatureProtoStorage.LookupEntry(SummonCritterID);

	if(!ci || !cp)
		return;

	Creature* pCreature = NULLCREATURE;
	pCreature = u_caster->GetMapMgr()->CreateCreature(SummonCritterID);
	if(pCreature == NULL)
		return;

	pCreature->SetInstanceID(u_caster->GetMapMgr()->GetInstanceID());
	pCreature->Load(cp, m_caster->GetMapMgr()->iInstanceMode, m_caster->GetPositionX(), m_caster->GetPositionY(), m_caster->GetPositionZ(), m_caster->GetOrientation());
	pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 35);
	pCreature->_setFaction();
	pCreature->SetUInt32Value(UNIT_FIELD_LEVEL, 1);
	pCreature->GetAIInterface()->Init(pCreature,AITYPE_PET,MOVEMENTTYPE_NONE,u_caster);
	pCreature->GetAIInterface()->SetUnitToFollow(u_caster);
	pCreature->GetAIInterface()->SetUnitToFollowAngle(float(-(M_PI/2)));
	pCreature->GetAIInterface()->SetFollowDistance(3.0f);
	pCreature->GetAIInterface()->disable_melee = true;
	pCreature->bInvincible = true;
	pCreature->PushToWorld(u_caster->GetMapMgr());
	pCreature->SetPhaseMask(u_caster->GetPhaseMask());
	if( m_summonProperties->slot < 7 )
	{
		pCreature->SetSummonOwnerSlot(u_caster->GetGUID(), m_summonProperties->slot);
		u_caster->m_SummonSlots[ m_summonProperties->slot ] = pCreature;
	}
}

void Spell::SpellEffectKnockBack(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	if(unitTarget->IsCreature() && isTargetDummy(unitTarget->GetEntry()))
		return;

	unitTarget->knockback(GetSpellProto()->EffectBasePoints[i]+1, GetSpellProto()->EffectMiscValue[i]);
}

void Spell::SpellEffectDisenchant(uint32 i)
{
	if(p_caster == NULL)
		return;

	Item* it = p_caster->GetItemInterface()->GetItemByGUID(m_targets.m_itemTarget);
	if( it == NULL )
	{
		SendCastResult(SPELL_FAILED_CANT_BE_DISENCHANTED);
		return;
	}

	if( !it->m_looted )
	{
		lootmgr.FillItemLoot(&it->m_loot, it->GetEntry(), p_caster->GetTeam());

		if( it->m_loot.items.size() > 0 )
		{
			//Check for skill, we can increase it upto 75
			uint32 skill=p_caster->_GetSkillLineCurrent( SKILL_ENCHANTING );
			if(skill < 75)//can up skill
			{
				if(Rand(float(100-skill*100.0/75.0)))
					p_caster->_AdvanceSkillLine(SKILL_ENCHANTING, float2int32( 1.0f * sWorld.getRate(RATE_SKILLRATE)));
			}
			sLog.outSpellDebug("SpellEffect","Succesfully disenchanted item %d", uint32(itemTarget->GetEntry()));
			p_caster->SendLoot( itemTarget->GetGUID(), itemTarget->GetMapId(), LOOT_GATHERING );
		}
		else
		{
			sLog.outSpellDebug("SpellEffect","Disenchanting failed, item %d has no loot", uint32(itemTarget->GetEntry()));
			SendCastResult(SPELL_FAILED_CANT_BE_DISENCHANTED);
			return;
		}
		// delete from db so it won't be re-saved
		it->DeleteFromDB();
		it->m_looted = true;
	}
	if(it == i_caster)
		i_caster = NULLITEM;
}
void Spell::SpellEffectInebriate(uint32 i) // lets get drunk!
{
	if( playerTarget == NULL )
		return;

	uint16 currentDrunk = playerTarget->m_drunk;
	uint16 drunkMod = damage * 256;
	if (currentDrunk + drunkMod > 0xFFFF)
		currentDrunk = 0xFFFF;
	else
		currentDrunk += drunkMod;
	playerTarget->SetDrunk(currentDrunk, i_caster ? i_caster->GetProto()->ItemId : 0);
}

void Spell::SpellEffectFeedPet(uint32 i)  // Feed Pet
{
	// food flags and food level are checked in Spell::CanCast()
	if( itemTarget == NULL || p_caster == NULL )
		return;

	Pet* pPet = NULL;
	pPet = p_caster->GetSummon();
	if(pPet== NULL)
		return;

	/**	Cast feed pet effect
	- effect is item level and pet level dependent, aura ticks are 35, 17, 8 (*1000) happiness
	- http://petopia.brashendeavors.net/html/articles/basics_feeding.shtml */
	int8 deltaLvl = pPet->getLevel() - itemTarget->GetProto()->ItemLevel;
	damage /= 1000; //damage of Feed pet spell is 35000
	if(deltaLvl > 10)
		damage = damage >> 1;//divide by 2
	if(deltaLvl > 20)
		damage = damage >> 1;
	damage *= 1000;

	SpellEntry *spellInfo = dbcSpell.LookupEntry(GetSpellProto()->EffectTriggerSpell[i]);
	Spell* sp = new Spell(p_caster, spellInfo, true, NULLAURA);
	sp->forced_basepoints[0] = damage - 1;
	SpellCastTargets tgt;
	tgt.m_unitTarget = pPet->GetGUID();
	sp->prepare(&tgt);

	if(itemTarget->GetUInt32Value(ITEM_FIELD_STACK_COUNT) > 1)
	{
		itemTarget->ModUnsigned32Value(ITEM_FIELD_STACK_COUNT, -1);
		itemTarget->m_isDirty = true;
	}
	else
	{
		p_caster->GetItemInterface()->SafeFullRemoveItemByGuid(itemTarget->GetGUID());
		itemTarget = NULLITEM;
	}
}

void Spell::SpellEffectReputation(uint32 i)
{
	if( playerTarget == NULL)
		return;

	playerTarget->ModStanding(GetSpellProto()->EffectMiscValue[i], damage);
}

void Spell::SpellEffectSummonObjectSlot(uint32 i)
{
	if( u_caster  == NULL || !u_caster->IsInWorld())
		return;

	GameObjectInfo * goi = NULL;
	goi = GameObjectNameStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if( goi == NULL )
	{
		if( p_caster != NULL )
		{
			sChatHandler.BlueSystemMessage(p_caster->GetSession(),
			"non-existant gameobject %u tried to be created by SpellEffectSummonObjectSlot. Report to devs!", GetSpellProto()->EffectMiscValue[i]);
		}
		return;
	}

	GameObject* GoSummon = NULLGOB;
	uint32 slot = GetSpellProto()->Effect[i] - SPELL_EFFECT_SUMMON_OBJECT_SLOT1;
	GoSummon = u_caster->m_ObjectSlots[slot] ? u_caster->GetMapMgr()->GetGameObject(u_caster->m_ObjectSlots[slot]) : NULLGOB;
	u_caster->m_ObjectSlots[slot] = 0;

	//slot allready used, empty it now.
	if( GoSummon != NULLGOB)
	{
		if(GoSummon->GetInstanceID() != u_caster->GetInstanceID())
			GoSummon->ExpireAndDelete();
		else
		{
			if( GoSummon->IsInWorld() )
				GoSummon->RemoveFromWorld(true);

			GoSummon->Destruct();
			GoSummon = NULLGOB;
		}
	}

	//create a new GoSummon
	GoSummon = u_caster->GetMapMgr()->CreateGameObject( goi->ID );
	if(GoSummon == NULL)
		return;
	GoSummon->SetInstanceID( u_caster->GetInstanceID() );
	if( GetSpellProto()->EffectImplicitTargetA[i] == EFF_TARGET_SIMPLE_AOE )
		GoSummon->CreateFromProto( goi->ID, m_caster->GetMapId(), m_targets.m_destX, m_targets.m_destY, m_targets.m_destZ, m_caster->GetOrientation(), 0, 0, 0, 0 );
	else
		GoSummon->CreateFromProto( goi->ID, m_caster->GetMapId(), m_caster->GetPosition());

	GoSummon->SetRotation( m_caster->GetOrientation() );
	GoSummon->SetUInt32Value(GAMEOBJECT_LEVEL, u_caster->getLevel());

	if(GoSummon->GetType() == GAMEOBJECT_TYPE_TRAP)
	{
		GoSummon->SetUInt64Value(OBJECT_FIELD_CREATED_BY, u_caster->GetGUID());
		GoSummon->invisible = true;
		GoSummon->invisibilityFlag = INVIS_FLAG_TRAP;
		GoSummon->charges = 1;
		GoSummon->checkrate = 1;
		sEventMgr.AddEvent(GoSummon, &GameObject::TrapSearchTarget, EVENT_GAMEOBJECT_TRAP_SEARCH_TARGET, 100, 0,0);
	}
	else
		GoSummon->ExpireAndDelete(GetDuration());

	GoSummon->PushToWorld(m_caster->GetMapMgr());
	GoSummon->SetSummoned(u_caster);
	GoSummon->SetPhaseMask(u_caster->GetPhaseMask());
	u_caster->m_ObjectSlots[slot] = GoSummon->GetUIdFromGUID();
}

void Spell::SpellEffectDispelMechanic(uint32 i)
{
	if( unitTarget == NULL || !unitTarget->isAlive() )
		return;

	int32 sMisc = (int32)GetSpellProto()->EffectMiscValue[i];
	uint32 x;
	SpellEntry *p;

	if( unitTarget == u_caster || !isAttackable( u_caster, unitTarget ) )
	{
		for( x = MAX_POSITIVE_AURAS ; x<MAX_AURAS ; x++ )
		{
			if( unitTarget->m_auras[x] != NULL && !unitTarget->m_auras[x]->IsPositive())
			{
				p = unitTarget->m_auras[x]->GetSpellProto();
				if( Spell::HasMechanic(p, sMisc) )
				{
					unitTarget->m_auras[x]->AttemptDispel( u_caster );
				}
			}
		}
	}
	else
	{
		for( x = 0 ; x<MAX_POSITIVE_AURAS ; x++ )
		{
			if( unitTarget->m_auras[x] != NULL && unitTarget->m_auras[x]->IsPositive())
			{
				p = unitTarget->m_auras[x]->GetSpellProto();
				if( Spell::HasMechanic(p, sMisc) )
				{
					unitTarget->m_auras[x]->AttemptDispel( u_caster );
				}
			}
		}
	}

	if( playerTarget && GetSpellProto()->NameHash == SPELL_HASH_DAZED && playerTarget->IsMounted() )
		TO_UNIT(playerTarget)->Dismount();
}

void Spell::SpellEffectSummonDeadPet(uint32 i)
{//this is pet resurrect
	if(p_caster == NULL )
		return;
	Pet* pPet = p_caster->GetSummon();
	if(pPet)
	{
		pPet->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
		pPet->SetUInt32Value(UNIT_FIELD_HEALTH, (uint32)((pPet->GetUInt32Value(UNIT_FIELD_MAXHEALTH) * damage) / 100));
		pPet->setDeathState(ALIVE);
		pPet->GetAIInterface()->HandleEvent(EVENT_FOLLOWOWNER, pPet, 0);
		sEventMgr.RemoveEvents(pPet, EVENT_PET_DELAYED_REMOVE);
	}
}

/* This effect has 2 functions
 * 1. It delete's all current totems from the player
 * 2. It returns a percentage of the mana back to the player
 *
 * Bur kick my ass if this is not safe:P
*/

void Spell::SpellEffectDestroyAllTotems(uint32 i)
{
	if(p_caster == NULL || !p_caster->IsInWorld())
		return;

	uint32 energize_amt = 0;
	for(uint32 x=SUMMON_TYPE_TOTEM_1;x<SUMMON_TYPE_TOTEM_4+1;x++)
	{
		SummonPropertiesEntry * spe = NULL;
		switch(x)
		{
			case SUMMON_TYPE_TOTEM_1:
			case SUMMON_TYPE_TOTEM_2:
			case SUMMON_TYPE_TOTEM_3:
			case SUMMON_TYPE_TOTEM_4:
			{
				spe = dbcSummonProps.LookupEntryForced(x);
			}break;
		}

		if(spe == NULL)
			continue;

		// atm totems are considert creature's
		if(p_caster->m_SummonSlots[spe->slot]!= NULL)
		{
			uint32 SpellID = p_caster->m_SummonSlots[spe->slot]->GetUInt32Value(UNIT_CREATED_BY_SPELL);
			SpellEntry * sp = NULL;
			sp = dbcSpell.LookupEntry(SpellID);
			if (sp == NULL)
				continue;
			if( sp->manaCost )
				energize_amt += float2int32( (sp->manaCost) * (damage/100.0f) );
			else if( sp->ManaCostPercentage )
				energize_amt += float2int32(((p_caster->GetUInt32Value(UNIT_FIELD_BASE_MANA)*sp->ManaCostPercentage)/100.0f) * (damage/100.0f) );

			if(p_caster->m_SummonSlots[spe->slot]->IsTotem())
				p_caster->SummonExpireSlot(spe->slot);
			else
			{
				p_caster->m_SummonSlots[spe->slot]->DeleteMe();
				p_caster->m_SummonSlots[spe->slot]=NULLCREATURE;
			}
		}
	}

	if( energize_amt > 0 )
		p_caster->Energize(p_caster, GetSpellProto()->Id, energize_amt, POWER_TYPE_MANA);
}

void Spell::SpellEffectSummonDemonOld(uint32 i)
{
	if(p_caster == NULL ) //p_caster->getClass() != WARLOCK ) //summoning a demon shouldn't be warlock only, see spells 25005, 24934, 24810 etc etc
		return;
	Pet* pPet = NULL;
	pPet = p_caster->GetSummon();
	if(pPet!= NULL)
	{
		pPet->Dismiss(false);
		pPet = NULL;
	}

	CreatureInfo *ci = CreatureNameStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(ci)
	{
		pPet = objmgr.CreatePet();
		pPet->SetInstanceID(p_caster->GetInstanceID());
		pPet->CreateAsSummon(GetSpellProto()->EffectMiscValue[i], ci, TO_CREATURE(pPet), p_caster, GetSpellProto(), 1, 300000);
	}

	//Create Enslave Aura if its inferno spell
	if(GetSpellProto()->Id == 1122)
	{
		SpellEntry *spellInfo = dbcSpell.LookupEntry(11726);

		Spell* sp(new Spell(TO_OBJECT(pPet),spellInfo,true,NULLAURA));
		SpellCastTargets tgt;
		tgt.m_unitTarget=pPet->GetGUID();
		sp->prepare(&tgt);
	}
}

void Spell::SpellEffectResurrect(uint32 i) // Resurrect (Flat)
{
	if( playerTarget == NULL )
	{
		if( corpseTarget == NULL )
		{
			// unit resurrection handler
			if(unitTarget != NULL )
			{
				if(unitTarget->GetTypeId()==TYPEID_UNIT && unitTarget->IsPet() && unitTarget->isDead())
				{
					uint32 hlth = ((uint32)GetSpellProto()->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) : (uint32)GetSpellProto()->EffectBasePoints[i];
					uint32 mana = ((uint32)GetSpellProto()->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1) : (uint32)GetSpellProto()->EffectBasePoints[i];

					if(!unitTarget->IsPet())
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					else
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_PET_DELAYED_REMOVE);
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, hlth);
					unitTarget->SetUInt32Value(UNIT_FIELD_POWER1, mana);
					unitTarget->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
					unitTarget->setDeathState(ALIVE);
					unitTarget->ClearLoot();
					TO_CREATURE(unitTarget)->ClearTag();
				}
			}

			return;
		}
		playerTarget = objmgr.GetPlayer(corpseTarget->GetUInt32Value(CORPSE_FIELD_OWNER));
		if( playerTarget == NULL )
			return;
	}

	if(playerTarget->isAlive() || !playerTarget->IsInWorld() || playerTarget->PreventRes)
		return;

	uint32 health = GetSpellProto()->EffectBasePoints[i];
	uint32 mana = GetSpellProto()->EffectMiscValue[i];

	playerTarget->m_resurrectHealth = health;
	playerTarget->m_resurrectMana = mana;

	SendResurrectRequest(playerTarget);
	playerTarget->SetMovement(MOVE_UNROOT, 1);
}

void Spell::SpellEffectAttackMe(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	unitTarget->GetAIInterface()->AttackReaction(u_caster,1,0);
}

void Spell::SpellEffectSkinPlayerCorpse(uint32 i)
{
	Corpse* corpse = NULLCORPSE;
	if(playerTarget == NULL)
	{
		// means we're "skinning" a corpse
		corpse = objmgr.GetCorpse((uint32)m_targets.m_unitTarget);  // hacky
	}
	else if(playerTarget->getDeathState() == CORPSE)	// repopped while we were casting
	{
		corpse = objmgr.GetCorpse(playerTarget->GetLowGUID());
	}

	if(m_caster == NULL || !m_caster->IsPlayer())
		return;

	if(playerTarget  != NULL && corpse == NULL )
	{
		if(!playerTarget->m_bg || !playerTarget->isDead() || playerTarget->m_insigniaTaken)
			return;

		// Set all the lootable stuff on the player. If he repops before we've looted, we'll set the flags
		// on corpse then :p
		playerTarget->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);

		// don't allow him to spawn a corpse
		playerTarget->m_insigniaTaken = true;

		// and.. force him to the graveyard and repop him.
		// this will also convert the corpse to bones.
		Corpse* pCorpse = NULL;
		pCorpse = playerTarget->RepopRequestedPlayer();

		// sanity checks
		if( pCorpse == NULL )
		{
			playerTarget->m_insigniaTaken = false;
			return;
		}

		// Send a message to the died player, telling him he has to resurrect at the graveyard.
		// Send an empty corpse location too, :P
		playerTarget->GetSession()->OutPacket(SMSG_PLAYER_SKINNED, 1, "\x00");
		playerTarget->GetSession()->OutPacket(MSG_CORPSE_QUERY, 1, "\x00");

		// fill loot vector
		playerTarget->GenerateLoot(pCorpse);

		// flag as lootable
		pCorpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, CORPSE_DYN_FLAG_LOOTABLE);

		// send the corpse's loot
		if( pCorpse != NULL )		// should never be null but /shrug
			p_caster->SendLoot(pCorpse->GetGUID(), pCorpse->GetMapId(), 2);

	}else if(corpse!= NULL)
	{
		if(corpse->GetUInt32Value(CORPSE_FIELD_FLAGS) != 60)
		{
			// cheater?
			return;
		}

		// find the corpses' owner
		Player* owner = NULL;
		//must be on same map!!
		owner = m_caster->GetMapMgr()->GetPlayer(corpse->GetUInt32Value(CORPSE_FIELD_OWNER));
		if(owner == NULL || owner->m_bg == NULL)
			return;

		owner->GetSession()->OutPacket(SMSG_PLAYER_SKINNED, 1, "\x00");
		owner->GetSession()->OutPacket(MSG_CORPSE_QUERY, 1, "\x00");

		// fill loot vector
		owner->GenerateLoot(corpse);

		// set lootable flag
		corpse->SetUInt32Value(CORPSE_FIELD_DYNAMIC_FLAGS, 1); // sets it so you can loot the plyr

		// remove skinnable flag
		corpse->SetUInt32Value(CORPSE_FIELD_FLAGS, 5);

		// remove owner association
		corpse->SetUInt64Value(CORPSE_FIELD_OWNER, 0);
		corpse->SetCorpseState(CORPSE_STATE_BONES);
		corpse->DeleteFromDB();
		objmgr.CorpseAddEventDespawn(corpse);

		// send loot
		p_caster->SendLoot(corpse->GetGUID(), corpse->GetMapId(), 2);
	}
}

void Spell::SpellEffectSkill(uint32 i)
{
	// Used by professions only
	// Effect should be renamed in RequireSkill
	if(p_caster == NULL )
		return;

	uint32 val = damage * 75 > 450 ? 450 :damage * 75;

	if( p_caster->_GetSkillLineMax(GetSpellProto()->EffectMiscValue[i]) >= val )
		return;

	if( GetSpellProto()->EffectMiscValue[i] == SKILL_RIDING )
		p_caster->_AddSkillLine( GetSpellProto()->EffectMiscValue[i], val, val );
	else
	{
		if( p_caster->_HasSkillLine(GetSpellProto()->EffectMiscValue[i]) )
			p_caster->_ModifySkillMaximum(GetSpellProto()->EffectMiscValue[i], val);
		else
			p_caster->_AddSkillLine( GetSpellProto()->EffectMiscValue[i], 1, val);
	}
}

void Spell::SpellEffectApplyPetAura(uint32 i)
{
	SpellEffectApplyAura(i);
}

void Spell::SpellEffectDummyMelee( uint32 i ) // Normalized Weapon damage +
{

	if( unitTarget == NULL || u_caster == NULL )
		return;

	uint32 pct_dmg_mod = 100;
	if( p_caster != NULL &&  GetSpellProto()->NameHash == SPELL_HASH_OVERPOWER) //warrior : overpower - let us clear the event and the combopoint count
	{
		p_caster->NullComboPoints(); //some say that we should only remove 1 point per dodge. Due to cooldown you can't cast it twice anyway..
	}
	else if( GetSpellProto()->NameHash == SPELL_HASH_DEVASTATE)
	{
		// Player can apply only 58567 Sunder Armor effect.
		Aura* aura = u_caster->FindActiveAura(58567);
		if(aura == NULL)
			u_caster->CastSpell(unitTarget, 58567, true);
		else
		{
			if(u_caster->HasAura(58388))
				aura->ModStackSize(2);
			else
				aura->ModStackSize(1);
			damage *= aura->stackSize;
		}
	}
	// rogue - mutilate ads dmg if target is poisoned
	// pure hax (damage join)
	else if( GetSpellProto()->NameHash == SPELL_HASH_MUTILATE )
	{
		damage = damage * 2;
		damage += CalculateDamage(u_caster, unitTarget, MELEE, GetSpellProto());
		damage += CalculateDamage(u_caster, unitTarget, OFFHAND, GetSpellProto());

		if( unitTarget->IsPoisoned() )
			pct_dmg_mod = 120;
	}
	//Hemorrhage
	else if( p_caster != NULL && GetSpellProto()->NameHash == SPELL_HASH_HEMORRHAGE )
		p_caster->AddComboPoints(p_caster->GetSelection(), 1);

	switch( GetSpellProto()->Id )
	{
	// AMBUSH
	case 8676: add_damage = 101; return; break;			// r1
	case 8724: add_damage = 145; return; break;			// r2
	case 8725: add_damage = 180; return; break;			// r3
	case 11267: add_damage = 269; return; break;		// r4
	case 11268: add_damage = 332; return; break;		// r5
	case 11269: add_damage = 420; return; break;		// r6
	case 27441: add_damage = 487; return; break;		// r7
	case 48689: add_damage = 670; return; break;		// r8
	case 48690: add_damage = 1018; return; break;		// r9
	case 48691: add_damage = 1198; return; break;		// r10

	// BACKSTAB
	case 53: add_damage = 15; return; break;			// r1
	case 2589: add_damage = 30; return; break;			// r2
	case 2590: add_damage = 48; return; break;			// r3
	case 2591: add_damage = 69; return; break;			// r4
	case 8721: add_damage = 90; return; break;			// r5
	case 11279: add_damage = 135; return; break;		// r6
	case 11280: add_damage = 165; return; break;		// r7
	case 11281: add_damage = 210; return; break;		// r8
	case 25300: add_damage = 225; return; break;		// r9
	case 26863: add_damage = 255; return; break;		// r10
	case 48656: add_damage = 383; return; break;		// r11
	case 48657: add_damage = 465; return; break;		// r12
	}

	// rogue ambush etc
	for (uint32 x =0;x<3;x++)
	{
		if(GetSpellProto()->Effect[x] == SPELL_EFFECT_WEAPON_PERCENT_DAMAGE)
		{
			add_damage = damage * (GetSpellProto()->EffectBasePoints[x]+1) /100;
			return;
		}
	}

	uint32 _type;
	if( GetType() == SPELL_DMG_TYPE_RANGED )
		_type = RANGED;
	else
	{
		if (GetSpellProto()->Flags4 & FLAGS4_OFFHAND)
			_type =  OFFHAND;
		else
			_type = MELEE;
	}
	u_caster->Strike( unitTarget, _type, GetSpellProto(), damage, pct_dmg_mod, 0, false, false );
}

void Spell::SpellEffectSpellSteal( uint32 i )
{
	if ( unitTarget  == NULL ||  u_caster  == NULL || !unitTarget->isAlive())
		return;

	if( p_caster && p_caster->GetGUID() != unitTarget->GetGUID() )
	{
		if( unitTarget->IsPvPFlagged() )
		{
			if( p_caster->IsPlayer() )
				TO_PLAYER( p_caster )->PvPToggle();
			else
				p_caster->SetPvPFlag();
		}
	}

	Aura* aur = NULL;
	uint32 start,end;
	int32 spells_to_steal = damage;
	uint32 m;
	if(isAttackable(u_caster,unitTarget))
	{
		start=0;
		end=MAX_POSITIVE_AURAS;
	}
	else
		return;

	for(uint32 x=start;x<end;x++)
	{
		if(unitTarget->m_auras[x])
		{
			aur = unitTarget->m_auras[x];
			if(aur != NULL && aur->GetSpellId() != 15007 && !aur->IsPassive() && aur->IsPositive() && !(aur->GetSpellProto()->Flags5 & FLAGS5_NOT_STEALABLE)) //Nothing can dispel resurrection sickness
			{
				if(aur->GetSpellProto()->DispelType == DISPEL_MAGIC && aur->GetDuration() > 0)
				{
					WorldPacket data(SMSG_SPELLDISPELLOG, 16);
					data << m_caster->GetNewGUID();
					data << unitTarget->GetNewGUID();
					data << (uint32)1;
					data << aur->GetSpellId();
					m_caster->SendMessageToSet(&data,true);
					Aura* aura(new Aura(aur->GetSpellProto(), (aur->GetDuration()>120000) ? 120000 : aur->GetDuration(), u_caster, u_caster));
					aura->stackSize = aur->stackSize;

					// copy the mods across
					for( m = 0; m < aur->GetModCount(); ++m )
					{
						Modifier *mod = aur->GetMod(m);
						aura->AddMod(mod->m_type, mod->m_baseAmount, mod->m_miscValue, mod->i);
					}

					u_caster->AddAura(aura);
					unitTarget->RemoveAuraBySlot(x);
					if( --spells_to_steal <= 0 )
						break; //exit loop now
				}
			}
		}
	}
}

void Spell::SpellEffectProspecting(uint32 i)
{
	if( p_caster == NULL)
		return;

	if( itemTarget == NULL) // this should never happen
	{
		SendCastResult(SPELL_FAILED_ITEM_GONE);
		return;
	}

	uint32 entry = itemTarget->GetEntry();

	if(p_caster->GetItemInterface()->RemoveItemAmt(entry, 5))
	{
		p_caster->SetLootGUID(p_caster->GetGUID());
		lootmgr.FillItemLoot(&p_caster->m_loot, entry, p_caster->GetTeam());
		p_caster->SendLoot(p_caster->GetGUID(), p_caster->GetMapId(), 2);
	}
	else // this should never happen either
		SendCastResult(SPELL_FAILED_NEED_MORE_ITEMS);
}

void Spell::SpellEffectResurrectNew(uint32 i)
{
	if(!u_caster)
		return;
	//base p =hp,misc mana
	if( playerTarget == NULL)
	{
		if( corpseTarget == NULL)
		{
			// unit resurrection handler
			if(unitTarget != NULL)
			{
				if(unitTarget->GetTypeId()==TYPEID_UNIT && unitTarget->IsPet() && unitTarget->isDead())
				{
					uint32 hlth = ((uint32)GetSpellProto()->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH) : (uint32)GetSpellProto()->EffectBasePoints[i];
					uint32 mana = ((uint32)GetSpellProto()->EffectBasePoints[i] > unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1)) ? unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1) : (uint32)GetSpellProto()->EffectBasePoints[i];

					if(!unitTarget->IsPet())
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					else
					{
						sEventMgr.RemoveEvents(unitTarget, EVENT_PET_DELAYED_REMOVE);
						sEventMgr.RemoveEvents(unitTarget, EVENT_CREATURE_REMOVE_CORPSE);
					}
					unitTarget->SetUInt32Value(UNIT_FIELD_HEALTH, hlth);
					unitTarget->SetUInt32Value(UNIT_FIELD_POWER1, mana);
					unitTarget->SetUInt32Value(UNIT_DYNAMIC_FLAGS, 0);
					unitTarget->setDeathState(ALIVE);
					unitTarget->ClearLoot();
					TO_CREATURE(unitTarget)->ClearTag();
				}
			}

			return;
		}
		playerTarget = objmgr.GetPlayer(corpseTarget->GetUInt32Value(CORPSE_FIELD_OWNER));
		if(playerTarget == NULL)
			return;
	}

	if(playerTarget->isAlive() || !playerTarget->IsInWorld() || playerTarget->PreventRes)
		return;
	//resurr
	playerTarget->resurrector = u_caster->GetLowGUID();
	playerTarget->m_resurrectHealth = damage;
	playerTarget->m_resurrectMana = GetSpellProto()->EffectMiscValue[i];
	playerTarget->m_resurrectLoction = u_caster->GetPosition();

	SendResurrectRequest(playerTarget);
}

void Spell::SpellEffectTranformItem(uint32 i)
{
	bool result;
	AddItemResult result2;

	if( i_caster == NULL)
		return;
	uint32 itemid=GetSpellProto()->EffectItemType[i];
	if(!itemid)
		return;

	//Save durability of the old item
	Player* owner=i_caster->GetOwner();
	uint32 dur= i_caster->GetUInt32Value(ITEM_FIELD_DURABILITY);

	result  = owner->GetItemInterface()->SafeFullRemoveItemByGuid(i_caster->GetGUID());
	if(!result)
	{
		//something went wrong if this happen, item doesnt exist, so it wasnt destroyed.
		return;
	}

	i_caster = NULLITEM;

	Item* it=objmgr.CreateItem(itemid,owner);
	it->SetDurability(dur);

	//additem
	result2 = owner->GetItemInterface()->AddItemToFreeSlot(it);
	if(!result2) //should never get here
	{
		owner->GetItemInterface()->BuildInventoryChangeError(NULLITEM, NULLITEM,INV_ERR_BAG_FULL);
		it->DeleteMe();
		it = NULLITEM;
	}
}

void Spell::SpellEffectEnvironmentalDamage(uint32 i)
{
	if(playerTarget == NULL)
		return;

	if(playerTarget->SchoolImmunityList[GetSpellProto()->School])
	{
		SendCastResult(SPELL_FAILED_IMMUNE);
		return;
	}

	//this is GO, not unit
	m_caster->SpellNonMeleeDamageLog(playerTarget,GetSpellProto()->Id,damage, pSpellId==0);

	WorldPacket data(SMSG_ENVIRONMENTALDAMAGELOG, 13);
	data << playerTarget->GetGUID();
	data << uint8(DAMAGE_FIRE);
	data << uint32(damage);
	playerTarget->SendMessageToSet( &data, true );
}

void Spell::SpellEffectDismissPet(uint32 i)
{
	// remove pet.. but don't delete so it can be called later
	if( p_caster == NULL)
		return;

	Pet* pPet = NULL;
	pPet = p_caster->GetSummon();
	if(pPet == NULL)
		return;
	pPet->Remove(true, true, true);
}

void Spell::SpellEffectEnchantHeldItem( uint32 i )
{
	if( playerTarget == NULL )
		return;

	Item* item = NULL;
	item = playerTarget->GetItemInterface()->GetInventoryItem( EQUIPMENT_SLOT_MAINHAND );
	if( item == NULL )
		return;

	uint32 Duration = 1800; // Needs to be found in dbc.. I guess?
	EnchantEntry * Enchantment = NULL;
	Enchantment = dbcEnchant.LookupEntry( GetSpellProto()->EffectMiscValue[i] );

	if( Enchantment == NULL )
		return;

	item->RemoveEnchantment( 1 );
	item->AddEnchantment( Enchantment, Duration, false, true, false, 1 );
	item->SaveToDB(-1, EQUIPMENT_SLOT_MAINHAND, true, NULL);
}

void Spell::SpellEffectAddHonor(uint32 i)
{
	if( playerTarget != NULL )
		HonorHandler::AddHonorPointsToPlayer( playerTarget, GetSpellProto()->EffectBasePoints[i] );
}

void Spell::SpellEffectSpawn(uint32 i)
{
	// this effect is mostly for custom teleporting
	switch(GetSpellProto()->Id)
	{
		case 10418: // Arugal spawn-in spell , teleports it to 3 locations randomly sneeking players (bastard ;P)
		{
			if( u_caster == NULL || u_caster->IsPlayer())
				return;

			static float coord[3][3]= {{-108.9034f,2129.5678f,144.9210f},{-108.9034f,2155.5678f,155.678f},{-77.9034f,2155.5678f,155.678f}};

			int i = (int)(rand()%3);
			u_caster->GetAIInterface()->SendMoveToPacket(coord[i][0],coord[i][1],coord[i][2],0.0f,0,u_caster->GetAIInterface()->getMoveFlags());
		}
	}
}

void Spell::SpellEffectApplyAura128(uint32 i)
{
	if(m_caster == NULL || !m_caster->IsInWorld())
		return;

	if( u_caster )
	{
		if(GetSpellProto()->EffectApplyAuraName[i] != 0)
			SpellEffectApplyAA(i);
	}
	else if(GetSpellProto() != NULL)
	{
		if(GetSpellProto()->EffectApplyAuraName[i] != 0)
			SpellEffectApplyAura(i);
	}
}

void Spell::SpellEffectRedirectThreat(uint32 i)
{
	if(!p_caster || !playerTarget)
		return;
	if(GetSpellProto()->NameHash == SPELL_HASH_TRICKS_OF_THE_TRADE)
		p_caster->CastSpell(playerTarget,57933,true);

	p_caster->SetRedirectThreat(playerTarget,GetSpellProto()->EffectBasePoints[i]+1,GetDuration());
}

void Spell::SpellEffectPlayMusic(uint32 i)
{
	if(!unitTarget)
		return;

	unitTarget->PlaySoundToSet(m_spellInfo->EffectMiscValue[i]);
}

void Spell::SpellEffectKillCredit( uint32 i )
{
	CreatureProto * cp = CreatureProtoStorage.LookupEntry( GetSpellProto()->EffectMiscValue[i] );
	if ( playerTarget != NULL && cp != NULL )
		sQuestMgr._OnPlayerKill( playerTarget, GetSpellProto()->EffectMiscValue[i]);
}

void Spell::SpellEffectRestoreManaPct(uint32 i)
{
	if( u_caster == NULL || unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 maxMana = (uint32)unitTarget->GetUInt32Value(UNIT_FIELD_MAXPOWER1);
	uint32 modMana = damage * maxMana / 100;

	u_caster->Energize(unitTarget, pSpellId ? pSpellId : GetSpellProto()->Id, modMana, POWER_TYPE_MANA);
}

void Spell::SpellEffectRestoreHealthPct(uint32 i)
{
	if( u_caster == NULL || unitTarget == NULL || !unitTarget->isAlive())
		return;

	uint32 maxHp = (uint32)unitTarget->GetUInt32Value(UNIT_FIELD_MAXHEALTH);
	uint32 modHp = damage * maxHp / 100;

	u_caster->Heal(unitTarget, GetSpellProto()->Id, modHp);
}

void Spell::SpellEffectForceCast( uint32 i )
{
	if( unitTarget == NULL )
		return;

	SpellEntry* TriggeredSpell = dbcSpell.LookupEntryForced( GetSpellProto()->EffectTriggerSpell[i] );
	if( TriggeredSpell == NULL )
		return;

	if(GetSpellProto() == TriggeredSpell) // Infinite loop fix.
		return;

	unitTarget->CastSpell( unitTarget, TriggeredSpell, true );
}

void Spell::SpellEffectTriggerSpellWithValue(uint32 i)
{
	if( unitTarget == NULL )
		return;

	SpellEntry* TriggeredSpell = dbcSpell.LookupEntryForced(GetSpellProto()->EffectTriggerSpell[i]);
	if( TriggeredSpell == NULL )
		return;

	if(GetSpellProto() == TriggeredSpell) // Infinite loop fix.
		return;

	Spell* sp= CREATESPELL(m_caster,dbcSpell.LookupEntry(TriggeredSpell->Id),true,NULLAURA);

	for(uint32 x=0;x<3;x++)
	{
		sp->forced_basepoints[x] = TriggeredSpell->EffectBasePoints[i];
	}

	SpellCastTargets tgt(unitTarget->GetGUID());
	sp->prepare(&tgt);
}

void Spell::SpellEffectJump(uint32 i)
{
	if( u_caster == NULL)
		return;

	if(u_caster->IsCreature() && isTargetDummy(u_caster->GetEntry()))
		return;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	if (m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION)
	{
		if(m_targets.m_destX == 0 || m_targets.m_destY == 0 || m_targets.m_destZ == 0)
			return;
		x = m_targets.m_destX;
		y = m_targets.m_destY;
		z = m_targets.m_destZ;
	}
	else
	{
		if( unitTarget == NULL )
			return;
		x = unitTarget->GetPositionX();
		y = unitTarget->GetPositionY();
		z = unitTarget->GetPositionZ();
		float ang = m_caster->calcAngle(m_caster->GetPositionX(), m_caster->GetPositionY(), unitTarget->GetPositionX(), unitTarget->GetPositionY());
		ang = ang * float(M_PI) / 180.0f;
		float distance = 1.6f * unitTarget->GetFloatValue(OBJECT_FIELD_SCALE_X);
		x = x - (distance * cosf(ang));
		y = y - (distance * sinf(ang));
	}

	float speed = 10.0f;
	if (m_spellInfo->EffectMiscValue[i])
		speed = float(m_spellInfo->EffectMiscValue[i])/10;
	else if (m_spellInfo->EffectMiscValueB[i])
		speed = float(m_spellInfo->EffectMiscValueB[i])/10;
	uint32 moveTime = uint32(speed * 100);
	u_caster->GetAIInterface()->StopMovement(moveTime);
	u_caster->GetAIInterface()->JumpTo( x, y, z, moveTime, GetSpellProto()->EffectMiscValue[i] ? GetSpellProto()->EffectMiscValue[i] : GetSpellProto()->EffectMiscValueB[i], GetSpellProto()->EffectDieSides[i] );
	
	if( p_caster != NULL)
	{
		p_caster->ResetHeartbeatCoords();
		p_caster->DelaySpeedHack(moveTime);
	}
}

void Spell::SpellEffectTeleportToCaster(uint32 i)
{
	if(!u_caster || !unitTarget)
		return;
	float x = u_caster->GetPositionX();
	float y = u_caster->GetPositionY();
	float z = u_caster->GetPositionZ();
	unitTarget->Teleport(x,y,z,u_caster->GetOrientation(),u_caster->GetPhaseMask());
}

void Spell::SpellEffectMilling(uint32 i)
{
	if(p_caster == NULL)
		return;

	if(itemTarget == NULL)
	{
		SendCastResult(SPELL_FAILED_ITEM_GONE);
		return;
	}

	uint32 entry = itemTarget->GetEntry();

	if(p_caster->GetItemInterface()->RemoveItemAmt(entry, 5))
	{
		p_caster->SetLootGUID(p_caster->GetGUID());
		lootmgr.FillItemLoot(&p_caster->m_loot, entry, p_caster->GetTeam());
		p_caster->SendLoot(p_caster->GetGUID(), p_caster->GetMapId(), 2);
	}
	else
		SendCastResult(SPELL_FAILED_NEED_MORE_ITEMS);
}

void Spell::SpellEffectAllowPetRename( uint32 i )
{
	if( !unitTarget || !unitTarget->IsPet() )
		return;

	unitTarget->SetByte( UNIT_FIELD_BYTES_2, 2, 0x03);
}

void Spell::SpellEffectStartQuest(uint32 i)
{
	if( !playerTarget )
		return;

	playerTarget->StartQuest(GetSpellProto()->EffectMiscValue[i]);
}

// Spells: 46716-46730
void Spell::SpellEffectCreatePet(uint32 i)
{
	if( !playerTarget )
		return;

	if( playerTarget->getClass() != HUNTER )
		return;

	if( playerTarget->GetSummon() )
		playerTarget->GetSummon()->Remove( true, true, true );

	CreatureInfo *ci = CreatureNameStorage.LookupEntry( GetSpellProto()->EffectMiscValue[i] );
	if( ci != NULL )
	{
		Pet *pPet = objmgr.CreatePet();
		if(pPet != NULL)
		{
			pPet->CreateAsSummon( GetSpellProto()->EffectMiscValue[i], ci, TO_CREATURE(pPet), playerTarget, GetSpellProto(), 2, 0 );
			if(!pPet->IsInWorld())
			{
				pPet->Destruct();
				pPet = NULL;
			}
		}
	}
}

void Spell::SpellEffectTitanGrip(uint32 i)
{
	if (playerTarget == NULL)
		return;

	playerTarget->titanGrip = true;
}

bool Spell::SpellEffectUpdateQuest(uint32 questid)
{
	QuestLogEntry *en = NULL;
	en = p_caster->GetQuestLogForEntry( questid );
	if( en != NULL && en->GetMobCount( 0 ) < en->GetQuest()->required_mobcount[0])
	{
		en->SetMobCount( 0, en->GetMobCount( 0 ) + 1 );//(collumn,count)
		en->SendUpdateAddKill( 0 );//(collumn)
		en->UpdatePlayerFields();
		return true;
	}
	return false;
}

void Spell::SummonLightwell(uint32 i)
{
	if( p_caster == NULL )
		return;

	uint32 cr_entry = GetSpellProto()->EffectMiscValue[i];
	uint32 level = u_caster->getLevel();

	CreatureInfo * ci = CreatureNameStorage.LookupEntry(cr_entry);
	CreatureProto * cp = CreatureProtoStorage.LookupEntry(cr_entry);

	if(!ci || !cp) 
		return;
	Creature * summon = u_caster->GetMapMgr()->CreateCreature(cr_entry);
	if(summon == NULL)
		return;
	summon->SetInstanceID(u_caster->GetMapMgr()->GetInstanceID());
	summon->Load(cp, m_caster->GetMapMgr()->iInstanceMode, m_targets.m_destX,m_targets.m_destY, m_targets.m_destZ, m_caster->GetOrientation());
	summon->SetUInt32Value(UNIT_FIELD_LEVEL,level);
	summon->GetAIInterface()->disable_melee = true;
	summon->bInvincible = true;
	summon->DisableAI();
	summon->Root();
	summon->PushToWorld(u_caster->GetMapMgr());
	summon->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, u_caster->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	summon->_setFaction();
	summon->AddAura(new Aura(dbcSpell.LookupEntry(59907), GetDuration(), summon, summon));
	summon->FindActiveAura(59907)->SetProcCharges(10);
	summon->SetSummonOwnerSlot(p_caster->GetGUID(), 0);
	p_caster->m_SummonSlots[0] = summon;
	sEventMgr.AddEvent( summon, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
}

void Spell::SummonTotemCopy(uint32 i)
{
	if(u_caster== NULL)
		return;

	uint32 entry = GetSpellProto()->EffectMiscValue[i];

	CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
	CreatureProto* cp = CreatureProtoStorage.LookupEntry(entry);
	if(!ci || !cp)
		return;

	// Obtain the spell we will be casting.
	SpellEntry * TotemSpell = ObjectMgr::getSingleton().GetTotemSpell(GetSpellProto()->Id);
	if(TotemSpell == 0)
	{
		sLog.outSpellDebug("SummonTotemCopy has no spell to cast for summon spellid %u, Creature Id %u",GetSpellProto()->Id, entry);
		return;
	}

	Creature* pTotem = NULLCREATURE;
	pTotem = u_caster->GetMapMgr()->CreateCreature(entry);
	if(pTotem == NULL)
		return;

	uint32 slot = m_summonProperties->slot;
	if( slot < 7 )
		u_caster->m_SummonSlots[slot] = pTotem;
	pTotem->SetSummonOwnerSlot(u_caster->GetGUID(),int8(slot));
	pTotem->SetTotem(true);
	float x,y,z;
	if( m_targets.m_targetMask & TARGET_FLAG_DEST_LOCATION && m_targets.m_destX && m_targets.m_destY && m_targets.m_destZ )
	{
		x = m_targets.m_destX;
		y = m_targets.m_destY;
		z = m_targets.m_destZ;
	}
	else
	{
		x = u_caster->GetPositionX();
		y = u_caster->GetPositionY();
		z = u_caster->GetPositionZ();
	}
	pTotem->Load(cp, 0, x, y, z);
	pTotem->Create(ci->Name, u_caster->GetMapId(), x, y, z, u_caster->GetOrientation());
	uint32 displayID = ci->Male_DisplayID;
	pTotem->SetUInt32Value(OBJECT_FIELD_ENTRY, entry);
	pTotem->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);
	pTotem->SetUInt64Value(UNIT_FIELD_CREATEDBY, u_caster->GetGUID());
	pTotem->SetUInt32Value(UNIT_FIELD_LEVEL, u_caster->getLevel());
	pTotem->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, u_caster->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE));
	pTotem->SetUInt32Value(UNIT_FIELD_BYTES_0, (1 << 8) | (2 << 16) | (1 << 24));
	pTotem->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED | UNIT_FLAG_SELF_RES);
	pTotem->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
	pTotem->SetUInt32Value(UNIT_FIELD_RANGEDATTACKTIME, 2000);
	pTotem->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 1.0f);
	pTotem->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.0f);
	pTotem->SetUInt32Value(UNIT_FIELD_DISPLAYID, displayID);
	pTotem->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, displayID);
	pTotem->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	pTotem->SetUInt32Value(UNIT_CREATED_BY_SPELL, GetSpellProto()->Id);
	pTotem->SetPhaseMask(u_caster->GetPhaseMask());
	pTotem->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
	if( u_caster->IsPvPFlagged() )
		pTotem->SetPvPFlag();

	if( u_caster->IsFFAPvPFlagged() )
		pTotem->SetFFAPvPFlag();

	// Initialize faction stuff.
	pTotem->m_faction = u_caster->m_faction;
	pTotem->m_factionDBC = u_caster->m_factionDBC;
	pTotem->PushToWorld(m_caster->GetMapMgr());
	sEventMgr.AddEvent( pTotem, &Creature::SafeDelete, EVENT_CREATURE_SAFE_DELETE, GetDuration(), 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT );
	pTotem->DisableAI();
	pTotem->GetAIInterface()->totemspell = GetSpellProto();
	Spell* pSpell(new Spell(pTotem, TotemSpell, true, NULLAURA));
	SpellCastTargets targets;
	targets.m_destX = pTotem->GetPositionX();
	targets.m_destY = pTotem->GetPositionY();
	targets.m_destZ = pTotem->GetPositionZ();
	targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
	pSpell->prepare(&targets);
}

void Spell::SpellEffectCreateRandomItem(uint32 i) // Create Random Item
{
	if( p_caster == NULL)
		return;

	ItemPrototype *m_itemProto = NULL;
	Item* newItem = NULL;
	Item* add = NULL;
	uint8 slot = NULL;
	uint32 itemid;
	SlotResult slotresult;

	skilllinespell* skill = NULL;
	skill = objmgr.GetSpellSkill(GetSpellProto()->Id);

	itemid = GetSpellProto()->EffectItemType[i];
	m_itemProto = ItemPrototypeStorage.LookupEntry( GetSpellProto()->EffectItemType[i] );
	if ( m_itemProto == NULL || itemid == 0)
		return;

	uint32 item_count = 0;
	// Random Item to Create Jewelcrafting part
	RandomItemCreation * ric = RandomItemCreationStorage.LookupEntry( GetSpellProto()->Id );
	if(ric)
	{
		// If we Have Perfect Gem Cutting then we have a chance to create a Perfect Gem, according to comments on wowhead chance is between 20 and 30%
		if (Rand(ric->Chance) && ric->Skill == SKILL_JEWELCRAFTING && p_caster->HasSpell(55534))
		{
			m_itemProto = ItemPrototypeStorage.LookupEntry( ric->ItemToCreate );
			itemid	=	ric->ItemToCreate;
		}

		//Tarot and Decks from Inscription + Northrend Inscription Research + Minor Inscription Research
		//Northrend Alchemy
		if (ric->Skill == SKILL_INSCRIPTION || ric->Skill == SKILL_ALCHEMY)
		{
			uint32 k;
			RandomCardCreation * rcc = RandomCardCreationStorage.LookupEntry(GetSpellProto()->Id);
			if(rcc)
			{
				//Same chance for every card to appear according wowhead and wowwiki info
				k = RandomUInt(rcc->itemcount-1);
				m_itemProto = ItemPrototypeStorage.LookupEntry( rcc->ItemId[k] );
				itemid = rcc->ItemId[k];
				item_count = 1;
				switch(GetSpellProto()->Id)
				{
					case 61288:
					case 61177:
						{
							item_count = RandomUInt(2);//This 2 can make random scrolls and vellum 1 or 2 according to info
						}break;
					case 60893:
						{
							item_count = RandomUInt(3);//Creates 3 random elixir/potion from alchemy
						}break;
				}
			}
		}
	}
	// Profession Discoveries used in Northrend Alchemy and Inscription Research plus Minor research
	uint32 discovered_recipe = 0;
	std::set<ProfessionDiscovery*>::iterator itr = objmgr.ProfessionDiscoveryTable.begin();
	for ( ; itr != objmgr.ProfessionDiscoveryTable.end(); itr++ )
	{
		ProfessionDiscovery * pf = NULL;
		pf = ( *itr );
		if ( pf != NULL && GetSpellProto()->Id == pf->SpellId && p_caster->_GetSkillLineCurrent( skill->skilline ) >= pf->SkillValue && !p_caster->HasSpell( pf->SpellToDiscover ))
		{
			discovered_recipe = pf->SpellToDiscover;
			break;
		}
	}

	// if something discovered learn p_caster that recipe and broadcast message
	if ( discovered_recipe != 0 )
	{
		SpellEntry * se = NULL;
		se = dbcSpell.LookupEntry( discovered_recipe );
		if ( se != NULL )
		{
			p_caster->addSpell( discovered_recipe );
			WorldPacket * data;
			char msg[256];
			sprintf( msg, "%sDISCOVERY! %s has discovered how to create %s.|r", MSG_COLOR_GOLD, p_caster->GetName(), se->Name );
			data = sChatHandler.FillMessageData( CHAT_MSG_SYSTEM, LANG_UNIVERSAL,  msg, p_caster->GetGUID(), 0 );
			p_caster->GetMapMgr()->SendChatMessageToCellPlayers( p_caster, data, 2, 1, LANG_UNIVERSAL, p_caster->GetSession() );
			delete data;
		}
	}
	if( m_itemProto == NULL )
		return;

	// item count cannot be more than allowed in a single stack
	if (item_count > m_itemProto->MaxCount)
		item_count = m_itemProto->MaxCount;

	// item count cannot be more than item unique value
	if (m_itemProto->Unique && item_count > m_itemProto->Unique)
		item_count = m_itemProto->Unique;

	if(p_caster->GetItemInterface()->CanReceiveItem(m_itemProto, item_count, NULL))
		{
			SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			return;
		}

	slot = 0;
	add = p_caster->GetItemInterface()->FindItemLessMax(itemid,1, false);
	if (add == NULL)
	{
		slotresult = p_caster->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
		if(!slotresult.Result)
		{
			  SendCastResult(SPELL_FAILED_TOO_MANY_OF_ITEM);
			  return;
		}

		newItem =objmgr.CreateItem(itemid,p_caster);
		if(newItem == NULL)
			return;
		newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
		newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count);


		if(p_caster->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
			p_caster->GetSession()->SendItemPushResult(newItem,true,false,true,true,slotresult.ContainerSlot,slotresult.Slot,item_count);
		else
		{
			newItem->DeleteMe();
			newItem = NULLITEM;
		}

		if(skill!= NULL)
			DetermineSkillUp(skill->skilline);
	}
	else
	{
		//scale item_count down if total stack will be more than 20
		if(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count > 20)
		{
			uint32 item_count_filled;
			item_count_filled = 20 - add->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
			add->SetCount(20);
			add->m_isDirty = true;

			slotresult = p_caster->GetItemInterface()->FindFreeInventorySlot(m_itemProto);
			if(!slotresult.Result)
				item_count = item_count_filled;
			else
			{
				newItem =objmgr.CreateItem(itemid,p_caster);
				if(newItem == NULL)
					return;
				newItem->SetUInt64Value(ITEM_FIELD_CREATOR,m_caster->GetGUID());
				newItem->SetUInt32Value(ITEM_FIELD_STACK_COUNT, item_count - item_count_filled);
				if(!p_caster->GetItemInterface()->SafeAddItem(newItem,slotresult.ContainerSlot, slotresult.Slot))
				{
					newItem->DeleteMe();
					newItem = NULLITEM;
					item_count = item_count_filled;
				}
				else
					p_caster->GetSession()->SendItemPushResult(newItem, true, false, true, true, slotresult.ContainerSlot, slotresult.Slot, item_count-item_count_filled);
			}
		}
		else
		{
			add->SetCount(add->GetUInt32Value(ITEM_FIELD_STACK_COUNT) + item_count);
			add->m_isDirty = true;
			p_caster->GetSession()->SendItemPushResult(add, true,false,true,false,p_caster->GetItemInterface()->GetBagSlotByGuid(add->GetGUID()),0xFFFFFFFF,item_count);
		}
		if(skill)
			DetermineSkillUp(skill->skilline);
	}
}

void Spell::SpellEffectSetTalentSpecsCount(uint32 i)
{
	if(!p_caster)
		return;

	if(p_caster->m_talentActiveSpec >= damage) // activate primary spec
		p_caster->ApplySpec(0, false);

	p_caster->m_talentSpecsCount = damage;

	// Send update
	p_caster->smsg_TalentsInfo(false);
}

void Spell::SpellEffectActivateTalentSpec(uint32 i)
{
	if(!p_caster)
		return;

	if(p_caster->m_bg)
	{
		SendCastResult(SPELL_FAILED_NOT_IN_BATTLEGROUND);
		return;
	}

	// 1 = primary, 2 = secondary
	p_caster->ApplySpec(uint8(damage - 1), false);

	// Use up all our power.
	switch(p_caster->GetPowerType())
	{
	case POWER_TYPE_MANA:
		p_caster->SetPower(POWER_TYPE_MANA, 0);
		break;
	case POWER_TYPE_RAGE:
		p_caster->SetPower(POWER_TYPE_RAGE, 0);
		break;
	case POWER_TYPE_ENERGY:
		p_caster->SetPower(POWER_TYPE_ENERGY, 0);
		break;
	case POWER_TYPE_RUNE:
		p_caster->SetPower(POWER_TYPE_RUNE, 0);
		break;
	}
}

void Spell::SpellEffectDisengage(uint32 i)
{
	if(unitTarget == NULL || !unitTarget->isAlive())
		return;

	unitTarget->knockback(GetSpellProto()->EffectBasePoints[i]+1, GetSpellProto()->EffectMiscValue[i], true);
}

void Spell::SpellEffectClearFinishedQuest(uint32 i)
{
	if (playerTarget == NULL)
		return;

	playerTarget->m_finishedQuests.erase(GetSpellProto()->EffectMiscValue[i]);
}

void Spell::SpellEffectApplyDemonAura( uint32 i )
{
	if (u_caster == NULL || !u_caster->IsPet() || TO_PET(u_caster)->GetPetOwner() == NULL)
		return;
	Aura* pAura = NULL;

	pAura = new Aura(GetSpellProto(), GetDuration(), u_caster, u_caster);
	Aura* otheraura = new Aura(GetSpellProto(), GetDuration(), u_caster, TO_PET(u_caster)->GetPetOwner());
	pAura->targets.insert(TO_PET(u_caster)->GetPetOwner()->GetUIdFromGUID());
	for (uint32 j=0; j<3; ++j)
	{
		pAura->AddMod(GetSpellProto()->EffectApplyAuraName[j], j == i? damage : CalculateEffect(j, unitTarget), GetSpellProto()->EffectMiscValue[j], j);
		otheraura->AddMod(GetSpellProto()->EffectApplyAuraName[j], j == i? damage : CalculateEffect(j, unitTarget), GetSpellProto()->EffectMiscValue[j], j);
	}

	u_caster->AddAura(pAura);
	TO_PET(u_caster)->GetPetOwner()->AddAura(otheraura);
}

void Spell::SpellEffectRemoveAura(uint32 i)
{
	if (!unitTarget)
		return;

	unitTarget->RemoveAura(GetSpellProto()->EffectTriggerSpell[i], unitTarget->GetGUID());
}

void Spell::SpellEffectActivateRune(uint32 i)
{
	if( !p_caster )
		return;

	if( p_caster->getClass() != DEATHKNIGHT )
		return;
    uint32 count = damage;
    if (count == 0) 
		count = 1;

	for( uint8 j = 0; j < 6 && count; ++j )
	{
		if( p_caster->GetRune(j) == RUNE_TYPE_RECHARGING && p_caster->GetRune(j) == GetSpellProto()->EffectMiscValue[i] )
		{
			p_caster->SetRune(j, p_caster->GetBaseRune(j));
			p_caster->ClearRuneCooldown(j);
		}
	}
}

void Spell::SpellEffectFailQuest(uint32 i)
{
	if( !p_caster )
		return;
	Quest * qst = QuestStorage.LookupEntry(GetSpellProto()->EffectMiscValue[i]);
	if(!qst)
		return;

	sQuestMgr.SendQuestFailed(FAILED_REASON_FAILED, qst, p_caster);
}