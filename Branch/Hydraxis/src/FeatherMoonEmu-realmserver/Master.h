/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef _R_MASTER_H
#define _R_MASTER_H

extern Database * Database_World;
extern Database * Database_Character;
extern Database * Database_Rip;

#define WorldDatabase (*Database_World)
#define CharacterDatabase (*Database_Character)
#define RipDatabase (*Rip_Database)

#endif
