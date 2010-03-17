/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "stdafx.h"

typedef uint32 TeamPair[2];

void Wintergrasp::SendInitWorldStatesTo(Player *player) const
{
    WorldPacket data(SMSG_INIT_WORLD_STATES, (4+4+4+2+(m_buildingStates.size()*8)));
    data << uint32(571);
    data << uint32(ZONE_WINTERGRASP);
    data << uint32(0);
    data << uint16(4+5+4+m_buildingStates.size());

    data << uint32(3803) << uint32(getDefenderTeam() == TEAM_ALLIANCE ? 1 : 0);
    data << uint32(3802) << uint32(getDefenderTeam() != TEAM_ALLIANCE ? 1 : 0);
    data << uint32(3801) << uint32(isWarTime() ? 0 : 1);
    data << uint32(3710) << uint32(isWarTime() ? 1 : 0);

    for (uint32 i = 0; i < 5; ++i)
        data << ClockWorldState[i] << m_clock[i];

    data << uint32(3490) << uint32(m_vehicles[TEAM_HORDE].size());
    data << uint32(3491) << m_workshopCount[TEAM_HORDE] * MAX_VEHICLE_PER_WORKSHOP;
    data << uint32(3680) << uint32(m_vehicles[TEAM_ALLIANCE].size());
    data << uint32(3681) << m_workshopCount[TEAM_ALLIANCE] * MAX_VEHICLE_PER_WORKSHOP;

    for (BuildingStateMap::const_iterator itr = m_buildingStates.begin(); itr != m_buildingStates.end(); ++itr)
        itr->second->FillData(data);

    if (player)
        player->GetSession()->SendPacket(&data);
    else
        BroadcastPacket(data);
}

void Wintergrasp::UpdateClockDigit(uint32 &timer, uint32 digit, uint32 mod)
{
    uint32 value = timer%mod;
    timer /= mod;
    if (m_clock[digit] != value)
    {
        m_clock[digit] = value;
        SendUpdateWorldState(ClockWorldState[digit], value);
    }
}

void Wintergrasp::UpdateClock()
{
    uint32 timer = m_timer / 1000;
    UpdateClockDigit(timer, 0, 10);
    UpdateClockDigit(timer, 1, 6);
    UpdateClockDigit(timer, 2, 10);
    UpdateClockDigit(timer, 3, 6);
    if (!isWarTime())
        UpdateClockDigit(timer, 4, 10);
}

bool Wintergrasp::Update(uint32 diff)
{
    if (m_timer > diff)
    {
        m_timer -= diff;

        if (isWarTime())
            Wintergrasp::Update(diff); // update capture points

        UpdateClock();
    }
    else
    {
        m_sendUpdate = false;

        if (isWarTime())
        {
            if (m_changeDefender)
            {
                m_changeDefender = false;
                m_defender = getAttackerTeam();
                if (getDefenderTeam() == TEAM_ALLIANCE)
                    sWorld.SendZoneText(ZONE_WINTERGRASP, "Alliance has taken over the fortress!");
                else
                    sWorld.SendZoneText(ZONE_WINTERGRASP, "Horde has taken over the fortress!");
            }
            else
            {
                if (getDefenderTeam() == TEAM_ALLIANCE)
                    sWorld.SendZoneText(ZONE_WINTERGRASP, "Alliance has successfully defended the fortress!");
                else
                    sWorld.SendZoneText(ZONE_WINTERGRASP, "Horde has successfully defended the fortress!");
            }

            //EndBattle();
        }
        else
        {
            sWorld.SendZoneText(ZONE_WINTERGRASP, "Battle begins!");
            //StartBattle();
        }

        //UpdateAllWorldObject();
        UpdateClock();

        SendInitWorldStatesTo();
        m_sendUpdate = true;
    }

    return false;
}

void Wintergrasp::BroadcastPacket(WorldPacket &data) const
{
    // This is faster than sWorld.SendZoneMessage
    for(uint32 team = 0; team < 2; ++team)
		for(PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
			(*itr)->GetSession()->SendPacket(&data);
}

void Wintergrasp::SendUpdateWorldState(uint32 field, uint32 value)
{
	if(m_sendUpdate)
		for(int i = 0; i < 2; ++i)
			for(PlayerSet::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				(*itr)->SendUpdateWorldState(field, value);
}

void Wintergrasp::HandlePlayerEnterZone(Player * plr, uint32 zone)
{
	if (isWarTime() && !plr->HasAura(SPELL_RECRUIT) && !plr->HasAura(SPELL_CORPORAL)
		&& !plr->HasAura(SPELL_LIEUTENANT))
		plr->CastSpell(plr, SPELL_RECRUIT, true);

	SendInitWorldStatesTo(plr);
	m_players[plr->GetTeam()].insert(plr);
	UpdateTenacityStack();
}

void Wintergrasp::HandlePlayerLeaveZone(Player * plr, uint32 zone)
{
	if (!plr->GetSession()->PlayerLogout())
	{
		if (plr->GetVehicle()) // dismiss in change zone case
			plr->GetVehicle()->Dismiss();
		REMOVE_WARTIME_AURAS(plr);
	}
	REMOVE_TENACITY_AURA(plr);

	//if(!plr->GetSession()->PlayerLogout())
	//	SendRemoveWorldStates(plr);

	m_players[plr->GetTeam()].erase(plr);
	sLog.outDebug("Player %s left an outdoorpvp zone", plr->GetName());

	UpdateTenacityStack();
}

// Recalculates Tenacity and applies it to Players / Vehicles
void Wintergrasp::UpdateTenacityStack()
{
    if (!isWarTime())
        return;

    TeamId team = TEAM_NEUTRAL;
    uint32 allianceNum = m_players[TEAM_ALLIANCE].size();;
    uint32 hordeNum = m_players[TEAM_HORDE].size();;
    int32 newStack = 0;

    if (allianceNum && hordeNum)
    {
        if (allianceNum < hordeNum)
            newStack = (hordeNum / allianceNum - 1)*4; // positive, should cast on alliance
        else if (allianceNum > hordeNum)
            newStack = (1 - int32(allianceNum / hordeNum))*4; // negative, should cast on horde
    }

    if (newStack == m_tenacityStack)
        return;

    if (m_tenacityStack > 0 && newStack <= 0) // old buff was on alliance
        team = TEAM_ALLIANCE;
    else if (m_tenacityStack < 0 && newStack >= 0) // old buff was on horde
        team = TEAM_HORDE;

    m_tenacityStack = newStack;

    // Remove old buff
    if (team != TEAM_NEUTRAL)
    {
        for (PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if ((*itr)->getLevel() > 69)
                REMOVE_TENACITY_AURA((*itr));

        for (CreatureSet::const_iterator itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
            REMOVE_TENACITY_AURA((*itr));
    }

    // Apply new buff
    if (newStack)
    {
        team = newStack > 0 ? TEAM_ALLIANCE : TEAM_HORDE;
        if (newStack < 0) newStack = -newStack;
        int32 auraStack = newStack > 20 ? 20 : newStack; // Dont let it be higher than 20

        for (PlayerSet::const_iterator itr = m_players[team].begin(); itr != m_players[team].end(); ++itr)
            if ((*itr)->getLevel() > 69)
                CastTenacity((*itr), auraStack);

        for (CreatureSet::const_iterator itr = m_vehicles[team].begin(); itr != m_vehicles[team].end(); ++itr)
            CastTenacity((*itr), auraStack);
    }
}

// Cast or removes Tenacity. MaxHP Modified, HP keeps it's % ratio
void Wintergrasp::CastTenacity(Unit *unit, int32 newStack)
{
    if (!unit)
        return;

    uint32 spellId = unit->GetTypeId() == TYPEID_PLAYER ? SPELL_TENACITY : SPELL_TENACITY_VEHICLE;
}
