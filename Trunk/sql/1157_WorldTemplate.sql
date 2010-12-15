# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.1.53-community
# Server OS:                    Win64
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2010-12-15 16:42:25
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table worldtemplate.achievement_rewards
CREATE TABLE IF NOT EXISTS `achievement_rewards` (
  `achievementid` smallint(5) unsigned NOT NULL,
  `title_alliance` smallint(5) unsigned NOT NULL,
  `title_horde` smallint(5) unsigned NOT NULL,
  `itemid` mediumint(8) unsigned NOT NULL,
  `SendMessage` int(3) NOT NULL DEFAULT '0',
  `SenderEntry` int(10) NOT NULL DEFAULT '0',
  `messageheader` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `messagebody` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  PRIMARY KEY (`achievementid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.ai_agents
CREATE TABLE IF NOT EXISTS `ai_agents` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `difficulty` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `type` enum('MELEE','RANGED','SPELL') NOT NULL DEFAULT 'SPELL',
  `chance` float unsigned NOT NULL DEFAULT '0',
  `maxcount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `spell` int(10) unsigned NOT NULL DEFAULT '0',
  `spelltype` enum('ROOT','HEAL','STUN','FEAR','SILENCE','CURSE','AOEDAMAGE','DAMAGE','SUMMON','BUFF','DEBUFF') NOT NULL DEFAULT 'ROOT',
  `targettype` enum('RANDOMTARGET','TARGETLOCATION','CREATURELOCATION','SELF','OWNER') NOT NULL DEFAULT 'RANDOMTARGET',
  `cooldown` int(8) NOT NULL DEFAULT '0',
  `floatMisc1` float NOT NULL DEFAULT '0',
  `Misc2` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`type`,`spell`),
  UNIQUE KEY `a` (`entry`,`spell`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='AI System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.ai_threattospellid
CREATE TABLE IF NOT EXISTS `ai_threattospellid` (
  `spell` int(10) unsigned NOT NULL DEFAULT '0',
  `mod` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='AI System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.areatriggers
CREATE TABLE IF NOT EXISTS `areatriggers` (
  `entry` smallint(5) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned DEFAULT '0',
  `map` smallint(5) unsigned DEFAULT NULL,
  `screen` smallint(5) unsigned DEFAULT NULL,
  `name` varchar(100) DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `required_honor_rank` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `required_level` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Trigger System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.auctionhouse
CREATE TABLE IF NOT EXISTS `auctionhouse` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `creature_entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `group` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Auction House';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.clientaddons
CREATE TABLE IF NOT EXISTS `clientaddons` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `crc` bigint(20) unsigned DEFAULT NULL,
  `banned` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `showinlist` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `index` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Client Addons';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.command_overrides
CREATE TABLE IF NOT EXISTS `command_overrides` (
  `command_name` varchar(100) NOT NULL,
  `access_level` varchar(10) NOT NULL,
  PRIMARY KEY (`command_name`),
  UNIQUE KEY `a` (`command_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Commands System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creatureloot
CREATE TABLE IF NOT EXISTS `creatureloot` (
  `entryid` mediumint(8) NOT NULL DEFAULT '0',
  `itemid` mediumint(8) NOT NULL DEFAULT '0',
  `difficulty0percentchance` float DEFAULT NULL,
  `difficulty1percentchance` float DEFAULT NULL,
  `difficulty2percentchance` float NOT NULL DEFAULT '0',
  `difficulty3percentchance` float NOT NULL DEFAULT '0',
  `mincount` smallint(5) NOT NULL DEFAULT '1',
  `maxcount` smallint(5) NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`),
  UNIQUE KEY `UNIQUE` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creatureloot_gathering
CREATE TABLE IF NOT EXISTS `creatureloot_gathering` (
  `entryid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `itemid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `difficulty0percentchance` float DEFAULT NULL,
  `difficulty1percentchance` float DEFAULT NULL,
  `difficulty2percentchance` float NOT NULL DEFAULT '0',
  `difficulty3percentchance` float NOT NULL DEFAULT '0',
  `mincount` smallint(5) DEFAULT '1',
  `maxcount` smallint(5) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_formations
CREATE TABLE IF NOT EXISTS `creature_formations` (
  `spawn_id` int(10) unsigned NOT NULL DEFAULT '0',
  `target_spawn_id` int(10) unsigned NOT NULL DEFAULT '0',
  `follow_angle` float NOT NULL DEFAULT '0',
  `follow_dist` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`spawn_id`,`target_spawn_id`),
  UNIQUE KEY `a` (`spawn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_info
CREATE TABLE IF NOT EXISTS `creature_info` (
  `entry` mediumint(8) NOT NULL DEFAULT '0',
  `default_emote_state` int(10) NOT NULL DEFAULT '0',
  `default_spawn_flags` int(10) NOT NULL DEFAULT '0',
  `default_stand_state` int(10) NOT NULL DEFAULT '0',
  `default_mount_displayid` int(10) NOT NULL DEFAULT '0',
  `CanRange` tinyint(3) NOT NULL DEFAULT '0',
  `CanCallForHelp` tinyint(3) NOT NULL DEFAULT '0',
  `CallForHelpHPPercent` float(8,4) NOT NULL DEFAULT '10.0000',
  `CanFlee` tinyint(3) NOT NULL DEFAULT '0',
  `FleeHealthPercent` float(8,4) NOT NULL DEFAULT '10.0000',
  `FleeMSDuration` int(10) NOT NULL DEFAULT '5000',
  `SendFleeMessage` int(10) NOT NULL DEFAULT '0',
  `FleeMessage` varchar(255) NOT NULL DEFAULT '' COMMENT 'DON''T KILL MEH!',
  `GivesNoSkill` tinyint(3) NOT NULL DEFAULT '0',
  `GivesNoXP` tinyint(3) DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_names
CREATE TABLE IF NOT EXISTS `creature_names` (
  `entry` mediumint(8) NOT NULL DEFAULT '0',
  `name` varchar(100) NOT NULL DEFAULT '',
  `subname` varchar(100) NOT NULL DEFAULT '',
  `info_str` varchar(500) NOT NULL DEFAULT '',
  `Flags1` int(10) unsigned NOT NULL DEFAULT '0',
  `type` smallint(5) NOT NULL DEFAULT '0',
  `loot_skill_type` tinyint(3) NOT NULL DEFAULT '1',
  `family` smallint(5) NOT NULL DEFAULT '0',
  `rank` tinyint(3) NOT NULL DEFAULT '0',
  `unk4` int(10) NOT NULL DEFAULT '0',
  `spelldataid` int(10) NOT NULL DEFAULT '0',
  `male_displayid` mediumint(8) NOT NULL DEFAULT '0',
  `female_displayid` mediumint(8) NOT NULL DEFAULT '0',
  `male_displayid2` mediumint(8) NOT NULL DEFAULT '0',
  `female_displayid2` mediumint(8) NOT NULL DEFAULT '0',
  `unknown_float1` float NOT NULL DEFAULT '0',
  `unknown_float2` float NOT NULL DEFAULT '0',
  `civilian` tinyint(3) NOT NULL DEFAULT '0',
  `leader` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_names_localized
CREATE TABLE IF NOT EXISTS `creature_names_localized` (
  `id` mediumint(8) unsigned NOT NULL,
  `language_code` varchar(5) CHARACTER SET utf8 NOT NULL,
  `name` varchar(100) CHARACTER SET utf8 NOT NULL,
  `subname` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`id`,`language_code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_proto
CREATE TABLE IF NOT EXISTS `creature_proto` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `minlevel` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `maxlevel` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `faction` smallint(5) unsigned NOT NULL DEFAULT '35',
  `minhealth` int(10) unsigned NOT NULL DEFAULT '1',
  `maxhealth` int(10) unsigned NOT NULL DEFAULT '1',
  `powertype` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `power` int(10) unsigned NOT NULL DEFAULT '0',
  `scale` float NOT NULL DEFAULT '1',
  `npcflags` int(10) unsigned NOT NULL DEFAULT '0',
  `attacktime` mediumint(8) unsigned NOT NULL DEFAULT '2500',
  `attacktype` tinyint(3) NOT NULL DEFAULT '0',
  `mindamage` float NOT NULL DEFAULT '1',
  `maxdamage` float NOT NULL DEFAULT '2',
  `rangedattacktime` mediumint(8) unsigned NOT NULL DEFAULT '3000',
  `rangedmindamage` float unsigned NOT NULL DEFAULT '1',
  `rangedmaxdamage` float unsigned NOT NULL DEFAULT '2',
  `Item1` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Item2` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Item3` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `respawntime` int(10) unsigned NOT NULL DEFAULT '360000',
  `resistance1` int(10) unsigned NOT NULL DEFAULT '0',
  `resistance2` int(10) unsigned NOT NULL DEFAULT '0',
  `resistance3` int(10) unsigned NOT NULL DEFAULT '0',
  `resistance4` int(10) unsigned NOT NULL DEFAULT '0',
  `resistance5` int(10) unsigned NOT NULL DEFAULT '0',
  `resistance6` int(10) unsigned NOT NULL DEFAULT '0',
  `resistance7` int(10) unsigned NOT NULL DEFAULT '0',
  `combat_reach` float NOT NULL DEFAULT '0',
  `bounding_radius` float NOT NULL DEFAULT '0',
  `auras` varchar(255) NOT NULL DEFAULT '',
  `boss` int(11) unsigned NOT NULL DEFAULT '0',
  `money` int(10) NOT NULL DEFAULT '0',
  `invisibility_type` int(10) unsigned NOT NULL DEFAULT '0',
  `death_state` int(10) unsigned NOT NULL DEFAULT '0',
  `walk_speed` float NOT NULL DEFAULT '2.5',
  `run_speed` float NOT NULL DEFAULT '8',
  `fly_speed` float NOT NULL DEFAULT '14',
  `extra_a9_flags` int(10) NOT NULL DEFAULT '0',
  `auraimmune_flag` int(10) unsigned NOT NULL DEFAULT '0',
  `vehicle_entry` mediumint(8) NOT NULL DEFAULT '-1',
  `SpellClickId` mediumint(8) NOT NULL DEFAULT '-1',
  `CanMove` tinyint(3) NOT NULL DEFAULT '1',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_proto_mode
CREATE TABLE IF NOT EXISTS `creature_proto_mode` (
  `entry` mediumint(8) NOT NULL,
  `mode` tinyint(3) NOT NULL DEFAULT '-1',
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

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_proto_vehicle
CREATE TABLE IF NOT EXISTS `creature_proto_vehicle` (
  `vehiclecreatureid` mediumint(8) NOT NULL DEFAULT '0',
  `increasehealthbydriver` tinyint(3) NOT NULL DEFAULT '0',
  `healthforitemlevel` mediumint(8) NOT NULL DEFAULT '0',
  `spell1` mediumint(8) NOT NULL DEFAULT '0',
  `spell2` mediumint(8) NOT NULL DEFAULT '0',
  `spell3` mediumint(8) NOT NULL DEFAULT '0',
  `spell4` mediumint(8) NOT NULL DEFAULT '0',
  `spell5` mediumint(8) NOT NULL DEFAULT '0',
  `spell6` mediumint(8) NOT NULL DEFAULT '0',
  `movementflags` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `accessoryseat0` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath0` int(10) NOT NULL DEFAULT '0',
  `unselectable0` int(10) NOT NULL DEFAULT '0',
  `accessoryseat1` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath1` int(10) NOT NULL DEFAULT '0',
  `unselectable1` int(10) NOT NULL DEFAULT '0',
  `accessoryseat2` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath2` int(10) NOT NULL DEFAULT '0',
  `unselectable2` int(10) NOT NULL DEFAULT '0',
  `accessoryseat3` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath3` int(10) NOT NULL DEFAULT '0',
  `unselectable3` int(10) NOT NULL DEFAULT '0',
  `accessoryseat4` mediumint(8) NOT NULL DEFAULT '0',
  `ejectondeath4` int(10) NOT NULL DEFAULT '0',
  `unselectable4` int(10) NOT NULL DEFAULT '0',
  `accessoryseat5` mediumint(8) NOT NULL DEFAULT '0',
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

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_quest_finisher
CREATE TABLE IF NOT EXISTS `creature_quest_finisher` (
  `id` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `quest` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_quest_starter
CREATE TABLE IF NOT EXISTS `creature_quest_starter` (
  `id` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `quest` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ROW_FORMAT=FIXED COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_spawns
CREATE TABLE IF NOT EXISTS `creature_spawns` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `entry` mediumint(8) NOT NULL,
  `map` smallint(5) NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL,
  `movetype` tinyint(3) NOT NULL DEFAULT '0',
  `displayid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `faction` smallint(5) NOT NULL DEFAULT '14',
  `flags` int(10) NOT NULL DEFAULT '0',
  `bytes` int(10) NOT NULL DEFAULT '0',
  `bytes1` int(10) NOT NULL DEFAULT '0',
  `bytes2` int(10) NOT NULL DEFAULT '0',
  `emote_state` int(10) NOT NULL DEFAULT '0',
  `channel_spell` int(10) NOT NULL DEFAULT '0',
  `channel_target_sqlid` int(10) NOT NULL DEFAULT '0',
  `channel_target_sqlid_creature` int(10) NOT NULL DEFAULT '0',
  `standstate` int(10) NOT NULL DEFAULT '0',
  `MountedDisplayID` mediumint(8) NOT NULL DEFAULT '0',
  `ItemSlot1` int(10) NOT NULL DEFAULT '0',
  `ItemSlot2` int(10) NOT NULL DEFAULT '0',
  `ItemSlot3` int(10) NOT NULL DEFAULT '0',
  `phase` tinyint(3) NOT NULL DEFAULT '0',
  `vehicle` int(10) NOT NULL DEFAULT '0',
  `CanMove` int(8) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `b` (`map`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_staticspawns
CREATE TABLE IF NOT EXISTS `creature_staticspawns` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `entry` mediumint(8) NOT NULL,
  `Map` smallint(5) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `orientation` float NOT NULL,
  `movetype` tinyint(3) NOT NULL DEFAULT '0',
  `displayid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `faction` smallint(5) NOT NULL DEFAULT '35',
  `flags` int(10) NOT NULL DEFAULT '0',
  `bytes` int(10) NOT NULL DEFAULT '0',
  `bytes1` int(10) NOT NULL,
  `bytes2` int(10) NOT NULL DEFAULT '0',
  `emote_state` int(10) NOT NULL DEFAULT '0',
  `channel_spell` int(10) NOT NULL DEFAULT '0',
  `channel_target_sqlid` int(10) NOT NULL,
  `channel_target_sqlid_creature` int(10) NOT NULL,
  `standstate` int(10) NOT NULL,
  `Phase` tinyint(3) NOT NULL,
  `vehicle` int(10) NOT NULL DEFAULT '0',
  `eventid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Relates to events table',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_teleport_info
CREATE TABLE IF NOT EXISTS `creature_teleport_info` (
  `entry` mediumint(10) NOT NULL DEFAULT '0',
  `intid` mediumint(10) NOT NULL DEFAULT '0',
  `iconid` mediumint(10) NOT NULL DEFAULT '0',
  `teleport` tinyint(3) NOT NULL DEFAULT '0',
  `textinfo` varchar(255) NOT NULL DEFAULT '',
  `teleportmapid` int(30) unsigned NOT NULL DEFAULT '0',
  `teleportx` float(30,0) NOT NULL DEFAULT '0',
  `teleporty` float(30,0) NOT NULL DEFAULT '0',
  `teleportz` float(30,0) NOT NULL DEFAULT '0',
  `teleporto` float(30,0) NOT NULL DEFAULT '0',
  `castspellid` int(30) unsigned NOT NULL DEFAULT '0',
  `removetargetaura` int(30) unsigned NOT NULL DEFAULT '0',
  `sendchatmessage` tinyint(3) NOT NULL DEFAULT '0',
  `messagetype` mediumint(10) NOT NULL DEFAULT '0',
  `messagetosend` varchar(255) NOT NULL DEFAULT '',
  UNIQUE KEY `Index` (`entry`,`intid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.creature_waypoints
CREATE TABLE IF NOT EXISTS `creature_waypoints` (
  `spawnid` int(10) unsigned NOT NULL DEFAULT '0',
  `waypointid` mediumint(5) NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `waittime` mediumint(8) NOT NULL DEFAULT '0',
  `flags` int(10) NOT NULL DEFAULT '0',
  `forwardemoteoneshot` mediumint(8) NOT NULL DEFAULT '0',
  `forwardemoteid` mediumint(8) NOT NULL DEFAULT '0',
  `backwardemoteoneshot` mediumint(8) NOT NULL DEFAULT '0',
  `backwardemoteid` mediumint(8) NOT NULL DEFAULT '0',
  `forwardskinid` mediumint(8) NOT NULL DEFAULT '0',
  `backwardskinid` mediumint(8) NOT NULL DEFAULT '0',
  `forwardStandState` tinyint(3) NOT NULL DEFAULT '0',
  `backwardStandState` tinyint(3) NOT NULL DEFAULT '0',
  `forwardSpellToCast` int(10) NOT NULL DEFAULT '0',
  `backwardSpellToCast` int(10) NOT NULL DEFAULT '0',
  `forwardSayText` varchar(256) NOT NULL,
  `backwardSayText` varchar(256) NOT NULL,
  PRIMARY KEY (`spawnid`,`waypointid`),
  UNIQUE KEY `a` (`waypointid`,`spawnid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.db_version
CREATE TABLE IF NOT EXISTS `db_version` (
  `info` char(4) COLLATE utf8_unicode_ci NOT NULL,
  `revision` smallint(5) unsigned NOT NULL DEFAULT '0',
  `changeset` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `updater` char(32) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`info`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.fishing
CREATE TABLE IF NOT EXISTS `fishing` (
  `Zone` mediumint(10) unsigned NOT NULL DEFAULT '0',
  `MinSkill` smallint(5) unsigned DEFAULT NULL,
  `MaxSkill` smallint(5) unsigned DEFAULT NULL,
  PRIMARY KEY (`Zone`),
  UNIQUE KEY `Zone` (`Zone`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Fishing System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.fishingloot
CREATE TABLE IF NOT EXISTS `fishingloot` (
  `entryid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `itemid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `percentchance` float NOT NULL DEFAULT '0',
  `mincount` smallint(5) unsigned NOT NULL DEFAULT '1',
  `maxcount` smallint(5) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_names
CREATE TABLE IF NOT EXISTS `gameobject_names` (
  `entry` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `DisplayID` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Name` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `IconName` varchar(100) NOT NULL,
  `CastBarText` varchar(100) NOT NULL,
  `spellfocus` int(10) NOT NULL DEFAULT '0',
  `sound1` int(10) unsigned NOT NULL DEFAULT '0',
  `sound2` int(10) NOT NULL DEFAULT '0',
  `sound3` int(10) NOT NULL DEFAULT '0',
  `sound4` int(10) NOT NULL DEFAULT '0',
  `sound5` int(10) NOT NULL DEFAULT '0',
  `sound6` int(10) unsigned NOT NULL DEFAULT '0',
  `sound7` int(10) NOT NULL DEFAULT '0',
  `sound8` int(10) NOT NULL DEFAULT '0',
  `sound9` int(10) NOT NULL DEFAULT '0',
  `unknown1` int(10) NOT NULL DEFAULT '0',
  `unknown2` int(10) NOT NULL DEFAULT '0',
  `unknown3` int(10) NOT NULL DEFAULT '0',
  `unknown4` int(10) NOT NULL DEFAULT '0',
  `unknown5` int(10) NOT NULL DEFAULT '0',
  `unknown6` int(10) NOT NULL DEFAULT '0',
  `unknown7` int(10) NOT NULL DEFAULT '0',
  `unknown8` int(10) NOT NULL DEFAULT '0',
  `unknown9` int(10) NOT NULL DEFAULT '0',
  `unknown10` int(10) NOT NULL DEFAULT '0',
  `unknown11` int(10) NOT NULL DEFAULT '0',
  `unknown12` int(10) NOT NULL DEFAULT '0',
  `unknown13` int(10) NOT NULL DEFAULT '0',
  `unknown14` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Gameobject System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_names_localized
CREATE TABLE IF NOT EXISTS `gameobject_names_localized` (
  `entry` int(30) NOT NULL,
  `language_code` char(5) COLLATE utf8_unicode_ci NOT NULL,
  `name` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_quest_finisher
CREATE TABLE IF NOT EXISTS `gameobject_quest_finisher` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `quest` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_quest_item_binding
CREATE TABLE IF NOT EXISTS `gameobject_quest_item_binding` (
  `entry` int(10) NOT NULL DEFAULT '0',
  `quest` mediumint(8) NOT NULL DEFAULT '0',
  `item` mediumint(8) NOT NULL DEFAULT '0',
  `item_count` smallint(5) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_quest_pickup_binding
CREATE TABLE IF NOT EXISTS `gameobject_quest_pickup_binding` (
  `entry` int(10) NOT NULL DEFAULT '0',
  `quest` mediumint(8) NOT NULL DEFAULT '0',
  `required_count` tinyint(3) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_quest_starter
CREATE TABLE IF NOT EXISTS `gameobject_quest_starter` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `quest` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_spawns
CREATE TABLE IF NOT EXISTS `gameobject_spawns` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Entry` int(10) unsigned NOT NULL DEFAULT '0',
  `Map` smallint(5) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `Facing` float NOT NULL DEFAULT '0',
  `orientation1` float NOT NULL DEFAULT '0',
  `orientation2` float NOT NULL DEFAULT '0',
  `orientation3` float NOT NULL DEFAULT '0',
  `orientation4` float NOT NULL DEFAULT '0',
  `State` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Faction` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Scale` float NOT NULL DEFAULT '0',
  `phase` tinyint(3) NOT NULL DEFAULT '1',
  PRIMARY KEY (`ID`),
  KEY `Map` (`Map`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251 COLLATE=cp1251_general_cs PACK_KEYS=0 ROW_FORMAT=FIXED COMMENT='Spawn System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.gameobject_staticspawns
CREATE TABLE IF NOT EXISTS `gameobject_staticspawns` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `entry` int(10) NOT NULL,
  `map` smallint(5) NOT NULL DEFAULT '0',
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `facing` float NOT NULL,
  `o1` float NOT NULL,
  `o2` float NOT NULL,
  `o3` float NOT NULL,
  `o4` float NOT NULL,
  `state` int(10) NOT NULL DEFAULT '0',
  `flags` int(10) NOT NULL DEFAULT '0',
  `faction` int(10) NOT NULL DEFAULT '0',
  `scale` float NOT NULL,
  `ncpRespawnLink` int(10) NOT NULL DEFAULT '0',
  `phase` tinyint(3) NOT NULL,
  `vehicle` int(10) NOT NULL DEFAULT '0',
  `eventid` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Relates to events table',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.graveyards
CREATE TABLE IF NOT EXISTS `graveyards` (
  `entry` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float NOT NULL DEFAULT '0',
  `zoneid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `adjacentzoneid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `mapid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `faction` smallint(5) unsigned NOT NULL DEFAULT '0',
  `name` varchar(100) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Graveyard System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.itemloot
CREATE TABLE IF NOT EXISTS `itemloot` (
  `entryid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `itemid` mediumint(8) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '0',
  `mincount` smallint(5) unsigned NOT NULL DEFAULT '1',
  `maxcount` smallint(5) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  KEY `i_gameobj_loot_entry` (`entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.itempages
CREATE TABLE IF NOT EXISTS `itempages` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `text` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `next_page` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.itempages_localized
CREATE TABLE IF NOT EXISTS `itempages_localized` (
  `entry` mediumint(8) NOT NULL,
  `language_code` char(3) COLLATE utf8_unicode_ci NOT NULL,
  `text` text COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`,`language_code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.itempetfood
CREATE TABLE IF NOT EXISTS `itempetfood` (
  `entry` mediumint(8) NOT NULL,
  `food_type` tinyint(3) NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.items
CREATE TABLE IF NOT EXISTS `items` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `class` int(10) NOT NULL DEFAULT '0',
  `subclass` int(10) NOT NULL DEFAULT '0',
  `field4` int(10) NOT NULL DEFAULT '-1',
  `name1` varchar(255) NOT NULL,
  `name2` varchar(255) NOT NULL DEFAULT '',
  `name3` varchar(255) NOT NULL DEFAULT '',
  `name4` varchar(255) NOT NULL DEFAULT '',
  `displayid` mediumint(10) unsigned NOT NULL DEFAULT '0',
  `quality` int(10) NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `buyprice` int(10) unsigned NOT NULL DEFAULT '0',
  `sellprice` int(10) unsigned NOT NULL DEFAULT '0',
  `inventorytype` int(10) NOT NULL DEFAULT '0',
  `allowableclass` int(10) NOT NULL DEFAULT '0',
  `allowablerace` int(10) NOT NULL DEFAULT '0',
  `itemlevel` int(10) NOT NULL DEFAULT '0',
  `requiredlevel` int(10) NOT NULL DEFAULT '0',
  `RequiredSkill` int(10) NOT NULL DEFAULT '0',
  `RequiredSkillRank` int(10) NOT NULL DEFAULT '0',
  `RequiredSkillSubRank` int(10) NOT NULL DEFAULT '0',
  `RequiredPlayerRank1` int(10) NOT NULL DEFAULT '0',
  `RequiredPlayerRank2` int(10) NOT NULL DEFAULT '0',
  `RequiredFaction` int(10) NOT NULL DEFAULT '0',
  `RequiredFactionStanding` int(10) NOT NULL DEFAULT '0',
  `Unique` int(10) NOT NULL DEFAULT '0',
  `maxcount` int(10) NOT NULL DEFAULT '0',
  `ContainerSlots` int(10) NOT NULL DEFAULT '0',
  `stat_type1` int(10) NOT NULL DEFAULT '0',
  `stat_value1` int(10) NOT NULL DEFAULT '0',
  `stat_type2` int(10) NOT NULL DEFAULT '0',
  `stat_value2` int(10) NOT NULL DEFAULT '0',
  `stat_type3` int(10) NOT NULL DEFAULT '0',
  `stat_value3` int(10) NOT NULL DEFAULT '0',
  `stat_type4` int(10) NOT NULL DEFAULT '0',
  `stat_value4` int(10) NOT NULL DEFAULT '0',
  `stat_type5` int(10) NOT NULL DEFAULT '0',
  `stat_value5` int(10) NOT NULL DEFAULT '0',
  `stat_type6` int(10) NOT NULL DEFAULT '0',
  `stat_value6` int(10) NOT NULL DEFAULT '0',
  `stat_type7` int(10) NOT NULL DEFAULT '0',
  `stat_value7` int(10) NOT NULL DEFAULT '0',
  `stat_type8` int(10) NOT NULL DEFAULT '0',
  `stat_value8` int(10) NOT NULL DEFAULT '0',
  `stat_type9` int(10) NOT NULL DEFAULT '0',
  `stat_value9` int(10) NOT NULL DEFAULT '0',
  `stat_type10` int(10) NOT NULL DEFAULT '0',
  `stat_value10` int(10) NOT NULL DEFAULT '0',
  `ScalingStatId` int(30) NOT NULL DEFAULT '0',
  `ScalingStatFlags` int(30) NOT NULL DEFAULT '0',
  `dmg_min1` float NOT NULL DEFAULT '0',
  `dmg_max1` float NOT NULL DEFAULT '0',
  `dmg_type1` int(10) NOT NULL DEFAULT '0',
  `dmg_min2` float NOT NULL DEFAULT '0',
  `dmg_max2` float NOT NULL DEFAULT '0',
  `dmg_type2` int(10) NOT NULL DEFAULT '0',
  `armor` int(10) NOT NULL DEFAULT '0',
  `holy_res` int(10) NOT NULL DEFAULT '0',
  `fire_res` int(10) NOT NULL DEFAULT '0',
  `nature_res` int(10) NOT NULL DEFAULT '0',
  `frost_res` int(10) NOT NULL DEFAULT '0',
  `shadow_res` int(10) NOT NULL DEFAULT '0',
  `arcane_res` int(10) NOT NULL DEFAULT '0',
  `delay` int(10) NOT NULL DEFAULT '0',
  `ammo_type` int(10) NOT NULL DEFAULT '0',
  `range` float NOT NULL DEFAULT '0',
  `spellid_1` int(10) NOT NULL DEFAULT '0',
  `spelltrigger_1` int(10) NOT NULL DEFAULT '0',
  `spellcharges_1` int(10) NOT NULL DEFAULT '0',
  `spellcooldown_1` int(10) NOT NULL DEFAULT '0',
  `spellcategory_1` int(10) NOT NULL DEFAULT '0',
  `spellcategorycooldown_1` int(10) NOT NULL DEFAULT '0',
  `spellid_2` int(10) NOT NULL DEFAULT '0',
  `spelltrigger_2` int(10) NOT NULL DEFAULT '0',
  `spellcharges_2` int(10) NOT NULL DEFAULT '0',
  `spellcooldown_2` int(10) NOT NULL DEFAULT '0',
  `spellcategory_2` int(10) NOT NULL DEFAULT '0',
  `spellcategorycooldown_2` int(10) NOT NULL DEFAULT '0',
  `spellid_3` int(10) NOT NULL DEFAULT '0',
  `spelltrigger_3` int(10) NOT NULL DEFAULT '0',
  `spellcharges_3` int(10) NOT NULL DEFAULT '0',
  `spellcooldown_3` int(10) NOT NULL DEFAULT '0',
  `spellcategory_3` int(10) NOT NULL DEFAULT '0',
  `spellcategorycooldown_3` int(10) NOT NULL DEFAULT '0',
  `spellid_4` int(10) NOT NULL DEFAULT '0',
  `spelltrigger_4` int(10) NOT NULL DEFAULT '0',
  `spellcharges_4` int(10) NOT NULL DEFAULT '0',
  `spellcooldown_4` int(10) NOT NULL DEFAULT '0',
  `spellcategory_4` int(10) NOT NULL DEFAULT '0',
  `spellcategorycooldown_4` int(10) NOT NULL DEFAULT '0',
  `spellid_5` int(10) NOT NULL DEFAULT '0',
  `spelltrigger_5` int(10) NOT NULL DEFAULT '0',
  `spellcharges_5` int(10) NOT NULL DEFAULT '0',
  `spellcooldown_5` int(10) NOT NULL DEFAULT '0',
  `spellcategory_5` int(10) NOT NULL DEFAULT '0',
  `spellcategorycooldown_5` int(10) NOT NULL DEFAULT '0',
  `bonding` int(10) NOT NULL DEFAULT '0',
  `description` varchar(255) NOT NULL DEFAULT '',
  `page_id` int(10) NOT NULL DEFAULT '0',
  `page_language` int(10) NOT NULL DEFAULT '0',
  `page_material` int(10) NOT NULL DEFAULT '0',
  `quest_id` int(10) NOT NULL DEFAULT '0',
  `lock_id` int(10) NOT NULL DEFAULT '0',
  `lock_material` int(10) NOT NULL DEFAULT '0',
  `sheathID` int(10) NOT NULL DEFAULT '0',
  `randomprop` int(10) NOT NULL DEFAULT '0',
  `RandomSuffixId` int(10) NOT NULL DEFAULT '0',
  `block` int(10) NOT NULL DEFAULT '0',
  `itemset` int(10) NOT NULL DEFAULT '0',
  `MaxDurability` int(10) NOT NULL DEFAULT '0',
  `ZoneNameID` int(10) NOT NULL DEFAULT '0',
  `mapid` int(10) DEFAULT NULL,
  `bagfamily` int(10) NOT NULL DEFAULT '0',
  `TotemCategory` int(10) DEFAULT NULL,
  `socket_color_1` int(10) DEFAULT NULL,
  `unk201_3` int(10) NOT NULL DEFAULT '0',
  `socket_color_2` int(10) DEFAULT NULL,
  `unk201_5` int(10) NOT NULL DEFAULT '0',
  `socket_color_3` int(10) DEFAULT NULL,
  `unk201_7` int(10) NOT NULL DEFAULT '0',
  `socket_bonus` int(10) DEFAULT NULL,
  `GemProperties` int(10) DEFAULT NULL,
  `ReqDisenchantSkill` int(10) NOT NULL DEFAULT '-1',
  `Lootgold` int(10) NOT NULL DEFAULT '0',
  `ArmorDamageModifier` int(10) NOT NULL DEFAULT '0',
  `ItemLimitCategory` int(10) DEFAULT '0',
  `HolidayId` int(10) DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.items_localized
CREATE TABLE IF NOT EXISTS `items_localized` (
  `entry` mediumint(8) NOT NULL,
  `language_code` char(5) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `name` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `description` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `last_update` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`language_code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.item_quest_association
CREATE TABLE IF NOT EXISTS `item_quest_association` (
  `item` mediumint(8) NOT NULL DEFAULT '0',
  `quest` mediumint(8) NOT NULL DEFAULT '0',
  `item_count` tinyint(3) NOT NULL DEFAULT '0',
  UNIQUE KEY `item` (`item`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quest System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.item_randomprop_groups
CREATE TABLE IF NOT EXISTS `item_randomprop_groups` (
  `entry_id` mediumint(8) NOT NULL,
  `randomprops_entryid` mediumint(8) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY (`entry_id`,`randomprops_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Item System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.item_randomsuffix_groups
CREATE TABLE IF NOT EXISTS `item_randomsuffix_groups` (
  `entry_id` mediumint(8) NOT NULL,
  `randomsuffix_entryid` mediumint(8) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY (`entry_id`,`randomsuffix_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Item System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.map_checkpoint
CREATE TABLE IF NOT EXISTS `map_checkpoint` (
  `entry` smallint(5) NOT NULL,
  `prereq_checkpoint_id` smallint(5) NOT NULL,
  `creature_id` int(10) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Map System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.news_announcements
CREATE TABLE IF NOT EXISTS `news_announcements` (
  `id` int(30) unsigned NOT NULL AUTO_INCREMENT,
  `faction_mask` int(30) NOT NULL,
  `lastTime` int(30) NOT NULL,
  `message_text` varchar(500) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.npc_gossip_textid
CREATE TABLE IF NOT EXISTS `npc_gossip_textid` (
  `creatureid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `textid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`creatureid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.npc_monstersay
CREATE TABLE IF NOT EXISTS `npc_monstersay` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `event` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `chance` float NOT NULL DEFAULT '0',
  `language` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `monstername` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text0` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text1` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text2` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text3` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  `text4` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci,
  PRIMARY KEY (`entry`,`event`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.npc_text
CREATE TABLE IF NOT EXISTS `npc_text` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `prob0` float NOT NULL DEFAULT '0',
  `text0_0` longtext NOT NULL,
  `text0_1` longtext NOT NULL,
  `lang0` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em0_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em0_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em0_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em0_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em0_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em0_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob1` float NOT NULL DEFAULT '0',
  `text1_0` longtext NOT NULL,
  `text1_1` longtext NOT NULL,
  `lang1` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em1_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em1_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em1_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em1_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em1_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em1_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob2` float NOT NULL DEFAULT '0',
  `text2_0` longtext NOT NULL,
  `text2_1` longtext NOT NULL,
  `lang2` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em2_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em2_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em2_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em2_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em2_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em2_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob3` float NOT NULL DEFAULT '0',
  `text3_0` longtext NOT NULL,
  `text3_1` longtext NOT NULL,
  `lang3` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em3_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em3_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em3_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em3_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em3_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em3_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob4` float NOT NULL DEFAULT '0',
  `text4_0` longtext NOT NULL,
  `text4_1` longtext NOT NULL,
  `lang4` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em4_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em4_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em4_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em4_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em4_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em4_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob5` float NOT NULL DEFAULT '0',
  `text5_0` longtext NOT NULL,
  `text5_1` longtext NOT NULL,
  `lang5` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em5_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em5_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em5_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em5_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em5_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em5_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob6` float NOT NULL DEFAULT '0',
  `text6_0` longtext NOT NULL,
  `text6_1` longtext NOT NULL,
  `lang6` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em6_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em6_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em6_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em6_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em6_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em6_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `prob7` float NOT NULL DEFAULT '0',
  `text7_0` longtext NOT NULL,
  `text7_1` longtext NOT NULL,
  `lang7` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `em7_0` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em7_1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em7_2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em7_3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em7_4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `em7_5` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.npc_text_localized
CREATE TABLE IF NOT EXISTS `npc_text_localized` (
  `entry` mediumint(8) NOT NULL,
  `language_code` char(5) COLLATE utf8_unicode_ci NOT NULL,
  `text0` text COLLATE utf8_unicode_ci NOT NULL,
  `text0_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text1` text COLLATE utf8_unicode_ci NOT NULL,
  `text1_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text2` text COLLATE utf8_unicode_ci NOT NULL,
  `text2_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text3` text COLLATE utf8_unicode_ci NOT NULL,
  `text3_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text4` text COLLATE utf8_unicode_ci NOT NULL,
  `text4_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text5` text COLLATE utf8_unicode_ci NOT NULL,
  `text5_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text6` text COLLATE utf8_unicode_ci NOT NULL,
  `text6_1` text COLLATE utf8_unicode_ci NOT NULL,
  `text7` text COLLATE utf8_unicode_ci NOT NULL,
  `text7_1` text COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.objectloot
CREATE TABLE IF NOT EXISTS `objectloot` (
  `entryid` int(10) unsigned NOT NULL DEFAULT '0',
  `itemid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `difficulty0percentchance` float DEFAULT NULL,
  `difficulty1percentchance` float DEFAULT NULL,
  `difficulty2percentchance` float NOT NULL DEFAULT '0',
  `difficulty3percentchance` float NOT NULL DEFAULT '0',
  `mincount` smallint(5) unsigned NOT NULL DEFAULT '1',
  `maxcount` smallint(5) unsigned NOT NULL DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.petdefaultspells
CREATE TABLE IF NOT EXISTS `petdefaultspells` (
  `entry` int(10) NOT NULL DEFAULT '0',
  `spell` int(10) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Pet System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.pickpocketingloot
CREATE TABLE IF NOT EXISTS `pickpocketingloot` (
  `entryid` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `itemid` mediumint(8) unsigned NOT NULL DEFAULT '25',
  `percentchance` float NOT NULL DEFAULT '100',
  `mincount` tinyint(3) DEFAULT '1',
  `maxcount` tinyint(3) DEFAULT '1',
  `ffa_loot` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.playercreateinfo
CREATE TABLE IF NOT EXISTS `playercreateinfo` (
  `Index` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `race` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `factiontemplate` smallint(5) NOT NULL DEFAULT '0',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `mapID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `zoneID` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  `Orientation` float NOT NULL DEFAULT '0',
  `displayID` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseStrength` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseAgility` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseStamina` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseIntellect` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseSpirit` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `BaseHealth` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseMana` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `BaseRage` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseFocus` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseEnergy` smallint(5) unsigned NOT NULL DEFAULT '0',
  `BaseRunic` smallint(5) DEFAULT NULL,
  `attackpower` int(8) unsigned NOT NULL DEFAULT '0',
  `mindmg` float NOT NULL DEFAULT '0',
  `maxdmg` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`Index`,`race`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Player System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.playercreateinfo_bars
CREATE TABLE IF NOT EXISTS `playercreateinfo_bars` (
  `race` tinyint(3) unsigned DEFAULT NULL,
  `class` tinyint(3) unsigned DEFAULT NULL,
  `button` int(10) unsigned DEFAULT NULL,
  `action` int(10) unsigned DEFAULT NULL,
  `type` int(10) unsigned DEFAULT NULL,
  `misc` int(10) unsigned DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.playercreateinfo_items
CREATE TABLE IF NOT EXISTS `playercreateinfo_items` (
  `indexid` tinyint(3) NOT NULL DEFAULT '0',
  `protoid` mediumint(8) NOT NULL DEFAULT '0',
  `slotid` tinyint(3) NOT NULL DEFAULT '0',
  `amount` smallint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`indexid`,`protoid`,`slotid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.playercreateinfo_skills
CREATE TABLE IF NOT EXISTS `playercreateinfo_skills` (
  `indexid` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `skillid` smallint(5) unsigned NOT NULL DEFAULT '0',
  `level` smallint(5) unsigned NOT NULL DEFAULT '0',
  `maxlevel` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`indexid`,`skillid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.playercreateinfo_spells
CREATE TABLE IF NOT EXISTS `playercreateinfo_spells` (
  `indexid` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `spellid` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`indexid`,`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Player System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.prestartqueries
CREATE TABLE IF NOT EXISTS `prestartqueries` (
  `Query` varchar(1024) NOT NULL,
  `SingleShot` int(10) unsigned NOT NULL DEFAULT '1',
  `Seq` int(10) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`Seq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.professiondiscoveries
CREATE TABLE IF NOT EXISTS `professiondiscoveries` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `SpellToDiscover` int(10) unsigned NOT NULL DEFAULT '0',
  `SkillValue` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Chance` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`,`SpellToDiscover`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.quests
CREATE TABLE IF NOT EXISTS `quests` (
  `entry` mediumint(8) unsigned NOT NULL,
  `ZoneId` smallint(5) NOT NULL DEFAULT '0',
  `sort` int(10) NOT NULL DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `questlevel` tinyint(3) NOT NULL DEFAULT '0',
  `Type` int(10) NOT NULL DEFAULT '0',
  `RequiredRaces` int(10) NOT NULL DEFAULT '0',
  `RequiredClass` int(10) NOT NULL DEFAULT '0',
  `RequiredTradeskill` int(10) NOT NULL DEFAULT '0',
  `RequiredTradeskillValue` int(10) NOT NULL DEFAULT '0',
  `RequiredRepFaction` int(10) NOT NULL DEFAULT '0',
  `RequiredRepValue` int(10) NOT NULL DEFAULT '0',
  `SuggestedPlayers` int(10) unsigned NOT NULL DEFAULT '0',
  `LimitTime` int(10) NOT NULL DEFAULT '0',
  `SpecialFlags` int(10) unsigned NOT NULL DEFAULT '0',
  `PrevQuestId` mediumint(8) NOT NULL DEFAULT '0',
  `NextQuestId` mediumint(8) NOT NULL DEFAULT '0',
  `srcItem` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `SrcItemCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  `Title` text NOT NULL,
  `Details` text NOT NULL,
  `Objectives` text NOT NULL,
  `CompletionText` text NOT NULL,
  `IncompleteText` text NOT NULL,
  `EndText` text NOT NULL,
  `ObjectiveText1` text NOT NULL,
  `ObjectiveText2` text NOT NULL,
  `ObjectiveText3` text NOT NULL,
  `ObjectiveText4` text NOT NULL,
  `ReqItemId1` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ReqItemId2` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ReqItemId3` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ReqItemId4` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ReqItemId5` mediumint(8) NOT NULL DEFAULT '0',
  `ReqItemId6` mediumint(8) NOT NULL DEFAULT '0',
  `ReqItemCount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqItemCount5` smallint(5) NOT NULL DEFAULT '0',
  `ReqItemCount6` smallint(5) NOT NULL DEFAULT '0',
  `ReqKillPlayer` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId1` mediumint(8) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId2` mediumint(8) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId3` mediumint(8) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOId4` mediumint(8) NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqKillMobOrGOCount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `ReqCastSpellId1` int(10) NOT NULL DEFAULT '0',
  `ReqCastSpellId2` int(10) NOT NULL DEFAULT '0',
  `ReqCastSpellId3` int(10) NOT NULL DEFAULT '0',
  `ReqCastSpellId4` int(10) NOT NULL DEFAULT '0',
  `RewChoiceItemId1` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId2` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId3` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId4` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId5` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemId6` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount5` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewChoiceItemCount6` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewItemId1` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewItemId2` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewItemId3` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewItemId4` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `RewItemCount1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewItemCount2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewItemCount3` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewItemCount4` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewRepFaction1` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewRepFaction2` smallint(5) unsigned NOT NULL DEFAULT '0',
  `RewRepFaction3` smallint(5) NOT NULL DEFAULT '0',
  `RewRepFaction4` smallint(5) NOT NULL DEFAULT '0',
  `RewRepFaction5` smallint(5) NOT NULL DEFAULT '0',
  `RewRepValue1` smallint(5) NOT NULL DEFAULT '0',
  `RewRepValue2` smallint(5) NOT NULL DEFAULT '0',
  `RewRepValue3` smallint(5) NOT NULL DEFAULT '0',
  `RewRepValue4` smallint(5) NOT NULL DEFAULT '0',
  `RewRepValue5` smallint(5) NOT NULL DEFAULT '0',
  `RewRepLimit` int(10) unsigned NOT NULL DEFAULT '0',
  `RewTitle` int(10) unsigned NOT NULL DEFAULT '0',
  `RewMoney` int(10) NOT NULL DEFAULT '0',
  `RewBonusHonor` int(10) unsigned NOT NULL DEFAULT '0',
  `RewXP` int(10) unsigned NOT NULL DEFAULT '0',
  `RewSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `RewTalent` int(10) unsigned NOT NULL DEFAULT '0',
  `CastSpell` int(10) unsigned NOT NULL DEFAULT '0',
  `PointMapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `PointX` float NOT NULL DEFAULT '0',
  `PointY` float NOT NULL DEFAULT '0',
  `PointOpt` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredMoney` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger1` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger2` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger3` int(10) unsigned NOT NULL DEFAULT '0',
  `ExploreTrigger4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest1` int(10) NOT NULL DEFAULT '0',
  `RequiredQuest2` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest3` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest4` int(10) unsigned NOT NULL DEFAULT '0',
  `RequiredQuest_and_or` int(3) NOT NULL DEFAULT '0',
  `ReceiveItemId1` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemId2` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemId3` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemId4` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount1` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount2` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount3` int(10) unsigned NOT NULL DEFAULT '0',
  `ReceiveItemCount4` int(10) unsigned NOT NULL DEFAULT '0',
  `IsRepeatable` tinyint(3) NOT NULL DEFAULT '0',
  `reward_arenapoints` int(10) unsigned NOT NULL DEFAULT '0',
  `start_phase` int(10) unsigned NOT NULL DEFAULT '0',
  `complete_phase` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Quests System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.quests_localized
CREATE TABLE IF NOT EXISTS `quests_localized` (
  `entry` mediumint(10) NOT NULL,
  `language_code` char(5) COLLATE utf8_unicode_ci NOT NULL,
  `Title` text COLLATE utf8_unicode_ci NOT NULL,
  `Details` text COLLATE utf8_unicode_ci NOT NULL,
  `Objectives` text COLLATE utf8_unicode_ci NOT NULL,
  `CompletionText` text COLLATE utf8_unicode_ci NOT NULL,
  `IncompleteText` text COLLATE utf8_unicode_ci NOT NULL,
  `EndText` text COLLATE utf8_unicode_ci NOT NULL,
  `ObjectiveText1` text COLLATE utf8_unicode_ci NOT NULL,
  `ObjectiveText2` text COLLATE utf8_unicode_ci NOT NULL,
  `ObjectiveText3` text COLLATE utf8_unicode_ci NOT NULL,
  `ObjectiveText4` text COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.quest_poi
CREATE TABLE IF NOT EXISTS `quest_poi` (
  `questId` int(10) unsigned NOT NULL DEFAULT '0',
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `objIndex` int(10) NOT NULL DEFAULT '0',
  `mapid` int(10) unsigned NOT NULL DEFAULT '0',
  `WorldMapAreaId` int(10) unsigned NOT NULL DEFAULT '0',
  `FloorId` int(10) unsigned NOT NULL DEFAULT '0',
  `unk3` int(10) unsigned NOT NULL DEFAULT '0',
  `unk4` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`questId`,`id`),
  KEY `questId` (`questId`,`id`),
  KEY `id` (`id`,`questId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.quest_poi_points
CREATE TABLE IF NOT EXISTS `quest_poi_points` (
  `questId` int(10) unsigned NOT NULL DEFAULT '0',
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `x` int(10) NOT NULL DEFAULT '0',
  `y` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`questId`,`id`,`x`,`y`),
  KEY `questId_id` (`questId`,`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.randomcardcreation
CREATE TABLE IF NOT EXISTS `randomcardcreation` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId0` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId1` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId2` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId3` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId4` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId5` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId6` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId7` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId8` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId9` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId10` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId11` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId12` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId13` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId14` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId15` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId16` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId17` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId18` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId19` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId20` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId21` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId22` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId23` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId24` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId25` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId26` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId27` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId28` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId29` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId30` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemId31` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `ItemCount` smallint(5) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.randomitemcreation
CREATE TABLE IF NOT EXISTS `randomitemcreation` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemToCreate` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `Skill` int(10) unsigned NOT NULL DEFAULT '0',
  `Chance` float unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ItemToCreate`,`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.recall
CREATE TABLE IF NOT EXISTS `recall` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `MapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `positionX` float NOT NULL DEFAULT '0',
  `positionY` float NOT NULL DEFAULT '0',
  `positionZ` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Tele Command';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.reputation_creature_onkill
CREATE TABLE IF NOT EXISTS `reputation_creature_onkill` (
  `creature_id` mediumint(8) NOT NULL,
  `faction_change_alliance` smallint(5) NOT NULL,
  `faction_change_horde` smallint(5) NOT NULL,
  `change_value` smallint(5) NOT NULL,
  `rep_limit` mediumint(8) NOT NULL,
  KEY `index` (`creature_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.reputation_faction_onkill
CREATE TABLE IF NOT EXISTS `reputation_faction_onkill` (
  `faction_id` smallint(5) NOT NULL,
  `change_factionid_alliance` smallint(5) NOT NULL,
  `change_deltamin_alliance` smallint(5) NOT NULL,
  `change_deltamax_alliance` smallint(5) NOT NULL,
  `change_factionid_horde` smallint(5) NOT NULL,
  `change_deltamin_horde` smallint(5) NOT NULL,
  `change_deltamax_horde` smallint(5) NOT NULL,
  KEY `factindex` (`faction_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.reputation_instance_onkill
CREATE TABLE IF NOT EXISTS `reputation_instance_onkill` (
  `mapid` smallint(5) NOT NULL,
  `mob_rep_reward` smallint(5) NOT NULL,
  `mob_rep_reward_heroic` smallint(5) NOT NULL,
  `mob_rep_limit` mediumint(8) NOT NULL,
  `mob_rep_limit_heroic` mediumint(8) NOT NULL,
  `boss_rep_reward` smallint(5) NOT NULL,
  `boss_rep_reward_heroic` smallint(5) NOT NULL,
  `boss_rep_limit` mediumint(8) NOT NULL,
  `boss_rep_limit_heroic` mediumint(8) NOT NULL,
  `faction_change_alliance` smallint(5) NOT NULL,
  `faction_change_horde` smallint(5) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Creature System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.scrollcreation
CREATE TABLE IF NOT EXISTS `scrollcreation` (
  `SpellId` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`SpellId`,`ItemId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spellfixes
CREATE TABLE IF NOT EXISTS `spellfixes` (
  `spellId` int(10) NOT NULL DEFAULT '0',
  `procFlags` int(10) NOT NULL DEFAULT '0',
  `procChance` int(10) NOT NULL DEFAULT '0',
  `procCharges` int(10) NOT NULL DEFAULT '0',
  `effect0ClassMask0` int(10) unsigned NOT NULL DEFAULT '0',
  `effect0ClassMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `effect0ClassMask2` int(10) unsigned NOT NULL DEFAULT '0',
  `effect1ClassMask0` int(10) unsigned NOT NULL DEFAULT '0',
  `effect1ClassMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `effect1ClassMask2` int(10) unsigned NOT NULL DEFAULT '0',
  `effect2ClassMask0` int(10) unsigned NOT NULL DEFAULT '0',
  `effect2ClassMask1` int(10) unsigned NOT NULL DEFAULT '0',
  `effect2ClassMask2` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spelloverride
CREATE TABLE IF NOT EXISTS `spelloverride` (
  `overrideId` int(30) unsigned NOT NULL DEFAULT '0',
  `spellId` int(30) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `overrideId` (`overrideId`,`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spell_coef_override
CREATE TABLE IF NOT EXISTS `spell_coef_override` (
  `id` int(10) NOT NULL DEFAULT '0',
  `spell_coef_override` float NOT NULL DEFAULT '0',
  `AP_coef_override` float NOT NULL DEFAULT '0',
  `RAP_coef_override` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spell_disable
CREATE TABLE IF NOT EXISTS `spell_disable` (
  `spellid` int(10) NOT NULL,
  `replacement_spellid` int(10) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spell_disable_trainers
CREATE TABLE IF NOT EXISTS `spell_disable_trainers` (
  `spellid` int(10) NOT NULL,
  `replacement_spellid` int(10) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Trainer System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spell_effects_override
CREATE TABLE IF NOT EXISTS `spell_effects_override` (
  `spellId` int(10) NOT NULL DEFAULT '0',
  `EffectID` int(10) NOT NULL,
  `Disable` int(10) NOT NULL,
  `Effect` int(10) NOT NULL DEFAULT '0',
  `BasePoints` int(10) NOT NULL DEFAULT '0',
  `ApplyAuraName` int(10) NOT NULL DEFAULT '0',
  `SpellGroupRelation` int(10) NOT NULL DEFAULT '0',
  `MiscValue` int(10) NOT NULL DEFAULT '0',
  `TriggerSpell` int(10) NOT NULL DEFAULT '0',
  `ImplicitTargetA` int(10) NOT NULL DEFAULT '0',
  `ImplicitTargetB` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spell_forced_targets
CREATE TABLE IF NOT EXISTS `spell_forced_targets` (
  `spellid` int(10) unsigned NOT NULL,
  `target` mediumint(8) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellid`,`target`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spell System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.spell_proc
CREATE TABLE IF NOT EXISTS `spell_proc` (
  `spellID` int(30) NOT NULL DEFAULT '0',
  `ProcOnNameHash` int(30) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spellID`,`ProcOnNameHash`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.teleport_coords
CREATE TABLE IF NOT EXISTS `teleport_coords` (
  `entry` int(10) NOT NULL,
  `name` char(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,
  `mapId` smallint(5) unsigned NOT NULL DEFAULT '0',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `orientation` float unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.totemspells
CREATE TABLE IF NOT EXISTS `totemspells` (
  `spell` int(10) unsigned NOT NULL DEFAULT '0',
  `castspell1` int(10) unsigned NOT NULL DEFAULT '0',
  `castspell2` int(10) unsigned NOT NULL DEFAULT '0',
  `castspell3` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`spell`,`castspell1`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Spell System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.trainerspelloverride
CREATE TABLE IF NOT EXISTS `trainerspelloverride` (
  `spellid` int(10) unsigned NOT NULL DEFAULT '0',
  `cost` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredspell` int(10) unsigned NOT NULL DEFAULT '0',
  `deletespell` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredskill` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredskillvalue` int(10) unsigned NOT NULL DEFAULT '0',
  `reqlevel` int(10) unsigned NOT NULL DEFAULT '0',
  `requiredclass` int(10) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `spellid` (`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Trainer System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.trainer_defs
CREATE TABLE IF NOT EXISTS `trainer_defs` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `required_skill` int(10) unsigned NOT NULL DEFAULT '0',
  `required_skillvalue` smallint(5) unsigned NOT NULL DEFAULT '0',
  `req_class` smallint(11) unsigned NOT NULL DEFAULT '0',
  `trainer_type` tinyint(11) unsigned NOT NULL DEFAULT '0',
  `trainer_ui_window_message` varchar(255) NOT NULL DEFAULT '',
  `can_train_gossip_textid` int(11) NOT NULL DEFAULT '0',
  `cannot_train_gossip_textid` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.trainer_spells
CREATE TABLE IF NOT EXISTS `trainer_spells` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `cast_spell` int(10) unsigned NOT NULL DEFAULT '0',
  `learn_spell` int(10) unsigned NOT NULL,
  `spellcost` int(10) unsigned NOT NULL DEFAULT '0',
  `reqspell` int(10) unsigned NOT NULL DEFAULT '0',
  `reqskill` smallint(5) unsigned NOT NULL DEFAULT '0',
  `reqskillvalue` smallint(5) unsigned NOT NULL DEFAULT '0',
  `reqlevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `deletespell` int(10) unsigned NOT NULL DEFAULT '0',
  `is_prof` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`,`cast_spell`,`learn_spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.transport_creatures
CREATE TABLE IF NOT EXISTS `transport_creatures` (
  `transport_entry` int(10) unsigned NOT NULL,
  `creature_entry` int(10) unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL,
  PRIMARY KEY (`transport_entry`,`creature_entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.vendors
CREATE TABLE IF NOT EXISTS `vendors` (
  `entry` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `item` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `amount` int(11) NOT NULL DEFAULT '0',
  `max_amount` int(11) NOT NULL DEFAULT '0',
  `inctime` bigint(20) NOT NULL DEFAULT '0',
  `extendedcost` int(11) NOT NULL DEFAULT '1',
  `isdependent` int(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`entry`,`item`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='NPC System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.weather
CREATE TABLE IF NOT EXISTS `weather` (
  `zoneId` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `high_chance` float unsigned NOT NULL DEFAULT '0',
  `high_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `med_chance` float unsigned NOT NULL DEFAULT '0',
  `med_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `low_chance` float unsigned NOT NULL DEFAULT '0',
  `low_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`zoneId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Weather System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.wordfilter_character_names
CREATE TABLE IF NOT EXISTS `wordfilter_character_names` (
  `regex_match` varchar(255) NOT NULL,
  `regex_ignore_if_matched` varchar(255) DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.wordfilter_chat
CREATE TABLE IF NOT EXISTS `wordfilter_chat` (
  `regex_match` varchar(500) NOT NULL,
  `regex_ignore_if_matched` varchar(500) NOT NULL DEFAULT ''
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.worldmap_info
CREATE TABLE IF NOT EXISTS `worldmap_info` (
  `entry` smallint(5) unsigned NOT NULL DEFAULT '0',
  `name` varchar(100) DEFAULT '',
  `load` int(3) unsigned NOT NULL DEFAULT '1',
  `type` tinyint(3) unsigned DEFAULT '0',
  `maxplayers` smallint(5) unsigned DEFAULT '0',
  `minlevel` smallint(5) unsigned DEFAULT '1',
  `repopx` float DEFAULT '0',
  `repopy` float DEFAULT '0',
  `repopz` float DEFAULT '0',
  `repopentry` int(10) unsigned DEFAULT '0',
  `flags` int(10) unsigned NOT NULL DEFAULT '0',
  `cooldown` int(10) unsigned NOT NULL DEFAULT '0',
  `required_quest` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `required_item` mediumint(8) unsigned NOT NULL DEFAULT '0',
  `heroic_keyid_1` mediumint(8) NOT NULL DEFAULT '0',
  `heroic_keyid_2` mediumint(8) NOT NULL DEFAULT '0',
  `viewingDistance` float NOT NULL DEFAULT '80',
  `required_checkpoint` int(10) NOT NULL DEFAULT '0',
  `hordephase` tinyint(4) unsigned NOT NULL DEFAULT '0',
  `alliancephase` tinyint(4) unsigned NOT NULL DEFAULT '0',
  `collision` tinyint(3) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

# Data exporting was unselected.


# Dumping structure for table worldtemplate.worldstate_template
CREATE TABLE IF NOT EXISTS `worldstate_template` (
  `mapid` int(10) NOT NULL,
  `zone_mask` int(10) NOT NULL,
  `faction_mask` int(10) NOT NULL,
  `field_number` int(10) unsigned NOT NULL,
  `initial_value` int(10) NOT NULL,
  `comment` varchar(200) NOT NULL,
  PRIMARY KEY (`field_number`,`mapid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

# Data exporting was unselected.


# Dumping structure for table worldtemplate.zoneguards
CREATE TABLE IF NOT EXISTS `zoneguards` (
  `zone` smallint(5) unsigned NOT NULL,
  `horde_entry` mediumint(8) unsigned DEFAULT NULL,
  `alliance_entry` mediumint(8) unsigned DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='World System';

# Data exporting was unselected.
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
