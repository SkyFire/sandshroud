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
#include "Setup.h"
#include "../Common/EasyFunctions.h"

class WoodlandWalker : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(WoodlandWalker);
	WoodlandWalker(Creature *pCreature) : CreatureAIScript(pCreature)
	{
		pCreature->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 35 );
	}
};

class WoodlandWalkerGossip : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *plr, bool AutoSend)
	{
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULLCREATURE;
		if( pCreature == NULL )
			return;

		uint32 chance = RandomUInt(1);
		if( chance == 0 )
		{
			pCreature->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 14 );
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The Woodlands Walker is angered by your request and attacks!" );
		}
		else
		{
			sEAS.AddItem( 36786, plr );
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Breaking off a piece of its bark, the Woodlands Walker hands it to you before departing." );
		}

	}

	void Destroy()
	{
		delete this;
	};
};

class WrathGateQuestCinema : public QuestScript
{	
public:
  void OnQuestComplete(Player *mTarget, QuestLogEntry *qLogEntry)
  {
	if( mTarget == NULL )
		return;

	// send packet for movie
	uint32 id = 14;	
	mTarget->GetSession()->OutPacket(SMSG_TRIGGER_MOVIE, sizeof(uint32), &id);
  }
};

void SetupDragonblight(ScriptMgr * mgr)
{
	mgr->register_creature_script(26421, &WoodlandWalker::Create);

	GossipScript * WW = (GossipScript*) new WoodlandWalkerGossip();
	mgr->register_gossip_script(26421, WW);
	
	QuestScript *WrathGateCinema = (QuestScript*) new WrathGateQuestCinema();
	mgr->register_quest_script(12499, WrathGateCinema);
	mgr->register_quest_script(12500, WrathGateCinema);
}