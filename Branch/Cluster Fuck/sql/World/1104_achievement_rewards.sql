ALTER TABLE `achievement_rewards`
 ADD COLUMN `SendMessage` int(3) NOT NULL DEFAULT '0',
 ADD COLUMN `SenderEntry` int(10) NOT NULL DEFAULT '0',
 ADD COLUMN `messageheader` longtext COLLATE utf8_unicode_ci,
 ADD COLUMN `messagebody` longtext COLLATE utf8_unicode_ci;