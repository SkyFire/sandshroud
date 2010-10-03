#include "StdAfx.h"

class WintergraspEngineers : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 32000, Plr);
		if(Plr->HasAura(33280))
			Menu->AddItem(0, "Build Catapult", 1);
		if(Plr->HasAura(55629))
		{
			Menu->AddItem(0, "Build Catapult", 1);
			Menu->AddItem(0, "Build Demolisher", 2);
			Menu->AddItem(0, "Build Siege Engine", 3);
		}
		Menu->AddItem(0, "Nevermind", 4);
		Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
    {
        switch(IntId)
        {
		case 1:
			{
				plr->Gossip_Complete();
			}break;
		case 2:
			{
				plr->Gossip_Complete();
			}break;
		case 3:
			{
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
