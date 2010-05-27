ALTER TABLE `items`  ADD COLUMN `ScalingStatId` INT(30) NOT NULL DEFAULT '0' AFTER `stat_value10`, 
ADD COLUMN `ScalingStatFlags` INT(30) NOT NULL DEFAULT '0' AFTER `ScalingStatId`;