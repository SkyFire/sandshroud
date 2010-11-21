ALTER TABLE `ai_agents` CHANGE `type` `type` enum('MELEE','RANGED','SPELL') NOT NULL DEFAULT 'SPELL';
ALTER TABLE `creature_info` ADD COLUMN `CanRange` tinyint(3) NOT NULL DEFAULT '0', ADD COLUMN `CanCallForHelp` tinyint(3) NOT NULL DEFAULT '0',
 ADD COLUMN `CallForHelpHPPercent` float(8,4) NOT NULL DEFAULT '10.0000',
 ADD COLUMN `CanFlee` tinyint(3) NOT NULL DEFAULT '0',
 ADD COLUMN `FleeHealthPercent` float(8,4) NOT NULL DEFAULT '10.0000',
 ADD COLUMN `FleeMSDuration` int(10) NOT NULL DEFAULT '5000',
 ADD COLUMN `SendFleeMessage` int(10) NOT NULL DEFAULT '0',
 ADD COLUMN `FleeMessage` varchar(255) NOT NULL DEFAULT '' COMMENT 'DON''T KILL MEH!',
 ADD COLUMN `GivesNoSkill` tinyint(3) NOT NULL DEFAULT '0',
 ADD COLUMN `GivesNoXP` tinyint(3) DEFAULT '0';
/* UPDATE `creature_info` JOIN creature_proto ON creature_info.entry = creature_proto.entry SET creature_info.GivesNoXP = creature_proto.no_XP; */
/* UPDATE `creature_info` JOIN creature_proto ON creature_info.entry = creature_proto.entry SET creature_info.no_skill_up = creature_proto.no_skill_up; */
ALTER TABLE `creature_proto` DROP COLUMN `no_XP`, DROP COLUMN `no_skill_up`;