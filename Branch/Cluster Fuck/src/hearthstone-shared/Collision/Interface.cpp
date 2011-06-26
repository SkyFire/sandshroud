/*
 * Sandshroud Hearthstone
 * Copyright (C) 2005 - 2008 Hearthstone <http://www.hearthstone-project.com/>
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

#include "vmap/VMapManager2.h"
#include "g3dlite/G3DAll.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "LocationVector.h"

VMAP::VMapManager2* vmgr;

void * collision_init()
{
	vmgr = new VMAP::VMapManager2();
	return vmgr;
}

void collision_shutdown()
{
	vmgr->unloadMap(0);
	vmgr->unloadMap(1);
	vmgr->unloadMap(530);
	vmgr->unloadMap(571);
}

