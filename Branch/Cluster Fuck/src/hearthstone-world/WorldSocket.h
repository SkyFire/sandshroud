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

// Class WorldSocket - Main network code functions, handles
// reading/writing of all packets.

#ifndef __WORLDSOCKET_H
#define __WORLDSOCKET_H

class WorldSocket
{
public:
	WorldSocket(uint32 sessionid);
	~WorldSocket();

	void Disconnect();
	bool IsConnected();
	HEARTHSTONE_INLINE string GetIP() { return string(inet_ntoa(m_address.sin_addr)); }
	HEARTHSTONE_INLINE uint32 GetPort() { return ntohs(m_address.sin_port); }

	HEARTHSTONE_INLINE void SendPacket(WorldPacket* packet) { if(!packet) return; OutPacket(packet->GetOpcode(), (uint16)packet->size(), (packet->size() ? (const void*)packet->contents() : NULL)); }
	void __fastcall OutPacket(uint16 opcode, uint16 len, const void* data);
	HEARTHSTONE_INLINE uint32 GetSessionId() { return m_sessionId; }

protected:
	uint32 m_sessionId;
	sockaddr_in m_address;
};

void FastGUIDPack(ByteBuffer & buf, const uint64 & oldguid);

//!!! warning. This presumes that all guids can be compressed at least 1 byte
//make sure you choose highguids acordingly
unsigned int FastGUIDPack(const uint64 & oldguid, unsigned char * buffer, uint32 pos);

#endif
