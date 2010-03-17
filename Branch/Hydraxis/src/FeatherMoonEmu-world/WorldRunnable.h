/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

//
// WorldRunnable.h
//

#ifndef __WORLDRUNNABLE_H
#define __WORLDRUNNABLE_H

class WorldRunnable : public ThreadContext
{
public:
	WorldRunnable();
	bool run();
};

#endif
