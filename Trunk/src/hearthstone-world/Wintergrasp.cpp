/*
 * Sandshroud Hearthstone
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

/*// Fatty array time.
const uint32 CreatureEntryPair[][2] =
{
    {32307, 32308}, // Guards
    {30739, 30740}, // Champions
    {31101, 31051}, // Hoodoo Master & Sorceress
    {31102, 31052}, // Vieron Blazefeather & Bowyer
    {32296, 32294}, // Quartermaster
    {31107, 31109}, // Lieutenant & Senior Demolitionist
    {31151, 31153}, // Tactical Officer
    {31106, 31108}, // Siegesmith & Siege Master
    {31053, 31054}, // Primalist & Anchorite
    {31091, 31036}, // Commander
    {32615, 32626}, // Warbringer & Brigadier General
    {0,0}
};*/

Wintergrasp::Wintergrasp(WintergraspInternal* WGI, MapMgr* mgr) : Internal(*WGI)
{
	for(int i = 0; i < 3; ++i)
		playercount[i] = 0;

	Internal.m_wintergrasp = 1;
	Player* plr = NULL;

	for(PlayerStorageMap::iterator itr =  mgr->m_PlayerStorage.begin(); itr != mgr->m_PlayerStorage.end(); ++itr)
	{
		plr = itr->second;
		if((plr->GetAreaID() == WINTERGRASP) || (plr->GetZoneId() == WINTERGRASP))
		{
			WGPlayers.insert(plr);
			++playercount[2]; // Total
			++playercount[plr->GetTeam()];
		}
	}
	if(WGPlayers.size() != playercount[2])
		printf("Player disfunction occured!\n");

	Init();
}

Wintergrasp::~Wintergrasp()
{
	Internal.m_wintergrasp = 2; // We are ending Wintergrasp.
	// Handle deletion and removal.
	Internal.m_wintergrasp = 0; // Wintergrasp has ended.
	Internal.EndWintergrasp();
	delete this; // Kill ourself.
}

void Wintergrasp::Init()
{
//	Internal.SendInitWorldStates();
//	Internal.UpdateClock();
//	Internal.StartWintergrasp();
}
