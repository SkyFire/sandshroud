/*
 * Sandshroud Hearthstone
 * Copyright (C) 2005 - 2010 MaNGOS <http://getmangos.com/>
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

#ifndef _VMAPFACTORY_H
#define _VMAPFACTORY_H

#include "VMapManager2.h"

/**
This is the access point to the VMapManager.
*/

namespace VMAP
{
	//===========================================================

	class VMapFactory
	{
		public:
			static VMapManager2* createOrGetVMapManager();
			static void clear();

			static void preventSpellsFromBeingTestedForLoS(const char* pSpellIdString);
			static bool checkSpellForLoS(unsigned int pSpellId);
	};

}
#endif
