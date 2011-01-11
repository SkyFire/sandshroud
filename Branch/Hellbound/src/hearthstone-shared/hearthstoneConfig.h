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

/** Enable/disable hearthstone world server optimized player saving.
 * It is not yet complete, things have to be updated, and player account data needs to be fixed.
 * Player guilds need to be updated as well, not saving correctly :|
 * Default: Disabled
 */

//#define OPTIMIZED_PLAYER_SAVING 1

#define GM_TICKET_MY_MASTER_COMPATIBLE

/** Enable/disable movement compression.
 * This allows the server to compress long-range creatures movement into a buffer and then flush
 * it periodically, compressed with deflate. This can make a large difference to server bandwidth.
 * Currently this sort of compression is only used for player and creature movement, although
 * it may be expanded in the future.
 * Default: disabled
 */

#define ENABLE_COMPRESSED_MOVEMENT 1
#define ENABLE_COMPRESSED_MOVEMENT_FOR_PLAYERS 1
#define ENABLE_COMPRESSED_MOVEMENT_FOR_CREATURES 1

/* Crow: Buffer saving.
 * Default: enabled
 */
#define BUFFER_SAVING 1

// Crow: This will nerf all Enchantments, making all of them pretty much unique internally.
//#define NERF_ENCHANTS

// Crow: This is for Linux, not entirely suggested.
//#define LOAD_LACRIMI

#endif		// __HEARTHSTONECONFIG_H
