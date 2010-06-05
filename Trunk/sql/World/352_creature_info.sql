CREATE TABLE `creature_info` (
  `entry` int(32) NOT NULL DEFAULT '0',
  `default_emote_state` int(32) NOT NULL DEFAULT '0',
  `default_spawn_flags` int(32) NOT NULL DEFAULT '0',
  `default_stand_state` int(32) NOT NULL DEFAULT '0',
  `default_mount_displayid` int(32) NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
