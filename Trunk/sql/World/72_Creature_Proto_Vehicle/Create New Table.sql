
CREATE TABLE `creature_proto_vehicle` (
  `vehiclecreatureid` int(30) NOT NULL DEFAULT '1',
  `increasehealthbydriver` int(4) NOT NULL DEFAULT '0',
  `healthforitemlevel` int(30) NOT NULL DEFAULT '0',
  `spell1` int(30) NOT NULL DEFAULT '0',
  `spell2` int(30) NOT NULL DEFAULT '0',
  `spell3` int(30) NOT NULL DEFAULT '0',
  `spell4` int(30) NOT NULL DEFAULT '0',
  `spell5` int(30) NOT NULL DEFAULT '0',
  `spell6` int(30) NOT NULL DEFAULT '0',
  `accessoryseat1` int(30) NOT NULL DEFAULT '0',
  `accessoryseat2` int(30) NOT NULL DEFAULT '0',
  `accessoryseat3` int(30) NOT NULL DEFAULT '0',
  `accessoryseat4` int(30) NOT NULL DEFAULT '0',
  `accessoryseat5` int(30) NOT NULL DEFAULT '0',
  `accessoryseat6` int(30) NOT NULL DEFAULT '0',
  `accessoryseat7` int(30) NOT NULL DEFAULT '0',
  `accessoryseat8` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`vehiclecreatureid`),
  UNIQUE KEY `SeatVehicle` (`vehiclecreatureid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
