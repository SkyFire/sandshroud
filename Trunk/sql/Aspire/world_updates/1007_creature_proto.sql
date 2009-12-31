-- FYI core doesn't realy care that much about names anyway, but these are the correct names:
alter table creature_proto change column resistance6 `resistance7` int(30) UNSIGNED NOT NULL DEFAULT 0;
alter table creature_proto change column resistance5 `resistance6` int(30) UNSIGNED NOT NULL DEFAULT 0;
alter table creature_proto change column resistance4 `resistance5` int(30) UNSIGNED NOT NULL DEFAULT 0;
alter table creature_proto change column resistance3 `resistance4` int(30) UNSIGNED NOT NULL DEFAULT 0;
alter table creature_proto change column resistance2 `resistance3` int(30) UNSIGNED NOT NULL DEFAULT 0;
alter table creature_proto change column resistance1 `resistance2` int(30) UNSIGNED NOT NULL DEFAULT 0;
alter table creature_proto change column armor `resistance1` int(30) UNSIGNED NOT NULL DEFAULT 0;

alter table creature_proto change column equipmodel1 `Item1` int(30) UNSIGNED NOT NULL DEFAULT 0 ;
alter table creature_proto change column equipmodel2 `Item2` int(30) UNSIGNED NOT NULL DEFAULT 0 ;
alter table creature_proto change column equipmodel3 `Item3` int(30) UNSIGNED NOT NULL DEFAULT 0 ; 

-- Made these queries available as R1007, as they seem to be a missing like with full structure dump of 1008
