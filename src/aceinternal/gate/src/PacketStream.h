#ifndef _PACKAGESTREAM_HPP
#define _PACKAGESTREAM_HPP

#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
//#include <google/protobuf/io/gzip_stream.h>
#include "typedef.h"
#include "GateLogger.h"

#define STAT_NUMBER_DEC() \
	public:	\
	static uint64 m_new_count;	\
	static uint64 m_delete_count;	\
	static ACE_Thread_Mutex m_new_mutex;	\
	static ACE_Thread_Mutex m_delete_mutex;	

#define STAT_NUMBER_IMP(class_name) \
	uint64 class_name::m_new_count = 0;	\
	uint64 class_name::m_delete_count = 0;	\
	ACE_Thread_Mutex class_name::m_new_mutex;	\
	ACE_Thread_Mutex class_name::m_delete_mutex;	

#define STAT_NUMBER_INC_NEW()	\
	{	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_new_mutex, ) \
	++m_new_count;	\
	}

#define STAT_NUMBER_INC_DEL() \
	{	\
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_delete_mutex, ) \
	++m_delete_count;	\
	}

using namespace std;

typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> ACE_SVC_HANDLER;

#pragma pack(1)

struct Header {
	Header (void) : opcode (0), size (0), guid(0L) {}
	Header (uint16 o, uint16 s, uint64 g) : opcode (o), size (s), guid(g){}

	uint16 opcode;  /// 消息操作码
	uint16 size;    /// 消息正文大小
	uint64 guid;    /// 需要发往目的端的GUID

	static uint32 getTempGuid()
	{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, temp_guid_mutex, -1);
		temp_guid = ++temp_guid % 0xFFFFFFF;
		return temp_guid;
	}
	static uint32 temp_guid;
	static ACE_Thread_Mutex temp_guid_mutex;
};

struct ClientHeader 
{
	ClientHeader (void) : opcode (0), size (0) {}
	ClientHeader (uint16 o, uint16 s) : opcode (o), size (s){}

	uint16 opcode;  /// 消息操作码
	uint16 size;    /// 消息正文大小
};

#pragma pack(8)

//#define STAT_NEW_COUNT \
//	{	\
//		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_new_mutex, ) \
//		++m_new_count;	\
//	}
//
//#define STAT_DELETE_COUNT \
//	{	\
//		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_delete_mutex, ) \
//		++m_delete_count;	\
//	}

/***
 * @class PacketStream
 *
 * @brief 网络包包装类，包括对于包头和包体的管理
 */
class PacketStream
{
public:
	PacketStream (void)
	: m_buffer (NULL)
	, m_total_size (0)
	, m_total_buffer(NULL)
	, m_svc_handler(NULL)
	, m_role_location(0)
	, m_line_no(0)
	{
		m_total_size = m_head_size;
		STAT_NUMBER_INC_NEW();
		//GATE_LOG_DEBUG("new packet, opcode is <%d>, size is <%d>\n", m_header.opcode, m_header.size);
	}

	/// 构造一个完整的协议包
	PacketStream (uint16 opcode, uint64 guid, const std::string &msg)
	: m_header(opcode, msg.length(), guid)
	, m_svc_handler(NULL)
	, m_role_location(0)
	, m_line_no(0)
	{
		//STAT_NEW_COUNT
		//if ((m_new_count % 100000) == 0)
		//{
		//	GATE_LOG_INFO(ACE_TEXT("packet new count number is <%s>\n"), boost::lexical_cast<string>(m_new_count).c_str());
		//}
		m_total_size = head_size() + msg.length();
		m_total_buffer = new char[m_total_size];
		memcpy (m_total_buffer, &m_header, m_head_size);
		memcpy (m_total_buffer + head_size (), msg.c_str (), msg.length ());
		m_buffer = m_total_buffer + m_head_size;

		STAT_NUMBER_INC_NEW();

		//GATE_LOG_DEBUG("new packet, opcode is <%d>, size is <%d>\n", m_header.opcode, m_header.size);
	}

	~PacketStream (void)
	{
		//STAT_DELETE_COUNT
		//if ((m_delete_count % 100000) == 0)
		//{
		//	GATE_LOG_INFO(ACE_TEXT("packet delete count number is <%s>\n"), boost::lexical_cast<string>(m_delete_count).c_str());
		//}
		delete []m_total_buffer;
		m_total_buffer = NULL;
		m_buffer = NULL;

		STAT_NUMBER_INC_DEL();

		//GATE_LOG_DEBUG("delete packet, opcode is <%d>, size is <%d>\n", m_header.opcode, m_header.size);
	}

	void set_header(uint16 opcode, uint16 body_size, uint64 guid)
	{
		m_header.opcode = opcode;
		m_header.size = body_size;
		m_header.guid = guid;
	}

	/// 获取操作码
	uint32 opcode (void) const
	{
		return m_header.opcode;
	}

	/// 获取包头大小
	uint32 head_size (void) const
	{
		return m_head_size;
	}

	/// 获取实体guid
	uint64 guid (void) const
	{
		return m_header.guid;
	}

	void guid(uint64 guid)
	{
		m_header.guid = guid;
	}

	/// 获取包体大小
	uint32 body_size (void) const
	{
		return m_header.size;
	}

	/// 获取包头加包体一共得大小
	uint32 stream_size (void) const
	{
		return m_total_size;
	}

	/// 当从流中接收一个头部结构时调用此方法
	char * re_head (void)
	{
		return reinterpret_cast<char*>(&m_header);
	}

	/// 调用此方法从套接口接收一个正文流
	char * re_body (void)
	{
		if ((m_total_buffer == NULL) 
		 && (m_header.size > 0))
		{
			m_total_size = m_head_size + m_header.size;
			m_total_buffer = new char[m_total_size];
			m_buffer = m_total_buffer + m_head_size;
		}
		return m_buffer;
	}

	/// 提取接收到的消息体
	const char * pe_body (void) const
	{
		return m_buffer;
	}

	/// 解析包体
	bool parse_protocol (google::protobuf::Message &msg) const
	{
		google::protobuf::io::ArrayInputStream istream (m_buffer, m_header.size);

		//if (! m_header.compress)
		if (true)
		{
			if (msg.ParseFromZeroCopyStream (&istream))
				return true;
			else
				return false;
		}
		//else
		//{
		//	uint8 feature = m_header.compress & 0x0F;
		//	if ((feature == 1) || (feature == 2))
		//	{
		//		/// 解析当前使用的压缩格式
		//		google::protobuf::io::GzipInputStream::Format format = 
		//			google::protobuf::io::GzipInputStream::Format (feature);

		//		google::protobuf::io::GzipInputStream gstream (&istream, format);
		//		if (msg.ParseFromZeroCopyStream (&gstream))
		//			return true;
		//		else
		//			return false;
		//	}
		//	else
		//	{
		//		return false;
		//	}
		//}
	}

	/// 发送时应该调用此方法来将组装好的协议一块发送
	const char * stream (void) const
	{
		if (NULL != m_total_buffer)
		{
			memcpy (m_total_buffer, &m_header, m_head_size);
			return m_total_buffer;
		}
		else
		{
			return reinterpret_cast<const char*>(&m_header);
		}
	}

	/// 重设包信息
	void reset()
	{
		delete [] m_total_buffer;
		m_total_buffer = NULL;
		m_buffer = NULL;
	}

	void resetBody(const char * body_buffer, size_t body_len)
	{
		if (body_len > body_size())
		{
			delete [] m_total_buffer;

			m_header.size = body_len;
			m_total_size = m_head_size + body_len;
			m_total_buffer = new char[m_total_size];
		}
		else
		{
			m_header.size = body_len;
			m_total_size = m_head_size + body_len;
		}

		memcpy (m_total_buffer, &m_header, m_head_size);
		memcpy (m_total_buffer + m_head_size, body_buffer, body_len);
		m_buffer = m_total_buffer + m_head_size;
	}

	void setEventHandler(ACE_SVC_HANDLER * svc_handler)
	{
		m_svc_handler = svc_handler;
	}

	ACE_SVC_HANDLER * getEventHandler()
	{
		return m_svc_handler;
	}

	void setRoleLocation(uint64 role_location)
	{
		m_role_location = role_location;
	}

	uint64 getRoleLocation()
	{
		return m_role_location;
	}

	void setLineNO(int line_no)
	{
		m_line_no = line_no;
	}

	int getLineNO()
	{
		return m_line_no;
	}

	/// 克隆一份
	PacketStream * clone()
	{
		PacketStream * ps = new PacketStream();
		ps->m_header = this->m_header;
		ps->m_total_size = this->m_total_size;
		ps->m_svc_handler = this->m_svc_handler;
		ps->m_total_buffer = new char[ps->m_total_size];
		ps->m_role_location = m_role_location;
		ps->m_line_no = m_line_no;

		memcpy(ps->m_total_buffer, this->m_total_buffer, ps->m_total_size);
		ps->m_buffer = ps->m_total_buffer + m_head_size;
		return ps;
	}

protected:
	Header m_header;
	char * m_buffer;
	char * m_total_buffer;
	int m_total_size;
	static int m_head_size;
	ACE_SVC_HANDLER * m_svc_handler;
	uint64 m_role_location;
	int    m_line_no;

	STAT_NUMBER_DEC();
	//static uint64 m_new_count;
	//static uint64 m_delete_count;
	//static ACE_Thread_Mutex m_new_mutex;
	//static ACE_Thread_Mutex m_delete_mutex;
};

#endif