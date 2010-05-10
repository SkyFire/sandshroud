/*
 * onyx MMORPG Server
 * Copyright (C) 2005-2008 onyx Team <http://www.onyxemu.com/>
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

#ifndef __onyx_PACKET_H
#define __onyx_PACKET_H

typedef struct  
{
	uint32 opcode;
	uint32 size;
	uint8* buffer;
	uint32 buffer_size;
	uint32 rpos;
	uint32 wpos;
	int stack;
} onyx_packet;

static onyx_packet* onyxpacket_create(uint32 opcode, uint32 size)
{
	onyx_packet * p;

	p = (onyx_packet*)vc_malloc(sizeof(onyx_packet));
	p->buffer = (uint8*)vc_malloc(size);
	p->buffer_size = size;
	p->opcode = opcode;
	p->size = size;
	p->wpos = p->rpos = 0;
	p->stack = 0;

	return p;
}

static void onyxpacket_init(uint32 opcode, uint32 size, onyx_packet *p)
{
	p->buffer = (uint8*)vc_malloc(size);
	p->buffer_size = size;
	p->opcode = opcode;
	p->size = size;
	p->wpos = p->rpos = 0;
	p->stack = 1;
}

static uint8* onyxpacket_getbuf(onyx_packet* p)
{
	return &p->buffer[p->wpos];
}

static void onyxpacket_free(onyx_packet* p)
{
	free(p->buffer);
	if( !p->stack )
		free(p);
}


/*
static uint32 onyxpacket_readu32(onyx_packet* p)
{
	uint32 orpos = p->rpos;
	p->rpos += sizeof(uint32);
	return *(uint32*)&p->buffer[orpos];
}
*/

#define DECLARE_onyxPACKET_OPERATOR(t, name) static t name(onyx_packet* p) { \
	uint32 orpos = p->rpos; \
	p->rpos += sizeof(t); \
	return *(t*)&p->buffer[orpos]; }

DECLARE_onyxPACKET_OPERATOR(uint32, onyxpacket_readu32);
DECLARE_onyxPACKET_OPERATOR(int32, onyxpacket_readi32);
DECLARE_onyxPACKET_OPERATOR(uint16, onyxpacket_readu16);
DECLARE_onyxPACKET_OPERATOR(int16, onyxpacket_readi16);
DECLARE_onyxPACKET_OPERATOR(uint8, onyxpacket_readu8);
DECLARE_onyxPACKET_OPERATOR(int8, onyxpacket_readi8);

/*
static void onyxpacket_writeu32(onyx_packet* p, uint32 v)
{
	*(uint32*)&p->buffer[p->wpos] = v;
	p->wpos += sizeof(uint32);
}
*/

#define DECLARE_onyxPACKET_WRITE_OPERATOR(t, name) static void name(onyx_packet* p, t v) { \
	*(t*)&p->buffer[p->wpos] = v; \
	p->wpos += sizeof(t); } 

DECLARE_onyxPACKET_WRITE_OPERATOR(uint32, onyxpacket_writeu32);
DECLARE_onyxPACKET_WRITE_OPERATOR(int32, onyxpacket_writei32);
DECLARE_onyxPACKET_WRITE_OPERATOR(uint16, onyxpacket_writeu16);
DECLARE_onyxPACKET_WRITE_OPERATOR(int16, onyxpacket_writei16);
DECLARE_onyxPACKET_WRITE_OPERATOR(uint8, onyxpacket_writeu8);
DECLARE_onyxPACKET_WRITE_OPERATOR(int8, onyxpacket_writei8);


#endif
