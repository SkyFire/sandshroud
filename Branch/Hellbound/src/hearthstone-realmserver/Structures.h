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

/* Copied structures from game */
struct player_item
{
	uint32 displayid;
	uint8 invtype;
	uint32 enchantment; // added in 2.4
};

struct SocketInfo
{
	uint32 SocketColor;
	uint32 Unk;
};

struct ItemSpell
{
	uint32 Id;
	uint32 Trigger;
	int32 Charges;
	int32 Cooldown;
	uint32 Category;
	int32 CategoryCooldown;
};

struct ItemDamage
{
	float Min;
	float Max;
	uint32 Type;
};

struct ItemStat
{
	uint32 Type;
	int32 Value;
};

struct ItemPrototype
{
	uint32 ItemId;
	uint32 Class;
	uint32 SubClass;
	int32 unknown_bc;
	char * Name1;
	uint32 DisplayInfoID;
	uint32 Quality;
	uint32 Flags;
	uint32 Faction;
	uint32 BuyPrice;
	uint32 SellPrice;
	uint32 InventoryType;
	int32 AllowableClass;
	int32 AllowableRace;
	uint32 ItemLevel;
	uint32 RequiredLevel;
	uint32 RequiredSkill;
	uint32 RequiredSkillRank;
	uint32 RequiredSpell;
	uint32 RequiredPlayerRank1;
	uint32 RequiredPlayerRank2;
	uint32 RequiredFaction;
	uint32 RequiredFactionStanding;
	uint32 Unique;
	uint32 MaxCount;
	uint32 ContainerSlots;
	ItemStat Stats[10];
	uint32 ScalingStatsEntry;
	uint32 ScalingStatsFlag;
	ItemDamage Damage[2];
	uint32 Armor;
	uint32 HolyRes;
	uint32 FireRes;
	uint32 NatureRes;
	uint32 FrostRes;
	uint32 ShadowRes;
	uint32 ArcaneRes;
	uint32 Delay;
	uint32 AmmoType;
	float  Range;
	ItemSpell Spells[5];
	uint32 Bonding;
	char * Description;
	uint32 PageId;
	uint32 PageLanguage;
	uint32 PageMaterial;
	uint32 QuestId;
	uint32 LockId;
	uint32 LockMaterial;
	uint32 SheathId;
	uint32 RandomPropId;
	uint32 RandomSuffixId;
	uint32 Block;
	uint32 ItemSet;
	uint32 MaxDurability;
	uint32 ZoneNameID;
	uint32 MapID;
	uint32 BagFamily;
	uint32 TotemCategory;
	SocketInfo Sockets[3];
	uint32 SocketBonus;
	uint32 GemProperties;
	int32 DisenchantReqSkill;
	int32 Lootgold;
	uint32 ArmorDamageModifier;
	uint32 ItemLimitCategory;
	uint32 HolidayId;

	uint32 FoodType;		//pet food type
	int32 ForcedPetId;
	uint32 DummySubClass;

	string ConstructItemLink(uint32 random_prop, uint32 random_suffix, uint32 stack);
	bool ValidateItemLink(const char *szLink);
	bool ValidateItemSpell(uint32 SpellID);
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

