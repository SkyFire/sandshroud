ALTER TABLE `worldmap_info` DROP COLUMN `screenid`;
ALTER TABLE `worldmap_info` CHANGE COLUMN `area_name` `name` VARCHAR(100) DEFAULT '' AFTER `entry`;
ALTER TABLE `worldmap_info` ADD COLUMN `load` INT(3) UNSIGNED NOT NULL DEFAULT '1' AFTER `name`;