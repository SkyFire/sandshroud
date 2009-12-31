/*
SQLyog Community Edition- MySQL GUI v5.32
Host - 6.0.7-alpha-community : Database - char
*********************************************************************
Server version : 6.0.7-alpha-community
*/


/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

/*Table structure for table `events_settings` */

DROP TABLE IF EXISTS `events_settings`;

CREATE TABLE `events_settings` (
  `eventid` tinyint(2) unsigned NOT NULL,
  `lastactivated` bigint(20) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`eventid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
