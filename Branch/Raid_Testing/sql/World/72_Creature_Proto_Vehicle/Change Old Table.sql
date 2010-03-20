/* Change table structure */

ALTER TABLE `creature_proto_vehicle` ADD `healthforitemlevel` int(30) NOT NULL DEFAULT '0' after `increasehealthbydriver`;

-- Passengers, they will have to be readded by hand.

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat6`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat7`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `accessoryseat8`;

-- Seat Has Spells

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat1hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7hasspells`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8hasspells`;

-- Seat Spells

ALTER TABLE `creature_proto_vehicle` CHANGE seat1spell1 `spell1` int(30);
ALTER TABLE `creature_proto_vehicle` CHANGE seat1spell2 `spell2` int(30);
ALTER TABLE `creature_proto_vehicle` CHANGE seat1spell3 `spell3` int(30);
ALTER TABLE `creature_proto_vehicle` CHANGE seat1spell4 `spell4` int(30);
ALTER TABLE `creature_proto_vehicle` CHANGE seat1spell5 `spell5` int(30);
ALTER TABLE `creature_proto_vehicle` CHANGE seat1spell6 `spell6` int(30);

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat2spell6`;

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat3spell6`;

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat4spell6`;

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat5spell6`;

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat6spell6`;

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat7spell6`;

ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8spell1`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8spell2`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8spell3`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8spell4`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8spell5`;
ALTER TABLE `creature_proto_vehicle` DROP COLUMN `seat8spell6`;

-- Readd accessory columns.

ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat1` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat2` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat3` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat4` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat5` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat6` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat7` int(30) NOT NULL DEFAULT '0';
ALTER TABLE `creature_proto_vehicle` ADD `accessoryseat8` int(30) NOT NULL DEFAULT '0';