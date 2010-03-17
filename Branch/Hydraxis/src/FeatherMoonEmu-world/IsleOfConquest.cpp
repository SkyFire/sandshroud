/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "StdAfx.h"

IsleOfConquest::IsleOfConquest( MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	m_playerCountPerTeam = 40;
	m_reinforcements[0] = IOC_NUM_REINFORCEMENTS;
	m_reinforcements[1] = IOC_NUM_REINFORCEMENTS;
	m_LiveCaptain[0] = true;
	m_LiveCaptain[1] = true;
	m_bonusHonor = HonorHandler::CalculateHonorPointsFormula(lgroup*10,lgroup*10);

//	memset(m_nodes, 0, sizeof(m_nodes)); TODO
}

void IsleOfConquest::Init()
{
	CBattleground::Init();
}

IsleOfConquest::~IsleOfConquest()
{

}

void IsleOfConquest::HookOnPlayerDeath(Player* plr)
{

}

void IsleOfConquest::HookFlagDrop(Player* plr, GameObject* obj)
{

}

void IsleOfConquest::HookFlagStand(Player* plr, GameObject* obj)
{

}

void IsleOfConquest::HookOnMount(Player* plr)
{

}

void IsleOfConquest::HookOnAreaTrigger(Player* plr, uint32 id)
{

}

bool IsleOfConquest::HookHandleRepop(Player* plr)
{
	return false;
}

void IsleOfConquest::OnAddPlayer(Player* plr)
{	
	if(!m_started)
		plr->CastSpell(plr, BG_PREPARATION, true);

	if( plr->GetTeam() )
	{
		plr->SetAtWar( 1050, true );
		plr->SetStanding( 1050, -9000);
		if(plr->GetStanding(1085) < 0)
			plr->SetStanding( 1085, 0);
	}
	else
	{
		plr->SetAtWar( 1085, true );
		plr->SetStanding( 1085, -9000);
		if(plr->GetStanding(1050) < 0)
			plr->SetStanding( 1050, 0);
	}
}

void IsleOfConquest::OnRemovePlayer(Player* plr)
{
	plr->RemoveAura(BG_PREPARATION);
}

void IsleOfConquest::OnCreate()
{

}

void IsleOfConquest::HookOnPlayerKill(Player* plr, Unit* pVictim)
{
	if(pVictim->IsPlayer())
	{
		plr->m_bgScore.KillingBlows++;
		UpdatePvPData();
	}
}

void IsleOfConquest::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void IsleOfConquest::HookOnShadowSight() 
{

}

bool IsleOfConquest::HookSlowLockOpen(GameObject* pGo, Player* pPlayer, Spell* pSpell)
{
	return false;
}

void IsleOfConquest::HookGenerateLoot(Player* plr, Corpse* pCorpse)
{

}

LocationVector IsleOfConquest::GetStartingCoords(uint32 Team)
{
	if(Team)
		return LocationVector(1202.641846f, -764.488892f, 48.914841f, 3.109386f);
	else
		return LocationVector(356.734833f, -834.945557f, 48.915386f, 6.263546f);
}
