/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef __CConsole_H
#define __CConsole_H

#include "Common.h"

class ConsoleThread : public ThreadContext
{
protected:
	bool m_isRunning;
public:
	void terminate();
	bool run();
};

#endif