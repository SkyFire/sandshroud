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

#ifndef _R_SESSION_H
#define _R_SESSION_H

struct OpcodeHandler
{
	uint16 status;
	void (Session::*handler)(WorldPacket&);
};

#ifndef _GAME

#define SKIP_READ_PACKET(pckt) pckt.rpos(pckt.wpos())

extern OpcodeHandler Handlers[NUM_MSG_TYPES];

struct AccountDataEntry
{
	time_t Time;
	char * data;
	uint32 sz;
	bool bIsDirty;
};

#endif

class Session
{
public:
	friend class ClientSocket;
	friend class CharacterStorage;
	Session(uint32 id);
	~Session();

protected:
	FastQueue<WorldPacket*, Mutex> m_readQueue;
	ClientSocket * m_socket;
	uint32 m_sessionId;
	uint32 m_accountId;
	uint32 m_ClientBuild;
	uint32 m_latency;
	uint32 m_accountFlags;
	string m_GMPermissions;
	string m_accountName;
	uint32 m_language;
	uint32 m_muted;
	bool m_hasDeathKnight;
	uint8 m_highestLevel;
	bool m_loadedPlayerData;
	AccountDataEntry sAccountData[8];

public:
	bool OnMasterServer;
	bool deleted;
	static void InitHandlers();
	void Update();

	HEARTHSTONE_INLINE void QueuePacket(WorldPacket* packet) { m_readQueue.Push(packet); }
	HEARTHSTONE_INLINE ClientSocket* GetSocket() { return m_socket; }
	HEARTHSTONE_INLINE void SetSocket(ClientSocket* s) { m_socket = s; }
	HEARTHSTONE_INLINE uint32 GetAccountId() { return m_accountId; }
	HEARTHSTONE_INLINE uint32 GetSessionId() { return m_sessionId; }
	HEARTHSTONE_INLINE std::string GetAccountPermissions() { return m_GMPermissions; }
	HEARTHSTONE_INLINE std::string GetAccountName() { return m_accountName; }
	HEARTHSTONE_INLINE uint32 GetAccountFlags() { return m_accountFlags; }
	HEARTHSTONE_INLINE uint32 GetClientBuild() { return m_ClientBuild; }

	bool CanUseCommand(std::string cmdstr)
	{
		if(m_GMPermissions.find("a") != string::npos)
			return true;
		
		if(cmdstr.length() <= 1)
			return (m_GMPermissions.find(cmdstr) != string::npos);
		else
		{
			for(size_t i = 0; i < cmdstr.length(); i++)
			{
				if(m_GMPermissions.find(cmdstr[i]) == string::npos)
					return false;
			}
		}
		return true;
	}

	bool HasFlag(uint32 flag) { return (m_accountFlags & flag) != 0; }

	bool CanCreateDeathKnight()
	{
		if( m_hasDeathKnight )
			return false;

		if( m_highestLevel < 55 )
			return false;

		return true;
	}

	HEARTHSTONE_INLINE void SetAccountData(uint32 index, char* data, bool initial, uint32 sz)
	{
		ASSERT(index < 8);
		if(sAccountData[index].data)
			delete [] sAccountData[index].data;
		sAccountData[index].data = data;
		sAccountData[index].sz = sz;
		sAccountData[index].Time = UNIXTIME;
		if(!initial && !sAccountData[index].bIsDirty)		// Mark as "changed" or "dirty"
			sAccountData[index].bIsDirty = true;
		else if(initial)
			sAccountData[index].bIsDirty = false;
	}

	HEARTHSTONE_INLINE AccountDataEntry* GetAccountData(uint32 index)
	{
		ASSERT(index < 8);
		return &sAccountData[index];
	}


	HEARTHSTONE_INLINE void SendPacket(WorldPacket * data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->SendPacket(data);
	}

	HEARTHSTONE_INLINE void OutPacket(uint16 opcode, uint16 len, const void* data)
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->OutPacket(opcode, len, data);
	}

	HEARTHSTONE_INLINE void Disconnect()
	{
		if(m_socket && m_socket->IsConnected())
			m_socket->Disconnect();
	}

	void SendChars();
	void HandleNameQueryOpcode(WorldPacket & pck);
	void HandlePlayerLogin(WorldPacket & pck);
	void HandlePlayerLogoutRequest(WorldPacket & pck);
	void HandleCharacterEnum(WorldPacket & pck);
	void HandleCharacterCreate(WorldPacket & pck);
	void HandleCharacterDelete(WorldPacket & pck);
	void HandleCharacterRename(WorldPacket & pck);
	void HandleCharacterCustomize(WorldPacket & pck);
	void HandleRealmSplitQuery(WorldPacket & pck);
	void HandleQueryTimeOpcode(WorldPacket & pck);
	void HandleUpdateAccountData(WorldPacket & pck);
	void HandleRequestAccountData(WorldPacket & pck);
	void HandleReadyForAccountDataTimes(WorldPacket & pck);
	void HandleEnableMicrophoneOpcode(WorldPacket & pck);
	void HandleVoiceChatQueryOpcode(WorldPacket & pck);

	void HandleItemQuerySingleOpcode(WorldPacket & pck);
	void HandleCreatureQueryOpcode(WorldPacket & pck);
	void HandleGameObjectQueryOpcode(WorldPacket & pck);
	void HandleItemPageQueryOpcode(WorldPacket & pck);
	void HandleNpcTextQueryOpcode(WorldPacket & pck);
};

HEARTHSTONE_INLINE void CapitalizeString(string& arg)
{
	if(arg.length() == 0)
		return;

	arg[0] = toupper(arg[0]);
	for(uint32 x = 1; x < arg.size(); ++x)
		arg[x] = tolower(arg[x]);
}

HEARTHSTONE_INLINE bool VerifyName(const char * name, size_t nlen)
{
	const char * p;
	size_t i;

	static const char * bannedCharacters = "\t\v\b\f\a\n\r\\\"\'\?<>[](){}_=+-|/!@#$%^&*~`.,0123456789\0";
	static const char * allowedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for(i = 0; i < nlen; i++)
	{
		p = allowedCharacters;
		bool Continue = false;
		for(; *p != 0; ++p)
		{
			if(name[i] == *p)
			{
				Continue = true;
				break;
			}
		}
		if(Continue)
			continue;

		return false;
	}
	return true;
}

#endif


