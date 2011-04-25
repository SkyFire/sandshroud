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

#ifndef _WORKER_OPCODES_H
#define _WORKER_OPCODES_H

enum WorkerServerOpcodes
{
	IWSMSG_SERVER_PING		= 1,
	IMSMSG_SERVER_PONG		= 2,

	IMSG_PLAYER_TRANSFER	= 3,

	IWSMSG_AUTH_REQUEST		= 4,
	IMSMSG_AUTH_REPLY		= 5,
	IWSMSG_AUTH_RESULT		= 6,

	IMSMSG_REGISTER_MASTER	= 7,
	IWSMSG_REGISTER_RESULT	= 8,

	IMSMSG_WOW_PACKET		= 9,
	IWSMSG_WOW_PACKET		= 10,

	IMSG_NUM_TYPES
};

#endif		// _WORKER_OPCODES_H


