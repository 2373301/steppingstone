
#include <ace/OS.h>
#include <ace/SOCK_Connector.h>
#include "MakeGuid.h"
#include "Logger.h"
#include "protocol/msg_guid.pb.h"
#include "Packet.h"
#include "opcode.h"

MakeGuid::MakeGuid()
: m_request_no(5000)
{
}

MakeGuid::~MakeGuid()
{
}
bool MakeGuid::get_guid(EntityType entity_type, uint64 & guid)
{
	if (m_guid_info.guid_rest == 0)
	{
		if (!getGuidInfo(m_guid_info))
		{
			return false;
		}
	}

	guid = MAKE_ENTITY_GUID(entity_type, m_guid_info.seed, m_guid_info.index);
	m_guid_info.index += 1;
	m_guid_info.guid_rest -= 1;

	return true;
}

int MakeGuid::init(const string & guid_srv_addr, uint32 once_request_no)
{
	m_guid_server_addr = guid_srv_addr;
	m_request_no = once_request_no;

	if (connectSrv() == -1)
	{
		DEF_LOG_ERROR("failed to connect to guid server, addr is <%s>\n", guid_srv_addr.c_str());
		return -1;
	}

	if (this->activate() == -1)
	{
		DEF_LOG_ERROR("failed to activate MakeGuid\n");
		return -1;
	}

	return 0;
}

int MakeGuid::svc()
{
	typed::protocol::RequestGuid request_guid;
	request_guid.set_request_no(m_request_no);
	ACE_Time_Value timeout_v(1, 0);
	ACE_Message_Block recv_buffer(255);

	MAKE_NEW_PACKET(ps, SMSG_GUID_REQUEST, 0, request_guid.SerializeAsString());

	ACE_Time_Value sleep_time(0, 10 * 1000);
	ACE_Time_Value start_time;
	ACE_Time_Value diff_time;
	while (true)
	{
		ACE_OS::sleep(sleep_time);

		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_info_que_mutex, -1);
			if (m_guid_info_que.size() > 0)
			{
				continue;
			}
		}

		start_time = ACE_OS::gettimeofday();
		int send_n = m_sock_stream.send_n(ps->stream(), ps->stream_size());
		if (send_n == ps->stream_size())
		{
			bool parsed_packet = false;
			recv_buffer.crunch();
			while (!parsed_packet)
			{
				int recv_n = m_sock_stream.recv(recv_buffer.wr_ptr(), recv_buffer.space(), &timeout_v);
				if (recv_n > 0)
				{
					recv_buffer.wr_ptr(recv_n);
					parsed_packet = parsePack(recv_buffer);
					if (parsed_packet)
					{
						diff_time = ACE_OS::gettimeofday() - start_time;
						ACE_UINT64 dt = 0;
						diff_time.to_usec(dt);
						DEF_LOG_INFO("request time is : <%s>\n", boost::lexical_cast<string>(dt).c_str());
					}
				}
				else
				{
					// error
				}
			}
		}
		else
		{
			// error
		}
	}
}

bool MakeGuid::getGuidInfo(GuidInfo & guid_info)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_info_que_mutex, false);
	if (m_guid_info_que.size() > 0)
	{
		guid_info = m_guid_info_que.front();
		m_guid_info_que.pop_front();
		return true;
	}

	return false;
}

int MakeGuid::connectSrv()
{
	ACE_SOCK_Connector sock_connector;
	ACE_INET_Addr addr(m_guid_server_addr.c_str());
	ACE_Time_Value timeout_v(3, 0);
	if (sock_connector.connect(m_sock_stream, addr, &timeout_v) == -1)
	{
		return -1;
	}

	return 0;
}

bool MakeGuid::parsePack(ACE_Message_Block & message_block)
{
	if (message_block.length() < Packet::head_size())
	{
		return false;
	}

	Packet * ps = new Packet();
	
	memcpy(ps->ch_head(), message_block.rd_ptr(), ps->head_size());
	if (message_block.length() >= ps->head_size() + ps->body_size())
	{
		message_block.rd_ptr(ps->head_size());
		memcpy(ps->ch_body(), message_block.rd_ptr(), ps->body_size());
		message_block.rd_ptr(ps->body_size());
		
		parsePack(ps);
		return true;
	}
	else
	{
		delete ps;
	}

	return false;
}

void MakeGuid::parsePack(Packet * packet)
{
	std::auto_ptr<Packet> auto_pak(packet);
	if (SMSG_GUID_RETURN == packet->opcode())
	{
		typed::protocol::ReturnGuid return_guid;
		if (PARSE_PACKET(packet, &return_guid))
		{
			GuidInfo guid_info;
			guid_info.guid_no = return_guid.return_no();
			guid_info.guid_rest = return_guid.return_no();
			guid_info.index = return_guid.start_index();
			guid_info.seed = return_guid.seed_value();
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_guid_info_que_mutex, );
			m_guid_info_que.push_back(guid_info);
		}
	}
}