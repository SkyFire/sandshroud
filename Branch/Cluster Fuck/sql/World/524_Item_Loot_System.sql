-- We should transfer all of the loot into the new table

INSERT INTO `itemloot` (entryid, itemid, percentchance, mincount, maxcount, ffa_loot ) SELECT entryid, itemid, percentchance, mincount, maxcount, ffa_loot  FROM `disenchantingloot`;
INSERT INTO `itemloot` (entryid, itemid, percentchance, mincount, maxcount, ffa_loot ) SELECT entryid, itemid, percentchance, mincount, maxcount, ffa_loot  FROM `prospectingloot`;
INSERT INTO `itemloot` (entryid, itemid, percentchance, mincount, maxcount, ffa_loot ) SELECT entryid, itemid, percentchance, mincount, maxcount, ffa_loot  FROM `millingloot`;

-- Remove the tables.
DROP TABLE IF EXISTS `disenchantingloot`;
DROP TABLE IF EXISTS `prospectingloot`;
DROP TABLE IF EXISTS `millingloot`;