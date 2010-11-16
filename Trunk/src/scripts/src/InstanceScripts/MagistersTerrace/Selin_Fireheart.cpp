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
#include "Magisters_Terrace_InstanceScript.h"

#define TRASH_FelCrystal 24722
#define BOSS_SelinFireheart 24723
#define FC_MANARAGE 44320
#define FC_MANARAGE_TRIGGER 44321
#define SF_FELEXPLOSION 44314
#define SF_DRAINLIFE 44294
#define SF_DRAINMANA 46153

// Fel Crystal Spawn Locations
static Coords FelCrystals[]={
	{225.969f, -20.0775f, -2.9731f, 0.942478f, TRASH_FelCrystal},
	{226.314f, 20.2183f, -2.98127f, 5.32325f, TRASH_FelCrystal},
	{247.888f, -14.6252f, 3.80777f, 2.33874f, TRASH_FelCrystal},
	{248.053f, 14.592f, 3.74882f, 3.94444f, TRASH_FelCrystal},
	{263.149f, 0.309245f, 1.32057f, 3.15905f, TRASH_FelCrystal}
};

class SelinFireheartAI : public MoonScriptCreatureAI
{
	MOONSCRIPT_FACTORY_FUNCTION(SelinFireheartAI, MoonScriptCreatureAI);
	SelinFireheartAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
	{
		AddSpell(SF_DRAINLIFE, Target_RandomPlayer, 8, 0, 35);

		if(_unit->GetMapMgr()->iInstanceMode == MODE_5PLAYER_HEROIC)
			AddSpell(SF_DRAINMANA, Target_RandomPlayer, 8, 0, 35);

		ManaRage = dbcSpell.LookupEntry(FC_MANARAGE);
		ManaRageTrigger = AddSpell(FC_MANARAGE_TRIGGER, Target_Self, 0, 0, 0);
		FelExplosion = AddSpell(SF_FELEXPLOSION, Target_Self, 0, 0, 0);
	}

	void OnCombatStart(Unit *pTarget)
	{
		/*
			Selin Fireheart starts with 0 mana and drains it from the felcrystals in the room
			- ToDo: Set it so mana regen is off
		*/
		_unit->SetUInt32Value(UNIT_FIELD_POWER1, 0);
		ParentClass::OnCombatStart(pTarget);
	}

	/*
		During the AIUpdate() Selin will spam FelExplosion until hes out of mana
		He will then attempt to gain mana from a FelCrystal thats in the room by running to them
	*/
	void AIUpdate()
	{
		// 10% of his mana according to wowhead is 3231 which is whats needed to cast FelExplosion
		if(GetManaPercent() < 10 || FelExplosion->mEnabled == false)
			Mana();
		else if(!IsCasting())// Mana is greater than 10%
			CastFelExplosion();

		ParentClass::AIUpdate();
	}

	void Mana()
	{
		/*
			Attempt to get a Fel Crystal and move to it if not in range.
			Once in range we get the FelCrystal to cast Mana Rage on Selin
		*/
		Unit *FelCrystal = NULLUNIT;
		PreventActions(false);

		FelCrystal = FindFelCrystal();

		if(!FelCrystal || !FelCrystal->isAlive())
		{
			PreventActions(true);
			FelCrystal = NULLUNIT;
			return;
		}

		// Not in range
		if(_unit->GetDistance2dSq(FelCrystal) > 100)
		{
			MoveTo(FelCrystal->GetPositionX(), FelCrystal->GetPositionY(), FelCrystal->GetPositionZ());
			FelCrystal = NULLUNIT;
			return;
		}

		_unit->GetAIInterface()->StopMovement(0);

		if(!FelCrystal->GetCurrentSpell())
			FelCrystal->CastSpell(_unit, ManaRage, false);

		// Mana Rage giving of mana doesnt work so we give 10%(3231) / AIUpdate() Event.
		CastSpellNowNoScheduling(ManaRageTrigger);
		uint32 mana = _unit->GetUInt32Value(UNIT_FIELD_POWER1)+3231;
		if(mana >= _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1))
			mana = _unit->GetUInt32Value(UNIT_FIELD_MAXPOWER1);

		_unit->SetUInt32Value(UNIT_FIELD_POWER1, mana);

		// Re-Enable FelExplosion
		if(GetManaPercent() >= 100)
			PreventActions(true);

		FelCrystal = NULLUNIT;
	}

	void PreventActions(bool Allow)
	{
		FelExplosion->mEnabled = Allow;
		SetAllowMelee(Allow);
		SetAllowRanged(Allow);
		SetAllowSpell(Allow);
		SetAllowTargeting(Allow);
	}

	Unit *FindFelCrystal()
	{
		/*
			Find a FelCrystal
		*/
		Unit *FC = NULLUNIT;
		for(int x=0; x<5; x++)
		{
			FC = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(FelCrystals[x].mX, FelCrystals[x].mY, FelCrystals[x].mZ, FelCrystals[x].mAddition);
			if(!FC || !FC->isAlive() || FC->GetInstanceID() != _unit->GetInstanceID())
				FC = NULLUNIT;
			else
				break;
		}
		return FC;
	}

	void CastFelExplosion()
	{
		CastSpellNowNoScheduling(FelExplosion);

		// No Idea why the mana isnt taken when the spell is cast so had to manually take it -_-
		_unit->SetUInt32Value(UNIT_FIELD_POWER1, _unit->GetUInt32Value(UNIT_FIELD_POWER1) - 3231);
	}

	void Destroy()
	{
		delete this;
	};
	
	SpellEntry* ManaRage;
	SpellDesc* ManaRageTrigger;
	SpellDesc* FelExplosion;
};

void SetupSelinFireheart(ScriptMgr* pScriptMgr) 
{
	pScriptMgr->register_creature_script(BOSS_SelinFireheart, &SelinFireheartAI::Create);
}
