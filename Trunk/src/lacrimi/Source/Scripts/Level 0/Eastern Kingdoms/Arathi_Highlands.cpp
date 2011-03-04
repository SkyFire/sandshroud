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

void SunkenTreasure::OnQuestStart( Player *mTarget, QuestLogEntry * qLogEntry)
{
	if( mTarget == NULL || mTarget->GetMapMgr() == NULL || mTarget->GetMapMgr()->GetInterface() == NULL )
		return;
	float SSX = mTarget->GetPositionX();
	float SSY = mTarget->GetPositionY();
	float SSZ = mTarget->GetPositionZ();

	Creature *creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 2768);
	if(creat == NULL)
		return;
	creat->m_escorter = mTarget;   
	creat->GetAIInterface()->setMoveType(11);
	creat->GetAIInterface()->StopMovement(3000);
	creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Defens Me!");
	creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

	CreateCustomWaypointMap(creat);
	WaypointCreate(creat,-2078.054443f, -2091.207764f, 9.526212f, 4.770276f, 0, 256, 4049);
	WaypointCreate(creat,-2076.626465f, -2109.960449f, 14.320494f, 4.821321f, 0, 256, 4049);
	WaypointCreate(creat,-2072.851074f, -2123.574219f, 18.482662f, 5.623996f, 0, 256, 4049);
	WaypointCreate(creat,-2063.878906f, -2132.617920f, 21.430487f, 5.512474f, 0, 256, 4049);
	WaypointCreate(creat,-2051.495117f, -2145.205811f, 20.500065f, 5.481060f, 0, 256, 4049);
	WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
	WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
	WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
	WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
	WaypointCreate(creat,-2044.748291f, -2152.411377f, 20.158432f, 5.437863f, 0, 256, 4049);
	WaypointCreate(creat,-2051.495117f, -2145.205811f, 20.500065f, 5.481060f, 0, 256, 4049);
	WaypointCreate(creat,-2063.878906f, -2132.617920f, 21.430487f, 5.512474f, 0, 256, 4049);
	WaypointCreate(creat,-2072.851074f, -2123.574219f, 18.482662f, 5.623996f, 0, 256, 4049);
	WaypointCreate(creat,-2076.626465f, -2109.960449f, 14.320494f, 4.821321f, 0, 256, 4049);
	WaypointCreate(creat,-2078.054443f, -2091.207764f, 9.526212f, 4.770276f, 0, 256, 4049);
	EnableWaypoints(creat);
}

void Professor_Phizzlethorpe::OnReachWP(uint32 iWaypointId, bool bForwards)
{
	if(iWaypointId == 15)
	{
		_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, I found the fact that, it searched");
		_unit->Despawn(5000,1000);
		DeleteWaypoints(_unit);
		if(_unit->m_escorter == NULL)
			return;
		Player *plr = _unit->m_escorter;
		_unit->m_escorter = NULLPLR;
		plr->GetQuestLogForEntry(665)->SendQuestComplete();
	}
}

void Lacrimi::SetupArathiHighlands()
{
	sMgr->register_creature_script(2768, &Professor_Phizzlethorpe::Create);
	sMgr->register_quest_script(665, ((QuestScript*)(new SunkenTreasure())));
}
