ALTER TABLE quests ADD COLUMN `ReqKillPlayer` int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `ReqItemCount4`;
ALTER TABLE quests ADD COLUMN `RewBonusHonor` int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `RewMoney`;
ALTER TABLE quests ADD COLUMN `RewTalentPoints` int(10) UNSIGNED NOT NULL DEFAULT 0 AFTER `RewSpell`;