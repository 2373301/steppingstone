
#include "Packet.h"

uint16 Packet::m_head_size = sizeof(Head);

string Packet::empty_stream = "";

STAT_PACKET_IMP(PackInfo);

STAT_PACKET_IMP(Packet);

bool parsePacket(Packet * packet, GOOGLE_MESSAGE_TYPE * msg)
{
	if (msg->ParseFromArray(packet->ch_body(), packet->body_size()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool parsePacket(const string & stream, GOOGLE_MESSAGE_TYPE * msg)
{
	if (msg->ParseFromString(stream))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void cleanPacketVec(PacketVec_t & packet_vec)
{
	for (PacketVec_t::iterator it = packet_vec.begin(); it != packet_vec.end(); ++it)
	{
		delete *it;
	}
}

void cleanPacketQue(PacketQue_t & packet_que)
{
	while (packet_que.size() > 0)
	{
		delete packet_que.front();
		packet_que.pop();
	}
}

void SHARE_EXOPRT cleanPackInfoVec(vector<PackInfo *> & pack_vec)
{
	for (vector<PackInfo *>::iterator it = pack_vec.begin(); it != pack_vec.end(); ++it)
	{
		delete *it;
	}
}

PackInfo::PackInfo()
: opcode(0)
, guid(0)
, msg(NULL)
, attach_data(0)
, owner(NULL)
{
	STAT_PACKET_INC_NEW();
}

PackInfo::~PackInfo()
{
	if (NULL != msg)
	{
		delete msg;
	}
	STAT_PACKET_INC_DEL()
}

PackInfo::PackInfo(uint32 opcode_value, uint64 guid_value, GOOGLE_MESSAGE_TYPE * msg_value)
: opcode(opcode_value)
, guid(guid_value)
, msg(msg_value)
, attach_data(0)
, owner(NULL)
{
	STAT_PACKET_INC_NEW();
}

PackInfo::PackInfo(uint32 opcode_value, uint64 guid_value, GOOGLE_MESSAGE_TYPE * msg_value, int attach_d)
: opcode(opcode_value)
, guid(guid_value)
, msg(msg_value)
, attach_data(attach_d)
, owner(NULL)
{
	STAT_PACKET_INC_NEW();
}

Packet::Packet()
: m_buffer(NULL)
, m_owner(NULL)
{
	STAT_PACKET_INC_NEW();
}

Packet::Packet(uint16 opcode, uint64 guid, const string & buffer)
: m_head(opcode, (uint16)buffer.size(), guid)
, m_buffer(NULL)
, m_owner(NULL)
{
	STAT_PACKET_INC_NEW();

	m_buffer = new char [buffer.size() + m_head_size];
	memcpy(m_buffer, &m_head, m_head_size);
	memcpy(m_buffer + m_head_size, buffer.c_str(), buffer.size());
}

Packet::~Packet()
{
	STAT_PACKET_INC_DEL();

	if (NULL != m_buffer)
	{
		delete [] m_buffer;
	}
}

char * Packet::ch_head()
{
	return (char *)&m_head;
}

char * Packet::ch_body()
{
	if (NULL == m_buffer)
	{
		m_buffer = new char [m_head.size + m_head_size];
		memset(m_buffer, 0, m_head.size + m_head_size);
		memcpy(m_buffer, &m_head, m_head_size);
	}
	return m_buffer + m_head_size;
}

char * Packet::stream()
{
	return m_buffer;
}

void Packet::setOwner(void * owner)
{
	m_owner = owner;
}

void * Packet::getOwner()
{
	return m_owner;
}
