-- Creature_spawns updates.
ALTER TABLE `creature_spawns` ADD `CanMove` int(8) NOT NULL DEFAULT '1';

-- Creature_staticspawns updates. -- Crow: As far as I know, most structures are not up to date. This is up to the DB team to be in charge of.

-- Set our weapon ids.
UPDATE `creature_spawns` JOIN creature_proto ON creature_proto.entry = creature_spawns.entry SET creature_spawns.CanMove = creature_proto.CanMove;