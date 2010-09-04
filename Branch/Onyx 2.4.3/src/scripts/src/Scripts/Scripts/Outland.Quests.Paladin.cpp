/*
 * Crow Scripts! By Crow@Sandshroud(Thetruecrow)
 * 
 */

#include "StdAfx.h"
#include "../Setup.h"

// Quest: The First Trial.

class GildedBraizerAI : public GameObjectAIScript
{
public:
	ADD_GAMEOBJECT_FACTORY_FUNCTION(GildedBraizerAI);

	void OnActivate(Player *pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(9678);
		if(qle && !qle->CanBeFinished())
		{
			qle->SetMobCount(0, 1);
			qle->SendUpdateAddKill(1);
			qle->UpdatePlayerFields();
			qle->SendQuestComplete();
		}
	}
};

void SetupPaladinQuests(ScriptMgr * mgr)
{
    mgr->register_gameobject_script(181956, &GildedBraizerAI::Create);
}
