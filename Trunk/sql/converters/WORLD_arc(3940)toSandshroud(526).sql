DROP TABLE IF EXISTS `achievement_rewards`;
CREATE TABLE `achievement_rewards` (
  `achievementid` int(10) unsigned NOT NULL,
  `title_alliance` int(10) unsigned NOT NULL default '0',
  `title_horde` int(10) unsigned NOT NULL default '0',
  `itemid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`achievementid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

ALTER TABLE `ai_agents` CHANGE COLUMN `targettype_overwrite` `targettype` INT(11) NOT NULL DEFAULT '-1' AFTER `spelltype`;
ALTER TABLE `ai_agents` CHANGE COLUMN `cooldown_overwrite` `cooldown` INT(11) NOT NULL DEFAULT '-1' AFTER `targettype`;
ALTER TABLE `ai_agents` DROP COLUMN `instance_mode`, DROP COLUMN `event`;

ALTER TABLE `ai_agents` CHANGE COLUMN `type` `type` varchar(255) NOT NULL DEFAULT 'ROOT';
ALTER TABLE `ai_agents` CHANGE COLUMN `spelltype` `spelltype` varchar(255) NOT NULL DEFAULT 'ROOT';
ALTER TABLE `ai_agents` CHANGE COLUMN `targettype` `targettype` varchar(255) NOT NULL DEFAULT 'ROOT';
ALTER TABLE `auctionhouse` CHANGE COLUMN `ahgroup` `group` int(32) NOT NULL DEFAULT '0';

UPDATE ai_agents SET type = 'SPELL' WHERE type = 0;
UPDATE ai_agents SET type = 'MELEE' WHERE type = 1;
UPDATE ai_agents SET type = 'RANGED' WHERE type = 2;
UPDATE ai_agents SET type = 'FLEE' WHERE type = 3;
UPDATE ai_agents SET type = 'SPELL' WHERE type = 4;
UPDATE ai_agents SET type = 'CALLFORHELP' WHERE type = 5;
UPDATE ai_agents SET type = 'SPELL' WHERE type > 5;
UPDATE ai_agents SET spelltype = 'ROOT' WHERE spelltype = 1;
UPDATE ai_agents SET spelltype = 'HEAL' WHERE spelltype = 2;
UPDATE ai_agents SET spelltype = 'STUN' WHERE spelltype = 3;
UPDATE ai_agents SET spelltype = 'FEAR' WHERE spelltype = 4;
UPDATE ai_agents SET spelltype = 'SILENCE' WHERE spelltype = 5;
UPDATE ai_agents SET spelltype = 'CURSE' WHERE spelltype = 6;
UPDATE ai_agents SET spelltype = 'AOEDAMAGE' WHERE spelltype = 7;
UPDATE ai_agents SET spelltype = 'DAMAGE' WHERE spelltype = 8;
UPDATE ai_agents SET spelltype = 'SUMMON' WHERE spelltype = 9;
UPDATE ai_agents SET spelltype = 'BUFF' WHERE spelltype = 10;
UPDATE ai_agents SET spelltype = 'DEBUFF' WHERE spelltype = 11;
UPDATE ai_agents SET targettype = 'RANDOMTARGET' WHERE targettype = -1;
UPDATE ai_agents SET targettype = 'RANDOMTARGET' WHERE targettype = 0;
UPDATE ai_agents SET targettype = 'RANDOMTARGET' WHERE targettype = 1;
UPDATE ai_agents SET targettype = 'TARGETLOCATION' WHERE targettype = 2;
UPDATE ai_agents SET targettype = 'CREATURELOCATION' WHERE targettype = 3;
UPDATE ai_agents SET targettype = 'SELF' WHERE targettype = 4;
UPDATE ai_agents SET targettype = 'OWNER' WHERE targettype = 5;
ALTER TABLE ai_agents CHANGE type type enum('MELEE','RANGED','FLEE','SPELL','CALLFORHELP') NOT NULL DEFAULT 'SPELL';
ALTER TABLE ai_agents CHANGE spelltype spelltype enum('ROOT','HEAL','STUN','FEAR','SILENCE','CURSE','AOEDAMAGE','DAMAGE','SUMMON','BUFF','DEBUFF') NOT NULL DEFAULT 'ROOT';
ALTER TABLE ai_agents CHANGE targettype targettype enum('RANDOMTARGET','TARGETLOCATION','CREATURELOCATION','SELF','OWNER') NOT NULL DEFAULT 'RANDOMTARGET';

ALTER TABLE `ai_threattospellid`  DROP COLUMN `modcoef`;

DROP TABLE IF EXISTS   `banned_phrases`;
DROP TABLE IF EXISTS   `battlemasters`;

DROP TABLE IF EXISTS `creature_info`;
CREATE TABLE `creature_info` (
  `entry` int(32) NOT NULL DEFAULT '0',
  `default_emote_state` int(32) NOT NULL DEFAULT '0',
  `default_spawn_flags` int(32) NOT NULL DEFAULT '0',
  `default_stand_state` int(32) NOT NULL DEFAULT '0',
  `default_mount_displayid` int(32) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


ALTER TABLE `creature_names`  ADD COLUMN `loot_skill_type` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `type`;
ALTER TABLE `creature_names`  
ADD COLUMN `unk4` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `rank`,  
ADD COLUMN `spelldataid` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `unk4`,  
ADD COLUMN `civilian` TINYINT(3) NOT NULL DEFAULT '1' AFTER `unknown_float2`,  
DROP COLUMN `killcredit1`,  
DROP COLUMN `killcredit2`;
ALTER TABLE `creature_names`  DROP COLUMN `questitem1`,  
DROP COLUMN `questitem2`,  DROP COLUMN `questitem3`,  
DROP COLUMN `questitem4`,  DROP COLUMN `questitem5`,  
DROP COLUMN `questitem6`,  DROP COLUMN `waypointid`;

-- creature_proto
ALTER TABLE `creature_proto`  ADD COLUMN `powertype` TINYINT(3) UNSIGNED NOT NULL AFTER `maxhealth`,  CHANGE COLUMN `mana` `power` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `powertype`;
ALTER TABLE `creature_proto`  DROP COLUMN `can_ranged`;
ALTER TABLE `creature_proto`  ADD COLUMN `Item1` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `respawntime`;
ALTER TABLE `creature_proto`  ADD COLUMN `Item2` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Item1`;  
ALTER TABLE `creature_proto`  ADD COLUMN `Item3` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Item2`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `armor` `Resistence1` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Item3`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance2` `resistance2_old` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance1`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance1` `Resistance2` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Resistence1`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance3` `resistance3_old` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance2_old`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance4` `resistance4_old` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance3_old`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance5` `resistance5_old` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance4_old`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance6` `resistance6_old` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance5_old`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance2_old` `Resistance3` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Resistance2`;  
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance3_old` `Resistance4` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Resistance3`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance4_old` `resistance5` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `Resistance4`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance5_old` `resistance6` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance5`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `resistance6_old` `resistance7` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `resistance6`;
ALTER TABLE `creature_proto`  ADD COLUMN `no_XP` TINYINT(3) NOT NULL DEFAULT '0' AFTER `money`;  
ALTER TABLE `creature_proto`  DROP COLUMN `spell1`;  
ALTER TABLE `creature_proto`  DROP COLUMN `spell2`;  
ALTER TABLE `creature_proto`  DROP COLUMN `spell3`;  
ALTER TABLE `creature_proto`  DROP COLUMN `spell4`;
ALTER TABLE `creature_proto`  ADD COLUMN `death_state` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `invisibility_type`;
ALTER TABLE `creature_proto`  DROP COLUMN `spell_flags`;
ALTER TABLE `creature_proto`  CHANGE COLUMN `modImmunities` `auraimmune_flag` INT(30) UNSIGNED NOT NULL DEFAULT '0' AFTER `extra_a9_flags`;
ALTER TABLE `creature_proto`  DROP COLUMN `isTrainingDummy`;  
ALTER TABLE `creature_proto`  DROP COLUMN `guardtype`;  
ALTER TABLE `creature_proto`  DROP COLUMN `summonguard`;
ALTER TABLE `creature_proto`  ADD COLUMN `vehicle_entry` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `auraimmune_flag`;
ALTER TABLE `creature_proto`  ADD COLUMN `SpellClickId` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `vehicle_entry`;
ALTER TABLE `creature_proto`  ADD COLUMN `CanMove` TINYINT(3) UNSIGNED NOT NULL DEFAULT '1' AFTER `SpellClickId`;

-- thanks for whydb
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '2' WHERE auraimmune_flag & '1';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '1' WHERE auraimmune_flag & '2';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '4' WHERE auraimmune_flag & '4';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '32' WHERE auraimmune_flag & '8';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '64' WHERE auraimmune_flag & '16';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '8' WHERE auraimmune_flag & '32';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '512' WHERE auraimmune_flag & '64';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '16' WHERE auraimmune_flag & '128';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '8' WHERE auraimmune_flag & '256';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '1024' WHERE auraimmune_flag & '512';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '256' WHERE auraimmune_flag & '1024';
UPDATE `creature_proto` SET `auraimmune_flag` = `auraimmune_flag` + '4096' WHERE auraimmune_flag & '8192';
UPDATE `creature_proto` JOIN creature_spawns ON creature_spawns.entry = creature_proto.entry SET creature_proto.Item1 = creature_spawns.slot1item;
UPDATE `creature_proto` JOIN creature_spawns ON creature_spawns.entry = creature_proto.entry SET creature_proto.Item2 = creature_spawns.slot2item;
UPDATE `creature_proto` JOIN creature_spawns ON creature_spawns.entry = creature_proto.entry SET creature_proto.Item3 = creature_spawns.slot3item;
UPDATE `creature_proto` JOIN creature_spawns ON creature_spawns.entry = creature_proto.entry SET creature_proto.death_state = creature_spawns.death_state;

DROP TABLE IF EXISTS `creature_proto_mode`;
CREATE TABLE `creature_proto_mode` (
  `entry` mediumint(10) NOT NULL,
  `mode` tinyint(3) NOT NULL DEFAULT '0',
  `minlevel` tinyint(8) unsigned NOT NULL DEFAULT '1',
  `maxlevel` tinyint(8) unsigned NOT NULL DEFAULT '1',
  `minhealth` int(10) NOT NULL DEFAULT '1',
  `maxhealth` int(10) NOT NULL DEFAULT '1',
  `mindmg` float NOT NULL DEFAULT '1',
  `maxdmg` float NOT NULL DEFAULT '1',
  `power` int(10) NOT NULL DEFAULT '0',
  `resistance1` int(10) NOT NULL DEFAULT '0',
  `resistance2` int(10) NOT NULL DEFAULT '0',
  `resistance3` int(10) NOT NULL DEFAULT '0',
  `resistance4` int(10) NOT NULL DEFAULT '0',
  `resistance5` int(10) NOT NULL DEFAULT '0',
  `resistance6` int(10) NOT NULL DEFAULT '0',
  `resistance7` int(10) NOT NULL DEFAULT '0',
  `auras` longtext NOT NULL,
  `auraimmune_flag` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`mode`),
  UNIQUE KEY `Index` (`entry`,`mode`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*Table structure for table `creature_proto_vehicle` */

DROP TABLE IF EXISTS `creature_proto_vehicle`;

CREATE TABLE `creature_proto_vehicle` (
  `vehiclecreatureid` mediumint(8) NOT NULL DEFAULT '0',
  `increasehealthbydriver` tinyint(3) NOT NULL DEFAULT '0',
  `healthforitemlevel` mediumint(8) NOT NULL DEFAULT '0',
  `spell1` mediumint(8) NOT NULL DEFAULT '0',
  `spell2` mediumint(8) NOT NULL DEFAULT '0',
  `spell3` mediumint(8) NOT NULL DEFAULT '0',
  `spell4` mediumint(8) NOT NULL DEFAULT '0',
  `spell5` mediumint(8) NOT NULL DEFAULT '0',
  `spell6` mediumint(8) NOT NULL DEFAULT '0',
  `accessoryseat0` smallint(5) NOT NULL DEFAULT '0',
  `ejectondeath0` int(10) NOT NULL DEFAULT '0',
  `unselectable0` int(10) NOT NULL DEFAULT '0',
  `accessoryseat1` smallint(5) NOT NULL DEFAULT '0',
  `ejectondeath1` int(10) NOT NULL DEFAULT '0',
  `unselectable1` int(10) NOT NULL DEFAULT '0',
  `accessoryseat2` smallint(5) NOT NULL DEFAULT '0',
  `ejectondeath2` int(10) NOT NULL DEFAULT '0',
  `unselectable2` int(10) NOT NULL DEFAULT '0',
  `accessoryseat3` smallint(5) NOT NULL DEFAULT '0',
  `ejectondeath3` int(10) NOT NULL DEFAULT '0',
  `unselectable3` int(10) NOT NULL DEFAULT '0',
  `accessoryseat4` smallint(5) NOT NULL DEFAULT '0',
  `ejectondeath4` int(10) NOT NULL DEFAULT '0',
  `unselectable4` int(10) NOT NULL DEFAULT '0',
  `accessoryseat5` smallint(5) NOT NULL DEFAULT '0',
  `ejectondeath5` int(10) NOT NULL DEFAULT '0',
  `unselectable5` int(10) NOT NULL DEFAULT '0',
  `accessoryseat6` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath6` int(10) NOT NULL DEFAULT '0',
  `unselectable6` int(10) NOT NULL DEFAULT '0',
  `accessoryseat7` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath7` int(10) NOT NULL DEFAULT '0',
  `unselectable7` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`vehiclecreatureid`),
  UNIQUE KEY `Vehicle` (`vehiclecreatureid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

ALTER TABLE `creature_spawns` CHANGE column `bytes0` `bytes` int(30) NOT NULL default '0';
ALTER TABLE `creature_spawns` CHANGE column `slot1item` `ItemSlot1` int(10) NOT NULL default '0';
ALTER TABLE `creature_spawns` CHANGE column `slot2item` `ItemSlot2` int(10) NOT NULL default '0';
ALTER TABLE `creature_spawns` CHANGE column `slot3item` `ItemSlot3` int(10) NOT NULL default '0';
ALTER TABLE `creature_spawns`  CHANGE COLUMN `channel_target_sqlid` `channel_target_sqlid_go` INT(30) NOT NULL DEFAULT '0' AFTER `channel_spell`;
ALTER TABLE `creature_spawns` ADD COLUMN `vehicle` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `phase`;
ALTER TABLE `creature_spawns` DROP COLUMN `npc_respawn_link`;
ALTER TABLE `creature_spawns` DROP COLUMN `CanFly`;
ALTER TABLE `creature_spawns` DROP COLUMN `death_state`;
ALTER TABLE `creature_staticspawns`  CHANGE COLUMN `bytes0` `bytes` INT(30) NOT NULL DEFAULT '0' AFTER `flags`;
ALTER TABLE `creature_staticspawns`  CHANGE COLUMN `channel_target_sqlid` `channel_target_sqlid_go` INT(30) NOT NULL DEFAULT '0' AFTER `channel_spell`;
ALTER TABLE `creature_staticspawns` DROP COLUMN `npc_respawn_link`,  DROP COLUMN `standstate`,  DROP COLUMN `death_state`,  DROP COLUMN `slot1item`,DROP COLUMN `slot2item`,  DROP COLUMN `slot3item`,  DROP COLUMN `CanFly`;

DROP TABLE `creature_timed_emotes`;

ALTER TABLE `creature_waypoints`  ADD COLUMN `orientation` FLOAT NOT NULL DEFAULT '0' AFTER `position_z`,
  ADD COLUMN `forwardStandState` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `backwardskinid`, 
  ADD COLUMN `backwardStandState` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `forwardStandState`,
  ADD COLUMN `forwardSpellToCast` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `backwardStandState`,
  ADD COLUMN `backwardSpellToCast` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `forwardSpellToCast`,
  ADD COLUMN `forwardSayText` longtext character set utf8 collate utf8_unicode_ci NOT NULL AFTER `backwardSpellToCast`,
  ADD COLUMN `backwardSayText` longtext character set utf8 collate utf8_unicode_ci NOT NULL AFTER `forwardSayText`;
ALTER TABLE `loot_creatures`  RENAME TO `creatureloot`,
  CHANGE COLUMN `normal10percentchance` `difficulty0percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `itemid`,
  CHANGE COLUMN `normal25percentchance` `difficulty1percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `difficulty0percentchance`,
  CHANGE COLUMN `heroic10percentchance` `difficulty2percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `difficulty1percentchance`,
  CHANGE COLUMN `heroic25percentchance` `difficulty3percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `difficulty2percentchance`;
  
ALTER TABLE `loot_skinning`  RENAME TO `creatureloot_gathering`;
ALTER TABLE `loot_fishing`  CHANGE COLUMN `normal10percentchance` `percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `itemid`,  DROP COLUMN `normal25percentchance`,  DROP COLUMN `heroic10percentchance`,  DROP COLUMN `heroic25percentchance`;
ALTER TABLE `loot_fishing`  RENAME TO `fishingloot`;
ALTER TABLE `gameobject_names`  DROP COLUMN `Category`,  DROP COLUMN `CastBarText`,  DROP COLUMN `UnkStr`,  DROP COLUMN `Size`,  DROP COLUMN `QuestItem1`,  DROP COLUMN `QuestItem2`,  DROP COLUMN `QuestItem3`,  DROP COLUMN `QuestItem4`,  DROP COLUMN `QuestItem5`,  DROP COLUMN `QuestItem6`;
ALTER TABLE `gameobject_spawns`  DROP COLUMN `stateNpcLink`,  DROP COLUMN `overrides`;
ALTER TABLE `gameobject_staticspawns`  DROP COLUMN `phase`,  DROP COLUMN `overrides`;
DROP TABLE IF EXISTS `gameobject_teleports`;
DROP TABLE IF EXISTS `instance_bosses`;
ALTER TABLE `loot_items`  RENAME TO `itemloot`,  
CHANGE COLUMN `normal10percentchance` `percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `itemid`,
  DROP COLUMN `normal25percentchance`,  DROP COLUMN `heroic10percentchance`,
  DROP COLUMN `heroic25percentchance`;
ALTER TABLE `items`  CHANGE COLUMN `entry` `entry` INT(10) UNSIGNED NOT NULL DEFAULT '0' FIRST,
  ADD COLUMN `name2` VARCHAR(255) NOT NULL AFTER `name1`,  
  ADD COLUMN `name3` VARCHAR(255) NOT NULL AFTER `name2`,
  ADD COLUMN `name4` VARCHAR(255) NOT NULL AFTER `name3`,
  ADD COLUMN `ReqiuredSkillSubRank` INT(30) NOT NULL DEFAULT '0' AFTER `RequiredSkillRank`,  
  CHANGE COLUMN `ScaledStatsDistributionId` `ScalingStatId` INT(32) UNSIGNED NOT NULL DEFAULT '0' AFTER `stat_value10`,  
  CHANGE COLUMN `ScaledStatsDistributionFlags` `ScalingStatFlags` INT(32) UNSIGNED NOT NULL DEFAULT '0' AFTER `ScalingStatId`,  
  CHANGE COLUMN `randomsuffix` `randomsuffixid` INT(30) NOT NULL DEFAULT '0' AFTER `randomprop`,  
  ADD COLUMN `Lootgold` INT(30) NOT NULL DEFAULT '-1' AFTER `ReqDisenchantSkill`,  
  DROP COLUMN `faction`,  DROP COLUMN `RequiredSpell`,  
  DROP COLUMN `itemstatscount`,DROP COLUMN `existingduration`,  
  DROP COLUMN `ItemLimitCategoryId`,  DROP COLUMN `HolidayId`;

DROP TABLE IF EXISTS `items_extendedcost`;

ALTER TABLE `items_localized`  ADD COLUMN `last_update` VARCHAR(255) NOT NULL AFTER `description`;

DROP TABLE IF EXISTS `itemnames`;
DROP TABLE IF EXISTS `spelltargetconstraints`;
DROP TABLE IF EXISTS `unit_display_sizes`;

ALTER TABLE `loot_gameobjects`  RENAME TO `objectloot`,
CHANGE COLUMN `normal10percentchance` `difficulty0percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `itemid`,  
CHANGE COLUMN `normal25percentchance` `difficulty1percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `difficulty0percentchance`,  
CHANGE COLUMN `heroic10percentchance` `difficulty2percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `difficulty1percentchance`,  
CHANGE COLUMN `heroic25percentchance` `difficulty3percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `difficulty2percentchance`;

DROP TABLE IF EXISTS `news_announcements`;
CREATE TABLE `news_announcements` (
  `id` int(30) unsigned NOT NULL auto_increment,
  `faction_mask` int(30) NOT NULL,
  `lastTime` int(30) NOT NULL,
  `message_text` varchar(500) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `npc_monstersay_localized`;
CREATE TABLE `npc_monstersay_localized` (
  `entry` int(11) NOT NULL,
  `event` int(11) NOT NULL,
  `chance` float NOT NULL,
  `language` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `monstername` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text0` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text1` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text2` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text3` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `text4` longtext character set utf8 collate utf8_unicode_ci NOT NULL,
  `language_code` varchar(5) character set utf8 collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `Lang_key` (`entry`,`event`,`language`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='NPC System';

ALTER TABLE `loot_pickpocketing`  CHANGE COLUMN `normal10percentchance` `percentchance` FLOAT NOT NULL DEFAULT '0' AFTER `itemid`, DROP COLUMN `normal25percentchance`,  DROP COLUMN `heroic10percentchance`,  DROP COLUMN `heroic25percentchance`;

ALTER TABLE `loot_pickpocketing`  RENAME TO `pickpocketingloot`;
ALTER TABLE `playercreateinfo`  ADD COLUMN `BaseRunic` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `BaseEnergy`,  DROP COLUMN `introid`,  DROP COLUMN `taximask`;

DROP TABLE IF EXISTS `prestartqueries`;
CREATE TABLE `prestartqueries` (
  `Query` varchar(1024) NOT NULL,
  `SingleShot` int(1) unsigned NOT NULL default '1',
  `Seq` int(11) NOT NULL auto_increment,
  PRIMARY KEY  (`Seq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

ALTER TABLE `quests`  
ADD COLUMN `SuggestedPlayers` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `RequiredRepValue`,
ADD COLUMN `ReqKillPlayer` INT(11) UNSIGNED NOT NULL DEFAULT '0' AFTER `ReqItemCount6`,
ADD COLUMN `RequiredMoney` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `PointOpt`,
ADD COLUMN `RequiredQuest_and_or` INT(3) UNSIGNED NOT NULL DEFAULT '0' AFTER `RequiredQuest4`,
CHANGE COLUMN `rewardtalents` `RewTalent` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewSpell`,
  DROP COLUMN `RewardMoneyAtMaxLevel`,
  DROP COLUMN `RemoveQuests`,
  DROP COLUMN `suggestedplayers`,
  DROP COLUMN `detailemotecount`,
  DROP COLUMN `detailemote1`,
  DROP COLUMN `detailemote2`,
  DROP COLUMN `detailemote3`,
  DROP COLUMN `detailemote4`,
  DROP COLUMN `ReqEmoteId1`,
  DROP COLUMN `ReqEmoteId2`,
  DROP COLUMN `ReqEmoteId3`,
  DROP COLUMN `ReqEmoteId4`,
  DROP COLUMN `RewRepFaction6`,
  DROP COLUMN `RewRepValue6`,
  DROP COLUMN `detailemotedelay1`,
  DROP COLUMN `detailemotedelay2`,
  DROP COLUMN `detailemotedelay3`,
  DROP COLUMN `detailemotedelay4`,
  DROP COLUMN `completionemotecnt`,
  DROP COLUMN `completionemote1`,
  DROP COLUMN `completionemote2`,
  DROP COLUMN `completionemote3`,
  DROP COLUMN `completionemote4`,
  DROP COLUMN `completionemotedelay1`,
  DROP COLUMN `completionemotedelay2`,
  DROP COLUMN `completionemotedelay3`,
  DROP COLUMN `completionemotedelay4`,
  DROP COLUMN `completeemote`,
  DROP COLUMN `incompleteemote`,
  DROP COLUMN `iscompletedbyspelleffect`,
  DROP COLUMN `RequiredOneOfQuest`,
  DROP COLUMN `RewXPId`,
  CHANGE COLUMN `bonushonor` `RewBonusHonor` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewMoney`,
  CHANGE COLUMN `rewardtitleid` `RewTitle` INT(10) UNSIGNED NOT NULL DEFAULT '0' AFTER `RewRepLimit`;

DROP TABLE IF EXISTS `randomcardcreation`;
CREATE TABLE `randomcardcreation` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `ItemId0` int(10) unsigned NOT NULL default '0',
  `ItemId1` int(10) unsigned NOT NULL default '0',
  `ItemId2` int(10) unsigned NOT NULL default '0',
  `ItemId3` int(10) unsigned NOT NULL default '0',
  `ItemId4` int(10) unsigned NOT NULL default '0',
  `ItemId5` int(10) unsigned NOT NULL default '0',
  `ItemId6` int(10) unsigned NOT NULL default '0',
  `ItemId7` int(10) unsigned NOT NULL default '0',
  `ItemId8` int(10) unsigned NOT NULL default '0',
  `ItemId9` int(10) unsigned NOT NULL default '0',
  `ItemId10` int(10) unsigned NOT NULL default '0',
  `ItemId11` int(10) unsigned NOT NULL default '0',
  `ItemId12` int(10) unsigned NOT NULL default '0',
  `ItemId13` int(10) unsigned NOT NULL default '0',
  `ItemId14` int(10) unsigned NOT NULL default '0',
  `ItemId15` int(10) unsigned NOT NULL default '0',
  `ItemId16` int(10) unsigned NOT NULL default '0',
  `ItemId17` int(10) unsigned NOT NULL default '0',
  `ItemId18` int(10) unsigned NOT NULL default '0',
  `ItemId19` int(10) unsigned NOT NULL default '0',
  `ItemId20` int(10) unsigned NOT NULL default '0',
  `ItemId21` int(10) unsigned NOT NULL default '0',
  `ItemId22` int(10) unsigned NOT NULL default '0',
  `ItemId23` int(10) unsigned NOT NULL default '0',
  `ItemId24` int(10) unsigned NOT NULL default '0',
  `ItemId25` int(10) unsigned NOT NULL default '0',
  `ItemId26` int(10) unsigned NOT NULL default '0',
  `ItemId27` int(10) unsigned NOT NULL default '0',
  `ItemId28` int(10) unsigned NOT NULL default '0',
  `ItemId29` int(10) unsigned NOT NULL default '0',
  `ItemId30` int(10) unsigned NOT NULL default '0',
  `ItemId31` int(10) unsigned NOT NULL default '0',
  `ItemCount` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `randomitemcreation`;
CREATE TABLE `randomitemcreation` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `ItemToCreate` int(10) unsigned NOT NULL default '0',
  `Skill` int(10) unsigned NOT NULL default '0',
  `Chance` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ItemToCreate`,`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

ALTER TABLE `recall`  
CHANGE COLUMN `id` `id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT FIRST,  
DROP COLUMN `Orientation`;

ALTER TABLE `reputation_instance_onkill`  
ADD COLUMN `mob_rep_reward_heroic` INT(30) NOT NULL AFTER `mob_rep_reward`,  
ADD COLUMN `mob_rep_limit_heroic` INT(30) NOT NULL AFTER `mob_rep_limit`,  
ADD COLUMN `boss_rep_reward_heroic` INT(30) NOT NULL AFTER `boss_rep_reward`,  
ADD COLUMN `boss_rep_limit_heroic` INT(30) NOT NULL AFTER `boss_rep_limit`;

DROP TABLE IF EXISTS `scrollcreation`;
CREATE TABLE `scrollcreation` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `ItemId` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`SpellId`,`ItemId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
ALTER TABLE `spell_coef_override`  
ADD COLUMN `AP_coef_override` FLOAT NULL DEFAULT NULL AFTER `OTspell_coef_override`,  
ADD COLUMN `RAP_coef_override` FLOAT NULL DEFAULT NULL AFTER `AP_coef_override`,
DROP COLUMN `name`;

DROP TABLE IF EXISTS `spell_forced_targets`;
CREATE TABLE `spell_forced_targets` (
  `spellid` int(8) unsigned NOT NULL,
  `target` int(8) unsigned NOT NULL default '0',
  UNIQUE KEY `entry_type_target` (`spellid`,`target`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spell System';

DROP TABLE IF EXISTS `spellfixes`;
CREATE TABLE `spellfixes` (
  `spellId` int(30) NOT NULL default '0',
  `procFlags` int(30) NOT NULL default '0',
  `procChance` int(30) NOT NULL default '0',
  `procCharges` int(30) NOT NULL default '0',
  `effect0ClassMask0` int(10) unsigned NOT NULL default '0',
  `effect0ClassMask1` int(10) unsigned NOT NULL default '0',
  `effect0ClassMask2` int(10) unsigned NOT NULL default '0',
  `effect1ClassMask0` int(10) unsigned NOT NULL default '0',
  `effect1ClassMask1` int(10) unsigned NOT NULL default '0',
  `effect1ClassMask2` int(10) unsigned NOT NULL default '0',
  `effect2ClassMask0` int(10) unsigned NOT NULL default '0',
  `effect2ClassMask1` int(10) unsigned NOT NULL default '0',
  `effect2ClassMask2` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
ALTER TABLE `teleport_coords`  
ADD COLUMN `orientation` FLOAT NOT NULL DEFAULT '0' AFTER `position_z`,  
DROP COLUMN `totrigger`;

ALTER TABLE `trainer_defs`  
DROP COLUMN `RequiredRace`,  
DROP COLUMN `RequiredReputation`,  
DROP COLUMN `RequiredReputationValue`;

DROP TABLE `transport_data`;
DROP TABLE `vendor_restrictions`;
DROP TABLE `worldbroadcast`;
DROP TABLE `worldbroadcast_localized`;

ALTER TABLE `worldmap_info`  
ADD COLUMN `hordephase` TINYINT(3) NOT NULL DEFAULT '0' AFTER `required_checkpoint`,  
ADD COLUMN `alliancephase` TINYINT(3) NOT NULL DEFAULT '0' AFTER `hordephase`,  
ADD COLUMN `collison` TINYINT(3) NOT NULL DEFAULT '0' AFTER `alliancephase`,  
DROP COLUMN `minlevel_heroic`,  DROP COLUMN `lvl_mod_a`;

UPDATE `worldmap_info` SET cooldown = '3000' WHERE cooldown = '0';

DROP TABLE `worldmap_info_localized`;

DROP TABLE `worldstring_tables`;
DROP TABLE `worldstring_tables_localized`;

DROP TABLE IF EXISTS `worldstate_template`;
CREATE TABLE `worldstate_template` (
  `mapid` int(30) unsigned NOT NULL,
  `zone_mask` int(30) NOT NULL,
  `faction_mask` int(30) NOT NULL,
  `field_number` int(30) unsigned NOT NULL,
  `initial_value` int(30) NOT NULL,
  `comment` varchar(200) NOT NULL,
  PRIMARY KEY  (`field_number`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

UPDATE `creature_proto` JOIN SpellClickSpells ON SpellClickSpells.CreatureID = creature_proto.entry SET creature_proto.SpellClickId = SpellClickSpells.SpellID;
DROP TABLE IF EXISTS `SpellClickSpells`;