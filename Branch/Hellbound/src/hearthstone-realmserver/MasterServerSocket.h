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

class Session;
class MasterServer;

class MasterServerSocket : public Socket
{
	bool _authenticated;
	uint32 _remaining;
	uint16 _cmd;
	MasterServer* Master;
public:
	uint32 m_id;

	MasterServerSocket(SOCKET fd);
	~MasterServerSocket();

	void SendPacket(WorldPacket * pck);
	void SendPacket(uint16 opcode, uint32 size, const void* data);
	void SendWoWPacket(Session * from, WorldPacket * pck);
	void OnRead();

	void HandleAuthRequest(WorldPacket & pck);
	void HandleRegisterMaster(WorldPacket & pck);
	void OnConnect();

};
