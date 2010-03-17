/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifdef CLUSTERING
#ifndef _WORKERSERVER_CLIENT_H
#define _WORKERSERVER_CLIENT_H

class WSClient : public Socket
{
	bool _authenticated;
	uint32 _remaining;
	uint16 _cmd;
public:
	WSClient(SOCKET fd);
	~WSClient();

	void SendPacket(WorldPacket * data);
	void OnRead();
	void OnConnect();
};

#endif
#endif
