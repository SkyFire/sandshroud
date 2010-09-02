
CREATE TABLE `playerphaseinfo` (
  `guid` int(10) unsigned NOT NULL DEFAULT '0',
  `areaid` int(10) unsigned NOT NULL DEFAULT '0',
  `phase` int(11) NOT NULL DEFAULT '1',
  UNIQUE KEY `Index` (`guid`,`areaid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
