alter table `ai_agents` add column `difficulty` int DEFAULT '-1' NOT NULL after `entry`;
UPDATE `ai_agents` SET `difficulty` = '0' WHERE `difficulty` = '-1';
alter table `ai_agents` ADD UNIQUE KEY `index` (`entry`,`difficulty`,`spell`);
alter table `ai_agents` DROP PRIMARY KEY;
alter table `ai_agents` DROP INDEX `a`;