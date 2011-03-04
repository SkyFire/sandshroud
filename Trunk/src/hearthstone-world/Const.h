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

#ifndef __CONST_H__
#define __CONST_H__

#define NUM_MAPS 750
#define CL_BUILD_SUPPORT 12340

class Database;
class CBattleground;
class CBattlegroundManager;
class AlteracValley;
class ArathiBasin;
class Arena;
class Aura;
class Container;
class Corpse;
class Creature;
class DynamicObject;
class EventableObject;
class EyeOfTheStorm;
class GameObject;
class Item;
class LootRoll;
class MapMgr;
class Object;
class Pet;
class Player;
class QuestMgr;
class Spell;
class Transporter;
class Unit;
class Vehicle;
class WarsongGulch;
class WeatherInfo;
class World;
class StrandOfTheAncients;

#define CharacterDatabase (*Database_Character)
#define WorldDatabase (*Database_World)
#define LogDatabase (*Database_Log)

#define CAST(x, y) static_cast<x*>(y)
#define TO_PLAYER(ptr) ((Player*)ptr)
#define TO_UNIT(ptr) ((Unit*)ptr)
#define TO_CREATURE(ptr) ((Creature*)ptr)
#define TO_PET(ptr) ((Pet*)ptr)
#define TO_CONTAINER(ptr) ((Container*)ptr)
#define TO_ITEM(ptr) ((Item*)ptr)
#define TO_OBJECT(ptr) ((Object*)ptr)
#define TO_GAMEOBJECT(ptr) ((GameObject*)ptr)
#define TO_DYNAMICOBJECT(ptr) ((DynamicObject*)ptr)
#define TO_CORPSE(ptr) ((Corpse*)ptr)
#define TO_EVENTABLEOBJECT(ptr) ((EventableObject*)ptr)
#define TO_CBATTLEGROUND(ptr) ((CBattleground*)ptr)
#define TO_CBATTLEGROUNDMGR(ptr) ((CBattlegroundManager*)ptr)
#define TO_ALTERACVALLEY(ptr) ((AlteracValley*)ptr)
#define TO_EYEOFTHESTORM(ptr) ((EyeOfTheStorm*)ptr)
#define TO_WARSONGGULCH(ptr) ((WarsongGulch*)ptr)
#define TO_SOTA(ptr) ((StrandOfTheAncients*)ptr)
#define TO_ARENA(ptr) ((Arena*)ptr)
#define TO_ARATHIBASIN(ptr) ((ArathiBasin*)ptr)
#define TO_TRANSPORT(ptr) ((Transporter*)ptr)
#define TO_AURA(ptr) ((Aura*)ptr)
#define TO_SPELL(ptr) ((Spell*)ptr)
#define TO_LOOTROLL(ptr) ((LootRoll*)ptr)
#define TO_VEHICLE(ptr) ((Vehicle*)ptr)
#define TO_WEATHER(ptr) ((WeatherInfo*)ptr)
#define TO_TAXI(ptr) ((TaxiPath*)ptr)
#define TO_WORLDSESSION(ptr) ((WorldSession*)ptr)

#define NULLPTR NULL
#define NULLGOB NULL
#define NULLPLR NULL
#define NULLOBJ NULL
#define NULLUNIT NULL
#define NULLPET NULL
#define NULLITEM NULL
#define NULLDYN NULL
#define NULLCREATURE NULL
#define NULLVEHICLE NULL
#define NULLCORPSE NULL
#define NULLTRANSPORT NULL
#define NULLEVENTABLEOBJECT NULL
#define NULLCONTAINER NULL
#define NULLBATTLEGROUND NULL
#define NULLWEATHER NULL
#define NULLAURA NULL
#define NULLSPELL NULL
#define NULLROLL NULL
#define NULLBGMGR NULL
#define NULLWORLD NULL
#define NULLMAPMGR NULL
#define NULLINSTANCEMGR NULL

SERVER_DECL extern Database* Database_Character;
SERVER_DECL extern Database* Database_Account;
SERVER_DECL extern Database* Database_World;
SERVER_DECL extern Database* Database_Log;

#endif
