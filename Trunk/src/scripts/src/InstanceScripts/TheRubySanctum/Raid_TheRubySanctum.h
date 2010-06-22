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


enum eRubyNPC
{
    NPC_BALTHARUS                       = 39751,
    NPC_BALTHARUSCLONE                  = 39899,
    NPC_ZARITHRIAN                      = 39746,
    NPC_SAVIANA                         = 39747,
    NPC_HALION                          = 39863
};

enum eRubySpells
{
    //Baltharus
    SPELL_BLADE_TEMPEST                 = 75125,
    SPELL_REPELLING_WAVE                = 74509,
    SPELL_ENERVATING_BRAND              = 74502,
    SPELL_SIPHONED_MIGHT                = 74507,
    SPELL_CLEAVE                        = 40504,
    //Zarithrian
    SPELL_CLEAVE_ARMOR                  = 74367,
    SPELL_INTIMIDATING_ROAR             = 74384,
    //Saviana
    SPELL_ENRAGE                        = 78722,
    SPELL_FLAME_BREATH                  = 74403,
    SPELL_CONFLAGRATION                 = 74452,
    //Halion http://ptr.wowhead.com/npc=39863
    SPELL_TWILIGHT_PRECISION            = 78243,
    SPELL_10_FLAME_BREATH               = 74525,
    SPELL_25_FLAME_BREATH               = 74527,
    SPELL_METEOR_STRIKE                 = 75877, //Possible needs a spellfunc implementation
    SPELL_FIERY_COMBUSTION              = 74562, //Possible needs a spellfunc implementation
    SPELL_MARK_OF_COMBUSTION            = 74567, //Possible needs a spellfunc implementation
    SPELL_10_COMBUSTION                 = 74630, //same :/
    SPELL_25_COMBUSTION                 = 75882, //same again :/
    SPELL_FIERY_COMBUSTION_KNOCKBACK    = 74607, //same again, fuck. No way to shorter defines, I love long spell defines.
    SPELL_10_DARK_BREATH                = 74806,
    SPELL_25_DARK_BREATH                = 75954,
    SPELL_10_DUSK_SHROUD                = 75484,
    SPELL_25_DUSK_SHROUD                = 75485,
    SPELL_TWILIGHT_CUTTER               = 77844,
    SPELL_SOUL_CONSUMPTION              = 74792, //Possible needs a spellfunc implementation
    SPELL_MARK_OF_CONSUMPTION           = 74795, //Possible needs a spellfunc implementation
    SPELL_10_CONSUMPTION                = 74802, //Possible needs a spellfunc implementation
    SPELL_25_CONSUMPTION                = 75874, //Possible needs a spellfunc implementation
    SPELL_SOUL_CONSUMPTION_PULL         = 74799, //Possible needs a spellfunc implementation
    SPELL_CORPOREALITY                  = 74826 //Possible needs a spellfunc implementation. SOMEHOW!
};

class BaltharusAI : public MoonScriptBossAI
{
	MOONSCRIPT_FACTORY_FUNCTION( BaltharusAI, MoonScriptCreatureAI );
	BaltharusAI( Creature *pCreature );

    void AIUpdate();

    bool bThreeClones;
    SpellDesc *pRepellingWave, *pEnervatingBrand, *pSiphonedMight;
    MoonScriptCreatureAI *mClone;
};

void SpellFunc_Enervating_Brand(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType)
{
	BaltharusAI *pBaltharus = (pCreatureAI != NULL) ? (BaltharusAI*)pCreatureAI : NULL;
	if (pBaltharus != NULL)
	{
        pBaltharus->GetUnit()->CastSpell(pTarget, SPELL_ENERVATING_BRAND, false);
        pBaltharus->CastSpell(pBaltharus->pSiphonedMight);
        Player* Jerk;
		for (PlayerStorageMap::iterator itr = pTarget->GetMapMgr()->m_PlayerStorage.begin(); itr != pTarget->GetMapMgr()->m_PlayerStorage.end(); ++itr)
		{            
			Jerk = itr->second;
            if (Jerk == NULLPLR) continue;
            if ((pTarget->CalcDistance(TO_OBJECT(Jerk))) <= 12.0f)
            {
                pBaltharus->GetUnit()->CastSpell(Jerk, SPELL_ENERVATING_BRAND, false);
                pBaltharus->CastSpell(pBaltharus->pSiphonedMight);
            }
               
        }
    }
};

class BaltharusCloneAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION( BaltharusCloneAI, MoonScriptCreatureAI );
	BaltharusCloneAI( Creature *pCreature );
};

class ZarithrianAI : public MoonScriptBossAI //He spawn 2 adds. http://ptr.wowhead.com/npc=39746
{
	MOONSCRIPT_FACTORY_FUNCTION( ZarithrianAI, MoonScriptCreatureAI );
	ZarithrianAI( Creature *pCreature );
};

class SavianaAI : public MoonScriptBossAI //AddSpellFunc for conflagration. http://ptr.wowhead.com/spell=74456
{
	MOONSCRIPT_FACTORY_FUNCTION( SavianaAI, MoonScriptCreatureAI );
	SavianaAI( Creature *pCreature );
    
    SpellDesc *pEnrage, *pConflagration;
};

void SpellFunc_Conflagration(SpellDesc* pThis, MoonScriptCreatureAI* pCreatureAI, Unit *pTarget, TargetType pType)
{
    SavianaAI *pSaviana = (pCreatureAI != NULL) ? (SavianaAI*)pCreatureAI : NULL;
	if (pSaviana != NULL)
	{
        float x = pSaviana->GetUnit()->GetSpawnX();
        float y = pSaviana->GetUnit()->GetSpawnY();
        float z = pSaviana->GetUnit()->GetSpawnZ();
        pSaviana->MoveTo(x,y,z, true);
        pSaviana->CastSpell(pSaviana->pConflagration);
        pSaviana->CastSpell(pSaviana->pConflagration);
        pSaviana->CastSpell(pSaviana->pConflagration);
        pSaviana->CastSpell(pSaviana->pEnrage);
    }

};
/*
class HalionAI : public MoonScriptBossAI
{
    
    This boss gotta be pain in my ass. 
    If raid doesn't got pawned lieutenants, this boss comes not attackable or better not spawned. //Dunno again, maybe false.
    
	MOONSCRIPT_FACTORY_FUNCTION( HalionAI, MoonScriptCreatureAI );
    HalionAI( Creature *pCreature ) : MoonScriptBossAI(pCreature){}
    void OnCombatStart(Unit *mTarget);
    void OnCombatStop(Unit *mTarget);
    void AIUpdate();
};
*/

void SetupTheRubySanctum(ScriptMgr* mgr)
{
    mgr->register_creature_script(NPC_BALTHARUS, &BaltharusAI::Create);
    mgr->register_creature_script(NPC_BALTHARUSCLONE, &BaltharusCloneAI::Create);
    mgr->register_creature_script(NPC_ZARITHRIAN, &ZarithrianAI::Create);
    mgr->register_creature_script(NPC_SAVIANA, &SavianaAI::Create);
    //mgr->register_creature_script(NPC_HALION, &HalionAI::Create);
};