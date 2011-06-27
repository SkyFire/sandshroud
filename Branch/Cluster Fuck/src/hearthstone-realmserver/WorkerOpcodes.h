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
	SMSGR_AUTH_REQUEST							= 1,
	CMSGR_AUTH_REPLY							= 2,
	SMSGR_AUTH_RESULT							= 3,
	CMSGR_REGISTER_WORKER						= 4,
	SMSGR_REGISTER_RESULT						= 5,
	SMSGR_CREATE_INSTANCE						= 6,
	SMSGR_DESTROY_INSTANCE						= 7,
	SMSGR_PLAYER_LOGIN							= 8,
	SMSGR_PLAYER_INFO							= 9,
	CMSGR_PLAYER_INFO							= 10,
	SMSGR_WOW_PACKET							= 11,
	CMSGR_WOW_PACKET							= 12,
	CMSGR_WHISPER								= 13,
	CMSGR_CHAT									= 14,
	SMSGR_WHISPER								= 15,
	SMSGR_CHAT									= 16,
	SMSGR_PACKED_PLAYER_INFO					= 17,
	CMSGR_PLAYER_LOGIN_RESULT					= 18,
	CMSGR_PLAYER_LOGOUT							= 19,
	SMSGR_DESTROY_PLAYER_INFO					= 20,
	CMSGR_TELEPORT_REQUEST						= 21,
	SMSGR_TELEPORT_RESULT						= 22,
	SMSGR_PLAYER_CHANGED_SERVERS				= 23,
	SMSGR_PLAYER_CHANGE_INSTANCES				= 24,
	CMSGR_PLAYER_CHANGE_SERVER_INFO				= 26,
	CMSGR_ERROR_HANDLER							= 27,
	SMSGR_SESSION_REMOVED						= 28,
	CMSGR_SWITCH_SERVER							= 29,
	CMSGR_SAVE_ALL_PLAYERS						= 30,
	SMSGR_SAVE_ALL_PLAYERS						= 31,
	CMSGR_TRANSPORTER_MAP_CHANGE				= 32,
	SMSGR_TRANSPORTER_MAP_CHANGE				= 33,
	CMSGR_PLAYER_TELEPORT						= 34,
	SMSGR_PLAYER_TELEPORT						= 35,
	CMSGR_CREATE_PLAYER							= 36,
	SMSGR_CHANNEL_ACTION						= 37,
	CMSGR_CHANNEL_ACTION						= 38,
	CMSGR_CHANNEL_UPDATE						= 39,
	SMSGR_CHANNEL_LFG_DUNGEON_STATUS_REQUEST	= 40,
	CMSGR_CHANNEL_LFG_DUNGEON_STATUS_REPLY		= 41,
	MSGR_NUM_TYPES
};

#endif		// _WORKER_OPCODES_H


