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

#include "StdAfx.h"
#include "../Setup.h"



#ifdef OldKingdom
enum bosses
{
	NPC_NADOX					= 29309,
	NPC_TALDARAM				= 29308,
	NPC_SHADOWSEEKER			= 29310,
	NPC_VOLAZI					= 29311,
	NPC_AMANITAR				= 30258
};

enum spell
{	//Nadox spells.
	SPELL_BROOD_PLAGUE			= 56130,
	SPELL_BERSER				= 26662,
	SPELL_GUARDIAN_AURA			= 56153,
	H_SPELL_BROOD_PLAGUE		= 59467,
	H_SPELL_BROOD_RAGE			= 59465

};

class NadoxAI : public MoonScriptCreatureAI
{
public:
	MOONSCRIPT_FACTORY_FUNCTION( NadoxAI, MoonScriptCreatureAI );
	NadoxAI( Creature *pCreature ) : MoonScriptCreatureAI( pCreature )
	{
		if(IsHeroic()){
			AddSpell(H_SPELL_BROOD_PLAGUE, Target_Current, 15, 0, 30, 0.0f, 5.0f);
			pBloodRage = AddSpell(H_SPELL_BROOD_RAGE, Target_Current, 0, 0, 90, 0.0f, 40.0f);
		}
		else
			AddSpell(SPELL_BROOD_PLAGUE, Target_Current, 15, 0, 60, 0.0f, 5.0f);
			
	};

	void AIUpdate()
	{
		
		ParentClass::AIUpdate();
	}
private:
	SpellDesc *pBloodRage;
};

#endif