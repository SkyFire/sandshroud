/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"
#include "Setup.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

class SCRIPT_DECL SkillTrainer : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player * Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		uint32 TextID = 3;
		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

		Menu->AddItem(9, "Help me Prepare.", 1);
		Menu->AddItem(0, "I must leave.", 2);

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
			{
				Plr->_AdvanceAllSkills(400, true);
				TO_CREATURE(pObject)->SendChatMessageToPlayer(CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, "I have helped you as best I can at the moment. Be safe, young one.", Plr);
				Plr->Gossip_Complete();
			}break;

		case 2:
			{
				TO_CREATURE(pObject)->SendChatMessageToPlayer(CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, "Be safe, young one. The world is very dangerous.", Plr);
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

void SetupSkillTrainer(ScriptMgr * mgr)
{
	GossipScript * st = ((GossipScript*)new SkillTrainer);
	mgr->register_gossip_script(54006, st);
}
