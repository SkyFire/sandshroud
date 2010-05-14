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

//***************
//***************
//******Trash****
//***************
//***************

//Brightscale Worm
#define BRIGHTSCALE_WYRM 24761
#define ENERGY_INFUSION 44406
class BrightscaleWyrmAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(BrightscaleWyrmAI, MoonScriptCreatureAI);
    BrightscaleWyrmAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {

    }
    
    void OnDied(UnitPointer pKiller)
    {
        AuraPointer pAura = AuraPointer(new Aura(dbcSpell.LookupEntry(ENERGY_INFUSION), 20000, pKiller, pKiller));
        if (pAura != NULL)    
            pKiller->AddAura(pAura,NULLAURA);
    }
};


//Coilskar Witch
#define COILSKAR_WITCH 24696
#define FORKED_LIGHTNING 46150
#define FROST_ARROW 44639
#define MANA_SHIELD 46151
#define SHOOT 35946
class CoilskarWitchAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(CoilskarWitchAI, MoonScriptCreatureAI);
    CoilskarWitchAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(FORKED_LIGHTNING, Target_RandomDestination, 35, 2, 7, 0, 30);
        AddSpell(FROST_ARROW, Target_Current, 30, 0, 10, 0, 50);
        AddSpell(MANA_SHIELD, Target_Self, 20, 0, 20);
        AddSpell(SHOOT, Target_RandomPlayer, 40, 0, 5, 5, 30);
    }
};

//Ethereum Smuggler
#define ETHEREUM_SMUGGLER 24698
#define ARCANE_EXPLOSION 44538
class EthereumSmugglerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EthereumSmugglerAI, MoonScriptCreatureAI);
    EthereumSmugglerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(ARCANE_EXPLOSION, Target_Self, 35, 1, 10);
    }
};

//Fizzle
#define FIZZLE 24656
#define FIREBOLT 44164
class FizzleAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(FizzleAI, MoonScriptCreatureAI);
    FizzleAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(FIREBOLT, Target_RandomPlayer, 35, 1, 1, 0, 30);
        _unit->GetAIInterface()->disable_melee = true;
    }
};

//Sister Of Torment
#define SISTER_OF_TORMENT 24697
#define DEADLY_EMBRACE 44547
#define LASH_OF_PAIN 44640
class TormentSisterAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(TormentSisterAI, MoonScriptCreatureAI);
    TormentSisterAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(DEADLY_EMBRACE, Target_RandomPlayer, 25, 1.5, 10, 0, 20);
        AddSpell(LASH_OF_PAIN, Target_RandomPlayer, 35, 0, 6, 0, 3);
    }
};

//Sunblade Blood Knight
#define SUNBLADE_BLOOD_KNIGHT 24684
#define HOLY_LIGHT 46029
#define SEAL_OF_WRATH 46030
#define JUDGEMENT_OF_WRATH 46033
class SunbladeBloodKnightAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeBloodKnightAI, MoonScriptCreatureAI);
    SunbladeBloodKnightAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(HOLY_LIGHT, Target_WoundedFriendly, 25, 2, 10, 0, 40);
        AddSpell(SEAL_OF_WRATH, Target_Self, 35, 0, 5);
        AddSpell(JUDGEMENT_OF_WRATH, Target_RandomPlayer, 30, 0, 8);
    }
};

//Sunblade Imp
#define SUNBLADE_IMP 24815
#define IMP_FIREBOLT 46044
class SunbladeImpAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeImpAI, MoonScriptCreatureAI);
    SunbladeImpAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(IMP_FIREBOLT, Target_RandomPlayer, 30, 1, 1, 0, 30);
    }
};

//Sunblade Keeper
#define SUNBLADE_KEEPER 24762
#define KEEPER_BANISH 44765
#define KEEPER_SHADOW_BOLT 15232
#define KEEPER_SHADOW_BOLT_VOLLEY 20741
class SunbladeKeeperAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeKeeperAI, MoonScriptCreatureAI);
    SunbladeKeeperAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(KEEPER_BANISH, Target_RandomPlayer, 20, 2, 9, 0, 30);
        AddSpell(KEEPER_SHADOW_BOLT, Target_RandomPlayer, 35, 3, 4, 0, 40);
        AddSpell(KEEPER_SHADOW_BOLT_VOLLEY, Target_Self, 30, 0, 10);
    }
};

//Sunblade Mage Guard
#define SUNBLADE_MAGE_GUARD 24683
#define GLAIVE_THROW 46028
#define MAGIC_DAMP_FIELD 44475
class SunbladeMageGuardAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeMageGuardAI, MoonScriptCreatureAI);
    SunbladeMageGuardAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(GLAIVE_THROW, Target_RandomPlayer, 20, 0, 6, 0, 60);
        AddSpell(MAGIC_DAMP_FIELD, Target_RandomDestination, 35, 1, 15, 0, 60);
    }
};

//Sunblade Magister
#define SUNBLADE_MAGISTER 24685
#define ARCANE_NOVA 46036
#define FROSTBOLT 46035
#define SPELLHASTE_BUFF 44604
class SunbladeMagisterAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeMagisterAI, MoonScriptCreatureAI);
    SunbladeMagisterAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(FROSTBOLT, Target_RandomPlayer, 35, 2, 4, 0, 40);
        AddSpell(ARCANE_NOVA, Target_Self, 30, 1.5, 10);
        AddSpell(SPELLHASTE_BUFF, Target_Self, 25, 0, 30);
    }
};

//Sunblade Physician
#define SUNBLADE_PHYSICIAN 24687
#define INJECT_POISON 46046
#define PRAYER_OF_MENDING 46045
class SunbladePhysicianAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladePhysicianAI, MoonScriptCreatureAI);
    SunbladePhysicianAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(INJECT_POISON, Target_Self, 35, 1, 8);
        AddSpell(PRAYER_OF_MENDING, Target_RandomFriendly, 25, 0, 10);
    }
};

//Sunblade Warlock
#define SUNBLADE_WARLOCK 24686
#define FEL_ARMOR 44520
#define IMMOLATE 46042
#define INCINERATE 46043
#define SUMMON_SUNBLADE_IMP 48320
class SunbladeWarlockAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(SunbladeWarlockAI, MoonScriptCreatureAI);
    SunbladeWarlockAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(FEL_ARMOR, Target_Self, 10, 0, 30);
        AddSpell(IMMOLATE, Target_RandomPlayer, 35, 2, 5);
        AddSpell(INCINERATE, Target_RandomPlayer, 30, 2, 5);
        AddSpell(SUMMON_SUNBLADE_IMP, Target_Self, 25, 0, 15);
    }
};

//Wretched Bruiser
#define WRETCHED_BRUISER 24689
#define WRETCHED_STRIKE 44534
#define DRINK_FEL_INFUSION 44505
class WretchedBruiserAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(WretchedBruiserAI, MoonScriptCreatureAI);
    WretchedBruiserAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(DRINK_FEL_INFUSION, Target_Self, 2, 0, 30);
        AddSpell(WRETCHED_STRIKE, Target_Current, 35, 0, 5);
    }
};

//Wretched HUSK
#define WRETCHED_HUSK 24690
#define WRETCHED_FROSTBOLT 44504
#define WRETCHED_FIREBALL 44503
class WretchedHuskAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(WretchedHuskAI, MoonScriptCreatureAI);
    WretchedHuskAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(DRINK_FEL_INFUSION, Target_Self, 2, 0, 30);
        AddSpell(WRETCHED_FROSTBOLT, Target_RandomPlayer, 35, 3, 5);
        AddSpell(WRETCHED_FIREBALL, Target_RandomPlayer, 35, 3, 5);
    }
};

//Wretched Skulker
#define WRETCHED_SKULKER 24688
#define WRETCHED_STAB 44533
class WretchedSkulkerAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(WretchedSkulkerAI, MoonScriptCreatureAI);
    WretchedSkulkerAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(DRINK_FEL_INFUSION, Target_Self, 2, 0, 30);
        AddSpell(WRETCHED_STAB, Target_Current, 35, 0, 5);
    }
};

//*************
//*************
//***BOSSES****
//*************
//*************

//Selin Fireheart Event
#define SELIN_FIREHEART 24723

//Fel Crystal
#define FEL_CRYSTAL 24722
class FelCrystalAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(FelCrystalAI);
    FelCrystalAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->GetAIInterface()->disable_melee = true;
        _unit->GetAIInterface()->m_canMove = false;
        _unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void OnDied(UnitPointer mKiller)
    {
        CreaturePointer selinfh = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), SELIN_FIREHEART);
        if(selinfh)
        {
            if(selinfh->GetCurrentSpell())
            {
                selinfh->GetCurrentSpell()->cancel();
            }
        }
    }
};


//Selin Fireheart
#define FEL_EXPLOSION 44314
#define DRAIN_LIFE 46155
#define CRYS_DRAIN 46291
#define MANA_RAGE 44320
#define DRAIN_MANA 46153
class SelinFireheartAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(SelinFireheartAI);
    SP_AI_Spell spells[2];
    bool m_spellcheck[2];

    SelinFireheartAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        nrspells = 2;
        for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;

        }

        spells[0].info = dbcSpell.LookupEntry(DRAIN_LIFE);
        spells[0].targettype = TARGET_RANDOM_SINGLE;
        spells[0].instant = false;
        spells[0].perctrigger = 20.0f;
        spells[0].cooldown = 5;
        spells[0].mindist2cast = 0.0f;
        spells[0].maxdist2cast = 20.0f;

        spells[1].info = dbcSpell.LookupEntry(DRAIN_MANA);
        spells[1].targettype = TARGET_RANDOM_SINGLE;
        spells[1].instant = false;
        spells[1].perctrigger = 20.0f;
        spells[1].cooldown = 5;
        spells[1].mindist2cast = 0.0f;
        spells[1].maxdist2cast = 45.0f;

    }
    
    void OnCombatStart(UnitPointer mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "You only waste my time!");
        _unit->PlaySoundToSet(12378);

        _unit->SetUInt32Value(UNIT_FIELD_POWER1, 0);

        RegisterAIUpdateEvent(1000);
    }

    void OnTargetDied(UnitPointer mTarget)
    {
        if (_unit->GetHealthPct() > 0)    
        {
            if(_unit->GetManaPct() > 10)
            {
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I AM INVINCIBLE!");
                _unit->PlaySoundToSet(12385);
            }
            else
            {
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Enough distractions!");
                _unit->PlaySoundToSet(12388);
            }

        }
    }
    
    void OnCombatStop(UnitPointer mTarget)
    {
            _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
            _unit->GetAIInterface()->SetAIState(STATE_EVADE);

            RemoveAIUpdateEvent();
    }
    
    void OnDied(UnitPointer mKiller)
    {
        RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
        CreaturePointer felcrys = NULLCREATURE;
        if(_unit->GetCurrentSpell() == NULL)
        {
            if (_unit->GetHealthPct() > 0)
            {
                if(_unit->GetUInt32Value(UNIT_FIELD_POWER1) >= 3231)
                {
                _unit->CastSpell(_unit, FEL_EXPLOSION, true);
                _unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_POWER1) - 3231);

                felcrys = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), FEL_CRYSTAL);
                if(felcrys)
                    felcrys->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                if(_unit->GetUInt32Value(UNIT_FIELD_POWER1) <= 3200)
                {
                        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "No! More... I must have more!");
                        _unit->PlaySoundToSet(12383);
                }
                else if(_unit->GetManaPct() > 85)
                {
                    _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Yes! I am a god!");
                    _unit->PlaySoundToSet(12382);
                }
                }
                else
                {
                    felcrys = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), FEL_CRYSTAL);
                    if(felcrys)
                    {
                        if (felcrys->GetHealthPct() > 1) 
                        {
                            if (RandomUInt(100) < 15)
                            {
                                //Drain Fel Crystal
                                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My hunger knows no bounds!");
                                _unit->PlaySoundToSet(12381);
                                _unit->CastSpell(felcrys, dbcSpell.LookupEntry(CRYS_DRAIN), false);
                                felcrys->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
                            }
                        }
                        else if(felcrys->GetHealthPct() <= 1) 
                        {
                            felcrys->Despawn(0,0);
                        }
                    }
                }

                    
            }
        }
        else
        {
            if(_unit->GetUInt32Value(UNIT_CHANNEL_SPELL) == CRYS_DRAIN)
            {
                if((_unit->GetUInt32Value(UNIT_FIELD_POWER1) + 3231) <= _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
                {
                    _unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_POWER1) + 3231);
                }
                else
                {
                    _unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
                }
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
            std::vector<UnitPointer > TargetTable;        /* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
                                                /* If anyone wants to use this function, then leave this note!                                         */
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Vexallus Event
#define VEXALLUS 24744

//Pure Energy
#define PURE_ENERGY 24745
#define ENERGY_BOLT 44342
class PureEnergyAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(PureEnergyAI);
    PureEnergyAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->GetAIInterface()->disable_melee = true;
    }

    void OnLoad()
    {
        RegisterAIUpdateEvent(1000);
    }

    void AIUpdate()
    {
        _unit->CastSpell(_unit, dbcSpell.LookupEntry(ENERGY_BOLT), true);
    }
    
    void OnDied(UnitPointer mKiller)
    {

        AuraPointer pAura = AuraPointer(new Aura(dbcSpell.LookupEntry(44335), 30000, mKiller, mKiller));
        if (pAura != NULL)
            mKiller->AddAura(pAura,NULLAURA);

        RemoveAIUpdateEvent();
    }

    void OnCombatStop(UnitPointer mTarget)
    {
        RemoveAIUpdateEvent();
        _unit->Despawn(0,0);
    }
};


//Vexallus
#define CHAIN_LIGHTNING 44318
#define ARCANE_SHOCK 44319
#define OVERLOAD 44353
class VexallusAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(VexallusAI);
    SP_AI_Spell spells[2];
    bool m_spellcheck[2];
    int SummonPasses;
    int Unleash;

    VexallusAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        nrspells = 2;
        for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;

        }

        spells[0].info = dbcSpell.LookupEntry(CHAIN_LIGHTNING);
        spells[0].targettype = TARGET_RANDOM_SINGLE;
        spells[0].instant = true;
        spells[0].perctrigger = 20.0f;
        spells[0].cooldown = 5;

        spells[1].info = dbcSpell.LookupEntry(ARCANE_SHOCK);
        spells[1].targettype = TARGET_RANDOM_SINGLE;
        spells[1].instant = true;
        spells[1].perctrigger = 20.0f;
        spells[1].cooldown = 5;
        spells[1].mindist2cast = 0.0f;
        spells[1].maxdist2cast = 20.0f;

    }

    void OnCombatStart(UnitPointer mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Drain... life...");
        _unit->PlaySoundToSet(12389);
        SummonPasses = Unleash = 0;

        RegisterAIUpdateEvent(1000);
    }
    
    void OnTargetDied(UnitPointer mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Con...sume.");
        _unit->PlaySoundToSet(12393);
    }
    
    void OnCombatStop(UnitPointer mTarget)
    {
            _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
            _unit->GetAIInterface()->SetAIState(STATE_EVADE);

            RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
        RemoveAIUpdateEvent();
    }

    void AIUpdate()
    {
        UnitPointer target = NULLUNIT;
        if(_unit->GetAIInterface()->GetNextTarget())
        {
            target = _unit->GetAIInterface()->GetNextTarget();

        if((SummonPasses == 0 && _unit->GetHealthPct() <= 85) ||
        (SummonPasses == 1 && _unit->GetHealthPct() <= 70) ||
        (SummonPasses == 2 && _unit->GetHealthPct() <= 55) ||
        (SummonPasses == 3 && _unit->GetHealthPct() <= 40) ||
        (SummonPasses == 4 && _unit->GetHealthPct() <= 25))
        {
            _unit->GetMapMgr()->GetInterface()->SpawnCreature(PURE_ENERGY, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
            _unit->GetMapMgr()->GetInterface()->SpawnCreature(PURE_ENERGY, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, true, false, 0, 0);
            SummonPasses++;
            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Un...con...tainable.");
            _unit->PlaySoundToSet(12392);
        }

        else if (_unit->GetHealthPct() <= 15)
        {
            if (RandomUInt(100) < 40)
            {
                _unit->CastSpell(_unit, dbcSpell.LookupEntry(OVERLOAD), true);

                if(Unleash == 0)
                {
                    _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Un...leash...");
                    _unit->PlaySoundToSet(12390);
                    Unleash = 1;
                }
            }
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
            std::vector<UnitPointer > TargetTable;        /* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
                                                /* If anyone wants to use this function, then leave this note!                                         */
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

//********************************************************************************

//Priestess Delrissa Event

#define PRIESTESS_DELRISSA 24560
//summons
#define KAGANI_NIGHTSTRIKE 24557
#define ELLRYS_DUSKHALLOW  24558
#define ERAMAS_BRIGHTBLAZE 24554
#define YAZZAI             24561
#define WARLORD_SALARIS    24559
#define GARAXXAS           24555
#define APOKO              24553
#define ZELFAN             24556

//Delrissa
#define DISPEL_MAGIC       27609
#define FLASH_HEAL         17843
#define SW_PAIN            14032
#define PW_SHIELD          44291
#define RENEW              46192

class PriestessDelrissaAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(PriestessDelrissaAI);
    SP_AI_Spell spells[5];
    bool m_spellcheck[5];
    int Spawn[7];

    PriestessDelrissaAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        nrspells = 5;
        for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;

        }

        spells[0].info = dbcSpell.LookupEntry(DISPEL_MAGIC);
        spells[0].targettype = TARGET_RANDOM_SINGLE;
        spells[0].instant = true;
        spells[0].perctrigger = 10.0f;
        spells[0].cooldown = 10;

        spells[1].info = dbcSpell.LookupEntry(DISPEL_MAGIC);
        spells[1].targettype = TARGET_RANDOM_SINGLE;
        spells[1].instant = true;
        spells[1].perctrigger = 10.0f;
        spells[1].cooldown = 10;

        spells[2].info = dbcSpell.LookupEntry(FLASH_HEAL);
        spells[2].targettype = TARGET_SELF;
        spells[2].instant = false;
        spells[2].perctrigger = 20.0f;
        spells[2].cooldown = 8;

        spells[3].info = dbcSpell.LookupEntry(PW_SHIELD);
        spells[3].targettype = TARGET_SELF;
        spells[3].instant = true;
        spells[3].perctrigger = 15.0f;
        spells[3].cooldown = 12;

        spells[4].info = dbcSpell.LookupEntry(RENEW);
        spells[4].targettype = TARGET_SELF;
        spells[4].instant = true;
        spells[4].perctrigger = 25.0f;
        spells[4].cooldown = 8;

/*   NO ES NECESARIO ESTO, YA QUE EN BLIZZ LOS MINIONS EN BLIZZ ESTAN YA SPAWNEADOS, ADEMAS ESTA FUNCION CRASHEA ^^
        Spawn[0] = KAGANI_NIGHTSTRIKE;
        Spawn[1] = ELLRYS_DUSKHALLOW;
        Spawn[2] = ERAMAS_BRIGHTBLAZE;
        Spawn[3] = YAZZAI;
        Spawn[4] = WARLORD_SALARIS;
        Spawn[5] = GARAXXAS;
        Spawn[6] = APOKO;
        Spawn[7] = ZELFAN;
    }

    void OnLoad()
    {
        //Summon random adds
        uint32 Pos1 = RandomUInt(7);
        uint32 Pos2 = RandomUInt(7);
        while (Pos1 == Pos2)
        { Pos2 = RandomUInt(7); }
        uint32 Pos3 = RandomUInt(7);
        while (Pos2 == Pos3 || Pos1 == Pos3)
        { Pos3 = RandomUInt(7); }
        uint32 Pos4 = RandomUInt(7);
        while (Pos3 == Pos4 || Pos2 == Pos4 || Pos1 == Pos4)
        { Pos4 = RandomUInt(7); }
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(Spawn[Pos1], 130.182861, 18.681139, -20.004908, 4.379380, true, false, 0, 0);
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(Spawn[Pos2], 132.17772, 16.45604, -20.004908, 3.4927, true, false, 0, 0);
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(Spawn[Pos3], 123.523895, 18.66, -20.004908, 5.17656, true, false, 0, 0);
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(Spawn[Pos4], 122.29137, 14.74014, -20.004908, 5.9148, true, false, 0, 0);

*/ 
    }
 
    void OnCombatStart(UnitPointer mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Annihilate them!");
        _unit->PlaySoundToSet(12395);

        RegisterAIUpdateEvent(1000);
    }

    void OnTargetDied(UnitPointer mTarget)
    {
		if (_unit->GetHealthPct() > 0)	
		{
			int RandomSpeach;
			RandomUInt(1000);
			RandomSpeach=rand()%4;
			switch (RandomSpeach)
			{
			case 0:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I call that a good start.");
				_unit->PlaySoundToSet(12405);
				break;
			case 1:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I could have sworn there were more of you.");
				_unit->PlaySoundToSet(12407);
				break;
			case 2:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Not really much of a group, anymore, is it?");
				_unit->PlaySoundToSet(12409);
				break;
			case 3:
				_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "One is such a lonely number.");
				_unit->PlaySoundToSet(12410);
				break;
			}
		}
    }
    
    void OnCombatStop(UnitPointer mTarget)
    {
            _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
            _unit->GetAIInterface()->SetAIState(STATE_EVADE);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "It's been a kick, really.");
			_unit->PlaySoundToSet(12411);

            RemoveAIUpdateEvent();
    }
    
    void OnDied(UnitPointer mKiller)
    {
        RemoveAIUpdateEvent();
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Not what I had... planned...");
        _unit->PlaySoundToSet(12397);
        
    }

    void AIUpdate()
    {
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
            std::vector<UnitPointer > TargetTable;        /* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
                                                /* If anyone wants to use this function, then leave this note!                                         */
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

//Delrissa Adds AI
//Kagani Nightstrike
#define KAGANI_GOUGE 12540
#define KAGANI_KIDNEY_SHOT 27615
#define KAGANI_BACKSTAB 15582
#define KAGANI_EVISCERATE 46189
#define KAGANI_KICK 27613
#define DRINK_POT 15503
class KaganiNightstrikeAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(KaganiNightstrikeAI, MoonScriptCreatureAI);
    KaganiNightstrikeAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
        AddSpell(KAGANI_GOUGE, Target_Current, 20, 0, 8);
        AddSpell(KAGANI_KICK, Target_Current, 20, 0, 8);
        AddSpell(KAGANI_BACKSTAB, Target_Current, 30, 0, 2);
        AddSpell(KAGANI_EVISCERATE, Target_Current, 20, 0, 10);
        AddSpell(KAGANI_KIDNEY_SHOT, Target_Current, 20, 0, 10);
    }
};

//Ellrys Duskhallow
#define ELLRYS_IMMOLATE 46191
#define ELLRYS_SHADOW_BOLT 15232
#define ELLRYS_SUMMON_IMP 44163
#define ELLRYS_SEED_OF_CORRUPTION 44141
#define ELLRYS_CURSE_OF_AGONY 46190
#define ELLRYS_FEAR 38595
class EllrysDuskhallowAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EllrysDuskhallowAI, MoonScriptCreatureAI);
    EllrysDuskhallowAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
        AddSpell(ELLRYS_FEAR, Target_Current, 15, 1.5, 8);
        AddSpell(ELLRYS_SEED_OF_CORRUPTION, Target_Current, 25, 2, 12);
        AddSpell(ELLRYS_IMMOLATE, Target_RandomPlayer, 35, 2, 5);
        AddSpell(ELLRYS_SHADOW_BOLT, Target_RandomPlayer, 35, 3, 5);
        AddSpell(ELLRYS_CURSE_OF_AGONY, Target_RandomPlayer, 35, 0, 14);
    }

        void OnCombatStart(UnitPointer mTarget)
    {
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(FIZZLE, _unit->GetPositionX() + 1.5f, _unit->GetPositionY(), _unit->GetPositionZ(), 2.5f, true, false, 0, 0);
        
    }
};

//Eramas Brightblaze
#define ERAMAS_KNOCKDOWN 46183
#define ERAMAS_SNAP_KICK 46182
class EramasBrightblazeAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(EramasBrightblazeAI, MoonScriptCreatureAI);
    EramasBrightblazeAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(ERAMAS_KNOCKDOWN, Target_Current, 20, 0, 8);
        AddSpell(ERAMAS_SNAP_KICK, Target_Current, 20, 0, 8);
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
    }
};

//Yazzai
#define YAZZAI_POLYMORPH 13323
#define YAZZAI_ICE_BLOCK 27619
#define YAZZAI_BLIZZARD 46195
#define YAZZAI_ICE_LANCE 46194
#define YAZZAI_CONE_OF_COLD 38384
#define YAZZAI_FROSTBOLT 15043
class YazzaiAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(YazzaiAI, MoonScriptCreatureAI);
    YazzaiAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(YAZZAI_FROSTBOLT, Target_Current, 35, 3, 2);
        AddSpell(YAZZAI_ICE_BLOCK, Target_Self, 13, 0, 18);
        AddSpell(YAZZAI_ICE_LANCE, Target_RandomPlayer, 30, 0, 6);
        AddSpell(YAZZAI_POLYMORPH, Target_RandomPlayer, 22, 1.5, 11);
        AddSpell(YAZZAI_BLIZZARD, Target_RandomDestination, 30, 8, 9);
        AddSpell(YAZZAI_CONE_OF_COLD, Target_RandomDestination, 30, 0, 9);
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
    }
};

//Warlord Salaris
#define SALARIS_INTERCEPT 20615
#define SALARIS_DISARM 27581
#define SALARIS_PIERCING_HOWL 23600
#define SALARIS_FRIGHT_SHOUT 19134
#define SALARIS_HAMSTRING 27584
#define SALARIS_BATTLE_SHOUT 27578
#define SALARIS_MORTAL_STRIKE 44268
class WarlordSalarisAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(WarlordSalarisAI, MoonScriptCreatureAI);
    WarlordSalarisAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(SALARIS_INTERCEPT, Target_Current, 35, 0, 6);
        AddSpell(SALARIS_DISARM, Target_Current, 30, 0, 11);
        AddSpell(SALARIS_BATTLE_SHOUT, Target_Self, 13, 0, 18);
        AddSpell(SALARIS_MORTAL_STRIKE, Target_Current, 27, 0, 10);
        AddSpell(SALARIS_HAMSTRING, Target_Current, 22, 0, 11);
        AddSpell(SALARIS_FRIGHT_SHOUT, Target_RandomDestination, 27, 0, 12);
        AddSpell(SALARIS_PIERCING_HOWL, Target_RandomDestination, 24, 0, 12);
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
    }
};

//Garaxxas
#define GARAXXAS_AIMED_SHOT 44271
#define GARAXXAS_SHOOT 22907
#define GARAXXAS_CONCUSSIVE_SHOT 27634
#define GARAXXAS_MULTI_SHOT 44285
#define GARAXXAS_WING_CLIP 44286
#define GARAXXAS_FREEZING_TRAP 44136
class GaraxxasAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(GaraxxasAI, MoonScriptCreatureAI);
    GaraxxasAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(GARAXXAS_WING_CLIP, Target_Current, 35, 0, 10);
        AddSpell(GARAXXAS_SHOOT, Target_RandomPlayer, 30, 0, 11, 5, 30);
        AddSpell(GARAXXAS_CONCUSSIVE_SHOT, Target_RandomPlayer, 30, 0, 10, 5, 30);
        AddSpell(GARAXXAS_AIMED_SHOT, Target_RandomPlayer, 30, 3, 6, 5, 35);
        AddSpell(GARAXXAS_FREEZING_TRAP, Target_Self, 25, 0, 18);
        AddSpell(GARAXXAS_MULTI_SHOT, Target_RandomDestination, 27, 0, 12, 5, 30);
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
    }
};

//Apoko
#define APOKO_EARTHBIND_TOTEM 15786
#define APOKO_FIRE_NOVA_TOTEM 44257
#define APOKO_FROST_SHOCK 46180
#define APOKO_LESSER_HEAL_WAVE 46181
#define APOKO_PURGE 27626
#define APOKO_WAR_STOMP 46026
#define APOKO_WINDFURY_TOTEM 27621
class ApokoAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ApokoAI, MoonScriptCreatureAI);
    ApokoAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(APOKO_LESSER_HEAL_WAVE, Target_RandomFriendly, 35, 1.5, 10, 0, 40);
        AddSpell(APOKO_PURGE, Target_RandomPlayer, 30, 0, 8, 0, 30);
        AddSpell(APOKO_FROST_SHOCK, Target_RandomPlayer, 30, 0, 8, 0, 20);
        AddSpell(APOKO_EARTHBIND_TOTEM, Target_Self, 20, 0, 25);
        AddSpell(APOKO_FIRE_NOVA_TOTEM, Target_Self, 20, 0, 8);
        AddSpell(APOKO_WINDFURY_TOTEM, Target_Self, 20, 0, 30);
        AddSpell(APOKO_WAR_STOMP, Target_Self, 27, 0, 12);
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
    }
};

//Zelfan
#define ZELFAN_GOBLIN_DRAGON_GUN 46185
#define ZELFAN_ROCKET_LAUNCH 46187
#define ZELFAN_HIGH_EXPLOSIVE_SHEEP 44276
#define ZELFAN_FEL_IRON_BOMB 46024
class ZelfanAI : public MoonScriptCreatureAI
{
    MOONSCRIPT_FACTORY_FUNCTION(ZelfanAI, MoonScriptCreatureAI);
    ZelfanAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        AddSpell(ZELFAN_GOBLIN_DRAGON_GUN, Target_RandomDestination, 25, 8, 8, 0, 30);
        AddSpell(ZELFAN_ROCKET_LAUNCH, Target_RandomPlayer, 30, 3.5, 8, 0, 45);
        AddSpell(ZELFAN_FEL_IRON_BOMB, Target_RandomDestination, 20, 1, 9, 0, 15);
        AddSpell(ZELFAN_HIGH_EXPLOSIVE_SHEEP, Target_RandomDestination, 20, 2, 15);
        AddSpell(DRINK_POT, Target_Self, 15, 0, 10);
    }
};

//********************************************************************************

//Kael'thas Sunstrider
#define KAELTHAS 24664
#define PHOENIX 24674
#define PHOENIX_FIREBALL 44202
#define PHOENIX_EMBER_BLAST 44199
#define PHOENIX_REBIRTH_DMG 44196
#define PHOENIX_HELLFIRE 39132
#define PHOENIX_EGG 24675

//Phoenix
class KaelPhoenixAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KaelPhoenixAI);

    KaelPhoenixAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {    }

    void OnLoad()
    {
        RegisterAIUpdateEvent(1000);
        CreaturePointer kaelthas = NULLCREATURE;
        kaelthas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), KAELTHAS);
        if(!kaelthas)
        {
            _unit->Despawn(0,0);
        }
        else
        {
            _unit->GetAIInterface()->SetNextTarget(kaelthas->GetAIInterface()->GetNextTarget());
        }
        
    }

    void AIUpdate()
    {
        CreaturePointer kaelthas = NULLCREATURE;
        kaelthas = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), KAELTHAS);
        if(!kaelthas)
        {
            RemoveAIUpdateEvent();
            _unit->Despawn(0,0);
        }
        else
        {
            if(kaelthas->GetHealthPct() < 50)
            {
                _unit->GetAIInterface()->m_canMove = false;
                CastSpellOnRandomTarget();
            }
            
            _unit->CastSpell(_unit, dbcSpell.LookupEntry(PHOENIX_HELLFIRE), true);

            if(_unit->GetHealthPct() > 5)
            {
                _unit->SetHealthPct(_unit->GetHealthPct() - 5);
            }
            else
            {
                _unit->GetMapMgr()->GetInterface()->SpawnCreature(PHOENIX_EGG, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
                RemoveAIUpdateEvent();
                _unit->Despawn(0,0);
            }
        }
    }
    
    void OnCombatStop(UnitPointer mTarget)
    {
        RemoveAIUpdateEvent();
    }

    void OnDied(UnitPointer mKiller)
    {
        _unit->GetMapMgr()->GetInterface()->SpawnCreature(PHOENIX_EGG, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
        RemoveAIUpdateEvent();
        _unit->Despawn(0,0);
    }

    void CastSpellOnRandomTarget()
    {
        if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->GetNextTarget())
            {
            std::vector<UnitPointer > TargetTable;     /* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */

            if (!TargetTable.size())
                return;

            size_t RandTarget = rand()%TargetTable.size();

            UnitPointer RTarget = TargetTable[RandTarget];

            if (!RTarget)
                return;

            _unit->CastSpell(RTarget, dbcSpell.LookupEntry(PHOENIX_FIREBALL), false);

            TargetTable.clear();
        }
    }
};

//Phoenix Egg
class PhoenixEggAI : public MoonScriptCreatureAI
{
public:
    int p;
    
    ADD_CREATURE_FACTORY_FUNCTION(PhoenixEggAI);
    PhoenixEggAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->GetAIInterface()->disable_melee = true;
        _unit->GetAIInterface()->m_canMove = false;
    }

    void OnLoad()
    {
        RegisterAIUpdateEvent(1000);
        p = 0;
    }

    void AIUpdate()
    {
        p++;
        if (p >= 12)
        {
            _unit->CastSpell(_unit, dbcSpell.LookupEntry(PHOENIX_REBIRTH_DMG), true);
            _unit->GetMapMgr()->GetInterface()->SpawnCreature(PHOENIX, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
            RemoveAIUpdateEvent();
            _unit->Despawn(0,0);
        }
    }

};

//Gravity Lapse Bubble
#define BUBBLE_DMG 29765
#define GL_BUBBLE 100922
class GLBubbleAI : public MoonScriptCreatureAI
{
public:
    int t;
    ADD_CREATURE_FACTORY_FUNCTION(GLBubbleAI);
    GLBubbleAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        _unit->GetAIInterface()->m_moveFly = true;
    }

    void OnLoad()
    {
        RegisterAIUpdateEvent(500);
        t = 0;
        AddRandomWPs(_unit);
        _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
        _unit->GetAIInterface()->setWaypointToMove(1);
    }

    void AIUpdate()
    {
        
        UnitPointer pUnit;
        float dist;

        for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
        {
            if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
                continue;

            pUnit = TO_UNIT((*itr));

            
            if(!pUnit->isAlive() || _unit == pUnit)
                continue;

            dist = _unit->GetDistance2dSq(pUnit);
            if(dist < 10)
            {
                _unit->CastSpell(pUnit, dbcSpell.LookupEntry(BUBBLE_DMG), true);
            }
        }
        t++;
        if(t > 60)
        {
            _unit->Despawn(0,0);
            RemoveAIUpdateEvent();
        }
        if((t % 8) == 0)
        {
            _unit->GetAIInterface()->setWaypointToMove(RandomUInt(4));
            _unit->GetAIInterface()->setMoveType(MOVEMENTTYPE_WANTEDWP);
        }
    }

    void AddRandomWPs(UnitPointer mUnit)
    {
    
    int n = 5;

    int r = 30;

    for(int i=0;i<n;i++)
    {
        float ang = rand()/100.0f;
        float ran = (rand()%(r*10))/10.0f;
        while(ran<1)
        {
            ran = (rand()%(r*10))/10.0f;
        }

        float x = mUnit->GetPositionX()+ran*sin(ang);
        float y = mUnit->GetPositionY()+ran*cos(ang);
        float z = mUnit->GetPositionZ() + (float)RandomFloat(5.0f);
         
        WayPoint* wp = new WayPoint;
        wp->id = (uint32)mUnit->GetAIInterface()->GetWayPointsCount()+1;
        wp->x = x;
        wp->y = y;
        wp->z = z;
        wp->waittime = 0;
        wp->flags = 0;
        wp->forwardemoteoneshot = 0;
        wp->forwardemoteid = 0;
        wp->backwardemoteoneshot = 0;
        wp->backwardemoteid = 0;
        wp->forwardskinid = 0;
        wp->backwardskinid = 0;
        
        mUnit->GetAIInterface()->addWayPoint(wp);
    }
    }

};

//Flamestrike AI
#define KAEL_FLAMESTRIKE 24666
#define FLAMESTRIKE_SPELL 44190
#define FLAME_STRIKE_TRIGGER_FLAME_STRIKE_EFFECT    36730
class KaelFlamestrikeAI : public MoonScriptCreatureAI
{
public:
    int g;
    ADD_CREATURE_FACTORY_FUNCTION(KaelFlamestrikeAI);
    KaelFlamestrikeAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        _unit->GetAIInterface()->disable_melee = true;
        _unit->CastSpell(_unit, dbcSpell.LookupEntry(FLAME_STRIKE_TRIGGER_FLAME_STRIKE_EFFECT), true);
        _unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        _unit->m_canMove = false;
        _unit->m_noRespawn = true;
    }

    void OnLoad()
    {
        RegisterAIUpdateEvent(1000);
        g = 0;
    }

    void AIUpdate()
    {
        if(g > 5)
        {
            _unit->CastSpell(_unit, dbcSpell.LookupEntry(FLAMESTRIKE_SPELL), true);
            RemoveAIUpdateEvent();
            _unit->Despawn(0,0);
        }
    }

};



//Kael'thas

#define KAEL_FLAME_STRIKE 46162
#define KAEL_PYROBLAST 36819
#define KAEL_SHOCK_BARRIER 46165
#define KAEL_GRAVITY_LAPSE 44224
#define KAEL_FIREBALL 46164
#define KAEL_GL_DMG_KBACK 44226
#define KAEL_GL_VISUAL 44251
#define KAEL_GL_SUMMON 35989
#define KAEL_KNOCKBACK 28405
class KaelThasMTAI : public MoonScriptCreatureAI
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(KaelThasMTAI);
    SP_AI_Spell spells[4];
    bool m_spellcheck[4];
    int gl;
    int tired;
    int sc;
    int p1;
    float Phase;

    KaelThasMTAI(CreaturePointer pCreature) : MoonScriptCreatureAI(pCreature)
    {
        nrspells = 4;
        for(int i=0;i<nrspells;i++)
        {
            m_spellcheck[i] = false;

        }

        spells[0].info = dbcSpell.LookupEntry(KAEL_FLAME_STRIKE);
        spells[0].targettype = TARGET_RANDOM_DESTINATION;
        spells[0].instant = true;
        spells[0].perctrigger = 23.0f;
        spells[0].cooldown = 6;

        spells[1].info = dbcSpell.LookupEntry(KAEL_PYROBLAST);
        spells[1].targettype = TARGET_ATTACKING;
        spells[1].instant = false;
        spells[1].perctrigger = 15.0f;
        spells[1].cooldown = 10;

        spells[2].info = dbcSpell.LookupEntry(KAEL_FIREBALL);
        spells[2].targettype = TARGET_ATTACKING;
        spells[2].instant = false;
        spells[2].perctrigger = 30.0f;
        spells[2].cooldown = 6;

        spells[3].info = dbcSpell.LookupEntry(KAEL_SHOCK_BARRIER);
        spells[3].targettype = TARGET_SELF;
        spells[3].instant = true;
        spells[3].perctrigger = 23.0f;
        spells[3].cooldown = 10;

    }


    void OnCombatStart(UnitPointer mTarget)
    {
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Don't look so smug! I know what you're thinking, but Tempest Keep was merely a set back. Did you honestly believe I would trust the future to some blind, half-night elf mongrel?");
          _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL,"Oh no, no, no, he was merely an instrument, a stepping stone to a much larger plan! It has all led to this...and this time, you will not interfere!");
        _unit->PlaySoundToSet(12413);

        gl = tired = p1 = 0;
        Phase = 1;

        RegisterAIUpdateEvent(1000);
    }


    void OnCombatStop(UnitPointer mTarget)
    {
        _unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
        _unit->GetAIInterface()->SetAIState(STATE_IDLE);

    if(!_unit->GetAIInterface()->GetNextTarget())
    {
            RemoveAIUpdateEvent();
    }
    }

    void OnDied(UnitPointer mKiller)
    {
        RemoveAIUpdateEvent();
        _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "My demise accomplishes nothing! The Master will have you! You will drown in your own blood! The world shall burn! Aaaghh!");
        _unit->PlaySoundToSet(12421);

        UnitPointer pUnit;
        for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
        {
            if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
            continue;

            pUnit = TO_UNIT((*itr));
            if(pUnit == _unit)
                continue;

            if(!pUnit->isAlive())
                continue;
            
            
            pUnit->DisableFlight();
        }
            CreaturePointer ball = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GL_BUBBLE);
            while(ball)
            {    
                ball->Despawn(0,0);
                ball = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GL_BUBBLE);
            }
    }

    void AIUpdate()
    {
    
    if (Phase == 1)
    {
        p1++;
        float val = (float)RandomFloat(100.0f);
        SpellCast(val);
        if(p1 == 20 || p1 == 40)
        {
            _unit->GetMapMgr()->GetInterface()->SpawnCreature(PHOENIX, _unit->GetPositionX() + RandomUInt(3), _unit->GetPositionY() + RandomUInt(2), _unit->GetPositionZ() + RandomUInt(2), 0, true, false, 0, 0);
            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Vengeance burns!");
            _unit->PlaySoundToSet(12415);
        }
    }
    
    if(_unit->GetHealthPct() <= 50)
    {
        if(Phase == 1)
        {
            Phase = 1.5;
            _unit->CastSpell(_unit, dbcSpell.LookupEntry(KAEL_GRAVITY_LAPSE), false);
            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "I'll turn your world...upside...down.");
            _unit->PlaySoundToSet(12418);
            sc = 0;
            _unit->GetAIInterface()->m_canMove = false;
        }
        
        else if (Phase == 1.5)
        {
            sc++;
            if(sc > 2)
            {
                Phase = 1.75;
                _unit->CastSpell(_unit, dbcSpell.LookupEntry(KAEL_GL_VISUAL), false);
            }
        }

        else if(Phase == 1.75)
        {
            
            //Gravity Lapse
            UnitPointer pUnit;
            for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
            {
                if((*itr)->GetTypeId() != TYPEID_PLAYER)
                    continue;

                pUnit = TO_UNIT((*itr));

                if(pUnit == _unit)
                    continue;
                if(!pUnit->isAlive())
                    continue;

                pUnit->EnableFlight();

            }

            Phase = 2;
            //Summon ball things
            UnitPointer ball1;
            UnitPointer ball2;
            UnitPointer ball3;

            ball1 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(GL_BUBBLE, _unit->GetPositionX() + RandomUInt(3), _unit->GetPositionY() + RandomUInt(2), _unit->GetPositionZ() + RandomUInt(2), 0, true, false, 0, 0);
            ball2 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(GL_BUBBLE, _unit->GetPositionX() - RandomUInt(3), _unit->GetPositionY() - RandomUInt(2), _unit->GetPositionZ() + RandomUInt(2), 0, true, false, 0, 0);
            ball3 = _unit->GetMapMgr()->GetInterface()->SpawnCreature(GL_BUBBLE, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 0, true, false, 0, 0);
        }

        else if (Phase == 2)
        {
            gl++;
            _unit->GetAIInterface()->m_canMove = false;

            if (gl > 30)
            {
                //Undo Gravity Lapse
                
                UnitPointer pUnit;
                for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
                {
                    if((*itr)->GetTypeId() != TYPEID_PLAYER)
                        continue;
    
                    pUnit = TO_UNIT((*itr));
                    if(pUnit == _unit)
                        continue;
    
                    if(!pUnit->isAlive())
                        continue;
                    
                    
                    pUnit->DisableFlight();
                    gl = 0;
                }
                
                    Phase = 2.5;
            
                    CreaturePointer ball = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GL_BUBBLE);
                    while (ball)
                    {
                        ball->Despawn(0,0);
                        ball = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords( _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), GL_BUBBLE);
                    }

            }

            else
            {
            float z_diff;
            UnitPointer pUnit;
            for (unordered_set<ObjectPointer>::iterator itr = _unit->GetInRangeOppFactsSetBegin(); itr != _unit->GetInRangeOppFactsSetEnd(); itr++)
            {
                if((*itr)->GetTypeId() != TYPEID_UNIT && (*itr)->GetTypeId() != TYPEID_PLAYER)
                continue;

                pUnit = TO_UNIT((*itr));
                if(pUnit == _unit)
                    continue;

                if(!pUnit->isAlive())
                    continue;

                pUnit->EnableFlight();
                z_diff = fabs(_unit->GetPositionZ() - pUnit->GetPositionZ());
                if(z_diff < 1)
                {
                    _unit->CastSpell(pUnit, dbcSpell.LookupEntry(KAEL_KNOCKBACK), true);
                }
            }
            }
        }

        else if (Phase == 2.5)
        {
            gl++;
            _unit->GetAIInterface()->m_canMove = false;
            if (gl > 10 && RandomUInt(100) < 20)
            {
            //Gravity Lapse
            Phase = 1.5;

            _unit->CastSpell(_unit, dbcSpell.LookupEntry(KAEL_GRAVITY_LAPSE), false);
            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Do not get...too comfortable. ");
            _unit->PlaySoundToSet(12420);
            gl = sc = 0;
            }

        }
        if(_unit->GetHealthPct() < 20 && tired == 0)
        {
            _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Master, grant me strength.");
            _unit->PlaySoundToSet(12419);
            tired = 1;
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
            std::vector<UnitPointer > TargetTable;        /* From M4ksiu - Big THX to Capt who helped me with std stuff to make it simple and fully working <3 */
                                                /* If anyone wants to use this function, then leave this note!                                         */
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
            {
                _unit->CastSpellAoF(RTarget->GetPositionX(), RTarget->GetPositionY(), RTarget->GetPositionZ(), spells[i].info, spells[i].instant);
                _unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Felomin ashal!");
                _unit->PlaySoundToSet(12417);
                break;
            }
            }

            TargetTable.clear();
        }
    }

protected:

    int nrspells;

};


void SetupMagistersTerrace(ScriptMgr * mgr) 
{
    mgr->register_creature_script(WRETCHED_HUSK, &WretchedHuskAI::Create);
    mgr->register_creature_script(WRETCHED_SKULKER, &WretchedSkulkerAI::Create);
    mgr->register_creature_script(WRETCHED_BRUISER, &WretchedBruiserAI::Create);
    mgr->register_creature_script(SUNBLADE_WARLOCK, &SunbladeWarlockAI::Create);
    mgr->register_creature_script(SUNBLADE_PHYSICIAN, &SunbladePhysicianAI::Create);
    mgr->register_creature_script(SUNBLADE_MAGISTER, &SunbladeMagisterAI::Create);
    mgr->register_creature_script(SUNBLADE_MAGE_GUARD, &SunbladeMageGuardAI::Create);
    mgr->register_creature_script(SUNBLADE_KEEPER, &SunbladeKeeperAI::Create);
    mgr->register_creature_script(SUNBLADE_IMP, &SunbladeImpAI::Create);
    mgr->register_creature_script(SISTER_OF_TORMENT, &TormentSisterAI::Create);
    mgr->register_creature_script(FIZZLE, &FizzleAI::Create);
    mgr->register_creature_script(ETHEREUM_SMUGGLER, &EthereumSmugglerAI::Create);
    mgr->register_creature_script(COILSKAR_WITCH, &CoilskarWitchAI::Create);
    mgr->register_creature_script(SUNBLADE_BLOOD_KNIGHT, &SunbladeBloodKnightAI::Create);
    mgr->register_creature_script(BRIGHTSCALE_WYRM, &BrightscaleWyrmAI::Create);
    mgr->register_creature_script(FEL_CRYSTAL, &FelCrystalAI::Create);
    mgr->register_creature_script(SELIN_FIREHEART, &SelinFireheartAI::Create);
    mgr->register_creature_script(PURE_ENERGY, &PureEnergyAI::Create);
    mgr->register_creature_script(VEXALLUS, &VexallusAI::Create);
    mgr->register_creature_script(PRIESTESS_DELRISSA, &PriestessDelrissaAI::Create);
    mgr->register_creature_script(KAGANI_NIGHTSTRIKE, &KaganiNightstrikeAI::Create);
    mgr->register_creature_script(ELLRYS_DUSKHALLOW, &EllrysDuskhallowAI::Create);
    mgr->register_creature_script(ERAMAS_BRIGHTBLAZE, &EramasBrightblazeAI::Create);
    mgr->register_creature_script(WARLORD_SALARIS, &WarlordSalarisAI::Create);
    mgr->register_creature_script(YAZZAI, &YazzaiAI::Create);
    mgr->register_creature_script(GARAXXAS, &GaraxxasAI::Create);
    mgr->register_creature_script(APOKO, &ApokoAI::Create);
    mgr->register_creature_script(ZELFAN, &ZelfanAI::Create);
    mgr->register_creature_script(KAELTHAS, &KaelThasMTAI::Create);
    mgr->register_creature_script(GL_BUBBLE, &GLBubbleAI::Create);
    mgr->register_creature_script(PHOENIX_EGG, &PhoenixEggAI::Create);
    mgr->register_creature_script(PHOENIX, &KaelPhoenixAI::Create);
    mgr->register_creature_script(KAEL_FLAMESTRIKE, &KaelFlamestrikeAI::Create);
}
