
-- Table "creature_teleport_info" DDL

CREATE TABLE `creature_teleport_info` (
  `entry` mediumint(10) NOT NULL DEFAULT '0',
  `intid` mediumint(10) NOT NULL DEFAULT '0',
  `iconid` mediumint(10) NOT NULL DEFAULT '0',
  `teleport` tinyint(3) NOT NULL DEFAULT '0',
  `textinfo` varchar(255) NOT NULL DEFAULT '',
  `teleportmapid` int(30) unsigned NOT NULL DEFAULT '0',
  `teleportx` float(30,3) NOT NULL DEFAULT '0',
  `teleporty` float(30,3) NOT NULL DEFAULT '0',
  `teleportz` float(30,3) NOT NULL DEFAULT '0',
  `teleporto` float(30,3) NOT NULL DEFAULT '0',
  `castspellid` int(30) unsigned NOT NULL DEFAULT '0',
  `removetargetaura` int(30) unsigned NOT NULL DEFAULT '0',
  `sendchatmessage` tinyint(3) NOT NULL DEFAULT '0',
  `messagetype` mediumint(10) NOT NULL DEFAULT '0',
  `messagetosend` varchar(255) NOT NULL DEFAULT '',
  UNIQUE KEY `Index` (`entry`,`intid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
