-- min/maxdmg must be floats
ALTER TABLE `creature_proto_heroic` MODIFY COLUMN `minlevel` int(10) unsigned NOT NULL DEFAULT 1 AFTER `entry`;

ALTER TABLE `creature_proto_heroic` MODIFY COLUMN `maxlevel` int(10) unsigned NOT NULL DEFAULT 1 AFTER `minlevel`;

ALTER TABLE `creature_proto_heroic` MODIFY COLUMN `mindmg` float NOT NULL DEFAULT 1 AFTER `maxhealth`;

ALTER TABLE `creature_proto_heroic` MODIFY COLUMN `maxdmg` float NOT NULL DEFAULT 1 AFTER `mindmg`;




