
-- Introducing loot_skill_type, default is tamamble 1, use 256 for herballoot and 512 for miningloot.
ALTER TABLE `creature_names` ADD COLUMN `loot_skill_type` int(10) NOT NULL DEFAULT 1 AFTER `type`;
RENAME TABLE skinningloot TO creatureloot_gathering;

