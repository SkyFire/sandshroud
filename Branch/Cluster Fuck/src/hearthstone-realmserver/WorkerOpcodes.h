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
	SMSGR_AUTH_REQUEST = 1,
	CMSGR_AUTH_REPLY,
	SMSGR_AUTH_RESULT,
	CMSGR_REGISTER_WORKER,
	SMSGR_REGISTER_RESULT,

	SMSGR_CREATE_INSTANCE,
	SMSGR_DESTROY_INSTANCE,

	SMSGR_PLAYER_LOGIN,

	SMSGR_PLAYER_INFO,
	CMSGR_PLAYER_INFO,

	SMSGR_WOW_PACKET,
	CMSGR_WOW_PACKET,

	CMSGR_WHISPER,
	SMSGR_WHISPER,

	CMSGR_CHAT,
	SMSGR_CHAT,

	SMSGR_PACKED_PLAYER_INFO,

	CMSGR_PLAYER_LOGIN_RESULT,
	CMSGR_PLAYER_LOGOUT,

	SMSGR_DESTROY_PLAYER_INFO,

	CMSGR_TELEPORT_REQUEST,
	SMSGR_TELEPORT_RESULT,

	SMSGR_PLAYER_CHANGED_SERVERS,
	SMSGR_PLAYER_CHANGE_INSTANCES,
	CMSGR_PLAYER_CHANGE_SERVER_INFO,

	SMSGR_ERROR_HANDLER,
	CMSGR_ERROR_HANDLER,

	SMSGR_SESSION_REMOVED,
	CMSGR_SWITCH_SERVER,
	CMSGR_SAVE_ALL_PLAYERS,
	SMSGR_SAVE_ALL_PLAYERS,
	CMSGR_TRANSPORTER_MAP_CHANGE,
	SMSGR_TRANSPORTER_MAP_CHANGE,
	CMSGR_PLAYER_TELEPORT,
	SMSGR_PLAYER_TELEPORT,
	CMSGR_CREATE_PLAYER,
	SMSGR_CHANNEL_ACTION,
	CMSGR_CHANNEL_ACTION,
	CMSGR_CHANNEL_UPDATE,
	SMSGR_CHANNEL_LFG_DUNGEON_STATUS_REQUEST,
	CMSGR_CHANNEL_LFG_DUNGEON_STATUS_REPLY,
	MSGR_NUM_TYPES
};

#endif		// _WORKER_OPCODES_H


