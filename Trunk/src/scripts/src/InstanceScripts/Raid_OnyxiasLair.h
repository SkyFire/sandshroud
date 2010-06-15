/*
 * Sun++ Scripts for Aspire MMORPG Server
 * Copyright (C) 2010-2011 Sandshroud Team <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"


#define MAP_LAIR 249

enum LairGO
{
	GO_EGG			= 176511,
	GO_LAVA			= 176912
};

enum LairBosses
{
	ONY_BOSS		= 10184,
	ONY_WHELP		= 11262,
	ONY_GUARD		= 36561
};

enum LairSpells
{
	ONY_FLAME_BREATH			= 18435,
	ONY_FLAME_BREATH25			= 68970,
	ONY_WING_BUFFET				= 18500,
	ONY_WING_BUFFET25			= 69293,
	ONY_TAIL_SWEEP				= 68867,
	ONY_TAIL_SWEEP25 			= 69286,
	ONY_ERUPTION 				= 17731,
	ONY_ERUPTION25 				= 69294,
	ONY_CLEAVE 					= 68868,
	ONY_BELLOWING_ROAR 			= 18431,
	ONY_FIREBALL 				= 18392,
	ONY_FIREBALL25 				= 68926,
	ONY_BREATH 					= 17086,
	GUARD_BLAST_NOVA			= 68958,
	GUARD_CLEAVE				= 15284,
	GUARD_IGNITE_WEAPON			= 68960
};

enum LairAchives
{	
	ACHIEV_BREATH_N			= 4404, 
	ACHIEV_BREATH_H			= 4407,
	ACHIEV_DEFEAT_N			= 4396,
	ACHIEV_DEFEAT_H			= 4397,
	ACHIEV_DOTS_N			= 4402,
	ACHIEV_DOTS_H			= 4405,
	ACHIEV_WHELPS_N			= 4403,
	ACHIEV_WHELPS_H			= 4406
};

static Coords whelpCoords[] =
{
    { -30.812f, -166.395f, -89.000f, 5.160f, Flag_Run },
    { -30.233f, -264.158f, -89.896f, 1.129f, Flag_Run },
    { -35.813f, -169.427f, -90.000f, 5.384f, Flag_Run },
    { -36.104f, -260.961f, -90.600f, 1.111f, Flag_Run },
    { -34.643f, -164.080f, -90.000f, 5.364f, Flag_Run },
    { -35.377f, -267.320f, -91.000f, 1.111f, Flag_Run }
};

static Coords coord[] =
{
	{ 0.0f, 0.0f, 0.0f, 0.0f, Flag_Fly},
    { -75.945f, -219.245f, -83.375f, 0.004947f, Flag_Fly },
    { -72.945f, -219.245f, -80.779f, 0.004947f, Flag_Fly },
    { 42.621f, -217.195f, -66.056f, 3.014011f, Flag_Fly },
    { 12.270f, -254.694f, -67.997f, 2.395585f, Flag_Fly },
    { -79.020f, -252.374f, -68.965f, 0.885179f, Flag_Fly },
    { -80.257f, -174.240f, -69.293f, 5.695741f, Flag_Fly },
    { 27.875f, -178.547f, -66.041f, 3.908957f, Flag_Fly },
    { -4.868f, -217.171f, -86.710f, 3.141590f, Flag_Fly }
};

class OnyxiaAI;
class OnyxiasLair;