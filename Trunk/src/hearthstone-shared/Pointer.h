#ifndef _SHARED_PTR_H
#define _SHARED_PTR_H

// Meh
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

#define NULLPTR 0
#define NULLGOB 0
#define NULLPLR 0
#define NULLOBJ 0
#define NULLUNIT 0
#define NULLPET 0
#define NULLITEM 0
#define NULLDYN 0
#define NULLCREATURE 0
#define NULLVEHICLE 0
#define NULLCORPSE 0
#define NULLTRANSPORT 0
#define NULLEVENTABLEOBJECT 0
#define NULLCONTAINER 0
#define NULLMAPMGR 0
#define NULLBGMGR 0
#define NULLBATTLEGROUND 0
#define NULLWEATHER 0
#define NULLWORLD 0
#define NULLINSTANCEMGR 0
#define NULLAURA 0
#define NULLROLL 0
#define NULLMAPMGR 0
#define NULLSPELL 0

#endif
