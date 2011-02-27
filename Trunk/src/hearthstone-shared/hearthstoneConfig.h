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

/** Enable/disable Hearthstone world server cluster support.
 * Warning: Hearthstone will no longer function independantly when built in cluster mode.
 * It will require a realmserver to function.
 * As of last update, it is nowhere near complete, only enable for dev use.
 * Default: Disabled
 */
//#define CLUSTERING 1

/** Enable/disable Hearthstone's Voice Chat support.
 * While not completely finished (but close!) you can link your Hearthstone server to a voice chat
 * server (also compiled by default) and use the client's ingame voice chat function.
 * At the moment it is only good for joining, you can't leave a voice channel yet :P
 * Also, it only works for parties. Custom chat channels are not fully supported yet.
 * However, if you want to play around with it, feel free to enable this define.
 */
//#define VOICE_CHAT 1

// For Linux, disabled for compile reasons.
//#define LOAD_LACRIMI

/* This makes it so that object pushing for players is handled in a different
 * thread than the map manager, so if a player enters a heavily populated area,
 * he doesn't get a hang up, and information is sent after the player enters world.
 */
//#define MULTI_THREADED_OBJECT_PUSHING

#endif		// __HEARTHSTONECONFIG_H
