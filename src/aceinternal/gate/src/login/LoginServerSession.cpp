
#include <ace/OS_NS_netdb.h>
#include <ace/os_include/netinet/os_tcp.h>
#include "LoginServerSession.h"
#include "ManageConfig.h"
#include "ManageClientValidation.h"
#include "ManageStat.h"
#include "ManageLoginStream.h"

#define BUFFER_SIZE 10240

LoginServerSession::LoginServerSession()
{
	m_input_msg_block.init(BUFFER_SIZE);
	m_output_msg_block.init(BUFFER_SIZE);
}

LoginServerSession::~LoginServerSession()
{
}

int LoginServerSession::open(void * p)
{
	GATE_LOG_DEBUG(ACE_TEXT("Enter LoginServerSession::open\n"));

	if (super::open(p) == -1)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call super::open in LoginServerSession::open, the last error is : <%d>\n"), ACE_OS::last_error());
		return -1;
	}

	this->peer().enable(ACE_NONBLOCK);

	int flag = 1;
	this->peer().set_option(IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));

	this->reactor()->remove_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

	ManageLoginStream::instance()->handleLoginSession(this);

	ACE_TCHAR peer_name[MAXHOSTNAMELEN];
	ACE_INET_Addr peer_addr;
	if (this->peer().get_remote_addr(peer_addr) == 0 &&
		peer_addr.addr_to_string(peer_name, MAXHOSTNAMELEN) == 0)
	{
		GATE_LOG_DEBUG(ACE_TEXT("Get new login connection, the ip is : %s\n"), peer_name);
	}
	return 0;
}

int LoginServerSession::handle_input(ACE_HANDLE fd)
{
	return rd_stream();
}

int LoginServerSession::handle_output(ACE_HANDLE fd)
{
	//if (m_report_current_online_accounts)
	//{
	//	ManageClientBeat::ClientSessionList online_client;
	//	ManageClientBeat::instance()->getOnlineClient(online_client);

	//	std::list<LogoutAccount> logout_wait_from_gs;
	//	ManageClientValidation::instance()->getOnlineClient(logout_wait_from_gs);
	//	m_report_current_online_accounts = false;
	//	reportOnlineClient(online_client, logout_wait_from_gs);
	//}
	return 0;
}

int LoginServerSession::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	return 0;
}

//void LoginServerSession::handleOutputPacket(PacketStream * packet_stream)
//{
//	//if (m_report_current_online_accounts)
//	//{
//	//	delete packet_stream;
//	//	return;
//	//}
//
//	{
//		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_output_packet_mutex, );
//		m_output_packet_list.push_back(packet_stream);
//	}
//
//	if (!m_is_writing)
//	{
//		this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
//	}
//}

int LoginServerSession::rd_stream()
{
	int r = this->peer().recv(m_input_msg_block.wr_ptr(), m_input_msg_block.space());
	if (r > 0)
	{
		m_input_msg_block.wr_ptr(r);
		parseInputPacket();
		return 0;
		//ManageClientValidation::instance()->handlePackage(m_recv_buffer);
	}
	else if (0 == r)
	{
		return 2;
		GATE_LOG_ERROR("the login server is closed\n");
	}
	else
	{
		int last_error = ACE_OS::last_error();
		if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error) || (ETIME == last_error))
		{
			// ok
			return 1;
		}
		else
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to recv data from login server, last error is <%d>\n"), ACE_OS::last_error());
			return 2;
		}
	}

	return 0;
}

int LoginServerSession::processWrite(PacketStreamQue_t & output_packet_que)
{
	m_output_msg_block.crunch();

	PacketStream * packet = NULL;

	while (output_packet_que.size() > 0)
	{
		packet = output_packet_que.front();
		if (packet->stream_size() <= m_output_msg_block.space())
		{
			m_output_msg_block.copy(packet->stream(), packet->stream_size());

			delete packet;
			packet = NULL;
			output_packet_que.pop();
		}
		else
		{
			break;
		}
	}

	if (m_output_msg_block.length() == 0)
	{
		return 1;
	}

	int send_n = this->peer().send(m_output_msg_block.rd_ptr(), m_output_msg_block.length());

	if (send_n > 0)
	{
		m_output_msg_block.rd_ptr(send_n);
	}
	else
	{
		int last_error = ACE_OS::last_error();
		if ((EWOULDBLOCK == last_error) || (EINTR == last_error) || (EAGAIN == last_error))
		{
			// ok
			return 1;
		}
		else
		{
			return 2;
		}
	}

	return 0;
}

void LoginServerSession::parseInputPacket()
{
	PacketStream * ps = NULL;
	int msg_length = 0;
//	try
	{
		msg_length = m_input_msg_block.length();
		while(true)
		{
			if (m_input_msg_block.length() < sizeof(Header))
			{
				break;
			}

			ps = new PacketStream();
			memcpy(ps->re_head(), m_input_msg_block.rd_ptr(), ps->head_size());
			if (m_input_msg_block.length() >= ps->head_size() + ps->body_size())
			{
				m_input_msg_block.rd_ptr(ps->head_size());
				if (ps->body_size() > 0)
				{
					memcpy(ps->re_body(), m_input_msg_block.rd_ptr(), ps->body_size());
					m_input_msg_block.rd_ptr(ps->body_size());
				}
				else
				{
					// do nothing
				}

//				ManageStat::instance()->statClientInputTraffic(ps->stream_size(), 1);
				ManageClientValidation::instance()->handlePackage(ps);
				msg_length = m_input_msg_block.length();
			}
			else
			{
				delete ps;
				break;
			}
		}

		if (m_input_msg_block.rd_ptr() != m_input_msg_block.base())
		{
			msg_length = m_input_msg_block.length();
			if (msg_length > 0)
			{
				memcpy(m_input_msg_block.base(), m_input_msg_block.rd_ptr(), msg_length);
			}
			m_input_msg_block.rd_ptr(m_input_msg_block.base());
			m_input_msg_block.wr_ptr(m_input_msg_block.base() + msg_length);
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in LoginServerSession::parseInputPacket, last error is <%d>\n"), ACE_OS::last_error());
	//}
}

void LoginServerSession::reportOnlineClient(ManageClientBeat::ClientSessionList & online_client, std::list<LogoutAccount> & logout_wait_from_gs)
{
	//typed::protocol::gmsg_online_players online_players;
	//online_players.set_gate_id(ManageConfig::instance()->getGateID());
	//for (ManageClientBeat::ClientSessionList::iterator it = online_client.begin(); it != online_client.end(); ++it)
	//{
	//	GATE_LOG_INFO(ACE_TEXT("login server restar, reoport online account, account id is <%d>\n"), (*it)->getAccountID());
	//	online_players.add_account_id((*it)->getAccountID());
	//	online_players.add_line_no((*it)->getLineNo());
	//}

	//for (std::list<LogoutAccount>::iterator it = logout_wait_from_gs.begin(); it != logout_wait_from_gs.end(); ++it)
	//{
	//	GATE_LOG_INFO(ACE_TEXT("login server restar, reoport logout wait from gs, account id is <%d>\n"), (*it).account_id);
	//	online_players.add_account_id((*it).account_id);
	//	online_players.add_line_no((*it).line_no);
	//}

	//PacketStream * ps = new PacketStream(GMSG_REPORT_ONLINE_PLAYERS, 0, online_players.SerializeAsString());
	//m_reoprt_online_user_packet = ps;
	//handleOutputPacket(ps);
}