-- ----------------------------
-- Table structure for chatlog
-- ----------------------------
CREATE TABLE `chatlog` (
  `timestamp` int(11) NOT NULL DEFAULT '0',
  `message` longtext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for cheaterlog
-- ----------------------------
CREATE TABLE `cheaterlog` (
  `timestamp` int(11) NOT NULL DEFAULT '0',
  `message` longtext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gmlog
-- ----------------------------
CREATE TABLE `gmlog` (
  `timestamp` int(11) NOT NULL DEFAULT '0',
  `message` longtext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for playerlog
-- ----------------------------
CREATE TABLE `playerlog` (
  `timestamp` int(11) NOT NULL DEFAULT '0',
  `message` longtext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;