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


enum eKingdomNPC
{
	NPC_NADOX					= 29309,
    NPC_SWARMER                 = 30178,
    NPC_GUARDIAN                = 30176,
	NPC_TALDARAM				= 29308,
	NPC_SHADOWSEEKER			= 29310,
	NPC_VOLAZI					= 29311,
	NPC_AMANITAR				= 30258
};

enum eKingdomSpell
{	//Nadox spells.
	SPELL_BROOD_PLAGUE			= 56130,
	SPELL_BERSERK				= 26662,
	SPELL_GUARDIAN_AURA			= 56153,
	H_SPELL_BROOD_PLAGUE		= 59467,
	SPELL_BROOD_RAGE			= 59465,
    //Taldaram
    SPELL_BLOODTHIRST           = 55968,
    SPELL_VANISH                = 55964,
    SPELL_EMBRACE               = 55959,
    H_SPELL_EMBRACE             = 59513,
    SPELL_SEAR_BEAM             = 57748, //Flame orb
    H_SPELL_SEAR_BEAM           = 58938

};

class NadoxAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( NadoxAI, MoonScriptCreatureAI );
	NadoxAI( Creature *pCreature );

    void OnCombatStart(Unit *pTarget);
	void AIUpdate();
private:
    MoonScriptCreatureAI *mSwarm, *mGuard;
    LocationVector mLoc;
    UnitArray Swarmers;
	SpellDesc *pBerserk, *pGuardA;
    int32 mSwarmerTimer;
};


enum eSphere
{
    GO_SPHERE1                                    = 193093,
    GO_SPHERE2                                    = 193094
};
/*
class TaldaramAI : public MoonScriptBossAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( TaldaramAI, MoonScriptBossAI );
	TaldaramAI( Creature *pCreature );

private:
    SpellDesc *pVanish, pEmbrace;

};*/

void SetupTheOldKingdom(ScriptMgr * mgr)
{
    mgr->register_creature_script(NPC_NADOX, &NadoxAI::Create);
}