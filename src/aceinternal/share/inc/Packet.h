
#ifndef PACKET_HPP
#define PACKET_HPP

#include "typedef.h"
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <google/protobuf/message.h>
#include "Share_def.h"
#include "Packet.h"

#define MAKE_NEW_PACKET(packet_ins, op_code, guid, stream_value)		Packet * packet_ins = new Packet(op_code, guid, stream_value)

#define MAKE_NEW_PACKET_NO_DEC(packet, op_code, guid, stream_value)		packet = new Packet(op_code, guid, stream_value)

#define STAT_PACKET_DEC() \
	public:	\
	static uint64 m_new_count;	\
	static uint64 m_delete_count;	\
	static ACE_Thread_Mutex m_new_mutex;	\
	static ACE_Thread_Mutex m_delete_mutex;	

#define STAT_PACKET_IMP(class_name) \
	uint64 class_name::m_new_count = 0;	\
	uint64 class_name::m_delete_count = 0;	\
	ACE_Thread_Mutex class_name::m_new_mutex;	\
	ACE_Thread_Mutex class_name::m_delete_mutex;	

#define STAT_PACKET_INC_NEW()	\
	{	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_new_mutex, ) \
	++m_new_count;	\
	}

#define STAT_PACKET_INC_DEL() \
	{	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_delete_mutex, ) \
	++m_delete_count;	\
	}

class Packet;
struct PackInfo;

typedef vector<Packet *> PacketVec_t;

typedef queue<Packet *> PacketQue_t;

#define MSG_TYPE	google::protobuf::Message
#define MSG_NAME			const std::string

#define PARSE_PACKET(src, msg) parsePacket(src, (MSG_TYPE *)msg)

bool SHARE_EXOPRT parsePacket(Packet * packet, MSG_TYPE * msg);

bool SHARE_EXOPRT parsePacket(const string & stream, MSG_TYPE * msg);

void SHARE_EXOPRT cleanPacketVec(PacketVec_t & packet_vec);

void SHARE_EXOPRT cleanPacketQue(PacketQue_t & packet_que);

void SHARE_EXOPRT cleanPackInfoVec(vector<PackInfo *> & pack_vec);

struct SHARE_EXOPRT PackInfo 
{
	PackInfo();

	~PackInfo();

	PackInfo(uint32 opcode_value, uint64 guid_value, MSG_TYPE * msg_value);

	PackInfo(uint32 opcode_value, uint64 guid_value, MSG_TYPE * msg_value, int attach_data);

private:
	PackInfo (const PackInfo & rsh){}

	STAT_PACKET_DEC();
public:

	uint32 opcode;
	uint64 guid;
	MSG_TYPE * msg;
	int    attach_data;
	void * owner;
};

#pragma pack(1)

struct SHARE_EXOPRT Head
{
	Head()
	: opcode(0)
	, size(0)
	, guid(0)
	{}

	Head(uint16 op, uint16 si, uint64 gu)
	: opcode(op)
	, size(si)
	, guid(gu)
	{}

	uint16	opcode;
	uint16	size;
	uint64	guid;
//	uint8	compress;
};

#pragma pack(8)

class SHARE_EXOPRT Packet
{
public:
	Packet();

	Packet(uint16 opcode, uint64 guid, const string & buffer);

	~Packet();

public:
	uint16 opcode()
	{
		return m_head.opcode;
	}

	static uint16 head_size()
	{
		return m_head_size;
	}

	uint16 body_size()
	{
		return m_head.size;
	}

	uint16 stream_size()
	{
		return m_head_size + m_head.size;
	}

	uint64 guid()
	{
		return m_head.guid;
	}

public:
	// call these methods of receiving packet
	char * ch_head();

	char * ch_body();

	// call this method of sending packet
	char * stream();

public:
	void setOwner(void * owner);

	void * getOwner();
public:
	static string empty_stream;

private:
	static uint16 m_head_size;

	STAT_PACKET_DEC();
private:
	Head m_head;

	char * m_buffer;

	void * m_owner;
};

#endif