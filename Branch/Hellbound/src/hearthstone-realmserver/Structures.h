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

#ifndef STRUCTURES_H
#define STRUCTURES_H

#ifndef _GAME

// This way we can grab enums and defines
#include "../hearthstone-world/ItemPrototype.h"

struct CreateInfo_ItemStruct
{
	uint32	protoid;
	uint8	slot;
	uint32	amount;
};

struct CreateInfo_SkillStruct
{
	uint32	skillid;
	uint32	currentval;
	uint32	maxval;
};

struct CreateInfo_ActionBarStruct
{
	uint32	button;
	uint32	action;
	uint32	type;
	uint32	misc;
};

struct PlayerCreateInfo
{
	uint8	index;
	uint8	race;
	uint32	factiontemplate;
	uint8	class_;
	uint32	mapId;
	uint32	zoneId;
	float	positionX;
	float	positionY;
	float	positionZ;
	float	Orientation;
	uint16	displayId;
	uint8	strength;
	uint8	ability;
	uint8	stamina;
	uint8	intellect;
	uint8	spirit;
	uint32	health;
	uint32	mana;
	uint32	rage;
	uint32	focus;
	uint32	energy;
	uint32	runic;
	uint32	attackpower;
	float	mindmg;
	float	maxdmg;
	std::list<CreateInfo_ItemStruct> items;
	std::list<CreateInfo_SkillStruct> skills;
	std::list<CreateInfo_ActionBarStruct> actionbars;
	std::set<uint32> spell_list;
};

/* Copied structures from game */
struct player_item
{
	uint32 displayid;
	uint8 invtype;
	uint32 enchantment; // added in 2.4
};

struct ActionButton
{
	uint16	Action;
	uint8	Misc;
	uint8	Type;
};

struct PlayerSkill
{
	skilllineentry * Skill;
	uint32 CurrentValue;
	uint32 MaximumValue;
	uint32 BonusValue;
	float GetSkillUpChance();
	bool Reset(uint32 Id);
};

struct CreatureInfo
{
	uint32 Id;
	char * Name;
	char * SubName;
	char * info_str;
	uint32 Flags1;
	uint32 Type;
	uint32 TypeFlags;
	uint32 Family;
	uint32 Rank;
	uint32 Unknown1;
	uint32 SpellDataID;
	uint32 Male_DisplayID;
	uint32 Female_DisplayID;
	uint32 Male_DisplayID2;
	uint32 Female_DisplayID2;
	float unkfloat1; // Something to do with elites.
	float unkfloat2; // Something to do with unkfloat1.
	uint8  Civilian;
	uint8  Leader;
};

struct GameObjectInfo
{
	uint32 ID;
	uint32 Type;
	uint32 DisplayID;
	char * Name;
	uint32 SpellFocus;
	uint32 sound1;
	uint32 sound2;
	uint32 sound3;
	uint32 sound4;
	uint32 sound5;
	uint32 sound6;
	uint32 sound7;
	uint32 sound8;
	uint32 sound9;
	uint32 Unknown1;
	uint32 Unknown2;
	uint32 Unknown3;
	uint32 Unknown4;
	uint32 Unknown5;
	uint32 Unknown6;
	uint32 Unknown7;
	uint32 Unknown8;
	uint32 Unknown9;
	uint32 Unknown10;
	uint32 Unknown11;
	uint32 Unknown12;
	uint32 Unknown13;
	uint32 Unknown14;
};

struct ItemPage
{
	uint32 id;
	char * text;
	uint32 next_page;
};


struct Quest
{
	uint32 id;
	uint32 zone_id;
	uint32 quest_sort;
	uint32 quest_flags;
	uint32 min_level;
	uint32 max_level;
	uint32 type;
	uint32 required_races;
	uint32 required_class;
	uint32 required_tradeskill;
	uint32 required_tradeskill_value;
	uint32 required_rep_faction;
	uint32 required_rep_value;

	uint32 suggested_players;

	uint32 time;
	uint32 special_flags;

	uint32 previous_quest_id;
	uint32 next_quest_id;

	uint32 srcitem;
	uint32 srcitemcount;

	char * title;
	char * details;
	char * objectives;
	char * completiontext;
	char * incompletetext;
	char * endtext;

	char * objectivetexts[4];

	uint32 required_item[6];
	uint32 required_itemcount[6];

	uint32 required_kill_player;

	uint32 required_mob[4];
	uint32 required_mobcount[4];
	uint32 required_spell[4];

	uint32 reward_choiceitem[6];
	uint32 reward_choiceitemcount[6];

	uint32 reward_item[4];
	uint32 reward_itemcount[4];

	uint32 reward_repfaction[5];
	int32 reward_repvalue[5];
	int32 reward_replimit/*[5]*/;

	uint32 reward_title;
	uint32 reward_money;
	uint32 reward_honor;
	uint32 reward_xp;
	uint32 reward_spell;
	uint32 reward_talents;
	uint32 effect_on_player;

	uint32 point_mapid;
	float point_x;
	float point_y;
	uint32 point_opt;

	uint32 required_money;
	uint32 required_triggers[4];
	uint32 required_quests[4];
	uint32 required_quest_and_or;
	uint32 receive_items[4];
	uint32 receive_itemcount[4];
	uint8  is_repeatable;
	uint32 reward_arenapoints;
	uint32 start_phase;
	uint32 complete_phase;
};

struct GossipText_Text
{
	float Prob;
	char * Text[2];
	uint32 Lang;
	uint32 Emote[6];
};

struct GossipText
{
	uint32 ID;
	GossipText_Text Texts[8];
};

struct MapInfo
{
	uint32 mapid;
	char * name;
	bool load;
	uint32 type;
	uint32 playerlimit;
	uint32 minlevel;
	float repopx;
	float repopy;
	float repopz;
	uint32 repopmapid;
	uint32 flags;
	uint32 cooldown;
 	uint32 required_quest;
	uint32 required_item;
	uint32 heroic_key[2];
	float update_distance;
	uint32 checkpoint_id;
	uint32 phasehorde;
	uint32 phasealliance;
	bool collision;

	bool HasFlag(uint32 flag)
	{
		return (flags & flag) != 0;
	}
};

enum Classes
{
	WARRIOR = 1,
	PALADIN = 2,
	HUNTER = 3,
	ROGUE = 4,
	PRIEST = 5,
	DEATHKNIGHT = 6,
	SHAMAN = 7,
	MAGE = 8,
	WARLOCK = 9,
	DRUID = 11,
};

enum Races
{
	RACE_HUMAN = 1,
	RACE_ORC = 2,
	RACE_DWARF = 3,
	RACE_NIGHTELF = 4,
	RACE_UNDEAD = 5,
	RACE_TAUREN = 6,
	RACE_GNOME = 7,
	RACE_TROLL = 8,
	RACE_GOBLIN	= 9,
	RACE_BLOODELF = 10,
	RACE_DRAENEI = 11,
	RACE_WORGEN	= 22
};

#endif // The game.

#endif // STRUCTURES_H