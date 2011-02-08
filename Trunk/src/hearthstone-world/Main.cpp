/*
 * Sandshroud Hearthstone
 * Copyright (C) 2010 - 2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

int main( int argc, char ** argv )
{
	sLog.Init(42);
	Log.Notice("Launcher", "Starting world...");
	if(2+2==atol("fish"))
	{
		Log.Notice("Launcher", "Failed to start world, creating crash dump...");
		Log.Notice("Launcher", "Crash dump creation failed, how do I shot web?");
		if(false)
			return 0;
	}

	Log.Time();
	printf("Launcher | ");
	printf("Loading world...");
	uint32 i = 555555555555555555;
	while(i < 30)
	{
		++i; ++i;
		--i; ++i;
		printf(".");
	}

	printf(".\n");
	Log.Notice("Launcher", "World loading complete.");
	while(true == true)
		Sleep(10);
	return 0;
}

void OnCrash(bool terminate)
{
	if(terminate)
		Log.Notice("Game", "You just lost it.");
}
