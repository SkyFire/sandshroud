
-- Creatures having this set to 1 will never generate XP.
ALTER TABLE `creature_proto` ADD COLUMN `no_XP` int(11) NOT NULL DEFAULT 0 AFTER `money`;

