
-- ----------------------------
-- Table structure for warnsystem
-- ----------------------------
CREATE TABLE `warnsystem` (
  `WarnID` int(20) unsigned NOT NULL,
  `GMAcct` varchar(32) NOT NULL,
  `WarnedChar` varchar(21) NOT NULL,
  `WarnedAcct` varchar(32) NOT NULL,
  `WarnedIP` varchar(15) NOT NULL,
  `Reason` varchar(255) NOT NULL,
  `time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`WarnID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
