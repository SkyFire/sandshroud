/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"
#include "Setup.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

class SCRIPT_DECL Warmage : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 TextID = 12;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

		if(Plr->GetAreaID() != 4129 && Plr->GetAreaID() != 3998)
			Menu->AddItem(6, "Stronghold(Mall)", 1);

		Menu->AddItem(9, "Sholazar Basin", 2);

		Menu->AddItem(9, "Icecrown", 3);

		Menu->AddItem(9, "Storm Peaks", 4);

		Menu->AddItem(9, "Zul'Drak", 5);

		Menu->AddItem(9, "Grizzly Hills", 6);

		Menu->AddItem(9, "Dragonblight", 7);

		Menu->AddItem(0, "Goodbye.", 100);

		if(AutoSend)
			Menu->SendTo(Plr);
	}

    void GossipSelectOption(Object * pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		printf("IntId: %u\n", IntId);
		switch(IntId)
        {
		case 0:
			GossipHello(pObject, Plr, true);
			break;

		case 1:
			{
				if(Plr->GetTeam() == TEAM_ALLIANCE)
				{
					Plr->EventTeleport(571, 1347.1f, -3159.0f, 171.0f, 2.1f); // Alliance Mall
				}
				if(Plr->GetTeam() == TEAM_HORDE)
				{
					Plr->EventTeleport(571, 2822.6f, 6172.5f, 85.6f, 0.75f); // Horde Mall
				}
			}break;

		case 2: // Sholozar Basin
			{
				GossipMenu *Menu;
				uint32 TextID = 12;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

				Plr->CurrentGossipMenu = Menu;
				Menu->AddItem(9, "Teleport me to the first base", 10);
				Menu->AddItem(9, "Teleport me to the second base", 11);
				Menu->AddItem(0, "Show me the list again.", 0);

				Menu->SendTo(Plr);
			}break;

		case 3:
			{
				GossipMenu *Menu;
				uint32 TextID = 12;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

				Menu->AddItem(9, "Teleport me to the first base", 0);
				Menu->AddItem(9, "Teleport me to the second base", 0);
				Menu->AddItem(0, "Show me the list again.", 0);

				Menu->SendTo(Plr);
			}break;

		case 4:
			{
				GossipMenu *Menu;
				uint32 TextID = 12;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

				Menu->AddItem(9, "Teleport me to the first base", 0);
				Menu->AddItem(9, "Teleport me to the second base", 0);
				Menu->AddItem(0, "Show me the list again.", 0);

				Menu->SendTo(Plr);
			}break;

		case 5:
			{
				GossipMenu *Menu;
				uint32 TextID = 12;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

				Menu->AddItem(9, "Teleport me to the first base", 0);
				Menu->AddItem(9, "Teleport me to the second base", 0);
				Menu->AddItem(0, "Show me the list again.", 0);

				Menu->SendTo(Plr);
			}break;

		case 6:
			{
				GossipMenu *Menu;
				uint32 TextID = 12;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

				Menu->AddItem(9, "Teleport me to the first base", 0);
				Menu->AddItem(9, "Teleport me to the second base", 0);
				Menu->AddItem(0, "Show me the list again.", 0);

				Menu->SendTo(Plr);
			}break;

		case 7:
			{
				GossipMenu *Menu;
				uint32 TextID = 12;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

				Menu->AddItem(9, "Teleport me to the first base", 0);
				Menu->AddItem(9, "Teleport me to the second base", 0);
				Menu->AddItem(0, "Show me the list again.", 0);

				Menu->SendTo(Plr);
			}break;

		case 10:
			{
				if(Plr->GetTeam() == TEAM_ALLIANCE)
				{
					Plr->EventTeleport(571, 5017.05f, 4297.66f, -64.59f, 4.50f);
				}
				if(Plr->GetTeam() == TEAM_HORDE)
				{
					Plr->EventTeleport(571, 6353.12f, 4317.39f, -25.44f, 3.37f);
				}
			}break;

		case 11:
			{
				if(Plr->GetTeam() == TEAM_ALLIANCE)
				{
					Plr->EventTeleport(571, 6495.75f, 4633.58f, -44.54f, 2.1f);
				}
				if(Plr->GetTeam() == TEAM_HORDE)
				{
					Plr->EventTeleport(571, 5292.15f, 5801.86f, -58.68f, 5.6f);
				}
			}break;

		case 100:
			Plr->Gossip_Complete();
			break;
		}

		GossipEnd(pObject, Plr);
	}

	void GossipEnd(Object * pObject, Player * Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

    void Destroy()
    {
		delete this;
    }
};

void SetupWarmage(ScriptMgr* mgr)
{
	GossipScript * tl = (GossipScript*)new Warmage;
	mgr->register_gossip_script(54007, tl);
	mgr->register_gossip_script(54008, tl);
}
