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

#ifndef WOWSERVER_CHAT_H
#define WOWSERVER_CHAT_H

class ChatHandler;
class WorldSession;
class Player;
class Unit;

enum ChatMsg
{
	CHAT_MSG_ADDON									= -1,
	CHAT_MSG_SYSTEM									= 0,
	CHAT_MSG_SAY									= 1,
	CHAT_MSG_PARTY									= 2,
	CHAT_MSG_RAID									= 3,
	CHAT_MSG_GUILD									= 4,
	CHAT_MSG_OFFICER								= 5,
	CHAT_MSG_YELL									= 6,
	CHAT_MSG_WHISPER								= 7,
	CHAT_MSG_WHISPER_MOB							= 8,
	CHAT_MSG_WHISPER_INFORM							= 9,
	CHAT_MSG_EMOTE									= 10,
	CHAT_MSG_TEXT_EMOTE								= 11,
	CHAT_MSG_MONSTER_SAY							= 12,
	CHAT_MSG_MONSTER_PARTY							= 13,
	CHAT_MSG_MONSTER_YELL							= 14,
	CHAT_MSG_MONSTER_WHISPER						= 15,
	CHAT_MSG_MONSTER_EMOTE							= 16,
	CHAT_MSG_CHANNEL								= 17,
	CHAT_MSG_CHANNEL_JOIN							= 18,
	CHAT_MSG_CHANNEL_LEAVE							= 19,
	CHAT_MSG_CHANNEL_LIST							= 20,
	CHAT_MSG_CHANNEL_NOTICE							= 21,
	CHAT_MSG_CHANNEL_NOTICE_USER					= 22,
	CHAT_MSG_AFK									= 23,
	CHAT_MSG_DND									= 24,
	CHAT_MSG_IGNORED								= 25,
	CHAT_MSG_SKILL									= 26,
	CHAT_MSG_LOOT									= 27,
	CHAT_MSG_MONEY									= 28,
	CHAT_MSG_OPENING								= 29,
	CHAT_MSG_TRADESKILLS							= 30,
	CHAT_MSG_PET_INFO								= 31,
	CHAT_MSG_COMBAT_MISC_INFO						= 32,
	CHAT_MSG_XP_GAIN								= 33,
	CHAT_MSG_HONOR_GAIN								= 34,
	CHAT_MSG_COMBAT_FACTION_CHANGE					= 35,
	CHAT_MSG_BG_SYSTEM_NEUTRAL						= 36,
	CHAT_MSG_BG_SYSTEM_ALLIANCE						= 37,
	CHAT_MSG_BG_SYSTEM_HORDE						= 38,
	CHAT_MSG_RAID_LEADER							= 39,
	CHAT_MSG_RAID_WARNING							= 40,
	CHAT_MSG_RAID_BOSS_EMOTE						= 41,
	CHAT_MSG_RAID_BOSS_WHISPER						= 42,
	CHAT_MSG_FILTERED								= 43,
	CHAT_MSG_BATTLEGROUND							= 44,
	CHAT_MSG_BATTLEGROUND_LEADER					= 45,
	CHAT_MSG_RESTRICTED								= 46,
	CHAT_MSG_ACHIEVEMENT							= 47,
	CHAT_MSG_GUILD_ACHIEVEMENT						= 48,
	CHAT_MSG_UNK50									= 50,
	CHAT_MSG_PARTY_LEADER							= 51
};

enum Languages
{
	LANG_ADDON									= -1,
    LANG_UNIVERSAL                              = 0x00,
    LANG_ORCISH                                 = 0x01,
    LANG_DARNASSIAN                             = 0x02,
    LANG_TAURAHE                                = 0x03,
    LANG_DWARVISH                               = 0x06,
    LANG_COMMON                                 = 0x07,
    LANG_DEMONIC                                = 0x08,
    LANG_TITAN                                  = 0x09,
    LANG_THELASSIAN                             = 0x0A,
    LANG_DRACONIC                               = 0x0B,
    LANG_KALIMAG                                = 0x0C,
    LANG_GNOMISH                                = 0x0D,
    LANG_TROLL                                  = 0x0E,
    LANG_GUTTERSPEAK                            = 0x21,
    LANG_DRAENEI                                = 0x23,
	LANG_ZOMBIE									= 0x24,
	LANG_GNOMISHBINARY							= 0x25,
	LANG_GOBLINBINARY							= 0x26,
	NUM_LANGUAGES								= 0x27
};

/*#define MSG_COLOR_YELLOW		"|r"
#define MSG_COLOR_RED			"|cffff2020"
#define MSG_COLOR_GREEN			"|c1f40af20"
#define MSG_COLOR_LIGHTRED		"|cffff6060"*/

#define MSG_COLOR_LIGHTRED		"|cffff6060"
#define MSG_COLOR_LIGHTBLUE		"|cff00ccff"
#define MSG_COLOR_BLUE			"|cff0000ff"
#define MSG_COLOR_GREEN			"|cff00ff00"
#define MSG_COLOR_RED			"|cffff0000"
#define MSG_COLOR_GOLD			"|cffffcc00"
#define MSG_COLOR_GREY			"|cff888888"
#define MSG_COLOR_WHITE			"|cffffffff"
#define MSG_COLOR_SUBWHITE		"|cffbbbbbb"
#define MSG_COLOR_MAGENTA		"|cffff00ff"
#define MSG_COLOR_YELLOW		"|cffffff00"
#define MSG_COLOR_CYAN			"|cff00ffff"
#define MSG_COLOR_TORQUISEBLUE	"|cff00C78C"
#define MSG_COLOR_GREENYELLOW	"|cffADFF2F"
#define MSG_COLOR_PURPLE		"|cffDA70D6"
#define MSG_COLOR_ORANGEY		"|cffFF4500"
#define MSG_COLOR_CHOCOLATE		"|cffCD661D"
#define MSG_COLOR_IVORY			"|cff8B8B83"
#define MSG_COLOR_LIGHTYELLOW	"|cffFFFFE0"
#define MSG_COLOR_SEXGREEN		"|cff71C671"
#define MSG_COLOR_SEXTEAL		"|cff388E8E"
#define MSG_COLOR_SEXPINK		"|cffC67171"
#define MSG_COLOR_SEXBLUE		"|cff00E5EE"
#define MSG_COLOR_SEXHOTPINK	"|cffFF6EB4"

#define CHECKSESSION if(m_session == NULL) return NULL; \
	if(m_session->GetPlayer() == NULL) return NULL;


class ChatCommand
{
public:
	const char *		Name;
	char				CommandGroup;
	bool (ChatHandler::*Handler)(const char* args) ;
	std::string			Help;
	ChatCommand *		ChildCommands;
	uint32				NormalValueField;
	uint32				MaxValueField;
	uint16				ValueType;	// 0 = nothing, 1 = uint, 2 = float
};

class SERVER_DECL CommandTableStorage : public Singleton<CommandTableStorage>
{
	ChatCommand * _modifyCommandTable;
	ChatCommand * _debugCommandTable;
	ChatCommand * _waypointCommandTable;
	ChatCommand * _GMTicketCommandTable;
	ChatCommand * _GameObjectCommandTable;
	ChatCommand * _BattlegroundCommandTable;
	ChatCommand * _NPCCommandTable;
	ChatCommand * _gamemasterCommandTable;
	ChatCommand * _trackerCommandTable;
	ChatCommand * _administratorCommandTable;
	ChatCommand * _accountCommandTable;
	ChatCommand * _CheatCommandTable;
	ChatCommand * _honorCommandTable;
	ChatCommand * _questCommandTable;
	ChatCommand * _lookupCommandTable;
	ChatCommand * _petCommandTable;
	ChatCommand * _recallCommandTable;
	ChatCommand * _commandTable;
	ChatCommand * _GuildCommandTable;
	ChatCommand * _TitleCommandTable;
	ChatCommand * _wintergraspCommandTable;
	ChatCommand * _FactionCommandTable;

	// DB commands
	ChatCommand * _DBItemCommandTable;
	ChatCommand * _DatabaseCommandTable;
	// End Database Commands

	ChatCommand * GetSubCommandTable(const char * name);
public:
	void Init();
	void Dealloc();
	void Load();
	void Override(const char * command, const char * level);
	HEARTHSTONE_INLINE ChatCommand * Get() { return _commandTable; }
};

class SERVER_DECL ChatHandler : public Singleton<ChatHandler>
{
	friend class CommandTableStorage;
public:
	ChatHandler();
	~ChatHandler();

	WorldPacket * FillMessageData( uint32 type, int32 language,  const char* message,uint64 guid, uint8 flag = 0) const;
	WorldPacket * FillSystemMessageData( const char* message ) const;

	int ParseCommands(const char* text, WorldSession *session);

	void SystemMessage(WorldSession *m_session, const char *message, ...);
	void ColorSystemMessage(WorldSession *m_session, const char *colorcode, const char *message, ...);
	void RedSystemMessage(WorldSession *m_session, const char *message, ...);
	void GreenSystemMessage(WorldSession *m_session, const char *message, ...);
	void WhiteSystemMessage(WorldSession *m_session, const char *message, ...);
	void BlueSystemMessage(WorldSession *m_session, const char *message, ...);
	void RedSystemMessageToPlr(Player* plr, const char *message, ...);
	void GreenSystemMessageToPlr(Player* plr, const char *message, ...);
	void BlueSystemMessageToPlr(Player* plr, const char *message, ...);
	void SystemMessageToPlr(Player* plr, const char *message, ...);

protected:

	bool hasStringAbbr(const char* s1, const char* s2);
	void SendMultilineMessage(WorldSession *m_session, const char *str);

	bool ExecuteCommandInTable(ChatCommand *table, const char* text, WorldSession* session);
	bool ShowHelpForCommand(WorldSession *m_session, ChatCommand *table, const char* cmd);

	ChatCommand* getCommandTable();

	// Level 0 commands
	bool HandleHelpCommand(const char* args);
	bool HandleCommandsCommand(const char* args);
	bool HandleNYICommand(const char* args);
	bool HandleAcctCommand(const char* args);
	bool HandleStartCommand(const char* args);
	bool HandleInfoCommand(const char* args);
	bool HandleDismountCommand(const char* args);
	bool HandleFullDismountCommand(const char * args);
	bool HandleSaveCommand(const char* args);
	bool HandleGMListCommand(const char* args);
	bool HandleGmLogCommentCommand(const char *args);
	bool HandleRatingsCommand(const char *args);

	// Level 1 commands
	bool CmdSetValueField(WorldSession *m_session, uint32 field, uint32 fieldmax, const char *fieldname, const char* args);
	bool CmdSetFloatField(WorldSession *m_session, uint32 field, uint32 fieldmax, const char *fieldname, const char* args);
	bool HandleSummonCommand(const char* args);
	bool HandleAppearCommand(const char* args);
	bool HandleAnnounceCommand(const char* args);
	bool HandleAdminAnnounceCommand(const char* args);
	bool HandleGMAnnounceCommand(const char* args);
	bool HandleWAnnounceCommand(const char* args);
	bool HandleGMOnCommand(const char* args);
	bool HandleGMOffCommand(const char* args);
	bool HandleGPSCommand(const char* args);
	bool HandleKickCommand(const char* args);
	bool HandleTaxiCheatCommand(const char* args);
	bool HandleModifySpeedCommand(const char* args);
	bool HandleModifyTPsCommand(const char* args);
	bool HandleToggleDevCommand(const char* args);

	// Debug Commands
	bool HandleDebugSetPhase(const char* args);
	bool HandleDebugGoDamage(const char* args);
	bool HandleDebugGoRepair(const char* args);
	bool HandleDebugInFrontCommand(const char* args);
	bool HandleShowReactionCommand(const char* args);
	bool HandleAIMoveCommand(const char* args);
	bool HandleMoveInfoCommand(const char* args);
	bool HandleDistanceCommand(const char* args);
	bool HandleFaceCommand(const char* args);
	bool HandleSetBytesCommand(const char* args);
	bool HandleGetBytesCommand(const char* args);
	bool HandleDebugLandWalk(const char* args);
	bool HandleDebugUnroot(const char* args);
	bool HandleDebugRoot(const char* args);
	bool HandleDebugWaterWalk(const char* args);
	bool HandleAggroRangeCommand(const char* args);
	bool HandleKnockBackCommand(const char* args);
	bool HandleFadeCommand(const char* args);
	bool HandleThreatModCommand(const char* args);
	bool HandleCalcThreatCommand(const char* args);
	bool HandleThreatListCommand(const char* args);
	bool HandleNpcSpawnLinkCommand(const char* args);
	bool HandleDebugDumpCoordsCommmand(const char * args);
    bool HandleSendpacket(const char * args);
	bool HandleSQLQueryCommand(const char* args);
	bool HandleRangeCheckCommand(const char * args);

	// WayPoint Commands
	bool HandleWPAddCommand(const char* args);
	bool HandleWPShowCommand(const char* args);
	bool HandleWPHideCommand(const char* args);
	bool HandleWPDeleteCommand(const char* args);
	bool HandleWPFlagsCommand(const char* args);
	bool HandleWPMoveHereCommand(const char* args);
	bool HandleWPWaitCommand(const char* args);
	bool HandleWPEmoteCommand(const char* args);
	bool HandleWPStandStateCommand(const char* args);
	bool HandleWPSpellToCastCommand(const char* args);
	bool HandleWPSkinCommand(const char* args);
	bool HandleWPChangeNoCommand(const char* args);
	bool HandleWPInfoCommand(const char* args);
	bool HandleWPMoveTypeCommand(const char* args);
	bool HandleSaveWaypoints(const char* args);
	bool HandleGenerateWaypoints(const char* args);
	bool HandleDeleteWaypoints(const char* args);
	bool HandleWaypointGettextCommand(const char* args);
	bool HandleWaypointBackwardTextCommand(const char* args);
	bool HandleWaypointForwardTextCommand(const char* args);

	// Guild commands
	bool HandleGuildMembersCommand(const char* args);
	bool CreateGuildCommand(const char* args);
	bool HandleRenameGuildCommand(const char* args);
	bool HandleGuildRemovePlayerCommand(const char* args);
	bool HandleGuildDisbandCommand(const char* args);
	bool HandleGuildSetLeaderCommand(const char *args);

	// Level 2 commands
	bool HandleNameCommand(const char* args);
	bool HandleSubNameCommand(const char* args);
	bool HandleDeleteCommand(const char* args);
	bool HandleDeMorphCommand(const char* args);
	bool HandleItemCommand(const char* args);
	bool HandleItemRemoveCommand(const char* args);
	bool HandleNPCFlagCommand(const char* args);
	bool HandleSaveAllCommand(const char* args);
	bool HandleRegenerateGossipCommand(const char* args);
	bool HandleStartBGCommand(const char* args);
	bool HandlePauseBGCommand(const char* args);
	bool HandleResetScoreCommand(const char* args);
	bool HandleBGInfoCommnad(const char *args);
	bool HandleInvincibleCommand(const char *args);
	bool HandleInvisibleCommand(const char *args);
	bool HandleKillCommand(const char *args);
	bool HandleKillByPlrCommand(const char *args);
	bool HandleCreatureSpawnCommand(const char *args);
	bool HandleCreatureRespawnCommand(const char *args);
	bool HandleGOSelect(const char *args);
	bool HandleGODelete(const char *args);
	bool HandleGOSpawn(const char *args);
	bool HandleGOInfo(const char *args);
	bool HandleGOEnable(const char *args);
	bool HandleGOActivate(const char* args);
	bool HandleGORotate(const char * args);
	bool HandleGOMove(const char * args);
	bool HandleAddAIAgentCommand(const char* args);
	bool HandleListAIAgentCommand(const char* args);

	// Level 3 commands
	bool HandleMassSummonCommand(const char* args);
	bool HandleWorldPortCommand(const char* args);
	bool HandleMoveCommand(const char* args);
	bool HandleLearnCommand(const char* args);
	bool HandleReviveCommand(const char* args);
	bool HandleAddGraveCommand(const char* args);
	bool HandleAddSHCommand(const char* args);
	bool HandleExploreCheatCommand(const char* args);
	bool HandleGMTicketListCommand(const char* args);
	bool HandleGMTicketGetByIdCommand(const char* args);
	bool HandleGMTicketRemoveByIdCommand(const char* args);
#ifndef GM_TICKET_MY_MASTER_COMPATIBLE
	void SendGMSurvey();
	bool HandleGMTicketAssignToCommand(const char* args);
	bool HandleGMTicketReleaseCommand(const char* args);
	bool HandleGMTicketCommentCommand(const char* args);
	bool HandleGMTicketDeletePermanentCommand(const char* args);
#endif
	bool HandleGMTicketToggleTicketSystemStatusCommand(const char* args);
	bool HandleAddSkillCommand(const char* args);
	bool HandleAddInvItemCommand(const char* args);
	bool HandleResetReputationCommand(const char* args);
	bool HandleLearnSkillCommand(const char* args);
	bool HandleModifySkillCommand(const char* args);
	bool HandleRemoveSkillCommand(const char* args);
	bool HandleNpcInfoCommand(const char* args);
	bool HandleEmoteCommand(const char* args);
	bool HandleIncreaseWeaponSkill(const char* args);
	bool HandleCastSpellCommand(const char* args);
	bool HandleCastSpellNECommand(const char* args);
	bool HandleGenderChanger(const char* args);
	bool HandleModifyGoldCommand(const char* args);
	bool HandleMonsterSayCommand(const char* args);
	bool HandleMonsterYellCommand(const char* args);
	bool HandleMonsterCastCommand(const char * args);
	bool HandleNPCEquipCommand(const char * args);
	bool HandleNPCSetOnObjectCommand(const char * args);
	bool HandleNpcComeCommand(const char* args);
	bool HandleNpcJumpCommand(const char* args);
	bool HandleClearCooldownsCommand(const char* args);
	bool HandleBattlegroundCommand(const char* args);
	bool HandleSetWorldStateCommand(const char* args);
	bool HandlePlaySoundCommand(const char* args);
	bool HandleSetBattlefieldStatusCommand(const char* args);
	bool HandleNpcReturnCommand(const char* args);
	bool HandleAccountBannedCommand(const char * args);
	bool HandleAccountLevelCommand(const char * args);
	bool HandleResetTalentsCommand(const char* args);
	bool HandleResetSpellsCommand(const char* args);
	bool HandleNpcFollowCommand(const char* args);
	bool HandleFormationLink1Command(const char* args);
	bool HandleFormationLink2Command(const char* args);
	bool HandleNullFollowCommand(const char* args);
	bool HandleFormationClearCommand(const char* args);
	bool HandleResetSkillsCommand(const char* args);
    bool HandleGetSkillLevelCommand(const char* args);
    bool HandleGetSkillsInfoCommand(const char *args);
    bool HandlePlayerInfo(const char* args);

	//Ban
	bool HandleBanCharacterCommand(const char* args);
	bool HandleUnBanCharacterCommand(const char* args);

	//BG
	bool HandleSetBGScoreCommand(const char* args);

	Player* getSelectedChar(WorldSession *m_session, bool showerror = true);
	Creature* getSelectedCreature(WorldSession *m_session, bool showerror = true);
	bool HandleGOScale(const char* args);
	bool HandleReviveStringcommand(const char* args);
	bool HandleMountCommand(const char* args);
	bool HandleGetPosCommand(const char* args);
	bool HandleGetTransporterTime(const char* args);
	bool HandleSendItemPushResult(const char* args);
	bool HandleGOAnimProgress(const char * args);
	bool HandleGOExport(const char * args);
	bool HandleRemoveAurasCommand(const char *args);
	bool HandleParalyzeCommand(const char* args);
	bool HandleUnParalyzeCommand(const char* args);
	bool HandleSetMotdCommand(const char* args);
	bool HandleAddItemSetCommand(const char* args);
	bool HandleTriggerCommand(const char* args);
	bool HandleModifyValueCommand(const char* args);
	bool HandleModifyBitCommand(const char* args);
	bool HandleBattlegroundExitCommand(const char* args);
	bool HandleBattlegroundForcestartCommand(const char* args);
	bool HandleExitInstanceCommand(const char* args);
	bool HandleCooldownCheatCommand(const char* args);
	bool HandleCastTimeCheatCommand(const char* args);
	bool HandlePowerCheatCommand(const char* args);
	bool HandleGodModeCommand(const char* args);
	bool HandleShowCheatsCommand(const char* args);
	bool HandleFlySpeedCheatCommand(const char* args);
	bool HandleStackCheatCommand(const char* args);
	bool HandleTriggerpassCheatCommand(const char* args);
	bool HandleFlyCommand(const char* args);
	bool HandleRemoveRessurectionSickessAuraCommand(const char *args);
	bool HandleDBReloadCommand(const char* args);
	bool HandleItemSetCommand(const char* args);

	// Database
	bool HandleDBItemCreateCommand(const char* args);
	bool HandleDBItemSetClassCommand(const char* args);
	bool HandleDBItemSetSubClassCommand(const char* args);
	bool HandleDBItemSetNameCommand(const char* args);
	bool HandleDBItemSetDisplayIdCommand(const char* args);
	bool HandleDBItemSetQualityCommand(const char* args);
	bool HandleDBItemSetFlagsCommand(const char* args);
	bool HandleDBItemSetBuyPriceCommand(const char* args);
	bool HandleDBItemSetSellPriceCommand(const char* args);
	bool HandleDBItemSetInventoryTypeCommand(const char* args);
	bool HandleDBItemSetAllowableClassCommand(const char* args);
	bool HandleDBItemSetAllowableRaceCommand(const char* args);
	bool HandleDBItemSetItemLevelCommand(const char* args);
	bool HandleDBItemSetRequiredLevelCommand(const char* args);
	bool HandleDBItemSetRequiredSkillCommand(const char* args);
	bool HandleDBItemSetRequiredSkillRankCommand(const char* args);
	bool HandleDBItemSetRequiredSpellCommand(const char* args);
	bool HandleDBItemSetRequiredRank1Command(const char* args);
	bool HandleDBItemSetRequiredRank2Command(const char* args);
	bool HandleDBItemSetRequiredFactionCommand(const char* args);
	bool HandleDBItemSetRequiredFactionStandingCommand(const char* args);
	bool HandleDBItemSetUniqueCommand(const char* args);
	bool HandleDBItemSetMaxCountCommand(const char* args);
	bool HandleDBItemSetContainerSlotsCommand(const char* args);
	bool HandleDBItemSetStatTypeCommand(const char* args);
	bool HandleDBItemSetStatValueCommand(const char* args);
	bool HandleDBItemSetScalingStatIDCommand(const char* args);
	bool HandleDBItemSetScalingStatFlagsCommand(const char* args);
	bool HandleDBItemSetDamageMinCommand(const char* args);
	bool HandleDBItemSetDamageMaxCommand(const char* args);
	bool HandleDBItemSetDamageTypeCommand(const char* args);
	bool HandleDBItemSetArmorCommand(const char* args);
	bool HandleDBItemSetResistanceCommand(const char* args);
	bool HandleDBItemSetDelayCommand(const char* args);
	bool HandleDBItemSetAmmoTypeCommand(const char* args);
	bool HandleDBItemSetRangeCommand(const char* args);
	bool HandleDBItemSetSpellInfoCommand(const char* args);
	bool HandleDBItemSetBondingCommand(const char* args);
	bool HandleDBItemSetDescriptionCommand(const char* args);
	bool HandleDBItemSetPageIDCommand(const char* args);
	bool HandleDBItemSetPageLanguageCommand(const char* args);
	bool HandleDBItemSetPageMaterialCommand(const char* args);
	bool HandleDBItemSetQuestIDCommand(const char* args);
	bool HandleDBItemSetLockIDCommand(const char* args);
	bool HandleDBItemSetSheathIDCommand(const char* args);
	bool HandleDBItemSetRandomPropertyIDCommand(const char* args);
	bool HandleDBItemSetRandomSuffixIDCommand(const char* args);
	bool HandleDBItemSetBlockCommand(const char* args);
	bool HandleDBItemSetItemSetCommand(const char* args);
	bool HandleDBItemSetMaxDurabilityCommand(const char* args);
	bool HandleDBItemSetZoneNameIDCommand(const char* args);
	bool HandleDBItemSetMapIdCommand(const char* args);
	bool HandleDBItemSetBagFamilyCommand(const char* args);
	bool HandleDBItemSetTotemCategoryCommand(const char* args);
	bool HandleDBItemSetSocketInfoCommand(const char* args);
	bool HandleDBItemSetSocketBonusCommand(const char* args);
	bool HandleDBItemSetGemPropertiesCommand(const char* args);
	bool HandleDBItemSetRequiredDisenchantSkillCommand(const char* args);
	bool HandleDBItemSetLootGoldCommand(const char* args);
	bool HandleDBItemSetArmorDamageModifierCommand(const char* args);

	// honor
	bool HandleAddHonorCommand(const char* args);
	bool HandleAddKillCommand(const char* args);
	bool HandleGlobalHonorDailyMaintenanceCommand(const char* args);
	bool HandleNextDayCommand(const char* args);
	bool HandlePVPCreditCommand(const char* args);

	bool HandleAddTitleCommand(const char* args);
	bool HandleRemoveTitleCommand(const char* args);
	bool HandleGetKnownTitlesCommand(const char* args);
	bool HandleSetChosenTitleCommand(const char* args);

	bool HandleUnlearnCommand(const char* args);
	bool HandleModifyLevelCommand(const char* args);

	// Pet
	bool HandleCreatePetCommand(const char* args);
	bool HandleAddPetSpellCommand(const char* args);
	bool HandleRemovePetSpellCommand(const char* args);
	bool HandleAddPetTalentPoints(const char* args);
	bool HandleResetPetTalents(const char* args);
	bool HandleRenamePetCommand(const char* args);

	// Shutdown
	bool HandleShutdownCommand(const char* args);
	bool HandleRestartCommand(const char* args);

	// Whispers
	bool HandleAllowWhispersCommand(const char* args);
	bool HandleBlockWhispersCommand(const char* args);

	// Skills
	bool HandleAdvanceAllSkillsCommand(const char* args);

	// Kill
	bool HandleKillBySessionCommand(const char* args);
	bool HandleKillByPlayerCommand(const char* args);

	// Castall
	bool HandleCastAllCommand(const char* args);

	// Recall
	bool HandleRecallListCommand(const char* args);
	bool HandleRecallGoCommand(const char* args);
	bool HandleRecallAddCommand(const char* args);
	bool HandleRecallDelCommand(const char* args);
	bool HandleModPeriodCommand(const char* args);
	bool HandleGlobalPlaySoundCommand(const char* args);
	bool HandleRecallPortPlayerCommand(const char* args);

	// Bans
	bool HandleIPBanCommand(const char * args);
	bool HandleIPUnBanCommand(const char * args);
	bool HandleAccountUnbanCommand(const char * args);

	// Item removal
	bool HandleRemoveItemCommand(const char * args);

	// Rename
	bool HandleRenameCommand(const char * args);
	bool HandleForceRenameCommand(const char * args);
	bool HandleRecustomizeCharCommand(const char * args);

	// Reputation
	bool HandleGetStandingCommand(const char * args);
	bool HandleSetStandingCommand(const char * args);
	bool HandleGetBaseStandingCommand(const char * args);

	// Lookups
	bool HandleLookupItemCommand(const char * args);
	bool HandleLookupCreatureCommand(const char * args);
	bool HandleLookupObjectCommand(const char * args);
	bool HandleLookupSpellCommand(const char * args);
	bool HandleLookupTitleCommand(const char * args);

	//bool HandleReloadScriptsCommand(const char * args);
	bool HandleNpcPossessCommand(const char * args);
	bool HandleNpcUnPossessCommand(const char * args);
	bool HandleRehashCommand(const char * args);

	/* QUEST COMMANDS */
	bool HandleQuestAddBothCommand(const char * args);
	bool HandleQuestAddFinishCommand(const char * args);
	bool HandleQuestAddStartCommand(const char * args);
	bool HandleQuestDelBothCommand(const char * args);
	bool HandleQuestDelFinishCommand(const char * args);
	bool HandleQuestFinisherCommand(const char * args);
	bool HandleQuestDelStartCommand(const char * args);
	bool HandleQuestFinishCommand(const char * args);
	bool HandleQuestGiverCommand(const char * args);
	bool HandleQuestItemCommand(const char * args);
	bool HandleQuestListCommand(const char * args);
	bool HandleQuestLoadCommand(const char * args);
	bool HandleQuestLookupCommand(const char * args);
	bool HandleQuestRemoveCommand(const char * args);
	bool HandleQuestRewardCommand(const char * args);
	bool HandleQuestSpawnCommand(const char * args);
	bool HandleQuestStartCommand(const char * args);
	bool HandleQuestStatusCommand(const char * args);

	/** AI AGENT DEBUG COMMANDS */
	bool HandleAIAgentDebugBegin(const char * args);
	bool HandleAIAgentDebugContinue(const char * args);
	bool HandleAIAgentDebugSkip(const char * args);

	bool HandleCreateArenaTeamCommands(const char * args);
	bool HandleNpcSelectCommand(const char * args);
	bool HandleWaypointAddFlyCommand(const char * args);
	bool HandleWhisperBlockCommand(const char * args);
	bool HandleShowItems(const char * args);
	bool HandleCollisionTestIndoor(const char * args);
	bool HandleDebugVmapAreaInfo(const char * args);
	bool HandleCollisionTestLOS(const char * args);
	bool HandleRenameAllCharacter(const char * args);
	bool HandleCollisionGetHeight(const char * args);
	bool HandleAccountMuteCommand(const char * args);
	bool HandleAccountUnmuteCommand(const char * args);
	/* For skill related GM commands */
	SkillNameMgr *SkillNameManager;

	bool HandleFixScaleCommand(const char * args);
	bool HandleAddTrainerSpellCommand(const char * args);
	bool HandleClearCorpsesCommand(const char *args);
	bool HandleClearBonesCommand(const char *args);

	bool HandleMultiMuteCommand(const char *args);
	bool HandleMultiBanCommand(const char *args);
	bool HandleMultiAccountBanCommand(const char *args);
	bool HandleMultiKickCommand(const char *args);

	// Reputation system command
	bool HandlePosRepCommand(const char *args);
	bool HandleNegRepCommand(const char *args);
	bool HandleGetRepCommand(const char *args);

	bool HandleDebugRetroactiveQuestAchievements(const char *args);
	bool HandleModifyScaleCommand(const char *args);
	bool HandleModifyFactionCommand(const char *args);
	bool HandleModifyPlayerFlagsCommand(const char *args);
	bool HandleModifyAuraStateCommand(const char *args);
	bool HandleMirrorTimerCommand(const char *args);

	bool HandleEnableAH(const char *args);
	bool HandleDisableAH(const char *args);
	bool HandleForceStartWintergrasp(const char *args);
	bool HandleForceEndWintergrasp(const char *args);

	bool HandleFactionSetStanding(const char *args);
	bool HandleFactionModStanding(const char *args);

	// GM Tracker related commands.
	bool HandleTrackerListCommand(const char * args);
	bool HandleAddTrackerCommand(const char * args);
	bool HandleDelTrackerCommand(const char * args);
	//Id from links
	uint16 GetItemIDFromLink(const char* link, uint32* itemid);
	int32 GetSpellIDFromLink(const char* link);
	//Session for commands
	WorldSession *m_session;
private:
	Mutex m_lock;
};

#define sChatHandler ChatHandler::getSingleton()
#define sComTableStore CommandTableStorage::getSingleton()
#endif
