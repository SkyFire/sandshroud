/*
MySQL Data Transfer
Source Host:
Source Database:
Target Host:
Target Database:
Date:
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for chatlog
-- ----------------------------
CREATE TABLE `chatlog` (
  `Timestamp` int(11) NOT NULL default '0',
  `AccountID` int(11) NOT NULL default '0',
  `AccountName` varchar(255) NOT NULL default '',
  `IPAdress` varchar(255) NOT NULL default '',
  `PlayerName` varchar(255) NOT NULL default '',
  `Message` longtext NOT NULL,
  UNIQUE KEY `Key` (`Timestamp`,`AccountID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for cheaterlog
-- ----------------------------
CREATE TABLE `cheaterlog` (
  `Timestamp` int(11) NOT NULL default '0',
  `AccountID` int(11) NOT NULL default '0',
  `AccountName` varchar(255) NOT NULL default '',
  `IPAdress` varchar(255) NOT NULL default '',
  `PlayerName` varchar(255) NOT NULL default '',
  `Message` longtext NOT NULL,
  UNIQUE KEY `Key` (`Timestamp`,`AccountID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gmlog
-- ----------------------------
CREATE TABLE `gmlog` (
  `Timestamp` int(11) NOT NULL default '0',
  `AccountID` int(11) NOT NULL default '0',
  `AccountName` varchar(255) NOT NULL default '',
  `IPAdress` varchar(255) NOT NULL default '',
  `PlayerName` varchar(255) NOT NULL default '',
  `Message` longtext NOT NULL,
  UNIQUE KEY `Key` (`Timestamp`,`AccountID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for playerlog
-- ----------------------------
CREATE TABLE `playerlog` (
  `Timestamp` int(11) NOT NULL default '0',
  `AccountID` int(11) NOT NULL default '0',
  `AccountName` varchar(255) NOT NULL default '',
  `IPAdress` varchar(255) NOT NULL default '',
  `PlayerName` varchar(255) NOT NULL default '',
  `Message` longtext NOT NULL,
  UNIQUE KEY `Key` (`Timestamp`,`AccountID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
