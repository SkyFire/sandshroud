CREATE TABLE `lfd_rewards` (
  `dungeonid` int(10) unsigned NOT NULL DEFAULT '0',
  `comments` varchar(255) NOT NULL DEFAULT '',
  `questid1` int(10) unsigned NOT NULL DEFAULT '0',
  `moneyreward1` int(10) NOT NULL DEFAULT '0',
  `xpreward1` int(10) NOT NULL DEFAULT '0',
  `questid2` int(10) NOT NULL DEFAULT '0',
  `moneyreward2` int(10) NOT NULL DEFAULT '0',
  `xpreward2` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`dungeonid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Loot System';
