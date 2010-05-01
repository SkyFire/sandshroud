DROP TABLE if EXISTS spelloverride;
CREATE TABLE `spelloverride` (
  `overrideId` INT(30) NOT NULL DEFAULT '0',
  `spellId` INT(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`overrideId`,`spellId`),
  UNIQUE KEY `overrideId` (`overrideId`,`spellId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
 
ALTER TABLE `spell_coef_override` CHANGE `Base_coef` `Dspell_coef_override` FLOAT DEFAULT '0';
ALTER TABLE `spell_coef_override` CHANGE `Misc_coef` `OTspell_coef_override` FLOAT DEFAULT '0';
ALTER TABLE `spell_coef_override` CHANGE `AP_coef` `AP_coef_override` FLOAT DEFAULT '0';
ALTER TABLE `spell_coef_override` CHANGE `RAP_coef` `RAP_coef_override` FLOAT DEFAULT '0';