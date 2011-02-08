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

class WintergraspEngineers : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 32000, Plr);
		if(Plr->HasAura(55629))
		{
			Menu->AddItem(0, "Build Catapult", 1);
			Menu->AddItem(0, "Build Demolisher", 2);
			Menu->AddItem(0, "Build Siege Engine", 3);
		}
		else if(Plr->HasAura(33280))
			Menu->AddItem(0, "Build Catapult", 1);

		Menu->AddItem(0, "Nevermind", 4);
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		switch(IntId)
		{
		case 1:	// Build Catapult
			{
				// Info: 56663 http://www.wowhead.com/spell=56663
				plr->CastSpell(plr, 56663, false);
				plr->Gossip_Complete();
			}break;
		case 2:	// Build Demolisher
			{
				// Info: 56575 http://www.wowhead.com/spell=56575
				plr->CastSpell(plr, 56575, false);
				plr->Gossip_Complete();
			}break;
		case 3:	// Build Siege Engine
			{
				// Info: (61408/56661) http://www.wowhead.com/spell=61408 / http://www.wowhead.com/spell=56661
				if(plr->GetTeam())
					plr->CastSpell(plr, 61408, false); // Horde
				else
					plr->CastSpell(plr, 56661, false); // Alliance

				plr->Gossip_Complete();
			}break;
		case 4:
			{
				plr->Gossip_Complete();
			}break;
		}
	}
};

void WintergraspInternal::WintergraspNPCAIRegister()
{
	GossipScript * WintergraspEngineer = (GossipScript*) new WintergraspEngineers;
	sScriptMgr.register_gossip_script(30400, WintergraspEngineer);
	sScriptMgr.register_gossip_script(30499, WintergraspEngineer);
}
