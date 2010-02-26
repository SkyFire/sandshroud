-- Prevents XP generation for NPC When set to 1
ALTER TABLE `creature_proto` ADD COLUMN `no_XP` int(1) NOT NULL DEFAULT 0 AFTER `money`;
