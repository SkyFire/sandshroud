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

#include "StdAfx.h"

initialiseSingleton(ClusterInterface);

ClusterInterfaceHandler ClusterInterface::PHandlers[IMSG_NUM_TYPES];

void ClusterInterface::InitHandlers()
{
	memset(PHandlers, 0, sizeof(void*) * IMSG_NUM_TYPES);
	PHandlers[IWSMSG_SERVER_PING]		= &ClusterInterface::HandleServerPing;
	PHandlers[IMSMSG_SERVER_PONG]		= &ClusterInterface::HandleServerPong;
	PHandlers[IWSMSG_AUTH_REQUEST]		= &ClusterInterface::HandleAuthRequest;
	PHandlers[IWSMSG_AUTH_RESULT]		= &ClusterInterface::HandleAuthResult;
	PHandlers[IWSMSG_REGISTER_RESULT]	= &ClusterInterface::HandleRegisterResult;
	PHandlers[IMSG_PLAYER_TRANSFER]		= &ClusterInterface::HandlePlayerLogin;
}

ClusterInterface::ClusterInterface()
{
	ClusterInterface::InitHandlers();
	_lastConnectTime = 0;
	m_connected = false;
	last_ping = 0;
	last_pong = 0;
	m_latency = 0;
	pingtime = 0;
}

ClusterInterface::~ClusterInterface()
{

}

const string* ClusterInterface::GetForcedPermissions(string& username)
{
	ForcedPermissionMap::iterator itr = forced_permissions.find(username);
	if(itr == forced_permissions.end())
		return NULL;

	return &itr->second;
}

string ClusterInterface::GenerateVersionString()
{
	char str[200];
	snprintf(str, 200, "Hearthstone r%u/%s-%s-%s", BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
	return string(str);
}

void ClusterInterface::ConnectionDropped()
{
	Log.Warning("ClusterInterface", "Socket disconnected, will attempt reconnect later");
	_clientSocket = NULL;
	m_connected = false;
}

void ClusterInterface::HandleServerPong(WorldPacket &pck)
{
	Log.Notice("ClusterInterface", "Pong");
	uint32 senttime;
	pck >> senttime;

	uint32 t = uint32(UNIXTIME);
	m_latency = t - senttime;
	last_pong = t;
}

void ClusterInterface::SendPing()
{
	Log.Notice("ClusterInterface", "Sending Ping");
	pingtime = getMSTime();
	WorldPacket data(IWSMSG_SERVER_PING, 4);
	data << uint32(pingtime);
	SendPacket(&data);

	last_ping = uint32(UNIXTIME);
}

void ClusterInterface::HandleServerPing(WorldPacket &pck)
{
	uint32 r;
	pck >> r;

	WorldPacket data(IMSMSG_SERVER_PONG, 4);
	data << r;
	SendPacket(&data);
}

void ClusterInterface::HandleRegisterResult(WorldPacket & pck)
{
	uint32 res;
	pck >> res;
	DEBUG_LOG("ClusterInterface", "Register Result: %u", res);
	if(!res)
	{
		Log.Error("ClusterInterface", "Registration Failed");
		_clientSocket->Disconnect();
		_clientSocket = 0;
		return;
	}
	else
	{
		string host = Config.RealmConfig.GetStringDefault( "Listen", "Host", DEFAULT_HOST );
		int wsport = Config.RealmConfig.GetIntDefault( "Listen", "WorldServerPort", DEFAULT_WORLDSERVER_PORT );

		sMaster.LWSS = new ListenSocket<WorldSocket>(host.c_str(), wsport+1);
		sMaster.listnersockcreate = sMaster.LWSS->IsOpen();

#ifdef WIN32
		if( sMaster.listnersockcreate )
			ThreadPool.ExecuteTask(sMaster.LWSS);
#endif
	}
}

void ClusterInterface::ForwardWoWPacket(uint16 opcode, uint32 size, const void * data, uint32 sessionid)
{
	bool rv;
	uint16 id = IMSMSG_WOW_PACKET;
	uint32 size2 = 10 + size;

	if(!_clientSocket)
		return;			// Shouldn't happen

	DEBUG_LOG("ForwardWoWPacket", "Forwarding %s to server", LookupName(opcode, g_worldOpcodeNames));
	_clientSocket->BurstBegin();
	_clientSocket->BurstSend((const uint8*)&id, 2);
	_clientSocket->BurstSend((const uint8*)&size2, 4);
	_clientSocket->BurstSend((const uint8*)&sessionid, 4);
	_clientSocket->BurstSend((const uint8*)&opcode, 2);
	rv = _clientSocket->BurstSend((const uint8*)&size, 4);
	if(size && rv)
		rv =_clientSocket->BurstSend((const uint8*)data, size);

	if(rv)
		_clientSocket->BurstPush();
	_clientSocket->BurstEnd();
}

void ClusterInterface::Startup()
{
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM account_forced_permissions");
	if( result != NULL )
	{
		do
		{
			string acct = result->Fetch()[0].GetString();
			string perm = result->Fetch()[1].GetString();

			HEARTHSTONE_TOUPPER(acct);
			forced_permissions.insert(make_pair(acct,perm));

		} while (result->NextRow());
		delete result;
	}
	ConnectToRealmServer();
}

void ClusterInterface::ConnectToRealmServer()
{
	if(m_connected)
		return;

	_lastConnectTime = UNIXTIME;

	int port;
	string strkey, hostname;
	if(!Config.MainConfig.GetString("Cluster", "RSHostName", &hostname) || !Config.MainConfig.GetInt("Cluster", "RSPort", &port) || !Config.MainConfig.GetString("Cluster", "Key", &strkey))
	{
		Log.Error("ClusterInterface", "Could not get necessary fields from config file. Please fix and rehash.");
		return;
	}

	/* hash the key */
	Sha1Hash k;
	k.UpdateData(strkey);
	k.Finalize();
	memcpy(key, k.GetDigest(), 20);

	Log.Notice("ClusterInterface", "Connecting to port %s:%u", hostname.c_str(), port);
	WSClient* s = ConnectTCPSocket<WSClient>(hostname.c_str(), port);
	if(!s)
	{
		Log.Error("ClusterInterface", "Could not connect to %s:%u", hostname.c_str(), port);
		return;
	}

	Log.Success("ClusterInterface", "Connected to %s:%u", hostname.c_str(), port);

	_clientSocket = s;
	m_connected = true;
	m_latency = getMSTime();
	SendPing();
}

void ClusterInterface::HandleAuthRequest(WorldPacket & pck)
{
	uint32 x;
	pck >> x;
	Log.Notice("ClusterInterface", "Incoming auth request from %s (RS build %u)", _clientSocket ? _clientSocket->GetRemoteIP().c_str() : "", x);

	WorldPacket data(IMSMSG_AUTH_REPLY, 50);
	data.append(key, 20);
	data << uint32(BUILD_REVISION);
	data << GenerateVersionString();
	SendPacket(&data);

	uint32 t = getMSTime();
	if(m_latency < t)
		m_latency = t - m_latency;
	else
		m_latency = 0;

	Log.Notice("ClusterInterface", "Latency between realm server is %u ms", m_latency);
}

void ClusterInterface::HandleAuthResult(WorldPacket & pck)
{
	uint32 res;
	pck >> res;
	DEBUG_LOG("ClusterInterface", "Auth Result: %u", res);
	if(!res)
	{
		Log.Error("ClusterInterface", "Authentication Failed");
		_clientSocket->Disconnect();
		_clientSocket = 0;
		return;
	}

	WorldPacket data(IMSMSG_REGISTER_MASTER, 4);
	data << uint32(BUILD_REVISION);
	SendPacket(&data);
}

void ClusterInterface::Update()
{
	updateMutex.Acquire();
	uint32 t = (uint32)UNIXTIME;
	if(!m_connected && t >= (_lastConnectTime + 10))
		ConnectToRealmServer();
	else if(m_connected && last_pong > 0)
	{
		if(last_pong < t && ((t - last_pong) > 60))
		{
			Log.Notice("ClusterInterface", "Connection lost");
			_clientSocket->Disconnect();
			_pckQueue.Clear();
			return;
		}
		if( (t - last_ping) > 15 )//ping every 15 seconds when connected
			SendPing();
	}

	if(!_clientSocket)
		return;

	uint16 opcode;
	WorldPacket * pck;
	while((pck = _pckQueue.Pop()))
	{
		opcode = pck->GetOpcode();
		if(opcode < IMSG_NUM_TYPES && ClusterInterface::PHandlers[opcode] != 0)
			(this->*ClusterInterface::PHandlers[opcode])(*pck);
		else
			Log.Error("ClusterInterface", "Unhandled packet %u\n", opcode);
	}
	updateMutex.Release();
}

void ClusterInterface::DestroySession(uint32 sid)
{
	WorldSession* s = NULL;
	if(s)
	{
		/* todo: replace this with an event so we don't remove from the wrong thread */
		if(s->GetPlayer())
			s->LogoutPlayer(true);

		delete s->GetSocket();
		sWorld.RemoveGlobalSession(s);
		sWorld.RemoveSession(sid);
	}
}

void ClusterInterface::HandlePlayerLogin(WorldPacket & pck)
{
	/* player x logging into instance y */
	uint32 Guid, AccountId, Account_Flags, sessionid, ClientBuild;
	string GMPermissions, accountname;
	pck >> Guid >> AccountId >> Account_Flags >> sessionid >> GMPermissions >> accountname >> ClientBuild;
	DEBUG_LOG( "ClusterInterface", "Recvd Player Login Information Cluster(%u/%u/%u)", Guid, sessionid, sessionid );

	/* create the session */
	WorldSession* s = sWorld.FindSession(AccountId);
	if(s != NULL)
		s->Disconnect();
	/* create the socket */
	WorldSocket* so = new WorldSocket(sessionid);
	if (s == NULL)
		s = new WorldSession(AccountId, accountname, so);
	else
		s->SetSocket(so);

	sWorld.AddSession(s);

	for(uint8 i = 0; i < 8; i++)
		s->SetAccountData(i, NULL, true, 0);

	if(sWorld.m_useAccountData)
	{
		if(pck.rpos() != pck.wpos())
		{
			uint32 size = 0;
			for(uint8 i = 0; i < 8; i++)
			{
				if(pck.rpos()+4 > pck.wpos())
					break; // Out of packet.

				pck >> size;
				if(size)
				{
					char* data = new char[size];
					pck.read(((uint8*)data), size);
					s->SetAccountData(i, data, true, size);
				}
			}
		}
	}

	bool login_result = false;
	if(Guid)
		login_result = s->ClusterTryPlayerLogin(Guid, ClientBuild, GMPermissions, Account_Flags);
	if(login_result)
	{
		/* login was ok. send a message to the realm server telling him to distribute our info to all other realm server */
		WorldPacket data(IMSG_PLAYER_TRANSFER, 5);
		data << Guid << sessionid << uint8(1);
		SendPacket(&data);
	}
	else
	{
		/* for some reason the login failed */
		WorldPacket data(IMSG_PLAYER_TRANSFER, 5);
		data << Guid << sessionid << uint8(0);
		SendPacket(&data);

		/* destroy the session */
		DestroySession(sessionid);
	}
}

bool WorldSession::ClusterTryPlayerLogin(uint64 Guid, uint32 ClientBuild, string GMPermissions, uint32 Account_Flags)
{
	DEBUG_LOG( "WorldSession"," Recvd Player Logon Message" );
	if(objmgr.GetPlayer((uint32)Guid) != NULL || m_loggingInPlayer || _player)
	{
		// A character with that name already exists 0x3E
		uint8 respons = CHAR_LOGIN_DUPLICATE_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return false;
	}

	_player = new Player((uint32)Guid);
	ASSERT(_player);
	_player->Init();

	permissioncount = 0;//just to make sure it's 0
	SetClientBuild(ClientBuild);
	LoadSecurity(GMPermissions);
	SetAccountFlags(Account_Flags);

	_player->SetSession(this);
	m_bIsWLevelSet = false;

	DEBUG_LOG("WorldSession", "Async loading player %u", Guid);
	m_loggingInPlayer = _player;
	_player->LoadFromDB(Guid);
	return true;
}
