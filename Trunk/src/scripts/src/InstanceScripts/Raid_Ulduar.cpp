/*
 * Sun++ Scripts for Aspire MMORPG Server
 * Copyright (C) 2009 Sun++ Team <http://www.sunscripting.com/>
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
#include "Raid_Ulduar.h"


#define MAP_ULDUAR 603
#define NPC_LEVIATHAN 33113
#define NPC_XT002 33293
#define NPC_KOLOGARN 32930
#define NPC_AURIAYA 33515

class UlduarScript : public MoonInstanceScript
{
public:
	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION( UlduarScript, MoonInstanceScript );
	UlduarScript( MapMgr *pMapMgr ) : MoonInstanceScript( pMapMgr )
	{
		// Way to select bosses
		BuildEncounterMap();

		if( mEncounters.size() == 0 )
			return;

	};

	void OnCreatureDeath( Creature *pVictim, Unit *pKiller )
	{
		if ( pVictim == NULLCREATURE || pVictim->GetCreatureInfo() == NULL || pVictim->GetCreatureInfo()->Rank != ELITE_WORLDBOSS )
			return;

		EncounterMap::iterator Iter = mEncounters.find( pVictim->GetEntry() );
		if ( Iter == mEncounters.end() )
			return;

		( *Iter ).second.mState = State_Finished;

	};
	
	void Destroy()
	{
		delete this;
	};

};

// Ulduar Teleporter
void UlduarTeleporter::GossipHello(Object * pObject, Player * Plr, bool AutoSend)
{
	GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 0, Plr);
	UlduarScript* pCheck = (UlduarScript*)Plr->GetMapMgr()->GetScript();
	Menu->AddItem(0, "Teleport to the Expedition Base Camp.", 0);
    Menu->AddItem(0, "Teleport to the Formation Grounds.", 1);
    if (pCheck->GetEncounterState(NPC_LEVIATHAN) == State_Finished)
    {
        Menu->AddItem(0, "Teleport to the Colossal Forge.", 2);
        if (pCheck->GetEncounterState(NPC_XT002) == State_Finished)
        {
            Menu->AddItem(0, "Teleport to the Scrapyard", 3);
            Menu->AddItem(0, "Teleport to the Antechamber of Ulduar", 4);
            if (pCheck->GetEncounterState(NPC_KOLOGARN) == State_Finished)
            {
                Menu->AddItem(0, "Teleport to the Shattered Walkway", 5);
                if (pCheck->GetEncounterState(NPC_AURIAYA) == State_Finished)
                    Menu->AddItem(0, "Teleport to the Conservatory of Life", 6);
            }
        }
    }

    if(AutoSend)
		Menu->SendTo(Plr);
};

void UlduarTeleporter::GossipSelectOption(Object * pObject, Player * Plr, uint32 Id, uint32 IntId, const char *Code)
{
	switch(IntId)
	{
	case 0:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), -735.864075f, -93.616364f, 429.841797f, 0.079723f);
		break;
	case 1:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 130.710297f, -35.272095f, 409.804901f, 6.276515f);
		break;
	case 2:
		Plr->SafeTeleport(603, Plr->GetInstanceID(), 539.894897f, -11.009894f, 409.804749f, 0.021830f);
		break;
    case 3:
        Plr->SafeTeleport(603, Plr->GetInstanceID(), 926.292f, -11.4635f, 418.595f, 0);
        break;
    case 4:
        Plr->SafeTeleport(603, Plr->GetInstanceID(), 1498.09f, -24.246f, 420.967f, 0);
        break;
    case 5:
        Plr->SafeTeleport(603, Plr->GetInstanceID(), 1859.45f, -24.1f, 448.9f, 0);
        break;
    case 6:
        Plr->SafeTeleport(603, Plr->GetInstanceID(), 2086.27f, -24.3134f, 421.239f, 0);
        break;
    }
};



////////////////////////
//Partial Boss Scripts//
////////////////////////

// Algalon the Observer
#define CN_ALGALON_THE_OBSERVER		32871
#define QUANTUM_STRIKE				64395 //Phase 1
#define PHASE_PUNCH					64412 //Phase 1
#define COSMIC_SMASH				62311 //Phase 1

class AlgalonTheObserverAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AlgalonTheObserverAI, MoonScriptBossAI);
	AlgalonTheObserverAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(QUANTUM_STRIKE, Target_Current, 10, 0, 0);
		AddSpell(PHASE_PUNCH, Target_RandomPlayer, 3, 15.0f, 0);
		AddSpell(COSMIC_SMASH, Target_RandomUnit, 25, 0, 0);

		//Emotes
		AddEmote(Event_OnCombatStart, "Your actions are illogical. All possible results for this encounter have been calculated. The Pantheon will receive the Observer's message regardless of outcome.", Text_Yell, 15386);
		AddEmote(Event_OnTargetDied, "Loss of life, unavoidable.", Text_Yell, 15387); 
		AddEmote(Event_OnTargetDied, "I do what I must.", Text_Yell, 15388); 
		AddEmote(Event_OnDied, "I have seen worlds bathed in the Makers' flames. Their denizens fading without so much as a whimper. Entire planetary systems born and raised in the time that it takes your mortal hearts to beat once. Yet all throughout, my own heart, devoid of emotion... of empathy. I... have... felt... NOTHING! A million, million lives wasted. Had they all held within them your tenacity? Had they all loved life as you do?", Text_Yell, 15393); 
		AddEmote(Event_OnDied, "Perhaps it is your imperfection that which grants you free will. That allows you to persevere against cosmically calculated odds. You prevailed where the Titans own perfect creations have failed.", Text_Yell, 15401); 
		AddEmote(Event_OnDied, "I've rearranged the reply code. Your planet will be spared. I cannot be certain of my own calculations anymore.", Text_Yell, 15401); 
		AddEmote(Event_OnDied, "I lack the strength to transmit the signal. You must hurry. Find a place of power close to the skies.", Text_Yell, 15402); 
	}
};

// Ancient Conservator
#define CN_ANCIENT_CONSERVATOR		33203
#define CONSERVATORS_GRIP			62532
#define NATURES_FURY				62589 // 63571?

class AncientConservatorAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AncientConservatorAI, MoonScriptBossAI);
	AncientConservatorAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(CONSERVATORS_GRIP, Target_Current, 20, 5, 0);
		AddSpell(NATURES_FURY, Target_RandomUnit, 15, 5, 0);
	}
};

// Ancient Water Spirit
#define CN_ANCIENT_WATER_SPIRIT		33202
#define TIDAL_WAVE					62653 // 62935?

class AncientWaterSpiritAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AncientWaterSpiritAI, MoonScriptBossAI);
	AncientWaterSpiritAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(TIDAL_WAVE, Target_SecondMostHated, 15, 2, 0);
	}
};

// Auriaya
#define CN_AURIAYA					33515
//#define SONIC_SCREECH				64422
#define SENTINEL_BLAST				64389
#define TERRIFYING_SCREECH			64386
//#define GUARDIAN_SWARM				64396

class AuriayaAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AuriayaAI, MoonScriptBossAI);
	AuriayaAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		//AddSpell(SONIC_SCREECH, Target_RandomUnit, 10, 2.5, 4);
		AddSpell(SENTINEL_BLAST, Target_Current, 15, 4, 0);
		AddSpell(TERRIFYING_SCREECH, Target_Self, 20, 2, 8);
		//AddSpell(GUARDIAN_SWARM, Target_RandomUnit, 5, 2, 6);

		//Emotes
		AddEmote(Event_OnCombatStart, "Some things are better left alone!", Text_Yell, 15473); 
		AddEmote(Event_OnTargetDied, "The secret dies with you!", Text_Yell, 15474); 
		AddEmote(Event_OnTargetDied, "There is no escape!", Text_Yell, 15475); 
		AddEmote(Event_OnDied, "", Text_Yell, 15476);
	}
};

// Flame Leviathan
#define CN_FLAME_LEVIATHAN		33113
#define FLAME_VENTS				62396
#define BATTERING_RAM			62376
#define GATHERING_SPEED			62375
#define OVERLOAD				62399
#define SHUTDOWN				62475
#define MISSLE_BARRAGE			62400

class FlameLeviathanAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FlameLeviathanAI, MoonScriptBossAI)
	FlameLeviathanAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(FLAME_VENTS, Target_RandomPlayer, 5.0, 0, 0);
		AddSpell(BATTERING_RAM, Target_Current, 15.0, 0, 0);
		AddSpell(GATHERING_SPEED, Target_Self, 5.0, 0, 0);
		AddSpell(OVERLOAD, Target_RandomPlayer, 6.0f, 0, 25);
		AddSpell(SHUTDOWN, Target_Current, 5.0, 0, 5);
		AddSpell(MISSLE_BARRAGE, Target_Current, 10.0, 0, 0);

		//Emotes
		AddEmote(Event_OnCombatStart, "Hostile entities detected. Threat assessment protocol active. Primary target engaged. Time minus thirty seconds to re-evaluation.!", Text_Yell, 15506);
		AddEmote(Event_OnTargetDied, "Threat assessment routine modified, current target threat level zero. Aquiring new target", Text_Yell, 15521);
		AddEmote(Event_OnDied, "Total systems failure. Defense protocols breached. Leviathan Unit shutting down.", Text_Yell, 15520);

	}
};

//Kologarn
#define CN_KOLOGARN				32930
#define FOCUSED_EYEBEAM			63346
//#define OVERHEAD_SMASH		63356
#define STONE_SHOUT				64004
//#define PETRIFYING_BREATH		62030

class KologarnAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KologarnAI, MoonScriptBossAI)
	KologarnAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(FOCUSED_EYEBEAM, Target_Self, 10, 0, 0);
		AddSpell(STONE_SHOUT, Target_Self, 10, 0, 0);
		//AddSpell(PETRIFYING_BREATH, Target_Current, 1, 0, 0);

		AddEmote(Event_OnCombatStart, "None shall pass!", Text_Yell, 15586);
		AddEmote(Event_OnDied, "Master, they come...", Text_Yell, 15593);
		AddEmote(Event_OnTargetDied, "KOL-THARISH!", Text_Yell, 15587);
		AddEmote(Event_OnTargetDied, "YOU FAIL!", Text_Yell, 15588);

	}
		void OnCombatStart( Unit * pTarget );
		void AIUpdate();
};

void KologarnAI::OnCombatStart( Unit * pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void KologarnAI::AIUpdate()
{
	ParentClass::AIUpdate();

	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
	if(!GetUnit()->GetMapMgr()->GetInterface()->GetPlayerCountInRadius(GetUnit()->GetPositionX(), GetUnit()->GetPositionY(), GetUnit()->GetPositionZ(), 25.0)) //Credits go to Nero for this, if you use this then you need to credit him.
	
{
	GetUnit()->CastSpell(GetUnit(), 62030, true); 
}
};

//Right Arm
#define CN_RIGHT_ARM			32934
#define STONE_GRIP				64290

class RightArmAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RightArmAI, MoonScriptBossAI)
		RightArmAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(STONE_GRIP, Target_Current, 10, 0, 0);

		AddEmote(Event_OnDied, "", Text_Yell, 15589);
		
		}
		void OnCombatStart( Unit * pTarget );
		void AIUpdate();
};

void RightArmAI::OnCombatStart( Unit * pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void RightArmAI::AIUpdate()
{
	ParentClass::AIUpdate();

	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

//Left Arm
#define	CN_LEFT_ARM				32993
#define SHOCKWAVE				63783

class LeftArmAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LeftArmAI, MoonScriptBossAI)
		LeftArmAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SHOCKWAVE, Target_Self, 10, 0, 0);

		AddEmote(Event_OnDied, "", Text_Yell, 15590);

		}
		void OnCombatStart( Unit * pTarget );
		void AIUpdate();
};

void LeftArmAI::OnCombatStart( Unit * pTarget )
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

void LeftArmAI::AIUpdate()
{
	ParentClass::AIUpdate();
	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

//Ignis the Furnace Master
#define CN_IGNIS_THE_FURNACE_MASTER	33118
#define FLAME_JETS					62680
#define SCORCH						62546
#define SLAG_POT					62717

class IgnisTheFurnaceMasterAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IgnisTheFurnaceMasterAI, MoonScriptBossAI)
	IgnisTheFurnaceMasterAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(FLAME_JETS, Target_Self, 10, 2.7f, 0);
		AddSpell(SCORCH, Target_Current, 12, 0, 5);
		AddSpell(SLAG_POT, TargetGen_RandomPlayer, 10, 0, 0);

		AddEmote(Event_OnCombatStart, "Insolent whelps! Your blood will temper the weapons used to reclaim this world!", Text_Yell, 15564);
		AddEmote(Event_OnTargetDied, "More scraps for the scrapheap!", Text_Yell, 15569);
		AddEmote(Event_OnTargetDied, "Your bones will serve as kindling!", Text_Yell, 15570);
		AddEmote(Event_OnDied, "I. Have. Failed.", Text_Yell, 15572);
	}
};

//Yogg-Saron
#define CN_YOGG_SARON			33288
#define LUNATIC_GAZE			64163
#define EMPOWERING_SHADOWS		64468
#define DEAFENING_ROAR			64189

class YoggSaronAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(YoggSaronAI, MoonScriptBossAI)
		YoggSaronAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(LUNATIC_GAZE, Target_Current, 10, 0, 0);
		AddSpell(EMPOWERING_SHADOWS, Target_RandomFriendly, 12, 0, 0);
		AddSpell(DEAFENING_ROAR, Target_Current, 10, 2.3f, 0);
	}
};

//Crusher Tentacles
#define CN_CRUSHER_TENTACLES	33966
#define ERUPT					64144

class CrusherTentaclesAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(CrusherTentaclesAI, MoonScriptBossAI)
		CrusherTentaclesAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(ERUPT, Target_Self, 25, 0, 0);
	}
};

//Steelbreaker
#define CN_STEELBREAKER			32867
#define HIGH_VOLTAGE			61890
#define FUSION_PUNCH			61903
#define STATIC_DISRUPTION		44008
#define OVERWHELMING_POWER		61888
#define MELTDOWN				61889
#define ELECTRICAL_CHARGE		61902

class SteelbreakerAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SteelbreakerAI, MoonScriptBossAI)
		SteelbreakerAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(HIGH_VOLTAGE, Target_Self, 10, 0, 0, 0, 0, false, "So fragile and weak!", Text_Yell, 15675); //Credits go to Ardiente for this fix.
		AddSpell(FUSION_PUNCH, Target_RandomPlayer, 14, 3, 0, 0, 0, false, "You seek the secrets of Ulduar? Then take them!", Text_Yell, 15677);

		//Emotes
		AddEmote(Event_OnCombatStart, "You will not defeat the Assembly of Iron so easily, invaders!", Text_Yell, 15674);
		AddEmote(Event_OnTargetDied, "My death only serves to hasten your demise.", Text_Yell, 15678);
		AddEmote(Event_OnDied, "Impossible!", Text_Yell, 15679);
	}
};

//Runemaster Molgeim
#define CN_RUNEMASTER_MOLGEIM	32927
#define SHIELD_OF_RUNES			62274
#define RUNE_POWER				64320
#define RUNE_OF_DEATH			62269
#define RUNE_OF_SUMMONING		62273
#define LIGHTNING_BLAST			62054

class RunemasterMolgeimAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RunemasterMolgeimAI, MoonScriptBossAI)
		RunemasterMolgeimAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SHIELD_OF_RUNES, Target_Self, 14, 1.5, 0);
		AddSpell(RUNE_POWER, Target_RandomFriendly, 14, 0, 0);

		//Emotes
		AddEmote(Event_OnCombatStart, "Whether the world's greatest gnats or the world's greatest heroes, you're still only mortal!", Text_Yell, 15684);
		AddEmote(Event_OnDied, "You rush headlong into the maw of madness!", Text_Yell, 15690);
	}
};

//Stormcaller Brundir
#define CN_STORMCALLER_BRUNDIR	32857
#define SCB_CHAIN_LIGHTNING		61879
#define BRUNDIR_OVERLOAD		61869
#define LIGHTNING_WHIRL			61915
#define LIGHTNING_TENDRILS		61887

class StormcallerBrundirAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(StormcallerBrundirAI, MoonScriptBossAI)
		StormcallerBrundirAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SCB_CHAIN_LIGHTNING, Target_Current, 14, 2, 0);
		AddSpell(BRUNDIR_OVERLOAD, Target_Self, 14, 6, 7);

		//Emotes
		AddEmote(Event_OnCombatStart, "Nothing short of total decimation will suffice.", Text_Yell, 15657);
		AddEmote(Event_OnTargetDied, "The legacy of storms shall not be undone.", Text_Yell, 15663);
		AddEmote(Event_OnDied, "What have you gained from my defeat? You are no less doomed, mortals!", Text_Yell, 15664);
	}
};

//Razorscale
#define CN_RAZORSCALE			33186
#define FIREBALL				63815
#define FLAME_BUFFET			64016
#define WING_BUFFET				62666
#define FUSE_ARMOR				64771
#define FLAME_BREATH			63317
#define DEVOURING_FLAME			64704

class RazorscaleAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RazorscaleAI, MoonScriptBossAI)
		RazorscaleAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(FIREBALL, Target_SecondMostHated, 5, 0, 0);
		AddSpell(FLAME_BUFFET, Target_Current, 10, 0, 1.5);
		AddSpell(WING_BUFFET, Target_Self, 15, 1.5, 0);
		AddSpell(FUSE_ARMOR, Target_RandomPlayer, 5, 0, 0);
		AddSpell(FLAME_BREATH, Target_Current, 10, 2.5, 2);
		AddSpell(DEVOURING_FLAME, Target_SecondMostHated, 5, 0, 0);
	}
};

//General Vezax
#define CN_GENERAL_VEZAX		33271
#define AURA_OF_DISPARE			62692
#define SHADOW_CRASH			62660
#define SEARING_FLAMES			62661
#define SURGE_OF_DARKNESS		62662 //Self Cast

class GeneralVezaxAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GeneralVezaxAI, MoonScriptBossAI)
		GeneralVezaxAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(SHADOW_CRASH, Target_RandomPlayer, 5, 0, 100);
		AddSpell(SEARING_FLAMES, Target_Current, 5, 0 , 2);
		AddSpell(SURGE_OF_DARKNESS, Target_Self, 5, 0, 0);
	}
};

//////////////////
///TRASH MOBS/////
//////////////////

// Aerial Command Unit
#define CN_AERIAL_COMMAND_UNIT		33670
#define PLASMA_BALL					63689

class AerialCommandUnitAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AerialCommandUnitAI, MoonScriptBossAI);
	AerialCommandUnitAI(Creature * pCreature) : MoonScriptBossAI(pCreature)
	{
		AddSpell(PLASMA_BALL, Target_Current, 15, 2, 30, 0, 0);
	}
};

//Assult Bot
#define CN_ASSULT_BOT			34057
#define EMERGENCY_MODE			64582
#define MAGNETIC_FIELD			64668

class AssultBotAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(AssultBotAI, MoonScriptCreatureAI)
		AssultBotAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
			AddSpell(EMERGENCY_MODE, Target_Self, 15, 0, 0);
			AddSpell(MAGNETIC_FIELD, Target_Current, 15, 0, 0);
	}
};

//Charged Sphere
#define CN_CHARGED_SPHERE		33715
#define SUPERCHARGED			63528

class ChargedSphereAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ChargedSphereAI, MoonScriptCreatureAI)
		ChargedSphereAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SUPERCHARGED, Target_Current, 10, 0, 0);
	}
};

//Dark Rune Acolyte
#define CN_DARK_RUNE_ACOLYTE	32886
//#define CIRCLE_OF_HEALING		61964
#define GREATER_HEAL			54337 //62334 Should be this but she casts it on players.
#define HOLY_SMITE				62335
//#define RENEW					61967

class DarkRuneAcolyteAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneAcolyteAI, MoonScriptCreatureAI)
		DarkRuneAcolyteAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//AddSpell(CIRCLE_OF_HEALING, Target_Self, 15, 2.5f, 0, 0, 0);
		AddSpell(GREATER_HEAL, Target_WoundedUnit, 20, 0, 0);
		AddSpell(HOLY_SMITE, Target_Current, 20, 1.5f, 0);
	}
};

//Dark Rune Champion
#define CN_DARK_RUNE_CHAMPION	32876
#define CHARGE					32323
#define MORTAL_STRIKE			35054
#define WHIRLWIND				15578 // 33500?

class DarkRuneChampionAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneChampionAI, MoonScriptCreatureAI)
		DarkRuneChampionAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CHARGE, Target_Current, 15, 0, 0, 8, 25);
		AddSpell(MORTAL_STRIKE, Target_Current, 15, 0, 0);
		AddSpell(WHIRLWIND, Target_Self, 10, 0, 0);
	}
};

//Dark Rune Commoner
#define CN_DARK_RUNE_COMMONER	32904
#define LOW_BLOW				62326
#define	PUMMEL					38313

class DarkRuneCommonerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneCommonerAI, MoonScriptCreatureAI)
		DarkRuneCommonerAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(LOW_BLOW, Target_Current, 15, 0, 0);
		AddSpell(PUMMEL, Target_Current, 15, 0, 0);
	}
};

// Dark Rune Evoker
#define CN_DARK_RUNE_EVOKER		32878
#define RUNIC_LIGHTNING			62445 // 62327?
#define RUNIC_MENDING			62446 // 62328?
#define RUNIC_SHIELD			62529 // 62321?

class DarkRuneEvokerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneEvokerAI, MoonScriptCreatureAI)
		DarkRuneEvokerAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(RUNIC_LIGHTNING, Target_Current, 10, 1.5f, 0);
		AddSpell(RUNIC_MENDING, Target_WoundedFriendly, 5, 0, 2);
		AddSpell(RUNIC_SHIELD, Target_Self, 3, 0.5, 6);
	}
};

//Dark Rune Warbringer
#define CN_DARK_RUNE_WARBRINGER	32877
//#define AURA_OF_CELERITY		62320
#define RUNIC_STRIKE			62322

class DarkRuneWarbringerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneWarbringerAI, MoonScriptCreatureAI)
		DarkRuneWarbringerAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//AddSpell(AURA_OF_CELERITY, Target_Self, 15, 0, 0, 0, 0);
		AddSpell(RUNIC_STRIKE, Target_Current, 15, 0, 0);
	}
};

//Dark Rune Watcher
#define CN_DARK_RUNE_WATCHER	33453
#define CHAIN_LIGHTNING			64758 // 64759?
#define LIGHTNING_BOLT			63809 // 64696?

class DarkRuneWatcherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneWatcherAI, MoonScriptCreatureAI)
		DarkRuneWatcherAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(CHAIN_LIGHTNING, Target_Self, 10, 1.5f, 0);
		AddSpell(LIGHTNING_BOLT, Target_Current, 15, 2, 0);
	}
};

//Dark Rune Sentinel
#define CN_DARK_RUNE_SENTINEL	33846
#define BATTLESHOUT				64062
#define HEROIC_STRIKE			45026
#define DRS_WHIRLWIND			63807

class DarkRuneSentinel : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DarkRuneSentinel, MoonScriptCreatureAI)
		DarkRuneSentinel(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HEROIC_STRIKE, Target_Current, 10, 0, 0);
		AddSpell(DRS_WHIRLWIND, Target_Self, 10, 0, 0);
	}
};


//Detonating Lasher
#define CN_DETONATING_LASHER	32918
#define DETONATE				62598 // 62937?
//#define FLAME_LASH			62608

class DetonatingLasherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(DetonatingLasherAI, MoonScriptCreatureAI)
		DetonatingLasherAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DETONATE, Target_Self, 10, 0, 0);
	}
};

//Eivi Nightfeather
#define CN_EIVI_NIGHTFEATHER	33325
#define WRATH					62793

class EiviNightfeatherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EiviNightfeatherAI, MoonScriptCreatureAI)
		EiviNightfeatherAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WRATH, Target_SecondMostHated, 17, 1.5f, 0);
	}
};

//Elementalist Avuun
#define CN_ELEMENTALIST_AVUUN	32900
#define LAVA_BURST				61924
//#define STORM_CLOUD			65123 // 65133?

class ElementalistAvuunAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ElementalistAvuunAI, MoonScriptCreatureAI)
		ElementalistAvuunAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(LAVA_BURST, Target_SecondMostHated, 25, 2.4f, 0);
		//AddSpell(STORM_CLOUD, Target_Self, 15, 2, 0, 0, 40);
	}
};

//Elementalist Mahfuun
#define CN_ELEMENTALIST_MAHFUNN	33328
#define LAVA_BURST				61924
//#define MAHFUNN_STORM_CLOUD	65133 // 65123?

class ElementalistMahfunnAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ElementalistMahfunnAI, MoonScriptCreatureAI)
		ElementalistMahfunnAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(LAVA_BURST, Target_SecondMostHated, 25, 2, 0);
		//AddSpell(MAHFUNN_STORM_CLOUD, Target_Self, 15, 2, 0);
	}
};

//Ellie Nightfeather
#define CN_ELLIE_NIGHTFEATHER	32901
#define WRATH					62793

class EllieNightfeatherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EllieNightfeatherAI, MoonScriptCreatureAI)
		EllieNightfeatherAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WRATH, Target_SecondMostHated, 17, 1.5f, 0);
	}
};

//Enslaved Fire Elemental
#define CN_ENSLAVED_FIRE_ELEMENTAL 33838
#define BLAST_WAVE				38064
#define FIRE_SHIELD				63778

class EnslavedFireElementalAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(EnslavedFireElementalAI, MoonScriptCreatureAI)
		EnslavedFireElementalAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BLAST_WAVE, Target_Self, 15, 0, 0);
		AddSpell(FIRE_SHIELD, Target_ClosestFriendly, 10, 0, 0);
	}
};

//Faceless Horror
#define CN_FACELESS_HORROR		33772
#define DEATH_GRIP				64429
#define FH_SHADOW_CRASH			63722
#define VOID_BARRIER			63710
//#define VOID_WAVE				63703

class FacelessHorrorAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FacelessHorrorAI, MoonScriptCreatureAI)
		FacelessHorrorAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DEATH_GRIP, Target_SecondMostHated, 25, 0, 0);
		AddSpell(SHADOW_CRASH, Target_RandomPlayer, 10, 1, 0);
		AddSpell(VOID_BARRIER, Target_Self, 10, 0, 0);
	}
};

//Feral Defender
#define CN_FERAL_DEFENDER		34035
#define FERAL_ESSENCE			64455
#define FERAL_POUNCE			64478 // 64669?

class FeralDefenderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FeralDefenderAI, MoonScriptCreatureAI)
		FeralDefenderAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FERAL_ESSENCE, Target_Self, 5, 0, 0);
		AddSpell(FERAL_POUNCE, Target_SecondMostHated, 20, 0, 5, 0);
	}
};

//Field Medic Jessi
#define CN_FIELD_MEDIC_JESSI	33326
#define DISPEL_MAGIC			63499
#define JESSI_GREATER_HEAL		54337 //62809 Should be this but she casts it on players.
#define SMITE					61923

class FieldMedicJessiAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FieldMedicJessiAI, MoonScriptCreatureAI)
		FieldMedicJessiAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DISPEL_MAGIC, Target_WoundedFriendly, 7, 0, 0);
		AddSpell(JESSI_GREATER_HEAL, Target_WoundedFriendly, 7, 0, 0);
		AddSpell(SMITE, Target_Current, 25, 2, 0);
	}
};

//Field Medic Penny
#define CN_FIELD_MEDIC_PENNY	32897
#define DISPEL_MAGIC			63499
#define PENNY_GREATER_HEAL		54337 //62809 Should be this but she casts it on players.
#define SMITE					61923

class FieldMedicPennyAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(FieldMedicPennyAI, MoonScriptCreatureAI)
		FieldMedicPennyAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DISPEL_MAGIC, Target_WoundedFriendly, 15, 0, 0);
		AddSpell(PENNY_GREATER_HEAL, Target_WoundedFriendly, 20, 0, 0);
		AddSpell(SMITE, Target_Current, 25, 2, 0);
	}
};

//Forest Swarmer
#define CN_FOREST_SWARMER		33431
#define POLINATE				63059 //Pollinates a Guardian Lasher healing it for 15% and increasing damage dealt by 25% for 30 sec.

class ForestSwarmerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ForestSwarmerAI, MoonScriptCreatureAI)
		ForestSwarmerAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(POLINATE, Target_Predefined, 25, 2, 0);		
	}
};

//Forge Construct
#define CN_FORGE_CONSTRUCT		34085
#define FORGE_CONSTRUCT_CHARGE	64719
#define FLAME_EMISSION			64720 // 64721?

class ForgeConstructAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ForgeConstructAI, MoonScriptCreatureAI)
		ForgeConstructAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FORGE_CONSTRUCT_CHARGE, Target_SecondMostHated, 15, 0, 0);
		AddSpell(FLAME_EMISSION, Target_Current, 15, 0, 0);
	}
};

//Guardian Lasher
#define CN_GUARDIAN_LASHER		33430
#define GUARDIAN_PHEROMONES		63007
#define GUARDIANS_LASH			63407

class GuardianLasherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GuardianLasherAI, MoonScriptCreatureAI)
		GuardianLasherAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(GUARDIAN_PHEROMONES, Target_Self, 20, 0, 0);
		AddSpell(GUARDIANS_LASH, Target_Current, 15, 0, 0);
	}
};

//Guardian of Life
#define CN_GUARDIAN_OF_LIFE		33528
#define POISON_BREATH			63226 // 63551?

class GuardianOfLifeAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(GuardianOfLifeAI, MoonScriptCreatureAI)
		GuardianOfLifeAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(POISON_BREATH, Target_RandomUnit, 20, 1, 0);
	}
};

//Hardened Iron Golem
#define CN_HARDENED_IRON_GOLEM	34190
#define HARDEN_FISTS			64877
#define RUNE_PUNCH				64874

class HardenedIronGolemAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(HardenedIronGolemAI, MoonScriptCreatureAI)
		HardenedIronGolemAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HARDEN_FISTS, Target_Self, 5, 2, 0);
		AddSpell(RUNE_PUNCH, Target_RandomUnit, 25, 0, 0);
	}
};

//Immortal Guardian
#define CN_IMMORTAL_GUARDIAN	33988
#define DRAIN_LIFE				64159 // 64160?

class ImmortalGuardianAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ImmortalGuardianAI, MoonScriptCreatureAI)
		ImmortalGuardianAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(DRAIN_LIFE, Target_SecondMostHated, 15, 0, 0);
	}
};

//Iron Honor Guard
#define CN_IRON_HONOR_GUARD		32875
#define HONOR_GUARD_CLEAVE		42724
#define HAMSTRING				48639
#define SHIELD_SMASH			62332 // 62420?

class IronHonorGuardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IronHonorGuardAI, MoonScriptCreatureAI)
		IronHonorGuardAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(HONOR_GUARD_CLEAVE, Target_Current, 10, 0, 0);
		AddSpell(HAMSTRING, Target_ClosestPlayer, 10, 0, 20);
		AddSpell(SHIELD_SMASH, Target_ClosestPlayer, 10, 0, 20);
	}
};

//Iron Ring Guard
#define CN_IRON_RING_GUARD		32874
#define IMPALE					62331
#define WHIRLING_TRIP			64151

class IronRingGuardAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IronRingGuardAI, MoonScriptCreatureAI)
		IronRingGuardAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(IMPALE, Target_Current, 15, 0, 0);
		AddSpell(WHIRLING_TRIP, Target_Current, 15, 0, 0);
	}
};

//Ironroot Lasher
#define CN_IRONROOT_LASHER		33526
#define IRONROOT_THORNS			63240

class IronRootLasherAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(IronRootLasherAI, MoonScriptCreatureAI)
		IronRootLasherAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(IRONROOT_THORNS, Target_RandomFriendly, 20, 0, 0);
	}
};

//Jormungar Behemoth
#define JORMUNGAR_BEHEMOTH		32882
#define ACID_BREATH				62315
#define SWEEP					62316

class JormungarBehemothAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(JormungarBehemothAI, MoonScriptCreatureAI)
		JormungarBehemothAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ACID_BREATH, Target_RandomPlayer, 10, 0, 0);
		AddSpell(SWEEP, Target_Current, 20, 0, 0);
	}
};

//Junk Bot
#define CN_JUNK_BOT				33855
#define EMERGENCY_MODE			64582

class JunkBotAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(JunkBotAI, MoonScriptCreatureAI)
		JunkBotAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(EMERGENCY_MODE, Target_Self, 5, 0, 0);
	}
};

//Kar Greycloud
#define CN_KAR_GREYCLOUD		33333
#define WRATH					62793

class KarGreyCloudAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(KarGreyCloudAI, MoonScriptCreatureAI)
		KarGreyCloudAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(WRATH, Target_Current, 20, 1.5, 0);
	}
};

//Leviathan Defense Turret
#define CN_LEVIATHAN_DEFENSE_TURRET	33142
#define SEARING_FLAME			62402

class LeviathanDefenseTurret : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LeviathanDefenseTurret, MoonScriptCreatureAI)
	LeviathanDefenseTurret(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
	AddSpell(SEARING_FLAME, Target_Current, 50, 0.5, 0, 0, 15);
	}
		void OnCombatStart( Unit * pTarget );
		void AIUpdate();
};

void LeviathanDefenseTurret::OnCombatStart( Unit * pTarget ) //Not sure if this is right.
{
	ParentClass::OnCombatStart( pTarget );
	SetBehavior( Behavior_Spell );
	SetCanMove( false ); 
	StopMovement();
};

void LeviathanDefenseTurret::AIUpdate()
{
	ParentClass::AIUpdate();

	SetBehavior( Behavior_Spell );
	SetCanMove( false );
	StopMovement();
};

//Leviathan Mk II
#define CN_LEVIATHAN_MK_II			33432
#define PROXIMITY_MINE				63016 //No Idea. http://www.wowhead.com/?npc=33432 Summons 8-10 http://www.wowhead.com/?npc=34362 
#define NAPALM_SHELL				63666
#define PLASMA_BLAST				62997
#define SHOCK_BLAST					63631

class LeviathanMkIIAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LeviathanMkIIAI, MoonScriptCreatureAI)
	LeviathanMkIIAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(NAPALM_SHELL, Target_RandomPlayer, 8, 0, 0);
		AddSpell(PLASMA_BLAST, Target_Current, 10, 3, 7);
		AddSpell(SHOCK_BLAST, Target_Self, 5, 4, 0);
		AddSpell(PROXIMITY_MINE, Target_Destination, 15, 0, 0); 
	}
};

//Proximity Mine
#define CN_PROXIMITY_MINE			34362
#define EXPLOSION					66351

class ProximityMineAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(ProximityMineAI, MoonScriptCreatureAI)
	ProximityMineAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(EXPLOSION, Target_Self, 100, 0, 0);
	}
		void OnCombatStart(Unit * pTarget);
		void AIUpdate();
};

void ProximityMineAI::OnCombatStart(Unit * pTarget) //Not sure if this is right. Can Mines move?
{
	ParentClass::OnCombatStart(pTarget);
	SetBehavior(Behavior_Spell);
	SetCanMove(false); 
	StopMovement();
};

void ProximityMineAI::AIUpdate()
{
	ParentClass::AIUpdate();

	SetBehavior(Behavior_Spell);
	SetCanMove(false);
	StopMovement();
};


//Life Spark
#define CN_LIFE_SPARK				34004
#define SHOCK						64320
#define STATIC_CHARGED				64227

class LifeSparkAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(LifeSparkAI, MoonScriptCreatureAI)
		LifeSparkAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SHOCK, Target_Current, 15, 0, 0);
		AddSpell(STATIC_CHARGED, Target_Self, 10, 0 ,0);
	}
};

//Runeforged Sentry
#define CN_RUNEFORGED_SENTRY		34234
#define FLAMING_RUNE				64852
#define RFS_LAVA_BURST				64870
#define RUNED_FLAME_JETS			64847

class RuneforgedSentryAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(RuneforgedSentryAI, MoonScriptCreatureAI)
		RuneforgedSentryAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FLAMING_RUNE, Target_Current, 10, 0, 5);
		AddSpell(RFS_LAVA_BURST, Target_SecondMostHated, 10, 0, 5);
		AddSpell(RUNED_FLAME_JETS, Target_Current, 10, 0, 5);
	}
};

//Steelforged Defender
#define CN_STEELFORGED_DEFENDER		33236
#define SFD_HAMSTRING				62845
#define SUNDER_ARMOR				50370

class SteelforgedDefenderAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SteelforgedDefenderAI, MoonScriptCreatureAI)
		SteelforgedDefenderAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SFD_HAMSTRING, Target_Current, 7, 0, 0);
		AddSpell(SUNDER_ARMOR, Target_Current, 7, 0, 0);
	}
};

//Mechagnome Battletank
#define CN_MECHAGNOME_BATTLETANK	34164
#define FLAME_CANNON				64692
#define JUMP_ATTACK					64953

class MechagnomeBattletankAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MechagnomeBattletankAI, MoonScriptCreatureAI)
		MechagnomeBattletankAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FLAME_CANNON, Target_RandomPlayer, 15, 1, 0);
		AddSpell(JUMP_ATTACK, Target_SecondMostHated, 20, 0, 0);
	}
};

//Ulduar Colossus
#define CN_ULDUAR_COLOSSUS			33237
#define GROUND_SLAM					62625

class UlduarColossusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(UlduarColossusAI, MoonScriptCreatureAI)
		UlduarColossusAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(GROUND_SLAM, Target_Current, 20, 4, 0, 0, 30);
	}
};

//Molten Colossus
#define	CN_MOLTEN_COLOSSUS			34069
#define EARTHQUAKE					64697
#define PYROBLAST					64698

class MoltenColossusAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MoltenColossusAI, MoonScriptCreatureAI)
		MoltenColossusAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(EARTHQUAKE, Target_Current, 10, 0, 0);
		AddSpell(PYROBLAST, Target_RandomPlayer, 15, 0, 0);
	}
};

//Magma Rager
#define CN_MAGMA_RAGER				34086
#define FIRE_BLAST					64773
#define SUPERHEATED_WINDS			64746

class MagmaRagerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(MagmaRagerAI, MoonScriptCreatureAI)
		MagmaRagerAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(FIRE_BLAST, Target_Current, 15, 0, 0);
		AddSpell(SUPERHEATED_WINDS, Target_RandomPlayer, 10, 1.25, 0);
	}
};

//XR-949 Salvagebot
#define CN_XR949_SALVAGEBOT			34269
#define DEPLOY_SALVAGE_SAWS			65009

class XR949SalvagebotAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(XR949SalvagebotAI, MoonScriptCreatureAI)
		XR949SalvagebotAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		//AddSpell(DEPLOY_SALVAGE_SAWS, Target_Self, 10, 0, 0);
	}
};

//XB-488 Disposalbot
#define CN_XB488_DISPOSALBOT		34273
#define CUT_SCRAP_METAL				65080

class XB488DisposalBotAI : public MoonScriptCreatureAI
{
		MOONSCRIPT_FACTORY_FUNCTION(XB488DisposalBotAI, MoonScriptCreatureAI)
			XB488DisposalBotAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
		{
			AddSpell(CUT_SCRAP_METAL, Target_RandomPlayer, 0, 0, 0);
		}
};

//XM-024 Pummeller
#define CN_XM024_PUMMELLER			33344
#define ARCING_SMASH				39144 //8374??
#define TRAMPLE						5568
#define UPPERCUT					10966

class XM024PummellerAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(XM024PummellerAI, MoonScriptCreatureAI)
		XM024PummellerAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(ARCING_SMASH, Target_Current, 8, 0, 15, 0, 10);
		AddSpell(TRAMPLE, Target_Self, 8, 0, 0);
		AddSpell(UPPERCUT, Target_Current, 8, 0, 0);
	}
};

//XE-321 Boombot
#define CN_XE321_BOOMBOT			33346
#define BOOM						62384

class XE321BoomBotAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(XE321BoomBotAI, MoonScriptCreatureAI)
		XE321BoomBotAI(Creature * pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(BOOM, Target_Self, 5, 0, 0);
		__mExplode = AddSpell(BOOM, Target_Self, 0, 0, 0);
	};

	void OnCombatStart(Unit * pTarget)
	{
		__mExplode->mEnabled = true;
		ParentClass::OnCombatStart(pTarget);
	}

	void AIUpdate()
	{
		if( __mExplode->mEnabled && GetHealthPercent() <= 5 && !IsCasting() )
		{
			CastSpellNowNoScheduling(__mExplode);
			__mExplode->mEnabled = false;
		}
		ParentClass::AIUpdate();
	}

	SpellDesc* __mExplode;
};

void SetupUlduar(ScriptMgr* mgr)
{
	mgr->register_instance_script( MAP_ULDUAR, &UlduarScript::Create );
	GossipScript * UlduarTele = (GossipScript*) new UlduarTeleporter();
	mgr->register_go_gossip_script(194569, UlduarTele);
	
	///////////
	//Bosses //
	///////////
	mgr->register_creature_script(CN_AERIAL_COMMAND_UNIT, &AerialCommandUnitAI::Create);
	mgr->register_creature_script(CN_ALGALON_THE_OBSERVER, &AlgalonTheObserverAI::Create);
	mgr->register_creature_script(CN_ANCIENT_CONSERVATOR, &AncientConservatorAI::Create);
	mgr->register_creature_script(CN_ANCIENT_WATER_SPIRIT, &AncientWaterSpiritAI::Create);
	mgr->register_creature_script(CN_AURIAYA, &AuriayaAI::Create);
	mgr->register_creature_script(CN_FLAME_LEVIATHAN, &FlameLeviathanAI::Create);
	mgr->register_creature_script(CN_KOLOGARN, &KologarnAI::Create);
	mgr->register_creature_script(CN_RIGHT_ARM, &RightArmAI::Create);
	mgr->register_creature_script(CN_LEFT_ARM, &LeftArmAI::Create);
	mgr->register_creature_script(CN_IGNIS_THE_FURNACE_MASTER, &IgnisTheFurnaceMasterAI::Create);
	mgr->register_creature_script(CN_CRUSHER_TENTACLES, &CrusherTentaclesAI::Create);
	mgr->register_creature_script(CN_STEELBREAKER, &SteelbreakerAI::Create);
	mgr->register_creature_script(CN_RUNEMASTER_MOLGEIM, &RunemasterMolgeimAI::Create);
	mgr->register_creature_script(CN_STORMCALLER_BRUNDIR, &StormcallerBrundirAI::Create);
	mgr->register_creature_script(CN_GENERAL_VEZAX, & GeneralVezaxAI::Create);
	mgr->register_creature_script(CN_RAZORSCALE, &RazorscaleAI::Create);
	mgr->register_creature_script(CN_YOGG_SARON, &YoggSaronAI::Create);
	
	//////////////
	//Trash Mobs//
	//////////////
	mgr->register_creature_script(CN_ASSULT_BOT, &AssultBotAI::Create);
	mgr->register_creature_script(CN_CHARGED_SPHERE, &ChargedSphereAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_ACOLYTE, &DarkRuneAcolyteAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_CHAMPION, &DarkRuneChampionAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_COMMONER, &DarkRuneCommonerAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_EVOKER, &DarkRuneEvokerAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_WARBRINGER, &DarkRuneWarbringerAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_WATCHER, &DarkRuneWatcherAI::Create);
	mgr->register_creature_script(CN_DARK_RUNE_SENTINEL, &DarkRuneWatcherAI::Create);
	mgr->register_creature_script(CN_DETONATING_LASHER, &DetonatingLasherAI::Create);
	mgr->register_creature_script(CN_EIVI_NIGHTFEATHER, &EiviNightfeatherAI::Create);
	mgr->register_creature_script(CN_ELEMENTALIST_AVUUN, &ElementalistAvuunAI::Create);
	mgr->register_creature_script(CN_ELEMENTALIST_MAHFUNN, &ElementalistMahfunnAI::Create);
	mgr->register_creature_script(CN_ELLIE_NIGHTFEATHER, &EllieNightfeatherAI::Create);
	mgr->register_creature_script(CN_ENSLAVED_FIRE_ELEMENTAL, &EnslavedFireElementalAI::Create);
	mgr->register_creature_script(CN_FACELESS_HORROR, &FacelessHorrorAI::Create);
	mgr->register_creature_script(CN_FERAL_DEFENDER, &FeralDefenderAI::Create);
	mgr->register_creature_script(CN_FIELD_MEDIC_JESSI, &FieldMedicJessiAI::Create);
	mgr->register_creature_script(CN_FIELD_MEDIC_PENNY, &FieldMedicPennyAI::Create);
	mgr->register_creature_script(CN_FOREST_SWARMER, &ForestSwarmerAI::Create);
	mgr->register_creature_script(CN_FORGE_CONSTRUCT, &ForgeConstructAI::Create);
	mgr->register_creature_script(CN_GUARDIAN_LASHER, &GuardianLasherAI::Create);
	mgr->register_creature_script(CN_GUARDIAN_OF_LIFE, &GuardianOfLifeAI::Create);
	mgr->register_creature_script(CN_HARDENED_IRON_GOLEM, &HardenedIronGolemAI::Create);
	mgr->register_creature_script(CN_IMMORTAL_GUARDIAN, &ImmortalGuardianAI::Create);
	mgr->register_creature_script(CN_IRON_HONOR_GUARD, &IronHonorGuardAI::Create);
	mgr->register_creature_script(CN_IRON_RING_GUARD, &IronRingGuardAI::Create);
	mgr->register_creature_script(CN_IRONROOT_LASHER, &IronRootLasherAI::Create);
	mgr->register_creature_script(CN_JUNK_BOT, &JunkBotAI::Create);
	mgr->register_creature_script(CN_KAR_GREYCLOUD, &KarGreyCloudAI::Create);
	mgr->register_creature_script(CN_LEVIATHAN_DEFENSE_TURRET, &LeviathanDefenseTurret::Create);
	mgr->register_creature_script(CN_LEVIATHAN_MK_II, &LeviathanMkIIAI::Create);
	//mgr->register_creature_script(CN_PROXIMITY_MINE, &ProximityMineAI::Create);
	mgr->register_creature_script(CN_LIFE_SPARK, &LifeSparkAI::Create);
	mgr->register_creature_script(CN_RUNEFORGED_SENTRY, &RuneforgedSentryAI::Create);
	mgr->register_creature_script(CN_STEELFORGED_DEFENDER, &SteelforgedDefenderAI::Create);
	mgr->register_creature_script(CN_MECHAGNOME_BATTLETANK, &MechagnomeBattletankAI::Create);
	mgr->register_creature_script(CN_ULDUAR_COLOSSUS, &UlduarColossusAI::Create);
	mgr->register_creature_script(CN_MOLTEN_COLOSSUS, &MoltenColossusAI::Create);
	mgr->register_creature_script(CN_MAGMA_RAGER, &MagmaRagerAI::Create);
	mgr->register_creature_script(CN_XR949_SALVAGEBOT, &XR949SalvagebotAI::Create);
	mgr->register_creature_script(CN_XB488_DISPOSALBOT, &XR949SalvagebotAI::Create);
	mgr->register_creature_script(CN_XM024_PUMMELLER, &XM024PummellerAI::Create);
	mgr->register_creature_script(CN_XE321_BOOMBOT, &XE321BoomBotAI::Create);
	};