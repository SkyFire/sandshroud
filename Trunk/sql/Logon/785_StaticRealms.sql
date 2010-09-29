
-- ----------------------------
-- Table structure for static_realms
-- ----------------------------
CREATE TABLE `static_realms` (
  `name` varchar(32) NOT NULL,
  `adress` varchar(32) NOT NULL DEFAULT 'localhost',
  `Port` int(11) NOT NULL DEFAULT '8127',
  `Icon` int(11) NOT NULL DEFAULT '0',
  `WorldRegion` int(11) NOT NULL DEFAULT '0',
  `Population` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`name`,`Port`)
) ENGINE=MyISAM AUTO_INCREMENT=20 DEFAULT CHARSET=utf8;
