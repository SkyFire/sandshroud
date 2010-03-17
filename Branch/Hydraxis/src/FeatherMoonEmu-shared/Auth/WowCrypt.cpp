/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#include "WowCrypt.h"
#include <algorithm>

WowCrypt::WowCrypt()
{
	_initialized = false;
}

void WowCrypt::Init(uint8 *K)
{
	const uint8 SeedKeyLen = 16;
	uint8 ServerEncryptionKey[SeedKeyLen] = { 0x22, 0xBE, 0xE5, 0xCF, 0xBB, 0x07, 0x64, 0xD9, 0x00, 0x45, 0x1B, 0xD0, 0x24, 0xB8, 0xD5, 0x45 };

	HMACHash auth;
	auth.Initialize(SeedKeyLen, (uint8*)ServerEncryptionKey);
	auth.UpdateData(K, 40);
	auth.Finalize();
	uint8 *encryptHash = auth.GetDigest();

	uint8 ServerDecryptionKey[SeedKeyLen] = { 0xF4, 0x66, 0x31, 0x59, 0xFC, 0x83, 0x6E, 0x31, 0x31, 0x02, 0x51, 0xD5, 0x44, 0x31, 0x67, 0x98 };
	HMACHash auth2;
	auth2.Initialize(SeedKeyLen, (uint8*)ServerDecryptionKey);
	auth2.UpdateData(K, 40);
	auth2.Finalize();
	uint8 *decryptHash = auth2.GetDigest();

	_Decrypt.Setup((uint8*)decryptHash, 20);
	_Encrypt.Setup((uint8*)encryptHash, 20);

	uint8 encryptRotateBuffer[1024];
	memset(encryptRotateBuffer, 0, 1024);
	_Encrypt.Process((uint8*)encryptRotateBuffer, (uint8*)encryptRotateBuffer, 1024);

	uint8 decryptRotateBuffer[1024];
	memset(decryptRotateBuffer, 0, 1024);
	_Decrypt.Process((uint8*)decryptRotateBuffer, (uint8*)decryptRotateBuffer, 1024);

	_initialized = true;
}

void WowCrypt::DecryptRecv(uint8 *data, size_t len)
{
	if (!_initialized)
		return;

	_Decrypt.Process((uint8*)data, (uint8*)data, len);
}

void WowCrypt::EncryptSend(uint8 *data, size_t len)
{
	if (!_initialized)
		return;

	_Encrypt.Process((uint8*)data, (uint8*)data, len);
}

WowCrypt::~WowCrypt()
{
}
