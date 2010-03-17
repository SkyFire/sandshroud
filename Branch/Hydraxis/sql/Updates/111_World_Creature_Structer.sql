ALTER TABLE `Creature_proto` ADD `spell1` INT(32) DEFAULT '0' NOT NULL AFTER `CanMove`;
ALTER TABLE `Creature_proto` ADD `spell2` INT(32) DEFAULT '0' NOT NULL AFTER `spell1`;
ALTER TABLE `Creature_proto` ADD `spell3` INT(32) DEFAULT '0' NOT NULL AFTER `spell2`;
ALTER TABLE `Creature_proto` ADD `spell4` INT(32) DEFAULT '0' NOT NULL AFTER `spell3`;
ALTER TABLE `Creature_proto` ADD `spell5` INT(32) DEFAULT '0' NOT NULL AFTER `spell4`;
ALTER TABLE `Creature_proto` ADD `spell6` INT(32) DEFAULT '0' NOT NULL AFTER `spell5`;
ALTER TABLE `Creature_proto` ADD `spell7` INT(32) DEFAULT '0' NOT NULL AFTER `spell6`;
ALTER TABLE `Creature_proto` ADD `spell8` INT(32) DEFAULT '0' NOT NULL AFTER `spell7`;