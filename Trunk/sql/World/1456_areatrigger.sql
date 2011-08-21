ALTER TABLE `areatriggers` ADD COLUMN `requiredteam` smallint(3) NOT NULL DEFAULT '-1' AFTER `type`;
ALTER TABLE `areatriggers` DROP `screen`;