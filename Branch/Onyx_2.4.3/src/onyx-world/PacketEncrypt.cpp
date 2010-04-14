/*
 * Sandshroud Onyx by Thetruecrow@Sandshroud
 *
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

static uint8 SeedKey[16] = { 0x38, 0xA7, 0x83, 0x15, 0xF8, 0x92, 0x25, 0x30, 0x71, 0x98, 0x67, 0xB1, 0x8C, 0x4, 0xE2, 0xAA };
static uint8 Length = 16;

uint8 *PacketKeyGenerator::GenerateKey(uint8 *sessionKey)
{
	uint8 *firstBuffer = new uint8[64];
	uint8 *secondBuffer = new uint8[64];
	
	memset(firstBuffer, 0x36, 64);
	memset(secondBuffer, 0x5C, 64);
		
	for (int i = 0; i < Length; i++)
	{
		firstBuffer[i] = (uint8)(SeedKey[i] ^ firstBuffer[i]);
		secondBuffer[i] = (uint8)(SeedKey[i] ^ secondBuffer[i]);
	}
			
	Sha1Hash shafirst;
			
	shafirst.UpdateData(firstBuffer, 64);
	shafirst.UpdateData(sessionKey, 40);
	shafirst.Finalize();
			
	uint8 *tempDigest = shafirst.GetDigest();
	delete[] firstBuffer;
			
	Sha1Hash shasecond;
			
	shasecond.UpdateData(secondBuffer, 64);
	shasecond.UpdateData(tempDigest, 20);
	shasecond.Finalize();
			
	uint8 *finalKey = shasecond.GetDigest();
	delete[] secondBuffer;
			
	return finalKey;
}