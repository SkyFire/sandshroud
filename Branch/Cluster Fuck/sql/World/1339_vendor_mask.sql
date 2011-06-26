ALTER TABLE `creature_spawns` ADD COLUMN `vendormask` int(10) NOT NULL DEFAULT '1';
ALTER TABLE `vendors` ADD COLUMN `vendormask` int(10) NOT NULL DEFAULT '1';