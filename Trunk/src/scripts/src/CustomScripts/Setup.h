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

#ifndef CUSTOM_SCRIPTS_SETUP_H
#define CUSTOM_SCRIPTS_SETUP_H

#include "StdAfx.h"

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

#endif // CUSTOM_SCRIPTS_SETUP_H
