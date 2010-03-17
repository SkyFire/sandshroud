/*
 * FeatherMoonEmu by Crow@Sandshroud
 * Sandshroud <http://www.Sandshroud.servegame.org/>
 *
 */

#ifndef __FEATHERMOONEMU_PACKET_H
#define __FEATHERMOONEMU_PACKET_H

typedef struct  
{
	uint32 opcode;
	uint32 size;
	uint8* buffer;
	uint32 buffer_size;
	uint32 rpos;
	uint32 wpos;
	int stack;
} feathermoonemu_packet;

static feathermoonemu_packet* feathermoonemupacket_create(uint32 opcode, uint32 size)
{
	feathermoonemu_packet * p;

	p = (feathermoonemu_packet*)vc_malloc(sizeof(feathermoonemu_packet));
	p->buffer = (uint8*)vc_malloc(size);
	p->buffer_size = size;
	p->opcode = opcode;
	p->size = size;
	p->wpos = p->rpos = 0;
	p->stack = 0;

	return p;
}

static void feathermoonemupacket_init(uint32 opcode, uint32 size, feathermoonemu_packet *p)
{
	p->buffer = (uint8*)vc_malloc(size);
	p->buffer_size = size;
	p->opcode = opcode;
	p->size = size;
	p->wpos = p->rpos = 0;
	p->stack = 1;
}

static uint8* feathermoonemupacket_getbuf(feathermoonemu_packet* p)
{
	return &p->buffer[p->wpos];
}

static void feathermoonemupacket_free(feathermoonemu_packet* p)
{
	free(p->buffer);
	if( !p->stack )
		free(p);
}


/*
static uint32 feathermoonemupacket_readu32(feathermoonemu_packet* p)
{
	uint32 orpos = p->rpos;
	p->rpos += sizeof(uint32);
	return *(uint32*)&p->buffer[orpos];
}
*/

#define DECLARE_FEATHERMOONEMUPACKET_OPERATOR(t, name) static t name(feathermoonemu_packet* p) { \
	uint32 orpos = p->rpos; \
	p->rpos += sizeof(t); \
	return *(t*)&p->buffer[orpos]; }

DECLARE_FEATHERMOONEMUPACKET_OPERATOR(uint32, feathermoonemupacket_readu32);
DECLARE_FEATHERMOONEMUPACKET_OPERATOR(int32, feathermoonemupacket_readi32);
DECLARE_FEATHERMOONEMUPACKET_OPERATOR(uint16, feathermoonemupacket_readu16);
DECLARE_FEATHERMOONEMUPACKET_OPERATOR(int16, feathermoonemupacket_readi16);
DECLARE_FEATHERMOONEMUPACKET_OPERATOR(uint8, feathermoonemupacket_readu8);
DECLARE_FEATHERMOONEMUPACKET_OPERATOR(int8, feathermoonemupacket_readi8);

/*
static void feathermoonemupacket_writeu32(feathermoonemu_packet* p, uint32 v)
{
	*(uint32*)&p->buffer[p->wpos] = v;
	p->wpos += sizeof(uint32);
}
*/

#define DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(t, name) static void name(feathermoonemu_packet* p, t v) { \
	*(t*)&p->buffer[p->wpos] = v; \
	p->wpos += sizeof(t); } 

DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(uint32, feathermoonemupacket_writeu32);
DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(int32, feathermoonemupacket_writei32);
DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(uint16, feathermoonemupacket_writeu16);
DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(int16, feathermoonemupacket_writei16);
DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(uint8, feathermoonemupacket_writeu8);
DECLARE_FEATHERMOONEMUPACKET_WRITE_OPERATOR(int8, feathermoonemupacket_writei8);


#endif
