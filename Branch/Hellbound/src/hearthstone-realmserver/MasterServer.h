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

typedef void(MasterServer::*WServerHandler)(WorldPacket &);

class MasterServer
{
	static WServerHandler PHandlers[IMSG_NUM_TYPES];
	MasterServerSocket* m_socket;
	FastQueue<WorldPacket*, Mutex> m_recvQueue;
	Mutex UpdateMutex;

public:
	static void InitHandlers();
	MasterServer(MasterServerSocket * s);
	~MasterServer();

	HEARTHSTONE_INLINE void SendPacket(WorldPacket * data)
	{
		if(m_socket)
			m_socket->SendPacket(data);
	}

	HEARTHSTONE_INLINE void SendWoWPacket(Session * from, WorldPacket * data)
	{
		if(m_socket)
			m_socket->SendWoWPacket(from, data);
	}

	HEARTHSTONE_INLINE void QueuePacket(WorldPacket * data)
	{
		m_recvQueue.Push(data);
	}

	void Update();
	void SendPong();

protected:
	bool removed;
	uint32 last_ping;

	/* packet handlers */
	void HandleServerPing(WorldPacket & pck);
	void HandleWoWPacket(WorldPacket &pck);
	void HandlePlayerLoginResponse(WorldPacket &pck);
};
