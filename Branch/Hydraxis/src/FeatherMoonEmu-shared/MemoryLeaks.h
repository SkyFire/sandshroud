/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef WOWSERVER_MEMORY_H
#define WOWSERVER_MEMORY_H

#include "Common.h"
#include "Singleton.h"

struct MemoryManager : public Singleton < MemoryManager > {
	MemoryManager();
};

#endif

