-- Creature_spawns updates.
ALTER TABLE `creature_spawns` ADD `ItemSlot1` int(10) NOT NULL DEFAULT '0' after `MountedDisplayID`;
ALTER TABLE `creature_spawns` ADD `ItemSlot2` int(10) NOT NULL DEFAULT '0' after `ItemSlot1`;
ALTER TABLE `creature_spawns` ADD `ItemSlot3` int(10) NOT NULL DEFAULT '0' after `ItemSlot2`;

-- Creature_staticspawns updates. -- Crow: As far as I know, most structures are not up to date. This is up to the DB team to be in charge of.
-- ALTER TABLE `creature_staticspawns` ADD `ItemSlot1` int(10) NOT NULL DEFAULT '0' after `MountedDisplayID`;
-- ALTER TABLE `creature_staticspawns` ADD `ItemSlot2` int(10) NOT NULL DEFAULT '0' after `ItemSlot1`;
-- ALTER TABLE `creature_staticspawns` ADD `ItemSlot3` int(10) NOT NULL DEFAULT '0' after `ItemSlot2`;

-- Set our weapon ids.
UPDATE `creature_spawns` JOIN creature_proto ON creature_proto.entry = creature_spawns.entry SET creature_spawns.ItemSlot1 = creature_proto.Item1;
UPDATE `creature_spawns` JOIN creature_proto ON creature_proto.entry = creature_spawns.entry SET creature_spawns.ItemSlot2 = creature_proto.Item2;
UPDATE `creature_spawns` JOIN creature_proto ON creature_proto.entry = creature_spawns.entry SET creature_spawns.ItemSlot3 = creature_proto.Item3;