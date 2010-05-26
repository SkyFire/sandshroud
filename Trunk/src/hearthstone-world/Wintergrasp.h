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

#ifndef WINTERGRASP_H
#define WINTERGRASP_H

// Crow: Sometimes we get zone as area and area as zone?
#define WINTERGRASP 4197
#define A_NUMVEH_WORLDSTATE 3680
#define A_MAXVEH_WORLDSTATE 3681
#define H_NUMVEH_WORLDSTATE 3490
#define H_MAXVEH_WORLDSTATE 3491
const uint32 ClockWorldState[5] = { 3785, 3784, 3782, 3976, 3975};

typedef std::set<Creature*> CreatureSwapSet;
typedef std::set<Player*> WintergraspPlayerSet;

class Wintergrasp
{
public:
	Wintergrasp(WintergraspInternal* WGI, MapMgr* mgr);
	~Wintergrasp();
	static Wintergrasp* Create( WintergraspInternal* i, MapMgr* m) { return new Wintergrasp(i, m); }
	void Init();


	WintergraspPlayerSet WGPlayers;
	CreatureSwapSet WGCreatures;
	uint32 GetNumVehicles(uint32 team) { if(team > 1) return 0; return numvehicles[team]; };
	uint32 GetNumWorkshops(uint32 team) { if(team > 1) return 0; return numworkshop[team]; };

private:
	WintergraspInternal Internal;

	// Counts
	uint32 playercount[3];

	// Buildings

	// Vehicles
	uint32 numvehicles[2];

	// Workshops
	uint32 numworkshop[2];
};

#endif // WINTERGRASP_H
