/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef AUTHSOCKET_H
#define AUTHSOCKET_H

#include "AccountCache.h"
#include "AuthStructs.h"

class LogonCommServerSocket;
struct Patch;
class PatchJob;

class AuthSocket : public Socket
{
	friend class LogonCommServerSocket;
public:

	///////////////////////////////////////////////////
	// Netcore shit
	//////////////////////////
	AuthSocket(SOCKET fd);
	~AuthSocket();

	void OnRead();

	///////////////////////////////////////////////////
	// Client Packet Handlers
	//////////////////////////

	void HandleChallenge();
	void HandleProof();
	void HandleRealmlist();
	void HandleReconnectChallenge();
	void HandleReconnectProof();
	void HandleTransferAccept();
	void HandleTransferResume();
	void HandleTransferCancel();

	///////////////////////////////////////////////////
	// Server Packet Builders
	//////////////////////////

	void SendChallengeError(uint8 Error);
	void SendProofError(uint8 Error, uint8 * M2);
	FEATHERMOONEMU_INLINE sAuthLogonChallenge_C * GetChallenge() { return &m_challenge; }
	FEATHERMOONEMU_INLINE void SendPacket(const uint8* data, const uint16 len) { Send(data, len); }
	void OnDisconnect();
	FEATHERMOONEMU_INLINE time_t GetLastRecv() { return last_recv; }
	bool removedFromSet;
	FEATHERMOONEMU_INLINE uint32 GetAccountID() { return m_account ? m_account->AccountId : 0; }

protected:

	sAuthLogonChallenge_C m_challenge;
	Account * m_account;
	bool m_authenticated;
	std::string AccountName;

	//////////////////////////////////////////////////
	// Authentication BigNumbers
	/////////////////////////
	BigNumber N, s, g, v;
	BigNumber b, B;
	BigNumber K;
	BigNumber rs;

	//////////////////////////////////////////////////
	// Session Key
	/////////////////////////

	BigNumber m_sessionkey;
	time_t last_recv;

	//////////////////////////////////////////////////////////////////////////
	// Patching stuff
	//////////////////////////////////////////////////////////////////////////
public:
	Patch * m_patch;
	PatchJob * m_patchJob;
};

#endif
