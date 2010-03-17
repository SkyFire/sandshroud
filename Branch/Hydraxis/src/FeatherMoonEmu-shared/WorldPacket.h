/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef WOWSERVER_WORLDPACKET_H
#define WOWSERVER_WORLDPACKET_H

#include "Common.h"
#include "ByteBuffer.h"
#include "StackBuffer.h"
#include "Log.h"

class SERVER_DECL WorldPacket : public ByteBuffer
{
public:
	__inline WorldPacket() : ByteBuffer(), m_opcode(0), m_bufferPool(-1) { }
	__inline WorldPacket(uint16 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode), m_bufferPool(-1) {}
	__inline WorldPacket(size_t res) : ByteBuffer(res), m_opcode(0), m_bufferPool(-1) { }
	__inline WorldPacket(const WorldPacket &packet) : ByteBuffer(packet), m_opcode(packet.m_opcode), m_bufferPool(-1) {}

	//! Clear packet and set opcode all in one mighty blow
	__inline void Initialize(uint16 opcode )
	{
		clear();
		m_opcode = opcode;
	}

	__inline uint16 GetOpcode() const { return m_opcode; }
	__inline void SetOpcode(uint16 opcode) { m_opcode = opcode; }

	static WorldPacket* Create() { return new WorldPacket(); }

protected:
	uint16 m_opcode;
	std::vector<uint8> _storage;

public:
	int8 m_bufferPool;

	void print_storage() const
	{
		if(sLog.m_screenLogLevel >= 4)
		{
			sLog.outDebug("STORAGE_SIZE: %lu", (unsigned long)size() );
			for(uint32 i = 0; i < size(); ++i)
				sLog.outDebugInLine("%u - ", read<uint8>(i) );
			sLog.outDebug(" ");
		}
	}

	void appendPackGUID(uint64 guid)
	{
		if (_storage.size() < _wpos + sizeof(guid) + 1)
			_storage.resize(_wpos + sizeof(guid) + 1);

		size_t mask_position = wpos();
		*this << uint8(0);
		for(uint8 i = 0; i < 8; ++i)
		{
			if(guid & 0xFF)
			{
				_storage[mask_position] |= uint8(1 << i);
				*this << uint8(guid & 0xFF);
			}

			guid >>= 8;
		}
	}
};

class SERVER_DECL StackPacket : public StackBuffer
{
	uint16 m_opcode;
public:
	__inline StackPacket(uint16 opcode, uint8* ptr, uint32 sz) : StackBuffer(ptr, sz), m_opcode(opcode) { }

	//! Clear packet and set opcode all in one mighty blow
	__inline void Initialize(uint16 opcode )
	{
		StackBuffer::Clear();
		m_opcode = opcode;
	}

	uint16 GetOpcode() { return m_opcode; }
	__inline void SetOpcode(uint16 opcode) { m_opcode = opcode; }
};

#endif
