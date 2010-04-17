/*
SQLyog Ultimate v8.4 Beta3
MySQL - 5.0.51a-24+lenny3-log : Database - SADB_world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `achievement_rewards` */

DROP TABLE IF EXISTS `achievement_rewards`;

CREATE TABLE `achievement_rewards` (
  `achievementid` int(10) unsigned NOT NULL,
  `title_alliance` int(10) unsigned NOT NULL default '0',
  `title_horde` int(10) unsigned NOT NULL default '0',
  `itemid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`achievementid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `ai_agents` */

DROP TABLE IF EXISTS `ai_agents`;

CREATE TABLE `ai_agents` (
  `entry` int(11) unsigned NOT NULL default '0',
  `type` enum('MELEE','RANGED','FLEE','SPELL','CALLFORHELP') collate utf8_unicode_ci NOT NULL default 'SPELL',
  `chance` int(11) unsigned NOT NULL default '0',
  `maxcount` int(11) unsigned NOT NULL default '0',
  `spell` int(11) unsigned NOT NULL default '0',
  `spelltype` enum('ROOT','HEAL','STUN','FEAR','SILENCE','CURSE','AOEDAMAGE','DAMAGE','SUMMON','BUFF','DEBUFF') collate utf8_unicode_ci NOT NULL default 'ROOT',
  `targettype` enum('RANDOMTARGET','TARGETLOCATION','CREATURELOCATION','SELF','OWNER') collate utf8_unicode_ci NOT NULL default 'RANDOMTARGET',
  `cooldown` int(8) NOT NULL default '0',
  `floatMisc1` float NOT NULL default '0',
  `Misc2` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`,`type`,`spell`),
  UNIQUE KEY `a` (`entry`,`spell`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='AI System';

/*Table structure for table `ai_threattospellid` */

DROP TABLE IF EXISTS `ai_threattospellid`;

CREATE TABLE `ai_threattospellid` (
  `spell` int(11) unsigned NOT NULL default '0',
  `mod` int(11) NOT NULL default '0',
  PRIMARY KEY  (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='AI System';

/*Table structure for table `areatriggers` */

DROP TABLE IF EXISTS `areatriggers`;

CREATE TABLE `areatriggers` (
  `entry` int(11) unsigned NOT NULL default '0',
  `type` tinyint(3) unsigned default '0',
  `map` int(11) unsigned default NULL,
  `screen` int(11) unsigned default NULL,
  `name` varchar(100) default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `required_honor_rank` int(11) unsigned NOT NULL default '0',
  `required_level` tinyint(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Trigger System';

/*Table structure for table `auctionhouse` */

DROP TABLE IF EXISTS `auctionhouse`;

CREATE TABLE `auctionhouse` (
  `id` int(32) NOT NULL auto_increment,
  `creature_entry` bigint(64) unsigned NOT NULL default '0',
  `group` int(32) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=41 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Auction House';

/*Table structure for table `command_overrides` */

DROP TABLE IF EXISTS `command_overrides`;

CREATE TABLE `command_overrides` (
  `command_name` varchar(100) collate utf8_unicode_ci NOT NULL,
  `access_level` varchar(10) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`command_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Commands System';

/*Table structure for table `creature_formations` */

DROP TABLE IF EXISTS `creature_formations`;

CREATE TABLE `creature_formations` (
  `spawn_id` int(10) unsigned NOT NULL default '0',
  `target_spawn_id` int(10) unsigned NOT NULL default '0',
  `follow_angle` float NOT NULL default '0',
  `follow_dist` float NOT NULL default '0',
  PRIMARY KEY  (`spawn_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Creature System';

/*Table structure for table `creature_names` */

DROP TABLE IF EXISTS `creature_names`;

CREATE TABLE `creature_names` (
  `entry` int(20) NOT NULL default '0',
  `name` varchar(100) character set utf8 collate utf8_unicode_ci NOT NULL,
  `subname` varchar(100) character set utf8 collate utf8_unicode_ci NOT NULL,
  `info_str` varchar(500) character set utf8 collate utf8_unicode_ci NOT NULL,
  `Flags1` int(30) unsigned NOT NULL default '0',
  `type` int(30) NOT NULL default '0',
  `loot_skill_type` int(10) NOT NULL default '1',
  `family` int(30) NOT NULL default '0',
  `rank` int(30) NOT NULL default '0',
  `unk4` int(30) NOT NULL default '0',
  `spelldataid` int(30) NOT NULL default '0',
  `male_displayid` int(30) NOT NULL,
  `female_displayid` int(30) NOT NULL,
  `male_displayid2` int(30) NOT NULL default '0',
  `female_displayid2` int(30) NOT NULL default '0',
  `unknown_float1` float NOT NULL default '0',
  `unknown_float2` float NOT NULL default '0',
  `civilian` tinyint(30) NOT NULL default '0',
  `leader` tinyint(30) NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Creature System';

/*Table structure for table `creature_names_localized` */

DROP TABLE IF EXISTS `creature_names_localized`;

CREATE TABLE `creature_names_localized` (
  `id` int(30) unsigned NOT NULL,
  `language_code` varchar(5) character set utf8 NOT NULL,
  `name` varchar(100) character set utf8 NOT NULL,
  `subname` varchar(100) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`id`,`language_code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `creature_proto` */

DROP TABLE IF EXISTS `creature_proto`;

CREATE TABLE `creature_proto` (
  `entry` int(30) unsigned NOT NULL default '0',
  `minlevel` int(30) unsigned NOT NULL default '1',
  `maxlevel` int(30) unsigned NOT NULL default '1',
  `faction` int(30) unsigned NOT NULL default '0',
  `minhealth` int(30) unsigned NOT NULL default '1',
  `maxhealth` int(30) unsigned NOT NULL default '1',
  `powertype` int(11) NOT NULL default '0',
  `power` int(10) default NULL,
  `scale` float NOT NULL default '0',
  `npcflags` int(30) unsigned NOT NULL default '0',
  `attacktime` int(30) unsigned NOT NULL default '0',
  `attacktype` int(4) NOT NULL default '0',
  `mindamage` float NOT NULL default '0',
  `maxdamage` float NOT NULL default '0',
  `rangedattacktime` int(30) unsigned NOT NULL default '0',
  `rangedmindamage` float unsigned NOT NULL default '0',
  `rangedmaxdamage` float unsigned NOT NULL default '0',
  `Item1` int(30) unsigned NOT NULL default '0',
  `Item2` int(30) unsigned NOT NULL default '0',
  `Item3` int(30) unsigned NOT NULL default '0',
  `respawntime` int(30) unsigned NOT NULL default '0',
  `resistance1` int(30) unsigned NOT NULL default '0',
  `resistance2` int(30) unsigned NOT NULL default '0',
  `resistance3` int(30) unsigned NOT NULL default '0',
  `resistance4` int(30) unsigned NOT NULL default '0',
  `resistance5` int(30) unsigned NOT NULL default '0',
  `resistance6` int(30) unsigned NOT NULL default '0',
  `resistance7` int(30) unsigned NOT NULL default '0',
  `combat_reach` float NOT NULL default '0',
  `bounding_radius` float NOT NULL default '0',
  `auras` longtext NOT NULL,
  `boss` int(11) unsigned NOT NULL default '0',
  `money` int(30) NOT NULL default '0',
  `no_XP` int(11) NOT NULL default '0',
  `invisibility_type` int(30) unsigned NOT NULL default '0',
  `death_state` int(30) unsigned NOT NULL default '0',
  `walk_speed` float NOT NULL default '2.5',
  `run_speed` float NOT NULL default '8',
  `fly_speed` float NOT NULL default '14',
  `extra_a9_flags` int(30) NOT NULL default '0',
  `auraimmune_flag` int(11) NOT NULL default '0',
  `vehicle_entry` int(11) NOT NULL default '-1',
  `SpellClickId` int(30) NOT NULL default '0',
  `CanMove` int(11) NOT NULL default '7',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Creature System';

/*Table structure for table `creature_proto_heroic` */

DROP TABLE IF EXISTS `creature_proto_heroic`;

CREATE TABLE `creature_proto_heroic` (
  `entry` int(10) NOT NULL,
  `minlevel` int(10) unsigned NOT NULL default '1',
  `maxlevel` int(10) unsigned NOT NULL default '1',
  `minhealth` int(10) NOT NULL default '1',
  `maxhealth` int(10) NOT NULL default '1',
  `mindmg` float NOT NULL default '1',
  `maxdmg` float NOT NULL default '1',
  `powertype` int(11) NOT NULL default '0',
  `power` int(10) default NULL,
  `resistance1` int(10) NOT NULL default '0',
  `resistance2` int(10) NOT NULL default '0',
  `resistance3` int(10) NOT NULL default '0',
  `resistance4` int(10) NOT NULL default '0',
  `resistance5` int(10) NOT NULL default '0',
  `resistance6` int(10) NOT NULL default '0',
  `resistance7` int(10) NOT NULL default '0',
  `auras` longtext NOT NULL,
  `auraimmune_flag` int(10) NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `creature_proto_vehicle` */

DROP TABLE IF EXISTS `creature_proto_vehicle`;

CREATE TABLE `creature_proto_vehicle` (
  `vehiclecreatureid` int(30) NOT NULL default '1',
  `increasehealthbydriver` int(4) NOT NULL default '0',
  `healthforitemlevel` int(30) NOT NULL default '0',
  `spell1` int(30) NOT NULL default '0',
  `spell2` int(30) NOT NULL default '0',
  `spell3` int(30) NOT NULL default '0',
  `spell4` int(30) NOT NULL default '0',
  `spell5` int(30) NOT NULL default '0',
  `spell6` int(30) NOT NULL default '0',
  `accessoryseat1` int(30) NOT NULL default '0',
  `accessoryseat2` int(30) NOT NULL default '0',
  `accessoryseat3` int(30) NOT NULL default '0',
  `accessoryseat4` int(30) NOT NULL default '0',
  `accessoryseat5` int(30) NOT NULL default '0',
  `accessoryseat6` int(30) NOT NULL default '0',
  `accessoryseat7` int(30) NOT NULL default '0',
  `accessoryseat8` int(30) NOT NULL default '0',
  PRIMARY KEY  (`vehiclecreatureid`),
  UNIQUE KEY `Vehicle` (`vehiclecreatureid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `creature_quest_finisher` */

DROP TABLE IF EXISTS `creature_quest_finisher`;

CREATE TABLE `creature_quest_finisher` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci ROW_FORMAT=FIXED COMMENT='Creature System';

/*Table structure for table `creature_quest_starter` */

DROP TABLE IF EXISTS `creature_quest_starter`;

CREATE TABLE `creature_quest_starter` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci ROW_FORMAT=FIXED COMMENT='Creature System';

/*Table structure for table `creature_spawns` */

DROP TABLE IF EXISTS `creature_spawns`;

CREATE TABLE `creature_spawns` (
  `id` int(30) NOT NULL auto_increment,
  `entry` int(30) NOT NULL,
  `map` int(30) NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL,
  `movetype` int(30) NOT NULL default '0',
  `displayid` int(30) unsigned NOT NULL default '0',
  `faction` int(30) NOT NULL default '35',
  `flags` int(20) NOT NULL default '0',
  `bytes` int(10) NOT NULL default '0',
  `bytes1` int(10) NOT NULL default '0',
  `bytes2` int(10) NOT NULL default '0',
  `emote_state` int(30) NOT NULL default '0',
  `channel_spell` int(30) unsigned NOT NULL default '0',
  `channel_target_sqlid_go` int(30) NOT NULL default '0',
  `channel_target_sqlid_creature` int(30) NOT NULL default '0',
  `standstate` int(10) NOT NULL default '0',
  `MountedDisplayID` int(11) NOT NULL default '0',
  `phase` int(11) NOT NULL default '1',
  `vehicle` int(10) NOT NULL default '0',
  `eventid` tinyint(3) unsigned NOT NULL default '0' COMMENT 'Relates to events table',
  PRIMARY KEY  (`id`),
  KEY `b` (`map`)
) ENGINE=MyISAM AUTO_INCREMENT=212110 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Spawn System';

/*Table structure for table `creature_staticspawns` */

DROP TABLE IF EXISTS `creature_staticspawns`;

CREATE TABLE `creature_staticspawns` (
  `id` int(30) NOT NULL auto_increment,
  `entry` int(30) NOT NULL,
  `map` int(11) NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `o` float NOT NULL,
  `movetype` int(11) NOT NULL default '0',
  `displayid` int(11) NOT NULL,
  `factionid` int(11) NOT NULL default '0',
  `flags` int(30) NOT NULL default '0',
  `bytes` int(30) NOT NULL default '0',
  `bytes1` int(30) NOT NULL default '0',
  `bytes2` int(30) NOT NULL default '0',
  `emote_state` int(11) NOT NULL default '0',
  `channel_spell` int(30) NOT NULL default '0',
  `channel_target_sqlid_go` int(30) NOT NULL default '0',
  `channel_target_sqlid_creature` int(30) NOT NULL default '0',
  `MountedDisplayID` int(11) NOT NULL default '0',
  `Phase` int(11) NOT NULL default '1',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

/*Table structure for table `creature_waypoints` */

DROP TABLE IF EXISTS `creature_waypoints`;

CREATE TABLE `creature_waypoints` (
  `spawnid` int(10) NOT NULL,
  `waypointid` int(2) NOT NULL default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `waittime` int(6) NOT NULL default '0',
  `flags` int(3) NOT NULL default '0',
  `forwardemoteoneshot` tinyint(1) NOT NULL default '0',
  `forwardemoteid` int(4) NOT NULL default '0',
  `backwardemoteoneshot` tinyint(1) NOT NULL default '0',
  `backwardemoteid` int(4) NOT NULL default '0',
  `forwardskinid` int(10) NOT NULL default '0',
  `backwardskinid` int(10) NOT NULL default '0',
  `forwardStandState` int(11) NOT NULL default '0',
  `backwardStandState` int(11) NOT NULL default '0',
  `forwardSpellToCast` int(11) NOT NULL default '0',
  `backwardSpellToCast` int(11) NOT NULL default '0',
  `forwardSayText` varchar(1024) collate utf8_unicode_ci NOT NULL,
  `backwardSayText` varchar(1024) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`spawnid`,`waypointid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Creature System';

/*Table structure for table `creatureloot` */

DROP TABLE IF EXISTS `creatureloot`;

CREATE TABLE `creatureloot` (
  `entryid` int(11) NOT NULL default '0',
  `itemid` int(11) NOT NULL default '0',
  `difficulty0percentchance` float default NULL,
  `difficulty1percentchance` float default NULL,
  `difficulty2percentchance` float NOT NULL default '0',
  `difficulty3percentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `entryid` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `creatureloot_gathering` */

DROP TABLE IF EXISTS `creatureloot_gathering`;

CREATE TABLE `creatureloot_gathering` (
  `entryid` int(10) unsigned NOT NULL default '0',
  `itemid` int(10) unsigned NOT NULL default '0',
  `difficulty0percentchance` float default NULL,
  `difficulty1percentchance` float default NULL,
  `difficulty2percentchance` float NOT NULL default '0',
  `difficulty3percentchance` float NOT NULL default '0',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `Unique` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Loot System';

/*Table structure for table `db_version` */

DROP TABLE IF EXISTS `db_version`;

CREATE TABLE `db_version` (
  `info` varchar(4) collate utf8_unicode_ci NOT NULL,
  `revision` int(4) unsigned NOT NULL default '0',
  `updater` varchar(32) collate utf8_unicode_ci NOT NULL,
  `patch_date` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`info`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `disenchantingloot` */

DROP TABLE IF EXISTS `disenchantingloot`;

CREATE TABLE `disenchantingloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float unsigned NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` tinyint(4) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `events` */

DROP TABLE IF EXISTS `events`;

CREATE TABLE `events` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'Id of event you want to trigger',
  `daynumber` tinyint(3) unsigned NOT NULL COMMENT '1 - 31 day of the month you want to trigger event',
  `monthnumber` tinyint(3) unsigned default '0' COMMENT '1 - 12 month of the year you want to trigger event',
  `eventname` varchar(255) collate utf8_unicode_ci default NULL COMMENT 'Just for human readable purposes, this is not used by the server at all',
  `activedays` tinyint(3) unsigned default NULL COMMENT 'Number of days the event will remain active',
  `ishourlyevent` tinyint(1) default NULL COMMENT 'Is this an hourly event? This cannot be used in conjunction with daily events',
  `starthour` tinyint(3) unsigned default NULL COMMENT '0 - 23 the hour that the event begins',
  `endhour` tinyint(3) unsigned default NULL COMMENT '0 - 23 the hour that the event ends, if this is smaller that the starthour, it will end on the next day',
  PRIMARY KEY  (`eventid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `events_creature` */

DROP TABLE IF EXISTS `events_creature`;

CREATE TABLE `events_creature` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'Relates to creature_spawn eventid',
  `id` tinyint(3) unsigned NOT NULL COMMENT 'Relates to creature_spawn id',
  `changesflag` tinyint(1) NOT NULL default '0' COMMENT 'See enum in the core for permissable flags in World.h',
  `phase` tinyint(3) default NULL COMMENT 'This is the phase the creature is in when the event is active',
  `displayid` int(10) unsigned default NULL COMMENT 'This is the displayid to change to while the event is active',
  `item1` int(10) unsigned default NULL COMMENT 'This is the id of equip slot 1 to change while the event is active',
  `item2` int(10) unsigned default NULL COMMENT 'This is the id of equip slot 2 to change while the event is active',
  `item3` int(10) unsigned default NULL COMMENT 'This is the id of equip slot 3 to change while the event is active',
  PRIMARY KEY  (`eventid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `events_gameobject` */

DROP TABLE IF EXISTS `events_gameobject`;

CREATE TABLE `events_gameobject` (
  `eventid` tinyint(3) unsigned NOT NULL COMMENT 'This relates to gameobject_spawns eventid',
  `id` int(11) unsigned NOT NULL COMMENT 'Relates to gameobject_spawn id',
  `changesflag` tinyint(1) NOT NULL default '0' COMMENT 'See enum in core for permissable flags in World.h',
  `phase` tinyint(3) default NULL COMMENT 'Phase the go is in when the event is active',
  `displayid` int(10) unsigned default NULL COMMENT 'This is the displayid to change to while the event is active',
  PRIMARY KEY  (`eventid`,`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `fishing` */

DROP TABLE IF EXISTS `fishing`;

CREATE TABLE `fishing` (
  `Zone` int(10) unsigned NOT NULL default '0',
  `MinSkill` int(10) unsigned default NULL,
  `MaxSkill` int(10) unsigned default NULL,
  PRIMARY KEY  (`Zone`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Fishing System';

/*Table structure for table `fishingloot` */

DROP TABLE IF EXISTS `fishingloot`;

CREATE TABLE `fishingloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '0',
  `percentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `Unique` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Loot System';

/*Table structure for table `gameobject_names` */

DROP TABLE IF EXISTS `gameobject_names`;

CREATE TABLE `gameobject_names` (
  `entry` int(10) unsigned NOT NULL default '0',
  `Type` int(10) unsigned NOT NULL default '0',
  `DisplayID` int(10) unsigned NOT NULL default '0',
  `Name` varchar(100) collate utf8_unicode_ci NOT NULL,
  `spellfocus` int(10) unsigned NOT NULL default '0',
  `sound1` int(10) unsigned NOT NULL default '0',
  `sound2` int(10) unsigned NOT NULL default '0',
  `sound3` int(10) unsigned NOT NULL default '0',
  `sound4` int(10) unsigned NOT NULL default '0',
  `sound5` int(10) unsigned NOT NULL default '0',
  `sound6` int(10) unsigned NOT NULL default '0',
  `sound7` int(10) unsigned NOT NULL default '0',
  `sound8` int(10) unsigned NOT NULL default '0',
  `sound9` int(10) unsigned NOT NULL default '0',
  `unknown1` int(10) unsigned NOT NULL default '0',
  `unknown2` int(10) unsigned NOT NULL default '0',
  `unknown3` int(10) unsigned NOT NULL default '0',
  `unknown4` int(10) unsigned NOT NULL default '0',
  `unknown5` int(10) unsigned NOT NULL default '0',
  `unknown6` int(10) unsigned NOT NULL default '0',
  `unknown7` int(10) unsigned NOT NULL default '0',
  `unknown8` int(10) unsigned NOT NULL default '0',
  `unknown9` int(10) unsigned NOT NULL default '0',
  `unknown10` int(10) unsigned NOT NULL default '0',
  `unknown11` int(10) unsigned NOT NULL default '0',
  `unknown12` int(10) unsigned NOT NULL default '0',
  `unknown13` int(10) unsigned NOT NULL default '0',
  `unknown14` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Gameobject System';

/*Table structure for table `gameobject_names_localized` */

DROP TABLE IF EXISTS `gameobject_names_localized`;

CREATE TABLE `gameobject_names_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) collate utf8_unicode_ci NOT NULL,
  `name` varchar(100) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `gameobject_quest_finisher` */

DROP TABLE IF EXISTS `gameobject_quest_finisher`;

CREATE TABLE `gameobject_quest_finisher` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Quest System';

/*Table structure for table `gameobject_quest_item_binding` */

DROP TABLE IF EXISTS `gameobject_quest_item_binding`;

CREATE TABLE `gameobject_quest_item_binding` (
  `entry` int(11) NOT NULL default '0',
  `quest` int(11) NOT NULL default '0',
  `item` int(11) NOT NULL default '0',
  `item_count` int(11) NOT NULL default '0',
  PRIMARY KEY  (`entry`,`quest`,`item`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Quest System';

/*Table structure for table `gameobject_quest_pickup_binding` */

DROP TABLE IF EXISTS `gameobject_quest_pickup_binding`;

CREATE TABLE `gameobject_quest_pickup_binding` (
  `entry` int(11) NOT NULL default '0',
  `quest` int(11) NOT NULL default '0',
  `required_count` int(11) NOT NULL default '0',
  PRIMARY KEY  (`entry`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Quest System';

/*Table structure for table `gameobject_quest_starter` */

DROP TABLE IF EXISTS `gameobject_quest_starter`;

CREATE TABLE `gameobject_quest_starter` (
  `id` int(11) unsigned NOT NULL default '0',
  `quest` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Quest System';

/*Table structure for table `gameobject_spawns` */

DROP TABLE IF EXISTS `gameobject_spawns`;

CREATE TABLE `gameobject_spawns` (
  `ID` int(30) unsigned NOT NULL auto_increment,
  `Entry` int(10) unsigned NOT NULL default '0',
  `Map` int(10) unsigned NOT NULL default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `Facing` float NOT NULL default '0',
  `orientation1` float NOT NULL default '0',
  `orientation2` float NOT NULL default '0',
  `orientation3` float NOT NULL default '0',
  `orientation4` float NOT NULL default '0',
  `State` int(10) unsigned NOT NULL default '0',
  `Flags` int(10) unsigned NOT NULL default '0',
  `Faction` int(10) unsigned NOT NULL default '0',
  `Scale` float NOT NULL default '0',
  `phase` int(11) NOT NULL default '1',
  `eventid` tinyint(3) unsigned NOT NULL default '0' COMMENT 'relates to events table',
  PRIMARY KEY  (`ID`),
  KEY `Map` (`Map`)
) ENGINE=MyISAM AUTO_INCREMENT=143340 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci PACK_KEYS=0 ROW_FORMAT=FIXED COMMENT='Spawn System';

/*Table structure for table `gameobject_staticspawns` */

DROP TABLE IF EXISTS `gameobject_staticspawns`;

CREATE TABLE `gameobject_staticspawns` (
  `id` int(30) NOT NULL auto_increment,
  `entry` int(30) NOT NULL,
  `map` int(11) NOT NULL default '0',
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `facing` float NOT NULL,
  `o` float NOT NULL,
  `o1` float NOT NULL,
  `o2` float NOT NULL,
  `o3` float NOT NULL,
  `state` int(11) NOT NULL default '0',
  `flags` int(30) NOT NULL default '0',
  `faction` int(11) NOT NULL default '0',
  `scale` float NOT NULL,
  `respawnNpcLink` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spawn System';

/*Table structure for table `graveyards` */

DROP TABLE IF EXISTS `graveyards`;

CREATE TABLE `graveyards` (
  `id` int(10) unsigned NOT NULL auto_increment,
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `orientation` float NOT NULL default '0',
  `zoneid` int(10) unsigned NOT NULL default '0',
  `adjacentzoneid` int(10) unsigned NOT NULL default '0',
  `mapid` int(10) unsigned NOT NULL default '0',
  `faction` int(10) unsigned NOT NULL default '0',
  `name` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1475 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Graveyard System';

/*Table structure for table `item_quest_association` */

DROP TABLE IF EXISTS `item_quest_association`;

CREATE TABLE `item_quest_association` (
  `item` int(11) NOT NULL default '0',
  `quest` int(11) NOT NULL default '0',
  `item_count` int(11) NOT NULL default '0',
  PRIMARY KEY  (`item`,`quest`),
  UNIQUE KEY `item` (`item`,`quest`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Quest System';

/*Table structure for table `item_randomprop_groups` */

DROP TABLE IF EXISTS `item_randomprop_groups`;

CREATE TABLE `item_randomprop_groups` (
  `entry_id` int(30) NOT NULL,
  `randomprops_entryid` int(30) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY  (`entry_id`,`randomprops_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Item System';

/*Table structure for table `item_randomsuffix_groups` */

DROP TABLE IF EXISTS `item_randomsuffix_groups`;

CREATE TABLE `item_randomsuffix_groups` (
  `entry_id` int(30) NOT NULL,
  `randomsuffix_entryid` int(30) NOT NULL,
  `chance` float NOT NULL,
  PRIMARY KEY  (`entry_id`,`randomsuffix_entryid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Item System';

/*Table structure for table `itemloot` */

DROP TABLE IF EXISTS `itemloot`;

CREATE TABLE `itemloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float unsigned NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `Unique` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Loot System';

/*Table structure for table `itempages` */

DROP TABLE IF EXISTS `itempages`;

CREATE TABLE `itempages` (
  `entry` int(10) unsigned NOT NULL default '0',
  `text` longtext collate utf8_unicode_ci NOT NULL,
  `next_page` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Item System';

/*Table structure for table `itempages_localized` */

DROP TABLE IF EXISTS `itempages_localized`;

CREATE TABLE `itempages_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) collate utf8_unicode_ci NOT NULL,
  `text` text collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `itempetfood` */

DROP TABLE IF EXISTS `itempetfood`;

CREATE TABLE `itempetfood` (
  `entry` int(11) NOT NULL,
  `food_type` int(11) NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `items` */

DROP TABLE IF EXISTS `items`;

CREATE TABLE `items` (
  `entry` int(30) unsigned NOT NULL default '0',
  `class` int(30) NOT NULL default '0',
  `subclass` int(30) NOT NULL default '0',
  `field4` int(10) NOT NULL default '0',
  `name1` varchar(255) collate utf8_unicode_ci NOT NULL,
  `name2` varchar(255) collate utf8_unicode_ci NOT NULL,
  `name3` varchar(255) collate utf8_unicode_ci NOT NULL,
  `name4` varchar(255) collate utf8_unicode_ci NOT NULL,
  `displayid` int(30) unsigned NOT NULL default '0',
  `quality` int(30) NOT NULL default '0',
  `flags` int(30) NOT NULL default '0',
  `buyprice` int(30) NOT NULL default '0',
  `sellprice` int(30) NOT NULL default '0',
  `inventorytype` int(30) NOT NULL default '0',
  `allowableclass` int(30) NOT NULL default '0',
  `allowablerace` int(30) NOT NULL default '0',
  `itemlevel` int(30) NOT NULL default '0',
  `requiredlevel` int(30) NOT NULL default '0',
  `RequiredSkill` int(30) NOT NULL default '0',
  `RequiredSkillRank` int(30) NOT NULL default '0',
  `RequiredSkillSubRank` int(30) NOT NULL default '0',
  `RequiredPlayerRank1` int(30) NOT NULL default '0',
  `RequiredPlayerRank2` int(30) NOT NULL default '0',
  `RequiredFaction` int(30) NOT NULL default '0',
  `RequiredFactionStanding` int(30) NOT NULL default '0',
  `Unique` int(30) NOT NULL default '0',
  `maxcount` int(30) NOT NULL default '0',
  `ContainerSlots` int(30) NOT NULL default '0',
  `stat_type1` int(30) NOT NULL default '0',
  `stat_value1` int(30) NOT NULL default '0',
  `stat_type2` int(30) NOT NULL default '0',
  `stat_value2` int(30) NOT NULL default '0',
  `stat_type3` int(30) NOT NULL default '0',
  `stat_value3` int(30) NOT NULL default '0',
  `stat_type4` int(30) NOT NULL default '0',
  `stat_value4` int(30) NOT NULL default '0',
  `stat_type5` int(30) NOT NULL default '0',
  `stat_value5` int(30) NOT NULL default '0',
  `stat_type6` int(30) NOT NULL default '0',
  `stat_value6` int(30) NOT NULL default '0',
  `stat_type7` int(30) NOT NULL default '0',
  `stat_value7` int(30) NOT NULL default '0',
  `stat_type8` int(30) NOT NULL default '0',
  `stat_value8` int(30) NOT NULL default '0',
  `stat_type9` int(30) NOT NULL default '0',
  `stat_value9` int(30) NOT NULL default '0',
  `stat_type10` int(30) NOT NULL default '0',
  `stat_value10` int(30) NOT NULL default '0',
  `dmg_min1` float NOT NULL default '0',
  `dmg_max1` float NOT NULL default '0',
  `dmg_type1` int(30) NOT NULL default '0',
  `dmg_min2` float NOT NULL default '0',
  `dmg_max2` float NOT NULL default '0',
  `dmg_type2` int(30) NOT NULL default '0',
  `armor` int(30) NOT NULL default '0',
  `holy_res` int(30) NOT NULL default '0',
  `fire_res` int(30) NOT NULL default '0',
  `nature_res` int(30) NOT NULL default '0',
  `frost_res` int(30) NOT NULL default '0',
  `shadow_res` int(30) NOT NULL default '0',
  `arcane_res` int(30) NOT NULL default '0',
  `delay` int(30) NOT NULL default '0',
  `ammo_type` int(30) NOT NULL default '0',
  `range` float NOT NULL default '0',
  `spellid_1` int(30) NOT NULL default '0',
  `spelltrigger_1` int(30) NOT NULL default '0',
  `spellcharges_1` int(30) NOT NULL default '0',
  `spellcooldown_1` int(30) NOT NULL default '0',
  `spellcategory_1` int(30) NOT NULL default '0',
  `spellcategorycooldown_1` int(30) NOT NULL default '0',
  `spellid_2` int(30) NOT NULL default '0',
  `spelltrigger_2` int(30) NOT NULL default '0',
  `spellcharges_2` int(30) NOT NULL default '0',
  `spellcooldown_2` int(30) NOT NULL default '0',
  `spellcategory_2` int(30) NOT NULL default '0',
  `spellcategorycooldown_2` int(30) NOT NULL default '0',
  `spellid_3` int(30) NOT NULL default '0',
  `spelltrigger_3` int(30) NOT NULL default '0',
  `spellcharges_3` int(30) NOT NULL default '0',
  `spellcooldown_3` int(30) NOT NULL default '0',
  `spellcategory_3` int(30) NOT NULL default '0',
  `spellcategorycooldown_3` int(30) NOT NULL default '0',
  `spellid_4` int(30) NOT NULL default '0',
  `spelltrigger_4` int(30) NOT NULL default '0',
  `spellcharges_4` int(30) NOT NULL default '0',
  `spellcooldown_4` int(30) NOT NULL default '0',
  `spellcategory_4` int(30) NOT NULL default '0',
  `spellcategorycooldown_4` int(30) NOT NULL default '0',
  `spellid_5` int(30) NOT NULL default '0',
  `spelltrigger_5` int(30) NOT NULL default '0',
  `spellcharges_5` int(30) NOT NULL default '0',
  `spellcooldown_5` int(30) NOT NULL default '0',
  `spellcategory_5` int(30) NOT NULL default '0',
  `spellcategorycooldown_5` int(30) NOT NULL default '0',
  `bonding` int(30) NOT NULL default '0',
  `description` varchar(255) collate utf8_unicode_ci NOT NULL default '',
  `page_id` int(30) NOT NULL default '0',
  `page_language` int(30) NOT NULL default '0',
  `page_material` int(30) NOT NULL default '0',
  `quest_id` int(30) NOT NULL default '0',
  `lock_id` int(30) NOT NULL default '0',
  `lock_material` int(30) NOT NULL default '0',
  `sheathID` int(30) NOT NULL default '0',
  `randomprop` int(30) NOT NULL default '0',
  `RandomSuffixId` int(11) NOT NULL default '0',
  `block` int(30) NOT NULL default '0',
  `itemset` int(30) NOT NULL default '0',
  `MaxDurability` int(30) NOT NULL default '0',
  `ZoneNameID` int(30) NOT NULL default '0',
  `mapid` int(30) NOT NULL default '0',
  `bagfamily` int(30) NOT NULL default '0',
  `TotemCategory` int(30) NOT NULL default '0',
  `socket_color_1` int(30) NOT NULL default '0',
  `unk201_3` int(30) NOT NULL default '0',
  `socket_color_2` int(30) NOT NULL default '0',
  `unk201_5` int(30) NOT NULL default '0',
  `socket_color_3` int(30) NOT NULL default '0',
  `unk201_7` int(30) NOT NULL default '0',
  `socket_bonus` int(30) NOT NULL default '0',
  `GemProperties` int(30) NOT NULL default '0',
  `ReqDisenchantSkill` int(30) NOT NULL default '-1',
  `Lootgold` int(30) NOT NULL default '0',
  `ArmorDamageModifier` int(30) NOT NULL default '0',
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Item System';

/*Table structure for table `items_localized` */

DROP TABLE IF EXISTS `items_localized`;

CREATE TABLE `items_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) character set utf8 collate utf8_unicode_ci NOT NULL,
  `name` varchar(255) character set utf8 collate utf8_unicode_ci NOT NULL,
  `description` varchar(255) character set utf8 collate utf8_unicode_ci NOT NULL,
  `last_update` int(11) unsigned NOT NULL default '0',
  UNIQUE KEY `entry` (`entry`,`language_code`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `map_checkpoint` */

DROP TABLE IF EXISTS `map_checkpoint`;

CREATE TABLE `map_checkpoint` (
  `entry` int(30) NOT NULL,
  `prereq_checkpoint_id` int(30) NOT NULL,
  `creature_id` int(30) NOT NULL,
  `name` varchar(255) collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Map System';

/*Table structure for table `millingloot` */

DROP TABLE IF EXISTS `millingloot`;

CREATE TABLE `millingloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '0',
  `percentchance` float unsigned NOT NULL default '0',
  `mincount` int(30) unsigned NOT NULL default '1',
  `maxcount` int(30) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `Unique` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `news_announcements` */

DROP TABLE IF EXISTS `news_announcements`;

CREATE TABLE `news_announcements` (
  `id` int(30) unsigned NOT NULL auto_increment,
  `faction_mask` int(30) NOT NULL,
  `lastTime` int(30) NOT NULL,
  `message_text` varchar(500) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `npc_gossip_textid` */

DROP TABLE IF EXISTS `npc_gossip_textid`;

CREATE TABLE `npc_gossip_textid` (
  `creatureid` int(10) unsigned NOT NULL default '0',
  `textid` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`creatureid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='NPC System';

/*Table structure for table `npc_monstersay` */

DROP TABLE IF EXISTS `npc_monstersay`;

CREATE TABLE `npc_monstersay` (
  `entry` int(10) unsigned NOT NULL default '0',
  `event` int(10) unsigned NOT NULL default '0',
  `chance` float NOT NULL default '0',
  `language` int(10) unsigned NOT NULL default '0',
  `type` int(10) unsigned NOT NULL default '0',
  `monstername` longtext character set utf8 collate utf8_unicode_ci,
  `text0` longtext character set utf8 collate utf8_unicode_ci,
  `text1` longtext character set utf8 collate utf8_unicode_ci,
  `text2` longtext character set utf8 collate utf8_unicode_ci,
  `text3` longtext character set utf8 collate utf8_unicode_ci,
  `text4` longtext character set utf8 collate utf8_unicode_ci,
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='NPC System';

/*Table structure for table `npc_monstersay_localized` */

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

/*Table structure for table `npc_text` */

DROP TABLE IF EXISTS `npc_text`;

CREATE TABLE `npc_text` (
  `entry` int(10) unsigned NOT NULL default '0',
  `prob0` float NOT NULL default '0',
  `text0_0` longtext collate utf8_unicode_ci NOT NULL,
  `text0_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang0` int(10) unsigned NOT NULL default '0',
  `em0_0` int(10) unsigned NOT NULL default '0',
  `em0_1` int(10) unsigned NOT NULL default '0',
  `em0_2` int(10) unsigned NOT NULL default '0',
  `em0_3` int(10) unsigned NOT NULL default '0',
  `em0_4` int(10) unsigned NOT NULL default '0',
  `em0_5` int(10) unsigned NOT NULL default '0',
  `prob1` float NOT NULL default '0',
  `text1_0` longtext collate utf8_unicode_ci NOT NULL,
  `text1_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang1` int(10) unsigned NOT NULL default '0',
  `em1_0` int(10) unsigned NOT NULL default '0',
  `em1_1` int(10) unsigned NOT NULL default '0',
  `em1_2` int(10) unsigned NOT NULL default '0',
  `em1_3` int(10) unsigned NOT NULL default '0',
  `em1_4` int(10) unsigned NOT NULL default '0',
  `em1_5` int(10) unsigned NOT NULL default '0',
  `prob2` float NOT NULL default '0',
  `text2_0` longtext collate utf8_unicode_ci NOT NULL,
  `text2_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang2` int(10) unsigned NOT NULL default '0',
  `em2_0` int(10) unsigned NOT NULL default '0',
  `em2_1` int(10) unsigned NOT NULL default '0',
  `em2_2` int(10) unsigned NOT NULL default '0',
  `em2_3` int(10) unsigned NOT NULL default '0',
  `em2_4` int(10) unsigned NOT NULL default '0',
  `em2_5` int(10) unsigned NOT NULL default '0',
  `prob3` float NOT NULL default '0',
  `text3_0` longtext collate utf8_unicode_ci NOT NULL,
  `text3_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang3` int(10) unsigned NOT NULL default '0',
  `em3_0` int(10) unsigned NOT NULL default '0',
  `em3_1` int(10) unsigned NOT NULL default '0',
  `em3_2` int(10) unsigned NOT NULL default '0',
  `em3_3` int(10) unsigned NOT NULL default '0',
  `em3_4` int(10) unsigned NOT NULL default '0',
  `em3_5` int(10) unsigned NOT NULL default '0',
  `prob4` float NOT NULL default '0',
  `text4_0` longtext collate utf8_unicode_ci NOT NULL,
  `text4_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang4` int(10) unsigned NOT NULL default '0',
  `em4_0` int(10) unsigned NOT NULL default '0',
  `em4_1` int(10) unsigned NOT NULL default '0',
  `em4_2` int(10) unsigned NOT NULL default '0',
  `em4_3` int(10) unsigned NOT NULL default '0',
  `em4_4` int(10) unsigned NOT NULL default '0',
  `em4_5` int(10) unsigned NOT NULL default '0',
  `prob5` float NOT NULL default '0',
  `text5_0` longtext collate utf8_unicode_ci NOT NULL,
  `text5_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang5` int(10) unsigned NOT NULL default '0',
  `em5_0` int(10) unsigned NOT NULL default '0',
  `em5_1` int(10) unsigned NOT NULL default '0',
  `em5_2` int(10) unsigned NOT NULL default '0',
  `em5_3` int(10) unsigned NOT NULL default '0',
  `em5_4` int(10) unsigned NOT NULL default '0',
  `em5_5` int(10) unsigned NOT NULL default '0',
  `prob6` float NOT NULL default '0',
  `text6_0` longtext collate utf8_unicode_ci NOT NULL,
  `text6_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang6` int(10) unsigned NOT NULL default '0',
  `em6_0` int(10) unsigned NOT NULL default '0',
  `em6_1` int(10) unsigned NOT NULL default '0',
  `em6_2` int(10) unsigned NOT NULL default '0',
  `em6_3` int(10) unsigned NOT NULL default '0',
  `em6_4` int(10) unsigned NOT NULL default '0',
  `em6_5` int(10) unsigned NOT NULL default '0',
  `prob7` float NOT NULL default '0',
  `text7_0` longtext collate utf8_unicode_ci NOT NULL,
  `text7_1` longtext collate utf8_unicode_ci NOT NULL,
  `lang7` int(10) unsigned NOT NULL default '0',
  `em7_0` int(10) unsigned NOT NULL default '0',
  `em7_1` int(10) unsigned NOT NULL default '0',
  `em7_2` int(10) unsigned NOT NULL default '0',
  `em7_3` int(10) unsigned NOT NULL default '0',
  `em7_4` int(10) unsigned NOT NULL default '0',
  `em7_5` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='NPC System';

/*Table structure for table `npc_text_localized` */

DROP TABLE IF EXISTS `npc_text_localized`;

CREATE TABLE `npc_text_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(4) collate utf8_unicode_ci NOT NULL,
  `text0` text collate utf8_unicode_ci NOT NULL,
  `text0_1` text collate utf8_unicode_ci NOT NULL,
  `text1` text collate utf8_unicode_ci NOT NULL,
  `text1_1` text collate utf8_unicode_ci NOT NULL,
  `text2` text collate utf8_unicode_ci NOT NULL,
  `text2_1` text collate utf8_unicode_ci NOT NULL,
  `text3` text collate utf8_unicode_ci NOT NULL,
  `text3_1` text collate utf8_unicode_ci NOT NULL,
  `text4` text collate utf8_unicode_ci NOT NULL,
  `text4_1` text collate utf8_unicode_ci NOT NULL,
  `text5` text collate utf8_unicode_ci NOT NULL,
  `text5_1` text collate utf8_unicode_ci NOT NULL,
  `text6` text collate utf8_unicode_ci NOT NULL,
  `text6_1` text collate utf8_unicode_ci NOT NULL,
  `text7` text collate utf8_unicode_ci NOT NULL,
  `text7_1` text collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `objectloot` */

DROP TABLE IF EXISTS `objectloot`;

CREATE TABLE `objectloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '0',
  `difficulty0percentchance` float default NULL,
  `difficulty1percentchance` float default NULL,
  `difficulty2percentchance` float NOT NULL default '0',
  `difficulty3percentchance` float NOT NULL default '0',
  `mincount` int(11) unsigned NOT NULL default '1',
  `maxcount` int(11) unsigned NOT NULL default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `Unique` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Loot System';

/*Table structure for table `pickpocketingloot` */

DROP TABLE IF EXISTS `pickpocketingloot`;

CREATE TABLE `pickpocketingloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '100',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Loot System';

/*Table structure for table `playercreateinfo` */

DROP TABLE IF EXISTS `playercreateinfo`;

CREATE TABLE `playercreateinfo` (
  `Index` tinyint(3) unsigned NOT NULL auto_increment,
  `race` tinyint(3) unsigned NOT NULL default '0',
  `factiontemplate` int(10) unsigned NOT NULL default '0',
  `class` tinyint(3) unsigned NOT NULL default '0',
  `mapID` int(10) unsigned NOT NULL default '0',
  `zoneID` int(10) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  `displayID` smallint(5) unsigned NOT NULL default '0',
  `BaseStrength` tinyint(3) unsigned NOT NULL default '0',
  `BaseAgility` tinyint(3) unsigned NOT NULL default '0',
  `BaseStamina` tinyint(3) unsigned NOT NULL default '0',
  `BaseIntellect` tinyint(3) unsigned NOT NULL default '0',
  `BaseSpirit` tinyint(3) unsigned NOT NULL default '0',
  `BaseHealth` int(10) unsigned NOT NULL default '0',
  `BaseMana` int(10) unsigned NOT NULL default '0',
  `BaseRage` int(10) unsigned NOT NULL default '0',
  `BaseFocus` int(10) unsigned NOT NULL default '0',
  `BaseEnergy` int(10) unsigned NOT NULL default '0',
  `BaseRunic` int(10) unsigned NOT NULL default '0',
  `attackpower` int(10) unsigned NOT NULL default '0',
  `mindmg` float NOT NULL default '0',
  `maxdmg` float NOT NULL default '0',
  PRIMARY KEY  (`Index`)
) ENGINE=MyISAM AUTO_INCREMENT=66 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Player System';

/*Table structure for table `playercreateinfo_bars` */

DROP TABLE IF EXISTS `playercreateinfo_bars`;

CREATE TABLE `playercreateinfo_bars` (
  `race` tinyint(3) unsigned default NULL,
  `class` tinyint(3) unsigned default NULL,
  `button` int(10) unsigned default NULL,
  `action` int(10) unsigned default NULL,
  `type` int(10) unsigned default NULL,
  `misc` int(10) unsigned default NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Player System';

/*Table structure for table `playercreateinfo_items` */

DROP TABLE IF EXISTS `playercreateinfo_items`;

CREATE TABLE `playercreateinfo_items` (
  `indexid` tinyint(3) unsigned NOT NULL default '0',
  `protoid` int(10) unsigned NOT NULL default '0',
  `slotid` tinyint(3) unsigned NOT NULL default '0',
  `amount` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Player System';

/*Table structure for table `playercreateinfo_skills` */

DROP TABLE IF EXISTS `playercreateinfo_skills`;

CREATE TABLE `playercreateinfo_skills` (
  `indexid` tinyint(3) unsigned NOT NULL default '0',
  `skillid` int(10) unsigned NOT NULL default '0',
  `level` int(10) unsigned NOT NULL default '0',
  `maxlevel` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Player System';

/*Table structure for table `playercreateinfo_spells` */

DROP TABLE IF EXISTS `playercreateinfo_spells`;

CREATE TABLE `playercreateinfo_spells` (
  `indexid` tinyint(3) unsigned NOT NULL default '0',
  `spellid` smallint(5) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Player System';

/*Table structure for table `prestartqueries` */

DROP TABLE IF EXISTS `prestartqueries`;

CREATE TABLE `prestartqueries` (
  `Query` varchar(1024) NOT NULL,
  `SingleShot` int(1) unsigned NOT NULL default '1',
  `Seq` int(11) NOT NULL auto_increment,
  PRIMARY KEY  (`Seq`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `professiondiscoveries` */

DROP TABLE IF EXISTS `professiondiscoveries`;

CREATE TABLE `professiondiscoveries` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `SpellToDiscover` int(10) unsigned NOT NULL default '0',
  `SkillValue` int(10) unsigned NOT NULL default '0',
  `Chance` float NOT NULL default '0',
  PRIMARY KEY  (`SpellId`,`SpellToDiscover`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `prospectingloot` */

DROP TABLE IF EXISTS `prospectingloot`;

CREATE TABLE `prospectingloot` (
  `entryid` int(11) unsigned NOT NULL default '0',
  `itemid` int(11) unsigned NOT NULL default '25',
  `percentchance` float NOT NULL default '0',
  `mincount` int(30) default '1',
  `maxcount` int(30) default '1',
  `ffa_loot` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`entryid`,`itemid`),
  UNIQUE KEY `Unique` (`entryid`,`itemid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `quests` */

DROP TABLE IF EXISTS `quests`;

CREATE TABLE `quests` (
  `entry` int(10) unsigned NOT NULL default '0',
  `ZoneId` int(10) unsigned NOT NULL default '0',
  `sort` int(10) unsigned NOT NULL default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `MinLevel` int(10) unsigned NOT NULL default '0',
  `questlevel` int(10) NOT NULL default '0',
  `Type` int(10) unsigned NOT NULL default '0',
  `RequiredRaces` int(10) unsigned NOT NULL default '0',
  `RequiredClass` int(10) unsigned NOT NULL default '0',
  `RequiredTradeskill` int(10) unsigned NOT NULL default '0',
  `RequiredTradeskillValue` int(5) unsigned NOT NULL default '0',
  `RequiredRepFaction` int(10) unsigned NOT NULL default '0',
  `RequiredRepValue` int(10) unsigned NOT NULL default '0',
  `SuggestedPlayers` int(11) NOT NULL default '0',
  `LimitTime` int(10) unsigned NOT NULL default '0',
  `SpecialFlags` int(10) unsigned NOT NULL default '0',
  `PrevQuestId` int(10) unsigned NOT NULL default '0',
  `NextQuestId` int(10) unsigned NOT NULL default '0',
  `srcItem` int(10) unsigned NOT NULL default '0',
  `SrcItemCount` int(10) unsigned NOT NULL default '0',
  `Title` char(255) collate utf8_unicode_ci NOT NULL,
  `Details` text collate utf8_unicode_ci NOT NULL,
  `Objectives` text collate utf8_unicode_ci NOT NULL,
  `CompletionText` text collate utf8_unicode_ci NOT NULL,
  `IncompleteText` text collate utf8_unicode_ci NOT NULL,
  `EndText` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText1` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText2` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText3` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText4` text collate utf8_unicode_ci NOT NULL,
  `ReqItemId1` int(10) unsigned NOT NULL default '0',
  `ReqItemId2` int(10) unsigned NOT NULL default '0',
  `ReqItemId3` int(10) unsigned NOT NULL default '0',
  `ReqItemId4` int(10) unsigned NOT NULL default '0',
  `ReqItemId5` int(10) NOT NULL default '0',
  `ReqItemId6` int(10) NOT NULL default '0',
  `ReqItemCount1` int(10) unsigned NOT NULL default '0',
  `ReqItemCount2` int(10) unsigned NOT NULL default '0',
  `ReqItemCount3` int(10) unsigned NOT NULL default '0',
  `ReqItemCount4` int(10) unsigned NOT NULL default '0',
  `ReqItemCount5` int(10) NOT NULL default '0',
  `ReqItemCount6` int(10) NOT NULL default '0',
  `ReqKillPlayer` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId1` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId2` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId3` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOId4` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount1` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount2` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount3` int(10) unsigned NOT NULL default '0',
  `ReqKillMobOrGOCount4` int(10) unsigned NOT NULL default '0',
  `ReqCastSpellId1` int(11) NOT NULL default '0',
  `ReqCastSpellId2` int(11) NOT NULL default '0',
  `ReqCastSpellId3` int(11) NOT NULL default '0',
  `ReqCastSpellId4` int(11) NOT NULL default '0',
  `RewChoiceItemId1` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId2` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId3` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId4` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId5` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemId6` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount1` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount2` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount3` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount4` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount5` int(10) unsigned NOT NULL default '0',
  `RewChoiceItemCount6` int(10) unsigned NOT NULL default '0',
  `RewItemId1` int(10) unsigned NOT NULL default '0',
  `RewItemId2` int(10) unsigned NOT NULL default '0',
  `RewItemId3` int(10) unsigned NOT NULL default '0',
  `RewItemId4` int(10) unsigned NOT NULL default '0',
  `RewItemCount1` int(10) unsigned NOT NULL default '0',
  `RewItemCount2` int(10) unsigned NOT NULL default '0',
  `RewItemCount3` int(10) unsigned NOT NULL default '0',
  `RewItemCount4` int(10) unsigned NOT NULL default '0',
  `RewRepFaction1` int(10) unsigned NOT NULL default '0',
  `RewRepFaction2` int(10) unsigned NOT NULL default '0',
  `RewRepFaction3` int(11) NOT NULL default '0',
  `RewRepFaction4` int(11) NOT NULL default '0',
  `RewRepFaction5` int(11) NOT NULL default '0',
  `RewRepValue1` int(10) NOT NULL default '0',
  `RewRepValue2` int(10) NOT NULL default '0',
  `RewRepValue3` int(11) NOT NULL default '0',
  `RewRepValue4` int(11) NOT NULL default '0',
  `RewRepValue5` int(11) NOT NULL default '0',
  `RewRepLimit` int(10) unsigned NOT NULL default '0',
  `RewTitle` int(30) NOT NULL default '0',
  `RewMoney` int(10) unsigned NOT NULL default '0',
  `RewBonusHonor` int(10) unsigned NOT NULL default '0',
  `RewXP` int(10) unsigned NOT NULL default '0',
  `RewSpell` int(10) unsigned NOT NULL default '0',
  `RewTalentPoints` int(10) unsigned NOT NULL default '0',
  `CastSpell` int(10) unsigned NOT NULL default '0',
  `PointMapId` int(10) unsigned NOT NULL default '0',
  `PointX` float NOT NULL default '0',
  `PointY` float NOT NULL default '0',
  `PointOpt` int(10) unsigned NOT NULL default '0',
  `RequiredMoney` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger1` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger2` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger3` int(10) unsigned NOT NULL default '0',
  `ExploreTrigger4` int(10) unsigned NOT NULL default '0',
  `RequiredQuest1` int(10) unsigned NOT NULL default '0',
  `RequiredQuest2` int(10) unsigned NOT NULL default '0',
  `RequiredQuest3` int(10) unsigned NOT NULL default '0',
  `RequiredQuest4` int(10) unsigned NOT NULL default '0',
  `RequiredQuest_and_or` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId1` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId2` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId3` int(10) unsigned NOT NULL default '0',
  `ReceiveItemId4` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount1` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount2` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount3` int(10) unsigned NOT NULL default '0',
  `ReceiveItemCount4` int(10) unsigned NOT NULL default '0',
  `IsRepeatable` int(11) NOT NULL default '0',
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `entry` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Quests System';

/*Table structure for table `quests_localized` */

DROP TABLE IF EXISTS `quests_localized`;

CREATE TABLE `quests_localized` (
  `entry` int(30) NOT NULL,
  `language_code` varchar(5) collate utf8_unicode_ci NOT NULL,
  `Title` text collate utf8_unicode_ci NOT NULL,
  `Details` text collate utf8_unicode_ci NOT NULL,
  `Objectives` text collate utf8_unicode_ci NOT NULL,
  `CompletionText` text collate utf8_unicode_ci NOT NULL,
  `IncompleteText` text collate utf8_unicode_ci NOT NULL,
  `EndText` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText1` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText2` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText3` text collate utf8_unicode_ci NOT NULL,
  `ObjectiveText4` text collate utf8_unicode_ci NOT NULL,
  PRIMARY KEY  (`entry`,`language_code`),
  KEY `lol` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `randomcardcreation` */

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

/*Table structure for table `randomitemcreation` */

DROP TABLE IF EXISTS `randomitemcreation`;

CREATE TABLE `randomitemcreation` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `ItemToCreate` int(10) unsigned NOT NULL default '0',
  `Skill` int(10) unsigned NOT NULL default '0',
  `Chance` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ItemToCreate`,`SpellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `recall` */

DROP TABLE IF EXISTS `recall`;

CREATE TABLE `recall` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `name` varchar(100) NOT NULL,
  `MapId` int(10) unsigned NOT NULL default '0',
  `positionX` float NOT NULL default '0',
  `positionY` float NOT NULL default '0',
  `positionZ` float NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7557 DEFAULT CHARSET=utf8 COMMENT='Tele Command';

/*Table structure for table `reputation_creature_onkill` */

DROP TABLE IF EXISTS `reputation_creature_onkill`;

CREATE TABLE `reputation_creature_onkill` (
  `creature_id` int(30) NOT NULL,
  `faction_change_alliance` int(30) NOT NULL,
  `faction_change_horde` int(30) NOT NULL,
  `change_value` int(30) NOT NULL,
  `rep_limit` int(30) NOT NULL,
  PRIMARY KEY  (`creature_id`,`faction_change_horde`,`faction_change_alliance`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Creature System';

/*Table structure for table `reputation_faction_onkill` */

DROP TABLE IF EXISTS `reputation_faction_onkill`;

CREATE TABLE `reputation_faction_onkill` (
  `faction_id` int(30) NOT NULL,
  `change_factionid_alliance` int(30) NOT NULL,
  `change_deltamin_alliance` int(30) NOT NULL,
  `change_deltamax_alliance` int(30) NOT NULL,
  `change_factionid_horde` int(30) NOT NULL,
  `change_deltamin_horde` int(30) NOT NULL,
  `change_deltamax_horde` int(30) NOT NULL,
  PRIMARY KEY  (`faction_id`,`change_factionid_horde`,`change_factionid_alliance`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Creature System';

/*Table structure for table `reputation_instance_onkill` */

DROP TABLE IF EXISTS `reputation_instance_onkill`;

CREATE TABLE `reputation_instance_onkill` (
  `mapid` int(30) NOT NULL,
  `mob_rep_reward` int(30) NOT NULL,
  `mob_rep_reward_heroic` int(30) NOT NULL,
  `mob_rep_limit` int(30) NOT NULL,
  `mob_rep_limit_heroic` int(30) NOT NULL,
  `boss_rep_reward` int(30) NOT NULL,
  `boss_rep_reward_heroic` int(30) NOT NULL,
  `boss_rep_limit` int(30) NOT NULL,
  `boss_rep_limit_heroic` int(30) NOT NULL,
  `faction_change_alliance` int(30) NOT NULL,
  `faction_change_horde` int(30) NOT NULL,
  PRIMARY KEY  (`mapid`,`faction_change_horde`,`faction_change_alliance`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Creature System';

/*Table structure for table `scrollcreation` */

DROP TABLE IF EXISTS `scrollcreation`;

CREATE TABLE `scrollcreation` (
  `SpellId` int(10) unsigned NOT NULL default '0',
  `ItemId` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`SpellId`,`ItemId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `spell_coef_override` */

DROP TABLE IF EXISTS `spell_coef_override`;

CREATE TABLE `spell_coef_override` (
  `id` int(11) default '0',
  `Dspell_coef_override` float default '0',
  `OTspell_coef_override` float default '0',
  `AP_coef_override` float default '0',
  `RAP_coef_override` float default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

/*Table structure for table `spell_disable` */

DROP TABLE IF EXISTS `spell_disable`;

CREATE TABLE `spell_disable` (
  `spellid` int(30) NOT NULL,
  `replacement_spellid` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Spell System';

/*Table structure for table `spell_disable_trainers` */

DROP TABLE IF EXISTS `spell_disable_trainers`;

CREATE TABLE `spell_disable_trainers` (
  `spellid` int(30) NOT NULL,
  `replacement_spellid` int(30) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Trainer System';

/*Table structure for table `spell_forced_targets` */

DROP TABLE IF EXISTS `spell_forced_targets`;

CREATE TABLE `spell_forced_targets` (
  `spellid` int(8) unsigned NOT NULL,
  `target` int(8) unsigned NOT NULL default '0',
  UNIQUE KEY `entry_type_target` (`spellid`,`target`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Spell System';

/*Table structure for table `spellfixes` */

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

/*Table structure for table `spelloverride` */

DROP TABLE IF EXISTS `spelloverride`;

CREATE TABLE `spelloverride` (
  `overrideId` int(10) unsigned NOT NULL default '0',
  `spellId` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `overrideId` (`overrideId`,`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Spell System';

/*Table structure for table `teleport_coords` */

DROP TABLE IF EXISTS `teleport_coords`;

CREATE TABLE `teleport_coords` (
  `id` int(10) unsigned NOT NULL default '0',
  `name` char(255) collate utf8_unicode_ci NOT NULL,
  `mapId` int(10) unsigned NOT NULL default '0',
  `position_x` float NOT NULL default '0',
  `position_y` float NOT NULL default '0',
  `position_z` float NOT NULL default '0',
  `orientation` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='World System';

/*Table structure for table `totemspells` */

DROP TABLE IF EXISTS `totemspells`;

CREATE TABLE `totemspells` (
  `spell` int(10) unsigned NOT NULL default '0',
  `castspell1` int(10) unsigned NOT NULL default '0',
  `castspell2` int(10) unsigned NOT NULL default '0',
  `castspell3` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`spell`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Spell System';

/*Table structure for table `trainer_defs` */

DROP TABLE IF EXISTS `trainer_defs`;

CREATE TABLE `trainer_defs` (
  `entry` int(11) unsigned NOT NULL default '0',
  `required_skill` int(11) unsigned NOT NULL default '0',
  `required_skillvalue` int(11) unsigned default '0',
  `req_class` int(11) unsigned NOT NULL default '0',
  `trainer_type` int(11) unsigned NOT NULL default '0',
  `trainer_ui_window_message` text,
  `can_train_gossip_textid` int(11) NOT NULL,
  `cannot_train_gossip_textid` int(11) NOT NULL,
  PRIMARY KEY  (`entry`),
  UNIQUE KEY `entry` (`entry`),
  KEY `entry_2` (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Trainer System';

/*Table structure for table `trainer_spells` */

DROP TABLE IF EXISTS `trainer_spells`;

CREATE TABLE `trainer_spells` (
  `entry` int(11) unsigned NOT NULL default '0',
  `cast_spell` int(11) unsigned NOT NULL default '0',
  `learn_spell` int(11) unsigned NOT NULL,
  `spellcost` int(11) unsigned NOT NULL default '0',
  `reqspell` int(11) unsigned NOT NULL default '0',
  `reqskill` int(11) unsigned NOT NULL default '0',
  `reqskillvalue` int(11) unsigned NOT NULL default '0',
  `reqlevel` int(11) unsigned NOT NULL default '0',
  `deletespell` int(11) unsigned NOT NULL default '0',
  `is_prof` tinyint(1) unsigned NOT NULL default '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `trainerspelloverride` */

DROP TABLE IF EXISTS `trainerspelloverride`;

CREATE TABLE `trainerspelloverride` (
  `spellid` int(10) unsigned NOT NULL default '0',
  `cost` int(10) unsigned NOT NULL default '0',
  `requiredspell` int(10) unsigned NOT NULL default '0',
  `deletespell` int(10) unsigned NOT NULL default '0',
  `requiredskill` int(10) unsigned NOT NULL default '0',
  `requiredskillvalue` int(10) unsigned NOT NULL default '0',
  `reqlevel` int(10) unsigned NOT NULL default '0',
  `requiredclass` int(10) unsigned NOT NULL default '0',
  UNIQUE KEY `spellid` (`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Trainer System';

/*Table structure for table `transport_creatures` */

DROP TABLE IF EXISTS `transport_creatures`;

CREATE TABLE `transport_creatures` (
  `transport_entry` int(10) unsigned NOT NULL,
  `creature_entry` int(10) unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `vendors` */

DROP TABLE IF EXISTS `vendors`;

CREATE TABLE `vendors` (
  `entry` int(10) unsigned NOT NULL default '0',
  `item` int(10) unsigned NOT NULL default '0',
  `amount` int(11) NOT NULL default '0',
  `max_amount` int(11) NOT NULL default '0',
  `inctime` bigint(20) NOT NULL default '0',
  `extendedcost` int(10) unsigned NOT NULL,
  UNIQUE KEY `Unique` (`entry`,`item`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='NPC System';

/*Table structure for table `weather` */

DROP TABLE IF EXISTS `weather`;

CREATE TABLE `weather` (
  `zoneId` int(11) unsigned NOT NULL default '0',
  `high_chance` int(11) unsigned NOT NULL default '0',
  `high_type` int(11) unsigned NOT NULL default '0',
  `med_chance` int(11) unsigned NOT NULL default '0',
  `med_type` int(11) unsigned NOT NULL default '0',
  `low_chance` int(11) unsigned NOT NULL default '0',
  `low_type` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`zoneId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Weather System';

/*Table structure for table `wordfilter_character_names` */

DROP TABLE IF EXISTS `wordfilter_character_names`;

CREATE TABLE `wordfilter_character_names` (
  `regex_match` varchar(500) collate utf8_unicode_ci NOT NULL,
  `regex_ignore_if_matched` varchar(500) collate utf8_unicode_ci NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `wordfilter_chat` */

DROP TABLE IF EXISTS `wordfilter_chat`;

CREATE TABLE `wordfilter_chat` (
  `regex_match` varchar(500) collate utf8_unicode_ci NOT NULL,
  `regex_ignore_if_matched` varchar(500) collate utf8_unicode_ci NOT NULL default ''
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

/*Table structure for table `worldmap_info` */

DROP TABLE IF EXISTS `worldmap_info`;

CREATE TABLE `worldmap_info` (
  `entry` int(10) unsigned NOT NULL default '0',
  `screenid` int(10) unsigned default '0',
  `type` int(10) unsigned default '0',
  `maxplayers` int(10) unsigned default '0',
  `minlevel` int(10) unsigned default '1',
  `repopx` float default '0',
  `repopy` float default '0',
  `repopz` float default '0',
  `repopentry` int(10) unsigned default '0',
  `area_name` varchar(100) collate utf8_unicode_ci default '0',
  `flags` int(10) unsigned NOT NULL default '0',
  `cooldown` int(10) unsigned NOT NULL default '0',
  `required_quest` int(10) unsigned NOT NULL default '0',
  `required_item` int(10) unsigned NOT NULL default '0',
  `heroic_keyid_1` int(30) NOT NULL default '0',
  `heroic_keyid_2` int(30) NOT NULL default '0',
  `viewingDistance` float NOT NULL default '80',
  `required_checkpoint` int(30) NOT NULL default '0',
  `collision` int(10) unsigned NOT NULL default '1',
  PRIMARY KEY  (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='World System';

/*Table structure for table `worldstate_template` */

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

/*Table structure for table `zoneguards` */

DROP TABLE IF EXISTS `zoneguards`;

CREATE TABLE `zoneguards` (
  `zone` int(10) unsigned NOT NULL,
  `horde_entry` int(10) unsigned default NULL,
  `alliance_entry` int(10) unsigned default NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='World System';

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
