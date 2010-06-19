/*
 * Sun++ Scripts for Sandshroud MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2009-2011 Sandshroud <http://www.sandshroud.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MISC_SCRIPTS_SETUP_H
#define MISC_SCRIPTS_SETUP_H

/* A hook defined to ALL_MAPS will be called regardless of the Map ID */
#define ALL_MAPS 0xFFFFFFFF

/* Crow: Quick example of what you should do.
We have here a void, but we would use the following structure:
type: void name: Setup%s Pointer: (ScriptMgr * mgr); Comment: // Set up for %s
Type should always be void, %s is set to the name we want, and the comment should explain what the name cannot.

Example 1:
void SetupGenericCreatureScripts(ScriptMgr * mgr); // Set up for generic scripts.
Example 2:
/_* Set up for generic scripts. *_/_
void SetupGenericCreatureScripts(ScriptMgr * mgr);
*/

#endif
