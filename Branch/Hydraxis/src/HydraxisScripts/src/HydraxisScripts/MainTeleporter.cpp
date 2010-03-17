/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"
#include "Setup.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
	Menu->SendTo(Plr);

class SCRIPT_DECL MainTeleporter : public GossipScript
{
public:
	void GossipHello(Object * pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 TextID = 12;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

		if(Plr->GetTeam() == 0) // Alliance
		{ // There area ZoneId checks, no point in going back if your already there.
			if(Plr->GetZoneId() != 495)
			{
				Menu->AddItem(6, "Stronghold(Mall)", 1);
			}
			if(Plr->GetZoneId() == 495) // Alliance Mall Center
			{
				Menu->AddItem(6, "Center Room(Mall)", 3);
			}
			if(Plr->GetZoneId() != 357)
			{
				Menu->AddItem(1, "Staging Area(Starting Area)", 20);
			}
		}

		if ( Plr->GetTeam() == 1 ) // Horde
		{ // There area ZoneId checks, no point in going back if your already there.
			if(Plr->GetZoneId() != 3537)
			{
				Menu->AddItem(6, "Stronghold(Mall)", 2);
			}
			if(Plr->GetZoneId() == 3537)
			{
				Menu->AddItem(1, "Downstairs(Mall)", 4);
			}
			if(Plr->GetZoneId() != 267)
			{
				Menu->AddItem(1, "Staging Area(Starting Area)", 21);
			}
		}

		if(Plr->GetZoneId() != 4197)
		{
			Menu->AddItem(9, "Wintergrasp", 18);
		}

		if(Plr->GetZoneId() != 33)
		{
			Menu->AddItem(9, "Gurubashi Arena", 22);
		}

		if(Plr->GetZoneId() != 618)
		{
			Menu->AddItem(9, "Winterspring Event", 23);
		}

		if(Plr->GetZoneId() != 2817)
		{
			Menu->AddItem(9, "Crystalsong Event", 24);
		}

		if(Plr->GetZoneId() != 4197)
		{
			Menu->AddItem(4, "Dalaran", 25);
		}

		Menu->AddItem(0, "Goodbye.", 26);

		if(AutoSend)
			Menu->SendTo(Plr);
	}

	void GossipSelectOption(Object * pObject, Player * Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		switch(IntId)
		{
		case 0:
			GossipHello(pObject, Plr, true);
			break;
		case 1:
			Plr->EventTeleport(571, 1428.2f, -3321.7f, 178.7f, 5.2f); // Alliance Mall
			break;
		case 2:
			Plr->EventTeleport(571, 2762.9f, 6268.6f, 208.5f, 5.4f); // Horde Mall
			break;
		case 3:
			Plr->EventTeleport(571, 1347.1f, -3159.0f, 171.0f, 2.1f); // Alliance Mall Center
			break;
		case 4:
			Plr->EventTeleport(571, 2822.6f, 6172.5f, 85.6f, 0.75f); // Horde Mall Downstairs
			break;
		case 18:
			{
				if(Plr->GetTeam() == 0) // Alliance
				{ 
					Plr->EventTeleport(571, 5086.6f, 2180.0f, 365.6f, 0.9f);
				}
				if(Plr->GetTeam() == 1) // Horde
				{ 
					Plr->EventTeleport(571, 5040.8f, 3675.6f, 363.1f, 3.14f);
				}
			}break;
		case 20:
			Plr->EventTeleport(1, -4472.8f, 3311.4f, 14.8f, 2.5f); // Alliance Starting Area
			break;
		case 21:
			Plr->EventTeleport(0, -1326.9f, 550.3f, 101.0f, 2.3f); // Horde Starting Area
			break;
		case 22:
			switch(rand()%101)
			{
			case 1:
				{
					Plr->EventTeleport(0, -13200.579102f, 283.729828f, 30.491329f, 4.227683f); // Right in the center, unlucky bloke.
				}break;
			default:
				{
					switch(rand()%5)
					{
					case 1:
						{
							Plr->EventTeleport(0, -13226.362305f, 231.975830f, 34.285782f, 1.102694f); // Entrance
						}break;
					case 2:
						{
							Plr->EventTeleport(0, -13174.314453f, 312.758820f, 34.242992f, 4.081713f); // Across from the Entrance
						}break;
					case 3:
						{
							Plr->EventTeleport(0, -13255.584961f, 284.184814f, 34.242607f, 6.117346f);
						}break;
					case 4:
						{
							Plr->EventTeleport(0, -13167.706055f, 237.680695f, 34.241318f, 2.342727f);
						}break;
					}
				}
			}break;

		case 23:
			{
				if ( Plr->GetTeam() == 1 ) // Horde
				{
					Plr->EventTeleport(1, 6715.829102f, -4673.578590f, 720.998657f, 1.329695f);
					Plr->SendAreaTriggerMessage("Event not setup yet. Teleported to Everlook");
				}
				else // For now, send to the same area.
				{
					Plr->EventTeleport(1, 6715.829102f, -4673.578590f, 720.998657f, 1.329695f);
					Plr->SendAreaTriggerMessage("Event not setup yet. Teleported to Everlook");
				}
			}break;

		case 24:
			{
				if ( Plr->GetTeam() == 1 ) // Horde
				{
					Plr->EventTeleport(571, 5807.830566f, 588.148682f, 660.939270f, 1.673696f);
					//Plr->EventTeleport(571, 5612.302734f, -688.100342f, 206.626038f, 3.626979f);
					Plr->SendAreaTriggerMessage("Event not setup yet. Teleported to Dalaran");
				}
				else // For now, send to the same area.
				{
					Plr->EventTeleport(571, 5807.830566f, 588.148682f, 660.939270f, 1.673696f);
					//Plr->EventTeleport(571, 5031.252930f, -519.304504f, 226.279648f, 0.241128f);
					Plr->SendAreaTriggerMessage("Event not setup yet. Teleported to Dalaran");
				}
			}break;

		case 25:
			{
				Plr->EventTeleport(571, 5807.830566f, 588.148682f, 660.939270f, 1.673696f);
			}break;

		case 26:
			{
				Plr->Gossip_Complete();
			}break;
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

void SetupMainTeleporter(ScriptMgr * mgr)
{
	GossipScript * tl = (GossipScript*)new MainTeleporter;
	mgr->register_gossip_script(54000, tl);
	mgr->register_gossip_script(54001, tl);
}
