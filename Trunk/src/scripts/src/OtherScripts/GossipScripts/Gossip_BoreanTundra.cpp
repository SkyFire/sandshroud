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

class TiareGossipScript : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
		Menu->AddItem(0, "Teleport me to Amber Ledge!", 1); 
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object *pObject, Player *plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature *Tiare = TO_CREATURE((pObject));		
		plr->Gossip_Complete();
		Tiare->CastSpell(plr, dbcSpell.LookupEntry(50135), true);
	}			
	void Destroy()
    {
        delete this;
	}

};

class SurristraszGossipScript : public GossipScript
{
public:
    void GossipHello(Object *pObject, Player *plr, bool AutoSend)
    {
        GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
        Menu->AddItem( 0, "I'd like passage to the Transitus Shield.", 0);
		Menu->AddItem( 0, "May I use a drake to fly elsewhere?", 1);
		Menu->SendTo(plr);
    }

    void GossipSelectOption(Object *pObject, Player *plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
		Creature *pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULLCREATURE;
		if(pCreature==NULLCREATURE)
			return;

        switch(IntId)
        {
        case 0:
			{
				TaxiPath * path = sTaxiMgr.GetTaxiPath( 795 ); // Flight Path
				plr->TaxiStart( path, 25854, 0 ); // Dragon
            }break;
		case 1:
			{
				plr->GetSession()->SendTaxiList(pCreature);
            }break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupBoreanTundraGossip(ScriptMgr * mgr)
{
	GossipScript * TiareGossip = (GossipScript*) new TiareGossipScript;
	mgr->register_gossip_script(30051, TiareGossip);		// Tiare
	GossipScript * SurristraszGossip = (GossipScript*) new SurristraszGossipScript;
	mgr->register_gossip_script(24795, SurristraszGossip);		// Surristrasz
}