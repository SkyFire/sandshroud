/*Table structure for table `tracker` */
CREATE TABLE `tracker` (
  `Id` int(10) unsigned NOT NULL,
  `AcctId` int(20) unsigned NOT NULL,
  `Name` varchar(21) NOT NULL,
  `IP_Address` varchar(15) NOT NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
