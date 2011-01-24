/*
 * Lacrimi Scripts Copyright 2010 - 2011
 *
 * ############################################################
 * # ##            #       ####### ####### ##    #    #    ## #
 * # ##           ###      ##      ##   ## ##   ###  ###   ## #
 * # ##          ## ##     ##      ##   ## ##   ###  ###   ## #
 * # ##         #######    ##      ####### ##  ## #### ##  ## #
 * # ##        ##     ##   ##      #####   ##  ## #### ##  ## #
 * # ##       ##       ##  ##      ##  ##  ## ##   ##   ## ## #
 * # ####### ##         ## ####### ##   ## ## ##   ##   ## ## #
 * # :::::::.::.........::.:::::::.::...::.::.::...::...::.:: #
 * ############################################################
 *
 */

#include "LacrimiStdAfx.h"

/********************************************
 * NPC
 * Ghamoora
 * ID: 4887
 ********************************************/
GhamooraAI::GhamooraAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	// Trample
	AddSpell(5568, Target_Self, 12, 0, 0);
}
/********************************************/

/********************************************
 * NPC
 * Lady Sarevess
 * ID: 4831
 ********************************************/
LadySarevessAI::LadySarevessAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	// Forked Lightning
	AddSpell(8435, Target_Current, 10, 2, 0);
	// Frost Nova
	AddSpell(865, Target_Self, 15, 0, 25);
	// Slow
	AddSpell(246, Target_Current, 15, 0, 10);
	AddEmote(Event_OnCombatStart, "You should not be here! Slay them!", Text_Yell, 5799);
}
/********************************************/

/********************************************
 * NPC
 * Gelihast
 * ID: 6243
 ********************************************/
GelihastAI::GelihastAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	// Net
	AddSpell(6533, Target_RandomPlayer, 10, 0, 0);
}
/********************************************/

/********************************************
 * NPC
 * Lorgus Jett
 * ID: 12902
 ********************************************/
LorgusJettAI::LorgusJettAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature) 
{
	// Lightning Bolt
	AddSpell(12167, Target_Current, 20, 3, 0);
}
/********************************************/

/********************************************
 * NPC
 * Baron Aquanis
 * ID: 12876
 ********************************************/
BaronAquanisAI::BaronAquanisAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature) 
{
	// Frostbolt
	AddSpell(15043, Target_Current, 20, 3, 0);
	AggroNearestPlayer();
	SetDespawnWhenInactive(true);
}
/********************************************/

/********************************************
 * NPC
 * Twilight Lord Kelris
 * ID: 4832
 ********************************************/
TwilightLordKelrisAI::TwilightLordKelrisAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell, 5802);
	AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell, 5803);

	// Sleep
	AddSpell(8399, Target_RandomPlayer, 12, 1.3f, 0, 0, 0, false, "Sleep...", Text_Yell, 5804);

	// Mind Blast
	AddSpell(15587, Target_Current, 16, 1.5f, 0);
}
/********************************************/

/********************************************
 * NPC
 * Akumai
 * ID: 4829
 ********************************************/
AkumaiAI::AkumaiAI(Creature *pCreature) : MoonScriptCreatureAI(pCreature)
{
	AddEmote(Event_OnCombatStart, "Who dares disturb my meditation?!", Text_Yell); // Should this really be here?
	AddEmote(Event_OnTargetDied, "Dust to dust.", Text_Yell); // Should this really be here?

	// Frenzied Rage
	AddSpell(3490, Target_Self, 12, 0, 0, 0, 0);

	// Poison Cloud
	AddSpell(3815, Target_Self, 16, 0, 45);
}
/********************************************/

/********************************************
 * NPC GOSSIP
 * Morridune
 * ID: 6729
 ********************************************/
void MorriduneGossip::GossipHello(Object *pObject, Player * plr, bool AutoSend)
{
    GossipMenu *Menu;
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7247, plr);
    
	if(plr->GetTeam() == 0)
		Menu->AddItem( 0, "Please Teleport me to Darnassus.", 1);
	Menu->AddItem( 0, "I wish to leave this horrible place", 2);

    if(AutoSend)
        Menu->SendTo(plr);
}

void MorriduneGossip::GossipSelectOption(Object *pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
{
	switch (IntId)
	{
	case 0:
		GossipHello(pObject, plr, true);
		break;
	case 1:
		// port to Darnassus
		plr->SafeTeleport(1, 0, 9951.52f, 2280.32f, 1341.39f, 0);
		break;
	case 2:
		// Port to entrance
		plr->SafeTeleport(1, 0, 4247.74f, 745.879f, -24.2967f, 4.36996f);
		break;
	}
}

void MorriduneGossip::Destroy()
{
    delete this;
}
/********************************************/

/********************************************
 * GO
 * Fathom Stone
 * ID: 177964
 ********************************************/
FathomStone::FathomStone(GameObject * goinstance) : GameObjectAIScript(goinstance)
{

}

void FathomStone::OnActivate(Player * pPlayer)
{
	if(pPlayer->GetTeam() == 1 && !BaronAquanis) // Horde
	{
		// Spawn Baron Aquanis
		_gameobject->GetMapMgr()->GetInterface()->SpawnCreature(12876, -782.021f, -63.5876f, -45.0935f, -2.44346f, true, false, 0, 0);
		BaronAquanis = true;
	}
}
/********************************************/

void Lacrimi::SetupBlackfathomDeeps()
{
	// NPC Ghamoora
	sMgr->register_creature_script(4887, &GhamooraAI::Create);

	// NPC Lady Sarevess
	sMgr->register_creature_script(4831, &LadySarevessAI::Create);

	// NPC Gelihast
	sMgr->register_creature_script(6243, &GelihastAI::Create);

	// NPC Lorgus Jett
	sMgr->register_creature_script(12902, &LorgusJettAI::Create);

	// NPC Baron Aquanis
	sMgr->register_creature_script(12876, &BaronAquanisAI::Create);

	// NPC Twilight Lord Kelris
	sMgr->register_creature_script(4832, &TwilightLordKelrisAI::Create);

	// NPC Akumai
	sMgr->register_creature_script(4829, &AkumaiAI::Create);

	// NPC GOSSIP Morridune
	sMgr->register_gossip_script(6729, ((GossipScript*)(new MorriduneGossip())));

	// GO Fathom Stone
	sMgr->register_gameobject_script(177964, &FathomStone::Create);
}
