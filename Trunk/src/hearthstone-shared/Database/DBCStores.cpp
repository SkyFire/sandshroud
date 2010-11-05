/*
 * Sandshroud Hearthstone
 * Copyright (C) 2004 - 2005 Antrix Team
 * Copyright (C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 - 2009 AspireDev <http://www.aspiredev.org/>
 * Copyright (C) 2009 - 2010 Sandshroud <http://www.sandshroud.org/>
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

// Crow: If it has more than 40 columns, break em up.
#include "SharedStdAfx.h"

#ifndef	CATACLYSM

SERVER_DECL DBCStorage<AchievementEntry> dbcAchievement;
SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchievementCriteria;
SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
SERVER_DECL DBCStorage<CurrencyTypesEntry> dbcCurrencyTypesStore;
SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
SERVER_DECL DBCStorage<Lock> dbcLock;
SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
SERVER_DECL DBCStorage<SpellRuneCostEntry> dbcSpellRuneCost;
SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
SERVER_DECL DBCStorage<SpellDifficultyEntry> dbcSpellDifficulty;
SERVER_DECL DBCStorage<AreaTable> dbcArea;
SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
SERVER_DECL DBCStorage<CreatureDisplayInfo> dbcCreatureDisplayInfo;
SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
SERVER_DECL DBCStorage<MapEntry> dbcMap;
SERVER_DECL DBCStorage<ItemEntry> dbcItem;
SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices;
SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyle;
SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
SERVER_DECL DBCStorage<SpellShapeshiftForm> dbcSpellShapeshiftForm;
SERVER_DECL DBCStorage<VehicleEntry> dbcVehicle;
SERVER_DECL DBCStorage<VehicleSeatEntry> dbcVehicleSeat;
SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOAreaTable;
SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProps;
SERVER_DECL DBCStorage<ScalingStatDistributionEntry> dbcScalingStatDistribution;
SERVER_DECL DBCStorage<ScalingStatValuesEntry> dbcScalingStatValues;
SERVER_DECL DBCStorage<DestructibleModelDataEntry> dbcDestructibleModelDataEntry;

const char* SummonPropertiesfmt = "uuuuuu";
const char* AreaGroupFormat="niiiiiii";
const char* CurrencyTypesEntryFormat = "xnxu";
const char* ItemSetFormat = "uxxxxxxxxxxxxxxxxxuuuuuuuuuuxxxxxxxuuuuuuuuuuuuuuuuuu";
const char* LockFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuxxxxxxxx";
const char* EmoteEntryFormat = "uxuxxxxxxxxxxxxxxxx";
const char* skilllinespellFormat = "uuuuuxxuuuuuxx";
const char* EnchantEntrYFormat = "uxuuuuuuuuuuuusxxxxxxxxxxxxxxxxuuuuxxx";
const char* GemPropertyEntryFormat = "uuuuu";
const char* GlyphPropertyEntryFormat = "uuuu";
const char* skilllineentrYFormat = "uuusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* spellentryFormat =
	"u" // Id
	"u" // Category
	"u" // DispelType
	"u" // MechanicsType
	"uuuuuuuu" // 8x Flags
	"u" // RequiredShapeShift
	"x" // unk00
	"u" // NotAllowedShapeShift
	"x" // unk01
	"u" // Targets
	"u" // TargetCreatureType
	"u" // RequiresSpellFocus
	"u" // FacingCasterFlags
	"uuuuuuuu" // 8x Aura crap
	"u" // CastingTimeIndex
	"u" // RecoveryTime
	"u" // CategoryRecoveryTime
	"u" // InterruptFlags
	"u" // AuraInterruptFlags
	"u" // ChannelInterruptFlags
	"u" // procFlags
	"u" // procChance
	"u" // procCharges
	"u" // maxLevel
	"u" // baseLevel
	"u" // spellLevel
	"u" // DurationIndex
	"u" // powerType
	"u" // manaCost
	"u" // manaCostPerlevel
	"u" // manaPerSecond
	"u" // manaPerSecondPerLevel
	"u" // rangeIndex
	"f" // Speed
	"u" // modalNextSpell
	"u" // maxstack
	"uu" // Totem
	"uuuuuuuu" // 8x Reagent
	"uuuuuuuu" // 8x ReagentCount
	"i" // EquippedItemClass
	"u" // EquippedItemSubClass
	"u" // RequiredItemFlags
	"uuu" // 3x Spell Effect
	"uuu" // 3x Spell Effect Die Sides
	"fff" // 3x Spell Effect Real Points per Level
	"iii" // 3x Spell Effect Base Points
	"iii" // 3x Spell Effect Mechanic
	"uuu" // 3x Spell Effect Target A
	"uuu" // 3x Spell Effect Target B
	"uuu" // 3x Spell Effect Radius index
	"uuu" // 3x Spell Effect Apply Aura Name
	"uuu" // 3x Spell Effect Amplitude
	"fff" // 3x Spell Effect Multiple Value
	"uuu" // 3x Spell Effect Chain Target
	"uuu" // 3x Spell Effect Item Type
	"uuu" // 3x Spell Effect Misc Value
	"uuu" // 3x Spell Effect Misc Value B
	"uuu" // 3x Spell Effect Trigger Spell
	"fff" // 3x Spell Effect Points per Combo Point
	"uuuuuuuuu" // 3x Spell Effect Class Mask[3]
	"uu" // 2x SpellVisual
	"u" // SpellIconID
	"u" // activeIconID
	"u" // spellPriority
	"sxxxxxxxxxxxxxxxx" // Name
	"sxxxxxxxxxxxxxxxx" // Rank
	"sxxxxxxxxxxxxxxxx" // Spell Description
	"sxxxxxxxxxxxxxxxx" // Buff Description
	"u" // ManaCostPercentage
	"u" // StartRecoveryCategory
	"u" // StartRecoveryTime
	"u" // MaxTargetLevel
	"u" // SpellFamilyName
	"uuu" // 3x SpellGroupType
	"u" // MaxTargets
	"u" // Spell_Dmg_Type
	"u" // PreventionType
	"i" // StanceBarOrder
	"fff" // 3x dmg_multiplier
	"u" // MinFactionID
	"u" // MinReputation
	"u" // RequiredAuraVision
	"uu" // 2x TotemCategory
	"i" // AreaGroupId
	"u" // School
	"ux" // runeCostID and spellMissileID
	"u" // PowerDisplayId
	"xxxx"// Unk
	"u"; // SpellDifficultyID
const char* spelldifficultyFormat = "uuuuu";
const char* itemFormat = "uuuiiuuu";
const char* itemextendedcostFormat = "uuuuuuuuuuuuuuux";
const char* talententryFormat = "uuuuuuuuuxxxxuxxuxxxuxx";
const char* talenttabentryFormat = "uxxxxxxxxxxxxxxxxxxxuxux";
const char* spellcasttimeFormat = "uuxx";
const char* spellradiusFormat = "ufxf";
const char* spellrangeFormat =
	"u"					// Id
	"f"					// minRangeHostile
	"f"					// minRangeFriend
	"f"					// maxRangeHostile
	"f"					// maxRangeFriend
	"u"					// type
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

const char* WorldMapOverlayfmt="uxuxxxxxxxxxxxxxx";
const char* WMOAreaEntryfmt="niiixxxxxiixxxxxxxxxxxxxxxxx";
const char* SpellRuneCostfmt="uuuuu";
const char* spelldurationFormat = "uiii";
const char* randompropsFormat = "usuuuxxxxxxxxxxxxxxxxxxx";
const char* areatableFormat = "uuuuuxxxxxusxxxxxxxxxxxxxxxxuxxxxxxx";
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";
const char* auctionhousedbcFormat = "uuuuxxxxxxxxxxxxxxxxx";
const char* factiondbcFormat = "uiuuuuuuuuuuuuuuuuuxxxxssssssssssssssssxxxxxxxxxxxxxxxxxx";
const char* dbctaxinodeFormat = "uufffxxxxxxxxxxxxxxxxxuu";
const char* dbctaxipathFormat = "uuuu";
const char* dbctaxipathnodeFormat = "uuuufffuuxx";
const char* creaturedisplayFormat = "uxxxfxxxxxxxxxxx";
const char* creaturespelldataFormat = "uuuuuuuuu";
const char* charraceFormat = "uxxxxxxxxxxxuusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* charclassFormat = "uxuxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuxux";
const char* creaturefamilyFormat = "ufufuuuuuxsxxxxxxxxxxxxxxxxx";
const char* mapentryFormat =
	"u"					// 0 id
	"x"					// 1 name_internal
	"u"					// 2 map_type
	"x"					// 3 is_pvp_zone
	"x"					// 4 0 or 1 for battlegrounds (not arenas)
	"xxxxxxxxxxxxxxxxx"	// 5-21 real_name
	"x"					// 22 linked_zone
	"xxxxxxxxxxxxxxxxx" // 23-39 hordeIntro
	"xxxxxxxxxxxxxxxxx" // 40-56 allianceIntro
	"u"					// 57 multimap_id
	"x"					// 58 unk_float (all 1 but arathi 1.25)
	"x"					// 59 parent_map
	"x"					// 60 start_x
	"x"					// 61 start_y
	"x"					// 62 unk
	"x"					// 63 addon
	"x"					// 64 normalReqText
	"u";				// 65 Max players

const char* itemrandomsuffixformat = "usxxxxxxxxxxxxxxxxxuuuuuuuuuu";
const char* chatchannelformat = "uuxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* durabilityqualityFormat = "uf";
const char* durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char* bankslotpriceformat = "uu";
const char* barbershopstyleFormat="nusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuu";
const char* gtfloatformat = "f";
const char* spellshapeshiftformformat = "uxxxxxxxxxxxxxxxxxxuixuxxxxuuuuuuuu";

const char* vehicleseatentryFormat = "uuxfffxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuxxxxxxxxxxxx";
const char* vehicleentryFormat = "uxffxxuuuuuuuuxxxxxxxxxxxxxxxxxxxxxxxuxx";

const char* achievementfmt=
	"n" // Index
	"i" // Faction
	"i" // Mapid
	"u" // Previous Achievement
	"s" // Name
	"xxxxxxxxxxxxxxxx"
	"s" // Description
	"xxxxxxxxxxxxxxxx"
	"i" // Category Id
	"x" // points
	"x" // Order In Category
	"i" // Flags
	"xxxxxxxxxxxxxxxxxxxx";

const char* achievementCriteriafmt="niiiiiiiisxxxxxxxxxxxxxxxxiixix";

const char* scalingstatdistributionformat =
	"u" // ID
	"iiiiiiiiii" // Stat Mod
	"uuuuuuuuuu" // Modifier
	"u"; // Max Level

const char* scalingstatvaluesformat =
	"x" // Id
	"u" // Level
	"uuuu" // ScalingStatD modifier
	"uuuu" // Armor Mod
	"uuuuuu" // DPS mod
	"u" // Spell Power
	"uux" // Multipliers
	"uuuu"; // Armor Type[level]

const char* DestructibleModelDataFormat = "uxxuxxxuxxxuxxxuxxx";

#else // Cataclysm

SERVER_DECL DBCStorage<AchievementEntry> dbcAchievement;
const char* achievementfmt=
	"n" 	// 0 - Index
	"i" 	// 1 - Faction
	"i" 	// 2 - Mapid
	"u" 	// 3 - Previous Achievement
	"s" 	// 4 - Name
	"s" 	// 5 - Description
	"i" 	// 6 - Category Id
	"x" 	// 7 - points
	"x" 	// 8 - Order In Category
	"i" 	// 9 - Flags
	"x"		// 10 - Icon
	"x"		// 11 - Title Reward
	"x"		// 12 - Count
	"x";	// 13 - Referenced Achievement

SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchievementCriteria;
const char* achievementCriteriafmt="niiiiiiiisiixixxx";

SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
const char* AreaGroupFormat =
	"n"			// 0 		- AreaGroupID
	"iiiiii"	// 1-7 		- AreaId[6]
	"i";		// 8 		- nextGroup

SERVER_DECL DBCStorage<AreaTable> dbcArea;
const char* areatableFormat =
	"u"				// 0 		- Area ID
	"x"				// 1 		- Map ID
	"u"				// 2 		- Zone ID
	"u"				// 3 		- Exploration Flags
	"u"				// 4 		- Area Flags
	"xxxxx"			// 5 		- Unused
	"u"				// 6 		- Area Level
	"s"				// 7 		- Area Name
	"x"				// 8 		- Team
	"u"				// 9 		- Category
	"xxxxxxxxxx";	// 10-19 	- Unk[10]

SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
const char* auctionhousedbcFormat = 
	"u"		// 0 - Auction House ID
	"u"		// 1 - Faction
	"u"		// 2 - Fee
	"u"		// 3 - Tax
	"x";	// 4 - Name

SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
const char* bankslotpriceformat = 
	"u"		// 0 - Slot ID
	"u";	// 1 - Slot Price

SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyle;
const char* barbershopstyleFormat=
	"n"		// 0 - Barber Shop ID
	"u"		// 1 - Type
	"s"		// 2 - Name
	"x"		// 3 - Name unk
	"x"		// 4 - Cost Multplier
	"u"		// 5 - Race
	"u"		// 6 - Gender
	"u";	// 7 - Hair ID

SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
const char* charclassFormat = 
	"u"		// 0 		- Class ID
	"u"		// 1 		- Power Type
	"x"		// 2 		- Unused
	"s"		// 3 		- Name
	"x"		// 4 		- Female Name
	"x"		// 5 		- Neutral Name
	"x"		// 6 		- Capitalized Name
	"u"		// 7 		- Spell Family
	"x"		// 8 		- Unused
	"u"		// 9 		- Expansion
	"xxxx";	// 10-14 	- unk[4]

SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
const char* charraceFormat = 
	"u"		// 0 		- Race ID
	"x"		// 1 		- UNUSED
	"x"		// 2 		- Faction
	"x"		// 3 		- UNUSED
	"x"		// 4 		- Male Model
	"x"		// 5 		- Female Model
	"x"		// 6 		- UNUSED
	"u"		// 7 		- Team ID
	"xxxx"	// 8-11 	- UNUSED[4]
	"u"		// 12 		- Cinematic ID
	"x"		// 13 		- unk
	"s"		// 14 		- Name
	"x"		// 15 		- Female Name
	"x"		// 16 		- Neutral Name
	"xxx"	// 17-19 	- UNUSED[3]
	"x"		// 20 		- Expansion (0 - Vanilla, 1 - TBC, 2 - WOTLK, 3 - Cataclysm)
	"xxx";	// 21-23 	- Unk[3]

SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
const char* chatchannelformat = 
	"u"		// 0 - Channel ID
	"u"		// 1 - Channel Flags
	"x"		// 2 - Unk
	"s"		// 3 - Pattern
	"x";	// 4 - Channel Name

SERVER_DECL DBCStorage<CreatureDisplayInfo> dbcCreatureDisplayInfo;
const char* creaturedisplayFormat = 
	"u"					// 0 	- ID
	"xxx"				// 1-3 	- UNUSED[3]
	"f"					// 4 	- Scale
	"xxxxxxxxxxxx";		// 5-16 - UNUSED[12]

SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
const char* creaturefamilyFormat = 
	"u"		// 0 	- ID
	"f"		// 1 	- Minimum Size
	"u"		// 2 	- Minimum Level
	"f"		// 3 	- Maximum Size
	"u"		// 4 	- Maximum Level
	"u"		// 5 	- Skill Line
	"u"		// 6 	- Tameable
	"u"		// 7 	- Pet Diet Flags
	"u"		// 8 	- Pet Talent Type
	"x"		// 9 	- Some kind of Category
	"s"		// 10 	- Name
	"x";	// 11 	- Icon

SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
const char* creaturespelldataFormat = 
	"u"		// 0 	- ID
	"uuuu"	// 1-4 	- Spell IDs
	"uuuu";	// 5-8	- Availability

SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
const char* dbctaxinodeFormat =
	"u"		// 0 - ID
	"u"		// 1 - Map ID
	"f"		// 2 - X
	"f"		// 3 - Y
	"f"		// 4 - Z
	"x"		// 5 - Name
	"u"		// 6 - Horde Mount
	"u";	// 7 - Alliance Mount

SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
const char* dbctaxipathFormat = 
		"u"		// 0 - ID
		"u"		// 1 - From Node
		"u"		// 2 - To Node
		"u";	// 3 - Price

SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
const char* dbctaxipathnodeFormat = 
	"u"		// 0 	- ID
	"u"		// 1 	- Path
	"u"		// 2 	- Sequence
	"u"		// 3 	- Map ID
	"f"		// 4 	- X
	"f"		// 5 	- Y
	"f"		// 6 	- Z
	"u"		// 7 	- Flag
	"u"		// 8 	- Delay
	"x"		// 9 	- Arrival Event ID
	"x";	// 10 	- Departure Event ID

/** Cataclysm Format Currently Unknown **/
SERVER_DECL DBCStorage<DestructibleModelDataEntry> dbcDestructibleModelDataEntry;
const char* DestructibleModelDataFormat =
	"u"
	"uxxx"	// ID and xx
	"uxxxx"	// Display Id 1
	"uxxxx"	// Display Id 2
	"uxxxx"	// Display Id 3
	"uxxx";	// Display Id 4

SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
const char* durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";

SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
const char* durabilityqualityFormat = "uf";

SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
const char* EmoteEntryFormat = "uxuxxxxxxxxxxxxxxxx";

SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
const char* EnchantEntrYFormat = "uxuuuuuuuuuuuusuuuuxxxx";

SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
const char* factiondbcFormat = "uiuuuuxxxxiiiixxxxuxxxxsxx";

SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";

SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
const char* GemPropertyEntryFormat = "uuuuux";

SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
const char* GlyphPropertyEntryFormat = "uuuu";

SERVER_DECL DBCStorage<SpellDifficultyEntry> dbcSpellDifficulty;
const char* spelldifficultyFormat = "uuuuu";

SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
const char* gtfloatformat = "xf";

SERVER_DECL DBCStorage<ItemEntry> dbcItem;
const char* itemFormat = "uuuiiuuu";

SERVER_DECL DBCStorage<ItemExtendedCostEntry> dbcItemExtendedCost;
const char* itemextendedcostFormat = "uuuuuuuuuuuuuuuxxxxxxxxxxxxxxxx";

SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
const char* itemrandomsuffixformat = "usxuuuuuuuuuu";

SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
const char* ItemSetFormat = "uxuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";

SERVER_DECL DBCStorage<Lock> dbcLock;
const char* LockFormat = "uuuuuuxxxuuuuuxxxuuuuuxxxxxxxxxxx";

SERVER_DECL DBCStorage<MapEntry> dbcMap;
const char* mapentryFormat =
	"u"					// 0	- id
	"x"					// 1	- name_internal
	"u"					// 2	- map_type
	"x"					// 3	- is_pvp_zone
	"x"					// 4	- 0 or 1 for battlegrounds (not arenas)
	"x"					// 5	- real_name
	"x"					// 6	- linked_zone
	"x" 				// 7	- hordeIntro
	"x" 				// 8	- allianceIntro
	"u"					// 9	- multimap_id
	"x"					// 10	- unk_float (all 1 but arathi 1.25)
	"x"					// 11	- parent_map
	"x"					// 12	- start_x
	"x"					// 13	- start_y
	"x"					// 14	- unk
	"x"					// 15	- addon
	"x"					// 16	- normalReqText
	"u"					// 17	- Max players
	"x";				// 18 	- MaNGOS: Map ID, related to phasing.
	
SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
const char* randompropsFormat = "usuuuuux";

SERVER_DECL DBCStorage<ScalingStatDistributionEntry> dbcScalingStatDistribution;
const char* scalingstatdistributionformat =
	"u" // ID
	"iiiiiiiiii" // Stat Mod
	"uuuuuuuuuu" // Modifier
	"x"
	"u"; // Max Level
	
SERVER_DECL DBCStorage<ScalingStatValuesEntry> dbcScalingStatValues;
const char* scalingstatvaluesformat =
	"x" // Id
	"u" // Level
	"uuuuuu" // DPS mod
	"u" // Spell Power
	"uuuuu" // ScalingStatD modifier
	"uuuu" // Armor Mod
	"uuuu" // Armor Type[level]
	"xxxxxxxxxxxxxxxxxxxxxxx" // UNUSED
	"xx"; // unk
	
SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
const char* skilllineentrYFormat = "uuusxxxx";

SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
const char* skilllinespellFormat = "uuuuuxxuuuuuxx";

SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProps;
const char* SummonPropertiesfmt = "uuuuuu";

SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
const char* talententryFormat = "uuuuuuuuuuxxuxxxuxx";

SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
const char* talenttabentryFormat = "uxxuuuxxxxx";

SERVER_DECL DBCStorage<VehicleEntry> dbcVehicle;
const char* vehicleentryFormat = "uxxxxxuuuuuuuuxxxxxxxxxxxxxxxxxxxxxxuxxx";

SERVER_DECL DBCStorage<VehicleSeatEntry> dbcVehicleSeat;
const char* vehicleseatentryFormat = "uuxfffxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuxxxxxxxxxxxxxxxxxxx";

SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOAreaTable;
const char* WMOAreaEntryfmt="niiixxxxxiixxxx";

SERVER_DECL DBCStorage<WorldMapOverlayEntry> dbcWorldMapOverlay;
const char* WorldMapOverlayfmt="uxuxxxxxxxxxxxxxx";

//=========================================================
// Spell Handling DBCs
//=========================================================

SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
const char* spellcasttimeFormat = "uuxx";

SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
const char* spelldurationFormat = "uiii";
	
SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
const char* spellradiusFormat = "ufxf";

SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
const char* spellrangeFormat =
	"u"					// Id
	"f"					// MinRange
	"f"
	"f"					// MaxRange
	"f"
	"u"
	"x"
	"x";

SERVER_DECL DBCStorage<SpellRuneCostEntry> dbcSpellRuneCost;
const char* SpellRuneCostfmt="uuuuu";

SERVER_DECL DBCStorage<SpellShapeshiftForm> dbcSpellShapeshiftForm;
const char* spellshapeshiftformformat = "uxxxxxxxxxxuuuuuuuu";

SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
const char* spellentryFormat =
	"u"			// Id
	"uuuuuuuu"	// Flags
	"xx"		// 400 Unk[2]
	"u"			// CastingTimeIndex
	"u"			// DurationIndex
	"f"			// Speed
	"u"			// rangeIndex
	"u"			// powerType
	"ux"		// SpellVisual, second one is unnessessary.
	"u"			// SpellIconID
	"u"			// activeIconID
	"s"			// Name
	"s"			// Rank
	"s"			// Spell Description
	"x"			// Buff Description
	"u"			// School
	"u"			// runeCostID
	"x"			// SpellMissileID
	"u"			// SpellDescriptionVariableID
	"u"			// SpellDifficulty
	"x"			// unk_f1
	"u"			// SpellScalingId
	"u"			// SpellAuraOptionsId
	"u"			// SpellAuraRestrictionsId
	"u"			// SpellCastingRequirementsId
	"u"			// SpellCategoriesId
	"u"			// SpellClassOptionsId
	"u"			// SpellCooldownsId
	"x"			// unkIndex7
	"u"			// SpellEquippedItemsId
	"u"			// SpellInterruptsId
	"u"			// SpellLevelsId
	"u"			// SpellPowerId
	"u"			// SpellReagentsId
	"u"			// SpellShapeshiftId
	"u"			// SpellTargetRestrictionsId
	"u"			// SpellTotemsId
	"x";		// ResearchProject

SERVER_DECL DBCStorage<SpellScalingEntry> dbcSpellScaling;
const char* SpellScalingfmt =
"u"			// 0: Spell ID
"uuuuuuuu"	// 1 - 8: Reagent ID
"uuuuuuuu";	// 9 - 16: Reagent Count

SERVER_DECL DBCStorage<SpellReagentEntry> dbcSpellReagents;
const char* SpellReagentfmt =
"u"			// 0: Spell ID
"uuuuuuuu"	// 1 - 8: Reagent ID
"uuuuuuuu";	// 9 - 16: Reagent Count

SERVER_DECL DBCStorage<SpellTargetRestrict> dbcSpellTargetRestrict;
const char* spelltargetrestrictionfmt = 
"x"		// ID
"u"		// MaxTargets
"u"		// MaxTargetLevel
"u"		// TargetCreatureType
"u";	// Targets

SERVER_DECL DBCStorage<SpellAuraOptionEntry> dbcSpellAuraOptions;
const char* SpellAuraOptionsfmt =
"u"		// 0 - ID
"u"		// 1 - maxstack
"u"		// 2 - procChance
"u"		// 3 - procCharges
"u";	// 4 - procFlags

SERVER_DECL DBCStorage<SpellAuraRestrictionEntry> dbcSpellAuraRestrict;
const char* SpellAuraRestrictfmt =
"u"		// 0 - ID
"u"		// 1 - CasterAuraState
"u"		// 2 - TargetAuraState
"u"		// 3 - CasterAuraStateNot
"u"		// 4 - TargetAuraStateNot
"u"		// 5 - casterAuraSpell
"u"		// 6 - targetAuraSpell
"u"		// 7 - excludeCasterAuraSpell
"u";	// 8 - excludeTargetAuraSpell

SERVER_DECL DBCStorage<SpellCastingRequirementEntry> dbcSpellCastingReq;
const char* SpellCastingReqfmt =
"u"		// 0 - ID
"u"		// 1 - FacingCasterFlags
"u"		// 2 - MinFactionID
"u"		// 3 - MinReputation
"u"		// 4 - AreaGroupId
"u"		// 5 - RequiredAuraVision
"u";	// 6 - RequiresSpellFocus

SERVER_DECL DBCStorage<SpellCategoriesEntry> dbcSpellCategories;
const char* SpellCategoriesfmt =
"u"		// 0 - ID;
"u"		// 1 - Category;
"u"		// 2 - Spell_Dmg_Type;
"u"		// 3 - DispelType;
"u"		// 4 - MechanicsType;
"u"		// 5 - PreventionType;
"u";	// 6 - StartRecoveryCategory;

SERVER_DECL DBCStorage<SpellClassOptionEntry> dbcSpellClassOptions;
const char* SpellClassOptionsfmt =
"u"		// 0: ID
"u"		// 1: modalNextSpell
"uuu"	// 2 - 4: SpellGroupType[3]
"x"		// 5: Unk
"u";	// 6: SpellFamilyName

SERVER_DECL DBCStorage<SpellCooldownEntry> dbcSpellCooldowns;
const char* SpellCooldownsfmt =
"u"		// 0 - ID
"u"		// 1 - CategoryRecoveryTime
"u"		// 2 - RecoveryTime
"u";	// 3 - StartRecoveryTime

SERVER_DECL DBCStorage<SpellEffectEntry> dbcSpellEffect;
const char* SpellEffectfmt =
"u"		// 0: ID;
"u"		// 1: Effect;
"f"		// 2: float EffectMultipleValue;
"u"		// 3: EffectApplyAuraName;
"u"		// 4: EffectAmplitude;
"u"		// 5: EffectBasePoints;
"x"		// 6: float unk
"f"		// 7: float DmgMultiplier;
"u"		// 8: EffectChainTarget;
"u"		// 9: EffectDieSides;
"u"		// 10: EffectItemType;
"u"		// 11: EffectMechanic;
"u"		// 12: EffectMiscValue;
"u"		// 13: EffectMiscValueB;
"f"		// 14: EffectPointsPerComboPoint;
"u"		// 15: EffectRadiusIndex;
"x"   	// 16: unk_400
"f"		// 17: EffectRealPointsPerLevel;
"uuu"	// 18 - 20: EffectSpellClassMaskA[3];
"u"		// 21: EffectTriggerSpell;
"u"		// 22: EffectImplicitTargetA;
"u"		// 23: EffectImplicitTargetB;
"u"		// 24: EffectSpellId;
"u";	// 25: EffectIndex;

#endif

template<class T>
bool loader_stub(const char * filename, const char * format, bool ind, T& l, bool loadstrs)
{
	Log.Notice("DBC", "Loading %s.", filename);
	return l.Load(filename, format, ind, loadstrs);
}

#define LOAD_DBC(filename, format, ind, stor, strings) if(!loader_stub(filename, format, ind, stor, strings)) { return false; } 

bool LoadRSDBCs()
{
	/* Needed for: */
	LOAD_DBC("DBC/AreaTable.dbc", areatableFormat, true, dbcArea, true);
	/* Needed for: */
	LOAD_DBC("DBC/ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, true);
	return true;
}

bool LoadDBCs()
{
	/* Needed for: Used in loading of achievements and finding saving information and grabbing criteria
	info to see if player deserves achievement. */
	LOAD_DBC("DBC/Achievement.dbc", achievementfmt,true, dbcAchievement,true);
	/* Needed for: */
	LOAD_DBC("DBC/Achievement_Criteria.dbc", achievementCriteriafmt,true,dbcAchievementCriteria,true);
	/* Needed for: */
	LOAD_DBC("DBC/AreaGroup.dbc", AreaGroupFormat, true, dbcAreaGroup, true);
	/* Needed for: */
	LOAD_DBC("DBC/AreaTable.dbc", areatableFormat, true, dbcArea, true);
	/* Needed for: */
	LOAD_DBC("DBC/AuctionHouse.dbc", auctionhousedbcFormat, true, dbcAuctionHouse, false);
	/* Needed for: */
	LOAD_DBC("DBC/BankBagSlotPrices.dbc", bankslotpriceformat, true, dbcBankSlotPrices, false);
	/* Needed for: */
	LOAD_DBC("DBC/BarberShopStyle.dbc", barbershopstyleFormat, true, dbcBarberShopStyle, true);
	/* Needed for: */
	LOAD_DBC("DBC/ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, true);
#ifndef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/CurrencyTypes.dbc", CurrencyTypesEntryFormat, true, dbcCurrencyTypesStore, true);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/ChrClasses.dbc", charclassFormat, true, dbcCharClass, true);
	/* Needed for: */
	LOAD_DBC("DBC/ChrRaces.dbc", charraceFormat, true, dbcCharRace, true);
	/* Needed for: */
	LOAD_DBC("DBC/CreatureDisplayInfo.dbc", creaturedisplayFormat, true, dbcCreatureDisplayInfo, true);
	/* Needed for: */
	LOAD_DBC("DBC/CreatureFamily.dbc", creaturefamilyFormat, true, dbcCreatureFamily, true);
	/* Needed for: */
	LOAD_DBC("DBC/CreatureSpellData.dbc", creaturespelldataFormat, true, dbcCreatureSpellData, false);
	/* Needed for: */
	LOAD_DBC("DBC/DurabilityQuality.dbc", durabilityqualityFormat, true, dbcDurabilityQuality, false);
	/* Needed for: */
	LOAD_DBC("DBC/DurabilityCosts.dbc", durabilitycostsFormat, true, dbcDurabilityCosts, false);
	/* Needed for: */
	LOAD_DBC("DBC/EmotesText.dbc", EmoteEntryFormat, true, dbcEmoteEntry, false);
	/* Needed for: */
	LOAD_DBC("DBC/Faction.dbc", factiondbcFormat, true, dbcFaction, true);
	/* Needed for: */
	LOAD_DBC("DBC/FactionTemplate.dbc", factiontemplatedbcFormat, true, dbcFactionTemplate, false);
	/* Needed for: */
	LOAD_DBC("DBC/GemProperties.dbc", GemPropertyEntryFormat, true, dbcGemProperty, false);
	/* Needed for: */
	LOAD_DBC("DBC/GlyphProperties.dbc", GlyphPropertyEntryFormat, true, dbcGlyphProperty, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtBarberShopCostBase.dbc", gtfloatformat, false, dbcBarberShopPrices, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtChanceToMeleeCrit.dbc", gtfloatformat, false, dbcMeleeCrit, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtChanceToMeleeCritBase.dbc", gtfloatformat, false, dbcMeleeCritBase, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtChanceToSpellCrit.dbc", gtfloatformat, false, dbcSpellCrit, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtChanceToSpellCritBase.dbc", gtfloatformat, false, dbcSpellCritBase, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtCombatRatings.dbc", gtfloatformat, false, dbcCombatRating, false);
#ifndef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/gtOCTRegenHP.dbc", gtfloatformat, false, dbcHPRegen, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/gtOCTRegenMP.dbc", gtfloatformat, false, dbcManaRegen, false);
#ifndef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/gtRegenHPPerSpt.dbc", gtfloatformat, false, dbcHPRegenBase, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/gtRegenMPPerSpt.dbc", gtfloatformat, false, dbcManaRegenBase, false);
	/* Needed for: */
	LOAD_DBC("DBC/Item.dbc", itemFormat, true, dbcItem, false);
	/* Needed for: */
	LOAD_DBC("DBC/ItemExtendedCost.dbc", itemextendedcostFormat, true, dbcItemExtendedCost, false);
	/* Needed for: */
	LOAD_DBC("DBC/ItemSet.dbc", ItemSetFormat, true, dbcItemSet, true);
	/* Needed for: */
	LOAD_DBC("DBC/ItemRandomProperties.dbc", randompropsFormat, true, dbcRandomProps, false);
	/* Needed for: */
	LOAD_DBC("DBC/ItemRandomSuffix.dbc", itemrandomsuffixformat, true, dbcItemRandomSuffix, false);
	/* Needed for: */
	LOAD_DBC("DBC/Lock.dbc", LockFormat, true, dbcLock, false);
	/* Needed for: */
	LOAD_DBC("DBC/Map.dbc", mapentryFormat, true, dbcMap, true);
	/* Needed for: */
	LOAD_DBC("DBC/ScalingStatDistribution.dbc", scalingstatdistributionformat, true, dbcScalingStatDistribution, false);
	/* Needed for: */
	LOAD_DBC("DBC/ScalingStatValues.dbc", scalingstatvaluesformat, true, dbcScalingStatValues, false);
#ifndef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/StableSlotPrices.dbc", bankslotpriceformat, true, dbcStableSlotPrices, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/SkillLine.dbc", skilllineentrYFormat, true, dbcSkillLine, true);
	/* Needed for: */
	LOAD_DBC("DBC/SkillLineAbility.dbc", skilllinespellFormat, false, dbcSkillLineSpell, false);
	/* Needed for: */
	LOAD_DBC("DBC/Spell.dbc", spellentryFormat, true, dbcSpell, true);
#ifdef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/SpellAuraOptions.dbc", SpellAuraOptionsfmt, true, dbcSpellAuraOptions, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellAuraRestrictions.dbc", SpellAuraRestrictfmt, true, dbcSpellAuraRestrict, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellCastingRequirements.dbc", SpellCastingReqfmt, true, dbcSpellCastingReq, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/SpellCastTimes.dbc", spellcasttimeFormat, true, dbcSpellCastTime, false);
#ifdef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/SpellCategories.dbc", SpellCategoriesfmt, true, dbcSpellCategories, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellClassOptions.dbc", SpellClassOptionsfmt, true, dbcSpellClassOptions, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellCooldowns.dbc", SpellCooldownsfmt, true, dbcSpellCooldowns, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/SpellDifficulty.dbc", spelldifficultyFormat, true, dbcSpellDifficulty, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellDuration.dbc", spelldurationFormat, true, dbcSpellDuration, false);
#ifdef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/SpellEffect.dbc", SpellEffectfmt, true, dbcSpellEffect, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/SpellItemEnchantment.dbc", EnchantEntrYFormat, true, dbcEnchant, true);
	/* Needed for: */
	LOAD_DBC("DBC/SpellRadius.dbc", spellradiusFormat, true, dbcSpellRadius, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellRange.dbc", spellrangeFormat, true, dbcSpellRange, false);
#ifdef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/SpellReagents.dbc", SpellReagentfmt, true, dbcSpellReagents, false);
#endif
	/* Needed for: Spell costs and calculations for dummy scripts or scripted spells for DK's. */
	LOAD_DBC("DBC/SpellRuneCost.dbc", SpellRuneCostfmt, true, dbcSpellRuneCost, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellShapeshiftForm.dbc", spellshapeshiftformformat, true, dbcSpellShapeshiftForm, false);
#ifdef CATACLYSM
	/* Needed for: */
	LOAD_DBC("DBC/SpellTargetRestrictions.dbc", spelltargetrestrictionfmt, true, dbcSpellTargetRestrict, false);
#endif
	/* Needed for: */
	LOAD_DBC("DBC/SummonProperties.dbc", SummonPropertiesfmt, true, dbcSummonProps, false);
	/* Needed for: */
	LOAD_DBC("DBC/Talent.dbc", talententryFormat, true, dbcTalent, false);
	/* Needed for: */
	LOAD_DBC("DBC/TalentTab.dbc", talenttabentryFormat, true, dbcTalentTab, false);
	/* Needed for: */
	LOAD_DBC("DBC/TaxiNodes.dbc", dbctaxinodeFormat, false, dbcTaxiNode, false);
	/* Needed for: */
	LOAD_DBC("DBC/TaxiPath.dbc", dbctaxipathFormat, false, dbcTaxiPath, false);
	/* Needed for: */
	LOAD_DBC("DBC/TaxiPathNode.dbc", dbctaxipathnodeFormat, false, dbcTaxiPathNode, false);
	/* Needed for: */
	LOAD_DBC("DBC/Vehicle.dbc", vehicleentryFormat, true, dbcVehicle, true);
	/* Needed for: */
	LOAD_DBC("DBC/VehicleSeat.dbc", vehicleseatentryFormat, true, dbcVehicleSeat, true);
	/* Needed for: */
	LOAD_DBC("DBC/WorldMapOverlay.dbc", WorldMapOverlayfmt, true, dbcWorldMapOverlay, true);
	/* Needed for: */
	LOAD_DBC("DBC/WMOAreaTable.dbc", WMOAreaEntryfmt, true, dbcWMOAreaTable, true);
	/* Needed for: */
	LOAD_DBC("DBC/DestructibleModelData.dbc", DestructibleModelDataFormat, true, dbcDestructibleModelDataEntry, false);
	return true;
}
