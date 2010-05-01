/*
SQLyog Enterprise - MySQL GUI v8.04 
MySQL - 6.0.7-alpha-community : Database - char
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

/*Table structure for table `playerpetactionbar` */

DROP TABLE IF EXISTS `playerpetactionbar`;

CREATE TABLE `playerpetactionbar` (
  `ownerguid` bigint(20) NOT NULL DEFAULT '0',
  `petnumber` int(11) NOT NULL DEFAULT '0',
  `spellid_1` int(11) NOT NULL DEFAULT '0',
  `spellid_2` int(11) NOT NULL DEFAULT '0',
  `spellid_3` int(11) NOT NULL DEFAULT '0',
  `spellid_4` int(11) NOT NULL DEFAULT '0',
  `spellid_5` int(11) NOT NULL DEFAULT '0',
  `spellid_6` int(11) NOT NULL DEFAULT '0',
  `spellid_7` int(11) NOT NULL DEFAULT '0',
  `spellid_8` int(11) NOT NULL DEFAULT '0',
  `spellid_9` int(11) NOT NULL DEFAULT '0',
  `spellid_10` int(11) NOT NULL DEFAULT '0',
  `spellstate_1` int(11) NOT NULL DEFAULT '0',
  `spellstate_2` int(11) NOT NULL DEFAULT '0',
  `spellstate_3` int(11) NOT NULL DEFAULT '0',
  `spellstate_4` int(11) NOT NULL DEFAULT '0',
  `spellstate_5` int(11) NOT NULL DEFAULT '0',
  `spellstate_6` int(11) NOT NULL DEFAULT '0',
  `spellstate_7` int(11) NOT NULL DEFAULT '0',
  `spellstate_8` int(11) NOT NULL DEFAULT '0',
  `spellstate_9` int(11) NOT NULL DEFAULT '0',
  `spellstate_10` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ownerguid`,`petnumber`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;

ALTER TABLE `playerpets` DROP COLUMN `actionbar`;
