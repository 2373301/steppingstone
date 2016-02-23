
#include "LoginServerSession.h"
#include "ManageLoginStream.h"
#include "protocol/msg_member.pb.h"

ManageLoginStream::ManageLoginStream()
: m_login_server_session(NULL)
, m_stop(false)
, m_wait(true)
{
}

ManageLoginStream::~ManageLoginStream()
{
}

int ManageLoginStream::svc()
{
	LoginServerSession * login_server_session = NULL;
	ACE_Time_Value sleep_time(0, 1000);

	PacketStreamQue_t packet_stream_que;

	while (!m_stop)
	{
		if (NULL != m_login_server_session)
		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_login_server_session_mutex, 1);
			if (NULL != login_server_session)
			{
				login_server_session->close();
				delete login_server_session;
			}

			login_server_session = m_login_server_session;
			reportOnlinePlayer(login_server_session);
			m_login_server_session = NULL;
		}

		{
			ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_stream_que_mutex, 1);
			while (m_packet_stream_que.size() > 0)
			{
				packet_stream_que.push(m_packet_stream_que.front());
				m_packet_stream_que.pop();
			}
		}

		if (NULL == login_server_session)
		{
			while (packet_stream_que.size() > 0)
			{
				delete packet_stream_que.front();
				packet_stream_que.pop();
			}
			ACE_OS::sleep(sleep_time);
			continue;
		}


		int read_result = processRead(login_server_session);

		int write_result = processWrite(login_server_session, packet_stream_que);

		if ((1 == read_result) && (1 == write_result))
		{
			ACE_OS::sleep(sleep_time);
		}
		else if ((2 == read_result) || (2 == write_result))
		{
			login_server_session->close();
			delete login_server_session;
			login_server_session = NULL;
		}
	}

	m_wait = false;
	return 0;
}

int ManageLoginStream::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate() == -1)
	{
		return -1;
	}

	return 0;
}

/// fini, stop
int ManageLoginStream::fini()
{
	m_stop = true;
	return 0;
}

/// wait for finish
int ManageLoginStream::wait()
{
	while (m_wait)
	{
		DEF_LOG_INFO("wait for ManageLoginStream\n");
		ACE_OS::sleep(1);
	}
	return 0;
}


void ManageLoginStream::handleLoginSession(LoginServerSession * login_server_session)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_login_server_session_mutex, );
	if (NULL != m_login_server_session)
	{
		m_login_server_session->close();
		delete m_login_server_session;
		m_login_server_session = NULL;
	}

	m_login_server_session = login_server_session;
}

void ManageLoginStream::handleOutputStream(PacketStream * packet_stream)
{
	ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_packet_stream_que_mutex, );
	m_packet_stream_que.push(packet_stream);
}

int ManageLoginStream::processRead(LoginServerSession * login_session)
{
	return login_session->rd_stream();
}

int ManageLoginStream::processWrite(LoginServerSession * login_session, PacketStreamQue_t & packet_stream_que)
{
	return login_session->processWrite(packet_stream_que);
}

int ManageLoginStream::reportOnlinePlayer(LoginServerSession * login_session)
{
	ManageClientBeat::ClientSessionList online_client_list;

	ManageClientBeat::instance()->getOnlineClient(online_client_list);

	typed::protocol::smsg_report_online_number sr_online_number;
	for (ManageClientBeat::ClientSessionList::iterator it = online_client_list.begin(); it != online_client_list.end(); ++it)
	{
		ClientSession * client_session = *it;
		::typed::protocol::smsg_report_online_number_OnlineInfo * online_info = sr_online_number.add_online_info();
		online_info->set_account(client_session->getAccountID());
		online_info->set_line_id(client_session->getLineNo());
	}

	PacketStream * ps = new PacketStream(SMSG_REPORT_ONLINE_NUMBER, 0, sr_online_number.SerializeAsString());
	PacketStreamQue_t ps_que;
	ps_que.push(ps);
	login_session->processWrite(ps_que);

	return 0;
}