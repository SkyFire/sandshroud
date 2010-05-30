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

initialiseSingleton( WintergraspInternal );

#ifdef WIN32
static HANDLE m_abortEvent = INVALID_HANDLE_VALUE;
#else
static pthread_cond_t abortcond;
static pthread_mutex_t abortmutex;
#endif

WintergraspInternal::WintergraspInternal() : WGMgr(*(sInstanceMgr.GetMapMgr(571)))
{
	m_threadRunning = true;
	WGcounter = 0;
	SetWGTimer((6*60000/*Hour*/)*3);
	m_wintergrasp = 0;
}

WintergraspInternal::~WintergraspInternal()
{
	// Did we died'd?
}

void WintergraspInternal::terminate()
{
	m_threadRunning = false;
}

void WintergraspInternal::dupe_tm_pointer(tm * returnvalue, tm * mypointer)
{
	memcpy(mypointer, returnvalue, sizeof(tm));
}

bool WintergraspInternal::has_timeout_expired(tm * now_time, tm * last_time)
{
	if(now_time->tm_hour != last_time->tm_hour)
		return true;

	if(now_time->tm_mday != last_time->tm_mday)
		return true;

	return false;
}

bool WintergraspInternal::run()
{
	SetThreadName("WGInternal");
	currenttime = UNIXTIME;
	dupe_tm_pointer(localtime(&currenttime), &local_currenttime);
	last_countertime = UNIXTIME;
	dupe_tm_pointer(localtime(&last_countertime), &local_last_countertime);

	Log.Notice("WintergraspInternal", "Wintergrasp Handler Initiated.");

	while(m_threadRunning)
	{
		if(has_timeout_expired(&local_currenttime, &local_last_countertime))
		{
			++WGcounter;

			if(WGcounter >= 3/*3 hours*/) // TODO: sWorld.WintergraspHourInterval
			{
				Log.Notice("WintergraspInternal", "Wintergrasp function called.");
				if(m_wintergrasp == 0)
				{
					Log.Notice("WintergraspInternal", "Starting Wintergrasp.");
					WG = Wintergrasp::Create(this, &WGMgr);
				}
				WGcounter = 0; // Reset our timer.
			}
			else
				Log.Notice("WintergraspInternal", "Wintergrasp counter at %u/3", WGcounter);

			last_countertime = UNIXTIME;
			dupe_tm_pointer(localtime(&last_countertime), &local_last_countertime);
		}
	}
	return false;
}

void WintergraspInternal::SendInitWorldStates(Player* plr)
{
	printf("Pieflavor");
	WorldPacket data(SMSG_INIT_WORLD_STATES, (4+4+4+2));
	data << uint32(571);
	data << uint32(WINTERGRASP);
	data << uint32(0);
	data << uint16(4+5+(WG ? 4 : 0));

	data << uint32(3803) << uint32(defendingteam == ALLIANCE ? 1 : 0);
	data << uint32(3802) << uint32(defendingteam != ALLIANCE ? 1 : 0);
	data << uint32(3801) << uint32(WG_started ? 0 : 1);
	data << uint32(3710) << uint32(WG_started ? 1 : 0);

	for (uint32 i = 0; i < 5; ++i)
		data << ClockWorldState[i] << m_clock[i];

	if(WG_started && WG != NULL)
	{
		data << uint32(A_NUMVEH_WORLDSTATE) << uint32(WG->GetNumVehicles(ALLIANCE));
		data << uint32(A_MAXVEH_WORLDSTATE) << uint32(WG->GetNumWorkshops(ALLIANCE)*4);
		data << uint32(H_NUMVEH_WORLDSTATE) << uint32(WG->GetNumVehicles(HORDE));
		data << uint32(H_MAXVEH_WORLDSTATE) << uint32(WG->GetNumWorkshops(HORDE)*4);
	}

	if(plr)
		plr->GetSession()->SendPacket(&data);
	else
	{
		if(WG != NULL)
		{
			if(WG->WGPlayers.size())
			{
				for(WintergraspPlayerSet::iterator itr = WG->WGPlayers.begin(); itr != WG->WGPlayers.end(); ++itr)
					(*itr)->GetSession()->SendPacket(&data);
			}
		}
		else
		{
			if(WGMgr.m_PlayerStorage.size())
			{
				for(PlayerStorageMap::iterator itr =  WGMgr.m_PlayerStorage.begin(); itr != WGMgr.m_PlayerStorage.end(); ++itr)
				{
					plr = itr->second;
					if((plr->GetAreaID() == WINTERGRASP) || (plr->GetZoneId() == WINTERGRASP))
						plr->GetSession()->SendPacket(&data);
				}
				plr = NULL;
			}
		}
	}
}

void WintergraspInternal::UpdateClockDigit(uint32 timer, uint32 digit, uint32 mod)
{
	uint32 value = timer%mod;
	timer /= mod;
	if(m_clock[digit] != value)
	{
		m_clock[digit] = value;
		SendWSUpdateToAll(ClockWorldState[digit], value);
	}
	WGMgr.GetStateManager().UpdateWorldState(m_clock[digit], value);
}

void WintergraspInternal::SendWSUpdateToAll(uint32 WorldState, uint32 Value)
{
	Player* plr = NULL;
	WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);
	data << WorldState;
	data << Value;
	if(WG != NULL)
	{
		for(WintergraspPlayerSet::iterator itr = WG->WGPlayers.begin(); itr != WG->WGPlayers.end(); ++itr)
			(*itr)->GetSession()->SendPacket(&data);
	}
	else
	{
		for(PlayerStorageMap::iterator itr =  WGMgr.m_PlayerStorage.begin(); itr != WGMgr.m_PlayerStorage.end(); ++itr)
		{
			plr = itr->second;
			if((plr->GetAreaID() == WINTERGRASP) || (plr->GetZoneId() == WINTERGRASP))
				plr->GetSession()->SendPacket(&data);
		}
	}
}

void WintergraspInternal::UpdateClock()
{
	uint32 timer = m_timer / 1000;
	UpdateClockDigit(timer, 0, 10);
	UpdateClockDigit(timer, 1, 6);
	UpdateClockDigit(timer, 2, 10);
	UpdateClockDigit(timer, 3, 6);
	if(!WG_started)
		UpdateClockDigit(timer, 4, 10);
}