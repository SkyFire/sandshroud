CREATE TABLE `gm_surveys` (
  `id` int(11) unsigned NOT NULL auto_increment,
  `playerguid` int(11) unsigned NOT NULL,
  `question1` int(11) unsigned NOT NULL default '0',
  `answer1` int(11) unsigned NOT NULL default '0',
  `question2` int(11) unsigned NOT NULL default '0',
  `answer2` int(11) unsigned NOT NULL default '0',
  `question3` int(11) unsigned NOT NULL default '0',
  `answer3` int(11) unsigned NOT NULL default '0',
  `question4` int(11) unsigned NOT NULL default '0',
  `answer4` int(11) unsigned NOT NULL default '0',
  `question5` int(11) unsigned NOT NULL default '0',
  `answer5` int(11) unsigned NOT NULL default '0',
  `question6` int(11) unsigned NOT NULL default '0',
  `answer6` int(11) unsigned NOT NULL default '0',
  `question7` int(11) unsigned NOT NULL default '0',
  `answer7` int(11) unsigned NOT NULL default '0',
  `comment` text collate utf8_unicode_ci NOT NULL,
  `timestamp` int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

ALTER TABLE `playertalents`  ENGINE=INNODB CHECKSUM=1 COMMENT='' DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC CHARSET=latin1 COLLATE=latin1_swedish_ci;