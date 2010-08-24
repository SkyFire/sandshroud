/*
 * Sun++ Scripts for Sandshroud MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2009-2011 Sandshroud <http://www.sandshroud.org/>
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

#ifndef __TABLEFUNCTIONS_H
#define __TABLEFUNCTIONS_H

#include "../LUAEngine.h"
// Crow: Some of the following functions are based off of functions created by Hypersniper of LuaHypeArc.
// Areas where this applies, credit has been given in the form of a name declaration.
#include "FunctionGameObjects.h"
#include "FunctionGlobal.h"
#include "FunctionItems.h"
//#include "FunctionPacket.h"
//#include "FunctionSpell.h"
//#include "FunctionAura.h"
#include "FunctionTaxi.h"
#include "FunctionUnits.h"

template<typename T>
struct RegType
{
	const char * name;
	int(*mfunc)(lua_State*,T*);
};

// Crow: Some of the following functions are based off of functions created by Hypersniper of LuaHypeArc.
// Areas where this applies, credit has been given in the form of a name declaration.
/************************************************************************/
/* SCRIPT FUNCTION TABLES                                               */
/************************************************************************/
RegType<Item> ItemMethods[] = 
{
	{ "GossipCreateMenu", &luaItem_GossipCreateMenu },
	{ "GossipMenuAddItem", &luaItem_GossipMenuAddItem },
	{ "GossipSendMenu", &luaItem_GossipSendMenu },
	{ "GossipComplete", &luaItem_GossipComplete },
	{ "GossipSendPOI", &luaItem_GossipSendPOI },
	{ "PerformWorldDBQuery", &luaItem_PerformWorldDBQuery },
	{ "PerformCharDBQuery", &luaItem_PerformCharDBQuery },
	{ "GetOwner", &luaItem_GetOwner },
	{ "AddEnchantment", &luaItem_AddEnchantment },
	{ "RemoveEnchantment", &luaItem_RemoveEnchantment },
	{ "GetEntryId", &luaItem_GetEntryId },
	{ "GetName", &luaItem_GetName },
	{ "GetSpellId", &luaItem_GetSpellId },
	{ "GetSpellTrigger", &luaItem_GetSpellTrigger },
	{ "GetGUID", &luaItem_GetGUID },
	{ "AddLoot", &luaItem_AddLoot},
	{ "SetByteValue", &luaItem_SetByteValue },
	{ "GetByteValue", &luaItem_GetByteValue },
	{ "GetItemLevel", &luaItem_GetItemLevel },
	{ "GetRequiredLevel", &luaItem_GetRequiredLevel },
	{ "GetBuyPrice", &luaItem_GetBuyPrice },
	{ "GetSellPrice", &luaItem_GetSellPrice },
	{ "RepairItem", &luaItem_RepairItem },
	{ "GetMaxDurability", &luaItem_GetMaxDurability },
	{ "GetDurability", &luaItem_GetDurability },
	{ "HasEnchantment", &luaItem_HasEnchantment },
	{ "ModifyEnchantmentTime", &luaItem_ModifyEnchantmentTime },
	{ "SetStackCount", &luaItem_SetStackCount },
	{ "HasFlag", &luaItem_HasFlag },
	{ "IsSoulbound", &luaItem_IsSoulbound },
	{ "IsAccountbound", &luaItem_IsAccountbound },
	{ "IsContainer", &luaItem_IsContainer },
	{ "GetContainerItemCount", &luaItem_GetContainerItemCount },
	{ "GetEquippedSlot", &luaItem_GetEquippedSlot },
	{ "GetObjectType", &luaItem_GetObjectType },
	{ "Remove", &luaItem_Remove },
	{ "Create", &luaItem_Create },
	{ NULL, NULL },
};

RegType<Unit> UnitMethods[] = 
{
	{ "GossipCreateMenu", &luaUnit_GossipCreateMenu },
	{ "GossipMenuAddItem", &luaUnit_GossipMenuAddItem },
	{ "GossipSendMenu", &luaUnit_GossipSendMenu },
	{ "GossipComplete", &luaUnit_GossipComplete },
	{ "GossipSendPOI", &luaUnit_GossipSendPOI },
	{ "GetName", &luaUnit_GetName },
	{ "SendChatMessage", &luaUnit_SendChatMessage },
	{ "MoveTo", &luaUnit_MoveTo },
	{ "SetMovementType", &luaUnit_SetMovementType },
	{ "CastSpell", &luaUnit_CastSpell },
	{ "FullCastSpell", &luaUnit_FullCastSpell },
	{ "CastSpellOnTarget", &luaUnit_CastSpellOnTarget },
	{ "FullCastSpellOnTarget", &luaUnit_FullCastSpellOnTarget },
	{ "SpawnCreature", &luaUnit_SpawnCreature },
	{ "SpawnGameObject", &luaUnit_SpawnGameObject },
	{ "GetX", &luaUnit_GetX },
	{ "GetY", &luaUnit_GetY },
	{ "GetZ", &luaUnit_GetZ },
	{ "GetO", &luaUnit_GetO },
	{ "IsPlayer", &luaUnit_IsPlayer },
	{ "IsCreature", &luaUnit_IsCreature },
	{ "RegisterEvent", &luaUnit_RegisterEvent },
	{ "RemoveEvents", &luaUnit_RemoveEvents },
	{ "SendBroadcastMessage", &luaUnit_SendBroadcastMessage },
	{ "SendAreaTriggerMessage", &luaUnit_SendAreaTriggerMessage },
	{ "KnockBack", &luaUnit_KnockBack },
	{ "MarkQuestObjectiveAsComplete", &luaUnit_MarkQuestObjectiveAsComplete },
	{ "LearnSpell", &luaUnit_LearnSpell },
	{ "UnlearnSpell", &luaUnit_UnlearnSpell },
	{ "HasFinishedQuest", &luaUnit_HasFinishedQuest },
	{ "GetItemCount", &luaUnit_GetItemCount },
	{ "IsInCombat", &luaUnit_IsInCombat },
	{ "GetMainTank", &luaUnit_GetMainTank },
	{ "GetAddTank", &luaUnit_GetAddTank },
	{ "ClearThreatList", &luaUnit_ClearThreatList },
	{ "GetTauntedBy", &luaUnit_GetTauntedBy },
	{ "SetTauntedBy", &luaUnit_SetTauntedBy },
	{ "ChangeTarget", &luaUnit_ChangeTarget },
	{ "GetHealthPct", &luaUnit_GetHealthPct },
	{ "SetHealthPct", &luaUnit_SetHealthPct },
	{ "GetManaPct", &luaUnit_GetManaPct },
	{ "Despawn", &luaUnit_Despawn },
	{ "GetUnitBySqlId", &luaUnit_GetUnitBySqlId },
	{ "PlaySoundToSet", &luaUnit_PlaySoundToSet },
	{ "RemoveAura", &luaUnit_RemoveAura },
	{ "StopMovement", &luaUnit_StopMovement },
	{ "Emote", &luaUnit_Emote },
	{ "GetInstanceID", &luaUnit_GetInstanceID },
	{ "GetClosestPlayer", &luaUnit_GetClosestPlayer },
	{ "GetRandomPlayer", &luaUnit_GetRandomPlayer },
	{ "GetRandomFriend", &luaUnit_GetRandomFriend },
	{ "AddItem", &luaUnit_AddItem },
	{ "RemoveItem", &luaUnit_RemoveItem },
	{ "CreateCustomWaypointMap", &luaUnit_CreateCustomWaypointMap },
	{ "CreateWaypoint", &luaUnit_CreateWaypoint },
	{ "MoveToWaypoint", &luaUnit_MoveToWaypoint },
	{ "DestroyCustomWaypointMap", &luaUnit_DestroyCustomWaypointMap },
	{ "SetCombatCapable", &luaUnit_SetCombatCapable },
	{ "SetCombatMeleeCapable", &luaUnit_SetCombatMeleeCapable },
	{ "SetCombatRangedCapable", &luaUnit_SetCombatRangedCapable },
	{ "SetCombatSpellCapable", &luaUnit_SetCombatSpellCapable },
	{ "SetCombatTargetingCapable", &luaUnit_SetCombatTargetingCapable },
	{ "SetNPCFlags", &luaUnit_SetNPCFlags },
	{ "SetModel", &luaUnit_SetModel },
	{ "SetScale", &luaUnit_SetScale },
	{ "SetFaction", &luaUnit_SetFaction },
	{ "SetStandState",&luaUnit_SetStandState },
	{ "Teleport" , &luaUnit_TeleportUnit },
	{ "GetPlayerClass", &luaUnit_GetPlayerClass },
	{ "ClearThreatList", &luaUnit_ClearHateList },
	{ "WipeThreatList", &luaUnit_WipeHateList },
	{ "WipeTargetList", &luaUnit_WipeTargetList },
	{ "WipeCurrentTarget", &luaUnit_WipeCurrentTarget },
	{ "GetHealth", &luaUnit_GetHealth },
	{ "GetMaxHealth", &luaUnit_GetMaxHealth },
	{ "SetHealth", &luaUnit_SetHealth },
	{ "SetMaxHealth", &luaUnit_SetMaxHealth },
	{ "HasAura", &luaUnit_HasAura },
	{ "Land", &luaUnit_Land },
	{ "SetFlying", &luaUnit_SetFlying },
	{ "SetMana", &luaUnit_SetMana },
	{ "SetMaxMana", &luaUnit_SetMaxMana },
	{ "GetDistance", &luaUnit_GetDistance },
	{ "GetGUID", &luaUnit_GetGUID },
	{ "GetCreatureNearestCoords", &luaUnit_GetCreatureNearestCoords },
    { "CastSpellAoF", &luaUnit_CastSpellAoF },
	{ "GetGameObjectNearestCoords", &luaUnit_GetGameObjectNearestCoords },
	{ "SetInFront", &luaUnit_SetInFront },
	{ "RemoveAllAuras", &luaUnit_RemoveAllAuras },
	{ "ReturnToSpawnPoint", &luaUnit_ReturnToSpawnPoint },
	{ "CancelSpell", &luaUnit_CancelSpell },
	{ "IsAlive", &luaUnit_IsAlive },
	{ "IsDead", &luaUnit_IsDead },
	{ "IsInWorld", &luaUnit_IsInWorld },
	{ "GetZoneId", &luaUnit_GetZoneId },
	{ "GetMana", &luaUnit_GetMana },
	{ "GetMaxMana", &luaUnit_GetMaxMana },
	{ "Root", &luaUnit_Root },
	{ "Unroot", &luaUnit_Unroot },
	{ "IsCreatureMoving", &luaUnit_IsCreatureMoving },
	{ "SetOutOfCombatRange", &luaUnit_SetOutOfCombatRange },
	{ "ModifyRunSpeed", &luaUnit_ModifyRunSpeed },
	{ "ModifyWalkSpeed", &luaUnit_ModifyWalkSpeed },
	{ "ModifyFlySpeed" , &luaUnit_ModifyFlySpeed },
	{ "GetCurrentSpell", &luaUnit_GetCurrentSpell },
	{ "IsFlying", &luaUnit_IsFlying },
	{ "SetRotation", &luaUnit_SetRotation },
	{ "SetOrientation", &luaUnit_SetOrientation },
	{ "GetSpawnX", &luaUnit_GetSpawnX },
	{ "GetSpawnY", &luaUnit_GetSpawnY },
	{ "GetSpawnZ", &luaUnit_GetSpawnZ },
	{ "GetSpawnO", &luaUnit_GetSpawnO },
	{ "GetInRangePlayersCount", &luaUnit_GetInRangePlayersCount },
	{ "GetEntry", &luaUnit_GetEntry },
	{ "SetMoveRunFlag", &luaUnit_SetMoveRunFlag },
	{ "GetAIState", &luaUnit_GetAIState },
	{ "ModUInt32Value", &luaUnit_ModUInt32Value },
	{ "ModFloatValue", &luaUnit_ModFloatValue },
	{ "SetUInt32Value", &luaUnit_SetUInt32Value },
	{ "SetUInt64Value", &luaUnit_SetUInt64Value },
	{ "SetFloatValue", &luaUnit_SetFloatValue },
	{ "GetUInt32Value", &luaUnit_GetUInt32Value },
	{ "GetUInt64Value", &luaUnit_GetUInt64Value },
	{ "GetFloatValue", &luaUnit_GetFloatValue },
	{ "InitPacket", &luaUnit_InitPacket },
	{ "AddDataToPacket", &luaUnit_AddDataToPacket },
	{ "AddGuidDataToPacket", &luaUnit_AddGuidDataToPacket },
	{ "SendData", &luaUnit_SendData },
	{ "AdvanceQuestObjective", &luaUnit_AdvanceQuestObjective },
	{ "Heal", &luaUnit_Heal },
	{ "Energize", &luaUnit_Energize },
	{ "SendChatMessageAlternateEntry", &luaUnit_SendChatMessageAlternateEntry },
	{ "SendChatMessageToPlayer", &luaUnit_SendChatMessageToPlayer },
	{ "SetPowerType", &luaUnit_SetPowerType },
	{ "Strike", &luaUnit_Strike },
	{ "SetAttackTimer", &luaUnit_SetAttackTimer },
	{ "Kill", &luaUnit_Kill },
	{ "DealDamage", &luaUnit_DealDamage },
	{ "IsInFront", &luaUnit_IsInFront },
	{ "IsInBack", &luaUnit_IsInBack },
	{ "IsPacified", &luaUnit_IsPacified },
	{ "IsStunned", &luaUnit_IsStunned },
	{ "IsFeared", &luaUnit_IsFeared },
	{ "CreateGuardian", &luaUnit_CreateGuardian },
	{ "HandleEvent", &luaUnit_HandleEvent },
	{ "HasInRangeObjects", &luaUnit_HasInRangeObjects },
	{ "IsInWater", &luaUnit_IsInWater },
	{ "GetInRangeObjects", &luaUnit_GetInRangeGameObjects },
	{ "GetInRangeObjectsCount", &luaUnit_GetInRangeObjectsCount },
	{ "GetAITargetsCount", &luaUnit_GetAITargetsCount },
	{ "SetUnitToFollow", &luaUnit_SetUnitToFollow },
	{ "GetUnitToFollow", &luaUnit_GetUnitToFollow },
	{ "DismissPet", &luaUnit_DismissPet },
	{ "IsPet", &luaUnit_IsPet },
	{ "SetNextTarget", &luaUnit_SetNextTarget },
	{ "GetNextTarget", &luaUnit_GetNextTarget },
	{ "SetPetOwner", &luaUnit_SetPetOwner },
	{ "GetPetOwner", &luaUnit_GetPetOwner },
	{ "CalcToDistance", &luaUnit_CalcToDistance },
	{ "CalcAngle", &luaUnit_CalcAngle },
	{ "CalcRadAngle", &luaUnit_CalcRadAngle },
	{ "SetFacing", &luaUnit_SetFacing },
	{ "SetDeathState", &luaUnit_SetDeathState },
	{ "IsInvisible", &luaUnit_IsInvisible },
	{ "IsInvincible", &luaUnit_IsInvincible },
	{ "ResurrectPlayer", &luaUnit_ResurrectPlayer },
	{ "KickPlayer", &luaUnit_KickPlayer },
	{ "CanCallForHelp", &luaUnit_CanCallForHelp },
	{ "CallForHelpHp", &luaUnit_CallForHelpHp },
	{ "SetCreatureNameById", &luaUnit_SetCreatureName },
	{ "GetInRangePlayers", &luaUnit_GetInRangePlayers },
	{ "GetUnitByGUID", &luaUnit_GetUnitByGUID },
	{ "RemoveFromWorld", &luaUnit_RemoveFromWorld },
	{ "GetFaction", &luaUnit_GetFaction },
	{ "EnableMoveFly", &luaUnit_MoveFly },
	{ "SpellNonMeleeDamageLog", &luaUnit_SpellNonMeleeDamageLog },
	{ "DisableRespawn", &luaUnit_NoRespawn },
	{ "ModThreat", &luaUnit_ModThreat },
	{ "GetThreat", &luaUnit_GetThreatByPtr },
	{ "GetInRangeFriends", &luaUnit_GetInRangeFriends },
	{ "GetPowerType", &luaUnit_GetPowerType },
	{ "GetMapId", &luaUnit_GetMapId },
	{ "CalcDistance" , &luaUnit_CalcDistance },
	{ "AttackReaction", &luaUnit_AttackReaction },
	{ "EventCastSpell", &luaUnit_EventCastSpell },
	{ "IsPlayerMoving", &luaUnit_IsPlayerMoving },
	{ "IsPlayerAttacking", &luaUnit_IsPlayerAttacking },
	{ "RemoveThreat", &luaUnit_RemoveThreatByPtr },
	{ "SetPlayerAtWar", &luaUnit_SetPlayerAtWar },
	{ "GetFactionStanding", &luaUnit_GetFactionStanding },
	{ "PlaySpellVisual", &luaUnit_PlaySpellVisual },
	{ "GetPlayerLevel", &luaUnit_GetPlayerLevel },
	{ "SetPlayerLevel", &luaUnit_SetPlayerLevel },
	{ "SetStanding", &luaUnit_SetPlayerStanding },
	{ "GetStanding", &luaUnit_GetStanding },
	{ "HasItem", &luaUnit_HasItem },
	{ "AdvanceSkill", &luaUnit_AdvanceSkill },
	{ "AddSkill", &luaUnit_AddSkill },
	{ "RemoveSkill", &luaUnit_RemoveSkill },
	{ "EnableFlyCheat", &luaUnit_FlyCheat },
	{ "AddAssistTarget", &luaUnit_AddAssistTargets },
	{ "GetCurrentSpellId", &luaUnit_GetCurrentSpellId },
	{ "GetPlayerRace", &luaUnit_GetPlayerRace },
	{ "RemoveAurasByMechanic", &luaUnit_RemoveAurasByMechanic },
	{ "RemoveAurasType", &luaUnit_RemoveAurasType },

	/* LuaHypArc: Crow: Judging by the comments, this is contributions by Hypersniper(ofc), Halestorm and Alvanaar. */
	{ "AddAura", &luaUnit_AddAura },
	{ "SetAIState", &luaUnit_SetAIState },
	{ "InterruptSpell", &luaUnit_InterruptSpell },
	{ "RemoveStealth", &luaUnit_RemoveStealth },
	{ "IsPoisoned", &luaUnit_IsPoisoned },
	{ "SetStealthLevel", &luaUnit_SetStealth },
	{ "GetStealthLevel", &luaUnit_GetStealthLevel },
	{ "IsStealthed", &luaUnit_IsStealthed },
	{ "RemoveFlag", &luaUnit_RemoveFlag },
	{ "RegisterAIUpdateEvent", &luaUnit_RegisterAIUpdateEvent },
	{ "ModifyAIUpdateEvent", &luaUnit_ModifyAIUpdateEvent },
	{ "RemoveAIUpdateEvent", &luaUnit_RemoveAIUpdateEvent },
	{ "DeleteWaypoint", &luaUnit_deleteWaypoint },
	{ "DealGoldCost", &luaUnit_DealGoldCost },
	{ "DealGoldMerit", &luaUnit_DealGoldMerit },
	{ "CanUseCommand", &luaUnit_CanUseCommand },
	{ "DeMorph", &luaUnit_DeMorph },
	{ "Attack", &luaUnit_Attack },
	{ "GetTarget", &luaUnit_GetTarget },
	{ "GetSelection", &luaUnit_GetSelection },
	{ "SetMount", &luaUnit_SetMount },
	{ "StartQuest", &luaUnit_StartQuest },
	{ "FinishQuest", &luaUnit_FinishQuest },
	{ "RepairAllPlayerItems", &luaUnit_RepairAllPlayerItems },
	{ "SetKnownTitle", &luaUnit_SetKnownTitle },
	{ "LifeTimeKills", &luaUnit_LifeTimeKills },
	{ "HasTitle", &luaUnit_HasTitle },
	{ "GetMaxSkill", &luaUnit_GetMaxSkill },
	{ "GetCurrentSkill", &luaUnit_GetCurrentSkill },
	{ "HasSkill", &luaUnit_HasSkill },
	{ "GetGuildName", &luaUnit_GetGuildName },
	{ "ClearCooldownForSpell", &luaUnit_ClearCooldownForSpell },
	{ "HasSpell", &luaUnit_HasSpell },
	{ "ClearAllCooldowns", &luaUnit_ClearAllCooldowns },
	{ "ResetAllTalents", &luaUnit_ResetAllTalents },
	{ "GetAccountName", &luaUnit_GetAccountName },
	{ "SavePlayer", &luaUnit_SavePlayer },
	{ "HasQuest", &luaUnit_HasQuest },
	{ "RemovePvPFlag", &luaUnit_RemovePvPFlag },
	{ "RemoveNegativeAuras", &luaUnit_RemoveNegativeAuras },
	{ "GossipMiscAction", &luaUnit_GossipMiscAction },
	{ "EquipWeapons", &luaUnit_EquipWeapons },
	{ "Dismount", &luaUnit_Dismount },
	{ "AdvanceAllSkills", &luaUnit_AdvanceAllSkills },
	{ "GetTeam", &luaUnit_GetTeam },
	{ "Possess", &luaUnit_Possess },
	{ "Unpossess", &luaUnit_Unpossess },
	{ "StartTaxi", &luaUnit_StartTaxi },
	{ "ChannelSpell", &luaUnit_ChannelSpell },
	{ "StopChannel", &luaUnit_StopChannel },
	{ "EnableFlight", &luaUnit_EnableFlight },
	{ "GetCoinage", &luaUnit_GetCoinage },
	{ "FlagPvP", &luaUnit_FlagPvP },
	{ "GetDisplay", &luaUnit_GetDisplay },
	{ "GetNativeDisplay", &luaUnit_GetNativeDisplay },
	{ "IsMounted", &luaUnit_IsMounted },
	{ "GetGameTime", &luaUnit_GetGameTime },
	{ "PlaySoundToPlayer", &luaUnit_PlaySoundToPlayer },
	{ "GetDuelState", &luaUnit_GetDuelState }, 
	{ "SetPosition", &luaUnit_SetPosition},
	{ "CastSpellOnTarget", &luaUnit_CastSpellOnTarget},
	{ "GetLandHeight", &luaUnit_GetLandHeight},
	{ "QuestAddStarter", &luaUnit_QuestAddStarter},
	{ "QuestAddFinisher", &luaUnit_QuestAddFinisher},
	{ "SetPlayerSpeed", &luaUnit_SetPlayerSpeed},
	{ "GiveHonor", &luaUnit_GiveHonor},
	{ "SetBindPoint", &luaUnit_SetBindPoint},
	{ "SoftDisconnect", &luaUnit_SoftDisconnect},
	{ "SetZoneWeather", &luaUnit_SetZoneWeather},
	{ "SetPlayerWeather", &luaUnit_SetPlayerWeather},
	{ "SendPacketToPlayer", &luaUnit_SendPacketToPlayer},
	{ "PlayerSendChatMessage", &luaUnit_PlayerSendChatMessage},
	{ "GetDistanceYards", &luaUnit_GetDistanceYards},
	{ "VendorAddItem", &luaUnit_VendorAddItem},
	{ "VendorRemoveItem", &luaUnit_VendorRemoveItem},
	{ "VendorRemoveAllItems", &luaUnit_VendorRemoveAllItems},
	{ "CreatureHasQuest", &luaUnit_CreatureHasQuest},
	{ "SendVendorWindow", &luaUnit_SendVendorWindow},
	{ "SendTrainerWindow", &luaUnit_SendTrainerWindow},
	{ "SendInnkeeperWindow", &luaUnit_SendInnkeeperWindow},
	{ "SendBankWindow", &luaUnit_SendBankWindow},
	{ "SendAuctionWindow", &luaUnit_SendAuctionWindow},
	{ "SendBattlegroundWindow", &luaUnit_SendBattlegroundWindow},
	{ "GetInventoryItem", &luaUnit_GetInventoryItem},
	{ "GetInventoryItemById", &luaUnit_GetInventoryItemById},
//	{ "PhaseSet", &luaUnit_PhaseSet},
//	{ "PhaseAdd", &luaUnit_PhaseAdd},
//	{ "PhaseDelete", &luaUnit_PhaseDelete},
//	{ "GetPhase", &luaUnit_GetPhase},
	{ "AggroWithInRangeFriends", &luaUnit_AggroWithInRangeFriends},
	{ "MoveRandomArea", &luaUnit_MoveRandomArea},
	{ "SendLootWindow", &luaUnit_SendLootWindow},
	{ "AddLoot", &luaUnit_AddLoot},
//	{ "SetPacified", &luaUnit_SetPacified},
	{ "SpawnVehicle", &luaUnit_SpawnVehicle},
	{ "SetVehicle", &luaUnit_SetVehicle},
	{ "GetVehicle", &luaUnit_GetVehicle},
	{ "RemoveFromVehicle", &luaUnit_RemoveFromVehicle},
	{ "GetVehicleSeat", &luaUnit_GetVehicleSeat},
	{ "IsVehicle", &luaUnit_IsVehicle},
	{ "GetPassengerCount", &luaUnit_GetPassengerCount},
	{ "MoveVehicle", &luaUnit_MoveVehicle},
	{ "SetPlayerLock", &luaUnit_SetPlayerLock},
	{ "GetGroupPlayers", &luaUnit_GetGroupPlayers},
	{ "IsGm", &luaUnit_IsGm},
	{ "GetDungeonDifficulty", &luaUnit_GetDungeonDifficulty},
	{ "GetGroupLeader", &luaUnit_GetGroupLeader},
	{ "SetGroupLeader", &luaUnit_SetGroupLeader},
	{ "AddGroupMember", &luaUnit_AddGroupMember},
	{ "SetDungeonDifficulty", &luaUnit_SetDungeonDifficulty},
	{ "ExpandToRaid", &luaUnit_ExpandToRaid},
	{ "SendPacketToGroup", &luaUnit_SendPacketToGroup},
	{ "IsGroupFull", &luaUnit_IsGroupFull},
	{ "IsGroupedWith", &luaUnit_IsGroupedWith},
	{ "GetTotalHonor", &luaUnit_GetTotalHonor},
	{ "GetHonorToday", &luaUnit_GetHonorToday},
	{ "GetHonorYesterday", &luaUnit_GetHonorYesterday},
	{ "GetArenaPoints", &luaUnit_GetArenaPoints},
	{ "AddArenaPoints", &luaUnit_AddArenaPoints},
	{ "AddLifetimeKills", &luaUnit_AddLifetimeKills},
	{ "GetGender", &luaUnit_GetGender}, //9
	{ "SetGender", &luaUnit_SetGender},
	{ "GetGroupType", &luaUnit_GetGroupType},
	{ "SendPacketToGuild", &luaUnit_SendPacketToGuild },
	{ "GetGuildId", &luaUnit_GetGuildId },
	{ "GetGuildRank", &luaUnit_GetGuildRank },
	{ "SetGuildRank", &luaUnit_SetGuildRank },
	{ "IsInGuild", &luaUnit_IsInGuild }, //5
	{ "SendGuildInvite", &luaUnit_SendGuildInvite },
	{ "DemoteGuildMember", &luaUnit_DemoteGuildMember },
	{ "PromoteGuildMember", &luaUnit_PromoteGuildMember },
	{ "SetGuildMotd", &luaUnit_SetGuildMotd },
	{ "GetGuildMotd", &luaUnit_GetGuildMotd },
	{ "SetGuildInformation", &luaUnit_SetGuildInformation },
	{ "AddGuildMember", &luaUnit_AddGuildMember },
	{ "RemoveGuildMember", &luaUnit_RemoveGuildMember },
	{ "SetPublicNote", &luaUnit_SetPublicNote },
	{ "SetOfficerNote", &luaUnit_SetOfficerNote },
	{ "DisbandGuild", &luaUnit_DisbandGuild },
	{ "ChangeGuildMaster", &luaUnit_ChangeGuildMaster },
	{ "SendGuildChatMessage", &luaUnit_SendGuildChatMessage },
	{ "SendGuildLog", &luaUnit_SendGuildLog },
	{ "GuildBankDepositMoney", &luaUnit_GuildBankDepositMoney },
	{ "GuildBankWithdrawMoney", &luaUnit_GuildBankWithdrawMoney },
	{ "GetInstanceOwner", &luaUnit_GetInstanceOwner },
	{ "GetGmRank", &luaUnit_GetGmRank },
	{ "SetByteValue", &luaUnit_SetByteValue },
	{ "GetByteValue", &luaUnit_GetByteValue },
	{ "IsPvPFlagged", &luaUnit_IsPvPFlagged },
	{ "IsFFAPvPFlagged", &luaUnit_IsFFAPvPFlagged },
	{ "IsFFAFlagged", &luaUnit_IsFFAPvPFlagged },
	{ "GetGuildLeader", &luaUnit_GetGuildLeader },
	{ "GetGuildMemberCount", &luaUnit_GetGuildMemberCount },
	{ "CanAttack", &luaUnit_CanAttack },
	{ "GetInRangeUnits", &luaUnit_GetInRangeUnits },
	{ "GetInRangeEnemies", &luaUnit_GetInRangeEnemies },
	{ "IsFriendly", &luaUnit_IsFriendly },
	{ "MovePlayerTo", &luaUnit_MovePlayerTo },
	{ "GetPlayerMovementVector", &luaUnit_GetPlayerMovementVector},
	{ "UnsetKnownTitle", &luaUnit_UnsetKnownTitle},
//	{ "IsInPhase", &luaUnit_IsInPhase},
	{ "HasFlag", &luaUnit_HasFlag },
	{ "Repop", &luaUnit_Repop },
	{ "SetMovementFlags", &luaUnit_SetMovementFlags },
	{ "GetSpawnId", &luaUnit_GetSpawnId },
	{ "ResetTalents", &luaUnit_ResetTalents },
	{ "SetTalentPoints", &luaUnit_SetTalentPoints },
	{ "GetTalentPoints", &luaUnit_GetTalentPoints },
	{ "EventChat", &luaUnit_EventChat },
	{ "GetEquippedItemBySlot", &luaUnit_GetEquippedItemBySlot },
	{ "GetGuildMembers", &luaUnit_GetGuildMembers },
	{ "AddAchievement", &luaUnit_AddAchievement },
	{ "RemoveAchievement", &luaUnit_RemoveAchievement },
	{ "HasAchievement", &luaUnit_HasAchievement },
	{ "RemoveArenaPoints", &luaUnit_RemoveArenaPoints},
	{ "TakeHonor", &luaUnit_TakeHonor},
//	{ "SetPhase", &luaUnit_PhaseSet},
//	{ "DeletePhase", &luaUnit_PhaseDelete},
//	{ "AddToPhase", &luaUnit_PhaseAdd},
	{ "GetAreaId", &luaUnit_GetAreaId},
	{ "ResetPetTalents", &luaUnit_ResetPetTalents},
	{ "IsDazed", &luaUnit_IsDazed },
	{ "GetAura", &luaUnit_GetAura },
	{ "IsRooted", &luaUnit_IsRooted },
	{ "HasAuraWithMechanic", &luaUnit_HasAuraWithMechanic },
	{ "HasNegativeAura", &luaUnit_HasNegativeAura },
	{ "HasPositiveAura", &luaUnit_HasPositiveAura },
	{ "SetActionBar", &luaUnit_SetActionBar },
	{ "GetClosestEnemy", &luaUnit_GetClosestEnemy },
	{ "GetClosestFriend", &luaUnit_GetClosestFriend },
	{ "IsOnTaxi", &luaUnit_IsOnTaxi },
	{ "GetTaxi", &luaUnit_GetTaxi },
	{ "GetObjectType", &luaUnit_GetObjectType },
	{ "GiveXp", &luaUnit_GiveXp },
	{ "GetPower", &luaUnit_GetPower },
	{ "GetPowerPct", &luaUnit_GetPowerPct },
	{ "GetMaxPower", &luaUnit_GetMaxPower },
	{ "SetPower", &luaUnit_SetPower },
	{ "SetPowerPct", &luaUnit_SetPowerPct },
	{ "SetMaxPower", &luaUnit_SetMaxPower },
	{ "LearnSpells", &luaUnit_LearnSpells },
	{ "GetCurrentWaypoint", &luaUnit_GetCurrentWaypoint },
	{ "GetSelectedGO", &luaUnit_GetSelectedGO },
	{ "FullCastSpell", &luaUnit_FullCastSpell },
	{ "FullCastSpellOnTarget", &luaUnit_FullCastSpellOnTarget },
	{ "DisableMelee", &luaUnit_DisableMelee },
	{ "DisableRanged", &luaUnit_DisableRanged },
	{ "DisableSpells", &luaUnit_DisableSpells },
	{ "DisableCombat", &luaUnit_DisableCombat },
	{ "DisableTargeting", &luaUnit_DisableTargeting },
	{ "IsInGroup", &luaUnit_IsInGroup },
	{ "GetLocation", &luaUnit_GetLocation },
	{ "GetSpawnLocation", &luaUnit_GetSpawnLocation },
	{ "GetPlayerMovementFlags", &luaUnit_GetPlayerMovementFlags},
	{ "GetObject", &luaUnit_GetObject },
	{ "GetSecondHated", &luaUnit_GetSecondHated },
	{ "UseAI", &luaUnit_UseAI },
	{ "FlagFFA", &luaUnit_FlagFFA },
	{ "TeleportCreature", &luaUnit_TeleportCreature },
	{ "IsInDungeon", &luaUnit_IsInDungeon },
	{ "IsInRaid", &luaUnit_IsInRaid },
//	{ "CreateLuaEvent", &luaUnit_CreateLuaEvent },
	{ "IsHostile", &luaUnit_IsHostile },
	{ "IsAttackable", &luaUnit_IsAttackable },
	{ "GetNumWaypoints", &luaUnit_GetNumWaypoints },
	{ "GetWayPointsCount", &luaUnit_GetNumWaypoints },
	{ "GetMoveType", &luaUnit_GetMovementType },
	{ "GetQuestLogSlot", &luaUnit_GetQuestLogSlot },
	{ "GetAuraObject", &luaUnit_GetAuraObject },
	{ "GetAuraStackCount", &luaUnit_GetAuraStackCount },
	{ "AddAuraObject", &luaUnit_AddAuraObject },
	{ "GetAuraObjectById", &luaUnit_GetAuraObjectById },
	{ "GetNativeFaction", &luaUnit_GetNativeFaction },
	{ NULL, NULL },
};


RegType<GameObject> GOMethods[] = 
{
	{ "GossipCreateMenu", &luaGameObject_GossipCreateMenu },
	{ "GossipSendMenu", &luaGameObject_GossipSendMenu },
	{ "GossipComplete", &luaGameObject_GossipComplete },
	{ "GossipSendPOI", &luaGameObject_GossipSendPOI },
	{ "GetX", &luaGameObject_GetX },
	{ "GetY", &luaGameObject_GetY },
	{ "GetZ", &luaGameObject_GetZ },
	{ "GetO", &luaGameObject_GetO },
	{ "PlaySoundToSet", &luaGameObject_PlaySoundToSet },
 	{ "GetName", &luaGameObject_GetName },
	{ "Teleport" , &luaGameObject_TeleportPlr },
	{ NULL, NULL },
};

RegType<TaxiPath> LuaTaxiMethods[] = 
{
	{ "CreateTaxi", &LuaTaxi::CreateTaxi },
	{ "GetNodeCount", &LuaTaxi::GetNodeCount },
	{ "AddPathNode", &LuaTaxi::AddPathNode },
	{ "GetId", &LuaTaxi::GetId },
	{ "GetObjectType", &LuaTaxi::GetObjectType},
	{NULL, NULL},
};

void RegisterGlobalFunctions(lua_State *L)
{
	lua_register(L,"PerformIngameSpawn",&luaGlobalFunctions_PerformIngameSpawn);
	lua_register(L,"GetPlayer",&luaGlobalFunctions_GetPlayer);
	lua_register(L,"GetLUAEngine",&luaGlobalFunctions_GetLUAEngine);
	lua_register(L,"GetLuaEngine",&luaGlobalFunctions_GetLUAEngine);
	lua_register(L,"GetLuaEngineVersion",&luaGlobalFunctions_GetLuaEngineVersion);
	lua_register(L,"GetGameTime",&luaGlobalFunctions_GetGameTime);
	//lua_register(L,"WorldDBQuery",&luaGlobalFunctions_WorldDBQuery);
	//lua_register(L,"CharDBQuery",&luaGlobalFunctions_CharDBQuery);
	//lua_register(L,"WorldDBQueryTable",&luaGlobalFunctions_WorldDBQueryTable);
	//lua_register(L,"CharDBQueryTable",&luaGlobalFunctions_CharDBQueryTable);
	lua_register(L,"SendWorldMessage",&luaGlobalFunctions_SendWorldMessage);
	lua_register(L,"ReloadTable",&luaGlobalFunctions_ReloadTable);
	lua_register(L,"ReloadLuaEngine",&luaGlobalFunctions_ReloadLuaEngine);
	lua_register(L,"Rehash",&luaGlobalFunctions_Rehash);
	lua_register(L,"logcol",&luaGlobalFunctions_logcol);
	lua_register(L,"GetPlayersInWorld",&luaGlobalFunctions_GetPlayersInWorld);
	//lua_register(L,"GetburningemuRevision",&luaGlobalFunctions_GetburningemuRevision);
	lua_register(L,"GetPlayersInMap",&luaGlobalFunctions_GetPlayersInMap);
	lua_register(L,"GetPlayersInZone",&luaGlobalFunctions_GetPlayersInZone);
//	lua_register(L,"SendMail",&luaGlobalFunctions_SendMail);
	lua_register(L, "GetTaxiPath", &luaGlobalFunctions_GetTaxiPath);
	//lua_register(L, "SetDBCSpellVar", &luaGlobalFunctions_SetDBCSpellVar);
	//lua_register(L, "GetDBCSpellVar", &luaGlobalFunctions_GetDBCSpellVar);
	//Lua's bit instructions
	lua_register(L, "bit_and", &luaGlobalFunctions_bit_and);
	lua_register(L, "bit_or", &luaGlobalFunctions_bit_or);
	lua_register(L, "bit_xor", &luaGlobalFunctions_bit_xor);
	lua_register(L, "bit_not", &luaGlobalFunctions_bit_not);
	lua_register(L, "bit_shiftleft", &luaGlobalFunctions_bit_shiftleft);
	lua_register(L, "bit_shiftright", &luaGlobalFunctions_bit_shiftright);

	/*lua_register(L, "RemoveTimedEventsInTable", &luaGlobalFunctions_RemoveTimedEventsInTable);
	lua_register(L, "RemoveTimedEventsWithName", &luaGlobalFunctions_RemoveTimedEventsWithName);
	lua_register(L, "RemoveTimedEvent", &luaGlobalFunctions_RemoveTimedEvent);
	lua_register(L, "HasTimedEvents", &luaGlobalFunctions_HasTimedEvents);
	lua_register(L, "HasTimedEventInTable", &luaGlobalFunctions_HasTimedEventInTable);
	lua_register(L, "HasTimedEventWithName", &luaGlobalFunctions_HasTimedEventWithName);
	lua_register(L, "HasTimedEvent", &luaGlobalFunctions_HasTimedEvent);
	lua_register(L,"GetPlatform",&luaGlobalFunctions_GetPlatform);
	lua_register(L, "NumberToGUID", &luaGlobalFunctions_NumberToGUID);
	lua_register(L, "SendPacketToWorld", &luaGlobalFunctions_SendPacketToWorld);
	lua_register(L, "SendPacketToInstance", &luaGlobalFunctions_SendPacketToInstance);
	lua_register(L, "SendPacketToZone", &luaGlobalFunctions_SendPacketToZone);
	lua_register(L, "SendPacketToChannel", &luaGlobalFunctions_SendPacketToChannel);*/
}

template<typename T> RegType<T>* GetMethodTable() { return NULL; }
template<> RegType<Item>* GetMethodTable<Item>() { return ItemMethods; }
template<> RegType<Unit>* GetMethodTable<Unit>() { return UnitMethods; }
template<> RegType<GameObject>* GetMethodTable<GameObject>() { return GOMethods; }
template<> RegType<TaxiPath>* GetMethodTable<TaxiPath>() { return LuaTaxiMethods; }
//template<> RegType<WorldPacket>* GetMethodTable<WorldPacket>() { return LuaPacketMethods; }
//template<> RegType<Spell>* GetMethodTable<Spell>() { return SpellMethods; }
//template<> RegType<QueryResult>* GetMethodTable<QueryResult>() { return QResultMethods; }
//template<> RegType<Field> * GetMethodTable<Field>() { return SQLFieldMethods; }
//template<> RegType<Aura> * GetMethodTable<Aura>() { return AuraMethods; }

#endif // __TABLEFUNCTIONS_H