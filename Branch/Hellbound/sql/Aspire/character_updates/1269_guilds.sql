/*
MySQL Data Transfer
Target Database: HS_characters
Date: 17/01/2009 13:16:47
*/

-- Just some cosmetics.
ALTER TABLE `guilds` DROP INDEX `guildId`;
ALTER TABLE `guilds` ADD UNIQUE `guildName` (`guildName`);
ALTER TABLE `guilds` ADD UNIQUE `leaderGuid` (`leaderGuid`);