/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

/************************************************************************/
/* GENERAL GUARD SCRIPT                                                 */
/************************************************************************/

// Covers *all* guard types, scripting their texts to guide players around.
// Enable this define to make all gossip texts have a "back" / "I was looking
// for somethinge else" button added.

#define BACK_BUTTON

#ifdef BACK_BUTTON

// Make code neater with this define.
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

#else

// Make code neater with this define.
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->AddItem(0, "I was looking for something else.", 0); \
    Menu->SendTo(Plr);

#endif

class SCRIPT_DECL HordeMallGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player*  Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
		
		Menu->AddItem(0, "Simple Pointer", 1);
		Menu->AddItem(0, "List in List", 2);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {// Gossip_SendPOI(1595.64 X, 232.45 Y, 7(Red Flag) Icon, 6(Probably Trainers) Flags, 0 Data, "1" Name)
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;
			
		case 1:     // Simple Pointer
            SendQuickMenu(1);
            Plr->Gossip_SendPOI(0, 0, 0, 0, 0, "1");
            break;

        case 2:    // List in List
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, Plr);
                Menu->AddItem( 0, "Arch-Mage Vane(Teleporter)", 3);
                Menu->AddItem( 0, "2", 4);
                Menu->AddItem( 0, "3", 5);
                Menu->AddItem( 0, "4", 6);
                Menu->AddItem( 0, "5", 7);
                Menu->AddItem( 0, "6", 8);
                Menu->SendTo(Plr);
            }break;

        case 3: // 1
            {
                Plr->Gossip_SendPOI(2756.59f, 6275.67f, 7, 1, 0, "Arch-Mage Vane");
                SendQuickMenu(1);
            }break;

        case 4: // 2
            {
                Plr->Gossip_SendPOI(0, 0, 1, 1, 0, "4");
                SendQuickMenu(1);
            }break;

        case 5: // 3
            {
                Plr->Gossip_SendPOI(0, 0, 2, 2, 0, "5");
                SendQuickMenu(1);
            }break;

        case 6: // 4
            {
                Plr->Gossip_SendPOI(0, 0, 3, 3, 0, "6");
                SendQuickMenu(1);
            }break;

        case 7: // 5
            {
                Plr->Gossip_SendPOI(0, 0, 4, 4, 0, "7");
                SendQuickMenu(1);
            }break;

        case 8: // 6
            {
                Plr->Gossip_SendPOI(0, 0, 5, 5, 0, "8");
                SendQuickMenu(1);
            }break;
        }
    }
};

void SetupGuardGossip(ScriptMgr * mgr)
{
	GossipScript * hordemall = (GossipScript*) new HordeMallGuard();
	mgr->register_gossip_script(14304, hordemall);
}