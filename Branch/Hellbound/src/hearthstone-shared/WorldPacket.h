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
	__inline WorldPacket(uint32 opcode, size_t res) : ByteBuffer(res), m_opcode(opcode), m_bufferPool(-1) {}
	__inline WorldPacket(size_t res) : ByteBuffer(res), m_opcode(0), m_bufferPool(-1) { }
	__inline WorldPacket(const WorldPacket &packet) : ByteBuffer(packet), m_opcode(packet.m_opcode), m_bufferPool(-1) {}

	//! Clear packet and set opcode all in one mighty blow
	__inline void Initialize(uint32 opcode )
	{
		clear();
		m_opcode = opcode;
	}

	__inline uint32 GetOpcode() const { return m_opcode; }
	__inline void SetOpcode(uint32 opcode) { m_opcode = opcode; }

protected:
	uint32 m_opcode;

public:
	int8 m_bufferPool;

	void print_storage() const
	{
		if(sLog.m_screenLogLevel >= 5)
		{
			sLog.outDebugInLine("STORAGE_SIZE: %lu\n", ulong(size()) );
			sLog.outDebugInLine("START: ");
			for(uint32 i = 0; i < size(); ++i)
				sLog.outDebugInLine("%u - ", read<uint8>(i) );
			sLog.outDebugInLine("END\n");
		}
	}
};

class SERVER_DECL StackPacket : public StackBuffer
{
	uint32 m_opcode;
public:
	__inline StackPacket(uint32 opcode, uint8* ptr, uint32 sz) : StackBuffer(ptr, sz), m_opcode(opcode) { }

	//! Clear packet and set opcode all in one mighty blow
	__inline void Initialize( uint32 opcode )
	{
		StackBuffer::Clear();
		m_opcode = opcode;
	}

	uint32 GetOpcode() { return m_opcode; }
	__inline void SetOpcode(uint32 opcode) { m_opcode = opcode; }
};

#endif
