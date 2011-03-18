/*
 * onyx MMORPG Server
 * Copyright (C) 2005-2008 onyx Team <http://www.onyxemu.com/>
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

// onyx Configuration Header File
// Feel free to customize to your needs.

#ifndef __onyxCONFIG_H
#define __onyxCONFIG_H

/** Enable/disable 3D geometry calculations
 * Warning: This may be resource heavy.
 * As of last update, it was nowhere near complete. Only enable for dev use.
 * Default: Disabled
 */
#ifdef ENABLE_COLLISION
# define COLLISION 1
#endif
//#define COLLISION 1

#endif		// __onyxCONFIG_H

