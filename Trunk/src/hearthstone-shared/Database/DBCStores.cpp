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

// Crow: If it has more than 40 columns, break em up.

#include "DBCStores.h"
#include "DataStore.h"
#include "NGLog.h"

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
SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProps;
SERVER_DECL DBCStorage<ScalingStatDistributionEntry> dbcScalingStatDistribution;
SERVER_DECL DBCStorage<ScalingStatValuesEntry> dbcScalingStatValues;

const char* SummonPropertiesfmt = "uuuuuu";
const char* AreaGroupFormat="niiiiiii";
const char* CurrencyTypesEntryFormat = "xnxu";
const char* ItemSetFormat = "uxxxxxxxxxxxxxxxxxuuuuuuuuuuxxxxxxxuuuuuuuuuuuuuuuuuu";
const char* LockFormat = "uuuuuuxxxuuuuuxxxuuuuuxxxxxxxxxxx";
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
	"xxxxx"; // Unk

const char* itemextendedcostFormat = "uuuuuuuuuuuuuuux";
const char* talententryFormat = "uuuuuuuuuxxxxuxxuxxxuxx";
const char* talenttabentryFormat = "uxxxxxxxxxxxxxxxxxxxuxux";
const char* spellcasttimeFormat = "uuxx";
const char* spellradiusFormat = "ufxf";
const char* spellrangeFormat =
	"u"					// Id
	"f"					// MinRange
	"x"
	"f"					// MaxRange
	"x"
	"u"
	"sxxxxxxxxxxxxxxxx"
	"sxxxxxxxxxxxxxxxx";

const char* WorldMapOverlayfmt="uxuxxxxxxxxxxxxxx";
const char* SpellRuneCostfmt="uuuuu";
const char* spelldurationFormat = "uiii";
const char* randompropsFormat = "usuuuxxxxxxxxxxxxxxxxxxx";
const char* areatableFormat = "uxuuuxxxxxusxxxxxxxxxxxxxxxxuxxxxxxx";
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";
const char* auctionhousedbcFormat = "uuuuxxxxxxxxxxxxxxxxx";
const char* factiondbcFormat = "uiuuuuxxxxiiiixxxxxxxxusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
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
	"x";				// 65 Max players

const char* itemrandomsuffixformat = "usxxxxxxxxxxxxxxxxxuuuuuuuuuu";
const char* chatchannelformat = "uuxsxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* durabilityqualityFormat = "uf";
const char* durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char* bankslotpriceformat = "uu";
const char* barbershopstyleFormat="nusxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuu";
const char* gtfloatformat = "f";
const char* spellshapeshiftformformat = "uxxxxxxxxxxxxxxxxxxxxxxxxxxxuuuuuuu";

const char* vehicleseatentryFormat = "uuxfffxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxuuxxxxxxxxxxxx";
const char* vehicleentryFormat = "uxxxxxuuuuuuuuxxxxxxxxxxxxxxxxxxxxxxxxxx";

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

template<class T>
bool loader_stub(const char * filename, const char * format, bool ind, T& l, bool loadstrs)
{
	Log.Notice("DBC", "Loading %s.", filename);
	return l.Load(filename, format, ind, loadstrs);
}

#define LOAD_DBC(filename, format, ind, stor, strings) if(!loader_stub(filename, format, ind, stor, strings)) { return false; } 

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
	/* Needed for: */
	LOAD_DBC("DBC/CurrencyTypes.dbc", CurrencyTypesEntryFormat, true, dbcCurrencyTypesStore, true);
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
	/* Needed for: */
	LOAD_DBC("DBC/gtOCTRegenHP.dbc", gtfloatformat, false, dbcHPRegen, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtOCTRegenMP.dbc", gtfloatformat, false, dbcManaRegen, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtRegenHPPerSpt.dbc", gtfloatformat, false, dbcHPRegenBase, false);
	/* Needed for: */
	LOAD_DBC("DBC/gtRegenMPPerSpt.dbc", gtfloatformat, false, dbcManaRegenBase, false);
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
	/* Needed for: */
	LOAD_DBC("DBC/StableSlotPrices.dbc", bankslotpriceformat, true, dbcStableSlotPrices, false);
	/* Needed for: */
	LOAD_DBC("DBC/SkillLine.dbc", skilllineentrYFormat, true, dbcSkillLine, true);
	/* Needed for: */
	LOAD_DBC("DBC/SkillLineAbility.dbc", skilllinespellFormat, false, dbcSkillLineSpell, false);
	/* Needed for: */
	LOAD_DBC("DBC/Spell.dbc", spellentryFormat, true, dbcSpell, true);
	/* Needed for: */
	LOAD_DBC("DBC/SpellCastTimes.dbc", spellcasttimeFormat, true, dbcSpellCastTime, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellDuration.dbc", spelldurationFormat, true, dbcSpellDuration, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellItemEnchantment.dbc", EnchantEntrYFormat, true, dbcEnchant, true);
	/* Needed for: */
	LOAD_DBC("DBC/SpellRadius.dbc", spellradiusFormat, true, dbcSpellRadius, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellRange.dbc", spellrangeFormat, true, dbcSpellRange, false);
	/* Needed for: Spell costs and calculations for dummy scripts or scripted spells for DK's. */
	LOAD_DBC("DBC/SpellRuneCost.dbc", SpellRuneCostfmt, true, dbcSpellRuneCost, false);
	/* Needed for: */
	LOAD_DBC("DBC/SpellShapeshiftForm.dbc", spellshapeshiftformformat, true, dbcSpellShapeshiftForm, false);
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

	return true;
}
