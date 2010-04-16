/*Table structure for table `equipmentsets` */

DROP TABLE IF EXISTS `equipmentsets`;

CREATE TABLE `equipmentsets` (
  `guid` int(11) NOT NULL DEFAULT '0',
  `setguid` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `iconname` varchar(100) NOT NULL,
  `item0` int(11) NOT NULL DEFAULT '0',
  `item1` int(11) NOT NULL DEFAULT '0',
  `item2` int(11) NOT NULL DEFAULT '0',
  `item3` int(11) NOT NULL DEFAULT '0',
  `item4` int(11) NOT NULL DEFAULT '0',
  `item5` int(11) NOT NULL DEFAULT '0',
  `item6` int(11) NOT NULL DEFAULT '0',
  `item7` int(11) NOT NULL DEFAULT '0',
  `item8` int(11) NOT NULL DEFAULT '0',
  `item9` int(11) NOT NULL DEFAULT '0',
  `item10` int(11) NOT NULL DEFAULT '0',
  `item11` int(11) NOT NULL DEFAULT '0',
  `item12` int(11) NOT NULL DEFAULT '0',
  `item13` int(11) NOT NULL DEFAULT '0',
  `item14` int(11) NOT NULL DEFAULT '0',
  `item15` int(11) NOT NULL DEFAULT '0',
  `item16` int(11) NOT NULL DEFAULT '0',
  `item17` int(11) NOT NULL DEFAULT '0',
  `item18` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`setguid`),
  UNIQUE KEY `idx_set` (`guid`,`setguid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
