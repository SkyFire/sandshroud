DROP TABLE IF EXISTS `events`;
DROP TABLE IF EXISTS `events_creature`;
DROP TABLE IF EXISTS `events_gameobject`;
ALTER TABLE `creature_spawns` DROP COLUMN `eventid`;
ALTER TABLE `gameobject_spawns` DROP COLUMN `eventid`;