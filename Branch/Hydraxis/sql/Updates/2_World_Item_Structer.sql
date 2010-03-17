ALTER TABLE `Items` ADD `existingduration` INT(32) DEFAULT 0 AFTER `ArmorDamageModifier`;
ALTER TABLE `Items` ADD `ItemLimitCategoryId` INT(32) DEFAULT 0 AFTER `existingduration`;