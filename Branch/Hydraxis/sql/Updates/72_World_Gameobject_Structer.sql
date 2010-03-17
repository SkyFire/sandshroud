ALTER TABLE `Gameobject_names` ADD `Category` VARCHAR(32) DEFAULT '' NOT NULL AFTER `Name`;
ALTER TABLE `Gameobject_names` ADD `Castbartext` VARCHAR(32) DEFAULT '' NOT NULL AFTER `Category`;
ALTER TABLE `Gameobject_names` ADD `Unkstr` VARCHAR(32) DEFAULT '' NOT NULL AFTER `Castbartext`;