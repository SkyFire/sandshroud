/*
 * Sandshroud Hearthstone
 * Copyright (C) 2004 - 2005 Antrix Team
 * Copyright (C) 2005 - 2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 - 2009 AspireDev <http://www.aspiredev.org/>
 * Copyright (C) 2009 - 2010 Sandshroud <http://www.sandshroud.org/>
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

// Hearthstone Configuration Header File
// Feel free to customize to your needs.

#ifndef __HEARTHSTONECONFIG_H
#define __HEARTHSTONECONFIG_H

// For Linux, disabled for compile reasons.
//#define LOAD_LACRIMI

/* This makes it so that object pushing for players is handled in a different
 * thread than the map manager, so if a player enters a heavily populated area,
 * he doesn't get a hang up, and information is sent after the player enters world.
 */
//#define MULTI_THREADED_OBJECT_PUSHING

#endif		// __HEARTHSTONECONFIG_H
