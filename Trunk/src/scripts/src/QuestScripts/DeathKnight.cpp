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

class ScourgeGryphonOne : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *plr, bool AutoSend)
	{
		TaxiPath * path = sTaxiMgr.GetTaxiPath( 1053 );
		plr->TaxiStart( path, 26308, 0 );
	}

	void Destroy()
	{
		delete this;
	};
};

class ScourgeGryphonTwo : public GossipScript
{
public:
	void GossipHello(Object *pObject, Player *plr, bool AutoSend)
	{
		TaxiPath * path = sTaxiMgr.GetTaxiPath( 1054 );
		plr->TaxiStart( path, 26308, 0 );
	}

	void Destroy()
	{
		delete this;
	};
};

#define CN_INITIATE_1				29519
#define CN_INITIATE_2				29565
#define CN_INITIATE_3				29567
#define CN_INITIATE_4				29520

class AcherusSoulPrison : GameObjectAIScript
{
public:
    AcherusSoulPrison(GameObject *goinstance) : GameObjectAIScript(goinstance) {}
    static GameObjectAIScript *Create(GameObject *GO) 
	{ 
	return new AcherusSoulPrison(GO); 
	}

    void OnActivate(Player *pPlayer)
    {
        QuestLogEntry * en = pPlayer->GetQuestLogForEntry(12848);
        if(!en)
        return;
            
            float SSX = pPlayer->GetPositionX();
            float SSY = pPlayer->GetPositionY();
            float SSZ = pPlayer->GetPositionZ();
			float SSO = pPlayer->GetOrientation();
            
            Creature *pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, SSO);
		     
			if(!pCreature || !pCreature->isAlive())
            return;
			
			if (pCreature->GetEntry() == CN_INITIATE_1 || pCreature->GetEntry() == CN_INITIATE_2 || pCreature->GetEntry() == CN_INITIATE_3 || pCreature->GetEntry() == CN_INITIATE_4)
			{
			pPlayer->SendChatMessage(CHAT_MSG_SAY, LANG_UNIVERSAL, "I give you the key to your salvation");
            pCreature->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
            pCreature->GetAIInterface()->SetNextTarget(pPlayer);
            pCreature->GetAIInterface()->AttackReaction(pPlayer, 1, 0);
            pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You have committed a big mistake, demon");
			
			if(en->GetMobCount(0) != 0)
			return;

			en->SetMobCount(0, 1);
			en->SendUpdateAddKill(0);
			en->UpdatePlayerFields();
			}
		
    }
};

bool PreparationForBattle(uint32 i, Spell *pSpell)
{
	if ( pSpell == NULL || pSpell->u_caster == NULL || !pSpell->u_caster->IsPlayer() )
	return true;

	Player *pPlayer = TO_PLAYER( pSpell->u_caster );
	QuestLogEntry *pQuest = pPlayer->GetQuestLogForEntry( 12842 );
	if ( pQuest != NULL )
	{
		if ( pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0] )
		{
			pQuest->SetMobCount( 0, pQuest->GetMobCount( 0 ) + 1 );
			pQuest->SendUpdateAddKill( 0 );
			pQuest->UpdatePlayerFields();
			pQuest->SendQuestComplete();
		}
	}
	return true;
};

void SetupDeathKnight(ScriptMgr * mgr)
{
	GossipScript * SGO = (GossipScript*) new ScourgeGryphonOne();
	mgr->register_gossip_script(29488, SGO);
	GossipScript * SGT = (GossipScript*) new ScourgeGryphonTwo();
	mgr->register_gossip_script(29501, SGT);
	
	mgr->register_gameobject_script(191588, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191577, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191580, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191581, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191582, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191583, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191584, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191585, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191586, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191587, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191589, &AcherusSoulPrison::Create);
    mgr->register_gameobject_script(191590, &AcherusSoulPrison::Create);
	
	mgr->register_dummy_spell(53341, &PreparationForBattle);
	mgr->register_dummy_spell(53343, &PreparationForBattle);
}