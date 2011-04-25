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

#ifndef G3D_GAMEUNITS_H
#define G3D_GAMEUNITS_H

#include "platform.h"

namespace G3D {
/**
 Time, in seconds.
 */
typedef double GameTime;
typedef double SimTime;

/**
 Actual wall clock time in seconds.
 */
typedef double RealTime;

enum AMPM {AM, PM};

/** \deprecated */
enum {SECOND=1, MINUTE=60, HOUR = 60*60, DAY=24*60*60, SUNRISE=24*60*60/4, SUNSET=24*60*60*3/4, MIDNIGHT=0, METER=1, KILOMETER=1000};

/**
 Converts a 12 hour clock time into the number of seconds since 
 midnight.  Note that 12:00 PM is noon and 12:00 AM is midnight.

 Example: <CODE>toSeconds(10, 00, AM)</CODE>
 */
SimTime toSeconds(int hour, int minute, double seconds, AMPM ap);
SimTime toSeconds(int hour, int minute, AMPM ap);

}

#endif
